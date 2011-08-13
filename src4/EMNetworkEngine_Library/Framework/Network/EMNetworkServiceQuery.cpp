#include "EMNetworkServiceQuery.h"

#include "EMNetworkEngine.h"
#include "EMNetworkServiceModule.h"
#include "EMGlobals.h"

#include <windows.h>
#include <process.h>


EMNetworkServiceQuery::EMNetworkServiceQuery() 
{
	m_oEvent = CreateEvent(NULL,false, false, NULL);
}

EMNetworkServiceQuery::~EMNetworkServiceQuery() 
{
	CloseHandle(m_oEvent);
}
	
void EMNetworkServiceQuery::Clear() 
{ 
	SetEvent(m_oEvent);
}


void EMNetworkServiceQuery::Wait() 
{
	WaitForSingleObject(m_oEvent, 15000);
}

void EMNetworkServiceQuery::WaitAndNotify() 
{
	_beginthread(Thread, 0, this);
}

void EMNetworkServiceQuery::Thread(void* p_vParam)
{
	bool vSend = false;
	EMNetworkServiceQuery* opThis = static_cast<EMNetworkServiceQuery*>(p_vParam);
	DWORD vResult;

	eo << "EMNetworkServiceQuery::Thread - started" << ef;

	try
	{
		vResult = WaitForSingleObject(opThis -> m_oEvent, 15000); 
	}
	catch(...)
	{
		vResult = WAIT_FAILED;
		eo <<"EMNetworkServiceQuery::Thread - Error: " << GetLastError() <<ef;
	}

	eo << "EMNetworkServiceQuery::Thread - Got object" << ef;

	if(vResult == WAIT_OBJECT_0 && opThis -> m_opUser -> m_oServiceInstance != -1)
	{
		eo << "EMNetworkServiceQuery::Thread - Instance recv'd!!" <<ef;
		while(!vSend)
		{
			if(EMNetworkEngine::Instance() -> GetServiceModule() -> SendServiceData(opThis -> m_opUser, "c"))
			{
				eo << "EMNetworkServiceQuery::Thread - Data sent" << ef;
				vSend = true;
			}
			else
			{
				eo << "EMNetworkServiceQuery::Thread - Data NOT sent" << ef;
			}
			Sleep(250);
		}
	}
	else
	{
		eo << "EMNetworkServiceQuery::Thread - No Instance recv!!" <<ef;
	}

	_endthread();
}



uint64 EMNetworkServiceQuery::GetId() 
{
	return m_oQuery.systemId;
}
