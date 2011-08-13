#include "EMMediaSignalMeter.h"

#include "EMMediaIDManager.h"
#include "EMMediaAudioSignalMeter.h"
#include "EMMediaInputAudioSignalMeter.h"
#include "EMMediaMIDISignalMeter.h"
#include "EMSettingIDs.h"
#include "EMMediaEngine.h"
#include "EMSettingsRepository.h"
#include "EMMediaTimer.h"
#include "EMMessages.h"

EMMediaSignalMeter* EMMediaSignalMeter::CreateEMSignalMeter(EMMediaType p_eType, bool m_vIsInputMeter)
{
	if((p_eType & EM_TYPE_MIDI) > 0)
		return EM_new EMMediaMIDISignalMeter();
	else if((p_eType & EM_TYPE_RAW_AUDIO) > 0 && !m_vIsInputMeter)
		return EM_new EMMediaAudioSignalMeter();
	else if((p_eType & EM_TYPE_RAW_AUDIO) > 0 && m_vIsInputMeter)
		return EM_new EMMediaInputAudioSignalMeter();
	else
		EMDebugger("ERROR! Cannot instantiate that type of signal meter!");
	return NULL;
}

EMMediaSignalMeter::EMMediaSignalMeter(EMMediaType p_eType)
	:	EMBufferDestination(EM_OUTPUT_TYPE_PREEFFECTS, EM_TRACK, string("EMMediaSignalMeter")),
		m_eType(p_eType),
		m_vSignalMetersActive(false)
{
	m_vID = EMMediaIDManager::MakeID();
	EMMediaEngine::Instance() -> GetSettingsRepository() -> AddListener(this);
	EMMediaTimer::Instance() -> AddListener(this);
	m_vSignalMetersActive = *(static_cast<bool*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_SIGNAL_METERS_ENABLED)));
}

EMMediaSignalMeter::~EMMediaSignalMeter()
{
	EMMediaEngine::Instance() -> GetSettingsRepository() -> RemoveListener(this);
	EMMediaTimer::Instance() -> RemoveListener(this);
}

int32 EMMediaSignalMeter::GetID() const
{
	return m_vID;
}

EMMediaType EMMediaSignalMeter::GetType() const
{
	return m_eType;
}

bool EMMediaSignalMeter::PrepareToPlayE()
{
	return true;
}

EMMediaDataBuffer* EMMediaSignalMeter::ProcessBufferE(list<EMMediaDataBuffer*>* p_opBufferList)
{
	EMDebugger("ERROR! ProcessBufferE in EMMediaSignalMeter (base class) should NOT be called!");
	return NULL;
}

bool EMMediaSignalMeter::MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage)
{
	switch(p_vMessage)
	{
		case EM_SETTING_UPDATED:
		{
			m_vSignalMetersActive = *(static_cast<bool*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_SIGNAL_METERS_ENABLED)));
			break;
		}
		case EM_MESSAGE_TIME_WAS_SEEKED:
			OnSeek();
			break;
		case EM_MESSAGE_BEGIN_PLAYORRECORD:
			OnStart();
			break;
		case EM_MESSAGE_STOP_PLAYORRECORD:
			OnStop();
			break;

		default:
			return false;
	};
	return true;
}

void EMMediaSignalMeter::OnStart()
{
}

void EMMediaSignalMeter::OnStop()
{
}

void EMMediaSignalMeter::OnSeek()
{
}
