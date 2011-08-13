#include "CommandDisplayVideoSpeedDialog.h"

CommandDisplayVideoSpeedDialog::CommandDisplayVideoSpeedDialog(EMWindow* m_opWindow) :
m_opParentWindow(m_opWindow),
m_vInitialized(false)
{
}

void CommandDisplayVideoSpeedDialog::ExecuteE()
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

bool CommandDisplayVideoSpeedDialog::IsUndoable()
{
	return false;
}

bool CommandDisplayVideoSpeedDialog::RequiresParameters()
{
	return false;
}


