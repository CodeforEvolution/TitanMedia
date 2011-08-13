#include "MediaInfoCommandGetAudioEncoders.h"

#include "EMMediaEngine.h"
#include "EMEncoderRepository.h"
#include "EMRealtimeEncoderDescriptor.h"
#include "EMWinQuickTimeWriterSuper.h"
#include "EMWinMediaSingletonMuxer.h"

#include <list>

MediaInfoCommandGetAudioEncoders::MediaInfoCommandGetAudioEncoders()
{
}

void* MediaInfoCommandGetAudioEncoders::ExecuteE(void* p_upIDs, void*, void*)
{
	int cnt = 0;
	uint32* vpIDs = static_cast<uint32*>(p_upIDs);

	if(EMWinMediaSingletonMuxer::Instance() ->m_vCurrentlyUsedFamily == EM_FAMILY_QUICKTIME)
	{
		EMWinQuickTimeWriterSuper::Instance() -> GetAudioEncoderIDs(static_cast<uint32*>(vpIDs));
		return NULL;
	}

	EMEncoderRepository* opOutputs = EMEncoderRepository::Instance();

	try
	{
		opOutputs -> LockContainer();
		opOutputs -> Rewind();
		while(opOutputs -> Current() != NULL)
		{
			int32 vOutputID = opOutputs -> Current() -> GetID();
			
			if((opOutputs -> Current() ->GetType () & EM_TYPE_ANY_AUDIO) > 0)
				vpIDs[cnt++] = vOutputID;

			opOutputs -> Next();
		}
	}
	catch(...)
	{
		EMDebugger("ERROR! Exception in MediaInfoCommandGetAudioRenderOutput::ExecuteE");
	}

	opOutputs -> UnlockContainer();

	return NULL;
}

bool MediaInfoCommandGetAudioEncoders::RequiresParameters()
{
	return true;
}

bool MediaInfoCommandGetAudioEncoders::IsUndoable()
{
	return false;
}