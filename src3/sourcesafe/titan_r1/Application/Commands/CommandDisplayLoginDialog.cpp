#include "CommandDisplayLoginDialog.h"


CommandDisplayLoginDialog::CommandDisplayLoginDialog(EMWindow* p_opWindow) :
m_opWindow(p_opWindow),
m_vInitialized(false)
{
}

void CommandDisplayLoginDialog::ExecuteE()
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

bool CommandDisplayLoginDialog::IsUndoable()
{
	return false;
}

bool CommandDisplayLoginDialog::RequiresParameters()
{
	return false;
}


