#include "MediaInfoCommandGetOutputs.h"

#include "EMOutputRepository.h"

MediaInfoCommandGetOutputs::MediaInfoCommandGetOutputs()
{
}

MediaInfoCommandGetOutputs::~MediaInfoCommandGetOutputs()
{
}

void* MediaInfoCommandGetOutputs::ExecuteE(void* p_upMediaType, void*, void*)
{
	//eo << "MediaInfoCommandGetOutputs" << ef;
	EMMediaType* epType = static_cast<EMMediaType*>(p_upMediaType);
	EMOutputRepository* opOutputs = EMOutputRepository::Instance();
	
	m_oReturnValue.clear();

	opOutputs -> LockContainer();
	
	try
	{
		if(((*epType) & EM_TYPE_MIDI) > 0)
			int a = 1;
		opOutputs -> Rewind();
		uint64 vSize = opOutputs -> Size();
		while(opOutputs -> Current() != NULL)
		{
			string oName = opOutputs -> Current() -> GetName();
			if((opOutputs -> Current() -> GetType() & *epType) > 0 && ! opOutputs -> Current() -> IsRenderOutput())
			{
				int32 vID = opOutputs -> Current() -> GetID();
				m_oReturnValue.push_back(vID);
			}
			opOutputs -> Next();
		}
	}
	catch(...)
	{
		EMDebugger("ERROR! Exception in MediaInfoCommandGetOutputs::ExecuteE");
	}
	opOutputs -> UnlockContainer(); 

	return static_cast<void*>(&m_oReturnValue);
}

bool MediaInfoCommandGetOutputs::RequiresParameters()
{
	return true;
}

bool MediaInfoCommandGetOutputs::IsUndoable()
{
	return false;
}
