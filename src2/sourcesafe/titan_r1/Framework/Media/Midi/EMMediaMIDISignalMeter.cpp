#include "EMMediaMIDISignalMeter.h"

#include "EMMediaTimer.h"
#include "EMSemaphore.h"
#include "EMMediaIDManager.h"

EMMediaMIDISignalMeter::EMMediaMIDISignalMeter()
	:	EMMediaSignalMeter(EM_TYPE_MIDI),
		m_vLastReceiveTime(0),
		m_opSemaphore(NULL),
		m_vLastValue(0.0)
{
	m_opSemaphore = EMSemaphore::CreateEMSemaphore(EMMediaIDManager::MakeUniqueString(), 1, 1);
}

EMMediaMIDISignalMeter::~EMMediaMIDISignalMeter()
{
	delete m_opSemaphore;
}

void EMMediaMIDISignalMeter::RegisterSignal(uint8 p_vLengthBytes, const uint8* p_vpEventData)
{	
	if(! m_vSignalMetersActive)
		return;

	float vFloatValue = 0.0;
	m_opSemaphore -> Acquire();
	if((p_vpEventData[0] & 240) == 144)		//Note On
	{ 
		if(p_vpEventData[2] == 0)
		{
			m_opSemaphore -> Release();
			return;
		}
		vFloatValue = ((float) (p_vpEventData[2])) / 127.0;
	}
	else if((p_vpEventData[0] & 240) == 176)	//Control change (modulation wheel)
	{
		if(p_vpEventData[1] == 1)
			vFloatValue = ((float) (p_vpEventData[2])) / 127.0;
		else
		{
			m_opSemaphore -> Release();
			return;
		}
	}
	else if((p_vpEventData[0] & 240) == 224)	//Pitch bend
	{
		uint16 vLSB = p_vpEventData[1];
		uint16 vMSB = p_vpEventData[2];
		vMSB <<= 7;
		vFloatValue = ((float) (vMSB | vLSB)) / 16383.0;
	}
	else
	{
		m_opSemaphore -> Release();
		return;
	}

	m_vLastReceiveTime = EMMediaTimer::Instance() -> RealTime();
	m_vLastValue = vFloatValue;
	m_opSemaphore -> Release();
}

float EMMediaMIDISignalMeter::GetLatestSignal()
{
	if(! m_vSignalMetersActive)
		return 0.0;
 
	int64 vNow = EMMediaTimer::Instance() -> RealTime();
	m_opSemaphore -> Acquire();
	if((m_vLastReceiveTime + 50000) > vNow)
	{
		//Signal active
		//if(p_vpInputDescriptorID != NULL)
			//(*p_vpInputDescriptorID) = m_vLastReceivingDeviceID;
		m_opSemaphore -> Release();
		return m_vLastValue;
	}
	else
	{
		//No signal!
		//if(p_vpInputDescriptorID != NULL)
			//(*p_vpInputDescriptorID) = -1;
		m_opSemaphore -> Release();
		return 0.0;
	}
}
