#include "CommandDisplayAboutDialog.h"


CommandDisplayAboutDialog::CommandDisplayAboutDialog(EMWindow* p_opWindow) :
m_opWindow(p_opWindow),
m_vInitialized(false)
{
}

void CommandDisplayAboutDialog::ExecuteE()
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

bool CommandDisplayAboutDialog::IsUndoable()
{
	return false;
}

bool CommandDisplayAboutDialog::RequiresParameters()
{
	return false;
}


