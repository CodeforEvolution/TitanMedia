#include "CommandDisplaySystemSettingsDialog.h"

CommandDisplaySystemSettingsDialog::CommandDisplaySystemSettingsDialog(EMWindow* m_opWindow) :
m_opParentWindow(m_opWindow),
m_vInitialized(false)
{
}

void CommandDisplaySystemSettingsDialog::ExecuteE()
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

bool CommandDisplaySystemSettingsDialog::IsUndoable()
{
	return false;
}

bool CommandDisplaySystemSettingsDialog::RequiresParameters()
{
	return false;
}


