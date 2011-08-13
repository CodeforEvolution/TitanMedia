#include "srk_defs.h"
#pragma hdrstop
#include "srk_ipc.h"
#include "srk_codec_io.h"
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

// ----------------------------------------------------------------------------
// NOTE: This is the core engine of the IPC module and is provided just for
//	 those who are curious about how the IPC works internally.
//	 However, there is no need to understand this module (srk_ipc.cpp)
//	 in order to use it.
// ----------------------------------------------------------------------------

#define SONORK_IPC_LOCK_TIMEOUT		60000	//1000
#define SONORK_IPC_MESSAGE_TIMEOUT	60000	//1500
#define SONORK_IPC_MESSAGE		(WM_APP+1)
static ATOM	ipcWinClassAtom=NULL;
static const char *ipcWinClassName="SonorkIpcClient";

// ----------------------------------------------------------------------------
// Send Files
SONORK_RESULT
 TSonorkIpc::SendFiles(const TSonorkId&		userId
                	, const TSonorkDynStringQueue&	queue)
{
	TSonorkListIterator	I;
        TSonorkDynString*	dynString;


	if( !Lock(SONORK_IPC_LOCK_REQ) )
        	return LastResult();
	dta.client.reqBlock->params.v[0]=queue.Items();
	dta.client.reqBlock->params.v[1]=0;
	dta.client.reqBlock->params.d[4].Set( userId );

	dta.reqData.bufferSize=queue.SumAtomsCodecSize();
	dta.reqData.bufferPtr =SONORK_MEM_ALLOC(BYTE,dta.reqData.bufferSize);
        {
        	TSonorkCodecWriter
                	CODEC(dta.reqData.bufferPtr
                        	,dta.reqData.bufferSize
                                ,SONORK_ATOM_DYN_STRING);

		queue.BeginEnum(I);
                while( (dynString=queue.EnumNext(I)) != NULL )
                {
                	CODEC.Write(dynString);
			if( !CODEC.CodecOk() )break;

		}
		queue.EndEnum(I);
                ctrl.result = CODEC.Result();
        }
        if( LastResult() == SONORK_RESULT_OK )
        {
		memcpy(dta.client.reqBlock->data
			, dta.reqData.bufferPtr
			, dta.reqData.bufferSize > dta.blockDataSize
                        	? dta.blockDataSize
                                : dta.reqData.bufferSize
                        );
        	dta.client.reqBlock->dataSize = dta.reqData.bufferSize;
		SendIpcMsg( SONORK_IPC_FUNCTION_SEND_FILES );
	}
	SONORK_MEM_FREE( dta.reqData.bufferPtr );
        dta.reqData.bufferPtr	= NULL;

        Unlock(SONORK_IPC_LOCK_REQ);
	return LastResult();
}
// ----------------------------------------------------------------------------
SONORK_RESULT
 TSonorkIpc::RecvFile(const TSonorkId&		userId
			, const TSonorkCCacheMark&	lineMarker
			, TSonorkShortString&		path)
{
	if( !Lock(SONORK_IPC_LOCK_REQ) )
		return LastResult();
	dta.client.reqBlock->params.d[0].Set(userId);
	dta.client.reqBlock->params.v[2]=0;
	dta.client.reqBlock->params.v[8]=sizeof(TSonorkCCacheMark);
	dta.client.reqBlock->dataSize=0;
	memcpy(&dta.client.reqBlock->params.v[16],&lineMarker,sizeof(TSonorkCCacheMark));
	lstrcpyn((char*)dta.client.reqBlock->data
		,path.CStr()
		,dta.blockDataSize);
	SendIpcMsg( SONORK_IPC_FUNCTION_RECV_FILE );
	if( LastResult() == SONORK_RESULT_OK )
	{
		dta.client.reqBlock->data[dta.blockDataSize-1]=0;
		path.Set((char*)dta.client.reqBlock->data);
	}
	Unlock(SONORK_IPC_LOCK_REQ);
	return LastResult();
}

// ----------------------------------------------------------------------------
// Services
SONORK_RESULT
 TSonorkIpc::OpenServiceCircuit(
			  DWORD			sourceInstance
			, TSonorkServiceCircuit*circuit
			, DWORD 		userParam
			, DWORD 		userFlags
			, DWORD			timeoutMsecs
			, const void*		dataPtr
			, DWORD			dataSize
			, SONORK_DWORD2*	circuitTag)
{
	if( Lock(SONORK_IPC_LOCK_REQ) )
	{
		dta.client.reqBlock->params.v[0]=sourceInstance;
		dta.client.reqBlock->params.v[1]=0;
		dta.client.reqBlock->params.v[2]=0;
		dta.client.reqBlock->params.v[3]=userParam;
		dta.client.reqBlock->params.v[4]=userFlags;
		dta.client.reqBlock->params.v[5]=timeoutMsecs;
		if( circuitTag )
		{
			dta.client.reqBlock->params.v[6]=circuitTag->v[0];
			dta.client.reqBlock->params.v[7]=circuitTag->v[1];
		}
		else
		{
			dta.client.reqBlock->params.v[6]=
			dta.client.reqBlock->params.v[7]=0;
		}
		dta.client.reqBlock->params.v[8]=sizeof(TSonorkServiceCircuit);
		dta.client.reqBlock->params.v[9]=0;
		memcpy(&dta.client.reqBlock->params.v[10]
			,circuit
			,sizeof(TSonorkServiceCircuit));

		if(dataPtr==NULL)
			dataSize=0;

		dta.reqData.bufferSize = dta.client.reqBlock->dataSize = dataSize;
		if(dataSize > 0 )
		{
			// Copy the first block of data
			if( dataSize > dta.blockDataSize )
			{
				// If data does not fit, load the <reqData> items
				// in order to process the GET_REQ_DATA
				// message the application will send to us

				dta.reqData.bufferPtr   = (BYTE*)dataPtr;
				// Can load only <blockDataSize> bytes
				dataSize=dta.blockDataSize;
			}
			memcpy(dta.client.reqBlock->data
				, dataPtr
				, dataSize );
		}
		SendIpcMsg( SONORK_IPC_FUNCTION_OPEN_SERVICE_CIRCUIT );
		dta.reqData.bufferPtr	= NULL;
		if( LastResult() == SONORK_RESULT_OK )
		{
			memcpy(circuit
				,&dta.client.reqBlock->params.v[10]
				,sizeof(TSonorkServiceCircuit)
				);
		}
		Unlock(SONORK_IPC_LOCK_REQ);
	}
	return LastResult();
}

SONORK_RESULT
 TSonorkIpc::OpenServiceCircuit(
			  DWORD			sourceInstance
			, TSonorkServiceCircuit*circuit
			, DWORD 		userParam
			, DWORD 		userFlags
			, DWORD			timeoutMsecs
			, const TSonorkCodecAtom *atom
			, SONORK_DWORD2*	circuitTag)
{
	TSonorkDynData	DD;
	ctrl.result=atom->CODEC_Write(&DD);
	if(ctrl.result!=SONORK_RESULT_OK)
		return LastResult();
	return	OpenServiceCircuit(
			  sourceInstance
			, circuit
			, userParam
			, userFlags
			, timeoutMsecs
			, DD.Buffer()
			, DD.DataSize()
			, circuitTag);
}

// ----------------------------------------------------------------------------
// Must load circuit->systemId with the desired
// circuit_id before calling.
SONORK_RESULT
 TSonorkIpc::AcceptServiceCircuit(
			  DWORD			oldSourceInstance
			, DWORD			newSourceInstance
			, TSonorkServiceCircuit*circuit)
{
	if( Lock(SONORK_IPC_LOCK_REQ) )
	{
		dta.client.reqBlock->params.v[0]=oldSourceInstance;
		dta.client.reqBlock->params.v[1]=newSourceInstance;
		dta.client.reqBlock->params.v[2]=sizeof(TSonorkServiceCircuit);
		dta.client.reqBlock->params.v[3]=0;
		memcpy(&dta.client.reqBlock->params.v[6]
			,circuit
			,sizeof(TSonorkServiceCircuit));

		SendIpcMsg( SONORK_IPC_FUNCTION_ACCEPT_SERVICE_CIRCUIT );

		memcpy(circuit
			,&dta.client.reqBlock->params.v[6]
			,sizeof(TSonorkServiceCircuit));

		Unlock(SONORK_IPC_LOCK_REQ);
	}
	return LastResult();
}
// ----------------------------------------------------------------------------
// Must load circuit->systemId with the desired
// circuit_id before calling.

SONORK_RESULT
 TSonorkIpc::GetServiceCircuitHandle(
			  DWORD 		 sourceInstance
			, TSonorkServiceCircuit* circuit)
{
	if( Lock(SONORK_IPC_LOCK_REQ) )
	{
		dta.client.reqBlock->params.v[0]=sourceInstance;
		dta.client.reqBlock->params.v[1]=0;
		dta.client.reqBlock->params.v[2]=sizeof(TSonorkServiceCircuit);
		dta.client.reqBlock->params.v[3]=0;

		memcpy(&dta.client.reqBlock->params.v[6]
			,circuit
			,sizeof(TSonorkServiceCircuit));

		SendIpcMsg( SONORK_IPC_FUNCTION_GET_SERVICE_CIRCUIT );

		memcpy(circuit
			,&dta.client.reqBlock->params.v[6]
			,sizeof(TSonorkServiceCircuit));

		Unlock(SONORK_IPC_LOCK_REQ);
	}
	return LastResult();
}
// ----------------------------------------------------------------------------
SONORK_RESULT
 TSonorkIpc::CloseServiceCircuit(
 			  DWORD			sourceInstance
			, DWORD			circuitId
			, SONORK_RESULT		result)
{
	if( Lock(SONORK_IPC_LOCK_REQ) )
	{
		dta.client.reqBlock->params.v[0]=sourceInstance;
		dta.client.reqBlock->params.v[1]=circuitId;
		dta.client.reqBlock->params.v[2]=result;
		dta.client.reqBlock->params.v[3]=0;
		SendIpcMsg( SONORK_IPC_FUNCTION_CLOSE_SERVICE_CIRCUIT );
		Unlock(SONORK_IPC_LOCK_REQ);
	}
	return LastResult();
}
// ----------------------------------------------------------------------------
SONORK_RESULT
 TSonorkIpc::ServiceReply(
			  SONORK_CTRLMSG_CMD 	cmd
			, DWORD 		cmdParam
			, DWORD 		cmdFlags
			, const void*		dataPtr
			, DWORD			dataSize)
{
	if( Lock(SONORK_IPC_LOCK_REQ) )
	{
		dta.client.reqBlock->params.v[0]=0;
		dta.client.reqBlock->params.v[1]=0;
		dta.client.reqBlock->params.v[2]=cmd;
		dta.client.reqBlock->params.v[3]=cmdParam;
		dta.client.reqBlock->params.v[4]=cmdFlags;
		dta.client.reqBlock->params.v[5]=0;

		if(dataPtr==NULL)
			dataSize=0;

		dta.reqData.bufferSize = dta.client.reqBlock->dataSize = dataSize;
		if(dataSize > 0 )
		{
			// Copy the first block of data
			if( dataSize > dta.blockDataSize )
			{
				// If data does not fit, load the <reqData> items
				// in order to process the GET_REQ_DATA
				// message the application will send to us

				dta.reqData.bufferPtr   = (BYTE*)dataPtr;
				// Can load only <blockDataSize> bytes
				dataSize=dta.blockDataSize;
			}
			memcpy(dta.client.reqBlock->data
				, dataPtr
				, dataSize );
		}
		SendIpcMsg( SONORK_IPC_FUNCTION_REPLY_SERVICE_QUERY );
		dta.reqData.bufferPtr	= NULL;
		Unlock(SONORK_IPC_LOCK_REQ);
	}
	return LastResult();
}

// ----------------------------------------------------------------------------
SONORK_RESULT
 TSonorkIpc::ServiceReply(
			  SONORK_CTRLMSG_CMD 	cmd
			, DWORD 		cmdParam
			, DWORD 		cmdFlags
			, const TSonorkCodecAtom*atom)
{
	TSonorkDynData	DD;
	ctrl.result=atom->CODEC_Write(&DD);
	if(ctrl.result!=SONORK_RESULT_OK)
		return LastResult();
	return	ServiceReply(
			  cmd
			, cmdParam
			, cmdFlags
			, DD.Buffer()
			, DD.DataSize());
}

// ----------------------------------------------------------------------------

SONORK_RESULT
 TSonorkIpc::StartServiceQuery(
			  DWORD			sourceInstance
			, TSonorkServiceQuery*	queryHandle
			, SONORK_CTRLMSG_CMD 	cmd
			, DWORD 		userParam
			, DWORD 		userFlags
			, DWORD			timeoutMsecs
			, const void*		dataPtr
			, DWORD			dataSize
			, SONORK_DWORD2*	queryTag)
{

	if( Lock(SONORK_IPC_LOCK_REQ) )
	{
		dta.client.reqBlock->params.v[0]=sourceInstance;
		dta.client.reqBlock->params.v[1]=0;
		dta.client.reqBlock->params.v[2]=cmd;
		dta.client.reqBlock->params.v[3]=userParam;
		dta.client.reqBlock->params.v[4]=userFlags;
		dta.client.reqBlock->params.v[5]=timeoutMsecs;
		if(queryTag)
		{
			dta.client.reqBlock->params.v[6]=queryTag->v[0];
			dta.client.reqBlock->params.v[7]=queryTag->v[1];
		}
		else
		{
			dta.client.reqBlock->params.v[6]=
			dta.client.reqBlock->params.v[7]=0;
		}
		dta.client.reqBlock->params.v[8]=sizeof(TSonorkServiceQuery);
		dta.client.reqBlock->params.v[9]=0;
		memcpy(&dta.client.reqBlock->params.v[10]
			,queryHandle
			,sizeof(TSonorkServiceQuery));

		if(dataPtr==NULL)
			dataSize=0;

		dta.reqData.bufferSize = dta.client.reqBlock->dataSize = dataSize;
		if(dataSize > 0 )
		{
			// Copy the first block of data
			if( dataSize > dta.blockDataSize )
			{
				// If data does not fit, load the <reqData> items
				// in order to process the GET_REQ_DATA
				// message the application will send to us

				dta.reqData.bufferPtr   = (BYTE*)dataPtr;
				// Can load only <blockDataSize> bytes
				dataSize=dta.blockDataSize;
			}
			memcpy(dta.client.reqBlock->data
				, dataPtr
				, dataSize );
		}
		SendIpcMsg( SONORK_IPC_FUNCTION_START_SERVICE_QUERY );
		dta.reqData.bufferPtr	= NULL;

		memcpy(queryHandle
			,&dta.client.reqBlock->params.v[10]
			,sizeof(TSonorkServiceQuery));

		Unlock(SONORK_IPC_LOCK_REQ);
	}
	return LastResult();

}

// ----------------------------------------------------------------------------
SONORK_RESULT
 TSonorkIpc::PokeServiceData(
			  DWORD			sourceInstance
			, TSonorkServiceHandle*	targetService
			, SONORK_CTRLMSG_CMD 	cmd
			, DWORD 		userParam
			, DWORD 		userFlags
			, const TSonorkCodecAtom* atom)
{
	TSonorkDynData	DD;
	ctrl.result=atom->CODEC_Write(&DD);
	if(ctrl.result!=SONORK_RESULT_OK)
		return LastResult();
	return	PokeServiceData(
			  sourceInstance
			, targetService
			, cmd
			, userParam
			, userFlags
			, DD.Buffer()
			, DD.DataSize());
}

// ----------------------------------------------------------------------------

SONORK_RESULT
 TSonorkIpc::PokeServiceData(
			  DWORD			sourceInstance
			, TSonorkServiceHandle*	targetService
			, SONORK_CTRLMSG_CMD 	cmd
			, DWORD 		userParam
			, DWORD 		userFlags
			, const void*		dataPtr
			, DWORD			dataSize)
{
	if( Lock(SONORK_IPC_LOCK_REQ) )
	{
		dta.client.reqBlock->params.v[0]=sourceInstance;
		dta.client.reqBlock->params.v[1]=0;
		dta.client.reqBlock->params.v[2]=cmd;
		dta.client.reqBlock->params.v[3]=userParam;
		dta.client.reqBlock->params.v[4]=userFlags;
		dta.client.reqBlock->params.v[5]=0;
		dta.client.reqBlock->params.v[8]=sizeof(TSonorkServiceHandle);
		dta.client.reqBlock->params.v[9]=0;
		memcpy(&dta.client.reqBlock->params.v[10]
			,targetService
			,sizeof(TSonorkServiceQuery));

		if(dataPtr==NULL)
			dataSize=0;

		dta.reqData.bufferSize = dta.client.reqBlock->dataSize = dataSize;
		if(dataSize > 0 )
		{
			// Copy the first block of data
			if( dataSize > dta.blockDataSize )
			{
				// If data does not fit, load the <reqData> items
				// in order to process the GET_REQ_DATA
				// message the application will send to us

				dta.reqData.bufferPtr   = (BYTE*)dataPtr;
				// Can load only <blockDataSize> bytes
				dataSize=dta.blockDataSize;
			}
			memcpy(dta.client.reqBlock->data
				, dataPtr
				, dataSize );
		}
		SendIpcMsg( SONORK_IPC_FUNCTION_POKE_SERVICE_DATA );
		dta.reqData.bufferPtr	= NULL;
		Unlock(SONORK_IPC_LOCK_REQ);
	}
	return LastResult();

}


// ----------------------------------------------------------------------------

SONORK_RESULT
 TSonorkIpc::RegisterService(
		  SONORK_APP_SERVICE_TYPE	svcType
		, TSonorkServiceInstanceInfo*   svcInfo
		, DWORD		 		svcTag
		, SKIN_ICON			svcIcon)
{
	if( Lock(SONORK_IPC_LOCK_REQ) )
	{
		dta.client.reqBlock->params.v[0]=svcType;
		dta.client.reqBlock->params.v[1]=svcTag;
		dta.client.reqBlock->params.v[2]=svcIcon;
		dta.client.reqBlock->params.v[3]=0;
		dta.client.reqBlock->dataSize=sizeof(TSonorkServiceInstanceInfo);
		memcpy(dta.client.reqBlock->data
			, svcInfo
			, sizeof(TSonorkServiceInstanceInfo));
		SendIpcMsg( SONORK_IPC_FUNCTION_REGISTER_SERVICE );
		if( LastResult() == SONORK_RESULT_OK )
		{
			memcpy(svcInfo
				, dta.client.reqBlock->data
				, sizeof(TSonorkServiceInstanceInfo));
		}
		Unlock(SONORK_IPC_LOCK_REQ);
	}
	return LastResult();
}

// ----------------------------------------------------------------------------

SONORK_RESULT
 TSonorkIpc::UnregisterService(
		  DWORD			instanceId
		, BOOL 			invokeCallbacks
		, SONORK_SERVICE_ID 	serviceId)
{
	if( Lock(SONORK_IPC_LOCK_REQ) )
	{
		dta.client.reqBlock->params.v[0]=instanceId;
		dta.client.reqBlock->params.v[1]=invokeCallbacks;
		dta.client.reqBlock->params.v[2]=serviceId;
		dta.client.reqBlock->params.v[3]=0;
		dta.client.reqBlock->dataSize=sizeof(TSonorkServiceInstanceInfo);
		SendIpcMsg( SONORK_IPC_FUNCTION_UNREGISTER_SERVICE );
		Unlock(SONORK_IPC_LOCK_REQ);
	}
	return LastResult();
}

// ----------------------------------------------------------------------------

SONORK_RESULT
 TSonorkIpc::SendMsg(
			TSonorkMsgHandle*		msgHandle
			, const TSonorkUserHandle*	userHandle
			, const TSonorkMsg* 		msg
			, const TSonorkMsgTarget*	target
			)
{
	BYTE *ptr;
	if( Lock(SONORK_IPC_LOCK_REQ) )
	{
		dta.client.reqBlock->params.v[0]=sizeof(TSonorkMsgHandle);
		dta.client.reqBlock->params.v[1]=sizeof(TSonorkUserHandle);
		dta.client.reqBlock->params.v[2]=target?sizeof(TSonorkMsgTarget):0;
		dta.client.reqBlock->params.v[3]=0;

		ptr=(BYTE*)&dta.client.reqBlock->params.v[10];
		memcpy(ptr,msgHandle,sizeof(TSonorkMsgHandle));
		ptr+=sizeof(TSonorkMsgHandle);
		memcpy(ptr,userHandle,sizeof(TSonorkUserHandle));
		ptr+=sizeof(TSonorkUserHandle);
		if( target )
			memcpy(ptr,target,sizeof(TSonorkMsgTarget));

		dta.reqData.bufferSize=msg->CODEC_Size();
		if( dta.reqData.bufferSize > dta.blockDataSize )
		{
			// The message does not fit in a single block:
			// Allocate a temporal buffer and write the portion
			// that fits. The application will send back
			// SONORK_IPC_FUNCTION_GET_REQ_DATA messages
			// until the whole buffer is retrieved.

			dta.reqData.bufferPtr=new BYTE[dta.reqData.bufferSize];

			ctrl.result=msg->CODEC_WriteMem(dta.reqData.bufferPtr
					,dta.reqData.bufferSize);

		}
		else
		{
			// The message fits in a single block:
			// write to the block without allocating the temporal buffer.
			ctrl.result=msg->CODEC_WriteMem(dta.client.reqBlock->data
					,dta.reqData.bufferSize);

		}
		if( ctrl.result== SONORK_RESULT_OK)
		{
			dta.client.reqBlock->dataSize=dta.reqData.bufferSize;
			SendIpcMsg( SONORK_IPC_FUNCTION_SEND_MSG );
			if( LastResult() == SONORK_RESULT_OK )
			{
				memcpy(msgHandle
					, &dta.client.reqBlock->params.v[10]
					, sizeof(TSonorkMsgHandle));
			}
		}
		if(dta.reqData.bufferPtr!=NULL)
		{
			delete[] dta.reqData.bufferPtr;
			dta.reqData.bufferPtr=NULL;
		}
		Unlock(SONORK_IPC_LOCK_REQ);
	}
	return LastResult();
}


// ----------------------------------------------------------------------------

SONORK_RESULT
 TSonorkIpc::PrepareMsg(
			  TSonorkMsgHandle*	handle
			, TSonorkMsg* 		msg
			, DWORD 		sysFlags
			, DWORD 		usrFlags
			, DWORD 		pcFlags
			, SONORK_SERVICE_ID     dataServiceId
			, SONORK_SERVICE_TYPE	dataServiceType
			, DWORD 		replyTrackingNo
			)
{
	if( Lock(SONORK_IPC_LOCK_REQ) )
	{
		dta.client.reqBlock->params.v[0]=sysFlags;
		dta.client.reqBlock->params.v[1]=usrFlags;
		dta.client.reqBlock->params.v[2]=pcFlags;
		dta.client.reqBlock->params.v[3]=dataServiceId;
		dta.client.reqBlock->params.v[4]=dataServiceType;
		dta.client.reqBlock->params.v[5]=replyTrackingNo;
		dta.client.reqBlock->params.v[6]=sizeof(TSonorkMsgHandle);
		dta.client.reqBlock->params.v[7]=0;
		dta.client.reqBlock->dataSize=sizeof(TSonorkMsgHeader);
		memcpy(dta.client.reqBlock->data
			, &msg->header
			, sizeof(TSonorkMsgHeader));
		SendIpcMsg( SONORK_IPC_FUNCTION_PREPARE_MSG );
		if( LastResult() == SONORK_RESULT_OK )
		{
			memcpy(handle
				, &dta.client.reqBlock->params.v[4]
				, sizeof(TSonorkMsgHandle));
				
			memcpy(&msg->header
				, dta.client.reqBlock->data
				, sizeof(TSonorkMsgHeader));
		}
		Unlock(SONORK_IPC_LOCK_REQ);
	}
	return LastResult();
}

// ----------------------------------------------------------------------------

SONORK_RESULT
  TSonorkIpc::GetUserData(TSonorkExtUserData* UD)
{
	TSrkExtUserDataAtom	atom(UD);
	if( Lock(SONORK_IPC_LOCK_REQ) )
	{
		dta.client.reqBlock->dataSize=0;
		dta.client.reqBlock->params.d[0]=UD->userId;
		SendIpcMsg( SONORK_IPC_FUNCTION_GET_USER_DATA );
		if( LastResult() == SONORK_RESULT_OK )
		{
			ctrl.result=atom.CODEC_ReadMem(
				dta.client.reqBlock->data
				, dta.client.reqBlock->dataSize);
		}
		Unlock(SONORK_IPC_LOCK_REQ);

	}
	return LastResult();

}

// ----------------------------------------------------------------------------

SONORK_RESULT
  TSonorkIpc::GetUserHandle(TSonorkUserHandle* handle)
{
	if( Lock(SONORK_IPC_LOCK_REQ) )
	{
		dta.client.reqBlock->params.v[0]=0;
		dta.client.reqBlock->dataSize=sizeof(TSonorkUserHandle);
		memcpy(dta.client.reqBlock->data,handle,sizeof(TSonorkUserHandle));
		SendIpcMsg( SONORK_IPC_FUNCTION_GET_USER_HANDLE );
		if( LastResult() == SONORK_RESULT_OK )
		{
			memcpy(handle,dta.client.reqBlock->data,sizeof(TSonorkUserHandle));
		}
		else
		{
			handle->sid.Clear();
			handle->utsLinkId=SONORK_INVALID_LINK_ID;
		}
		Unlock(SONORK_IPC_LOCK_REQ);

	}
	return LastResult();
}


// ----------------------------------------------------------------------------

SONORK_RESULT
  TSonorkIpc::EnumUserGroups(TSonorkIpcDataCallback*callback,void*param)
{
	if( Lock(SONORK_IPC_LOCK_REQ) )
	{
		dta.reqData.function	= SONORK_IPC_FUNCTION_ENUM_USER_GROUPS;
		dta.reqData.callbackPtr	= callback;
		dta.reqData.callbackData= param;
		dta.client.reqBlock->dataSize=0;
		dta.client.reqBlock->params.v[0]=0;
		SendIpcMsg( dta.reqData.function );
		dta.reqData.function	= SONORK_IPC_FUNCTION_NONE;

		Unlock(SONORK_IPC_LOCK_REQ);
	}
	return LastResult();
}

// ----------------------------------------------------------------------------

SONORK_RESULT
  TSonorkIpc::EnumUserList(TSonorkIpcDataCallback*callback,void*param)
{
	if( Lock(SONORK_IPC_LOCK_REQ) )
	{
		dta.reqData.function	= SONORK_IPC_FUNCTION_ENUM_USER_LIST;
		dta.reqData.callbackPtr	= callback;
		dta.reqData.callbackData= param;
		dta.client.reqBlock->dataSize=0;
		dta.client.reqBlock->params.v[0]=0;
		SendIpcMsg( dta.reqData.function );
		dta.reqData.function	= SONORK_IPC_FUNCTION_NONE;

		Unlock(SONORK_IPC_LOCK_REQ);
	}
	return LastResult();
}

// ----------------------------------------------------------------------------

LRESULT
 TSonorkIpc::OnGetReqData(DWORD offset)
{
	DWORD	blockSize;

	sonork_printf(">OnGetReqData(%u/%u)",offset,dta.reqData.bufferSize);
	// if one of these conditions are met, it means
	//  something is VERY wrong .
	if( dta.reqData.bufferPtr ==NULL || offset  > dta.reqData.bufferSize )
	{
		sonork_printf("<INVALID!>");
		return 0;
	}

	if( offset + dta.blockDataSize > dta.reqData.bufferSize )
		blockSize = dta.reqData.bufferSize - offset;
	else
		blockSize = dta.blockDataSize;

	dta.client.reqBlock->dataSize=blockSize;
	memcpy(dta.client.reqBlock->data
		, dta.reqData.bufferPtr + offset
		, blockSize);
	sonork_printf("<OnGetReqData(%u+%u)=%u",offset,blockSize,offset+blockSize);
	return 1;

}

// ----------------------------------------------------------------------------

LRESULT
 TSonorkIpc::OnGetEvtData(DWORD offset)
{
	DWORD	blockSize;
//	sonork_printf(">OnGetEvtData(%u/%u)",offset,dta.evtData.bufferSize);

	// if one of these conditions are met, it means
	//  something is VERY wrong .
	if( dta.evtData.bufferPtr==NULL || offset  > dta.evtData.bufferSize )
	 {
		sonork_printf("<INVALID!>");
		return 0;
	 }

	if( offset + dta.blockDataSize > dta.evtData.bufferSize )
		blockSize = dta.evtData.bufferSize - offset;
	else
		blockSize = dta.blockDataSize;

	dta.client.evtBlock->dataSize=blockSize;
	memcpy(dta.client.evtBlock->data
		, dta.evtData.bufferPtr + offset
		, blockSize);
//	sonork_printf("<OnGetEvtData(%u+%u)=%u",offset,blockSize,offset+blockSize);
	return 1;

}

// ----------------------------------------------------------------------------

LRESULT
 TSonorkIpc::OnPutReqData(DWORD dataSize)
{
	SONORK_RESULT result;
	switch( dta.reqData.function )
	{
	case SONORK_IPC_FUNCTION_ENUM_USER_GROUPS:
	{
		TSonorkGroup	  group;
		result=group.CODEC_ReadMem(dta.client.reqBlock->data,dataSize);
		if( result == SONORK_RESULT_OK )
		{
			dta.reqData.callbackPtr(dta.reqData.callbackData,&group);
		}
	}
	break;

	case SONORK_IPC_FUNCTION_ENUM_USER_LIST:
	{
		TSonorkExtUserData	userData(SONORK_USER_TYPE_UNKNOWN);
		TSrkExtUserDataAtom	atom(&userData);
		result=atom.CODEC_ReadMem(dta.client.reqBlock->data,dataSize);
		if( result == SONORK_RESULT_OK )
		{
			dta.reqData.callbackPtr(dta.reqData.callbackData,&userData);
		}
	}
	break;

	}

	return 0L;
}

// ----------------------------------------------------------------------------

LRESULT
 TSonorkIpc::OnPutEvtData(DWORD blockSize)
{
	sonork_printf("OnPutEvtData(%u)",blockSize);
	switch( cb.eventId )
	{
	case SONORK_IPC_EVENT_APP_EVENT:

		// Check the type of appEvent
		if( cb.eventParam!=SONORK_APP_EVENT_SONORK_MSG_RCVD
		 && cb.eventParam!=SONORK_APP_EVENT_SONORK_MSG_SENDING )
		{
			sonork_printf("OnPutEvtData::Invalid state");
			return false;
		}

		// if #1: pre-check
		if(blockSize==0 || dta.evtData.loadedDataSize==(DWORD)-1)
		{
			sonork_printf("OnPutEvtData::Precheck fail");
			return false;
		}

		// if loadedDataSize==0, this is the first block
		//  Allocate the buffer
		if(dta.evtData.loadedDataSize == 0 )
		{
			dta.evtData.bufferSize=dta.client.evtBlock->dataSize;
			// data size can't be 0, otherwise why send us a message
			// telling us there is data?
			if( dta.evtData.bufferSize == 0)
			{
				// First block initialization failed, set
				// <dataSize> to -1 to avoid re-entering this
				// section (by if #1 )
				dta.evtData.loadedDataSize=(DWORD)-1;
				sonork_printf("OnPutEvtData::dataSize==0");
				return false;
			}

			dta.evtData.bufferPtr=new BYTE[dta.evtData.bufferSize];
		}
		if(dta.evtData.loadedDataSize+blockSize > dta.evtData.bufferSize
		|| blockSize > dta.blockDataSize)
		{
			dta.evtData.loadedDataSize=(DWORD)-1;
			sonork_printf("OnPutEvtData::Overflow");
			return false;
		}

		memcpy(dta.evtData.bufferPtr+dta.evtData.loadedDataSize
			, dta.client.evtBlock->data
			, blockSize);
		dta.evtData.loadedDataSize+=blockSize;
		return true;

	case SONORK_IPC_EVENT_SERVICE_EVENT:

		// if #1: pre-check
		if(blockSize==0 || dta.evtData.loadedDataSize==(DWORD)-1 )
			return false;
		if( dta.evtData.loadedDataSize < dta.evtData.bufferSize )
		{
			if(dta.evtData.loadedDataSize + blockSize > dta.evtData.bufferSize )
				blockSize=dta.evtData.bufferSize-dta.evtData.loadedDataSize;

			memcpy(dta.evtData.bufferPtr+dta.evtData.loadedDataSize
				, dta.client.evtBlock->data
				, blockSize);
			dta.evtData.loadedDataSize+=blockSize;
		}
		return true;

	}

	return 0L;
}

// ----------------------------------------------------------------------------

SONORK_RESULT
 TSonorkIpc::GetCurrentAppMessage(TSonorkMsg* msg)
{
	if( !(ctrl.flags&SONORK_ICF_IN_CALLBACK)
	||  (cb.eventId!=SONORK_IPC_EVENT_APP_EVENT)
	||  (cb.eventParam!=SONORK_APP_EVENT_SONORK_MSG_RCVD&&cb.eventParam!=SONORK_APP_EVENT_SONORK_MSG_SENDING))
	{
		return SONORK_RESULT_INVALID_OPERATION;
	}
	if( Lock(SONORK_IPC_LOCK_EVT) )
	{
		dta.evtData.bufferPtr=NULL;
		dta.evtData.bufferSize=	dta.evtData.loadedDataSize= 0;
		dta.client.evtBlock->dataSize=0;
		dta.client.evtBlock->params.v[0]=0;
		SendIpcMsg( SONORK_IPC_FUNCTION_GET_EVENT_DATA );
		if( LastResult() == SONORK_RESULT_OK )
		{
			if( dta.evtData.loadedDataSize==(DWORD)-1)
			{
				ctrl.result=SONORK_RESULT_INTERNAL_ERROR;
			}
			else
			if( dta.evtData.bufferPtr==NULL )
			{
				ctrl.result=SONORK_RESULT_NO_DATA;
			}
			else
			{
				ctrl.result=msg->CODEC_ReadMem(
					 dta.evtData.bufferPtr
					,dta.evtData.loadedDataSize);
			}
		}
		Unlock(SONORK_IPC_LOCK_EVT);
		if( dta.evtData.bufferPtr!=NULL )
			delete[] dta.evtData.bufferPtr;
	}
	return LastResult();
}

// ----------------------------------------------------------------------------

SONORK_RESULT
 TSonorkIpc::SetServerData(TSonorkUserServer* userver)
{
	if( !(ctrl.flags&SONORK_ICF_IN_CALLBACK)
	||  cb.eventId!=SONORK_IPC_EVENT_SERVICE_EVENT
	||  cb.eventParam!=SONORK_APP_SERVICE_EVENT_GET_SERVER_DATA)
	{
		ctrl.result=SONORK_RESULT_INVALID_OPERATION;
		return LastResult();
	}
	if( Lock(SONORK_IPC_LOCK_EVT) )
	{
		dta.evtData.bufferSize=userver->CODEC_Size();
		if( dta.evtData.bufferSize > dta.blockDataSize )
		{
			// The data  does not fit in a single block:
			// Allocate a temporal buffer and write the portion
			// that fits. The application will send back
			// SONORK_IPC_FUNCTION_GET_REQ_DATA messages
			// until the whole buffer is retrieved.

			dta.evtData.bufferPtr=new BYTE[dta.evtData.bufferSize];
			ctrl.result=userver->CODEC_WriteMem(dta.evtData.bufferPtr,dta.evtData.bufferSize);
			if(ctrl.result==SONORK_RESULT_OK)
			{
				memcpy(dta.client.evtBlock->data
					,dta.evtData.bufferPtr
					,dta.blockDataSize);
			}

		}
		else
		{
			// The data fits in a single block:
			// write to the block without allocating the temporal buffer.
			dta.evtData.bufferPtr=NULL;
			ctrl.result=userver->CODEC_WriteMem(dta.client.evtBlock->data,dta.evtData.bufferSize);

		}
		if( ctrl.result== SONORK_RESULT_OK)
		{
			dta.client.evtBlock->dataSize=dta.evtData.bufferSize;
			SendIpcMsg( SONORK_IPC_FUNCTION_SET_SERVER_DATA );
		}
		if(dta.evtData.bufferPtr!=NULL)
		{
			delete[] dta.evtData.bufferPtr;
			dta.evtData.bufferPtr=NULL;
		}
		Unlock(SONORK_IPC_LOCK_EVT) ;
	}
	return LastResult();
}

// ----------------------------------------------------------------------------

SONORK_RESULT
 TSonorkIpc::GetServiceDataAtom(TSonorkCodecAtom* A)
{
	TSonorkDynData	DD;
	if(GetServiceData(&DD)==SONORK_RESULT_OK)
	{
		ctrl.result = A->CODEC_Read(&DD);
	}
	return LastResult();
}


// ----------------------------------------------------------------------------
SONORK_RESULT
 TSonorkIpc::GetServiceData(TSonorkDynData* dynDataPtr)
{
	if( !(ctrl.flags&SONORK_ICF_IN_CALLBACK)
	||  cb.eventId!=SONORK_IPC_EVENT_SERVICE_EVENT
	||  cb.eventDataSize==0)
	{
		ctrl.result=SONORK_RESULT_INVALID_OPERATION;
		return LastResult();
	}
	dynDataPtr->SetDataSize(
		  cb.eventDataSize
		, true			// adjust buffer
		);
	return GetServiceData(dynDataPtr->wBuffer() , cb.eventDataSize );

}

// ----------------------------------------------------------------------------

SONORK_RESULT
 TSonorkIpc::GetServiceData(BYTE*bufferPtr, DWORD bufferSize)
{
	if( !(ctrl.flags&SONORK_ICF_IN_CALLBACK)
	||  cb.eventId!=SONORK_IPC_EVENT_SERVICE_EVENT
	||  cb.eventDataSize==0)
	{
		ctrl.result=SONORK_RESULT_INVALID_OPERATION;
		return LastResult();
	}
	if( bufferPtr==NULL  || bufferSize==0)
	{
		ctrl.result=SONORK_RESULT_INVALID_PARAMETER;
		return LastResult();
	}
	if(bufferSize > cb.eventDataSize )
		bufferSize=cb.eventDataSize;
	if( bufferSize <= dta.blockDataSize )
	{
		// we've got all the data in the buffer:
		// no need to invoke the application
		memcpy(bufferPtr
			,dta.server.block->data
			,bufferSize);
		ctrl.result=SONORK_RESULT_OK;
		return LastResult();
	}
	// Load first block
	memcpy(bufferPtr
		,dta.server.block->data
		,dta.blockDataSize);
	if( Lock(SONORK_IPC_LOCK_EVT) )
	{
		dta.evtData.loadedDataSize	=dta.blockDataSize;
		dta.evtData.bufferPtr		=bufferPtr;
		dta.evtData.bufferSize		=bufferSize;
		// param 1: Loaded data
		dta.client.evtBlock->params.v[0]=dta.evtData.loadedDataSize;
		// param 2: buffer size
		dta.client.evtBlock->params.v[1]=bufferSize;
		// param 3 & data size: zero (0)
		dta.client.evtBlock->params.v[2]=
		dta.client.evtBlock->dataSize	=0;
		SendIpcMsg( SONORK_IPC_FUNCTION_GET_EVENT_DATA );
		Unlock(SONORK_IPC_LOCK_EVT);
		dta.evtData.bufferSize		=0;
	}
	return LastResult();
}

// ----------------------------------------------------------------------------

LRESULT
 TSonorkIpc::OnServiceEvent(SONORK_APP_SERVICE_EVENT eventId)
{
	BYTE*	ptr;
	LRESULT		eventResult;
	cb.eventId    = SONORK_IPC_EVENT_SERVICE_EVENT;
	cb.eventParam = eventId;
	switch(eventId)
	{
		default:
		case SONORK_APP_SERVICE_EVENT_NONE:
			return 0;

		case SONORK_APP_SERVICE_EVENT_EXPORT:
			cb.event.svcEvent.setExport.value=dta.server.block->params.v[4];
			cb.eventDataSize=0;
			break;

		case SONORK_APP_SERVICE_EVENT_BUSY:
			cb.event.svcEvent.setBusy.value=dta.server.block->params.v[4];
			cb.eventDataSize=0;
			break;

		case SONORK_APP_SERVICE_EVENT_GET_NAME:
			cb.event.svcEvent.getName.str=new TSonorkShortString;
			cb.eventDataSize=0;
			break;

		case SONORK_APP_SERVICE_EVENT_GET_SERVER_DATA:
			// params.v[4] : IsRemoteQuery
			if(dta.server.block->params.v[4]==false)
			{
				// Not remote: No message nor requesting service
				cb.event.svcEvent.msgInfo.serviceHandle=NULL;
				cb.event.svcEvent.msgInfo.ctrlMsg=NULL;
				cb.eventDataSize=
				cb.event.svcEvent.msgInfo.dataSize=0;
				break;
			}

		// Is remote: There is a message and service
		// no break , fall through to POKE_DATA,QUERY_REQ,etc..
		case SONORK_APP_SERVICE_EVENT_POKE_DATA:
		case SONORK_APP_SERVICE_EVENT_QUERY_REQ:
		case SONORK_APP_SERVICE_EVENT_QUERY_AKN:
		case SONORK_APP_SERVICE_EVENT_CIRCUIT_DATA:

			ptr=dta.server.block->params.b;

			ptr+=dta.server.block->params.v[8];
			cb.event.svcEvent.msgInfo.serviceHandle=
				(TSonorkServiceHandle*)ptr;

			ptr+=dta.server.block->params.v[9];
			cb.event.svcEvent.msgInfo.ctrlMsg=
				(TSonorkCtrlMsg*)ptr;

			cb.eventDataSize=
			cb.event.svcEvent.msgInfo.dataSize=
				dta.server.block->dataSize;
			break;

		case SONORK_APP_SERVICE_EVENT_QUERY_END:
		case SONORK_APP_SERVICE_EVENT_CIRCUIT_CLOSED:
			cb.event.svcEvent.close.systemId=dta.server.block->params.v[4];
			cb.event.svcEvent.close.result=(SONORK_RESULT)dta.server.block->params.v[5];
			cb.eventDataSize=0;
			break;

		case SONORK_APP_SERVICE_EVENT_CIRCUIT_REQ:
		case SONORK_APP_SERVICE_EVENT_CIRCUIT_OPEN:
		case SONORK_APP_SERVICE_EVENT_CIRCUIT_UPDATE:
			cb.eventDataSize=0;
			break;
	}
	cb.event.svcEvent.handlerTag=dta.server.block->params.v[0];
	cb.event.svcEvent.serviceInstance=dta.server.block->params.v[1];
	cb.event.svcEvent.eventTag=(SONORK_DWORD2*)&dta.server.block->params.v[2];
	eventResult = InvokeCallback();
	switch(eventId)
	{
		case SONORK_APP_SERVICE_EVENT_GET_NAME:
			if( eventResult )
			{
				dta.server.block->dataSize=
					cb.event.svcEvent.getName.str->Length();

				if(dta.server.block->dataSize>dta.blockDataSize)
					dta.server.block->dataSize=dta.blockDataSize;

				memcpy(dta.server.block->data
					, cb.event.svcEvent.getName.str->CStr()
					, dta.server.block->dataSize);
				dta.server.block->data[dta.server.block->dataSize]=0;

			}
			delete cb.event.svcEvent.getName.str;
			break;

	}
	return eventResult;
}
// ----------------------------------------------------------------------------
LRESULT
 TSonorkIpc::OnAppEvent(SONORK_APP_EVENT eventId)
{
	cb.eventId    = SONORK_IPC_EVENT_APP_EVENT;
	cb.eventParam = eventId;
	switch( eventId )
	{
		default:
		case SONORK_APP_EVENT_OLD_CTRL_MSG:
		case SONORK_APP_EVENT_NONE:
		case SONORK_APP_EVENT_DESKTOP_SIZE_CHANGE:
		case SONORK_APP_EVENT_START_WAPP:
		case SONORK_APP_EVENT_USER_MTPL_CHANGE:
		case SONORK_APP_EVENT_SONORK_MSG_QUERY_LOCK:
		case SONORK_APP_EVENT_SYS_DIALOG:
		case SONORK_APP_EVENT_MAIN_VIEW_SELECTION:
			return 0;	// Not mapped /unknown to IPC


		case SONORK_APP_EVENT_OPEN_PROFILE:
		case SONORK_APP_EVENT_SHUTDOWN:
		case SONORK_APP_EVENT_MAINTENANCE:
		case SONORK_APP_EVENT_SET_PROFILE:
		case SONORK_APP_EVENT_SKIN_COLOR_CHANGED:
		case SONORK_APP_EVENT_CX_STATUS:
			break;

		case SONORK_APP_EVENT_FLUSH_BUFFERS:
			cb.event.appEvent.flushBuffers.msecs = dta.server.block->params.v[0];
			break;

		case SONORK_APP_EVENT_SONORK_MSG_RCVD:
		case SONORK_APP_EVENT_SONORK_MSG_SENDING:
		case SONORK_APP_EVENT_SONORK_MSG_SENT:
			memcpy(&cb.event.appEvent.msg
				,&dta.server.block->params.v[8]
				,sizeof(DWORD)*5);
			cb.event.appEvent.msg.mark=
				(TSonorkCCacheMark*)&dta.server.block->params.v[16];
			cb.event.appEvent.msg.header=
				(TSonorkMsgHeader*)dta.server.block->data;
			break;

		case SONORK_APP_EVENT_SONORK_MSG_PROCESSED:
			memcpy(&cb.event.appEvent.msgProcessed
				,&dta.server.block->params.v[8]
				,sizeof(DWORD)*6);
			cb.event.appEvent.msgProcessed.mark=
				(TSonorkCCacheMark*)&dta.server.block->params.v[16];
			break;


		case SONORK_APP_EVENT_SET_LANGUAGE:
			cb.event.appEvent.setLanguage.code = dta.server.block->params.v[0];
			break;

		case SONORK_APP_EVENT_SID_CHANGED:
			cb.event.appEvent.sidChanged.sidFlags.v[0].Set(dta.server.block->params.d[4]);
			cb.event.appEvent.sidChanged.sidFlags.v[1].Set(dta.server.block->params.d[5]);
			cb.event.appEvent.sidChanged.userAddr=(TSonorkNewUserAddr*)dta.server.block->data;
			break;

		case SONORK_APP_EVENT_USER_SID:
			cb.event.appEvent.userSid.type=dta.server.block->params.v[0];
			cb.event.appEvent.userSid.userId.Set(dta.server.block->params.d[4]);
			if(cb.event.appEvent.userSid.type==SONORK_APP_EVENT_USER_SID_LOCAL)
			{
				cb.event.appEvent.userSid.local.oldSidMode=
					dta.server.block->params.d[5].v[0];
					
				cb.event.appEvent.userSid.local.onlineDir=(int)
					dta.server.block->params.d[5].v[1];
				memcpy(&cb.event.appEvent.userSid.local.userAddr
					,&dta.server.block->params.d[6]
					,sizeof(cb.event.appEvent.userSid.local.userAddr));

			}
			else
			if(cb.event.appEvent.userSid.type==SONORK_APP_EVENT_USER_SID_REMOTE)
			{
				cb.event.appEvent.userSid.remote.sid.Set(
					dta.server.block->params.d[5]);

				cb.event.appEvent.userSid.remote.sidFlags.v[0].Set(
					dta.server.block->params.d[6]);

				cb.event.appEvent.userSid.remote.sidFlags.v[1].Set(
					dta.server.block->params.d[7]);

			}
			else
				return 0;
			break;
			
		case SONORK_APP_EVENT_USER_UTS:
			cb.event.appEvent.userUts.utsLinkId=dta.server.block->params.v[0];
			cb.event.appEvent.userUts.userId.Set(dta.server.block->params.d[4]);
			break;

		case SONORK_APP_EVENT_SET_USER:
			cb.event.appEvent.setUser.flags=dta.server.block->params.v[0];
			cb.event.appEvent.setUser.userId.Set(dta.server.block->params.d[4]);
			break;

		case SONORK_APP_EVENT_DEL_USER:
			cb.event.appEvent.delUser.userId.Set(dta.server.block->params.d[4]);
			break;
	}
	return InvokeCallback();
}

// ----------------------------------------------------------------------------
SONORK_RESULT
 TSonorkIpc::SetEventMask(DWORD mask)
{
	if( Lock(SONORK_IPC_LOCK_REQ) )
	{
		dta.client.reqBlock->params.v[0]=mask;
		dta.client.reqBlock->dataSize=0;
		SendIpcMsg( SONORK_IPC_FUNCTION_SET_EVENT_MASK );
		Unlock(SONORK_IPC_LOCK_REQ);
	}
	return LastResult();

}
// ----------------------------------------------------------------------------
BOOL
 TSonorkIpc::Lock(SONORK_IPC_LOCK lock)
{
	if( dta.client.hwnd )
	{
		if( lock == SONORK_IPC_LOCK_REQ && (ctrl.flags&SONORK_ICF_IN_CALLBACK) )
		{
			ctrl.result=SONORK_RESULT_SERVICE_BUSY;
		}
		else
		if(!(ctrl.flags&lock))
		{
			ctrl.flags|=lock;
			return true;
		}
		else
		{
			ctrl.result=SONORK_RESULT_SERVICE_BUSY;
		}
	}
	else
		ctrl.result=SONORK_RESULT_NOT_READY;
	return false;
}

// ----------------------------------------------------------------------------
void
 TSonorkIpc::Unlock(SONORK_IPC_LOCK lock)
{
	assert(ctrl.flags&lock);
	ctrl.flags&=~lock;
}

// ----------------------------------------------------------------------------
BOOL
 TSonorkIpc::SendIpcMsg(SONORK_IPC_FUNCTION function)

{
	BOOL rv;

	// and, of course, the IPC must have been initialized
	assert( dta.server.hwnd!=NULL && dta.client.hwnd != NULL);

	rv = SendMessageTimeout(
		  dta.server.hwnd
		, dta.server.msg
		, function
		, ctrl.ipcId
		, SMTO_NORMAL
		, SONORK_IPC_MESSAGE_TIMEOUT
		, &ctrl.result);

	if( !rv )ctrl.result = SONORK_RESULT_TIMEOUT;

	return rv;
}
// ----------------------------------------------------------------------------
LRESULT CALLBACK
 TSonorkIpc::WinProc( HWND hwnd , UINT uMsg , WPARAM wParam , LPARAM lParam)
{
	static TSonorkIpc*_this;
	switch(uMsg)
	{

	case SONORK_IPC_MESSAGE:
		_this=(TSonorkIpc*)GetWindowLong(hwnd,GWL_USERDATA);
		if(!_this)return 0;
		switch(wParam)
		{
		case SONORK_IPC_FUNCTION_APP_EVENT:
			return _this->OnAppEvent((SONORK_APP_EVENT)lParam);
			
		case SONORK_IPC_FUNCTION_PUT_REQ_DATA:
			return _this->OnPutReqData(lParam);

		case SONORK_IPC_FUNCTION_GET_REQ_DATA:
			return _this->OnGetReqData(lParam);

		case SONORK_IPC_FUNCTION_GET_EVT_DATA:
			return _this->OnGetEvtData(lParam);

		case SONORK_IPC_FUNCTION_PUT_EVT_DATA:
			return _this->OnPutEvtData(lParam);

		case SONORK_IPC_FUNCTION_SERVICE_EVENT:
			return _this->OnServiceEvent((SONORK_APP_SERVICE_EVENT)lParam);
		default:
			break;
		}
		break;

	case WM_DESTROY:
		_this=(TSonorkIpc*)GetWindowLong(hwnd,GWL_USERDATA);
		if(_this)
		if(_this->dta.client.hwnd == hwnd )
		{
			_this->dta.client.hwnd =NULL;
			_this->Shutdown();
		}
		break;
		
	}
	return	::DefWindowProc( hwnd , uMsg, wParam, lParam );


}

// ----------------------------------------------------------------------------
DWORD
 TSonorkIpc::InvokeCallback()
{
	DWORD	rv;
	if( !(ctrl.flags&SONORK_ICF_IN_CALLBACK) )
	{
		ctrl.flags|=SONORK_ICF_IN_CALLBACK;
		rv=cb.ptr(cb.param
			,cb.eventId
			,cb.eventParam
			,&cb.event);
		ctrl.flags&=~SONORK_ICF_IN_CALLBACK;
		return rv;
	}
	return 0;

}
// ----------------------------------------------------------------------------
TSonorkIpc::TSonorkIpc()
{
	memset(&ctrl,0,sizeof(ctrl));
	memset(&dta,0,sizeof(dta));

}
// ----------------------------------------------------------------------------
TSonorkIpc::~TSonorkIpc()
{
	Shutdown();
}
// ----------------------------------------------------------------------------
SONORK_RESULT
 TSonorkIpc::Startup(	 HWND 			ownerHwnd
			,SONORK_SERVICE_ID	serviceId
			,const char *		configName
			,TSonorkIpcMainCallback*cbPtr
			,void* 			cbParam
			,DWORD			eventMask)

{
	char tmp[128];
	BYTE	*ptr;
	WNDCLASS 	winclass;
	HINSTANCE	hInstance;

	ctrl.result   = SONORK_RESULT_INVALID_HANDLE;
	ctrl.ownerHwnd=ownerHwnd;
	cb.ptr 	= cbPtr;
	cb.param= cbParam;
	memset(&dta,0,sizeof(dta));

	hInstance =(HINSTANCE)GetWindowLong(ownerHwnd,GWL_HINSTANCE);
	sprintf(tmp,"sonorkipclock%s",configName);
	SONORK_StrToLower(tmp);
	for(;;)
	{
		dta.server.mutex = OpenMutex(MUTEX_ALL_ACCESS,false,tmp);
		if( dta.server.mutex == NULL )
			break;

		sprintf(tmp,"sonorkipcdata%s",configName);
		SONORK_StrToLower(tmp);
		dta.server.handle = OpenFileMapping(FILE_MAP_ALL_ACCESS
				,false
				,tmp);
		if( dta.server.handle == NULL)
			break;

		ptr = (BYTE*)MapViewOfFile(
			 dta.server.handle
			,FILE_MAP_ALL_ACCESS
			,0,0,0);
		if(ptr == NULL )
			break;

		dta.server.header =  (TSonorkIpcFileHeader*)(ptr);
		dta.server.block  =  (TSonorkIpcFileBlock*)(ptr+dta.server.header->blockOffset );
		dta.server.hwnd	  =  (HWND)dta.server.header->serverHwnd;
		if(!IsWindow(dta.server.hwnd))
			break;

		dta.server.msg		=dta.server.header->serverMsg;
		dta.blockDataSize	=dta.server.header->blockDataSize;
		dta.blockFullSize	=dta.server.header->blockFullSize;
		if( ipcWinClassAtom == NULL )
		{
			winclass.style=0;
			winclass.cbClsExtra=
			winclass.cbWndExtra=0;
			winclass.hInstance=hInstance;
			winclass.hIcon=NULL;
			winclass.hCursor=NULL;
			winclass.hbrBackground=NULL;
			winclass.lpszMenuName=NULL;
			winclass.lpszClassName=ipcWinClassName;
			winclass.lpfnWndProc=WinProc;
			ipcWinClassAtom=RegisterClass(&winclass);
			if(ipcWinClassAtom==NULL)
			{
				ctrl.result=SONORK_RESULT_INTERNAL_ERROR;
				break;
			}
		}
		dta.client.hwnd=
			CreateWindow(ipcWinClassName
				, configName
				, WS_CHILD | WS_MINIMIZE
				, 0, 0 , 200 , 100
				, ownerHwnd
				, NULL
				, hInstance
				, (void*)this);
		if(dta.client.hwnd==NULL)
		{
			ctrl.result=SONORK_RESULT_INTERNAL_ERROR;
			break;
		}
		SetWindowLong(dta.client.hwnd , GWL_USERDATA , (LONG)this);


		if( !LockServer() )
		{
			ctrl.result=SONORK_RESULT_TIMEOUT;
			break;
		}
		dta.server.header->link.req.clientHwnd = (DWORD)dta.client.hwnd;
		dta.server.header->link.req.clientMsg  = SONORK_IPC_MESSAGE;
		dta.server.header->link.req.eventMask  = eventMask;
		ctrl.ipcId = serviceId;
		SendIpcMsg(SONORK_IPC_FUNCTION_LINK);
		if(ctrl.result==SONORK_RESULT_OK)
		{
			ctrl.ipcId    = dta.server.header->link.akn.ipcId;
			ctrl.ipcFlags = dta.server.header->link.akn.ipcFlags;
			ctrl.serviceId= serviceId;
		}
		else
		{
			ctrl.ipcId = 0;
		}
		UnlockServer();

		if(ctrl.result==SONORK_RESULT_OK && ctrl.ipcId==0 )
		{
			ctrl.result=SONORK_RESULT_INTERNAL_ERROR;
			break;
		}

		sprintf(tmp,"sonorkipcdata%s%08x",configName,ctrl.ipcId);
		SONORK_StrToLower(tmp);
		dta.client.handle = OpenFileMapping(FILE_MAP_ALL_ACCESS
				,false
				,tmp);
		if( dta.client.handle == NULL)
		{
			ctrl.result=SONORK_RESULT_STORAGE_ERROR;
			break;
		}
		ptr = (BYTE*)MapViewOfFile(
			 dta.client.handle
			,FILE_MAP_ALL_ACCESS
			,0,0,0);
		if(ptr == NULL )
		{
			ctrl.result=SONORK_RESULT_STORAGE_ERROR;
			break;
		}
		dta.client.reqBlock = (TSonorkIpcFileBlock*)(ptr + dta.blockFullSize * 0);
		dta.client.evtBlock = (TSonorkIpcFileBlock*)(ptr + dta.blockFullSize * 1);

		break;
	}
	if( ctrl.result != SONORK_RESULT_OK)
	{
		SONORK_RESULT	result;
		//Save result , in case Shutdown() changes it
		result=(SONORK_RESULT)ctrl.result;

		Shutdown();

		// restore result
		ctrl.result = result;
	}
	return LastResult();
}
// ----------------------------------------------------------------------------
void
 TSonorkIpc::Shutdown()
{
	if( ctrl.flags & SONORK_ICF_STOPPING)
		return;
	ctrl.flags|=SONORK_ICF_STOPPING;
	if( ctrl.ipcId != 0 )
	{
		if(Lock(SONORK_IPC_LOCK_REQ))
		{
			SendIpcMsg(SONORK_IPC_FUNCTION_UNLINK);
			Unlock(SONORK_IPC_LOCK_REQ);
			ctrl.ipcId=0;
		}
	}

	if( dta.client.hwnd !=NULL )
	{
		DestroyWindow(dta.client.hwnd);
	}
	
	if(dta.server.mutex)
	{
		if(dta.server.handle)
		{
			if(dta.server.header)
			{
				UnmapViewOfFile(dta.server.header);
			}
			CloseHandle(dta.server.handle);
		}
		if(dta.client.handle)
		{
			if(dta.client.reqBlock)
			{
				UnmapViewOfFile(dta.client.reqBlock);
			}
			CloseHandle(dta.client.handle);
		}
		CloseHandle(dta.server.mutex);
	}
	ctrl.ownerHwnd=NULL;
	ctrl.ipcId=
	ctrl.ipcFlags=
	ctrl.flags=0;
	memset(&dta,0,sizeof(dta));
}
// ----------------------------------------------------------------------------
BOOL
 TSonorkIpc::LockServer()
{
	DWORD aux;
	aux=WaitForSingleObject(dta.server.mutex ,SONORK_IPC_LOCK_TIMEOUT);
	if( aux !=WAIT_ABANDONED && aux != WAIT_OBJECT_0)
	{
		ctrl.result=SONORK_RESULT_TIMEOUT;
		return false;
	}
	return true;
}
// ----------------------------------------------------------------------------
void
 TSonorkIpc::UnlockServer()
{
	ReleaseMutex(dta.server.mutex);

}

// ----------------------------------------------------------------------------

SONORK_IPC_SLOT_HANDLE
  TSonorkIpc::AllocSharedSlot(DWORD param,SONORK_IPC_ALLOC_METHOD method, SONORK_SERVICE_ID serviceId)
{
	BYTE	*slotPtr;
	if(dta.slot.handle != NULL )
	{
		ctrl.result=SONORK_RESULT_OUT_OF_RESOURCES;
		return NULL;
	}
	if( serviceId == SONORK_SERVICE_ID_NONE )
		serviceId = ServiceId();
	if( Lock(SONORK_IPC_LOCK_REQ) )
	{
		dta.client.reqBlock->dataSize=0;
		dta.client.reqBlock->params.v[0]=serviceId;
		dta.client.reqBlock->params.v[1]=param;
		dta.client.reqBlock->params.v[2]=method;
		dta.client.reqBlock->params.v[4]=0;
		SendIpcMsg( SONORK_IPC_FUNCTION_ALLOC_SHARED_SLOT );
		if( LastResult() == SONORK_RESULT_OK )
		{
			dta.slot.handle = (SONORK_IPC_SLOT_HANDLE)dta.client.reqBlock->params.v[0];
			slotPtr=(BYTE*)dta.server.header;
			slotPtr+=dta.server.header->sharedSlotsOffset ;
			slotPtr+=dta.client.reqBlock->params.v[1];
			dta.slot.ptr    = (TSonorkIpcSharedSlot*)slotPtr;
		}
		Unlock(SONORK_IPC_LOCK_REQ);

	}
	return dta.slot.handle;
}

// ----------------------------------------------------------------------------

SONORK_RESULT
  TSonorkIpc::FreeSharedSlot(SONORK_IPC_SLOT_HANDLE slotHandle)
{
	if(dta.slot.handle == NULL || slotHandle != dta.slot.handle )
	{
		ctrl.result=SONORK_RESULT_INVALID_HANDLE;
	}
	else
	{
		if( Lock(SONORK_IPC_LOCK_REQ) )
		{
			dta.client.reqBlock->dataSize=0;
			dta.client.reqBlock->params.v[0]=(DWORD)dta.slot.handle;
			dta.client.reqBlock->params.v[1]=0;
			SendIpcMsg( SONORK_IPC_FUNCTION_FREE_SHARED_SLOT );
			if( LastResult() == SONORK_RESULT_OK )
			{
				dta.slot.handle = NULL;
				dta.slot.ptr    = NULL;
			}
			Unlock(SONORK_IPC_LOCK_REQ);
		}
	}
	return LastResult();
}

// ----------------------------------------------------------------------------

DWORD
  TSonorkIpc::GetSharedSlotValue(SONORK_IPC_SLOT_HANDLE slotHandle
			,SONORK_IPC_SHARED_SLOT_INDEX slotIndex)
{
	DWORD	slotValue;
	if(dta.slot.handle == NULL || slotHandle != dta.slot.handle )
	{
		ctrl.result=SONORK_RESULT_INVALID_HANDLE;
		slotValue=(DWORD)-1;
	}
	else
	if(slotIndex<0 || slotIndex>=SONORK_IPC_SHARED_SLOT_INDEXES)
	{
		ctrl.result=SONORK_RESULT_INVALID_PARAMETER;
		slotValue=(DWORD)-1;
	}
	else
	if( LockServer() )
	{
		ctrl.result=SONORK_RESULT_OK;
		slotValue = dta.slot.ptr->data[slotIndex];
		UnlockServer();
	}
	else
		slotValue=(DWORD)-1;
	return slotValue;
}

// ----------------------------------------------------------------------------

SONORK_RESULT
  TSonorkIpc::SetSharedSlotValue(SONORK_IPC_SLOT_HANDLE slotHandle
			,SONORK_IPC_SHARED_SLOT_INDEX   slotIndex
			,DWORD				slotValue)
{
	if(dta.slot.handle == NULL || slotHandle != dta.slot.handle )
	{
		ctrl.result=SONORK_RESULT_INVALID_HANDLE;
	}
	else
	if(slotIndex<0 || slotIndex>=SONORK_IPC_SHARED_SLOT_INDEXES)
	{
		ctrl.result=SONORK_RESULT_INVALID_PARAMETER;
	}
	else
	if( LockServer() )
	{
		ctrl.result=SONORK_RESULT_OK;
		dta.slot.ptr->data[slotIndex] = slotValue;
		UnlockServer();
	}
	return LastResult();
}

// ----------------------------------------------------------------------------

