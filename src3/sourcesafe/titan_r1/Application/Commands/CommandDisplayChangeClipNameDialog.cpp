#include "CommandDisplayChangeClipNameDialog.h"

#include "EMGClip.h"
#include "EMRect.h"
#include "EMView.h"
#include "TrackView.h"

CommandDisplayChangeClipNameDialog::CommandDisplayChangeClipNameDialog(TrackView* p_opTrackView) :
m_opTrackView(p_opTrackView),
m_vInitialized(false)
{
}

void* CommandDisplayChangeClipNameDialog::ExecuteE(void* p_upParameterOne, void* p_upParameterTwo, void* p_upParameterThree)
{
	EMGClip* opClip = static_cast<EMGClip*>(p_upParameterOne);
	
	if(!m_vInitialized)
	{
		m_oDialog.Init();
		m_vInitialized = true;
	}

	m_oDialog.SetText(opClip -> GetLabel());
	EMRect oRect(opClip -> Frame());

	m_opTrackView -> GetView() -> ConvertToScreenCoordinates(oRect.m_vLeft, oRect.m_vTop);

	m_oDialog.MoveDialog(oRect.m_vLeft, oRect.m_vTop);
	m_oDialog.DisplayDialog(opClip, m_opTrackView -> GetView());

	return NULL;
}

bool CommandDisplayChangeClipNameDialog::IsUndoable()
{
	return false;
}

bool CommandDisplayChangeClipNameDialog::RequiresParameters()
{
	return true;
}


