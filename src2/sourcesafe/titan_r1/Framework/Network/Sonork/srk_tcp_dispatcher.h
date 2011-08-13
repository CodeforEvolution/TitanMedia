#if !defined(SONORK_TCP_DISPATCHER_H)
#define SONORK_TCP_DISPATCHER_H

#include "srk_defs.h"

#include "srk_thread.h"
#include "srk_tcpio.h"



enum SONORK_TCPD_EVENT
{
	SONORK_TCPD_EVENT_NONE
,	SONORK_TCPD_EVENT_WRITE
,	SONORK_TCPD_EVENT_CLOSE
};
enum SONORK_TCPD_DATA_RESULT
{
	SONORK_TCPD_DATA_INVALID_PACKET	= -1
,	SONORK_TCPD_DATA_DELETE_PACKET	= 0
,	SONORK_TCPD_DATA_OK		= 1
};
enum SONORK_TCPD_EVENT_RESULT
{
	SONORK_TCPD_EVENT_NOT_PROCESSED	= 0
,	SONORK_TCPD_EVENT_PROCESSED	= 1
};


// The Data handler should return:
//  SONORK_TCPD_DATA_OK				 : Don't delete tcp packet, handler will delete
//  SONORK_TCPD_DATA_DELETE_PACKET	 : Delete tcp packet
//  SONORK_TCPD_DATA_INVALID_PACKET	 : Delete tcp packet and kill connection
// The event handler should return:
//  SONORK_TCPD_ECB_PROCCESSED		:	Event processed
//  SONORK_TCPD_ECB_NOT_PROCESSED	:	Event not processed/unknown
//  Any other values are invalid and could cause problems in future versions

typedef SONORK_TCPD_DATA_RESULT	(SONORK_CALLBACK fnTcpDispatcherDataCB)(void *param,SONORK_SID_ID,SOCKET,void*data);
typedef fnTcpDispatcherDataCB*
			pfnTcpDispatcherDataCB;

typedef SONORK_TCPD_EVENT_RESULT(SONORK_CALLBACK fnTcpDispatcherEventCB)(void *param,SONORK_TCPD_EVENT,SONORK_SID_ID,SOCKET,void *event_data);
typedef fnTcpDispatcherEventCB*
			pfnTcpDispatcherEventCB;

struct TSonorkTcpDispatcherEntry
{
friend class TSonorkTcpDispatcher;
private:
	SOCKET 		sk;
	SONORK_SID_ID   sid_id;
	UINT		flags;
public:
	SOCKET
		Sk()	const
		{ return sk; }

	SONORK_SID_ID
		SidId()	const
		{ return sid_id;}

	UINT
		Flags()	const
		{ return flags;}

	virtual UINT
		PendingOutputPackets() const =0;
};
struct TSonorkPacketTcpDispatcherEntry
:public TSonorkTcpDispatcherEntry
{
	UINT			o_offset;
	TSonorkTcpPacket*	o_packet;
	TSonorkTcpPacketQueue	o_queue;
	UINT		     	i_offset;
	TSonorkTcpPacket	i_header;
	TSonorkTcpPacket*	i_packet;

	UINT
		PendingOutputPackets() const
		{ return o_queue.Items(); }
};

struct TSonorkRawTcpDispatcherEntry
:public TSonorkTcpDispatcherEntry
{
	UINT				o_offset;
	TSonorkRawTcpPacket*		o_packet;
	TSonorkRawTcpPacketQueue	o_queue;

	UINT
		PendingOutputPackets() const
		{ return o_queue.Items(); }
};

enum TCP_DISPATCHER_WRITE_RESULT
{
	TCP_DISPATCHER_WRITE_OK
,	TCP_DISPATCHER_WRITE_ERROR
,	TCP_DISPATCHER_WRITE_BLOCKED
};
enum TCP_DISPATCHER_READ_RESULT
{
	TCP_DISPATCHER_READ_OK
,	TCP_DISPATCHER_READ_ERROR
};

class TSonorkTcpDispatcher
:public TSonorkThread
{
private:
	struct XLAT
	{
		TSonorkTcpDispatcherEntry	**table;
		UINT				 max_entries
						,entries;
		TSonorkLock			mutex;
	}xlat;

	struct {
		bool	dirty;
		UINT 	select_msecs;
		fd_set	*active_r_set;
		fd_set	*active_w_set;
		fd_set	*active_e_set;
		fd_set	*select_r_set;
		fd_set	*select_w_set;
		fd_set	*select_e_set;
		char	recv_buffer[SONORK_TCPIO_DEFAULT_BUFFER_SIZE];
		DWORD	low_mark,high_mark;
	}fd;

	struct {
		void*				param;
		pfnTcpDispatcherDataCB		data;
		pfnTcpDispatcherEventCB		event;
		bool				in_send_function;
		bool				in_callback;
	}CB;

private:
	void	EV_Write(TSonorkTcpDispatcherEntry*E);
	void    EV_Read(TSonorkTcpDispatcherEntry*E);
	SONORK_SID_ID
		_DelConnection(SOCKET sk, bool advise_server);

	virtual TCP_DISPATCHER_WRITE_RESULT
			OnWrite(TSonorkTcpDispatcherEntry*E)=0;

	virtual TCP_DISPATCHER_READ_RESULT
			OnRead(TSonorkTcpDispatcherEntry*E,const BYTE*,DWORD size)=0;

	virtual TSonorkTcpDispatcherEntry*
			OnAddConnection()=0;

	virtual void
			OnDelConnection(TSonorkTcpDispatcherEntry*E)=0;


	int
		Run();

	void
		SortXlatTable();

	UINT
		GetXlatEntryNo(SOCKET sk);

	void
		MarkFdSetDirty()
		{	fd.dirty=true;	}
		
	void
		RebuildFdSet();

protected:

	SONORK_TCPD_EVENT_RESULT
		InvokeEventHandler(SONORK_TCPD_EVENT,SONORK_SID_ID,SOCKET,void *event_data);

	SONORK_TCPD_DATA_RESULT
		InvokeDataHandler(TSonorkTcpDispatcherEntry*,void*data);
	void
		LockXlatTable(const char*file,UINT line)	{ xlat.mutex.Lock(file,line);}

	TSonorkTcpDispatcherEntry*
		GetXlatEntry(SOCKET sk);
		
	void
		UnlockXlatTable()
		{ xlat.mutex.Unlock();}

	void
		Flush(TSonorkTcpDispatcherEntry*);
protected:
	TSonorkTcpDispatcher(void*cb_param
				, pfnTcpDispatcherDataCB	cb_data
				, pfnTcpDispatcherEventCB	cb_event
				, UINT				select_msecs
				, UINT				max_connections
						);

public:						
	bool
		AddConnection(SOCKET sk,SONORK_SID_ID);
		
	SONORK_SID_ID
		DelConnection(SOCKET sk);

	BOOL	GetSidSocket(SONORK_SID_ID, SOCKET* p_sk);

	void
		SetHighMark(DWORD n)
		{ fd.high_mark=n;}

	void
		SetLowMark(DWORD n)
		{ fd.low_mark=n;}
};

class TSonorkPacketTcpDispatcher
:public TSonorkTcpDispatcher
{

	TSonorkTcpDispatcherEntry*OnAddConnection();

	TCP_DISPATCHER_WRITE_RESULT
				OnWrite(TSonorkTcpDispatcherEntry*E);
	TCP_DISPATCHER_READ_RESULT
				OnRead(TSonorkTcpDispatcherEntry*E,const BYTE*,DWORD size);

	void
				OnDelConnection(TSonorkTcpDispatcherEntry*E);

public:
	TSonorkPacketTcpDispatcher(void*		cb_param
				, pfnTcpDispatcherDataCB	cb_data
				, pfnTcpDispatcherEventCB	cb_event
				, UINT				select_msecs
				, UINT				max_connections
						);

	BOOL	SendWithBlocked(SOCKET sk
				,SONORK_NETIO_PACKET_HFLAG
				,const void *buffer
				,DWORD size
				,BOOL&	blocked
				,SONORK_QUEUE_ITEM_PRIORITY );	// waiting because socket is blocked

	BOOL	SendWithQueueLength(SOCKET sk
				,SONORK_NETIO_PACKET_HFLAG
				,const void *buffer
				,DWORD size
				,DWORD&	queue_length
				,SONORK_QUEUE_ITEM_PRIORITY p=SONORK_QUEUE_PRIORITY_DEFAULT);

};

class TSonorkRawTcpDispatcher
:public TSonorkTcpDispatcher
{

	TSonorkTcpDispatcherEntry*OnAddConnection();

	TCP_DISPATCHER_WRITE_RESULT
				OnWrite(TSonorkTcpDispatcherEntry*E);
	TCP_DISPATCHER_READ_RESULT
				OnRead(TSonorkTcpDispatcherEntry*E,const BYTE*,DWORD size);

	void
				OnDelConnection(TSonorkTcpDispatcherEntry*E);

public:
	TSonorkRawTcpDispatcher(void*			cb_param
			, pfnTcpDispatcherDataCB	cb_data
			, pfnTcpDispatcherEventCB	cb_event
			, UINT				select_msecs
			, UINT				max_connections
						);
	// <queued_packets> returns number of packet currently
	// waiting because socket is blocked

	BOOL	TCP_Send(SOCKET sk
				,const void *buffer
				,DWORD size
				,BOOL&	blocked
				,SONORK_QUEUE_ITEM_PRIORITY p=SONORK_QUEUE_PRIORITY_DEFAULT);

	BOOL	TCP_Send(SOCKET sk
				,SONORK_C_CSTR string
				,BOOL&	blocked
				,SONORK_QUEUE_ITEM_PRIORITY p=SONORK_QUEUE_PRIORITY_DEFAULT);

};

#endif