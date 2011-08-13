#include "CommandDisplayWipeTransitionPropertiesDialog.h"

CommandDisplayWipeTransitionPropertiesDialog::CommandDisplayWipeTransitionPropertiesDialog(EMWindow* p_opWindow) :
m_opWindow(p_opWindow),
m_vInitialized(false)
{
}

void* CommandDisplayWipeTransitionPropertiesDialog::ExecuteE(void* p_upParameterOne, void* p_upParameterTwo, void* p_upParameterThree)
{
	EMWindow* opParentWindow = static_cast<EMWindow*>(p_upParameterOne);
	int32* vpTransitionID = static_cast<int32*>(p_upParameterTwo);
	char* vpTransitionName = static_cast<char*>(p_upParameterThree);

	if(!m_vInitialized)
	{
		m_oDialog.Init();
		m_oDialog.SetMainWindow(m_opWindow);
		m_oDialog.SetParent(opParentWindow);
		m_vInitialized = true;
	}
	m_oDialog.SetTransition(*vpTransitionID, vpTransitionName);
	m_oDialog.MoveDialog();
	m_oDialog.DisplayDialog();

	return NULL;
}

bool CommandDisplayWipeTransitionPropertiesDialog::IsUndoable()
{
	return false;
}

bool CommandDisplayWipeTransitionPropertiesDialog::RequiresParameters()
{
	return true;
}

