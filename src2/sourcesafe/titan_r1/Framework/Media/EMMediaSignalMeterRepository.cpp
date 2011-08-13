#include "EMMediaSignalMeterRepository.h"

#include "EMMediaIDManager.h"
#include "EMMediaSignalMeter.h"
#include "EMMediaItemContainer.h"
#include "EMMediaEngine.h"
#include "EMMediaProject.h"

EMMediaSignalMeterRepository* EMMediaSignalMeterRepository::m_opInstance = NULL;

EMMediaSignalMeterRepository* EMMediaSignalMeterRepository::Instance()
{
	EMMediaSignalMeterRepository* opInstance = NULL;
	if(m_opInstance == NULL)
		m_opInstance = EM_new EMMediaSignalMeterRepository();
	opInstance = m_opInstance;
	return opInstance;
}

void EMMediaSignalMeterRepository::Delete()
{
	delete m_opInstance;
	m_opInstance = NULL;
}

EMMediaSignalMeterRepository::EMMediaSignalMeterRepository()
	:	EMMediaItemContainer<EMMediaSignalMeter>(EM_TYPE_ANY)
{
	m_vID = EMMediaIDManager::MakeID(); 
}

EMMediaSignalMeterRepository::~EMMediaSignalMeterRepository()
{ 
}

void EMMediaSignalMeterRepository::OnItemAdd()
{
	EMMediaEngine::Instance() -> GetMediaProject() -> SetDirty(true);
}

void EMMediaSignalMeterRepository::OnItemRemove()
{
	EMMediaEngine::Instance() -> GetMediaProject() -> SetDirty(true);
}

int32 EMMediaSignalMeterRepository::GetID() const
{
	return m_vID;
}

EMMediaSignalMeter* EMMediaSignalMeterRepository::InstanceSignalMeter(EMMediaType p_eType)
{
	if((p_eType & EM_TYPE_ANY_AUDIO) > 0)
	{
		EMMediaSignalMeter* opMeter = EMMediaSignalMeter::CreateEMSignalMeter(EM_TYPE_RAW_AUDIO);
		LockContainer();
		try
		{
			Add(opMeter);
		}
		catch(...)
		{
			EMDebugger("ERROR! Caught an exception in EMMediaSignalMeterRepository::InstanceSignalMeter");
		}
		UnlockContainer();
		return opMeter;
	}
	else if((p_eType & EM_TYPE_MIDI) > 0)
	{
		EMMediaSignalMeter* opMeter = EMMediaSignalMeter::CreateEMSignalMeter(EM_TYPE_MIDI);
		LockContainer();
		try
		{
			Add(opMeter);
		}
		catch(...)
		{
			EMDebugger("ERROR! Caught an exception in EMMediaSignalMeterRepository::InstanceSignalMeter");
		}
		UnlockContainer();
		return opMeter;
	}
	else
		EMDebugger("ERROR! Signal meters only implemented for Audio and MIDI!");
	
	return NULL;
}

void EMMediaSignalMeterRepository::DeleteSignalMeter(EMMediaSignalMeter* p_opMeter)
{
	if(p_opMeter != NULL)
	{
		LockContainer();
		try
		{
			Remove(p_opMeter);
		}
		catch(...)
		{
			EMDebugger("ERROR! Caught an exception in EMMediaSignalMeterRepository::DeleteSignalMeter");
		}
		UnlockContainer();
		delete p_opMeter;
	}
}