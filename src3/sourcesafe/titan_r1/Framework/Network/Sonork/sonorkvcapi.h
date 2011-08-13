#if !defined(SONORKVCAPI_H)
#define SONORKVCAPI_H

#include "srk_data_types.h"
#include "srk_services.h"
#include "srk_client.h"
#include "srk_event_handler.h"

#define SONORKVCAPI_FUNC
struct TSonorkApi;
struct TSonorkApiFileSession;

enum SONORK_API_EVENT
{
	SONORK_API_EVENT_NONE
,	SONORK_API_EVENT_CLIENT
,	SONORK_API_EVENT_TASK
};
enum SONORK_API_FILE_EVENT
{
	SONORK_API_FILE_EVENT_NONE
,	SONORK_API_FILE_EVENT_PHASE_CHANGE
,	SONORK_API_FILE_EVENT_RECV_DATA
,	SONORK_API_FILE_EVENT_SEND_DATA
};
enum SONORK_API_FILE_PHASE
{
	SONORK_API_FILE_PHASE_ERROR=-1
,	SONORK_API_FILE_PHASE_IDLE
,	SONORK_API_FILE_PHASE_CONNECTING
,	SONORK_API_FILE_PHASE_AUTHORIZING
,	SONORK_API_FILE_PHASE_CONNECTED
,	SONORK_API_FILE_PHASE_REQUESTING
,	SONORK_API_FILE_PHASE_TRANSMITTING
,	SONORK_API_FILE_PHASE_WAIT_CONFIRM
,	SONORK_API_FILE_PHASE_COMPLETE
};
enum SONORK_API_FILE_TX_MODE
{
	SONORK_API_FILE_TX_NONE
,	SONORK_API_FILE_TX_SEND
,	SONORK_API_FILE_TX_RECV
,	SONORK_API_FILE_TX_DELE
};

struct TSonorkApiClientEvent
{
    SONORK_CLIENT_EVENT_TYPE	event;
	SONORK_DWORD2				objectId;
	TSonorkTime					objectTime;
	DWORD						eventData[4];
	TSonorkError				errorInfo;
};

struct TSonorkApiTaskEvent
{
	SONORK_CLIENT_REQUEST_EVENT_TYPE
							event;
	SONORK_FUNCTION			taskFunction;
	DWORD					taskSubFunction;
	SONORK_FUNCTION			packetFunction;
	DWORD					packetSubFunction;
	DWORD					taskId;
	DWORD					taskTag;
	TSonorkError			errorInfo;
	struct {
		TSonorkDataPacket*		ptr;
		UINT					size;
	}packet;	// <packet> is valid only if taskReqEvent==EVENT_PACKET
};

struct TSonorkApiUserSid
{
	TSonorkUserAddr			userAddr;	
	SONORK_DWORD2			serial;		
	TSonorkDynString		sidText;	
};


typedef void (fnSonorkApiHandler)(TSonorkApi*handle);
typedef void (fnSonorkApiFile)(TSonorkApiFileSession*handle);
typedef void (fnSonorkApiTracker)(const char*text);

struct TSonorkApiFileSession
{
friend struct TSonorkApi;
private:
	TSonorkPacketTcpEngine	tcp;
	TSonorkLocus1			senderLocus;
	TSonorkServiceLocus		serviceLocus;
	TSonorkFileLocus		fileLocus;
	SONORK_DWORD2			senderPin;
	SONORK_API_FILE_PHASE	prevPhase;
	SONORK_API_FILE_PHASE	txPhase;
	SONORK_API_FILE_TX_MODE	txMode;
	TSonorkError			txERR;
	SONORK_NETIO_STATUS		netStatus;
	DWORD					sessionTag;
	fnSonorkApiFile*		sessionProc;
	TSonorkCryptContext		cryptCtx;
	SONORK_API_FILE_EVENT	apiEvent;
	DWORD					maxRecvPacketsPerTimeSlot;
	DWORD					maxSendPacketsPerTimeSlot;
	DWORD					maxSendQueuedPackets;
	DWORD					refCount;
	bool					inCallback;
	struct
	{
		struct{
			void*	ptr;
			UINT	size;
		}block;
		UINT	offset,limit;
	}data;
	TSonorkApiFileSession(fnSonorkApiFile*sp,DWORD st);
	~TSonorkApiFileSession(){}

	void	
		InvokeEventHandler(SONORK_API_FILE_EVENT);

	void 
		SendLoginRequest();

	void
		ProcessTcpPacket(TSonorkTcpPacket *P);

	void
		RecvFileData(BYTE*dataPtr, UINT dataSize);

	void 
		RecvFileDataComplete();

	void
		SendFileData();

	void
		SetPhase(SONORK_API_FILE_PHASE, bool invokeHandler);

	void
		ProcessPacket(TSrkIpcPacket* ipcPacket,UINT ipcSize);

	void
		ProcessRequestingPacket(TSrkIpcPacket* ipcPacket,UINT ipcSize);

	void
		ProcessWaitConfirmPacket(TSrkIpcPacket* ipcPacket,UINT ipcSize);

public:

	void
		IncRefCount();

	void
		DecRefCount();
		
	DWORD	
		SessionTag() const
		{ return sessionTag;}

	SONORK_API_FILE_EVENT	
		ApiEvent() const
		{	return apiEvent;}

	SONORK_NETIO_STATUS		
		NetStatus() const
		{	return netStatus; }

	SONORK_API_FILE_PHASE
		TxPhase() const
		{ return txPhase; }

	SONORK_API_FILE_PHASE
		PrevTxPhase() const
		{ return prevPhase; }


	SONORK_API_FILE_TX_MODE	
		TxMode() const
		{ return txMode;}

	DWORD					
		GetMaxRecvPacketPerTimeSlot() const
		{ return maxRecvPacketsPerTimeSlot; }

	void
		SetMaxRecvPacketPerTimeSlot(DWORD v) 
		{ maxRecvPacketsPerTimeSlot=v; }

	DWORD					
		GetMaxSendPacketPerTimeSlot() const
		{ return maxSendPacketsPerTimeSlot; }

	void
		SetMaxSendPacketPerTimeSlot(DWORD v) 
		{ maxSendPacketsPerTimeSlot=v; }

	void 
		TimeSlot();

	const TSonorkError&
		GetLastResultInfo() const
		{ return txERR; }

	void
		GetLastResultInfo(TSonorkError &ERR) const
		{ ERR.Set(txERR); }

	SONORK_RESULT
		GetLastResult()
		{ return txERR.Result(); }

// ----------------
// NOTE: DataBlockPtr() and DataBlockSize() members are valid only
// while processing these events:
//  SONORK_API_FILE_EVENT_RECV_DATA 
//  SONORK_API_FILE_EVENT_SEND_DATA 

	void*
		 DataBlockPtr() const	
		{ return data.block.ptr; }

	UINT
		DataBlockSize() const	
		{ return data.block.size; }

	UINT	
		DataOffset() const
		{ return data.offset;}


	UINT	
		DataLimit() const
		{ return data.limit;}

	const TSonorkFileLocus&
		FileLocus() const
		{ return fileLocus;}

// ----------------
// Requests
	
	SONORK_RESULT
		RequestPutFile(const SONORK_DWORD2List *targetList
					, const TSonorkFileInfo*    fileInfo
					, SONORK_DWORD2*	trackingNo=NULL);

	SONORK_RESULT
		RequestGetFile(const TSonorkFileLocus*	fileLocus
					, BOOL				deleteFile);


	void
		Abort(const TSonorkError&ERR, bool invokeHandler);

	// Disconnect is the aame as Abort() with Result=OK
	void
		Disconnect(bool invokeHandler);
	
};

struct TSonorkApi
:public TSonorkEventHandler
{
private:
	void   	
			ProcessSonorkEvent(void*,TSonorkClientEvent*);

	void
			OnLoadLoginReq(TSonorkSvrLoginReqPacket*REQ);

	void
			OnGlobalTask(SONORK_FUNCTION function, const TSonorkError*pERR);

	void
			OnMsg(TSonorkMsg*msg);

	static void SONORK_CALLBACK 
			ProcessSonorkRequest(void *param,TSonorkClientRequestEvent*);

	void
			ProcessSonorkRequestPacket(TSonorkClientRequestEvent*E);
	
	void
			ProcessSonorkRequestEnd(TSonorkClientRequestEvent*E);

	DWORD
			StartRequest(TSonorkDataPacket*P
				, UINT pSize
				, SONORK_DWORD2& reqTag);

	void
			InvokeAppHandler(SONORK_API_EVENT event);
	

	struct TUSER
	{
		TSonorkUserData			data;
		TSonorkShortString		pwd;
		TSonorkSidFlags			sidFlags;
		TSonorkDynString		sidText;
	}user;

	struct TNETWORK
	{
		TSonorkHostInfo			sonork;
		TSonorkHostInfo			socks;
		SONORK_NETIO_STATUS		status;
		SONORK_NETIO_STATUS		prevStatus;
		SONORK_LOGIN_MODE		loginMode;
	}net;

	struct TAPP
	{
		bool				useUnicode;
		TSonorkVersion		version;
		TSonorkError		ERR;
		fnSonorkApiHandler*	handlerProc;
		DWORD				handlerTag;
		SONORK_API_EVENT	apiEvent;
		struct
		{
			TSonorkClientEvent*			app;
			TSonorkClientRequestEvent*	req;
		}event;

		union
		{
			void*						ptr;
			TSonorkUserDataNotes*		userDataNotes;
		}taskData;
	}app;


	TSonorkClient*				client;

public:
	TSonorkApi(fnSonorkApiHandler*,DWORD cbTag);
	~TSonorkApi();

	
	BOOL 
		TimeSlot();


	// -----------------------------------
	// Tag & event info

	DWORD
		HandlerTag() const
		{ return app.handlerTag;}


	// -----------------------------------
	// Error info

	SONORK_RESULT 
		GetLastResult() const
		{ return app.ERR.result; }

	SONORK_RESULT 
		GetLastResultInfo(TSonorkError*ERR);

	const TSonorkError*
		GetLastResultInfo() const
		{ return &app.ERR; }


	// -----------------------------------
	// Event processing

	SONORK_API_EVENT
		ApiEvent() const
		{ return app.apiEvent; }


	// -----------------------------------
	// Client event processing

	SONORK_RESULT 
		GetClientEvent(TSonorkApiClientEvent*apiEvent);

	TSonorkMsg*
 		GetEventMsg();		// Pointer is valid only while processing the event

	// Pointers are valid only while processing the event
	TSonorkUserData*
		GetEventUser(TSonorkAuth2**authPtr,TSonorkDynString**notesPtr=NULL);		

	SONORK_RESULT
		GetEventUser(TSonorkUserDataNotes*UDN);		

	TSonorkGroup*
		GetEventGroup();	// Pointer is valid only while processing the event

	SONORK_RESULT 
		GetEventSid(TSonorkApiUserSid*sid);




	// -----------------------------------
	// Task Event processing

	TSonorkClientRequestEvent*
		GetTaskEvent();		// Pointer is valid only while processing the event

	SONORK_RESULT 
		GetTaskEvent(TSonorkApiTaskEvent*taskEvent);

	TSonorkUserData*	
		GetTaskUserData();	// Pointer is valid only while processing the event
	
	TSonorkUserDataNotes*	
		GetTaskUserDataNotes();	// Pointer is valid only while processing the event



	// -----------------------------------
	// Connection

	SONORK_NETIO_STATUS
		GetNetworkStatus() const
		{ return net.status;}

	SONORK_RESULT 
		SetSonorkServer(
			 	SONORK_PHYS_ADDR_TYPE	addressType
			,	const char*			host
			,   DWORD				port
			,   SONORK_LOGIN_MODE	loginMode);

	SONORK_RESULT 
		Connect(const SONORK_DWORD2* userId, const char* password);

	void 
		Disconnect();

	// -----------------------------------
	// Profile Info

	TSonorkUserData&
		User()
		{ return user.data;}



	// -----------------------------------
	// Online mode

	SONORK_RESULT 
		SetSidMode(SONORK_SID_MODE sidMode, LPCSTR sidText="", BOOL*taskIsPending=NULL);

	static SONORK_SID_MODE 
		NormalizeSidMode(SONORK_SID_MODE sidMode);


	// -----------------------------------
	// User List/Info

	SONORK_RESULT 
		RefreshUserList();

	// -----------------------------------
	// Message sending

	SONORK_RESULT
		PrepareMsg(
			  TSonorkMsg*		msg
			, const char*		text
			, SONORK_SERVICE_ID	targetServiceId		=SONORK_SERVICE_ID_NONE
			, DWORD				targetServiceInstance=0
			, SONORK_SERVICE_ID	sourceServiceId		=SONORK_SERVICE_ID_NONE
			, DWORD				sourceServiceInstance=0
			, SONORK_SERVICE_ID	dataServiceId		=SONORK_SERVICE_ID_NONE
			, SONORK_SERVICE_TYPE dataServiceType	=SONORK_SERVICE_TYPE_NONE
			, const BYTE*		dataPtr				=NULL
			, DWORD				dataSize			=0
		);

	// returns task id
	DWORD 
		SendMsg(TSonorkLocus1*vbLocus,TSonorkMsg*vbMsg, DWORD taskTag=0);

	// returns task id
	DWORD 
		SendPlainTextMsg(TSonorkLocus1*vbLocus,const char *text,DWORD taskTag=0);

	
	// -----------------------------------
	// User Search

	// returns task id
	DWORD
		SearchUser(const TSonorkUserDataNotes* userDataNotes
				,	DWORD					searchFlags
				,	DWORD					maxCount=128
				,	DWORD					taskTag=0);

	// -----------------------------------
	// Authorized list handling 


	// returns task id
	DWORD
		AddUserAuth(const TSonorkId&
			,DWORD			groupNo
			,SONORK_C_CSTR	requestText
			,DWORD			taskTag);		

	// returns task id
	DWORD
		ReplyUserAuth(const TSonorkId&		
			, const TSonorkAuth2&
			, DWORD taskTag);	

	// returns task id
	DWORD
		DelUserAuth(const TSonorkId&
			, DWORD taskTag);	
			

	// -----------------------------------
	// File sessions

	// Note: CreateFileSession() takes ownership of <serviceData> which means
	// that the caller should NOT delete that pointer: The  pointer will automatically 
	// be freed when the returned session is destroyed.
	// This is applies even when the function fail: Once the function has been invoked
	// the caller should no longer reference <serviceData>

	TSonorkApiFileSession*
		CreateFileSession(fnSonorkApiFile*sessionProc,DWORD sessionTag);


	// -----------------------------------
	// UTS Address

	BOOL
		ResolveServiceAddress(
			  TSonorkPhysAddr&	targetAddr	// Resolved addr
			, TSonorkPhysAddr&	socksAddr	// if Socks must be Used
			, const TSonorkSidFlags& sidFlags
			, const TSonorkPhysAddr& serviceAddr
			, const TSonorkPhysAddr& hostAddr
			, bool  connectingToServerService
			);

};


#endif