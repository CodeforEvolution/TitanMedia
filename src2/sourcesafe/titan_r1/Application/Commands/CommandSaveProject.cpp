#include "CommandSaveProject.h"

#include "CommandIDs.h"
#include "EMCommandRepository.h"
#include "EMFactory.h"
#include "EMFileWindow.h"
#include "EMMediaGlobals.h"
#include "EMMessages.h"
#include "EMSettingIDs.h"
#include "EMSettingsRepository.h"
#include "EMWindow.h"
#include "FileDialogTarget.h"
#include "TrackWindow.h"
#include "TrackWindowStatusBar.h"

#include <string>

#define FILE_WINDOW_TITLE "Save Project"

CommandSaveProject::CommandSaveProject(FileDialogTarget* p_opFileDialogTarget, TrackWindow* p_opWindow) :
m_opFileDialogTarget(p_opFileDialogTarget),
m_opWindow(p_opWindow)
{
}

void CommandSaveProject::ExecuteE()
{
	int vProjectID = *(static_cast<int*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_PROJECT_ID)));
	if(reinterpret_cast<bool>(EMCommandRepository::Instance() -> ExecuteCommand(PROJECT_COMMAND_SAVE_NEEDED, NULL)) == false)
	{
		if(m_opWindow -> GetWindow() -> Lock())
		{
			TrackWindowStatusBar::Instance() -> WriteMessage(string("No need to save"));
			m_opWindow -> GetWindow() -> Unlock();
		}
		return;
	}

	EMCommandRepository::Instance() -> ExecuteCommand(PROJECT_COMMAND_SAVE_PROJECT, &vProjectID);
	EMCommandRepository::Instance() -> ExecuteCommand(PROJECT_COMMAND_READY_FOR_USE, &vProjectID);

	if(m_opWindow -> GetWindow() -> Lock())
	{
		TrackWindowStatusBar::Instance() -> WriteMessage(string("Project Saved"));
		m_opWindow -> GetWindow() -> Unlock();
	}
}

bool CommandSaveProject::IsUndoable()
{
	return false;
}

bool CommandSaveProject::RequiresParameters()
{
	return false;
}
