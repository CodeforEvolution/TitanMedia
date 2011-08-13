#include "VideoGTrackInfo.h"

#include "EMGTrack.h"
#include "EMGUIGlobals.h"
#include "EMView.h"
#include "GUIGlobals.h"
#include "TrackInfoButtonBar.h"

VideoGTrackInfo::VideoGTrackInfo(EMView* p_opView, TrackInfoButtonBar* p_opButtonBar) ://, EMGTrack* p_opTrack) :
GTrackInfo(p_opView, p_opButtonBar),
m_opTransitionTrack(NULL)
{
	m_oEditLabel.SetBackgroundColor(EM_COLOR_VIDEO_VIEW_BACKGROUND);
	m_oKeyOffsetValueBox.SetMode(VALUE_BOX_DRAW_BLANK);
	m_oVelocityOffsetValueBox.SetMode(VALUE_BOX_DRAW_BLANK);
//	m_oInputValueBox.SetMode(VALUE_BOX_DRAW_BLANK);
	m_oOutputValueBox.SetMode(VALUE_BOX_DRAW_BLANK);
	m_oPanValueBox.SetMode(VALUE_BOX_DRAW_BLANK);
	m_oTimeOffsetValueBox.SetMode(VALUE_BOX_DRAW_BLANK);
	m_oVolumeValueBox.SetMode(VALUE_BOX_DRAW_BLANK);
	m_oSignalMeter.SetMode(SIGNAL_METER_DRAW_BLANK);

	string oInputName;
	list<int32> oInputIDs;
	list<string*> oInputNames;
	EMMediaType eMediaType = EM_TYPE_ANY_VIDEO;
	oInputIDs = *(static_cast<list<int32>* >(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_INPUTS, &eMediaType, NULL, NULL)));
	list<int32>::iterator oIterator = oInputIDs.begin();
	while(oIterator != oInputIDs.end())
	{
		oInputName = string(static_cast<const char*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_INPUT_NAME, &(*oIterator))));
		oInputNames.push_back(EM_new string(oInputName));
		oIterator++;
	}
	m_oInputValueBox.SetValues(&oInputNames, true);
	
	m_oRenderButton.SetMode(EM_SLIM_BUTTON_DRAW_BLANK);
	m_oArmButton.SetMode(EM_SLIM_BUTTON_DRAW_BLANK);
m_oInputValueBox.SetMode(VALUE_BOX_DRAW_BLANK);
/*
	m_oEditLabel.SetBackgroundColor(EM_COLOR_VIDEO_VIEW_BACKGROUND);
	m_oKeyOffsetValueBox.SetMode(VALUE_BOX_DRAW_BLANK);
	m_oVelocityOffsetValueBox.SetMode(VALUE_BOX_DRAW_BLANK);
	m_oOutputValueBox.SetMode(VALUE_BOX_DRAW_BLANK);
	m_oPanValueBox.SetMode(VALUE_BOX_DRAW_BLANK);
	m_oTimeOffsetValueBox.SetMode(VALUE_BOX_DRAW_BLANK);
	m_oVolumeValueBox.SetMode(VALUE_BOX_DRAW_BLANK);
	m_oSignalMeter.SetMode(SIGNAL_METER_DRAW_BLANK);

	string oInputName;
	list<int32> oInputIDs;
	vector<string> oInputNames;
	EMMediaType eMediaType = EM_TYPE_ANY_VIDEO;
	oInputIDs = *(static_cast<list<int32>* >(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_INPUTS, &eMediaType, NULL, NULL)));
	list<int32>::iterator oIterator = oInputIDs.begin();
	while(oIterator != oInputIDs.end())
	{
		oInputName = string(static_cast<const char*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_INPUT_NAME, &(*oIterator))));
		oInputNames.push_back(oInputName);
		oIterator++;
	}
	m_oInputValueBox.SetValues(oInputNames);
	
	m_oRenderButton.SetMode(EM_SLIM_BUTTON_DRAW_BLANK);
	m_oArmButton.SetMode(EM_SLIM_BUTTON_DRAW_BLANK);
*/
}
/*
void VideoGTrackInfo::CalculateAndSetHidden(bool p_vClipEdit)
{
	m_vClipEdit = p_vClipEdit;
	if((m_vClipEdit && !IsSelected()) || !(TrackInfoButtonBar::Instance() -> GetVideoTrackFilterOn()))
		SetHidden(true);
	else
		SetHidden(false);
}
*/
EMGTrackInfo* VideoGTrackInfo::CreateInstance()
{
	return EM_new VideoGTrackInfo(m_opView, m_opButtonBar);
}

void VideoGTrackInfo::EditModeChanged(EMEditMode p_eEditMode)
{
	if(p_eEditMode == EM_TRACK_EDIT_MODE || p_eEditMode == EM_FX_EDIT_MODE)
		m_oSelectionButton.SetMode(SELECTION_BUTTON_MODE_NORMAL);
	else
		m_oSelectionButton.SetMode(SELECTION_BUTTON_MODE_VIDEO);
}

EMMediaType VideoGTrackInfo::GetType()
{
	return EM_TYPE_ANY_VIDEO;
}

EMGTrack* VideoGTrackInfo::GetTransitionTrack()
{
	return m_opTransitionTrack;
}

bool VideoGTrackInfo::IsHidden(bool p_vClipEdit)
{
//	m_vClipEdit = p_vClipEdit;
	return (p_vClipEdit && !IsSelected()) || !(TrackInfoButtonBar::Instance() -> GetVideoTrackFilterOn());
/*
	if((m_vClipEdit && !IsSelected()) || !(TrackInfoButtonBar::Instance() -> GetAudioTrackFilterOn()))
		SetHidden(true);
	else
		SetHidden(false);
*/
}

void VideoGTrackInfo::SetTransitionTrack(EMGTrack* p_opTransitionTrack)
{
	m_opTransitionTrack = p_opTransitionTrack;
}

/*
void VideoGTrackInfo::SetUserMode(bool p_vExpertMode)
{
	GTrackInfo::SetUserMode(p_vExpertMode);
	m_oRenderButton.SetMode(EM_SLIM_BUTTON_DRAW_BLANK);
}
*/