#include "CommandDisplayColorBalanceDialog.h"

#include "EMRGBFilter.h"

CommandDisplayColorBalanceDialog::CommandDisplayColorBalanceDialog(EMWindow* p_opWindow) :
m_opWindow(p_opWindow),
m_vInitialized(false)
{
}

void* CommandDisplayColorBalanceDialog::ExecuteE(void* p_opRGBFilter, void*, void* )
{
	if(!m_vInitialized)
	{
		m_oDialog.Init();
		m_oDialog.SetParent(m_opWindow);
		m_vInitialized = true;
	}
	m_oDialog.MoveDialog();
	m_oDialog.DisplayDialog(static_cast<EMRGBFilter*>(p_opRGBFilter));

	return NULL;
}

bool CommandDisplayColorBalanceDialog::IsUndoable()
{
	return false;
}

bool CommandDisplayColorBalanceDialog::RequiresParameters()
{
	return true;
}