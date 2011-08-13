#include "CommandDisplayEQDialog.h"

#include "EMEqualizerInterface.h"

CommandDisplayEQDialog::CommandDisplayEQDialog(EMWindow* p_opWindow) :
m_opWindow(p_opWindow),
m_vInitialized(false)
{
}

void* CommandDisplayEQDialog::ExecuteE(void* p_upEQ, void* p_upParameterTwo, void* p_upParameterThree)
{
	if(!m_vInitialized)
	{
		m_oDialog.Init();
		m_oDialog.SetParent(m_opWindow);
		m_vInitialized = true;
	}
	m_oDialog.SetEqualizer(static_cast<EMEqualizerInterface*>(p_upEQ));
	m_oDialog.MoveDialog();
	m_oDialog.DisplayDialog();

	return NULL;
}

bool CommandDisplayEQDialog::IsUndoable()
{
	return false;
}

bool CommandDisplayEQDialog::RequiresParameters()
{
	return true;
}