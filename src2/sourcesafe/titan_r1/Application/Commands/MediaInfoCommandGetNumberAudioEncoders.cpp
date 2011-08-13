#include "MediaInfoCommandGetNumberAudioEncoders.h"

#include "EMMediaEngine.h"
#include "EMEncoderRepository.h"
#include "EMRealtimeEncoderDescriptor.h"
#include "EMWinMediaSingletonMuxer.h"
#include "EMWinQuickTimeWriterSuper.h"
#include <list>

MediaInfoCommandGetNumberAudioEncoders::MediaInfoCommandGetNumberAudioEncoders()
{
}

void* MediaInfoCommandGetNumberAudioEncoders::ExecuteE(void*, void*, void*)
{
	m_vNumAudioEnc = 0;

	if(EMWinMediaSingletonMuxer::Instance() ->m_vCurrentlyUsedFamily == EM_FAMILY_QUICKTIME)
	{
		m_vNumAudioEnc = EMWinQuickTimeWriterSuper::Instance() -> GetNumberOfAudioEncoders();
		return static_cast<void*>(&m_vNumAudioEnc);
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
				m_vNumAudioEnc++;

			opOutputs -> Next();
		}
	}
	catch(...)
	{
		EMDebugger("ERROR! Exception in MediaInfoCommandGetAudioRenderOutput::ExecuteE");
	}

	opOutputs -> UnlockContainer();

	return static_cast<void*>(&m_vNumAudioEnc);
}

bool MediaInfoCommandGetNumberAudioEncoders::RequiresParameters()
{
	return true;
}

bool MediaInfoCommandGetNumberAudioEncoders::IsUndoable()
{
	return false;
}