#include "CommandToggleLoop.h"

#include "EMSettingsRepository.h"
#include "SettingIDs.h"
#include "TrackWindowToolBar.h"

CommandToggleLoop::CommandToggleLoop(TrackWindowToolBar* p_opToolBar) :
m_opToolBar(p_opToolBar)
{
}

void CommandToggleLoop::ExecuteE()
{
;//cout_commented_out_4_release << "CommandToggleLoop::ExecuteE" << endl;
//	m_opToolBar -> SetLoop();
	bool vLoop = *(static_cast<bool*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_LOOP)));
	if(vLoop)
	{
		vLoop = false;
		EMSettingsRepository::Instance() -> SetSetting(SETTING_LOOP, EM_SETTING_BOOL, &vLoop);
	}
	else
	{
		vLoop = true;
		EMSettingsRepository::Instance() -> SetSetting(SETTING_LOOP, EM_SETTING_BOOL, &vLoop);
	}
}

bool CommandToggleLoop::IsUndoable()
{
	return false;
}

bool CommandToggleLoop::RequiresParameters()
{
	return false;
}

