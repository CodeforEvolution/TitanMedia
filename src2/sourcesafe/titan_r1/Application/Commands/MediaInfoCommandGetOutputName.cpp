#include "MediaInfoCommandGetOutputName.h"

#include "EMOutputRepository.h"

MediaInfoCommandGetOutputName::MediaInfoCommandGetOutputName()
{
}

void* MediaInfoCommandGetOutputName::ExecuteE(void* p_upOutputID, void*, void*)
{
	//eo << "MediaInfoCommandGetOutputName" << ef;
	int32* vID = static_cast<int32*>(p_upOutputID);
	int32 vTargetID = *vID;
	EMOutputRepository* opOutputs = EMOutputRepository::Instance();

	opOutputs -> LockContainer();
	try
	{
		opOutputs -> Rewind();
		while(opOutputs -> Current() != NULL)
		{
			if(opOutputs -> Current() -> GetID() == vTargetID)
			{
				string oName = opOutputs -> Current() -> GetName();
				for(int v = 0; v < oName.length() +1; m_vpReturnValue[v++] = '\0')
					;
				memcpy(m_vpReturnValue, oName.c_str(), oName.length());
				opOutputs -> UnlockContainer();
				return static_cast<void*>(m_vpReturnValue);	
			}
			opOutputs -> Next();
		}
	}
	catch(...)
	{
		EMDebugger("ERROR! Exception in MediaInfoCommandGetOutputName::ExecuteE");
	}
	opOutputs -> UnlockContainer();
	return NULL;
}

bool MediaInfoCommandGetOutputName::RequiresParameters()
{
	return true;
}

bool MediaInfoCommandGetOutputName::IsUndoable()
{
	return false;
}
