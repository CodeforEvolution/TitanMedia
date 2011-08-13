#include "CommandDisplayRenderFormatDialog.h"

CommandDisplayRenderFormatDialog::CommandDisplayRenderFormatDialog()// :
//m_vInitialized(false)
{
}

void CommandDisplayRenderFormatDialog::ExecuteE()
{
//;//cout_commented_out_4_release << "CommandDisplayRenderFormatDialog::ExecuteE" << endl;
/*
	if(!m_vInitialized)
	{
		m_oDialog.Init();
		m_vInitialized = true;
	}
*/
	m_oDialog.DisplayDialog();
}

bool CommandDisplayRenderFormatDialog::IsUndoable()
{
	return false;
}

bool CommandDisplayRenderFormatDialog::RequiresParameters()
{
	return false;
}


