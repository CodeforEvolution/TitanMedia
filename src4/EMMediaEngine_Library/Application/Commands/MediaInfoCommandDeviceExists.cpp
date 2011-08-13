#include "MediaInfoCommandDeviceExists.h"

#include "EMOutputRepository.h"
#include "EMInputRepository.h"

MediaInfoCommandDeviceExists::MediaInfoCommandDeviceExists()
{
}

MediaInfoCommandDeviceExists::~MediaInfoCommandDeviceExists()
{
}

void* MediaInfoCommandDeviceExists::ExecuteE(void* p_upMediaType, void*, void*)
{
	//eo << "MediaInfoCommandDeviceExists" << ef;
	EMMediaType* epType = static_cast<EMMediaType*>(p_upMediaType);
	EMOutputRepository* opOutputs = EMOutputRepository::Instance();
	EMInputRepository* opInputs = EMInputRepository::Instance();
	
	opOutputs -> LockContainer();
	try
	{
		opOutputs -> Rewind();
		while(opOutputs -> Current() != NULL)
		{
			if((opOutputs -> Current() -> GetType() & *epType) > 0 && ! opOutputs -> Current() -> IsRenderOutput())
			{
				m_vReturnValue = true;
				return static_cast<void*>(&m_vReturnValue);
			}
			opOutputs -> Next();
		}
	}
	catch(...)
	{
		EMDebugger("ERROR! Exception in MediaInfoCommandDeviceExists::ExecuteE");
	}
	opOutputs -> UnlockContainer(); 

	opInputs -> LockContainer();
	try
	{
		opInputs -> Rewind();
		while(opInputs -> Current() != NULL)
		{
			if((opInputs -> Current() -> GetType() & *epType) > 0)
			{
				m_vReturnValue = true;
				return static_cast<void*>(&m_vReturnValue);
			}
			opInputs -> Next();
		}
	}
	catch(...)
	{
		EMDebugger("ERROR! Exception in MediaInfoCommandDeviceExists::ExecuteE");
	}
	opInputs -> UnlockContainer(); 

	m_vReturnValue = false;
	return static_cast<void*>(&m_vReturnValue);
}

bool MediaInfoCommandDeviceExists::RequiresParameters()
{
	return true;
}

bool MediaInfoCommandDeviceExists::IsUndoable()
{
	return false;
}
