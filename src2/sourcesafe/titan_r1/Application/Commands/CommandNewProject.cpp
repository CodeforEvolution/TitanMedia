
#include "CommandNewProject.h"

#include "CommandIDs.h"
#include "EMCommandRepository.h"
#include "EMFactory.h"
#include "EMMessageBox.h"
#include "EMSettingIDs.h"
#include "EMSettingsRepository.h"

CommandNewProject::CommandNewProject()
{
}

void* CommandNewProject::ExecuteE(void* p_upParameterOne, void* p_upParameterTwo, void* p_upParameterThree)
{
	m_vContinue = true;
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
				m_vContinue = false;
				return &m_vContinue;
			}
		}
		EMCommandRepository::Instance() -> ExecuteCommand(PROJECT_COMMAND_CLOSE_PROJECT, &vProjectID);
	}
	// Comment by Johan: The row below not needed, because "NewProjectDialog" is calling this command. Don't want it to be recursive. 
	//EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_DISPLAY_NEW_PROJECT_DIALOG);
	return &m_vContinue;
}

bool CommandNewProject::IsUndoable()
{
	return false;
}

bool CommandNewProject::RequiresParameters()
{
	return true;
}
