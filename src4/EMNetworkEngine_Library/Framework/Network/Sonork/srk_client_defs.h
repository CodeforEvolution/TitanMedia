#if !defined(SRK_CLIENT_DEFS_H)
#define SRK_CLIENT_DEFS_H

/*
	Sonork Messaging System

	Portions Copyright (C) 2001 Sonork SRL:

	This program is free software; you can redistribute it and/or modify
	it under the terms of the Sonork Source Code License (SSCL)
	which is based on the GNU General Public License (GPL).

	You should have received a copy of the SSCL	along with this program;
	if not, write to sscl@sonork.com.

	You may NOT use this source code before reading and accepting the
	Sonork Source Code License (SSCL).

	This comment section, indicating the existence and requirement of
	acceptance of the SSCL may not be removed from the source code.
*/

#include "srk_netio.h"
#include "srk_data_packet.h"

#define SONORK_CLIENT_F_SERVER_DISCONNECT	0x004000
#define SONORK_CLIENT_FM_INTERNAL           	0x00ffff
#define SONORK_CLIENT_F_NO_CALLBACK     	0x080000
#define SONORK_CLIENT_F_USE_SOCKS		0x100000
#define SONORK_CLIENT_MAX_PENDING_TASKS		2048

#if !defined(SONORK_CLIENT_MAX_ACTIVE_TASKS)
#	define SONORK_CLIENT_MAX_ACTIVE_TASKS	2
#endif

#define SONORK_CLIENT_DEFAULT_MAX_IDLE_SECS		120
#define SONORK_CLIENT_DEFAULT_MONITOR_CHECK_SECS	5


// we set the flag on the Guts connections
// initiated because of reverse requests
#define GUTS_LINK_F_REMOTE_REQUEST		GUTS_LINK_F_USER_1


#define SRK_DEFAULT_AKN_MSECS   		4000
#define SRK_DEFAULT_PCK_MSECS   		(SRK_DEFAULT_AKN_MSECS*4+SRK_DEFAULT_AKN_MSECS/2)


/// SONORK_CLIENT_CGK_xxx flags are used for TSonorkClient::ConnectGutsKernel()

#define	SONORK_CLIENT_CGK_REMOTE_REQUEST	0x00001
#define SONORK_CLIENT_CGK_USE_CRYPT		0x00002
#define SONORK_CLIENT_CGK_NO_CRYPT		0x00004
#define SONORK_CLIENT_CGK_USE_REVERSE		0x00010
#define SONORK_CLIENT_CGK_NO_USE_REVERSE	0x00020
// Note:
// REMOTE_REQUEST is used to indicate the the connection
// is being initiated because the remote side requested
// implies NO_USE_REVERSE and ignores USE_REVERSE and CRYPT flags

enum SONORK_CLIENT_REQUEST_STATUS
{
	SONORK_CLIENT_REQUEST_STATUS_INVALID
,	SONORK_CLIENT_REQUEST_STATUS_PENDING
,	SONORK_CLIENT_REQUEST_STATUS_TRANSMITTING
};
enum SONORK_CLIENT_EVENT_TYPE
{
	SONORK_CLIENT_EVENT_NONE		// Place holder
,	SONORK_CLIENT_EVENT_STATUS_CHANGE	// On SetStatus()
,	SONORK_CLIENT_EVENT_GLOBAL_TASK
,	SONORK_CLIENT_EVENT_MONITOR
,	SONORK_CLIENT_EVENT_MSG			// On SelfRequest(GET_MSG) or GUTS_EVENT_REMOTE_EXEC(PUT_MSG)
,	SONORK_CLIENT_EVENT_CTRL_MSG
,	SONORK_CLIENT_EVENT_USER_AUTH
,	SONORK_CLIENT_EVENT_USER_ADD
,	SONORK_CLIENT_EVENT_USER_SET		// On SelfRequest(GET_USER_DATA)
,	SONORK_CLIENT_EVENT_USER_SID		// On SID event
,	SONORK_CLIENT_EVENT_USER_DEL
,	SONORK_CLIENT_EVENT_GROUP_ADD
,	SONORK_CLIENT_EVENT_USER_SYNCH_END
,	SONORK_CLIENT_EVENT_WAPP_ADD
,	SONORK_CLIENT_EVENT_LOAD_LOGIN_REQ
,	SONORK_CLIENT_EVENT_SYS_MSG

};
enum SONORK_CLIENT_REQUEST_EVENT_TYPE
{
	SONORK_CLIENT_REQUEST_EVENT_NONE
,	SONORK_CLIENT_REQUEST_EVENT_PACKET	// On ConsumeRequestPacket()
,	SONORK_CLIENT_REQUEST_EVENT_END		// On RequestEnd()
};

enum SONORK_CLIENT_STATUS_CHANGE_FLAGS
{
	SONORK_CLIENT_STATUS_CHANGE_UNEXPECTED	= 0x00001
,   SONORK_CLIENT_STATUS_CHANGE_CX_KILLED	= 0x00002
,	SONORK_CLIENT_STATUS_CHANGE_CX_DENIED	= 0x00004
,	SONORK_CLIENT_STATUS_CHANGE_CX_LOST		= 0x00008	// Was connected
,	SONORK_CLIENT_STATUS_CHANGE_BY_TIMEOUT	= 0x00010
,	SONORK_CLIENT_STATUS_CHANGE_BAD_PROTOCOL= 0x00020
};




class  TSonorkClient;
struct TSonorkClientEvent;
struct TSonorkClientRequestEvent;
struct TSonorkDataPacket;

struct TSonorkClientEvent
{
friend TSonorkClient;
	SONORK_CLIENT_EVENT_TYPE	type;
	union {

		struct {
			TSonorkMsg		*ptr;
		}msg;

		struct {
			TSonorkLocusAtom*		locus;
			TSonorkDynString*	text;
			TSonorkSerial		userSerial;
		}sid;

		TSonorkAuthReqData*		auth_req;

		struct {
			DWORD				flags;
			TSonorkDynString*		notes;
			TSonorkAuth2*			auth;
		}userAddSet;

		struct {
			TSonorkGroup*		ptr;
		}group;

		TSonorkWappData			*wapp;

		struct {
			SONORK_NETIO_STATUS	status;
			DWORD			flags;
		}status;

		struct {
			BOOL			idle;
		}monitor;

		struct {
			SONORK_FUNCTION		function;
		}task;

		struct {
			TSonorkLocus1*	sender;
			TSonorkCtrlMsg*		msg;
			TSonorkDynData*		data;
			DWORD			flags;
		}ctrl;

		struct {
			DWORD			index;
			TSonorkSysMsg*		ptr;
		}sys_msg;

		struct {
			struct TSonorkSvrLoginReqPacket*packet;
		}login_req;

	}data;

	TSonorkId			user_id;
	TSonorkError	*		pERR;
	TSonorkUserData	*		user_data;
public:

// EventType,HandlerParam
//  All events
	SONORK_CLIENT_EVENT_TYPE
		EventType()	const
		{ return type;}


// Status
//	STATUS_CHANGE
	SONORK_NETIO_STATUS
		Status()	const
		{ return data.status.status;}
	DWORD
		StatusFlags()	const
		{ return data.status.flags;}


// ErrorInfo
//	STATUS_CHANGE
//	USER_GUTS
//  SET_BUSY

	TSonorkError*
		ErrorInfo()
		{ return pERR;}

// Msg()
// 	MSG
	TSonorkMsg*
		Msg()
		{ return data.msg.ptr;}


//  USER_SID
//  USER_ADD
//  USER_SET
//  USER_DEL
//  (not valid for USER_AUTH)
const	TSonorkId&
		UserId() const
		{ return user_id;}

	TSonorkUserData*
		UserData()
		{ return user_data;}

// USER_AUTH only
	TSonorkAuthReqData*
		UserAuthReq()
		{ return data.auth_req;}


// USER_ADD/USER_SET only
	TSonorkDynString*
		UserAddSetNotes() const
		{ return data.userAddSet.notes;}

	TSonorkAuth2*
		UserAddSetAuth() const
		{ return data.userAddSet.auth;}

	DWORD
		UserAddSetFlags()	const
		{ return data.userAddSet.flags;}

//  SidLocus(),SidUserSerial(),Version(),SidText()
// 	USER_SID

	TSonorkLocusAtom*
		SidLocus()
			{ return data.sid.locus;}

	TSonorkSerial*
		SidUserSerial()
			{ return &data.sid.userSerial;}

	TSonorkDynString*
		SidText()
			{ return data.sid.text;}

// For these events UserGuId() is loaded with the user's GuId
// but UserData() may be NULL if TSonorkClient does not find the user's data
//  on the local user list:
//  This can happen when the server's user list does not match the client's
//  user list or the remote user just does not belong to out user list

// TaskFunction()
//  TASK
//  We distinguish the task START from the task END event
//  because ErrorInfo() is NULL when task is starting.
	SONORK_FUNCTION
		TaskFunction()	const
			{ return data.task.function; }


// SysMsgTime(),SysMsgStr()
//  SYS_MSG

	DWORD
		SysMsgIndex()	const
		{ return data.sys_msg.index; }

	TSonorkSysMsg*
		SysMsg() const
		{ return data.sys_msg.ptr; }

//  CtrlLocus(),CtrlMsg(),CtrlData(),CtrlFlags()
//	CTRL_MSG
	TSonorkLocus1*
		CtrlMsgSender()
		{ return data.ctrl.sender;}

	TSonorkCtrlMsg*
		CtrlMsg()
		{ return data.ctrl.msg;}

	DWORD
		CtrlMsgFlags()	const
		{ return data.ctrl.flags;}

	TSonorkDynData*
		CtrlMsgData()
		{ return data.ctrl.data;}

//	SONORK_CLIENT_EVENT_MONITOR
	BOOL
		MonitorIdle() const
			{ return data.monitor.idle;	}


//	SONORK_CLIENT_EVENT_GROUP_ADD
	TSonorkGroup*
		Group()		const
			{ return data.group.ptr; }

//	SONORK_CLIENT_EVENT_LOAD_LOGIN_REQ
	TSonorkSvrLoginReqPacket*
		LoginReqPacket()
			{ return data.login_req.packet;	}

const TSonorkWappData*
		WappData()		const
			{ return data.wapp;}
};

typedef void (SONORK_CALLBACK fnSonorkRequestHandler)	(void *param,TSonorkClientRequestEvent*);
typedef 	fnSonorkRequestHandler* lpfnSonorkRequestHandler;





struct TSonorkClientRequest
{

private:
	DWORD				id;
	SONORK_FUNCTION		function;
	DWORD				subFunction;
	SONORK_DWORD2		tag;
	lpfnSonorkRequestHandler	cbProc;
	void*				cbParam;


	struct {
struct  TSonorkUdpPacket	*udp;
struct	TSonorkTcpPacket	*tcp;
	}pck;
public:
	TSonorkClock		idleClock;
	
	TSonorkClientRequest(	
		 DWORD						id
		,SONORK_FUNCTION			function
		,DWORD						subFunction
		,lpfnSonorkRequestHandler	h_callback
		,void*		 				h_param
		,const SONORK_DWORD2*		h_tag);
	~TSonorkClientRequest();

	void SetNetPacket(TSonorkTcpPacket *);
	void SetNetPacket(TSonorkUdpPacket *);

	TSonorkUdpPacket *ReleaseUdpPacket();
	TSonorkTcpPacket *ReleaseTcpPacket();

	lpfnSonorkRequestHandler 
			CallbackProc() 
			{ return cbProc;}
	void*	
			CallbackParam()		
			{ return cbParam;}

	DWORD	
			Id()	const 		
			{ return id;}
		
	SONORK_FUNCTION			
			Function()	const 	
			{ return function;}

	DWORD
			SubFunction()	const 	
			{ return subFunction;}

	void	
			Cancel()			
			{ cbProc = NULL;}

	bool	
			Canceled()	const 	
			{ return cbProc==NULL;}

	SONORK_DWORD2&			
			Tag()				
			{ return tag;}

	void
			InvokeCallback(TSonorkClientRequestEvent* eventPtr)
			{	(*cbProc)(cbParam,eventPtr); }
};

struct TSonorkClientRequestEvent
{
friend class TSonorkClient;
	DWORD			type;
	TSonorkClientRequest*	
					request;

	DWORD 			packet_size;

	TSonorkDataPacket*	packet;
	TSonorkError	 *	pERR;

	TSonorkClientRequestEvent(SONORK_CLIENT_REQUEST_EVENT_TYPE T)
	{	type=T; }

public:

// EventType
// RequestTaskId,RequestTaskId,RequestTag
//  < ALL EVENTS >
	SONORK_CLIENT_REQUEST_EVENT_TYPE
		EventType()		const
		{ return (SONORK_CLIENT_REQUEST_EVENT_TYPE)type;}

	DWORD
		RequestId()	const
		{ return request->Id();}

	SONORK_FUNCTION
		RequestFunction()	const
		{ return request->Function();}

	DWORD
		RequestSubFunction()	const
		{ return request->SubFunction();}

	SONORK_DWORD2&
		RequestTag()
		{ return request->Tag();}


// ErrorInfo
//  REQUEST_EVENT_END
	TSonorkError*
		ErrorInfo()
		{ return pERR;}

// Packet,PacketSize,ErrorInfo
//  REQUEST_EVENT_PACKET
	TSonorkDataPacket
		*Packet()
		{ return packet; }

	DWORD	
		PacketSize() const
		{ return packet_size;}

	SONORK_FUNCTION			
		PacketFunction()	const 	
			{ return packet->Function();}

	DWORD
		PacketSubFunction()	const 	
			{ return packet->SubFunction();}

};



/*
struct TSonorkPendingGutsCx
{

	TSonorkLocus2	locus;
	TSonorkClock		clock;
};


StartSonorkQueueClass(TSonorkPendingGutsCxQueue,TSonorkPendingGutsCx)
public:
	TSonorkPendingGutsCx* Get(TSonorkId&);
EndSonorkQueueClass;

*/

#endif
