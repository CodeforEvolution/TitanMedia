#include "CommandDisplayNewProjectDialog.h"
#include "EMWindow.h"

CommandDisplayNewProjectDialog::CommandDisplayNewProjectDialog(EMWindow* p_opWindow) :
m_opWindow(p_opWindow),
m_vInitialized(false)
{
}

void CommandDisplayNewProjectDialog::ExecuteE()
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

bool CommandDisplayNewProjectDialog::IsUndoable()
{
	return false;
}

bool CommandDisplayNewProjectDialog::RequiresParameters()
{
	return false;
}


