#include "TransitionGTrackInfo.h"

#include "CommandIDs.h"
#include "EMCommandRepository.h"
#include "EMGTrack.h"
#include "EMGUIGlobals.h"
#include "EMView.h"
#include "GUIGlobals.h"
#include "TrackInfoButtonBar.h"

TransitionGTrackInfo::TransitionGTrackInfo(EMView* p_opView, TrackInfoButtonBar* p_opButtonBar) :
GTrackInfo(p_opView, p_opButtonBar),
m_opVideoTrack(NULL)
//EMGTrackInfo(p_opView, p_opButtonBar)
//m_opButtonBar(p_opButtonBar),
//m_opView(p_opView)
{
	m_oEditLabel.SetBackgroundColor(EM_COLOR_TRANSITION_VIEW_BACKGROUND);

	m_oArmButton.SetMode(EM_SLIM_BUTTON_DRAW_BLANK);
	m_oLockButton.SetMode(EM_SLIM_BUTTON_DRAW_BLANK);
	m_oMuteButton.SetMode(EM_SLIM_BUTTON_DRAW_BLANK);
	m_oRenderButton.SetMode(EM_SLIM_BUTTON_DRAW_BLANK);
	m_oSoloButton.SetMode(EM_SLIM_BUTTON_DRAW_BLANK);
	m_oArmButton.SetMode(EM_SLIM_BUTTON_DRAW_BLANK);

	m_oInputValueBox.SetMode(VALUE_BOX_DRAW_BLANK);
	m_oKeyOffsetValueBox.SetMode(VALUE_BOX_DRAW_BLANK);
	m_oOutputValueBox.SetMode(VALUE_BOX_DRAW_BLANK);
	m_oPanValueBox.SetMode(VALUE_BOX_DRAW_BLANK);
	m_oTimeOffsetValueBox.SetMode(VALUE_BOX_DRAW_BLANK);
	m_oVelocityOffsetValueBox.SetMode(VALUE_BOX_DRAW_BLANK);
	m_oVolumeValueBox.SetMode(VALUE_BOX_DRAW_BLANK);

	m_oSignalMeter.SetMode(SIGNAL_METER_DRAW_BLANK);
}

EMGTrackInfo* TransitionGTrackInfo::CreateInstance()
{
	return new TransitionGTrackInfo(m_opView, m_opButtonBar);
}
/*
void TransitionGTrackInfo::Draw(EMRect p_oClippingRect, EMView* p_opView)
{
	if(!(Frame().Intersects(p_oClippingRect)))
		return;
	p_opView -> SetHighColor(EM_COLOR_VIEW_BACKGROUND);
	p_opView -> FillRect(Frame() & p_oClippingRect);

	p_opView -> SetHighColor(EM_COLOR_VIEW_BACKGROUND_HIGHLIGHT);
	p_opView -> StrokeLine(Frame().m_vLeft, Frame().m_vBottom - 2, Frame().m_vRight, Frame().m_vBottom - 2);
	p_opView -> SetHighColor(EM_COLOR_VIEW_BACKGROUND);
	p_opView -> StrokeLine(Frame().m_vLeft, Frame().m_vBottom - 1, Frame().m_vRight, Frame().m_vBottom - 1);
	p_opView -> SetHighColor(EM_COLOR_VIEW_BACKGROUND_SHADOW);
	p_opView -> StrokeLine(Frame().m_vLeft, Frame().m_vBottom, Frame().m_vRight, Frame().m_vBottom);
}
*/

void TransitionGTrackInfo::EditModeChanged(EMEditMode p_eEditMode)
{
	if(p_eEditMode == EM_TRACK_EDIT_MODE || p_eEditMode == EM_FX_EDIT_MODE)
		m_oSelectionButton.SetMode(SELECTION_BUTTON_MODE_NORMAL);
	else
		m_oSelectionButton.SetMode(SELECTION_BUTTON_MODE_VIDEO);
}

/*
string TransitionGTrackInfo::GetTrackName()
{
	return string("");
}
*/
EMMediaType TransitionGTrackInfo::GetType()
{
	return EM_TYPE_TRANSITION;
}

EMGTrack* TransitionGTrackInfo::GetVideoTrack()
{
	return m_opVideoTrack;
}

bool TransitionGTrackInfo::IsHidden(bool p_vClipEdit)
{
	return (p_vClipEdit && !IsSelected()) || !(TrackInfoButtonBar::Instance() -> GetTransitionTrackFilterOn());
}
/*
bool TransitionGTrackInfo::IsSelected()
{
	return false;
}

void TransitionGTrackInfo::LayoutComponents()
{
}

void TransitionGTrackInfo::MouseDown(int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree, EMView* p_opView)
{
}

void TransitionGTrackInfo::MouseDoubleClick(int32 p_vX, int32 p_vY, EMView* p_opView)
{
}

void TransitionGTrackInfo::MouseMoved(int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree)
{
}

void TransitionGTrackInfo::MouseUp(int32 p_vX, int32 p_vY)
{
}

void TransitionGTrackInfo::SetSelected(bool p_vSelected, bool p_vNotify)
{
}
*/
void TransitionGTrackInfo::SetSignal(list<float>* p_vSignal)
{
	EMDebugger("SetSignal is not relevat for transition tracks");
}
/*
void TransitionGTrackInfo::SetTrackName(string p_oName)
{
}
*/
void TransitionGTrackInfo::SetUserMode(bool p_vExpertMode)
{
	GTrackInfo::SetUserMode(p_vExpertMode);
	m_oSoloButton.SetMode(EM_SLIM_BUTTON_DRAW_BLANK);
}


void TransitionGTrackInfo::SetVideoTrack(EMGTrack* p_opVideoTrack)
{
	m_opVideoTrack = p_opVideoTrack;
}