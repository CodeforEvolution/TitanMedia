#include "CommandDisplaySearchUserDialog.h"

CommandDisplaySearchUserDialog::CommandDisplaySearchUserDialog(EMWindow* p_opWindow) :
m_opMainWindow(p_opWindow),
m_vInitialized(false)
{
}

void* CommandDisplaySearchUserDialog::ExecuteE(void* p_opParentWindow, void* p_opSystemSettingsDialog, void* )
{
	if(!m_vInitialized)
	{
		m_oDialog.Init();
		m_oDialog.SetMainWindow(m_opMainWindow);
		m_oDialog.SetParent(static_cast<EMWindow*>(p_opParentWindow));
		m_oDialog.SetSystemSettingsDialog(p_opSystemSettingsDialog);
		m_vInitialized = true;
	}
	m_oDialog.MoveDialog();
	m_oDialog.DisplayDialog();

	return NULL;
}

bool CommandDisplaySearchUserDialog::IsUndoable()
{
	return false;
}

bool CommandDisplaySearchUserDialog::RequiresParameters()
{
	return true;
}
