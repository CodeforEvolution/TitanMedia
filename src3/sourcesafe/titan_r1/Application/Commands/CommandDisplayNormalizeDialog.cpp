#include "CommandDisplayNormalizeDialog.h"

CommandDisplayNormalizeDialog::CommandDisplayNormalizeDialog(EMWindow* p_opWindow) :
m_opWindow(p_opWindow),
m_vInitialized(false)
{
}

void* CommandDisplayNormalizeDialog::ExecuteE(void* p_opCommandAudioNormalize, void* p_upParameterTwo, void* p_upParameterThree)
{
	if(!m_vInitialized)
	{
		m_oDialog.Init();
		m_oDialog.SetParent(m_opWindow);
		m_vInitialized = true;
	}
	m_oDialog.MoveDialog();
	m_oDialog.DisplayDialog(p_opCommandAudioNormalize);

	return NULL;
}

bool CommandDisplayNormalizeDialog::IsUndoable()
{
	return false;
}

bool CommandDisplayNormalizeDialog::RequiresParameters()
{
	return true;
}


