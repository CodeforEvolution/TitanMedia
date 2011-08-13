#include "CommandDisplayQuantizeDialog.h"

CommandDisplayQuantizeDialog::CommandDisplayQuantizeDialog() :
m_vInitialized(false)
{
}

void CommandDisplayQuantizeDialog::ExecuteE()
{
	if(!m_vInitialized)
	{
		m_oDialog.Init();
		m_vInitialized = true;
	}
	m_oDialog.MoveDialog(600, 400);
	m_oDialog.DisplayDialog();
}

bool CommandDisplayQuantizeDialog::IsUndoable()
{
	return false;
}

bool CommandDisplayQuantizeDialog::RequiresParameters()
{
	return false;
}


