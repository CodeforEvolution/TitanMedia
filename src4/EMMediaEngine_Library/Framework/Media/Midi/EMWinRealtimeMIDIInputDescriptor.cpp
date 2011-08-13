#include "EMWinRealtimeMIDIInputDescriptor.h"

#include "EMThread.h"
#include "EMWinDXMIDIConsumer.h"

EMWinRealtimeMIDIInputDescriptor::EMWinRealtimeMIDIInputDescriptor(const GUID* p_upMIDIPortGUID)
	:	m_uMIDIPortGUID(*p_upMIDIPortGUID),
		m_opMIDIConsumer(NULL)
{
}

EMWinRealtimeMIDIInputDescriptor::~EMWinRealtimeMIDIInputDescriptor()
{
}

bool EMWinRealtimeMIDIInputDescriptor::PrepareToPlayE()
{
	return true;
}

void EMWinRealtimeMIDIInputDescriptor::StartE()
{
	if(m_vIsInitialized)
		m_opMIDIConsumer -> StartRecordingE();
}

void EMWinRealtimeMIDIInputDescriptor::StopE()
{
	if(m_vIsInitialized)
		m_opMIDIConsumer -> StopRecordingE();
} 
 
bool EMWinRealtimeMIDIInputDescriptor::ClearData()
{
	return true;
}
 
bool EMWinRealtimeMIDIInputDescriptor::SaveData(EMProjectDataSaver* p_opSaver)
{
	return true;
}

bool EMWinRealtimeMIDIInputDescriptor::LoadData(EMProjectDataLoader* p_opLoader)
{
	return true;
}

bool EMWinRealtimeMIDIInputDescriptor::InitCheckE()
{
	if(! m_vIsInitialized)
	{
		m_opMIDIConsumer = EMWinDXMIDIConsumer::Instance();
		m_vIsInitialized = m_opMIDIConsumer -> InitCheckE();
		if(! m_vIsInitialized)
			return false;
		m_vIsInitialized = m_opMIDIConsumer -> AddPortE(this, static_cast<void*>(&m_uMIDIPortGUID));
	}
	return m_vIsInitialized;
}

bool EMWinRealtimeMIDIInputDescriptor::StartPreviewE()
{
	return true;
}

bool EMWinRealtimeMIDIInputDescriptor::StopPreviewE()
{
	return true;
}

void EMWinRealtimeMIDIInputDescriptor::SetActive(bool p_vFlag, int32 p_vInputID)
{
	ASSERT(m_opMIDIConsumer != NULL);
	m_opMIDIConsumer -> SetActiveMIDIRoutingInput(p_vFlag, p_vInputID);
}

bool EMWinRealtimeMIDIInputDescriptor::IsActive() const
{
	ASSERT(m_opMIDIConsumer != NULL);
	return m_opMIDIConsumer -> IsActiveMIDIRoutingInput();
}