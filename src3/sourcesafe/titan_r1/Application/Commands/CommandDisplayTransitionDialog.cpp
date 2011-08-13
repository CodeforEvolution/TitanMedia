#include "CommandDisplayTransitionDialog.h"
#include "EMGClip.h"


CommandDisplayTransitionDialog::CommandDisplayTransitionDialog(EMWindow* p_opWindow, EMView* p_opView) :
m_opView(p_opView),
m_opWindow(p_opWindow),
m_vInitialized(false)
{
}

void* CommandDisplayTransitionDialog::ExecuteE(void* p_opParameterOne, void* p_opParameterTwo, void* p_opParameterThree)
{
	EMGClip* vTransitionClip = static_cast<EMGClip*>(p_opParameterOne);
	if(!m_vInitialized)
	{
		m_oDialog.Init();
		m_oDialog.SetParent(m_opWindow);
		m_oDialog.SetTrackView(m_opView);
		m_vInitialized = true;
	}
	m_oDialog.SetTransitionClip(vTransitionClip);
	m_oDialog.MoveDialog();
	m_oDialog.DisplayDialog();

	return NULL;
}

bool CommandDisplayTransitionDialog::IsUndoable()
{
	return false;
}

bool CommandDisplayTransitionDialog::RequiresParameters()
{
	return true;
}
