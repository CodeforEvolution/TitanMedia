#include "CommandToggleEnableMIDI.h"

#include "EMMenu.h"
#include "EMMenuBar.h"
#include "EMMenuItem.h"
#include "EMResourceRepository.h"
#include "EMSettingIDs.h"
#include "EMSettingsRepository.h"
#include "EMView.h"
#include "ResourceIDs.h"
#include "TrackView.h"

#include <string>

CommandToggleEnableMIDI::CommandToggleEnableMIDI(EMMenuBar* p_opMenuBar, TrackView* p_opTrackView) :
m_opMenuBar(p_opMenuBar),
m_opTrackView(p_opTrackView)
{
}

void CommandToggleEnableMIDI::ExecuteE()
{
	bool vEnableMIDI = *(static_cast<bool*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_MIDI_ENABLED)));
	vEnableMIDI = !vEnableMIDI;
	EMSettingsRepository::Instance() -> SetSetting(SETTING_MIDI_ENABLED, EM_SETTING_BOOL, &vEnableMIDI);

	if(m_opTrackView -> GetView() -> LockWindow())
	{
		m_opTrackView -> GetView() -> Invalidate(m_opTrackView -> GetView() -> Bounds());
		m_opTrackView -> GetView() -> UnlockWindow();
	}
/*
	string* opMenuName = EMResourceRepository::GetString(RES_TM_USING);
	string* opItemName = EMResourceRepository::GetString(RES_TM_USING_MIDI);
	EMMenu* opMenu = m_opMenuBar -> GetMenu(opMenuName);
	EMMenuItem* opMenuItem = opMenu -> GetMenuItem(opItemName);
	opMenuItem -> SetMarked(vEnableMIDI);

	opMenuName = EMResourceRepository::GetString(RES_TM_MIDI);
	opMenu = m_opMenuBar -> GetMenu(opMenuName);
	opMenu -> SetEnabled(vEnableMIDI);

	opMenuName = EMResourceRepository::GetString(RES_TM_TRACK);
	opItemName = EMResourceRepository::GetString(RES_TM_CREATE_MIDI_TRACK);
	opMenu = m_opMenuBar -> GetMenu(opMenuName);
	opMenuItem = opMenu -> GetMenuItem(opItemName);
	opMenuItem -> SetEnabled(vEnableMIDI);
*/
}

bool CommandToggleEnableMIDI::IsUndoable()
{
	return false;
}

bool CommandToggleEnableMIDI::RequiresParameters()
{
	return false;
}


