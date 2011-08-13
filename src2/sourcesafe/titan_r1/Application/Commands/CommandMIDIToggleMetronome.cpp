#include "CommandMIDIToggleMetronome.h"

#include "EMMenu.h"
#include "EMMenuBar.h"
#include "EMMenuItem.h"
#include "EMResourceRepository.h"
#include "EMSettingIDs.h"
#include "EMSettingsRepository.h"
#include "ResourceIDs.h"

#include <string>

CommandMIDIToggleMetronome::CommandMIDIToggleMetronome(EMMenuBar* p_opMenuBar) :
m_opMenuBar(p_opMenuBar)
{
}

void CommandMIDIToggleMetronome::ExecuteE()
{
/*
	string* opMenuName = EMResourceRepository::GetString(RES_TM_MIDI);
	string* opItemName = EMResourceRepository::GetString(RES_TM_METRONOME);
	EMMenu* opMenu = m_opMenuBar -> GetMenu(opMenuName);
	EMMenuItem* opMenuItem = opMenu -> GetMenuItem(opItemName);
*/
	bool vMetronome = *(static_cast<bool*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_MIDI_METRONOME_ENABLED)));
	vMetronome = !vMetronome;
	EMSettingsRepository::Instance() -> SetSetting(SETTING_MIDI_METRONOME_ENABLED, EM_SETTING_BOOL, &vMetronome);
//	opMenuItem -> SetMarked(vMetronome);
}

bool CommandMIDIToggleMetronome::IsUndoable()
{
	return false;
}

bool CommandMIDIToggleMetronome::RequiresParameters()
{
	return false;
}


