#include "MediaInfoCommandGetVideoAVIEncoders.h"

#include "EMMediaEngine.h"
#include "EMEncoderRepository.h"
#include "EMRealtimeEncoderDescriptor.h"
#include "EMWinQuickTimeWriterSuper.h"
#include "EMWinMediaSingletonMuxer.h"
#include <list>

MediaInfoCommandGetVideoAVIEncoders::MediaInfoCommandGetVideoAVIEncoders()
{
}

void* MediaInfoCommandGetVideoAVIEncoders::ExecuteE(void* p_upIDs, void*, void*)
{
	int cnt = 0;
	uint32* vpIDs = static_cast<uint32*>(p_upIDs);

	if(EMWinMediaSingletonMuxer::Instance() ->m_vCurrentlyUsedFamily == EM_FAMILY_QUICKTIME)
	{
		EMWinQuickTimeWriterSuper::Instance() -> GetVideoEncoderIDs(static_cast<uint32*>(vpIDs));
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
			
			if((opOutputs -> Current() ->GetType () & EM_TYPE_ANY_VIDEO) > 0)
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

bool MediaInfoCommandGetVideoAVIEncoders::RequiresParameters()
{
	return true;
}

bool MediaInfoCommandGetVideoAVIEncoders::IsUndoable()
{
	return false;
}