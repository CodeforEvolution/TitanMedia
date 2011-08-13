#include "CommandDisplayRegistrationDialog.h"

CommandDisplayRegistrationDialog::CommandDisplayRegistrationDialog(EMWindow* p_opWindow) :
m_opWindow(p_opWindow),
m_vInitialized(false)
{
}

void* CommandDisplayRegistrationDialog::ExecuteE(void* p_upParameterOne, void* p_upParameterTwo, void* p_upParameterThree)
{

	if(!m_vInitialized)
	{
		m_oDialog.Init(static_cast<char*>(p_upParameterOne), *(static_cast<uint32*>(p_upParameterTwo)), static_cast<TrackWindow*>(p_upParameterThree));
		m_oDialog.SetParent(m_opWindow);
		m_vInitialized = true;
	}

	m_oDialog.MoveDialog();
	m_oDialog.DisplayDialog();

	return NULL;
}

bool CommandDisplayRegistrationDialog::IsUndoable()
{
	return false;
}

bool CommandDisplayRegistrationDialog::RequiresParameters()
{
	return true;
}
