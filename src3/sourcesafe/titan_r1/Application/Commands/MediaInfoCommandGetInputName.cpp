#include "MediaInfoCommandGetInputName.h"

#include "EMInputRepository.h"

MediaInfoCommandGetInputName::MediaInfoCommandGetInputName()
{
}

void* MediaInfoCommandGetInputName::ExecuteE(void* p_upInputID, void*, void*)
{
	string oString = "ERROR!";

	int32* vID = static_cast<int32*>(p_upInputID);
	EMInputRepository* opInputs = EMInputRepository::Instance();

	if(vID == NULL)
	{
		oString = "ERROR! Bad input ID!";
		for(int v = 0; v < oString.length() +1; m_vpReturnValue[v++] = '\0')
			;
		memcpy(m_vpReturnValue, oString.c_str(), oString.length());
		return static_cast<void*>(m_vpReturnValue);
	}

	opInputs -> LockContainer();
	try
	{
		opInputs -> Rewind();
		while(opInputs -> Current() != NULL)
		{
			if(opInputs -> Current() -> GetID() == *vID)
			{
				oString = opInputs -> Current() -> GetName();
				opInputs -> UnlockContainer();

				for(int v = 0; v < oString.length() +1; m_vpReturnValue[v++] = '\0')
					;
				memcpy(m_vpReturnValue, oString.c_str(), oString.length());

				return static_cast<void*>(m_vpReturnValue);	
			}
			opInputs -> Next();
		}
	}
	catch(...)
	{
		EMDebugger("ERROR! Exception in MediaInfoCommandGetInputName::ExecuteE");
	}
	opInputs -> UnlockContainer();
	for(int v = 0; v < oString.length() +1; m_vpReturnValue[v++] = '\0')
		;
	memcpy(m_vpReturnValue, oString.c_str(), oString.length());
	return static_cast<void*>(m_vpReturnValue);
}

bool MediaInfoCommandGetInputName::RequiresParameters()
{
	return true;
}

bool MediaInfoCommandGetInputName::IsUndoable()
{
	return false;
}
