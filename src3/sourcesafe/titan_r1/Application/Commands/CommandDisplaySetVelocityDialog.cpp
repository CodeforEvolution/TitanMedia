#include "CommandDisplaySetVelocityDialog.h"

CommandDisplaySetVelocityDialog::CommandDisplaySetVelocityDialog() :
m_vInitialized(false)
{
}

void CommandDisplaySetVelocityDialog::ExecuteE()
{
	if(!m_vInitialized)
	{
		m_oDialog.Init();
		m_vInitialized = true;
	}
	m_oDialog.MoveDialog(600, 400);
	m_oDialog.DisplayDialog();
}

bool CommandDisplaySetVelocityDialog::IsUndoable()
{
	return false;
}

bool CommandDisplaySetVelocityDialog::RequiresParameters()
{
	return false;
}


