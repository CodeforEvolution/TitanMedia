#include "CommandDisplayHSV_ValueDialog.h"

#include "EMHSVFilter.h"

CommandDisplayHSV_ValueDialog::CommandDisplayHSV_ValueDialog(EMWindow* p_opWindow) :
m_opWindow(p_opWindow),
m_vInitialized(false)
{
}

void* CommandDisplayHSV_ValueDialog::ExecuteE(void* p_opHSVFilter, void*, void* )
{
	if(!m_vInitialized)
	{
		m_oDialog.Init();
		m_oDialog.SetParent(m_opWindow);
		m_vInitialized = true;
	}
	m_oDialog.MoveDialog();
	m_oDialog.DisplayDialog(static_cast<EMHSVFilter*>(p_opHSVFilter));

	return NULL;
}

bool CommandDisplayHSV_ValueDialog::IsUndoable()
{
	return false;
}

bool CommandDisplayHSV_ValueDialog::RequiresParameters()
{
	return true;
}