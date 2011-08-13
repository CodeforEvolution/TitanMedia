#if !defined(SRK_IPC_H)
#define SRK_IPC_H
#include "srk_ipc_defs.h"
#include "srkwin32app_defs.h"
#include "srk_data_types.h"
#include "srk_ext_user_data.h"

/*
	Sonork Messaging System

	Portions Copyright (C) 2001 Sonork SRL:

	This program is free software; you can redistribute it and/or modify
	it under the terms of the Sonork Source Code License (SSCL) Version 1.

	You should have received a copy of the SSCL along with this program;
	if not, write to sscl@sonork.com.

	You should NOT use this source code before reading and accepting the
	Sonork Source Code License (SSCL), doing so will indicate your agreement
	to the the terms which may be differ for each version of the software.

	This comment section, indicating the existence and requirement of
	acceptance of the SSCL may not be removed from the source code.
*/


enum SONORK_IPC_CTRL_FLAGS
{
  SONORK_ICF_REQ_LOCK		= 0x00000001
, SONORK_ICF_EVT_LOCK		= 0x00000002
, SONORK_ICF_IN_CALLBACK	= 0x00000010
, SONORK_ICF_STOPPING		= 0x10000000
};
enum SONORK_IPC_LOCK
{
  SONORK_IPC_LOCK_REQ		= SONORK_ICF_REQ_LOCK
, SONORK_IPC_LOCK_EVT		= SONORK_ICF_EVT_LOCK
};

// ----------------------------------------------------------------------------
// Events reported by the IPC
// See <TSonorkIpcEvent> (below) for more info

enum SONORK_IPC_EVENT
{
  SONORK_IPC_EVENT_NONE
, SONORK_IPC_EVENT_APP_EVENT		= 1
, SONORK_IPC_EVENT_SERVICE_EVENT	= 2
};
struct SONORK_IPC_SLOT_HANDLE_STRUCT
{
  DWORD	value;
};
typedef SONORK_IPC_SLOT_HANDLE_STRUCT* SONORK_IPC_SLOT_HANDLE;

// ----------------------------------------------------------------------------
// Structures & Unions that help to parse the Ipc events data

// Extended data for SONORK_IPC_EVENT_APP_EVENT
union TSonorkIpcAppEvent
{
	// sidChanged
	//  Extended data for SONORK_APP_EVENT_SID_CHANGED
	//  Occurs when Sonork changes online status. (at work/friendly,etc)
	//  <sidFlags> are the session flags which include mode,
	//   visibility filters, support info, etc. (see TSonorkSidFlags)
	//  <userAddr> is the full address as kept by the engine.
	struct {
			TSonorkSidFlags		sidFlags;
		const TSonorkNewUserAddr*	userAddr;

	}sidChanged;


	// <setUser>
	//  Extended data for SONORK_APP_EVENT_SET_USER
	//  indicates that a user's attributes have changed. (other than
	//   its online status which is reported as SONORK_APP_EVENT_USER_SID)
	//  <userId> is the SONORK_DWORD2 id of the user
	//  <flags> are the attributes that have changed
	//    and are one or more of the SONORK_APP_EVENT_SET_USER_FLAGS
	//    defined in <sonorkwin32app_defs.h>
	struct {
		TSonorkId		userId;
		DWORD			flags;
	}setUser;

	// <delUser>
	//  Extended data for SONORK_APP_EVENT_DEL_USER
	//  A user is being removed from the user list, after call returns
	//  Sonork will discard cached copy of the user
	struct {
		TSonorkId		userId;
	}delUser;


	// <userSid>
	//  Extended data for SONORK_APP_EVENT_USER_SID
	//  indicates that a user has changed its online status
	//  if type==SONORK_APP_EVENT_USER_SID_LOCAL
	// 	it means that Sonork has a cached copy of the user's data,
	//	use the <local> member
	//  if type==SONORK_APP_EVENT_USER_SID_REMOTE
	//	it means that Sonork has no cache copy of the user's data,
	//	use the <remote> member
	//  if type is none of above, ignore this event.
	// For type LOCAL only:
	//  onlineDir:
	//   1 = user connected
	//   0 = user has changed status without cting/dcting
	//  -1 = user has disconnected
	//  oldSidMode: Previous sid mode
	struct {
		DWORD			type;
		TSonorkId		userId;
		union
		{
			struct	// to use when type==EVENT_USER_SID_LOCAL
			{
				TSonorkNewUserAddr	userAddr;
				DWORD			oldSidMode;
				int			onlineDir;
			}local;

			struct  // to use when type==EVENT_USER_SID_REMOTE
			{
				TSonorkSid		sid;
				TSonorkSidFlags		sidFlags;
			}remote;
		};
	}userSid;

	// <userUts>
	//  Extended data for SONORK_APP_EVENT_USER_UTS
	//  indicates that a peer 2 peer connection has been established/closed
	//  <userId> is the user on the other side.
	//  if <utsLinkId> is SONORK_INVALID_LINK_ID, the link has been closed
	//  otherwise there exists a direct connection with the user.

	struct {
		TSonorkId	userId;
		DWORD		utsLinkId;
	}userUts;


	// <setLanguage>
	//  Extended data for SONORK_APP_EVENT_SET_LANGUAGE
	//  The user has changed the interface's language
	struct {
		DWORD			code;
	}setLanguage;


	// <flushBuffers>
	//  Extended data for:
	//  SONORK_APP_EVENT_FLUSH_BUFFERS
	//   <msecs> is the milliseconds since last time this event was invoked
	struct {
		DWORD			msecs;
	}flushBuffers;

	// <msg>
	//  Extended data for:
	//  SONORK_APP_EVENT_SONORK_MSG_RCVD
	//  SONORK_APP_EVENT_SONORK_MSG_SENDING
	//  SONORK_APP_EVENT_SONORK_MSG_SENT
	//  Event is generated during message reception/sending
	//  <msg.header> is valid only if
	//   the event is MSG_RCVD or MSG_SENDING, undefined otherwise
	//  <msg.result> is valid only if
	//   the event is MSG_SENT, undefined otherwise
	//  <msg.mark> is valid only if <pc_flags> has the
	//   
	
	struct
	{
		DWORD			pcFlags;
		DWORD			ccFlags;
		DWORD			handleId;
		DWORD			taskId;
		DWORD			result;
		DWORD			reserved[4];
const		TSonorkCCacheMark*	mark;
const		TSonorkMsgHeader*	header;
	}msg;

	struct
	{
                TSonorkId		userId;
		DWORD			pcFlags;
		DWORD			ccFlags;
		DWORD			extIndexIsValid;
		DWORD			extIndex;
const		TSonorkCCacheMark*	mark;
	}msgProcessed;

};

// Extended data for SONORK_IPC_EVENT_SERVICE_EVENT
struct TSonorkIpcSvcEvent
{
	DWORD			handlerTag;
	DWORD			serviceInstance;
	const SONORK_DWORD2*	eventTag;
	union
	{
		struct
		{
			DWORD	value;
		}setBusy;

		struct
		{
			DWORD	value;
		}setExport;

		struct
		{
			TSonorkShortString*	str;
		}getName;

		struct
		{
			TSonorkServiceHandle*	serviceHandle;
			TSonorkCtrlMsg*		ctrlMsg;
			DWORD			dataSize;
		}msgInfo;

		struct
		{
			DWORD		systemId;
			SONORK_RESULT   result;
		}close;
	};
};
// ----------------------------------------------------------------------------
// The one union that englobes all events

union TSonorkIpcEvent
{
	// <appEvent>
	//  Extended data for SONORK_IPC_EVENT_APP_EVENT
	//  Mapped from the sonork application internal events.
	//  The application event is passed as <eventParam> to the
	//  callback and is one the the  SONORK_APP_EVENT values
	//  defined in <sonorkwin32app_defs.h>
	//  Not all application events have exetended data.
	TSonorkIpcAppEvent	appEvent;
	
	// <svcEvent>
	//  Extended data for SONORK_IPC_EVENT_SERVICE_EVENT
	//  Mapped from the sonork application service events.
	//  The service event is passed as <eventParam> to the
	//  callback and is one the the  SONORK_APP_SERVICE_EVENT_ID values
	//  defined in <sonorkwin32app_defs.h>
	//  Not all service events have exetended data.
	TSonorkIpcSvcEvent	svcEvent;

};
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// The main IPC callback provided by the host application

typedef DWORD (SONORK_CALLBACK TSonorkIpcMainCallback)(
		 void*				callbackParam
		,SONORK_IPC_EVENT		eventId
		,LPARAM				eventParam
		,TSonorkIpcEvent*		eventData);
typedef TSonorkIpcMainCallback* TSonorkIpcMainCallbackPtr;

// ----------------------------------------------------------------------------
// The IPC callback user for retrieving lists and log data

typedef DWORD (SONORK_CALLBACK TSonorkIpcDataCallback)(
		 void*				param
		,void*				data);
typedef TSonorkIpcDataCallback* TSonorkIpcDataCallbackPtr;



class TSonorkIpc
{
	struct
	{
		HWND			ownerHwnd;
		DWORD			ipcId;
		DWORD			ipcFlags;
		DWORD			flags;
		DWORD			result;
		SONORK_SERVICE_ID	serviceId;
	}ctrl;


	struct
	{
		TSonorkIpcMainCallback*	ptr;
		void*			param;
		SONORK_IPC_EVENT	eventId;
		DWORD			eventParam;
		DWORD			eventDataSize;
		TSonorkIpcEvent		event;
	}cb;


	struct
	{
		DWORD			blockDataSize;
		DWORD			blockFullSize;
		struct
		{
			HANDLE			mutex;
			HANDLE			handle;
			HWND			hwnd;
			DWORD			msg;
			TSonorkIpcFileHeader*	header;
			TSonorkIpcFileBlock*	block;
		}server;

		struct
		{
			HANDLE			handle;
			HWND			hwnd;
			TSonorkIpcFileBlock*	reqBlock;
			TSonorkIpcFileBlock*	evtBlock;
		}client;

		struct
		{
			SONORK_IPC_FUNCTION	function;
			TSonorkIpcDataCallback*	callbackPtr;
			void*			callbackData;
			BYTE*			bufferPtr;
			DWORD			bufferSize;
		}reqData;

		struct
		{
			DWORD			loadedDataSize;
			BYTE*			bufferPtr;
			DWORD			bufferSize;
		}evtData;

		struct
		{
			SONORK_IPC_SLOT_HANDLE	handle;
			TSonorkIpcSharedSlot*	ptr;
		}slot;
	}dta;


	BOOL
		SendIpcMsg(SONORK_IPC_FUNCTION 	function);
		
	DWORD
		InvokeCallback();
// ----------------------------------------------------------------------------
// Mutex control : only one application may use the IPC data transfer area (DTA)
//  at the same time.

	BOOL
		Lock(SONORK_IPC_LOCK lock);

	void
		Unlock(SONORK_IPC_LOCK lock);

	BOOL
		LockServer();

	void
		UnlockServer();

// ----------------------------------------------------------------------------
// Invisible window's procedure

	static LRESULT CALLBACK
		WinProc( HWND hwnd , UINT uMsg , WPARAM wParam , LPARAM lParam);

	LRESULT
		OnAppEvent( SONORK_APP_EVENT );

	LRESULT
		OnServiceEvent(SONORK_APP_SERVICE_EVENT size);

	LRESULT
		OnPutReqData(DWORD size);

	LRESULT
		OnPutEvtData(DWORD size);

	LRESULT
		OnGetReqData(DWORD size);

	LRESULT
		OnGetEvtData(DWORD size);
public:

// ----------------------------------------------------------------------------
// Constructor/Distructor

	TSonorkIpc();
	~TSonorkIpc();

// ----------------------------------------------------------------------------
// Startup/Shutdown

	SONORK_RESULT
		Startup(HWND 			ownerHwnd
			,SONORK_SERVICE_ID	serviceId
			,const char *		configName
			,TSonorkIpcMainCallback*cbPtr
			,void* 			cbParam
			,DWORD			eventMask);

	void
		Shutdown();

// ----------------------------------------------------------------------------
// Information/Status (Permanent)
// These functions may be called at any time

	BOOL
		IsStarted() const
		{ return ctrl.ipcId != 0; }

	DWORD
		IpcId() const
		{ return ctrl.ipcId; }

	SONORK_RESULT
		LastResult() const
		{ return (SONORK_RESULT)ctrl.result;}

// ----------------------------------------------------------------------------
// IMPORTANT:
// These functions below may be called ONLY after IPC is started, calling them
// when not started will generate a memory fault because the Memmory Mapped File
// pointers are all NULL

// ----------------------------------------------------------------------------
// Information/Status (Started only)

// UserId() is a SONORK_DWORD2 as defined in <srk_defs.h>
const	TSonorkId&
		UserId() const
		{ return dta.server.header->userId; }

// UserAlias() is of SONORK_USER_ALIAS_MAX_SIZE size (24)
const  char*
		UserAlias() const
		{ return dta.server.header->alias;}

// AppVersionNumber() as generated by MAKE_VERSION_NUMBER
// defined in <srk_defs.h>
	DWORD
		AppVersionNumber() const
		{ return dta.server.header->appVersionNumber;}

// SONORK_APP_CX_STATUS: enumeration in <sonorkwin32app_defs.h>
const	SONORK_APP_CX_STATUS
		AppStatus() const
		{ return (SONORK_APP_CX_STATUS)dta.server.header->appStatus;}

// The Service Id passed to Startup()		
const 	SONORK_SERVICE_ID
		ServiceId() const
		{ return ctrl.serviceId; }

// Whenever an event is dispatched, this number is incremented by 1
// It may be used to synchronize event processing between instances
//  of a IPC application
	DWORD
		EventSerial() const
		{ return dta.server.header->eventSerial;}


// ----------------------------------------------------------------------------
// API requests
// ----------------------------------------------------------------------------


// ----------------------------------------------------------------------------
// SetEventMask: Sets the filter mask for the events we wish to be notified
// It must be one or more of the SONORK_APP_EVENT_MASK values
// Normally this mask is set during Startup().

	SONORK_RESULT
		SetEventMask(DWORD);

// ----------------------------------------------------------------------------
// GetUserData: Retrieves the full user information (excluding notes) for
// the user. <userData->userId> must be loaded before invoking this method

	SONORK_RESULT
		GetUserData(TSonorkExtUserData* userData);

// ----------------------------------------------------------------------------
// GetUserHandle: Retrieves the network handle of the user.
// <handle.userId> must be loaded before invoking this method.
// Alternatively, the user handle may be obtained from a <TSonorkExtUserData*>
// class instance by invoking its GetUserHandle() method or by manualy loading
// all 3 members.
// <TSonorkUserHandle> is defined in <srk_defs.h> and is composed of
//  <userId> 	: The <SONORK_DWORD2> user's id
//  <sid> 	: The <SONORK_DWORD2> session id (zero if offline)
//  <utsLinkId>	: The peer 2 peer link id (zero if no peer 2 peer connection)
//  A full IPC application would keep a copy of either a <TSonorkExtUserData*>
//  or of a <TSonorkUserHandle*> instance for each user it will be sending
//  messages frequently, and update them from within the main callback
//  ( passed as parameter to Startup() ) everytime a SONORK_APP_EVENT_USER_SID
//  or a SONORK_APP_EVENT_USER_UTS event is received:
//  SONORK_APP_EVENT_USER_SID is invoked everytime the user's <sid> changes
//  and SONORK_APP_EVENT_USER_UTS everytime <utsLinkId> changes.

	SONORK_RESULT
		GetUserHandle(TSonorkUserHandle* handle);

// ----------------------------------------------------------------------------
// EnumUserGroups & EnumUserList: Enumerates the user list items
// and invokes <callback> once for each item. <param> is not used by the
// IPC and is simply passed back to the callback.
// The callback prototype is <TSonorkIpcDataCallback> and receives 2 parameters:
//  <param> (as passed to these functions) and <data>:
//  if the function lists Groups, <data> points to a (const TSonorkGroup*)
//  if the function lists Users, <data> points to a (const TSonorkExtUserData*)
// The callback must not modify nor delete the <data> passed back to it; if
//  it needs <data> after returning, it must make a copy of it.

	SONORK_RESULT
		EnumUserGroups(TSonorkIpcDataCallback*callback,void*param);

	SONORK_RESULT
		EnumUserList(TSonorkIpcDataCallback*callback,void*param);

// ----------------------------------------------------------------------------
// GetCurrentAppMessage: Retrieves the full message for the current event.
//  It may only be invoked from within the main callback (the one provided
//  to Startup() ) and when the ipc event is SONORK_IPC_EVENT_APP_EVENT
//  and the app event is either SONORK_APP_EVENT_SONORK_MSG_RCVD
//   or  SONORK_APP_EVENT_SONORK_MSG_SENDING.
//  Invoking it at any other time will return SONORK_RESULT_INVALID_OPERATION.

	SONORK_RESULT
		GetCurrentAppMessage(TSonorkMsg*);

// ----------------------------------------------------------------------------
// Shared Slots:
//  Small 4-dword buffers that may be shared between instances of
//   the IPC applications.
//  The framework uses a mutex to enforce integrity. Currently only one
//   slot may be alloced per IPC application.

	SONORK_IPC_SLOT_HANDLE
		AllocSharedSlot(DWORD param,SONORK_IPC_ALLOC_METHOD,SONORK_SERVICE_ID id=SONORK_SERVICE_ID_NONE);

	DWORD
		GetSharedSlotValue(SONORK_IPC_SLOT_HANDLE slotHandle
			,SONORK_IPC_SHARED_SLOT_INDEX);

	SONORK_RESULT
		SetSharedSlotValue(SONORK_IPC_SLOT_HANDLE slotHandle
			,SONORK_IPC_SHARED_SLOT_INDEX
			,DWORD );
	SONORK_RESULT
		FreeSharedSlot(SONORK_IPC_SLOT_HANDLE slotHandle);


// ----------------------------------------------------------------------------
// Message sending

	// PrepareMsg()
	//   Resets/loads all message fields using the current profile's settings.
	//   It also assigns a unique value to <msgHandle.handleId>: This value
	//   is included in all message events so each window/IPC application
	//   can detect if the event is originated from its own handle.
	//   If the same message is being sent to several user, the
	//   message needs to be prepared only once.

	SONORK_RESULT
		PrepareMsg(
			  TSonorkMsgHandle*	msgHandle
			, TSonorkMsg* 		msg
			, DWORD 		sysFlags
			, DWORD 		usrFlags
			, DWORD 		pcFlags
			, SONORK_SERVICE_ID     dataServiceId
			, SONORK_SERVICE_TYPE	dataServiceType
			, DWORD 		replyTrackingNo
			);

	// SendMsg()
	//   Sends <msg> to the user specified by <userHandle>
	//   <msgHandle> should be previously prepared with PrepareMsg()
	//   <usrHandle> is defined in <srk_defs.h> and is explained
	//    in the comments of the GetUserHandle() method.
	//   <target> is optional and allows the application to specify
	//   a subsystem running on the machine of <userHandle>
	//  NOTE: The function may be called with only the <userId>
	//   member of <userHandle> loaded, but then:
	//   a) The <sid> and <utsLinkId> members must be zeroed.
	//   b) The process will be slightly slower because the
	//      <sid> and <utsLinkId> must be resolved by the server.

	SONORK_RESULT
		SendMsg(  TSonorkMsgHandle*		msgHandle
			, const TSonorkUserHandle*	userHandle
			, const TSonorkMsg* 		msg
			, const TSonorkMsgTarget*	target=NULL
			);

	SONORK_RESULT
		SendFiles(const TSonorkId&		userId
			, const TSonorkDynStringQueue&	queue
			);

	SONORK_RESULT
		RecvFile(const TSonorkId&		userId
			, const TSonorkCCacheMark&	lineMarker
			, TSonorkShortString&		path
			);

// ----------------------------------------------------------------------------
// Services

	SONORK_RESULT
		RegisterService(
		  SONORK_APP_SERVICE_TYPE	svcType
		, TSonorkServiceInstanceInfo*   svcInfo
		, DWORD		 		svcTag
		, SKIN_ICON			svcIcon=SKIN_ICON_IPC
		);

	SONORK_RESULT
		UnregisterService(
		  DWORD			instanceId
		, BOOL 			invokeCallbacks=false
		, SONORK_SERVICE_ID 	serviceId=SONORK_SERVICE_ID_NONE);


	SONORK_RESULT
		GetServiceData(TSonorkDynData*);

	SONORK_RESULT
		GetServiceData(BYTE*bufferPtr, DWORD bufferSize);

	SONORK_RESULT
		GetServiceDataAtom(TSonorkCodecAtom*);

	SONORK_RESULT
		SetServerData(TSonorkUserServer*userver);

	SONORK_RESULT
		StartServiceQuery(
			  DWORD			sourceInstance
			, TSonorkServiceQuery*	queryHandle
			, SONORK_CTRLMSG_CMD 	cmd
			, DWORD 		userParam
			, DWORD 		userFlags
			, DWORD			timeoutMsecs
			, const void*		dataPtr
			, DWORD			dataSize
			, SONORK_DWORD2*	queryTag);
	SONORK_RESULT
		StartServiceQuery(
			  DWORD			sourceInstance
			, TSonorkServiceQuery*	queryHandle
			, SONORK_CTRLMSG_CMD 	cmd
			, DWORD 		userParam
			, DWORD 		userFlags
			, DWORD			timeoutMsecs
			, const TSonorkCodecAtom* atom
			, SONORK_DWORD2*	queryTag);

	SONORK_RESULT
		PokeServiceData(
			  DWORD			sourceInstance
			, TSonorkServiceHandle*	targetService
			, SONORK_CTRLMSG_CMD 	cmd
			, DWORD 		userParam
			, DWORD 		userFlags
			, const void*		dataPtr
			, DWORD			dataSize);

	SONORK_RESULT
		ServiceReply(
			  SONORK_CTRLMSG_CMD 	cmd
			, DWORD 		cmdParam
			, DWORD 		cmdFlags
			, const void*		dataPtr
			, DWORD			dataSize);

	SONORK_RESULT
		ServiceReply(
			  SONORK_CTRLMSG_CMD 	cmd
			, DWORD 		cmdParam
			, DWORD 		cmdFlags
			, const TSonorkCodecAtom*A);
	SONORK_RESULT
		PokeServiceData(
			  DWORD			sourceInstance
			, TSonorkServiceHandle*	targetService
			, SONORK_CTRLMSG_CMD 	cmd
			, DWORD 		userParam
			, DWORD 		userFlags
			, const TSonorkCodecAtom* atom);
			
	SONORK_RESULT
		OpenServiceCircuit(
			  DWORD			sourceInstance
			, TSonorkServiceCircuit*circuit
			, DWORD 		userParam
			, DWORD 		userFlags
			, DWORD			timeoutMsecs
			, const void*		dataPtr
			, DWORD			dataSize
			, SONORK_DWORD2*	circuitTag);

	SONORK_RESULT
		OpenServiceCircuit(
			  DWORD			sourceInstance
			, TSonorkServiceCircuit*circuit
			, DWORD 		userParam
			, DWORD 		userFlags
			, DWORD			timeoutMsecs
			, const TSonorkCodecAtom *atom
			, SONORK_DWORD2*	circuitTag);

	// Must load circuit->systemId with the desired
	// circuit_id before calling.
	SONORK_RESULT
		AcceptServiceCircuit(
			  DWORD			oldSourceInstance
			, DWORD			newSourceInstance
			, TSonorkServiceCircuit*circuit);

	// Must load circuit->systemId with the desired
	// circuit_id before calling.
	SONORK_RESULT
		GetServiceCircuitHandle(
			  DWORD 		 sourceInstance
			, TSonorkServiceCircuit* circuit);

	SONORK_RESULT
		CloseServiceCircuit(DWORD	sourceInstance
			, DWORD			circuitId
			, SONORK_RESULT		result);


};


#endif