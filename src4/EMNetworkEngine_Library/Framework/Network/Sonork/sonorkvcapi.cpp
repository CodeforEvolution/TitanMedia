#include "sonorkvcapi.h"
#include "srk_svr_login_packet.h"
#include "srk_codec_io.h"

// -----------------------------------------------------------------------------

enum REQUEST_TASK_TYPE
{
	REQUEST_TASK_NONE
,	REQUEST_TASK_SEARCH_USER
};

// -----------------------------------------------------------------------------

struct TSonorkApiTask
{
	DWORD	taskTag;
	union
	{
		void	*				ptr;
	};

	TSonorkApiTask(DWORD tag)
	{	taskTag=tag;ptr=NULL; }

};


// -----------------------------------------------------------------------------

static int SrkNetInitCounter=0;

// -----------------------------------------------------------------------------

DWORD
	TSonorkApi::SearchUser(const TSonorkUserDataNotes* userDataNotes
				,	DWORD					searchFlags
				,	DWORD					maxCount
				,	DWORD					taskTag)
{
	TSonorkDataPacket *P;
	SONORK_DWORD2	reqTag;
	UINT			pSize,aSize;
	TSonorkApiTask*	apiTask;

	apiTask = new TSonorkApiTask(taskTag);
	reqTag.v[0]=REQUEST_TASK_SEARCH_USER;
	reqTag.v[1]=(DWORD)apiTask;

	aSize = CODEC_Size(userDataNotes,SONORK_UDN_ENCODE_NOTES,sonorkServerVersion)+64;
	P=TSonorkDataPacket::Alloc( aSize );
	pSize=P->E_SearchUser_R_NEW( aSize 
		,*userDataNotes
		,SONORK_UDN_ENCODE_NOTES
		,searchFlags|SONORK_FUNC_F_SEARCH_USER_NEW|SONORK_FUNC_NF_SEARCH_USER_METHOD_USER_INFO
		,maxCount);

	return StartRequest(P,pSize,reqTag);

}


// -----------------------------------------------------------------------------

void   	
	TSonorkApi::ProcessSonorkEvent(void*,TSonorkClientEvent*E)
{
	app.event.app = E;
	switch(E->EventType())
	{
	case SONORK_CLIENT_EVENT_STATUS_CHANGE:
		net.prevStatus=net.status;
		net.status=client->Status();

		if(net.status == SONORK_NETIO_STATUS_DISCONNECTED
		|| net.status == SONORK_NETIO_STATUS_CONNECTED)
		{
			user.data.addr.header.sid.Set( client->Sid() );
			user.data.addr.header.sidFlags.Set( client->SidFlags() );
			user.data.addr.hostAddr.Set( client->HostAddr() );
			user.data.addr.utsAddr.Clear();
		}
		user.data.addr.header.version.Set( app.version );

		InvokeAppHandler(SONORK_API_EVENT_CLIENT);
		break;

	case SONORK_CLIENT_EVENT_GLOBAL_TASK:
		OnGlobalTask(E->TaskFunction(), E->ErrorInfo());
		break;

	case SONORK_CLIENT_EVENT_MONITOR:
		//InvokeAppHandler();
		break;

	case SONORK_CLIENT_EVENT_MSG:
		OnMsg( E->Msg() );
		break;

	case SONORK_CLIENT_EVENT_USER_ADD:
	case SONORK_CLIENT_EVENT_USER_SET:
	case SONORK_CLIENT_EVENT_USER_SID:
	case SONORK_CLIENT_EVENT_USER_DEL:
	case SONORK_CLIENT_EVENT_GROUP_ADD:
		InvokeAppHandler(SONORK_API_EVENT_CLIENT);
		break;

	case SONORK_CLIENT_EVENT_USER_AUTH:
		E->UserAuthReq()->header.time.ToLocal();
		InvokeAppHandler(SONORK_API_EVENT_CLIENT);
		break;

	case SONORK_CLIENT_EVENT_CTRL_MSG:
	case SONORK_CLIENT_EVENT_USER_SYNCH_END:
	case SONORK_CLIENT_EVENT_WAPP_ADD:
	case SONORK_CLIENT_EVENT_SYS_MSG:
		break;

	case SONORK_CLIENT_EVENT_LOAD_LOGIN_REQ:
		OnLoadLoginReq(E->LoginReqPacket());
		break;
	}
	app.event.app=NULL;
}

// -----------------------------------------------------------------------------

void SONORK_CALLBACK 
 TSonorkApi::ProcessSonorkRequest(void *param,TSonorkClientRequestEvent*E)
{
	TSonorkApi*_this = (TSonorkApi*)param;

	if(E->EventType() == SONORK_CLIENT_REQUEST_EVENT_PACKET )
	{
		_this->ProcessSonorkRequestPacket(E);
	}
	else
	if(E->EventType() == SONORK_CLIENT_REQUEST_EVENT_END )
	{
		_this->ProcessSonorkRequestEnd( E );
	}
}

// -----------------------------------------------------------------------------
// User Add

// returns task id
DWORD
 TSonorkApi::AddUserAuth(const TSonorkId&	userId
			,DWORD						groupNo
			,SONORK_C_CSTR				requestText
			,DWORD						taskTag)
{
	TSonorkAuth2	auth;
	TSonorkText		text;
	UINT			aSize,pSize;
	TSonorkDataPacket	*	P;
	SONORK_DWORD2	reqTag;


	reqTag.v[0]=REQUEST_TASK_NONE;
	reqTag.v[1]=taskTag;

	auth.Clear();
	auth.flags.SetUserInfoLevel(SONORK_USER_INFO_LEVEL_1);
	auth.SetGroupNo(groupNo);

	text.SetStr(0,requestText);

	aSize = text.CODEC_Size(sonorkServerVersion) + 64;

	P = TSonorkDataPacket::Alloc( aSize );
	pSize = P->E_ReqAuth_R(aSize
		,userId
		,auth,text);

	return StartRequest(P,pSize,reqTag);
}

// -----------------------------------------------------------------------------

// returns task id
DWORD
 TSonorkApi::ReplyUserAuth(const TSonorkId&	userId
			,const TSonorkAuth2&	auth 
			, DWORD taskTag)
{
	UINT			aSize,pSize;
	TSonorkDataPacket	*	P;
	SONORK_DWORD2	reqTag;


	reqTag.v[0]=REQUEST_TASK_NONE;
	reqTag.v[1]=taskTag;

	aSize = 128;

	P = TSonorkDataPacket::Alloc( aSize );
	pSize = P->E_AknAuth_R(aSize , userId , auth );

	return StartRequest(P,pSize,reqTag);
}

// -----------------------------------------------------------------------------


// returns task id
DWORD
 TSonorkApi::DelUserAuth(const TSonorkId& userId, DWORD taskTag)
{
	UINT			aSize,pSize;
	TSonorkDataPacket	*	P;
	SONORK_DWORD2	reqTag;


	reqTag.v[0]=REQUEST_TASK_NONE;
	reqTag.v[1]=taskTag;

	aSize = 128;

	P = TSonorkDataPacket::Alloc( aSize );
	pSize = P->E_DelAuth_R(aSize , userId );

	return StartRequest(P,pSize,reqTag);


}

// -----------------------------------------------------------------------------

void 
 TSonorkApi::ProcessSonorkRequestPacket(TSonorkClientRequestEvent*E)
{
	TSonorkApiTask*	apiTask;
	TSonorkDataPacket*	P;
	DWORD				pSize;

	app.event.req = E;
	if( E->RequestTag().v[0] != REQUEST_TASK_NONE )
	{
		apiTask=(TSonorkApiTask*)E->RequestTag().v[1];
		P=E->Packet();
		pSize = E->PacketSize();
		switch(E->RequestTag().v[0])
		{


		case REQUEST_TASK_SEARCH_USER:		
			if(P->Function() == SONORK_FUNCTION_SEARCH_USER_NEW
				&&
				P->SubFunction() == SONORK_SUBFUNC_NONE )
			{
				TSonorkUserDataNotesQueue	queue;
				P->D_SearchUser_A(pSize,queue);
				for(;;)
				{
					app.taskData.userDataNotes=queue.RemoveFirst();
					if(app.taskData.userDataNotes == NULL )
						break;
					InvokeAppHandler( SONORK_API_EVENT_TASK );
					SONORK_MEM_DELETE( app.taskData.userDataNotes );
				}
			}

			break;

		}
	}
	else
	{
		InvokeAppHandler( SONORK_API_EVENT_TASK );
	}
	app.event.req = NULL;

}

// -----------------------------------------------------------------------------

void 
 TSonorkApi::ProcessSonorkRequestEnd(TSonorkClientRequestEvent*E)
{
	TSonorkApiTask*	apiTask;

	app.event.req = E;
	InvokeAppHandler( SONORK_API_EVENT_TASK );
	app.event.req = NULL;

	if( E->RequestTag().v[0] != REQUEST_TASK_NONE )
	{
		apiTask=(TSonorkApiTask*)E->RequestTag().v[1];
		switch(E->RequestTag().v[0])
		{

			case REQUEST_TASK_SEARCH_USER:		
			default:
				break;

		}
		delete apiTask;
	}
}

// -----------------------------------------------------------------------------

SONORK_RESULT 
	TSonorkApi::GetClientEvent(TSonorkApiClientEvent*apiEvent)
{
	TSonorkClientEvent*		 E;

	E=app.event.app;

	if(E==NULL || app.apiEvent!=SONORK_API_EVENT_CLIENT)
	{
		app.ERR.Set(SONORK_RESULT_INVALID_OPERATION,"NoEvent",0,true);
		apiEvent->event=SONORK_CLIENT_EVENT_NONE;
		apiEvent->errorInfo.Set(app.ERR);
		return SONORK_RESULT_INVALID_OPERATION;
	}
	
	app.ERR.SetOk();
	apiEvent->event=E->EventType();
	switch(E->EventType())
	{
	case SONORK_CLIENT_EVENT_STATUS_CHANGE:
		apiEvent->eventData[0]=net.status;
		apiEvent->eventData[1]=net.prevStatus;
		apiEvent->eventData[2]=E->StatusFlags();
		apiEvent->errorInfo.Set( *E->ErrorInfo() );


		break;

	case SONORK_CLIENT_EVENT_GLOBAL_TASK:
		apiEvent->eventData[0]=E->TaskFunction();
		apiEvent->eventData[1]=(E->ErrorInfo()!=NULL);
		if(apiEvent->eventData[1])
			apiEvent->errorInfo.Set(*E->ErrorInfo());
		else
			apiEvent->errorInfo.SetOk();
		break;

	case SONORK_CLIENT_EVENT_MONITOR:
		apiEvent->eventData[0]=
		apiEvent->eventData[1]=0;
		apiEvent->errorInfo.SetOk();
		break;

	case SONORK_CLIENT_EVENT_MSG:
		apiEvent->objectId.Set( E->Msg()->UserId() );
		apiEvent->errorInfo.SetOk();
		break;

	case SONORK_CLIENT_EVENT_USER_SID:
		apiEvent->objectId.Set( E->UserId() );
		apiEvent->errorInfo.SetOk();
		break;

	case SONORK_CLIENT_EVENT_USER_ADD:
	case SONORK_CLIENT_EVENT_USER_SET:
		apiEvent->objectId.Set( E->UserData()->UserId() );
		apiEvent->errorInfo.SetOk();
		break;

	case SONORK_CLIENT_EVENT_USER_DEL:
		apiEvent->objectId.Set( E->UserId() );
		apiEvent->errorInfo.SetOk();
		break;

	case SONORK_CLIENT_EVENT_GROUP_ADD:
		apiEvent->objectId.Set( E->Group()->GroupNo() , E->Group()->GroupType() );
		apiEvent->errorInfo.SetOk();
		break;

	case SONORK_CLIENT_EVENT_USER_AUTH:
		apiEvent->objectId.Set( E->UserAuthReq()->header.requestorId );
		apiEvent->objectTime.Set( E->UserAuthReq()->header.time );
		apiEvent->errorInfo.SetOk();
		break;

	case SONORK_CLIENT_EVENT_CTRL_MSG:
	case SONORK_CLIENT_EVENT_USER_SYNCH_END:
	case SONORK_CLIENT_EVENT_WAPP_ADD:
	case SONORK_CLIENT_EVENT_SYS_MSG:
		apiEvent->errorInfo.SetOk();
		break;

	case SONORK_CLIENT_EVENT_LOAD_LOGIN_REQ:
	default:
		app.ERR.Set(SONORK_RESULT_INVALID_OPERATION
				,"Event not processable by Host application",0,true);
		apiEvent->event=SONORK_CLIENT_EVENT_NONE;
		apiEvent->errorInfo.Set(app.ERR);
		return SONORK_RESULT_INVALID_OPERATION;
	}

	return SONORK_RESULT_OK;
}


// -----------------------------------------------------------------------------

SONORK_RESULT
 TSonorkApi::GetEventSid(TSonorkApiUserSid*sid)
{
	TSonorkClientEvent*	E;
	const TSonorkLocusAtom *locus;
	
	E=app.event.app;
	if(E!=NULL && app.apiEvent==SONORK_API_EVENT_CLIENT)
	{			
		if(E->EventType() == SONORK_CLIENT_EVENT_USER_SID ) 
		{
			locus=E->SidLocus();
			if( locus->userLocusEx.sidFlags.IsOnline() )
			{
				sid->userAddr.header.sid.Set( locus->userLocusEx.sid );
				sid->userAddr.header.sidFlags.Set( locus->userLocusEx.sidFlags );
				sid->userAddr.header.version.Set( locus->userLocusEx.version);
				sid->userAddr.utsAddr.Set(locus->userLocusEx.utsAddr);
				sid->userAddr.hostAddr.Set(locus->userLocusEx.hostAddr);
				sid->serial.Set(*E->SidUserSerial());
				sid->sidText.Set(E->SidText()->ToCStr());
			}
			else
			{
				sid->userAddr.Clear();
				sid->serial.Clear();
				sid->sidText.Clear();
			}
			app.ERR.SetOk();
			return SONORK_RESULT_OK;
		}
	}
	app.ERR.Set(SONORK_RESULT_INVALID_OPERATION,"NoEvent",0,true);
	return GetLastResult();
}

// -----------------------------------------------------------------------------

SONORK_RESULT 
 TSonorkApi::SetSonorkServer(
			 	SONORK_PHYS_ADDR_TYPE	addressType
			,	const char*			host
			,   DWORD				port
			,   SONORK_LOGIN_MODE	loginMode)
{
	DWORD code;

	if( GetNetworkStatus()!=SONORK_NETIO_STATUS_DISCONNECTED)
	{
		app.ERR.Set(SONORK_RESULT_INVALID_OPERATION,"Must be disconnected",0,true);
	}
	else
	if( loginMode != SONORK_LOGIN_MODE_INTRANET && loginMode != SONORK_LOGIN_MODE_INTERNET )
	{
		app.ERR.Set(SONORK_RESULT_INVALID_PARAMETER,"Invalid login mode",0,true);
	}
	else
	{
		net.loginMode = loginMode ;
		code=net.sonork.physAddr.SetInet1(
			   addressType
			,  host
			,  (WORD)port);
		if( code != 0)
		{
			app.ERR.Set(SONORK_RESULT_NETWORK_ERROR,"invalid address/protocol",code,true);
		}
		else
		{
			app.ERR.SetOk();
		}
	}

	return GetLastResult();

}

// -----------------------------------------------------------------------------

SONORK_RESULT 
 TSonorkApi::Connect(const SONORK_DWORD2* userId, const char* password)
{
	assert(userId != NULL && password != NULL );
	if( GetNetworkStatus()!=SONORK_NETIO_STATUS_DISCONNECTED )
		app.ERR.Set(SONORK_RESULT_INVALID_OPERATION,"invalid state",0,true);
	else
	{
		user.data.userId.Set( *userId );
		user.pwd.Set( password );
		client->Connect(app.ERR , net.sonork.physAddr);
	}
	return GetLastResult();
}

// -----------------------------------------------------------------------------

void 
 TSonorkApi::Disconnect()
{
	client->Disconnect();
}

// -----------------------------------------------------------------------------

SONORK_RESULT 
 TSonorkApi::RefreshUserList()
{
	client->RefreshUserList(app.ERR);
	return GetLastResult();

}

// -----------------------------------------------------------------------------
// Pointers are valid only while processing the event

TSonorkUserData*
 TSonorkApi::GetEventUser(TSonorkAuth2**authPtr,TSonorkDynString**notesPtr)
{
	TSonorkClientEvent*	E;
	
	E=app.event.app;
	if(E!=NULL && app.apiEvent==SONORK_API_EVENT_CLIENT)
	{			
		app.ERR.SetOk();
		if(E->EventType() == SONORK_CLIENT_EVENT_USER_ADD 
		|| E->EventType() == SONORK_CLIENT_EVENT_USER_SET )
		{
			if( authPtr )*authPtr = E->UserAddSetAuth();
			if( notesPtr)*notesPtr =E->UserAddSetNotes();
			return E->UserData();
		}
		else
		if(E->EventType() == SONORK_CLIENT_EVENT_USER_AUTH )
		{
			if( authPtr )*authPtr  = &E->UserAuthReq()->header.auth;
			if( notesPtr)*notesPtr = &E->UserAuthReq()->notes;
			return &E->UserAuthReq()->userData;

		}
	}
	app.ERR.Set(SONORK_RESULT_INVALID_OPERATION,"NoEvent",0,true);
	return NULL;
}

// -----------------------------------------------------------------------------
SONORK_RESULT
 TSonorkApi::GetEventUser(TSonorkUserDataNotes* UDN)
{
	TSonorkClientEvent*	E;
	
	UDN->Clear();
	E=app.event.app;
	if(E!=NULL && app.apiEvent==SONORK_API_EVENT_CLIENT)
	{			
		app.ERR.SetOk();
		
		if(E->EventType() == SONORK_CLIENT_EVENT_USER_ADD 
		|| E->EventType() == SONORK_CLIENT_EVENT_USER_SET )
		{
			UDN->userData.Set(E->UserAuthReq()->userData);
			UDN->auth.Set(*E->UserAddSetAuth());
			UDN->notes.Set(*E->UserAddSetNotes());
			return SONORK_RESULT_OK;
		}
		else
		if(E->EventType() == SONORK_CLIENT_EVENT_USER_AUTH )
		{
			UDN->userData.Set(E->UserAuthReq()->userData);
			UDN->auth.Set(E->UserAuthReq()->header.auth);
			UDN->notes.Set(E->UserAuthReq()->notes);
			return SONORK_RESULT_OK;

		}
	}
	app.ERR.Set(SONORK_RESULT_INVALID_OPERATION,"NoEvent",0,true);
	return app.ERR.result;
}

// -----------------------------------------------------------------------------
TSonorkMsg*
 	TSonorkApi::GetEventMsg()	// Pointer is valid only while processing the event
{
	TSonorkClientEvent*	E;
	E=app.event.app;

	if(E!=NULL && app.apiEvent==SONORK_API_EVENT_CLIENT)
	{
		if(E->EventType() == SONORK_CLIENT_EVENT_MSG)
		{

			app.ERR.SetOk();
			return E->Msg();
		}
	}
	app.ERR.Set(SONORK_RESULT_INVALID_OPERATION,"NoEvent",0,true);
	return NULL;
}

// -----------------------------------------------------------------------------
TSonorkGroup*
 TSonorkApi::GetEventGroup()	// Pointer is valid only while processing the event
{
	TSonorkClientEvent*	E;
	E=app.event.app;

	if(E!=NULL && app.apiEvent==SONORK_API_EVENT_CLIENT)
	{
		if(E->EventType() == SONORK_CLIENT_EVENT_GROUP_ADD)
		{

			app.ERR.SetOk();
			return E->Group();
		}
	}
	app.ERR.Set(SONORK_RESULT_INVALID_OPERATION,"NoEvent",0,true);
	return NULL;

}

// -----------------------------------------------------------------------------

TSonorkClientRequestEvent*
 TSonorkApi::GetTaskEvent()		// Pointer is valid only while processing the event
{
	if( app.event.req==NULL || app.apiEvent!=SONORK_API_EVENT_TASK  )
	{
		app.ERR.Set(SONORK_RESULT_INVALID_OPERATION,"NoEvent",0,true);
		return NULL;
	}
	return app.event.req;
}

// -----------------------------------------------------------------------------

SONORK_RESULT 
 	TSonorkApi::GetTaskEvent(TSonorkApiTaskEvent*taskEvent)
{
	TSonorkClientRequestEvent*		 E;
	TSonorkApiTask*	apiTask;


	E=GetTaskEvent();

	if( E==NULL )
	{
		taskEvent->taskFunction=SONORK_FUNCTION_NONE;
		return GetLastResult();
	}
	
	taskEvent->event		= E->EventType();
	taskEvent->taskFunction = E->RequestFunction();
	taskEvent->taskSubFunction= E->RequestSubFunction();
	taskEvent->taskId		 = E->RequestId();

	if( E->RequestTag().v[0] == REQUEST_TASK_NONE )
	{
		apiTask				= NULL;
		taskEvent->taskTag	= E->RequestTag().v[1];
	}
	else
	{
		apiTask				= (TSonorkApiTask*) E->RequestTag().v[1];
		taskEvent->taskTag	= apiTask->taskTag;
	}
	if( E->EventType() == SONORK_CLIENT_REQUEST_EVENT_END )
	{
		app.ERR.SetOk();
		taskEvent->packetFunction=SONORK_FUNCTION_NONE;
		taskEvent->packetSubFunction=0;

		taskEvent->packet.ptr=NULL;
		taskEvent->packet.size=0;
		taskEvent->errorInfo.Set(*E->ErrorInfo());
	}
	else
	if( E->EventType() == SONORK_CLIENT_REQUEST_EVENT_PACKET )
	{
		app.ERR.SetOk();
		taskEvent->packetFunction=E->PacketFunction();
		taskEvent->packetSubFunction=E->PacketSubFunction();
		taskEvent->packet.ptr=E->Packet();
		taskEvent->packet.size=E->PacketSize();
		taskEvent->errorInfo.SetOk();
	}
	else
	{
		taskEvent->taskFunction=SONORK_FUNCTION_NONE;
		app.ERR.Set(SONORK_RESULT_INVALID_OPERATION,"NoEvent",0,true);
	}
	return GetLastResult();
}

// -----------------------------------------------------------------------------
TSonorkUserDataNotes*	
 TSonorkApi::GetTaskUserDataNotes()	// Pointer is valid only while processing the event
{
	TSonorkClientRequestEvent*		 E;

	E=GetTaskEvent();

	if( E==NULL )
	{
		return NULL;
	}
	
	if( E->EventType() == SONORK_CLIENT_REQUEST_EVENT_PACKET
	&&  E->RequestTag().v[0] ==  REQUEST_TASK_SEARCH_USER)
	{
		app.ERR.SetOk();
		return app.taskData.userDataNotes;
	}
	app.ERR.Set(SONORK_RESULT_INVALID_OPERATION,"NoEvent",0,true);
	return NULL;
}

TSonorkUserData*	
 TSonorkApi::GetTaskUserData()	// Pointer is valid only while processing the event
{
	TSonorkClientRequestEvent*		 E;

	E=GetTaskEvent();

	if( E==NULL )
	{
		return NULL;
	}
	if( E->EventType() == SONORK_CLIENT_REQUEST_EVENT_PACKET
	&&  E->RequestTag().v[0] ==  REQUEST_TASK_SEARCH_USER)
	{
		app.ERR.SetOk();
		return &app.taskData.userDataNotes->userData;
	}
	app.ERR.Set(SONORK_RESULT_INVALID_OPERATION,"NoEvent",0,true);
	return NULL;
}
	

// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------

SONORK_RESULT
TSonorkApi::PrepareMsg(
			  TSonorkMsg*		msg
			, const char*		text
			, SONORK_SERVICE_ID	targetServiceId	
			, DWORD				targetServiceInstance
			, SONORK_SERVICE_ID	sourceServiceId		
			, DWORD				sourceServiceInstance
			, SONORK_SERVICE_ID	dataServiceId		
			, SONORK_SERVICE_TYPE dataServiceType	
			, const BYTE*		dataPtr
			, DWORD				dataSize
		)
{
	msg->CODEC_Clear();
	msg->text.SetStr(0,text);
	msg->header.target.service.v[TSonorkServiceId::V_ID]		= targetServiceId;
	msg->header.target.service.v[TSonorkServiceId::V_INSTANCE]	= targetServiceInstance;
	msg->header.sourceService.v[TSonorkServiceId::V_ID]			= sourceServiceId;
	msg->header.sourceService.v[TSonorkServiceId::V_INSTANCE]	= sourceServiceInstance;
	msg->header.dataSvcInfo.SetInfo(dataServiceId,dataServiceType,0);
	return SONORK_RESULT_OK;
}

// -----------------------------------------------------------------------------

DWORD 
 TSonorkApi::SendPlainTextMsg(TSonorkLocus1*locus,const char *text, DWORD taskTag)
{
	TSonorkMsg	msg;
	PrepareMsg(&msg,text);
	return SendMsg(locus,&msg,taskTag);
}

// -----------------------------------------------------------------------------

DWORD 
 TSonorkApi::SendMsg(TSonorkLocus1*locus,TSonorkMsg*msg, DWORD taskTag)
{
	TSonorkDataPacket *P;
	SONORK_DWORD2	reqTag;
	UINT	aSize,pSize;

	msg->header.userId.Set( client->UserId() );
	msg->header.target.userId.Set( locus->userId );
	msg->header.trackingNo.v[0] = SONORK_GenTrackingNo(client->UserId(), locus->userId);

	reqTag.v[0]=REQUEST_TASK_NONE;
	reqTag.v[1]=taskTag;

	aSize = msg->CODEC_Size(sonorkServerVersion)+64;
	P=TSonorkDataPacket::Alloc(aSize);
	pSize=P->E_PutMsg_R(aSize
			, locus
			, msg);

	return StartRequest(P,pSize,reqTag);
}

// -----------------------------------------------------------------------------

void   	
 TSonorkApi::OnMsg(TSonorkMsg*msg)
{
	msg->header.time.ToLocal();
	InvokeAppHandler(SONORK_API_EVENT_CLIENT);

}

// -----------------------------------------------------------------------------

void   	
 TSonorkApi::OnGlobalTask(SONORK_FUNCTION function, const TSonorkError* pERR)
{
	
	if( pERR == NULL ) // NB!: pERR is NULL when task is starting
	{
	}
	else
	{
		switch( function )
		{
			case SONORK_FUNCTION_SET_SID:

				user.data.addr.header.sidFlags.Set( client->SidFlags() );
				user.sidFlags.Set( client->SidFlags() );
			
				break;
		}
	}
	InvokeAppHandler(SONORK_API_EVENT_CLIENT);
}

// -----------------------------------------------------------------------------

void   	
 TSonorkApi::OnLoadLoginReq(TSonorkSvrLoginReqPacket*REQ)
{
	DWORD loginFlags=net.loginMode;

	REQ->Prepare2(&user.data
		     ,user.pwd.CStr()
		     ,loginFlags
		     ,app.version);

}

// -----------------------------------------------------------------------------

void 
 TSonorkApi::InvokeAppHandler(SONORK_API_EVENT apiEvent)
{
	if(app.handlerProc != NULL)
	{
		app.apiEvent=apiEvent;
		app.handlerProc(this);
	}
}
// -----------------------------------------------------------------------------

BOOL 
 TSonorkApi::TimeSlot()
{
	return client->TimeSlot();
}

// -----------------------------------------------------------------------------

SONORK_RESULT 
 TSonorkApi::GetLastResultInfo(TSonorkError*pERR)
{
	pERR->Set(app.ERR);	
	return app.ERR.result;
}

// -----------------------------------------------------------------------------

SONORK_RESULT 
 TSonorkApi::SetSidMode(SONORK_SID_MODE sidMode, LPCSTR sidText, BOOL* taskIsPending)
{
	
	if(!IS_VALID_SID_MODE(sidMode))
	{
		app.ERR.Set(SONORK_RESULT_INVALID_PARAMETER,"InvalidSidMode",0,true);
	}
	else
	{
		user.sidText.Set(sidText);
		if( net.status==SONORK_NETIO_STATUS_DISCONNECTED )
		{
			user.sidFlags.SetSidMode( sidMode );
			app.ERR.SetOk();
		}
		else
		if(net.status==SONORK_NETIO_STATUS_CONNECTED)
		{
			TSonorkLocusAtom	A;
			
			A.CODEC_Clear();
			user.sidFlags.SetSidMode( sidMode );

			A.userLocusEx.sidFlags.Set( user.sidFlags );
			A.userLocusEx.version.Set( app.version );
			A.userLocusEx.hostAddr.Set( user.data.addr.hostAddr);
			A.userLocusEx.sidFlags.SetUtsMode(SONORK_SID_UTS_MODE_CLIENT);
			client->SyncSid(app.ERR , A , user.sidText);
			if( app.ERR.result == SONORK_RESULT_OK )
			{
				if( taskIsPending )
						*taskIsPending=true;
				return SONORK_RESULT_OK;
			}
		}
		else
		{
		// Can't change mode during connection phase
			app.ERR.Set(SONORK_RESULT_INVALID_OPERATION,"invalid state",0,true);
		}
	}
	if( taskIsPending )
		*taskIsPending=false;
	return app.ERR.Result();

}

// -----------------------------------------------------------------------------

void sonork_puts(const char*)
{

}

// -----------------------------------------------------------------------------

TSonorkApi::TSonorkApi(fnSonorkApiHandler* cbProc,DWORD cbTag)
{
	if(SrkNetInitCounter==0)
	{
		TSonorkNetIO::Start(1,1);
	}
	SrkNetInitCounter++;
	app.handlerProc = cbProc;
	app.handlerTag  = cbTag;
	app.event.app=NULL;
	app.event.req=NULL;
	app.apiEvent =SONORK_API_EVENT_NONE;
	app.ERR.SetOk();
	net.loginMode	=SONORK_LOGIN_MODE_INTRANET;
	net.status		=SONORK_NETIO_STATUS_DISCONNECTED;
	app.version.Load(SONORK_CLIENT_VERSION_NUMBER
			, SONORK_SID_VF_SUPPORTS_EXT_SID
			| SONORK_SID_VF_SUPPORTS_EXT_MSG
			, SONORK_OS_WIN
			, 0);
	user.data.SetUserInfoLevel(SONORK_USER_INFO_LEVEL_3,false);
	user.sidFlags.Clear();
	user.data.Clear(false);
	client=new TSonorkClient(this,NULL);
}

// -----------------------------------------------------------------------------

TSonorkApi::~TSonorkApi()
{
	assert(SrkNetInitCounter>0);
	Disconnect();
	delete client;

	if( SrkNetInitCounter==1 )
	{
		TSonorkNetIO::Stop();
	}
	SrkNetInitCounter--;

}

// -----------------------------------------------------------------------------

DWORD
 TSonorkApi::StartRequest(TSonorkDataPacket*P
				, UINT pSize
				, SONORK_DWORD2& reqTag)
{
	DWORD taskId;
	client->Request(
			 app.ERR
			,P
			,pSize
			,ProcessSonorkRequest
			,this
			,&reqTag
			,&taskId
			);
	P->Free();
	if( GetLastResult() != SONORK_RESULT_OK)
		return 0;
	return taskId;

}

// -----------------------------------------------------------------------------

SONORK_SID_MODE 
 TSonorkApi::NormalizeSidMode(SONORK_SID_MODE sidMode)
{
	switch(sidMode)
	{
		default:
		case SONORK_SID_MODE_ONLINE:
		case SONORK_SID_MODE_ONLINE_02:
		case SONORK_SID_MODE_ONLINE_03:
			return SONORK_SID_MODE_ONLINE;

		case SONORK_SID_MODE_BUSY:
		case SONORK_SID_MODE_BUSY_02:
		case SONORK_SID_MODE_BUSY_03:
			return SONORK_SID_MODE_BUSY;

		case SONORK_SID_MODE_AT_WORK:
		case SONORK_SID_MODE_AT_WORK_02:
		case SONORK_SID_MODE_AT_WORK_03:
			return SONORK_SID_MODE_AT_WORK;

 		case SONORK_SID_MODE_FRIENDLY:
		case SONORK_SID_MODE_FRIENDLY_02:
		case SONORK_SID_MODE_FRIENDLY_03:
			return SONORK_SID_MODE_FRIENDLY;

		case SONORK_SID_MODE_AWAY:
		case SONORK_SID_MODE_AWAY_AUTO:
		case SONORK_SID_MODE_AWAY_HOLD:
		case SONORK_SID_MODE_AWAY_PHONE:
		case SONORK_SID_MODE_AWAY_02:
		case SONORK_SID_MODE_AWAY_03:
			return SONORK_SID_MODE_AWAY;

		case SONORK_SID_MODE_INVISIBLE:
		case SONORK_SID_MODE_INVISIBLE_02:
		case SONORK_SID_MODE_INVISIBLE_03:
			return SONORK_SID_MODE_INVISIBLE;

		case SONORK_SID_MODE_DISCONNECTED:
			return SONORK_SID_MODE_DISCONNECTED;
	}
}

// -----------------------------------------------------------------------------
// TSonorkApiFileSession
// -----------------------------------------------------------------------------

SONORK_RESULT
 TSonorkApiFileSession::RequestGetFile(
			const TSonorkFileLocus*	fileLocus
			, BOOL				deleteFile)
{
	TSonorkDataServerNewGetFileReq REQ;
	UINT 			aSize,pSize;
	TSrkIpcPacket	*P;
	

	if(txPhase != SONORK_API_FILE_PHASE_CONNECTED
	&& txPhase != SONORK_API_FILE_PHASE_COMPLETE)
	{
		txERR.Set(SONORK_RESULT_INVALID_OPERATION,"InvalidState",0,true);
		return GetLastResult();
	}

	// Dummy crypt for this api (will do properly later)
	cryptCtx.SetSimple( ((DWORD)this+ (DWORD)fileLocus) | 1 );

	REQ.CODEC_Clear();
	txMode=deleteFile?SONORK_API_FILE_TX_DELE:SONORK_API_FILE_TX_RECV;

	REQ.header.flags = deleteFile?SONORK_DATA_SERVER_GET_FILE_F_DELETE:0;
	REQ.locus.Set( *fileLocus );
	cryptCtx.GetCryptInfo(&REQ.crypt_info);

	aSize=REQ.CODEC_Size(sonorkServerVersion);
	P=SONORK_AllocIpcPacket( aSize );
	pSize = P->E_Req(aSize,SONORK_DATA_SERVER_FUNCTION_GET_FILE_NEW,&REQ,sonorkServerVersion);
	tcp.SendPacket(SONORK_NETIO_HFLAG_TYPE_PACKET , P, pSize);
	SONORK_FreeIpcPacket( P );

	txERR.SetOk();
	txMode = SONORK_API_FILE_TX_RECV;
	SetPhase( SONORK_API_FILE_PHASE_REQUESTING , false );
	return SONORK_RESULT_OK;	

}

// -----------------------------------------------------------------------------

SONORK_RESULT
 TSonorkApiFileSession::RequestPutFile(
		  const SONORK_DWORD2List*  targetList
		, const TSonorkFileInfo*    fileInfo
		, SONORK_DWORD2* trackingNo)
{
	TSonorkDataServerNewPutFileReq REQ;
	UINT 			aSize,pSize;
	TSrkIpcPacket	*P;
	SONORK_DWORD2	*pdw2;
	

	if(txPhase != SONORK_API_FILE_PHASE_CONNECTED
	&& txPhase != SONORK_API_FILE_PHASE_COMPLETE)
	{
		txERR.Set(SONORK_RESULT_INVALID_OPERATION,"InvalidState",0,true);
		return GetLastResult();
	}


	// Dummy crypt for this api (will do properly later)
	cryptCtx.SetSimple( ((DWORD)this+(DWORD)targetList) | 1 );

	fileLocus.Clear();

	REQ.CODEC_Clear();
	REQ.file_info.Set(*fileInfo);

	aSize=targetList->Items();
	REQ.target_list.PrepareFor(aSize);

	for( pSize=0 , pdw2=targetList->ItemPtr(0)
		;pSize<aSize
		;pSize++ , pdw2++)
	{
		REQ.target_list.AddItem(pdw2);
	}

	// Load the tracking no
	if( trackingNo != NULL )
		REQ.header.trackingNo.Set( *trackingNo );
	else
		REQ.header.trackingNo.Clear();


	// Load the Encryption context.
	cryptCtx.GetCryptInfo(&REQ.crypt_info);
	memset(REQ.file_info.attr.reserved,0,sizeof(REQ.file_info.attr.reserved));

	data.limit = REQ.file_info.attr.stor_size.v[0];
	data.offset= 0;

	aSize=REQ.CODEC_Size(sonorkServerVersion);
	P=SONORK_AllocIpcPacket( aSize );
	pSize  = P->E_Req(aSize,SONORK_DATA_SERVER_FUNCTION_PUT_FILE_NEW,&REQ,sonorkServerVersion);
	tcp.SendPacket(SONORK_NETIO_HFLAG_TYPE_PACKET , P, pSize);
	SONORK_FreeIpcPacket( P );


	txERR.SetOk();
	txMode = SONORK_API_FILE_TX_SEND;
	SetPhase( SONORK_API_FILE_PHASE_REQUESTING , false );
	return SONORK_RESULT_OK;	



}

// -----------------------------------------------------------------------------

void	
 TSonorkApiFileSession::InvokeEventHandler(SONORK_API_FILE_EVENT evt)
{
	// TimeSlot() increses RefCount by 1 uppon entry
	// and releases uppon exit
	// So if caller has not called DecRefCount(), the refCount will be at least 2
	
	if( !inCallback 
		&& 
		refCount>=2)
	{
		inCallback=true;
		apiEvent=evt;
		sessionProc(this);
		apiEvent=SONORK_API_FILE_EVENT_NONE;
		inCallback=false;
	}
}

// -----------------------------------------------------------------------------

void
 TSonorkApiFileSession::SetPhase(SONORK_API_FILE_PHASE newPhase,bool invokeHandler)
{
	if( txPhase == newPhase)
		return;

	prevPhase = txPhase;
	txPhase   = newPhase;

	if( txPhase <= SONORK_API_FILE_PHASE_IDLE )
	{
		tcp.Shutdown();
	}
	if( invokeHandler )
	{
		InvokeEventHandler(SONORK_API_FILE_EVENT_PHASE_CHANGE);
	}
}

// -----------------------------------------------------------------------------

void
 TSonorkApiFileSession::Disconnect(bool invokeHandler)
{
	txERR.SetOk();
	SetPhase(SONORK_API_FILE_PHASE_ERROR , invokeHandler);
}

// -----------------------------------------------------------------------------
	
void
 TSonorkApiFileSession::Abort(const TSonorkError&ERR,bool invokeHandler)
{
	txERR.Set(ERR);
	SetPhase(SONORK_API_FILE_PHASE_ERROR , invokeHandler);
}

// -----------------------------------------------------------------------------

void 
 TSonorkApiFileSession::TimeSlot()
{
	TSonorkTcpPacket *P;
	DWORD	packetsProcessed;

	assert( !inCallback );
	if( txPhase > SONORK_API_FILE_PHASE_IDLE 
	&&  netStatus > SONORK_NETIO_STATUS_DISCONNECTED)
	{

		IncRefCount();

		for( packetsProcessed = 0; packetsProcessed < maxRecvPacketsPerTimeSlot; packetsProcessed++)
		{
			if((P = tcp.Recv(0)) == NULL )
				break;
			ProcessTcpPacket(P);
			P->Free();
		}


		if( netStatus != tcp.Status() )
		{
			netStatus = tcp.Status();
			if( netStatus == SONORK_NETIO_STATUS_CONNECTED )
			{
				SendLoginRequest();
			}
			else
			if( netStatus == SONORK_NETIO_STATUS_DISCONNECTED )
			{
				txERR.Set(SONORK_RESULT_NETWORK_ERROR,"ConnectionLost",0,true);
				SetPhase(SONORK_API_FILE_PHASE_ERROR , true );
			}
		}
		else
		{
			if( txPhase == SONORK_API_FILE_PHASE_TRANSMITTING && txMode==SONORK_API_FILE_TX_SEND )
			{
				// We're transmitting
				SendFileData();
			}
		}

		DecRefCount();
	}
}

// -----------------------------------------------------------------------------

void 
 TSonorkApiFileSession::SendFileData()
{
	void*	sendData;
	UINT	sendSize;
	BOOL	mustFreePtr;
	UINT	packetsSent;
	for(packetsSent=0
		; data.offset < data.limit	
		   && tcp.PendingPackets() < maxSendQueuedPackets
		   && packetsSent < maxSendPacketsPerTimeSlot
		; packetsSent++)
	{
		data.block.ptr  = tcp.Buffer();
		data.block.size = tcp.BufferSize();
		if( data.offset + data.block.size > data.limit )
			data.block.size = data.limit - data.offset;

		InvokeEventHandler(SONORK_API_FILE_EVENT_SEND_DATA);
		if( txPhase != SONORK_API_FILE_PHASE_TRANSMITTING)
			break;

		cryptCtx.Encrypt(
			 data.block.ptr
			,data.block.size
			,&sendData
			,&sendSize
			,&mustFreePtr);

		tcp.SendPacket(SONORK_NETIO_HFLAG_TYPE_RAW_DATA
			,sendData
			,sendSize);
		if(mustFreePtr)
		{
			SONORK_MEM_FREE( sendData );
		}
		data.offset+=data.block.size;
	}
	data.block.ptr = NULL;
	data.block.size= 0;

	if( data.offset >= data.limit )
	{
		SetPhase(SONORK_API_FILE_PHASE_WAIT_CONFIRM,true);
	}

}

// -----------------------------------------------------------------------------

void 
 TSonorkApiFileSession::SendLoginRequest()
{
	TSonorkDataServerLoginReq REQ;
	TSrkIpcPacket	*ipcPacket;
	UINT aSize,pSize;

	txERR.SetOk();
	SetPhase( SONORK_API_FILE_PHASE_AUTHORIZING , true );

	REQ.CODEC_Clear();

	REQ.header.locus.Set(senderLocus);
	REQ.header.pinType	= SONORK_PIN_TYPE_64;
	SONORK_EncodePin64(REQ.header.pin
		,senderLocus.userId		   // our userId
		,senderPin			    	// our Sid Pin
		,serviceLocus.locus.userLocusEx.userId	    	// remote userId
		,SONORK_SERVICE_ID_DATA_SERVER
		,0);

	REQ.header.serverNo 		= serviceLocus.ServerNo();
	REQ.header.serviceType		= SONORK_SERVICE_TYPE_SONORK_FILE;
	REQ.header.loginFlags		= SONORK_DATA_SERVER_LOGIN_F_CORRECTED_HEADER;
	REQ.header.clientVersion	= SONORK_CLIENT_VERSION_NUMBER;


	aSize = REQ.CODEC_Size( sonorkServerVersion )+32;
	ipcPacket=SONORK_AllocIpcPacket( aSize );
	pSize = ipcPacket->E_Req(aSize,SONORK_DATA_SERVER_FUNCTION_LOGIN_REQ,&REQ,sonorkServerVersion );
	tcp.SendPacket(SONORK_NETIO_HFLAG_TYPE_PACKET,ipcPacket,pSize);
	SONORK_FreeIpcPacket(ipcPacket);
}

// -----------------------------------------------------------------------------

void 
 TSonorkApiFileSession::ProcessTcpPacket(TSonorkTcpPacket *tcpPacket)
{

	// A TSonorkTcpPacket has 2 attributes: type and size
	// When the SonorkServer sends an TSrkIpcPacket it sets
	// type to SONORK_NETIO_HFLAG_TYPE_PACKET
	// when it is sending raw file data it sets type to
	// SONORK_NETIO_HFLAG_TYPE_RAW_DATA

	// A raw data packet: The data server is sending us the data
	// for a file we requested.
	if( tcpPacket->Type() == SONORK_NETIO_HFLAG_TYPE_RAW_DATA )
	{
		// Yes: Process the data
		RecvFileData(tcpPacket->DataPtr(),tcpPacket->DataSize());
		return;
	}
	// Is it an TSrkIpcPacket?
	if( tcpPacket->Type() != SONORK_NETIO_HFLAG_TYPE_PACKET )
		return;	// No: Don't know how to handle, so ignore

	ProcessPacket((TSrkIpcPacket*)tcpPacket->DataPtr(),tcpPacket->DataSize());

}

// -----------------------------------------------------------------------------

void
 TSonorkApiFileSession::ProcessPacket(TSrkIpcPacket* ipcPacket,UINT ipcSize)
{
	// TSonorkTcpPacket's data is a TSrkIpcPacket
	if(ipcSize < sizeof(TSrkIpcPacket))
	{
		// Size is less than the header of a TSrkIpcPacket
		// this should never happen!: We've probably connected
		// to something that is NOT a dataserver.
		txERR.SetSys(SONORK_RESULT_PROTOCOL_ERROR,GSS_PCKTOOSMALL,0);
		SetPhase( SONORK_API_FILE_PHASE_ERROR , true );
		return;
	}

	// When the data server is sending us an error instead
	// of the AKN for our request, it sets the ERROR flag in
	// the TSonorkTcpPacket. This flag means that the packet
	// contains an TSonorkError instead of the normal data
	// it would have contained for the AKN of the request.
	if( ipcPacket->IsError() )
	{
		// Extract the error
		if(!ipcPacket->D_Error(ipcSize,txERR))
		{
			// Geez, we could not even decode the error..
			// what is going on?. Set a default error:
			// We've probably connected to something
			// that is NOT a dataserver.
			txERR.SetSys(SONORK_RESULT_CODEC_ERROR
				,GSS_BADCODEC
				,SONORK_MODULE_LINE);
		}
		if( txERR.result != SONORK_RESULT_OK )
		{
			SetPhase( SONORK_API_FILE_PHASE_ERROR , true );
			return;
		}
	}
	else
		txERR.SetOk();

	switch(txPhase)
	{
	default:
		return;

	case SONORK_API_FILE_PHASE_AUTHORIZING:
		if( ipcPacket->Function() != SONORK_DATA_SERVER_FUNCTION_LOGIN_REQ )
		{
			// We do not understand the packet, assume ok and ignore
			return;
		}

		// The server accepted our login
		SetPhase(SONORK_API_FILE_PHASE_CONNECTED , true );
		break;

	case SONORK_API_FILE_PHASE_REQUESTING:
		ProcessRequestingPacket(ipcPacket,ipcSize);
		break;

	case SONORK_API_FILE_PHASE_WAIT_CONFIRM:
		ProcessWaitConfirmPacket(ipcPacket,ipcSize);
		break;
	}

}


// -----------------------------------------------------------------------------

void
 TSonorkApiFileSession::ProcessRequestingPacket(TSrkIpcPacket* ipcPacket,UINT ipcSize)
{
	if( txMode == SONORK_API_FILE_TX_SEND )
	{
		if( ipcPacket->Function() != SONORK_DATA_SERVER_FUNCTION_PUT_FILE_OLD
		&&  ipcPacket->Function() != SONORK_DATA_SERVER_FUNCTION_PUT_FILE_NEW)
		{
			// Don't understand the packet: Assume ok and return
			return;
		}

	}
	else
	{
		if( ipcPacket->Function() != SONORK_DATA_SERVER_FUNCTION_GET_FILE_OLD
		&&  ipcPacket->Function() != SONORK_DATA_SERVER_FUNCTION_GET_FILE_NEW)
		{
			// Don't understand the packet: Assume ok and return
			return;
		}
		
		if( txMode == SONORK_API_FILE_TX_DELE )
		{
			SetPhase(SONORK_API_FILE_PHASE_COMPLETE,true);
			return;
		}
		
		if( txMode == SONORK_API_FILE_TX_RECV )
		{
			TSonorkFileInfo	fileInfo;
			if( !ipcPacket->D_Akn(ipcSize,&fileInfo,sonorkServerVersion) )
			{
				txERR.SetSys(SONORK_RESULT_CODEC_ERROR
					,GSS_BADCODEC
					,SONORK_MODULE_LINE);
				SetPhase(SONORK_API_FILE_PHASE_ERROR,true);
				return;
			}
			data.limit = fileInfo.attr.stor_size.v[0];
			data.offset= 0;


		}
		else
			return;
	}
	SetPhase(SONORK_API_FILE_PHASE_TRANSMITTING ,true);
}

// -----------------------------------------------------------------------------

void
 TSonorkApiFileSession::ProcessWaitConfirmPacket(TSrkIpcPacket* ipcPacket,UINT ipcSize)
{
	if( txMode == SONORK_API_FILE_TX_RECV || txMode == SONORK_API_FILE_TX_DELE )
	{

		if( ipcPacket->Function() != SONORK_DATA_SERVER_FUNCTION_CLOSE_FILE )
		{
			// We do not understand the packet, assume ok and ignore
			
			return;
		}
		SetPhase(SONORK_API_FILE_PHASE_COMPLETE,true);
	}
	else
	if( txMode == SONORK_API_FILE_TX_SEND )
	{
		switch(ipcPacket->Function())
		{

		case SONORK_DATA_SERVER_FUNCTION_PUT_FILE_OLD:
		case SONORK_DATA_SERVER_FUNCTION_PUT_FILE_NEW:

			if( ipcPacket->D_Akn(ipcSize
					,&TSonorkCodecDW(&fileLocus,sizeof(fileLocus)
					,(SONORK_ATOM_TYPE)SRK_DATA_SERVER_ATOM_FILE_LOCUS)
					, sonorkServerVersion)  )
			{
				SetPhase(SONORK_API_FILE_PHASE_COMPLETE,true);
			}
			else
			{
				txERR.SetSys(SONORK_RESULT_CODEC_ERROR
					,GSS_BADCODEC
					,SONORK_MODULE_LINE);
				SetPhase(SONORK_API_FILE_PHASE_ERROR,true);
			}
			break;
		}
	}

}

// -----------------------------------------------------------------------------

void
 TSonorkApiFileSession::RecvFileData(BYTE*dataPtr, UINT dataSize)
{
	BOOL mustFreePtr;

	// Are we in receive mode?
	if( txMode != SONORK_API_FILE_TX_RECV || txPhase != SONORK_API_FILE_PHASE_TRANSMITTING )
	{
		// Nooot: Ignore
		return;
	}

	cryptCtx.Uncrypt(dataPtr
			,dataSize
			,(void**)&data.block.ptr
			,&data.block.size
			,&mustFreePtr);

	
	InvokeEventHandler(SONORK_API_FILE_EVENT_RECV_DATA);
	data.offset+=data.block.size;

	if( mustFreePtr )
	{
		SONORK_MEM_FREE( data.block.ptr );
	}
	data.block.ptr = NULL;
	data.block.size= 0;

	if( data.offset >= data.limit )
		RecvFileDataComplete();
}

// -----------------------------------------------------------------------------

void
 TSonorkApiFileSession::RecvFileDataComplete()
{
	TSrkIpcPacket*	P;
	UINT			pSize;
	P=SONORK_AllocIpcPacket( 0 );
	pSize = P->E_Req(0,SONORK_DATA_SERVER_FUNCTION_CLOSE_FILE,NULL,SONORK_NULL_CODEC_VERSION);
	tcp.SendPacket(SONORK_NETIO_HFLAG_TYPE_PACKET , P, pSize);
	SONORK_FreeIpcPacket( P );
	SetPhase(SONORK_API_FILE_PHASE_WAIT_CONFIRM , true );
}


// -----------------------------------------------------------------------------

TSonorkApiFileSession::TSonorkApiFileSession(fnSonorkApiFile*sp,DWORD st)
{
		sessionProc=sp;
		sessionTag=st;
		netStatus = SONORK_NETIO_STATUS_DISCONNECTED;
		refCount  = 1;
		inCallback=false;
		txPhase   = 
		prevPhase = SONORK_API_FILE_PHASE_IDLE;
		txMode	  = SONORK_API_FILE_TX_NONE;
		txERR.SetOk();
		maxRecvPacketsPerTimeSlot=
		maxSendPacketsPerTimeSlot=
		maxSendQueuedPackets	 =8;
		memset(&data,0,sizeof(data));
		
}

// -----------------------------------------------------------------------------

void
 TSonorkApiFileSession::IncRefCount()
{
	refCount++;
}

// -----------------------------------------------------------------------------

void
 TSonorkApiFileSession::DecRefCount()
{
	if(--refCount == 0)
	{
		delete this;
	}
}


// -----------------------------------------------------------------------------
BOOL
 TSonorkApi::ResolveServiceAddress(
			  TSonorkPhysAddr&	resolvedAddr	// Resolved addr
			, TSonorkPhysAddr& 	socksAddr	// if Socks must be Used
			, const TSonorkSidFlags& sidFlags
			, const TSonorkPhysAddr& utsAddr
			, const TSonorkPhysAddr& hostAddr
			, bool  connectingToServer)
{
	BOOL	resolved=true,useDefaultSocks=true,shouldUseSocks;
	DWORD	utsPort;
	const TSonorkP2Mapping*MAP;

	MAP=client->GetNetworkMapping(sidFlags.NetworkId());
	utsPort=utsAddr.GetInet1Port();
	socksAddr.Clear();
	for(;;)
	{
// ---------------

	if( !utsAddr.IsValid(SONORK_PHYS_ADDR_TCP_1 , false ) )
	{
		resolved=false;
		break;
	}
	
	if( MAP != NULL )
	if( MAP->MapType() == SONORK_NET_MAP_F_TYPE_PREFIXED )
	{
		goto use_mapping;
	}

	if( TSonorkNetIO::IsPrivateInetAddr( utsAddr.GetInet1Addr()) )
	{
		// UTS is private address
		if( TSonorkNetIO::IsPrivateInetAddr( client->ServerAddr().GetInet1Addr() ) )
		{
			// Server is also private:
			//  Most probably, both user and server are in the same network
			resolvedAddr.Set( utsAddr );
			shouldUseSocks = false;
			break;
		}

		if( user.data.addr.hostAddr.GetInet1Addr() == hostAddr.GetInet1Addr() )
		{
			resolvedAddr.Set( utsAddr );
			shouldUseSocks = false;
			break;
		}

		if( TSonorkNetIO::IsPrivateInetAddr(hostAddr.GetInet1Addr()) )
		{
			resolved=false;
			break;
		}

		if( sidFlags.UtsMode() == SONORK_SID_UTS_MODE_MAPPED )
		{
			resolvedAddr.Set( hostAddr );
			shouldUseSocks = true;
		}
		else
		{
			resolved=false;
		}
		break;
	}
	resolvedAddr.Set( utsAddr );
	shouldUseSocks = true;

// ---------------
	break;
	}

	if( !resolved && connectingToServer )
	{
		resolved=true;
		resolvedAddr.Set( client->ServerAddr() );
	}
	if( resolved )
	{
		resolvedAddr.SetInet1Port( (WORD)utsPort );
	}
	else
	if( MAP != NULL )
	if( MAP->MapType() == SONORK_NET_MAP_F_TYPE_SUFFIXED)
	{
use_mapping:
		resolved=true;
		utsPort+=MAP->MapperPort();
		switch(MAP->MapMethod())
		{
		default:
		case SONORK_NET_MAP_F_METHOD_FAIL:
			resolved=false;
			break;

		case SONORK_NET_MAP_F_METHOD_UTS:
			resolvedAddr.SetInet1Port( (WORD)(utsPort) );
			break;

		case SONORK_NET_MAP_F_METHOD_HOST:
			resolvedAddr.SetInet1(
				SONORK_PHYS_ADDR_TCP_1
				, hostAddr.GetInet1Addr()
				, (WORD)(utsPort) );
			break;

		case SONORK_NET_MAP_F_METHOD_MAPPER:
			resolvedAddr.SetInet1(
				SONORK_PHYS_ADDR_TCP_1
				, MAP->mapperHost.CStr()
				, (WORD)(utsPort) );
			break;

		}
		if( resolved )
		if( (shouldUseSocks =MAP->UseSocks()) != false )
		if( (useDefaultSocks=!MAP->OverrideSocks()) == false )
		{
			if(socksAddr.SetInet1(
				SONORK_PHYS_ADDR_TCP_1
				, MAP->socksHost.CStr()
				, MAP->SocksPort()) != 0)
			{
				// Invalid socks
				shouldUseSocks=false;
			}

		}

	}
		
	if( resolved )
	{
		resolvedAddr.SetType( SONORK_PHYS_ADDR_TCP_1 );
		if(shouldUseSocks && useDefaultSocks)
		{
			// We should use socks, if available.
			// Check that a default socks server has been
			// configured in the network panel.

			// The network panel allows the user to specify if
			// the SOCKS should be used when connecting to the
			// server or when connecting to another user.
			// So depending on the type of connection, the
			// use of SOCKS may be enabled or disabled.
			/*
			FIX THIS!!: We need to add support for SOCKS
			if(win32.server_profile.socks.TestFlag(SONORK_SAF_ENABLED))
			{
				socksAddr.Set(win32.socksPhysAddr );
			}
			*/
		}
	}
	else
	{
		resolvedAddr.Clear();
	}
	return resolved;
}


// -----------------------------------------------------------------------------

TSonorkApiFileSession*
	TSonorkApi::CreateFileSession(fnSonorkApiFile*sessionProc,DWORD sessionTag)
{
	TSonorkApiFileSession* session;
	TSonorkPhysAddr	socksAddr;
	TSonorkPhysAddr	targetAddr;
	UINT	errCode;
	session = new TSonorkApiFileSession(sessionProc,sessionTag);

	if( !client->GetServerService(SONORK_SERVICE_TYPE_SONORK_FILE,0,session->serviceLocus))
	{
		app.ERR.Set(SONORK_RESULT_NO_DATA,"No servers available",0,true);
		delete session;
		return NULL;
	}
	if( !ResolveServiceAddress(
		  targetAddr	// Resolved addr
		, socksAddr
		, session->serviceLocus.locus.userLocusEx.sidFlags
		, session->serviceLocus.locus.userLocusEx.utsAddr
		, session->serviceLocus.locus.userLocusEx.hostAddr
		, true	// Connecting to server
		))
	{
		app.ERR.SetSys(SONORK_RESULT_NETWORK_ERROR
			,GSS_NOUTSLNK
			,0);
		delete session;
		return NULL;

	}
	session->tcp.SetSocksInfo(&socksAddr);
	errCode = session->tcp.Connect( targetAddr );
	if( errCode == 0)
	{
		app.ERR.SetOk();
		session->netStatus=session->tcp.Status();
		session->txPhase = SONORK_API_FILE_PHASE_CONNECTING;
		user.data.GetLocus(&session->senderLocus);
		session->senderPin.Set(client->SidPin());
		return session;
	}
	else
	{
		app.ERR.Set(SONORK_RESULT_NETWORK_ERROR,"Connect() failed",errCode,true);
		delete session;
		return NULL;
	}
}

