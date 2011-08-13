#include "AudioGTrackInfo.h"

#include "CommandIDs.h"
#include "EMCommandRepository.h"
#include "EMGTrack.h"
#include "EMGUIGlobals.h"
#include "EMView.h"
#include "GUIGlobals.h"
#include "TrackInfoButtonBar.h"

#include <list>
#include <string>
#include <vector>

AudioGTrackInfo::AudioGTrackInfo(EMView* p_opView, TrackInfoButtonBar* p_opButtonBar) ://, EMGTrack* p_opTrack) :
GTrackInfo(p_opView, p_opButtonBar)
{
	m_oEditLabel.SetBackgroundColor(EM_COLOR_AUDIO_VIEW_BACKGROUND);
	string oInputName;
	string oOutputName;
	list<int32> oOutputIDs;
	list<string*> oOutputNames;
	list<int32> oInputIDs;
	list<string*> oInputNames;
	EMMediaType eMediaType = EM_TYPE_ANY_AUDIO;
	oOutputIDs = *(static_cast<list<int32>* >(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_OUTPUTS, &eMediaType, NULL, NULL)));
	oInputIDs = *(static_cast<list<int32>* >(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_INPUTS, &eMediaType, NULL, NULL)));
	list<int32>::iterator oIterator = oOutputIDs.begin();
	while(oIterator != oOutputIDs.end())
	{
		oOutputName = string(static_cast<const char*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_OUTPUT_NAME, &(*oIterator))));
		oOutputNames.push_back(EM_new string(oOutputName));
		oIterator++;
	}
	oIterator = oInputIDs.begin();
	while(oIterator != oInputIDs.end())
	{
		oInputName = string(static_cast<const char*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_INPUT_NAME, &(*oIterator))));
		oInputNames.push_back(EM_new string(oInputName));
		oIterator++;
	}
	
	m_oOutputValueBox.SetValues(&oOutputNames, true);
	m_oInputValueBox.SetValues(&oInputNames, true);
	m_oKeyOffsetValueBox.SetMode(VALUE_BOX_DRAW_BLANK);
	m_oVelocityOffsetValueBox.SetMode(VALUE_BOX_DRAW_BLANK);
	m_oTimeOffsetValueBox.SetMode(VALUE_BOX_DRAW_BLANK);

	m_oRenderButton.SetMode(EM_SLIM_BUTTON_DRAW_BLANK);
	/*
	m_oEditLabel.SetBackgroundColor(EM_COLOR_AUDIO_VIEW_BACKGROUND);
	string oInputName;
	string oOutputName;
	list<int32> oOutputIDs;
	vector<string> oOutputNames;
	list<int32> oInputIDs;
	vector<string> oInputNames;
	EMMediaType eMediaType = EM_TYPE_ANY_AUDIO;
	oOutputIDs = *(static_cast<list<int32>* >(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_OUTPUTS, &eMediaType, NULL, NULL)));
	oInputIDs = *(static_cast<list<int32>* >(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_INPUTS, &eMediaType, NULL, NULL)));
	list<int32>::iterator oIterator = oOutputIDs.begin();
	while(oIterator != oOutputIDs.end())
	{
		oOutputName = string(static_cast<const char*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_OUTPUT_NAME, &(*oIterator))));
		oOutputNames.push_back(string(oOutputName));
		oIterator++;
	}
	oIterator = oInputIDs.begin();
	while(oIterator != oInputIDs.end())
	{
		oInputName = string(static_cast<const char*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_INPUT_NAME, &(*oIterator))));
		oInputNames.push_back(string(oInputName));
		oIterator++;
	}
	
	m_oOutputValueBox.SetValues(oOutputNames);
	m_oInputValueBox.SetValues(oInputNames);
	m_oKeyOffsetValueBox.SetMode(VALUE_BOX_DRAW_BLANK);
	m_oVelocityOffsetValueBox.SetMode(VALUE_BOX_DRAW_BLANK);
	m_oTimeOffsetValueBox.SetMode(VALUE_BOX_DRAW_BLANK);

	m_oRenderButton.SetMode(EM_SLIM_BUTTON_DRAW_BLANK);
	*/
}

/*
void AudioGTrackInfo::CalculateAndSetHidden(bool p_vClipEdit)
{
	m_vClipEdit = p_vClipEdit;
	if((m_vClipEdit && !IsSelected()) || !(TrackInfoButtonBar::Instance() -> GetAudioTrackFilterOn()))
		SetHidden(true);
	else
		SetHidden(false);
}
*/
EMGTrackInfo* AudioGTrackInfo::CreateInstance()
{
	return new AudioGTrackInfo(m_opView, m_opButtonBar);
}

void AudioGTrackInfo::EditModeChanged(EMEditMode p_eEditMode)
{
	if(p_eEditMode == EM_TRACK_EDIT_MODE || p_eEditMode == EM_FX_EDIT_MODE)
		m_oSelectionButton.SetMode(SELECTION_BUTTON_MODE_NORMAL);
	else
		m_oSelectionButton.SetMode(SELECTION_BUTTON_MODE_AUDIO);
}

EMMediaType AudioGTrackInfo::GetType()
{
	return EM_TYPE_ANY_AUDIO;
}

bool AudioGTrackInfo::IsHidden(bool p_vClipEdit)
{
//	m_vClipEdit = p_vClipEdit;
	return (p_vClipEdit && !IsSelected()) || !(TrackInfoButtonBar::Instance() -> GetAudioTrackFilterOn());
/*
	if((m_vClipEdit && !IsSelected()) || !(TrackInfoButtonBar::Instance() -> GetAudioTrackFilterOn()))
		SetHidden(true);
	else
		SetHidden(false);
*/
}
/*
void AudioGTrackInfo::SetUserMode(bool p_vExpertMode)
{
	GTrackInfo::SetUserMode(p_vExpertMode);
	m_oRenderButton.SetMode(EM_SLIM_BUTTON_DRAW_BLANK);
}
*/
