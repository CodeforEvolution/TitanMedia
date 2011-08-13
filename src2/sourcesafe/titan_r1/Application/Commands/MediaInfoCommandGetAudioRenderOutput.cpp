#include "MediaInfoCommandGetAudioRenderOutput.h"

#include "EMOutputRepository.h"

MediaInfoCommandGetAudioRenderOutput::MediaInfoCommandGetAudioRenderOutput()
{
}

void* MediaInfoCommandGetAudioRenderOutput::ExecuteE(void*, void*, void*)
{
	//eo << "MediaInfoCommandGetAudioRenderOutput" << ef;
	EMOutputRepository* opOutputs = EMOutputRepository::Instance();
	opOutputs -> LockContainer();
	try
	{
		opOutputs -> Rewind();
		while(opOutputs -> Current() != NULL)
		{
			if(opOutputs -> Current() -> IsRenderOutput() && (opOutputs -> Current() -> GetType() & EM_TYPE_ANY_AUDIO) > 0)
			{
				m_vReturnValue = opOutputs -> Current() -> GetID();
				opOutputs -> UnlockContainer();
				return static_cast<void*>(&m_vReturnValue);	
			}
			opOutputs -> Next();
		}
	}
	catch(...)
	{
		EMDebugger("ERROR! Exception in MediaInfoCommandGetAudioRenderOutput::ExecuteE");
	}
	opOutputs -> UnlockContainer();

	EMDebugger("ERROR! No audio rendering output present!");

	m_vReturnValue = -1;
	return static_cast<void*>(&m_vReturnValue);	
}

bool MediaInfoCommandGetAudioRenderOutput::RequiresParameters()
{
	return true;
}

bool MediaInfoCommandGetAudioRenderOutput::IsUndoable()
{
	return false;
}
