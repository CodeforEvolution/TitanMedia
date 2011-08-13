#include "MediaInfoCommandGetNumberVideoAVIEncoders.h"

#include "EMMediaEngine.h"
#include "EMEncoderRepository.h"
#include "EMRealtimeEncoderDescriptor.h"
#include "EMWinQuickTimeWriterSuper.h"
#include "EMWinMediaSingletonMuxer.h"
#include <list>

MediaInfoCommandGetNumberVideoAVIEncoders::MediaInfoCommandGetNumberVideoAVIEncoders()
{
}

void* MediaInfoCommandGetNumberVideoAVIEncoders::ExecuteE(void*, void*, void*)
{
	m_vNumVideoEnc = 0;

	if(EMWinMediaSingletonMuxer::Instance() ->m_vCurrentlyUsedFamily == EM_FAMILY_QUICKTIME)
	{
		m_vNumVideoEnc = EMWinQuickTimeWriterSuper::Instance() -> GetNumberOfVideoEncoders();
		return static_cast<void*>(&m_vNumVideoEnc);
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
				m_vNumVideoEnc++;

			opOutputs -> Next();
		}
	}
	catch(...)
	{
		EMDebugger("ERROR! Exception in MediaInfoCommandGetAudioRenderOutput::ExecuteE");
	}

	opOutputs -> UnlockContainer();

	return static_cast<void*>(&m_vNumVideoEnc);
}

bool MediaInfoCommandGetNumberVideoAVIEncoders::RequiresParameters()
{
	return true;
}

bool MediaInfoCommandGetNumberVideoAVIEncoders::IsUndoable()
{
	return false;
}