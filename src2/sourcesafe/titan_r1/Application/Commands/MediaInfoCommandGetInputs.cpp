#include "MediaInfoCommandGetInputs.h"

#include "EMInputRepository.h"
#include "EMMediaSystemInspector.h"

MediaInfoCommandGetInputs::MediaInfoCommandGetInputs()
{
}

void* MediaInfoCommandGetInputs::ExecuteE(void* p_upMediaType, void*, void*)
{
	//cout << "MediaInfoCommandGetInputs" << endl;
	EMMediaType* epType = static_cast<EMMediaType*>(p_upMediaType);
	EMInputRepository* opInputs = EMInputRepository::Instance();
	
	m_oReturnValue.clear();

	opInputs -> LockContainer();
	
	try
	{
		opInputs -> Rewind();
		while(opInputs -> Current() != NULL)
		{
			if((opInputs -> Current() -> GetType() & *epType) > 0)
			{
				int32 vID = opInputs -> Current() -> GetID();
				m_oReturnValue.push_back(vID);
			}
			opInputs -> Next();
		}
	}
	catch(...)
	{
		EMDebugger("ERROR! Exception in MediaInfoCommandGetInputs::ExecuteE");
	}
	opInputs -> UnlockContainer();

	return static_cast<void*>(&m_oReturnValue);
}

bool MediaInfoCommandGetInputs::RequiresParameters()
{
	return true;
}

bool MediaInfoCommandGetInputs::IsUndoable()
{
	return false;
}
