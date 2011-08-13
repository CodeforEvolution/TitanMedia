#include "CommandDisplayExportMediaDialog.h"

CommandDisplayExportMediaDialog::CommandDisplayExportMediaDialog(EMWindow* p_opWindow) :
m_opWindow(p_opWindow),
m_vInitialized(false)
{
}

void CommandDisplayExportMediaDialog::ExecuteE()
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

bool CommandDisplayExportMediaDialog::IsUndoable()
{
	return false;
}

bool CommandDisplayExportMediaDialog::RequiresParameters()
{
	return false;
}


