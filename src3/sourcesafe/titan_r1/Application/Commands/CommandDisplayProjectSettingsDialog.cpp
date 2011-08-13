#include "CommandDisplayProjectSettingsDialog.h"

CommandDisplayProjectSettingsDialog::CommandDisplayProjectSettingsDialog(EMWindow* p_opWindow) :
m_opParentWindow(p_opWindow),
m_vInitialized(false)
{
}

void CommandDisplayProjectSettingsDialog::ExecuteE()
{
	if(!m_vInitialized)
	{
		m_oDialog.Init();
		m_oDialog.SetParent(m_opParentWindow);
		m_vInitialized = true;
	}
	m_oDialog.MoveDialog();
	m_oDialog.DisplayDialog();
}

bool CommandDisplayProjectSettingsDialog::IsUndoable()
{
	return false;
}

bool CommandDisplayProjectSettingsDialog::RequiresParameters()
{
	return false;
}


