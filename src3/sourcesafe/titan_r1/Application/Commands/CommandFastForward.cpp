#include "CommandFastForward.h"

#include "CommandIDs.h"
#include "EMCommandRepository.h"
#include "EMResourceRepository.h"
#include "EMSettingIDs.h"
#include "EMSettingsRepository.h"
#include "EMThreadRepository.h"
#include "GUIGlobals.h"
#include "ResourceIDs.h"
#include "SettingIDs.h"
#include "TrackWindowToolBar.h"

#include <list>
#include <string>

const int64 PIXEL_MOVE_DISTANCE = 5;

CommandFastForward::CommandFastForward(TrackWindowToolBar* p_opToolBar) :
m_opSemaphore(EMSemaphore::CreateEMSemaphore("Command Fast Forward Semaphore")),
m_opToolBar(p_opToolBar),
m_vKeyPressed(false)
{
}

CommandFastForward::~CommandFastForward()
{
	delete m_opSemaphore;
}

void CommandFastForward::ExecuteE()
{
	if(!(EMCommandRepository::Instance() -> TriggeredByBinding()))
		ThreadRun(NULL);
	else if(!(EMCommandRepository::Instance() -> KeyReleased()) && m_vKeyPressed)
		return; // Should not be triggered on key repeats
	else if(!(EMCommandRepository::Instance() -> KeyReleased()))//!m_vKeyPressed)
	{
		m_vKeyPressed = true;
		EMThreadRepository::Instance() -> GetThread(GUI_SERVICE_THREAD_NAME) -> AddListener(this);
	}
	else
	{
		m_vKeyPressed = false;
		EMThreadRepository::Instance() -> GetThread(GUI_SERVICE_THREAD_NAME) -> RemoveListener(this);
	}
}

bool CommandFastForward::IsUndoable()
{
	return false;
}

bool CommandFastForward::RequiresParameters()
{
	return false;
}

void CommandFastForward::ThreadRun(EMThread* p_opThread)
{
	int64 vTimeZoomScale = *(static_cast<int64*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_TIME_ZOOM_SCALE)));
	int64 vPosition = *(static_cast<int64*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_CURRENT_PROJECT_POSITION, NULL)));
	int64 vNewPosition(vPosition + (PIXEL_MOVE_DISTANCE * vTimeZoomScale));
	m_opToolBar -> SetFastForward();
	bool vIsPlaying(false);
	if(*(static_cast<bool*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_IS_PLAYING, NULL, NULL, NULL))))
	{
		vIsPlaying = true;
		EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_STOP_PLAYBACK);
	}
	EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_SEEK, &vNewPosition);
	EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_PLAY_BUFFER);
	if(vIsPlaying)
		EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_START_PLAYBACK);
}

bool CommandFastForward::TriggerOnKeyRelease()
{
	return true;
}


