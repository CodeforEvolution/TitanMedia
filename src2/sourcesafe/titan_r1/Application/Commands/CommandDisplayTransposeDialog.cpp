#include "CommandDisplayTransposeDialog.h"

CommandDisplayTransposeDialog::CommandDisplayTransposeDialog() :
m_vInitialized(false)
{
}

void CommandDisplayTransposeDialog::ExecuteE()
{
	if(!m_vInitialized)
	{
		m_oDialog.Init();
		m_vInitialized = true;
	}
	m_oDialog.MoveDialog(800, 100);
	m_oDialog.DisplayDialog();
}

bool CommandDisplayTransposeDialog::IsUndoable()
{
	return false;
}

bool CommandDisplayTransposeDialog::RequiresParameters()
{
	return false;
}


