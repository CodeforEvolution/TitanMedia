#include "MIDIGTrackInfo.h"

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

MIDIGTrackInfo* MIDIGTrackInfo::m_opActiveTrackInfo = NULL;

MIDIGTrackInfo::MIDIGTrackInfo(EMView* p_opView, TrackInfoButtonBar* p_opButtonBar) :
GTrackInfo(p_opView, p_opButtonBar),
m_vIsActive(false)
{
	m_oEditLabel.SetBackgroundColor(EM_COLOR_MIDI_VIEW_BACKGROUND);
	string oInputName;
	string oOutputName;
	list<int32> oOutputIDs;
	list<string*> oOutputNames;
	list<int32> oInputIDs;
	list<string*> oInputNames;
	EMMediaType eMediaType = EM_TYPE_MIDI;
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

	m_oRenderButton.SetMode(EM_SLIM_BUTTON_DRAW_BLANK);
	/*
	m_oEditLabel.SetBackgroundColor(EMColor(160, 255, 255, 255));
	string oOutputName;
	string oInputName;
	list<int32> oOutputIDs;
	vector<string> oOutputNames;
	list<int32> oInputIDs;
	vector<string> oInputNames;
	EMMediaType eMediaType = EM_TYPE_MIDI;
	oOutputIDs = *(static_cast<list<int32>* >(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_OUTPUTS, &eMediaType, NULL, NULL)));
	oInputIDs = *(static_cast<list<int32>* >(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_INPUTS, &eMediaType, NULL, NULL)));
	list<int32>::iterator oIterator = oOutputIDs.begin();
	while(oIterator != oOutputIDs.end())
	{
		oOutputName = string(static_cast<const char*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_OUTPUT_NAME, &(*oIterator))));
		oOutputNames.push_back(oOutputName);
		oIterator++;
	}
	oIterator = oInputIDs.begin();
	while(oIterator != oInputIDs.end())
	{
		oInputName = string(static_cast<const char*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_INPUT_NAME, &(*oIterator))));
		oInputNames.push_back(oInputName);
		oIterator++;
	}
	
	m_oOutputValueBox.SetValues(oOutputNames);
	m_oInputValueBox.SetValues(oInputNames);

	m_oRenderButton.SetMode(EM_SLIM_BUTTON_DRAW_BLANK);
	*/
}

MIDIGTrackInfo::~MIDIGTrackInfo()
{
	if(m_opActiveTrackInfo == this)
		m_opActiveTrackInfo = NULL;
}

EMGTrackInfo* MIDIGTrackInfo::CreateInstance()
{
	return new MIDIGTrackInfo(m_opView, m_opButtonBar);
}

void MIDIGTrackInfo::Draw(EMRect p_oClippingRect, EMView* p_opView)
{
	GTrackInfo::Draw(p_oClippingRect, p_opView);
	if(m_vIsActive)
	{
		EMRect oRect(m_oEditLabel.Frame());
		oRect.InsetBy(3, 3);
		oRect.m_vLeft = oRect.m_vRight - oRect.GetHeight();
		p_opView -> SetHighColor(EMColor(0, 0, 0, 255));
		p_opView -> StrokeRect(oRect);
/*
		p_opView -> SetHighColor(EMColor(200, 255, 200, 255));
		p_opView -> StrokeLine(oRect.m_vLeft, oRect.m_vTop, oRect.m_vRight - 1, oRect.m_vTop);
		p_opView -> StrokeLine(oRect.m_vLeft, oRect.m_vTop + 1, oRect.m_vLeft, oRect.m_vBottom);
		p_opView -> SetHighColor(EMColor(0, 100, 0, 255));
		p_opView -> StrokeLine(oRect.m_vLeft + 1, oRect.m_vBottom, oRect.m_vRight, oRect.m_vBottom);
		p_opView -> StrokeLine(oRect.m_vRight, oRect.m_vTop + 1, oRect.m_vRight, oRect.m_vBottom - 1);
*/
		oRect.InsetBy(1, 1);
		p_opView -> SetHighColor(EMColor(0, 255, 0, 255));
		p_opView -> FillRect(oRect);
	}
}

void MIDIGTrackInfo::EditModeChanged(EMEditMode p_eEditMode)
{
	if(p_eEditMode == EM_TRACK_EDIT_MODE || p_eEditMode == EM_FX_EDIT_MODE)
		m_oSelectionButton.SetMode(SELECTION_BUTTON_MODE_NORMAL);
	else
		m_oSelectionButton.SetMode(SELECTION_BUTTON_MODE_MIDI_KEYS);
}

MIDIGTrackInfo* MIDIGTrackInfo::GetActiveTrackInfo()
{
	return m_opActiveTrackInfo;
}

EMMediaType MIDIGTrackInfo::GetType()
{
	return EM_TYPE_MIDI;
}

bool MIDIGTrackInfo::IsHidden(bool p_vClipEdit)
{
	return (p_vClipEdit && !IsSelected()) || !(TrackInfoButtonBar::Instance() -> GetMIDITrackFilterOn());
}

void MIDIGTrackInfo::MouseDown(int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree, EMView* p_opView)
{
	GTrackInfo::MouseDown(p_vX, p_vY, p_vButtonOne, p_vButtonTwo, p_vButtonThree, p_opView);
	if(m_oEditLabel.Frame().Contains(p_vX, p_vY) && m_vIsActive == false)
	{
		m_vIsActive = true;
		if(m_opActiveTrackInfo != NULL)
		{
			m_opActiveTrackInfo -> m_vIsActive = false;
			m_oUpdateRect = m_oUpdateRect | (m_opActiveTrackInfo -> m_oEditLabel).Frame();
		}
		m_opActiveTrackInfo = this;
		m_oUpdateRect = m_oUpdateRect | m_oEditLabel.Frame();

		int32 vMediaTrackID = GetMediaTrackID();
		EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_SET_ACTIVE_MIDI_TRACK, &vMediaTrackID);
	}
}

void MIDIGTrackInfo::SetActive(bool p_vActive)
{
	if(p_vActive)
	{
		m_opActiveTrackInfo = this;
		int32 vMediaTrackID = GetMediaTrackID();
		EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_SET_ACTIVE_MIDI_TRACK, &vMediaTrackID);
	}
	else if(m_opActiveTrackInfo == this)
		m_opActiveTrackInfo = NULL;
	m_vIsActive = p_vActive;
}
/*
void MIDIGTrackInfo::SetUserMode(bool p_vExpertMode)
{
	GTrackInfo::SetUserMode(p_vExpertMode);
	m_oRenderButton.SetMode(EM_SLIM_BUTTON_DRAW_BLANK);
}
*/