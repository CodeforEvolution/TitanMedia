#include "EMNetworkMessageSender.h"
#include "EMUserInfo.h"
#include "EMNetworkEngine.h"
#include "EMNetworkUserDatabase.h"

#include "sonorkvcapi.h"
#include "srk_data_types.h"

EMNetworkMessageSender::EMNetworkMessageSender()
{
}
	
EMNetworkMessageSender::~EMNetworkMessageSender()
{
}

void EMNetworkMessageSender::SendTextMessage(uint64* p_opReceivers, char* p_opText)
{
	TSonorkLocus1 locus;
	int	index;

	for(index = 0; p_opReceivers[index] != 0; ++index)
	{
		EMUserInfo* opUser = EMNetworkEngine::Instance() -> GetUserDatabase() -> Find(p_opReceivers[index]);

		if (opUser != NULL)
		{
			eo << "EMNetworkMessageSender::SendMessage - Sending to: " << opUser -> m_opName.c_str()<< " msg: " << p_opText << ef;

			opUser -> GetHandle() -> GetLocus(&locus);
			EMNetworkEngine::Instance() -> GetAPI() -> SendPlainTextMsg(&locus, p_opText, 0);
		}
	}
}


uint64 EMNetworkMessageSender::SendFileMessage(uint64 p_vReceiver, const char* p_opFile)
{
	EMUserInfo* opUser = EMNetworkEngine::Instance() -> GetUserDatabase() -> Find(p_vReceiver);

	if(opUser == NULL)
		return -1;

	eo << "EMNetworkMessageSender::SendFileMessage(" << opUser -> m_opName.c_str()  <<", " << p_opFile<<");" << ef;

	EMNetworkSendFileInfo* opInfo = EM_new EMNetworkSendFileInfo(p_opFile);

	opInfo -> m_opRemoteUser = opUser;

	if(opInfo -> Upload() == 0)
	{
		eo << "EMNetworkMessageSender::SendFileMessage(" << opUser -> m_opName.c_str()  <<", " << p_opFile<<"); - FAILED!!!!" << ef;
		delete opInfo;
		return -1;
	}
	
	Add(opInfo);

	return opInfo -> GetId();
}

void EMNetworkMessageSender::TimeSlot()
{
	LockContainer();
	Rewind();

	while(Current() != NULL)
	{
		if(Current() -> m_vPhase == EM_FILE_PHASE_DONE)
		{
			eo << "EMNetworkMessageSender::TimeSlot - Removing file: " << Current() -> m_oFileName.c_str() << ef;

			EMNetworkSendFileInfo* opInfo = Current();
			Remove(opInfo);
			delete opInfo;
			Rewind();
		}
		Next();
	}
	
	Rewind();
	while(Current() != NULL)
	{
		if(Current() -> m_opSession != NULL)
			Current() -> m_opSession -> TimeSlot();
		Next();
	}
	UnlockContainer();
}