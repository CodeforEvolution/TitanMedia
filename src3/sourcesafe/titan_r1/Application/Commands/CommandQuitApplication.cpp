#include "CommandQuitApplication.h"

#include "CommandIDs.h"
#include "EMApplication.h"
#include "EMCommandRepository.h"
#include "EMFactory.h"
#include "EMMessageBox.h"
#include "EMMessages.h"
#include "EMSettingIDs.h"
#include "EMSettingsRepository.h"
#include "TrackWindow.h"


CommandQuitApplication::CommandQuitApplication(TrackWindow* p_opTrackWindow) :
m_opTrackWindow(p_opTrackWindow),
m_opMsgBox(EMFactory::Instance() -> CreateMessageBox())
{
}

void CommandQuitApplication::ExecuteE()
{
	if(EMSettingsRepository::Instance() -> SettingExists(SETTING_PROJECT_ID)) // If a project is open
	{
		int vProjectID = *(static_cast<int*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_PROJECT_ID)));

		bool vSaveNeeded = reinterpret_cast<bool>(EMCommandRepository::Instance() -> ExecuteCommand(PROJECT_COMMAND_SAVE_NEEDED, NULL, NULL, NULL));
		if(vSaveNeeded)
		{
			switch(m_opMsgBox -> DisplayDialog(m_opTrackWindow -> GetWindow() -> GetNativeView(), "Do you want to save changes?", "Save Changes?", EM_DIALOG_ICON_QUESTION | EM_DIALOG_BUTTON_YES_NO_CANCEL))
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

		//EMCommandRepository::Instance() -> ExecuteCommand(PROJECT_COMMAND_CLOSE_PROJECT, &vProjectID);
	}

	EMApplication::InitiateQuitApplication();
}

bool CommandQuitApplication::IsUndoable()
{
	return false;
}

bool CommandQuitApplication::RequiresParameters()
{
	return false;
}

