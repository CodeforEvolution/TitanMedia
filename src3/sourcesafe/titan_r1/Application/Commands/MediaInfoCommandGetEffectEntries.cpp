#include "MediaInfoCommandGetEffectEntries.h"

#include "EMPluginRepository.h"
#include "EMPluginEntry.h"

MediaInfoCommandGetEffectEntries::MediaInfoCommandGetEffectEntries()
{
}

void* MediaInfoCommandGetEffectEntries::ExecuteE(void*, void*, void*)
{
	m_oReturnValue.clear();
	EMPluginRepository* opRep = EMPluginRepository::Instance();

	opRep -> LockContainer();
	try
	{
		opRep -> Rewind();
		while(opRep -> Current() != NULL)
		{
			if(opRep -> Current() -> GetPluginType() != EM_NATIVE_VIDEO_TRANSITION && 
				opRep -> Current() -> GetPluginType() != EM_WINDOWS_DIRECTX_VIDEO_TRANSITION)
			{
				m_oReturnValue.push_back(opRep -> Current() -> GetID());
			}
			opRep -> Next();
		}
	}
	catch(...)
	{
		EMDebugger("ERROR! Exception in MediaInfoCommandGetEffectEntries::ExecuteE");
	}
	opRep -> UnlockContainer();
	return static_cast<void*>(&m_oReturnValue);
}

bool MediaInfoCommandGetEffectEntries::RequiresParameters()
{
	return true;
}

bool MediaInfoCommandGetEffectEntries::IsUndoable()
{
	return false;
}