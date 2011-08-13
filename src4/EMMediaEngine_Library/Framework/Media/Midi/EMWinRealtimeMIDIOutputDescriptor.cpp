#include "EMWinRealtimeMIDIOutputDescriptor.h"

#include "EMMediaDataBuffer.h"
#include "EMWinDXMIDIProducer.h"
#include "EMMediaMIDIEvent.h"
#include "EMMediaEngine.h"
#include "EMMediaProject.h"
#include "EMMediaMIDIEventRepository.h"
#include "EMListenerRepository.h"

EMWinRealtimeMIDIOutputDescriptor::EMWinRealtimeMIDIOutputDescriptor(const GUID* p_upMIDIPortGUID)
	:	m_uMIDIPortGUID(*p_upMIDIPortGUID),
		m_opMIDIProducer(NULL)
{
}

EMWinRealtimeMIDIOutputDescriptor::~EMWinRealtimeMIDIOutputDescriptor()
{
}

bool EMWinRealtimeMIDIOutputDescriptor::PrepareToPlayE()
{
	return true;
}

void EMWinRealtimeMIDIOutputDescriptor::StartE()
{
}

void EMWinRealtimeMIDIOutputDescriptor::StopE()
{ 
}

bool EMWinRealtimeMIDIOutputDescriptor::ClearData()
{
	return true;
}

bool EMWinRealtimeMIDIOutputDescriptor::SaveData(EMProjectDataSaver* p_opSaver) 
{
	return true;
}

bool EMWinRealtimeMIDIOutputDescriptor::LoadData(EMProjectDataLoader* p_opLoader)
{
	return true;
}

bool EMWinRealtimeMIDIOutputDescriptor::InitCheckE()
{
	if(! m_vIsInitialized)
	{
		m_opMIDIProducer = EMWinDXMIDIProducer::Instance();
		m_vIsInitialized = m_opMIDIProducer -> InitCheckE();
		if(! m_vIsInitialized)
			return false;
		m_vIsInitialized = m_opMIDIProducer -> AddPortE(this, static_cast<void*>(&m_uMIDIPortGUID));
	}
	return m_vIsInitialized;
}

EMMediaDataBuffer* EMWinRealtimeMIDIOutputDescriptor::ProcessBufferE(list<EMMediaDataBuffer*>* p_opBufferList)
{
	return NULL;
}

bool EMWinRealtimeMIDIOutputDescriptor::InitializeEventRepositoryE()
{
	return true;
}

bool EMWinRealtimeMIDIOutputDescriptor::UninitializeEventRepositoryE()
{
	return true;
}

bool EMWinRealtimeMIDIOutputDescriptor::MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage)
{
	switch(p_vMessage)
	{
		case EM_MESSAGE_AUDIO_TRACK_ACTIVATED:
			break;
		case EM_MESSAGE_VIDEO_TRACK_ACTIVATED:
			break;
		case EM_MESSAGE_AUDIO_TRACK_DEACTIVATED:
			break;
		case EM_MESSAGE_VIDEO_TRACK_DEACTIVATED:
			break;
		case EM_MESSAGE_BEGIN_PLAYORRECORD:
			break;
		case EM_MESSAGE_STOP_PLAYORRECORD:
			break;
		default:
			break;
	};
	return true;
}

void EMWinRealtimeMIDIOutputDescriptor::SetActive(bool p_vFlag)
{
	ASSERT(m_opMIDIProducer != NULL);
}

bool EMWinRealtimeMIDIOutputDescriptor::IsActive() const
{
	ASSERT(m_opMIDIProducer != NULL);
	return true;
}

EMWinDXMIDIProducer* EMWinRealtimeMIDIOutputDescriptor::GetProducer() const
{
	return m_opMIDIProducer;
}

bool EMWinRealtimeMIDIOutputDescriptor::RegisterAsClockMaster()
{
	return true;
}

bool EMWinRealtimeMIDIOutputDescriptor::UnregisterAsClockMaster()
{
	return true;
}