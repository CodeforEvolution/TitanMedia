#include "CommandDisplaySearchAddUserDialog.h"

CommandDisplaySearchAddUserDialog::CommandDisplaySearchAddUserDialog(EMWindow* p_opWindow) :
m_opWindow(p_opWindow),
m_vInitialized(false)
{
}

void CommandDisplaySearchAddUserDialog::ExecuteE()
{
	if(!m_vInitialized)
	{
		m_oDialog.Init();
		m_oDialog.SetParent(m_opWindow);
		m_vInitialized = true;
	}
	m_oDialog.MoveDialog();
	m_oDialog.DisplayDialog();
}

bool CommandDisplaySearchAddUserDialog::IsUndoable()
{
	return false;
}

bool CommandDisplaySearchAddUserDialog::RequiresParameters()
{
	return false;
}
