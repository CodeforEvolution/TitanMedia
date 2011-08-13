#include "EMGlobals.h"

#include "EMBufferDestination.h"
#include "EMMediaIDManager.h"
//#include "EMMediaSignalMeterRepository.h"
#include "EMMediaSignalMeter.h"
#include "EMSemaphore.h"

#include "EMProjectDataSaver.h"
#include "EMProjectDataLoader.h"

EMBufferDestination::EMBufferDestination(int32 p_vType, EMDestinationPosition p_ePosition, string p_oDebugDestinationName)
	:	m_opDestination(NULL),
		m_vIsPostFader(true),
		m_vType(p_vType),
		m_ePosition(p_ePosition),
		m_vProcessingReferenceCount(0),
		m_oDestinationName(p_oDebugDestinationName)
{

	m_vID = EMMediaIDManager::MakeID();
	if((m_vType & EM_TYPE_MIDI) > 0)
	{
	}
	m_opDestinationSemaphore = EMSemaphore::CreateEMSemaphore();
}

EMBufferDestination::EMBufferDestination()
{
}

EMBufferDestination::~EMBufferDestination()
{
	delete m_opDestinationSemaphore;
}

int32 EMBufferDestination::GetID() const
{
	return m_vID;
}

string EMBufferDestination::GetDestinationName() const
{
	return m_oDestinationName;
}

EMDestinationPosition EMBufferDestination::GetPosition() const
{
	return m_ePosition;
}

int32 EMBufferDestination::GetType() const
{
	return m_vType;
}

void EMBufferDestination::SetDestination(EMBufferDestination* p_opDestination)
{
	if(p_opDestination != NULL)
	{
//		m_opDestinationSemaphore -> Acquire();
		if(m_opDestination != NULL)
			m_opDestination -> RemoveProcessingReference();
		m_opDestination = p_opDestination;
		p_opDestination -> AddProcessingReference();
//		m_opDestinationSemaphore -> Release();
	}
}

EMBufferDestination* EMBufferDestination::GetDestination() const
{
//	m_opDestinationSemaphore -> Acquire();
	EMBufferDestination* opDest = m_opDestination;
//	m_opDestinationSemaphore -> Release(); 
	return opDest;
}

bool EMBufferDestination::IsPreFader() const
{
	return m_vIsPostFader;
}

void EMBufferDestination::SetPreFader(bool p_vFlag)
{
	m_vIsPostFader = p_vFlag;
}

bool EMBufferDestination::SaveData(EMProjectDataSaver* p_opSaver)
{
	p_opSaver -> SaveString("EMT");
	p_opSaver -> SaveUInt32(static_cast<uint32>(m_vID));
	p_opSaver -> SaveInt32(m_vType);
	p_opSaver -> SaveBool(m_vIsPostFader);
	p_opSaver -> SaveUInt32(static_cast<uint32>(m_ePosition));
	return true;
}

bool EMBufferDestination::LoadData(EMProjectDataLoader* p_opLoader)
{
	if(p_opLoader -> LoadString() != string("EMT"))
	{
		EMDebugger("ERROR! Error in project file!");
		return false;
	}
	m_vID = static_cast<int32>(p_opLoader -> LoadUInt32());
	m_vType = p_opLoader -> LoadInt32();	
	m_vIsPostFader = p_opLoader -> LoadBool();
	uint32 vPosition = p_opLoader -> LoadUInt32();
	if(vPosition == 0) m_ePosition = EM_CLIP;
	else if(vPosition == 1) m_ePosition = EM_TRACK;
	else if(vPosition == 2) m_ePosition = EM_EFFECT;
	else if(vPosition == 3) m_ePosition = EM_OUTPUT;
	else if(vPosition == 4) m_ePosition = EM_TRANSITION;
	else if(vPosition == 5) m_ePosition = EM_TRANSITION_TRACK;
	else
		EMDebugger("ERROR! Unknown position for destination described in project file!");
	return true;
}

void EMBufferDestination::AddProcessingReference()
{
	m_vProcessingReferenceCount++;
}

void EMBufferDestination::RemoveProcessingReference()
{
//	ASSERT(m_vProcessingReferenceCount > 0);
	if(m_vProcessingReferenceCount <= 0)
		EMDebugger("ERROR! Reference count in destination reached below zero!");
	m_vProcessingReferenceCount--;
}

uint32 EMBufferDestination::GetProcessingReferenceCount()
{
	return m_vProcessingReferenceCount;
}
