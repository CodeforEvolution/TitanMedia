#include "CommandOpenProject.h"

#include "CommandIDs.h"
#include "EMCommandRepository.h"
#include "EMFactory.h"
#include "EMFileWindow.h"
#include "EMMediaGlobals.h"
#include "EMMessageBox.h"
#include "EMMessages.h"
#include "EMSettingIDs.h"
#include "EMSettingsRepository.h"
#include "FileDialogTarget.h"
#include "TrackWindow.h"

#include <string>

#define FILE_WINDOW_TITLE "Load Project"

CommandOpenProject::CommandOpenProject(FileDialogTarget* p_opFileDialogTarget) :
m_opFileDialogTarget(p_opFileDialogTarget)
{
}

void CommandOpenProject::ExecuteE()
{
	/*
	if(EMSettingsRepository::Instance() -> SettingExists(SETTING_PROJECT_ID)) // If a project is open
	{
		int vProjectID = *(static_cast<int*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_PROJECT_ID)));

		bool vSaveNeeded = reinterpret_cast<bool>(EMCommandRepository::Instance() -> ExecuteCommand(PROJECT_COMMAND_SAVE_NEEDED, NULL, NULL, NULL));
		if(vSaveNeeded)
		{
			switch(EMFactory::Instance() -> CreateMessageBox() -> DisplayDialog(NULL, "Do you want to save changes?", "Save Changes?", EM_DIALOG_ICON_QUESTION | EM_DIALOG_BUTTON_YES_NO_CANCEL))
			{
			case EM_DIALOG_YES_BUTTON:
				{
					EMCommandRepository::Instance() -> ExecuteCommand(PROJECT_COMMAND_SAVE_PROJECT, &vProjectID);
					break;
				}
			case EM_DIALOG_NO_BUTTON:
				break;
			case EM_DIALOG_CANCEL_BUTTON:
				return;
			}
		}

		EMCommandRepository::Instance() -> ExecuteCommand(PROJECT_COMMAND_CLOSE_PROJECT, &vProjectID);
	}
	*/
	//char* vpStartDirectory = "/boot/home/";

	vector<string> oVisibleExtensions;
	oVisibleExtensions.push_back(string("Project Files"));
	oVisibleExtensions.push_back(string("*.tpf"));

	EMFileWindow* opFileWindow = EMFactory::Instance() -> CreateFileWindow(FILE_PANEL_LOAD_FILE, FILE_WINDOW_TITLE, NULL, false, "", &oVisibleExtensions, "", EM_LOAD_PROJECT_REQUESTED);

	//EMFileWindow* opFileWindow = EMFactory::Instance() -> CreateFileWindow(FILE_PANEL_LOAD_FILE, FILE_WINDOW_TITLE, NULL, false, NULL, NULL, ""/*vpStartDirectory*/, EM_LOAD_PROJECT_REQUESTED);
	opFileWindow -> AddListener(m_opFileDialogTarget);
	opFileWindow -> Show();
}

bool CommandOpenProject::IsUndoable()
{
	return false;
}

bool CommandOpenProject::RequiresParameters()
{
	return false;
}
