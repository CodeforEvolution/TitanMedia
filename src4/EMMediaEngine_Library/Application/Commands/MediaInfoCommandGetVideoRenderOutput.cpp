#include "MediaInfoCommandGetVideoRenderOutput.h"

#include "EMOutputRepository.h"

MediaInfoCommandGetVideoRenderOutput::MediaInfoCommandGetVideoRenderOutput()
{
}

void* MediaInfoCommandGetVideoRenderOutput::ExecuteE(void*, void*, void*)
{
	//cout << "MediaInfoCommandGetVideoRenderOutput" << endl;
	EMOutputRepository* opOutputs = EMOutputRepository::Instance();
	opOutputs -> LockContainer();
	try
	{
		opOutputs -> Rewind();
		while(opOutputs -> Current() != NULL)
		{
			if(opOutputs -> Current() -> IsRenderOutput() && (opOutputs -> Current() -> GetType() & EM_TYPE_ANY_VIDEO) > 0)
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
		EMDebugger("ERROR! Exception in MediaInfoCommandGetVideoRenderOutput::ExecuteE");
	}
	opOutputs -> UnlockContainer();
	EMDebugger("ERROR! No video rendering output present!");
	
	m_vReturnValue = -1;
	return static_cast<void*>(&m_vReturnValue);	
}

bool MediaInfoCommandGetVideoRenderOutput::RequiresParameters()
{
	return true;
}

bool MediaInfoCommandGetVideoRenderOutput::IsUndoable()
{
	return false;
}
