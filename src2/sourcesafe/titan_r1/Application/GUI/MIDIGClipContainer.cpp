#include "MIDIGClipContainer.h"

#include "EMGClipRepository.h"
#include "EMGRubberBand.h"
#include "EMGTrack.h"
#include "EMGUIGlobals.h"
#include "EMGUIUtilities.h"
#include "EMKeyboard.h"
#include "EMKeyboardGlobals.h"
#include "EMListenerRepository.h"
#include "EMMediaMIDIEvent.h"
#include "EMMouseCursor.h"
//#include "EMMessages.h"
#include "EMSettingIDs.h"
#include "EMSettingsRepository.h"
#include "EMView.h"
#include "GTrackInfo.h"
#include "GUIGlobals.h"
#include "MIDIGClip.h"
#include "SettingIDs.h"
#include "TrackWindowStatusBar.h"

#include <algorithm>

const int LOWER_BORDER_SIZE = 3;

EMRect MIDIGClipContainer::m_oMovedRect = EMRect();
EMRect MIDIGClipContainer::m_oOldMovedRect = EMRect();
EMRect MIDIGClipContainer::m_oSelectedMIDIEventFrame = EMRect();

MIDIGClipContainer::MIDIGClipContainer(EMRect p_oFrame, EMView* p_opView) :
EMGClipContainer(p_oFrame, p_opView),
m_eDisplayMode(CE_DISPLAY_MODE_DEFAULT),
m_opMouseDownClip(NULL),
m_opMouseDownEvent(NULL),
m_opRubberBand(NULL),
m_opSelectionButton(NULL),
m_vDrawEventsLength(1000000), // 1 second
m_vDrawEventsVelocity(64),
m_vHasResized(false),
m_vMouseIsDown(false),
m_vMoveDrawedEvent(false)
{
}

MIDIGClipContainer::~MIDIGClipContainer()
{
	delete m_opRubberBand;
}

void MIDIGClipContainer::BuildRubberBand(EMGRubberBand* p_opRubberBand, list<EMMediaMIDIEvent*>* p_opEvents, 
							EMMIDIEventType p_eType, EMMIDIControlChangeType p_eControlChangeType)
{
	p_opRubberBand -> ClearNodes(false);

	list<EMMediaMIDIEvent*>::iterator oIterator = p_opEvents -> begin();
	while(oIterator != p_opEvents -> end())
	{
		if(p_eType == EM_MIDI_EVENT_TYPE_CONTROL_CHANGE || p_eType == EM_MIDI_EVENT_TYPE_NOTE_ON)
		{
			if((*oIterator) -> m_vBaseType == p_eType && (p_eType == EM_MIDI_EVENT_TYPE_NOTE_ON || (*oIterator) -> m_vData1 == p_eControlChangeType))
				p_opRubberBand -> CreateNode(*((*oIterator) -> m_vpStart), (*oIterator) -> m_vData2, *oIterator, false, false);
		}
		else if((*oIterator) -> m_vBaseType == p_eType)
		{
			if(p_eType == EM_MIDI_EVENT_TYPE_PITCH_WHEEL_CONTROL)
			{
				int32 vValue = ((*oIterator) -> m_vData2) << 7;
				vValue += (*oIterator) -> m_vData1;
				p_opRubberBand -> CreateNode(*((*oIterator) -> m_vpStart), vValue, *oIterator, false, false);
			}
			else
				p_opRubberBand -> CreateNode(*((*oIterator) -> m_vpStart), (*oIterator) -> m_vData1, *oIterator, false, false);
		}

		oIterator++;
	}
}

// The events do not have to be in order
// Assumes that all events in the list are of type "note on"
EMRect MIDIGClipContainer::CalculateEventFrame(list<EMMediaMIDIEvent*>* p_opEvents)
{
	if(p_opEvents -> size() == 0)
		return EMRect();

	EMRect oRect;
	oRect.m_vTop = INT32_MAX;
	oRect.m_vBottom = -1;
	oRect.m_vLeft = INT32_MAX;//*(p_opEvents -> front() -> m_vpStart) / m_vTimeZoomScale;
	oRect.m_vRight = -1;//*(p_opEvents -> back() -> m_opNoteOff -> m_vpStart) / m_vTimeZoomScale;

	int vKey;
	int32 vTop;
	int32 vBottom;
	int32 vLeft;
	int32 vRight;
	list<EMMediaMIDIEvent*>::iterator oIterator = p_opEvents -> begin();
	while(oIterator != p_opEvents -> end())
	{
		vKey = (*oIterator) -> m_vData1;
		vTop = Frame().m_vBottom - LOWER_BORDER_SIZE - ((MIDI_KEY_HEIGHT - 1) * (vKey - GetLowestKey() + 1)) + 1;
		if(vTop < oRect.m_vTop)
			oRect.m_vTop = vTop;
		vBottom = Frame().m_vBottom - LOWER_BORDER_SIZE - ((MIDI_KEY_HEIGHT - 1) * (vKey - GetLowestKey())) - 1;
		if(vBottom > oRect.m_vBottom)
			oRect.m_vBottom = vBottom;
		vLeft = *((*oIterator) -> m_vpStart) / m_vTimeZoomScale;
		if(vLeft < oRect.m_vLeft)
			oRect.m_vLeft = vLeft;
		vRight = *((*oIterator) -> m_opNoteOff -> m_vpStart) / m_vTimeZoomScale;
		if(vRight > oRect.m_vRight)
			oRect.m_vRight = vRight;

		oIterator++;
	}

	return oRect;
}

void MIDIGClipContainer::ChangeDisplayMode()
{
	int32 vMediaTrackID = GetTrack() -> GetTrackInfo() -> GetMediaTrackID();
	list<EMMediaMIDIEvent*>* opEvents = static_cast<list<EMMediaMIDIEvent*>*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_MIDI_EVENTS_FOR_TRACK, &vMediaTrackID));

	if(m_eDisplayMode == CE_DISPLAY_MODE_MIDI_PITCH_BEND)
	{
		m_opRubberBand -> SetDrawMode(EM_RUBBERBAND_DRAW_SAMPLEHOLD);
		m_opRubberBand -> SetMinMax(0, 16383);
		m_opRubberBand -> SetDefaultValue(8192);
		m_opRubberBand -> SetPermissions(true, true, true);
		BuildRubberBand(m_opRubberBand, opEvents, EM_MIDI_EVENT_TYPE_PITCH_WHEEL_CONTROL);
	}
	else if(m_eDisplayMode == CE_DISPLAY_MODE_MIDI_AFTERTOUCH)
	{
		m_opRubberBand -> SetDrawMode(EM_RUBBERBAND_DRAW_SAMPLEHOLD);
		m_opRubberBand -> SetMinMax(0, 127);
		m_opRubberBand -> SetDefaultValue(0);
		m_opRubberBand -> SetPermissions(true, true, true);
		BuildRubberBand(m_opRubberBand, opEvents, EM_MIDI_EVENT_TYPE_CHANNEL_AFTERTOUCH);
	}
	else if(m_eDisplayMode == CE_DISPLAY_MODE_MIDI_MODULATION_WHEEL)
	{
		m_opRubberBand -> SetDrawMode(EM_RUBBERBAND_DRAW_SAMPLEHOLD);
		m_opRubberBand -> SetMinMax(0, 127);
		m_opRubberBand -> SetDefaultValue(0);
		m_opRubberBand -> SetPermissions(true, true, true);
		BuildRubberBand(m_opRubberBand, opEvents, EM_MIDI_EVENT_TYPE_CONTROL_CHANGE, EM_MIDI_CONTROL_CHANGE_TYPE_MODULATION_WHEEL_MSB);
	}
	else if(m_eDisplayMode == CE_DISPLAY_MODE_MIDI_VELOCITY)
	{
		m_opRubberBand -> SetDrawMode(EM_RUBBERBAND_DRAW_BARS);
		m_opRubberBand -> SetMinMax(0, 127);
		m_opRubberBand -> SetDefaultValue(127);
		m_opRubberBand -> SetPermissions(false, false, true);
		BuildRubberBand(m_opRubberBand, opEvents, EM_MIDI_EVENT_TYPE_NOTE_ON);
	}

	m_opView -> Invalidate(Frame() & m_opView -> Bounds());
}

EMGClipContainer* MIDIGClipContainer::CreateInstance()
{
	return new MIDIGClipContainer(EMRect(), m_opView);
}

bool MIDIGClipContainer::DeselectAll(EMGClipContainer* p_opNewSelectionOwner)
{
	EMRect oOldSelectedFrame = m_oSelectedMIDIEventFrame;
	m_oSelectedMIDIEventFrame = EMRect();
	//m_oSelectedMIDIEvents.clear();

	while(m_oSelectedMIDIEvents.size() > 0)
	{
		m_oSelectedMIDIEvents.front() -> m_vIsSelected = false;
		m_oSelectedMIDIEvents.pop_front();
	}

	list<EMGClip*>::iterator oClipIterator = m_oClipRepository.begin();
	while(oClipIterator != m_oClipRepository.end())
	{
		static_cast<MIDIGClip*>(*oClipIterator) -> DeselectAll();
		oClipIterator++;
	}

	m_opRubberBand -> DeselectAllNodes();

	if(p_opNewSelectionOwner != this)
		m_opClipContainerWithSelection = NULL;

	m_opView -> Invalidate(Frame() & m_opView -> Bounds());//oOldSelectedFrame);

	return true;
}

void MIDIGClipContainer::DrawAfterClips(EMRect p_oClippingRect, EMView* p_opView, EMEditMode p_eEditMode)
{
	if(p_eEditMode == EM_CLIP_EDIT_MODE)
	{
		if(m_eDisplayMode != CE_DISPLAY_MODE_DEFAULT)// && m_eDisplayMode != CE_DISPLAY_MODE_MIDI_VELOCITY)
			m_opRubberBand -> Draw(p_oClippingRect);

		else if(m_opClipContainerWithSelection == this && m_oSelectedMIDIEventFrame.IsValid())// && m_eDisplayMode == CE_DISPLAY_MODE_DEFAULT)
		{
			// Let's draw the box around selected events
			m_opView -> SetHighColor(EM_COLOR_CLIP_SELECTION_BOX);
			EMRect oDrawFrame = m_oSelectedMIDIEventFrame & p_oClippingRect;
			if(oDrawFrame.IsValid())
			{
				m_opView -> StrokeLine(oDrawFrame.m_vLeft, m_oSelectedMIDIEventFrame.m_vTop, oDrawFrame.m_vRight, m_oSelectedMIDIEventFrame.m_vTop);
				m_opView -> StrokeLine(oDrawFrame.m_vLeft, m_oSelectedMIDIEventFrame.m_vBottom, oDrawFrame.m_vRight, m_oSelectedMIDIEventFrame.m_vBottom);
				m_opView -> StrokeLine(m_oSelectedMIDIEventFrame.m_vLeft, m_oSelectedMIDIEventFrame.m_vTop, m_oSelectedMIDIEventFrame.m_vLeft, m_oSelectedMIDIEventFrame.m_vBottom);
				m_opView -> StrokeLine(m_oSelectedMIDIEventFrame.m_vRight, m_oSelectedMIDIEventFrame.m_vTop, m_oSelectedMIDIEventFrame.m_vRight, m_oSelectedMIDIEventFrame.m_vBottom);
			}

			// Let's draw the box around moving events
			if(m_oMovedRect.IsValid() && m_oMovedRect != m_oSelectedMIDIEventFrame)
			{
				m_opView -> SetHighColor(EM_COLOR_CLIP_MOVE_BOX);
				EMRect oDrawFrame = m_oMovedRect & p_oClippingRect;
				if(oDrawFrame.IsValid())
				{
					m_opView -> StrokeLine(oDrawFrame.m_vLeft, m_oMovedRect.m_vTop, oDrawFrame.m_vRight, m_oMovedRect.m_vTop);
					m_opView -> StrokeLine(oDrawFrame.m_vLeft, m_oMovedRect.m_vBottom, oDrawFrame.m_vRight, m_oMovedRect.m_vBottom);
					m_opView -> StrokeLine(m_oMovedRect.m_vLeft, m_oMovedRect.m_vTop, m_oMovedRect.m_vLeft, m_oMovedRect.m_vBottom);
					m_opView -> StrokeLine(m_oMovedRect.m_vRight, m_oMovedRect.m_vTop, m_oMovedRect.m_vRight, m_oMovedRect.m_vBottom);
				}
			}
		}
	}
}

void MIDIGClipContainer::DrawSelection(EMRect p_oClippingRect, EMView* p_opView)
{
	if(this != m_opClipContainerWithSelection)
		return;

	if(p_oClippingRect.IsValid())
	{
		if(p_oClippingRect.Intersects(m_oSelectionRect))
		{
			m_opView -> SetDrawingMode(EM_OP_INVERT);
			m_opView -> StrokeRect(m_oSelectionRect);
			m_opView -> SetDrawingMode(EM_OP_COPY);
		}
	}
	else
	{
		m_opView -> BeginPaint();
		m_opView -> EnableDoubleBuffering(false);
		m_opView -> SetDrawingMode(EM_OP_INVERT);
		if(m_oOldSelectionRect.IsValid())
			m_opView -> StrokeRect(m_oOldSelectionRect);
		if(m_oSelectionRect.IsValid())
			m_opView -> StrokeRect(m_oSelectionRect);
		m_oOldSelectionRect = m_oSelectionRect;
		m_opView -> SetDrawingMode(EM_OP_COPY);
		m_opView -> EnableDoubleBuffering(true);
		m_opView -> EndPaint();
	}
}

void MIDIGClipContainer::DrawTrack(EMRect p_oClippingRect, EMView* p_opView, EMEditMode p_eEditMode)
{
	if(!(Frame().Intersects(p_oClippingRect)))
		return;

	if(m_opRubberBand == NULL)
	{
		// Lazy init
		m_opRubberBand = EM_new EMGRubberBand(m_opView, 127, 0, 64, EM_RUBBERBAND_DRAW_BARS);
		m_opRubberBand -> SetFrame(EMRect(Frame().m_vLeft, Frame().m_vTop, Frame().m_vRight, Frame().m_vBottom - LOWER_BORDER_SIZE));
		m_opRubberBand -> SetTimeZoomScale(m_vTimeZoomScale);
		m_opRubberBand -> SetDrawMode(EM_RUBBERBAND_DRAW_SAMPLEHOLD);
		m_opRubberBand -> SetPermissions(true, true, true);
		m_opRubberBand -> AddListener(this);
	}

	EMRect oRect = Frame();
	oRect.m_vBottom -= LOWER_BORDER_SIZE;
	oRect = oRect & p_oClippingRect;
	if(p_eEditMode == EM_CLIP_EDIT_MODE)
	{
		p_opView -> SetHighColor(EM_COLOR_VIEW_BACKGROUND_ULTRA_HIGHLIGHT);
		p_opView -> FillRect(oRect);
		int32 vCurrentPos(Frame().m_vBottom - LOWER_BORDER_SIZE);
		p_opView -> SetHighColor(EM_COLOR_VIEW_BACKGROUND_HIGHLIGHT);
		while(vCurrentPos >= oRect.m_vTop)
		{
			p_opView -> StrokeLine(oRect.m_vLeft, vCurrentPos, oRect.m_vRight, vCurrentPos);
			vCurrentPos -= MIDI_KEY_HEIGHT - 1;
		}
	}
	else
	{
		p_opView -> SetHighColor(EM_COLOR_VIEW_BACKGROUND);
		p_opView -> FillRect(oRect);
	}

	p_opView -> SetHighColor(EM_COLOR_VIEW_BACKGROUND_HIGHLIGHT);
	p_opView -> StrokeLine(p_oClippingRect.m_vLeft, Frame().m_vBottom - 2, p_oClippingRect.m_vRight, Frame().m_vBottom - 2);
	p_opView -> SetHighColor(EM_COLOR_VIEW_BACKGROUND);
	p_opView -> StrokeLine(p_oClippingRect.m_vLeft, Frame().m_vBottom - 1, p_oClippingRect.m_vRight, Frame().m_vBottom - 1);
	p_opView -> SetHighColor(EM_COLOR_VIEW_BACKGROUND_SHADOW);
	p_opView -> StrokeLine(p_oClippingRect.m_vLeft, Frame().m_vBottom, p_oClippingRect.m_vRight, Frame().m_vBottom);
}

ClipEditDisplayMode MIDIGClipContainer::GetDisplayMode()
{
	return m_eDisplayMode;
}

uint8 MIDIGClipContainer::GetLowerBorderSize()
{
	return LOWER_BORDER_SIZE;
}

int MIDIGClipContainer::GetLowestKey()
{
	if(m_opSelectionButton != NULL)
		return m_opSelectionButton -> GetLowestKey();
	return 0;
}

EMGRubberBand* MIDIGClipContainer::GetRubberband()
{
	return m_opRubberBand;
}

list<EMMediaMIDIEvent*>* MIDIGClipContainer::GetSelectedMIDIEvents()
{
	return &m_oSelectedMIDIEvents;
}

uint8 MIDIGClipContainer::GetUpperBorderSize()
{
	return 0;
}

bool MIDIGClipContainer::MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage)
{
	// For these messages, assume that we are in clip edit mode
	switch(p_vMessage)
	{
	case SELECTION_BUTTON_LOWEST_KEY_CHANGED:
		Scroll(m_opSelectionButton -> GetLowestKey());
		return true;
	case SELECTION_BUTTON_DISPLAY_MODE_CHANGED:
		m_eDisplayMode = m_opSelectionButton -> GetDisplayMode();
		ChangeDisplayMode();
		return true;
	case EM_RUBBERBAND_NODES_ADDED: // Note: Velocity nodes cannot be added and are therefore not checked here
		{
			list<EMGRubberBandNode*>* opAffectedNodes = m_opRubberBand -> GetAffectedNodes();
			if(opAffectedNodes -> size() == 0)
				return true; // Nothing to do

			int64* vpEventData = EM_new int64[opAffectedNodes -> size() * 5];
			int vEventDataIndex(0);
			list<EMGRubberBandNode*>::iterator oNodeIterator = opAffectedNodes -> begin();
			while(oNodeIterator != opAffectedNodes -> end())
			{
				if(m_eDisplayMode == CE_DISPLAY_MODE_MIDI_PITCH_BEND)
				{
					vpEventData[vEventDataIndex] = EM_MIDI_EVENT_TYPE_PITCH_WHEEL_CONTROL;
					vpEventData[vEventDataIndex + 3] = 3;
				}
				else if(m_eDisplayMode == CE_DISPLAY_MODE_MIDI_MODULATION_WHEEL)
				{
					vpEventData[vEventDataIndex] = EM_MIDI_EVENT_TYPE_CONTROL_CHANGE;
					vpEventData[vEventDataIndex + 1] = EM_MIDI_CONTROL_CHANGE_TYPE_MODULATION_WHEEL_MSB;
					vpEventData[vEventDataIndex + 3] = 3;
				}
				else // aftertouch
				{
					vpEventData[vEventDataIndex] = EM_MIDI_EVENT_TYPE_CHANNEL_AFTERTOUCH;
					vpEventData[vEventDataIndex + 3] = 2;
				}
				if(m_eDisplayMode == CE_DISPLAY_MODE_MIDI_PITCH_BEND)
				{
					vpEventData[vEventDataIndex + 1] = ((*oNodeIterator) -> m_vValue) & 0x7F;
					uint16 vPitch = ((*oNodeIterator) -> m_vValue) & 0x3F80;
					vpEventData[vEventDataIndex + 2] = vPitch >> 7;
				}
				else if(m_eDisplayMode == CE_DISPLAY_MODE_MIDI_MODULATION_WHEEL)
					vpEventData[vEventDataIndex + 2] = (*oNodeIterator) -> m_vValue;
				else
					vpEventData[vEventDataIndex + 1] = (*oNodeIterator) -> m_vValue;
				vpEventData[vEventDataIndex + 4] = (*oNodeIterator) -> m_vPosition;

				vEventDataIndex += 5;
				oNodeIterator++;
			}

			int32 vpParameters[3];
			vpParameters[0] = GetTrack() -> GetTrackInfo() -> GetMediaTrackID();
			vpParameters[1] = -1;
			vpParameters[2] = opAffectedNodes -> size();

			list<EMMediaMIDIEvent*>* opMIDIEvents = static_cast<list<EMMediaMIDIEvent*>*>(EMCommandRepository::Instance() -> ExecuteCommandNoUndo(MEDIA_COMMAND_CREATE_MIDI_EVENT, vpParameters, vpEventData));
			EMCommand* opMediaUndoCommand = EMCommandRepository::Instance() -> GetUndoClone(MEDIA_COMMAND_CREATE_MIDI_EVENT);

			delete [] vpEventData;

			oNodeIterator = opAffectedNodes -> begin();
			list<EMMediaMIDIEvent*>::iterator oEventIterator = opMIDIEvents -> begin();
			while(oEventIterator != opMIDIEvents -> end())
			{
				(*oNodeIterator) -> m_opDataLink = *oEventIterator;
				oEventIterator++;
				oNodeIterator++;
			}

			list<EMCommand*> oMediaUndoCommands;
			oMediaUndoCommands.push_back(opMediaUndoCommand);

			EMCommandRepository::Instance() -> ExecuteCommand(UNDO_COMMAND_ADD_RUBBERBAND_NODES, m_opRubberBand, opAffectedNodes, &oMediaUndoCommands);

			list<EMGClip*>::iterator oIterator = m_oClipRepository.begin();
			while(oIterator != m_oClipRepository.end())
			{
				static_cast<MIDIGClip*>(*oIterator) -> UpdateFrame();
				oIterator++;
			}

			return true;
		}
	case EM_RUBBERBAND_NODES_DELETED: // Note: Velocity nodes cannot be deleted and are therefore not checked here
		{
			list<EMGRubberBandNode*>* opAffectedNodes = m_opRubberBand -> GetAffectedNodes();
			if(opAffectedNodes -> size() == 0)
				return true; // Nothing to do

			list<EMMediaMIDIEvent*> oEventsToDelete;
			EMCommand* opMediaUndoCommand;
			list<EMCommand*> oMediaUndoCommands;

			list<EMGRubberBandNode*>::iterator oNodeIterator = opAffectedNodes -> begin();
			while(oNodeIterator != opAffectedNodes -> end())
			{
				oEventsToDelete.push_back(static_cast<EMMediaMIDIEvent*>((*oNodeIterator) -> m_opDataLink));
				oNodeIterator++;
			}

			EMCommandRepository::Instance() -> ExecuteCommandNoUndo(MEDIA_COMMAND_DELETE_MIDI_EVENTS, &oEventsToDelete);
			opMediaUndoCommand = EMCommandRepository::Instance() -> GetUndoClone(MEDIA_COMMAND_DELETE_MIDI_EVENTS);
			if(opMediaUndoCommand != NULL)
				oMediaUndoCommands.push_back(opMediaUndoCommand);

			EMCommandRepository::Instance() -> ExecuteCommand(UNDO_COMMAND_DELETE_RUBBERBAND_NODES, m_opRubberBand, opAffectedNodes, &oMediaUndoCommands);

			return true;
		}
	case EM_RUBBERBAND_POSITIONS_CHANGED: // Is sent when either values or positions of nodes have changed
		{
			list<EMGRubberBandNode*>* opAffectedNodes = m_opRubberBand -> GetAffectedNodes();
			if(opAffectedNodes -> size() == 0)
				return true; // Nothing to do

			list<EMMediaMIDIEvent*> oEventsToChange;
			list<EMGRubberBandNode*>::iterator oNodeIterator = opAffectedNodes -> begin();
			while(oNodeIterator != opAffectedNodes -> end())
			{
				if(m_eDisplayMode == CE_DISPLAY_MODE_MIDI_PITCH_BEND)
				{
eo << "New rubberband node value: " << (*oNodeIterator) -> m_vValue << ef;
					static_cast<EMMediaMIDIEvent*>((*oNodeIterator) -> m_opDataLink) -> m_vData1 = ((*oNodeIterator) -> m_vValue) & 0x7F;
					uint16 vPitch = ((*oNodeIterator) -> m_vValue) & 0x3F80;
					static_cast<EMMediaMIDIEvent*>((*oNodeIterator) -> m_opDataLink) -> m_vData2 = vPitch >> 7;
				}
				else if(m_eDisplayMode == CE_DISPLAY_MODE_MIDI_MODULATION_WHEEL || m_eDisplayMode == CE_DISPLAY_MODE_MIDI_VELOCITY)
					static_cast<EMMediaMIDIEvent*>((*oNodeIterator) -> m_opDataLink) -> m_vData2 = (*oNodeIterator) -> m_vValue;
				else
					static_cast<EMMediaMIDIEvent*>((*oNodeIterator) -> m_opDataLink) -> m_vData1 = (*oNodeIterator) -> m_vValue;

				oEventsToChange.push_back(static_cast<EMMediaMIDIEvent*>((*oNodeIterator) -> m_opDataLink));
				oNodeIterator++;
			}

			// Find out the time offset (all affected nodes have been moved the same amount)
			int64 vNodePosition = opAffectedNodes -> front() -> m_vPosition;
			int64 vEventPosition = *(static_cast<EMMediaMIDIEvent*>(opAffectedNodes -> front() -> m_opDataLink) -> m_vpStart);
			int64 vOffset = vNodePosition - vEventPosition;

			EMCommandRepository::Instance() -> ExecuteCommandNoUndo(MEDIA_COMMAND_MOVE_MIDI_EVENTS, &oEventsToChange, &vOffset);
			list<EMCommand*> oMediaUndoCommands;

			EMCommandRepository::Instance() -> ExecuteCommand(UNDO_COMMAND_MOVE_RUBBERBAND_NODES, m_opRubberBand, opAffectedNodes, &oMediaUndoCommands);

			list<EMGClip*>::iterator oIterator = m_oClipRepository.begin();
			while(oIterator != m_oClipRepository.end())
			{
				static_cast<MIDIGClip*>(*oIterator) -> UpdateFrame();
				oIterator++;
			}

			return true;
		}
	}

	return false;
}

void MIDIGClipContainer::MouseDoubleClicked(EMView* p_opView, int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree, EMEditMode p_eEditMode, EMGClip* p_opClip)
{
}

void MIDIGClipContainer::MouseDown(EMView* p_opView, int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree, EMEditMode p_eEditMode)
{
	if(p_eEditMode != EM_CLIP_EDIT_MODE)
		return;

	if(!Frame().Contains(p_vX, p_vY))
		return;

	m_vMouseIsDown = true;
	m_vMouseDownXPos = p_vX * m_vTimeZoomScale;
	m_vMouseDownY = p_vY;

	if(m_opClipContainerWithSelection != NULL && m_opClipContainerWithSelection != this) // First, remove any previous track selection if the selection isn't in this track (if possible)
	{
		if(!(m_opClipContainerWithSelection -> DeselectAll(this)))
			return;

		//m_opClipContainerWithSelection = this;//NULL;
	}
	m_opClipContainerWithSelection = this;

	m_opMouseDownClip = dynamic_cast<MIDIGClip*>(EMGClipRepository::Instance() -> GetClip(p_vX, p_vY));
	if(m_opMouseDownClip != NULL && m_eDisplayMode == CE_DISPLAY_MODE_DEFAULT) // Only look for MIDI note on events if rubberbands aren't active
		m_opMouseDownEvent = m_opMouseDownClip -> GetMIDIEvent(p_vX, p_vY);
	else
		m_opMouseDownEvent = NULL;

	bool vRubberBandNodeHit(false);

	ToolboxMode eToolboxMode = static_cast<ToolboxMode>(*(static_cast<int*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_TOOLBOX_MODE))));
	/*
	if(m_eDisplayMode == CE_DISPLAY_MODE_MIDI_VELOCITY)
	{
		if(m_opMouseDownClip != NULL)
			vRubberBandNodeHit = m_opMouseDownClip -> MouseDown(p_opView, p_vX, p_vY, p_vButtonOne, p_vButtonTwo, p_vButtonThree);

		if(vRubberBandNodeHit)
			return;
	}
	
	else */if(m_eDisplayMode != CE_DISPLAY_MODE_DEFAULT)
	{
		vRubberBandNodeHit = m_opRubberBand -> MouseDown(p_vX, p_vY, p_vButtonOne, p_vButtonTwo, p_vButtonThree, eToolboxMode);

		if(vRubberBandNodeHit)
			return;
	}
	/*
	m_vMouseIsDown = true;
	m_vMouseDownXPos = p_vX * m_vTimeZoomScale;
	m_vMouseDownY = p_vY;
	*/
	bool vSomethingChanged = false;

	if(eToolboxMode == TOOLBOX_SELECT)
	{
		uint32 vModifiers = EMKeyboard::Instance() -> GetModifiers();

		if((vModifiers & EM_COMMAND_KEY) == EM_COMMAND_KEY)
			return;

		if(m_opMouseDownEvent == NULL)// || (vModifiers & EM_SHIFT_KEY) == EM_SHIFT_KEY)
		{
			m_oSelectionRect = EMRect(p_vX, p_vY, p_vX, p_vY);
			m_oOldSelectionRect = EMRect();
			//m_opClipContainerWithSelection = this;
			DrawSelection(EMRect(), p_opView);
		}

		EMRect oOldSelectedMIDIEventFrame = m_oSelectedMIDIEventFrame;

		if((vModifiers & EM_SHIFT_KEY) == EM_SHIFT_KEY)
		{
			if(m_opMouseDownEvent == NULL)
				return;

			if(m_opMouseDownEvent -> m_vIsSelected)
			{
				m_opMouseDownEvent -> m_vIsSelected = false;
				m_oSelectedMIDIEvents.remove(m_opMouseDownEvent);
			}
			else
			{
				m_opMouseDownEvent -> m_vIsSelected = true;
				m_oSelectedMIDIEvents.push_back(m_opMouseDownEvent);
			}
			
			char vMessage[80];
			sprintf(vMessage, "%ld event(s) selected", m_oSelectedMIDIEvents.size());
			TrackWindowStatusBar::Instance() -> WriteMessage(string(vMessage));
			vSomethingChanged = true;
		}		

		else if(m_opMouseDownEvent == NULL)
		{
			if(m_oSelectedMIDIEvents.size() > 0)
			{
				while(m_oSelectedMIDIEvents.size() > 0)
				{
					m_oSelectedMIDIEvents.front() -> m_vIsSelected = false;
					m_oSelectedMIDIEvents.pop_front();
				}

				char vMessage[80];
				sprintf(vMessage, "No events selected");
				TrackWindowStatusBar::Instance() -> WriteMessage(string(vMessage));
				vSomethingChanged = true;
			}
		}		
	
		else if(!(m_opMouseDownEvent -> m_vIsSelected))
		{
			while(m_oSelectedMIDIEvents.size() > 0)
			{
				m_oSelectedMIDIEvents.front() -> m_vIsSelected = false;
				m_oSelectedMIDIEvents.pop_front();
			}
			m_opMouseDownEvent -> m_vIsSelected = true;
			m_oSelectedMIDIEvents.push_back(m_opMouseDownEvent);

			char vMessage[80];
			sprintf(vMessage, "%ld event(s) selected", m_oSelectedMIDIEvents.size());
			TrackWindowStatusBar::Instance() -> WriteMessage(string(vMessage));
			vSomethingChanged = true;
		}	

		if(!vSomethingChanged)
			return; // Nothing needs to be redrawn or recalculated

		m_oSelectedMIDIEventFrame = CalculateEventFrame(&m_oSelectedMIDIEvents);
		m_opView -> Invalidate(oOldSelectedMIDIEventFrame | m_oSelectedMIDIEventFrame);
	}
	
	else if(eToolboxMode == TOOLBOX_ERASE)
	{
		if(m_eDisplayMode != CE_DISPLAY_MODE_DEFAULT)
			return;

		if(m_opMouseDownEvent == NULL)
			return;

		list<EMMediaMIDIEvent*> oEventsToDelete;
		oEventsToDelete.push_back(m_opMouseDownEvent);
		oEventsToDelete.push_back(m_opMouseDownEvent -> m_opNoteOff);
		int32 vMediaTrackID = GetTrack() -> GetTrackInfo() -> GetMediaTrackID();
		EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_DELETE_MIDI_EVENTS, &oEventsToDelete, &vMediaTrackID);
	}
	
	else if(eToolboxMode == TOOLBOX_RESIZE)
	{
		if(m_eDisplayMode != CE_DISPLAY_MODE_DEFAULT)
			return;

		if(m_opMouseDownEvent == NULL)
			return;
		
		m_vOriginalPosition = *(m_opMouseDownEvent -> m_vpStart);
		m_vOriginalLength = *(m_opMouseDownEvent -> m_opNoteOff -> m_vpStart) - m_vOriginalPosition + 1;

		// Determine if its a left or right resize
		int64 vStartPos = *(m_opMouseDownEvent -> m_vpStart);
		int64 vEndPos = *(m_opMouseDownEvent -> m_opNoteOff -> m_vpStart);
		int32 vMiddleOfEvent = (vStartPos + ((vEndPos - vStartPos) / 2)) / m_vTimeZoomScale;
		if(p_vX < vMiddleOfEvent)
			m_vResizeLeft = true;
		else
			m_vResizeLeft = false;
	}
	
	else if(eToolboxMode == TOOLBOX_DRAW)
	{
		if(m_eDisplayMode != CE_DISPLAY_MODE_DEFAULT)
			return;

		if(m_opMouseDownEvent)
		{
			// Use this event as a template for new drawn events
			m_vDrawEventsLength = *(m_opMouseDownEvent -> m_opNoteOff -> m_vpStart) - *(m_opMouseDownEvent -> m_vpStart) + 1;
			m_vDrawEventsVelocity = m_opMouseDownEvent -> m_vData2;
		}

		else
		{
			DeselectAll(this);

			list<EMMediaMIDIEvent*> oNewEvents;

			float vKey = static_cast<float>(Frame().m_vBottom - LOWER_BORDER_SIZE - p_vY) / (MIDI_KEY_HEIGHT - 1) + m_opSelectionButton -> GetLowestKey() - 1;
			// Round to nearest key
			if(vKey - static_cast<int>(vKey) < 0.5)
				vKey = static_cast<int>(vKey);
			else
				vKey = static_cast<int>(vKey) + 1;

			int64 m_vSnappedMouseDownPos = m_vMouseDownXPos;
			EMGUIUtilities::Instance() -> GetSnapPosition(&m_vSnappedMouseDownPos, false, false, NULL, true, true);

			// Create a note on/note off pair
			int32 p_vpParameters[3];
			p_vpParameters[0] = GetTrack() -> GetTrackInfo() -> GetMediaTrackID();
			p_vpParameters[1] = -1;
			p_vpParameters[2] = 2;

			int64 vpCommandData[10];
			vpCommandData[0] = EM_MIDI_EVENT_TYPE_NOTE_ON;
			vpCommandData[1] = vKey;
			vpCommandData[2] = m_vDrawEventsVelocity;
			vpCommandData[3] = 3;
			vpCommandData[4] = m_vSnappedMouseDownPos;

			vpCommandData[5] = EM_MIDI_EVENT_TYPE_NOTE_OFF;
			vpCommandData[6] = vKey;
			vpCommandData[7] = m_vDrawEventsVelocity;
			vpCommandData[8] = 3;
			vpCommandData[9] = m_vSnappedMouseDownPos + m_vDrawEventsLength - 1;

			list<EMMediaMIDIEvent*>* opEvents = static_cast<list<EMMediaMIDIEvent*>*>(EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_CREATE_MIDI_EVENTS, p_vpParameters, vpCommandData, this));
			opEvents -> front() -> m_vIsSelected = true;
			m_oSelectedMIDIEvents.push_back(opEvents -> front());
			m_oSelectedMIDIEventFrame = CalculateEventFrame(&m_oSelectedMIDIEvents);
			m_opMouseDownEvent = opEvents -> front();

			m_opView -> Invalidate(Frame() & m_opView -> Bounds());

			m_vMoveDrawedEvent = true;
		}
	}

	else if(eToolboxMode == TOOLBOX_CUT_PASTE)
	{
		if(m_eDisplayMode != CE_DISPLAY_MODE_DEFAULT)
			return;

		if(m_opMouseDownEvent == NULL)
			return;

		list<EMMediaMIDIEvent*> oCutEvents;
		oCutEvents.push_back(m_opMouseDownEvent);
		uint32 vMediaTrackID = GetTrack() -> GetTrackInfo() -> GetMediaTrackID();
		int64 vCutPos = p_vX * m_vTimeZoomScale;
		EMMediaMIDIEvent* opNewEvent = static_cast<EMMediaMIDIEvent*>(EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_CUT_MIDI_EVENTS, &oCutEvents, &vMediaTrackID, &vCutPos));

		if(m_opMouseDownEvent -> m_vIsSelected)
		{
			opNewEvent -> m_vIsSelected = true;
			m_oSelectedMIDIEvents.push_back(opNewEvent);
			m_oSelectedMIDIEventFrame = CalculateEventFrame(&m_oSelectedMIDIEvents);
		}
	}
}

void MIDIGClipContainer::MouseMoved(EMView* p_opView, int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree, EMEditMode p_eEditMode)
{
	//if(!m_vMouseIsDown)
	//	return;

	if(p_eEditMode != EM_CLIP_EDIT_MODE)
		return;

	ToolboxMode eToolboxMode = static_cast<ToolboxMode>(*(static_cast<int*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_TOOLBOX_MODE))));
	
	if(!m_vMouseIsDown)
	{
		if(!Frame().Contains(p_vX, p_vY))
			return;

		if(eToolboxMode == TOOLBOX_RESIZE)
		{
			if(m_eDisplayMode != CE_DISPLAY_MODE_DEFAULT)
				m_opView -> SetMouseCursor(EM_MOUSE_CURSOR_RESIZE_CLIP);
			else
			{
				MIDIGClip* opClip = dynamic_cast<MIDIGClip*>(EMGClipRepository::Instance() -> GetClip(p_vX, p_vY));
				EMMediaMIDIEvent* opEvent = NULL;
				if(opClip != NULL)// && m_eDisplayMode == CE_DISPLAY_MODE_DEFAULT) // Only look for MIDI note on events if rubberbands aren't active
					opEvent = opClip -> GetMIDIEvent(p_vX, p_vY);

				if(opEvent == NULL)
					m_opView -> SetMouseCursor(EM_MOUSE_CURSOR_RESIZE_CLIP);
				else
				{
					int32 vMiddleOfEvent = (*(opEvent -> m_vpStart) + ((*(opEvent -> m_opNoteOff -> m_vpStart) - *(opEvent -> m_vpStart) + 1) / 2)) / m_vTimeZoomScale;
					if(p_vX < vMiddleOfEvent)
						m_opView -> SetMouseCursor(EM_MOUSE_CURSOR_RESIZE_CLIP_LEFT);
					else
						m_opView -> SetMouseCursor(EM_MOUSE_CURSOR_RESIZE_CLIP_RIGHT);
				}
			}
		}

		else if(eToolboxMode == TOOLBOX_ERASE)
			m_opView -> SetMouseCursor(EM_MOUSE_CURSOR_DELETE);

		else if(eToolboxMode == TOOLBOX_DRAW)
			m_opView -> SetMouseCursor(EM_MOUSE_CURSOR_DRAW);

		else if(eToolboxMode == TOOLBOX_CUT_PASTE)
			m_opView -> SetMouseCursor(EM_MOUSE_CURSOR_CUT_PASTE);
		/*
		if(m_eDisplayMode == CE_DISPLAY_MODE_MIDI_VELOCITY)
		{
			if(m_opMouseDownClip != NULL)
				m_opMouseDownClip -> MouseMoved(p_opView, p_vX, p_vY, p_vButtonOne, p_vButtonTwo, p_vButtonThree);
		}
		else *///if(m_eDisplayMode != CE_DISPLAY_MODE_DEFAULT)
		//	m_opRubberBand -> MouseMoved(p_vX, p_vY, p_vButtonOne, p_vButtonTwo, p_vButtonThree, eToolboxMode);

		return;
	}

	if(m_eDisplayMode != CE_DISPLAY_MODE_DEFAULT && !m_oSelectionRect.IsValid())
	{
		m_opRubberBand -> MouseMoved(p_vX, p_vY, p_vButtonOne, p_vButtonTwo, p_vButtonThree, eToolboxMode);
		return;
	}

	if(eToolboxMode == TOOLBOX_SELECT || m_vMoveDrawedEvent)
	{
		if(p_vY > Frame().m_vBottom - GetLowerBorderSize())
			p_vY = Frame().m_vBottom - GetLowerBorderSize();
		else if(p_vY < Frame().m_vTop)
			p_vY = Frame().m_vTop;
		int64 vMouseXPos = p_vX * m_vTimeZoomScale;

		if(m_oSelectionRect.IsValid())
		{
			if(m_vMouseDownXPos / m_vTimeZoomScale < p_vX)
			{
				if(m_vMouseDownY <= p_vY)
					m_oSelectionRect = EMRect(m_vMouseDownXPos / m_vTimeZoomScale, m_vMouseDownY, p_vX, p_vY);
				else
					m_oSelectionRect = EMRect(m_vMouseDownXPos / m_vTimeZoomScale, p_vY, p_vX, m_vMouseDownY);
				m_vSelectionLeftXPos = m_vMouseDownXPos;
				m_vSelectionRightXPos = vMouseXPos;
			}
			else
			{
				if(m_vMouseDownY <= p_vY)
					m_oSelectionRect = EMRect(p_vX, m_vMouseDownY, m_vMouseDownXPos / m_vTimeZoomScale, p_vY);
				else
					m_oSelectionRect = EMRect(p_vX, p_vY, m_vMouseDownXPos / m_vTimeZoomScale, m_vMouseDownY);
				m_vSelectionLeftXPos = vMouseXPos;
				m_vSelectionRightXPos = m_vMouseDownXPos;
			}

			//m_oSelectionRect = m_oSelectionRect & (m_opView -> Bounds());
			if(m_oSelectionRect.m_vLeft < 0)
				m_oSelectionRect.m_vLeft = 0;
			if(m_oSelectionRect.m_vTop < 0)
				m_oSelectionRect.m_vTop = 0;
//eo << "m_oSelectionRect: ";
//m_oSelectionRect.Print();
			DrawSelection(EMRect(), p_opView);

			return;
		}
		
		if(m_opMouseDownEvent == NULL)
			return;

		if(p_vButtonOne)
		{
			m_oOldMovedRect = m_oMovedRect;
			m_oMovedRect = m_oSelectedMIDIEventFrame;
			m_oMovedRect.OffsetBy(p_vX - (m_vMouseDownXPos / m_vTimeZoomScale), p_vY - m_vMouseDownY);

			// Snap the y coordinate to nearest key
			float vKey = static_cast<float>(Frame().m_vBottom - LOWER_BORDER_SIZE - m_oMovedRect.m_vTop) / (MIDI_KEY_HEIGHT - 1);
			// Round to nearest key
			if(vKey - static_cast<int>(vKey) < 0.5)
				vKey = static_cast<int>(vKey);
			else
				vKey = static_cast<int>(vKey) + 1;
//			int32 vOffsetY = (LOWER_BORDER_SIZE + (vKey * (MIDI_KEY_HEIGHT - 1)) - 1) - (Frame().m_vBottom - LOWER_BORDER_SIZE - m_oMovedRect.m_vTop);
			int32 vOffsetY = (Frame().m_vBottom - LOWER_BORDER_SIZE + 1 - (vKey * (MIDI_KEY_HEIGHT - 1))) - m_oMovedRect.m_vTop;

			// Snap the x coordinate to an eventual grid
			m_vMovedPosition = m_oMovedRect.m_vLeft * m_vTimeZoomScale;
			if(m_vMovedPosition < 0)
				m_vMovedPosition = 0;
			EMGUIUtilities::Instance() -> GetSnapPosition(&m_vMovedPosition, false, false, NULL, true, true);
			int32 vOffsetX = (m_vMovedPosition / m_vTimeZoomScale) - m_oMovedRect.m_vLeft;
//eo << "Stuff: " << Frame().m_vBottom << ", " << m_oMovedRect.m_vTop << ", " << vKey << ef;
//eo << "m_oMovedRect before:";
//m_oMovedRect.Print();
			m_oMovedRect.OffsetBy(vOffsetX, vOffsetY);
//eo << "m_oMovedRect after:";
//m_oMovedRect.Print();
			m_opView -> Invalidate(m_oMovedRect | m_oOldMovedRect);
		}
	}
	
	else if(eToolboxMode == TOOLBOX_RESIZE)
	{
		if(m_opMouseDownClip == NULL || m_opMouseDownEvent == NULL)
		{
			m_opView -> SetMouseCursor(EM_MOUSE_CURSOR_RESIZE_CLIP);
			return;
		}

		//int32 vSnapThreshold = *(static_cast<int32*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_SNAP_THRESHOLD)));
		//int64 vPositionPointerPos = *(static_cast<int64*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_CURRENT_PROJECT_POSITION, NULL)));

		if(m_vResizeLeft)
		{		
			m_opView -> SetMouseCursor(EM_MOUSE_CURSOR_RESIZE_CLIP_LEFT);

			int64 vMovedPosition = m_vOriginalPosition + (p_vX * m_vTimeZoomScale) - m_vMouseDownXPos;
eo << "Event - vMovedPosition: " << vMovedPosition << ", m_vOriginalLength: " << m_vOriginalLength << ", m_vOriginalPosition: " << m_vOriginalPosition << ef;
			/*
			if((abs(vMovedPosition - m_vOriginalPosition)) / m_vTimeZoomScale < vSnapThreshold)
				vMovedPosition = m_vOriginalPosition;
			else if(abs((vMovedPosition / m_vTimeZoomScale) - (vPositionPointerPos / m_vTimeZoomScale)) < vSnapThreshold)
				vMovedPosition = vPositionPointerPos;
			else
			*/
			EMGUIUtilities::Instance() -> GetSnapPosition(&vMovedPosition, false, false, NULL, true, true, m_vOriginalPosition, true);

			if(vMovedPosition > m_vOriginalPosition + m_vOriginalLength - 1 - 1000)
				vMovedPosition = m_vOriginalPosition + m_vOriginalLength - 1 - 1000;
			else if(vMovedPosition < 0)
				vMovedPosition = 0;
			int64 vNewLength = m_vOriginalPosition + m_vOriginalLength - vMovedPosition;
eo << "new position: " << vMovedPosition << ", new length: " << vNewLength << ef;
			EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_RESIZE_MIDI_EVENT, m_opMouseDownEvent, &vMovedPosition, &vNewLength);			
		}
		else
		{		
			m_opView -> SetMouseCursor(EM_MOUSE_CURSOR_RESIZE_CLIP_RIGHT);

			int64 vMovedLength = m_vOriginalLength + (p_vX * m_vTimeZoomScale) - m_vMouseDownXPos;
eo << "Event - vMovedLength: " << vMovedLength << ", m_vOriginalLength: " << m_vOriginalLength << ", m_vOriginalPosition: " << m_vOriginalPosition << ef;
			/*
			if((abs(vMovedLength - m_vOriginalLength)) / m_vTimeZoomScale < vSnapThreshold)
				vMovedLength = m_vOriginalLength;
			else if(abs(m_vOriginalPosition + vMovedLength - vPositionPointerPos) / m_vTimeZoomScale < vSnapThreshold)
				vMovedLength = vPositionPointerPos - m_vOriginalPosition + 1;
			else
			{
			*/
			int64 vEndPos = m_vOriginalPosition + vMovedLength - 1;
			EMGUIUtilities::Instance() -> GetSnapPosition(&vEndPos, true, true, m_opMouseDownClip, true, true, m_vOriginalPosition + m_vOriginalLength - 1, true);
			vMovedLength = vEndPos - m_vOriginalPosition + 1;
			//}

			if(vMovedLength < 1000)
				vMovedLength = 1000; // FIXME: should perhaps have other minimum length
//eo << "new position: " << m_vOriginalPosition << ", new length: " << vNewLength << ef;
			EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_RESIZE_MIDI_EVENT, m_opMouseDownEvent, &m_vOriginalPosition, &vMovedLength);
		}

		if(m_opMouseDownEvent -> m_vIsSelected)
			m_oSelectedMIDIEventFrame = CalculateEventFrame(&m_oSelectedMIDIEvents);

		m_opMouseDownClip -> UpdateFrame();

		// Quick 'n dirty redraw
		m_opView -> Invalidate(Frame() & m_opView -> Bounds());

		m_vHasResized = true;
	}

	else if(eToolboxMode == TOOLBOX_ERASE)
	{
		m_opView -> SetMouseCursor(EM_MOUSE_CURSOR_DELETE);

		//if(m_opMouseDownClip == NULL)
		//	return;
		EMGClip* opClip = GetClip(p_vX, p_vY);
		if(opClip == NULL)
			return;

		EMMediaMIDIEvent* opMouseEvent = static_cast<MIDIGClip*>(opClip) -> GetMIDIEvent(p_vX, p_vY);
		if(opMouseEvent == NULL)
			return;

		list<EMMediaMIDIEvent*> oEventsToDelete;
		oEventsToDelete.push_back(opMouseEvent);
		oEventsToDelete.push_back(opMouseEvent -> m_opNoteOff);
eo << "Deleting events: " << (uint32) opMouseEvent << ", " << (uint32) opMouseEvent -> m_opNoteOff << ef;
		int32 vMediaTrackID = GetTrack() -> GetTrackInfo() -> GetMediaTrackID();
		EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_DELETE_MIDI_EVENTS, &oEventsToDelete, &vMediaTrackID);
	}	

	else if(eToolboxMode == TOOLBOX_CUT_PASTE)
		m_opView -> SetMouseCursor(EM_MOUSE_CURSOR_CUT_PASTE);
}

void MIDIGClipContainer::MouseUp(EMView* p_opView, int32 p_vX, int32 p_vY, EMEditMode p_eEditMode)
{
	if(!m_vMouseIsDown)
		return;

	m_vMouseIsDown = false;

	if(p_eEditMode != EM_CLIP_EDIT_MODE)
		return;

	ToolboxMode eToolboxMode = static_cast<ToolboxMode>(*(static_cast<int*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_TOOLBOX_MODE))));

	if(!m_oSelectionRect.IsValid())
	{
		/*
		if(m_eDisplayMode == CE_DISPLAY_MODE_MIDI_VELOCITY)
		{
			if(m_opMouseDownClip != NULL)
				m_opMouseDownClip -> MouseUp(p_opView, p_vX, p_vY, false, false, false);
		
			return;
		}
		*/
		if(m_eDisplayMode != CE_DISPLAY_MODE_DEFAULT)
		{
			m_opRubberBand -> MouseUp(p_vX, p_vY, eToolboxMode);	
			return;
		}
	}

	//if(!m_vMouseIsDown)
	//	return;
	
	if(p_vX < 0)
		p_vX = 0;
	if(p_vY < 0)
		p_vY = 0;

	//m_vMouseIsDown = false;

	uint32 vModifiers = EMKeyboard::Instance() -> GetModifiers();
	
	if(eToolboxMode == TOOLBOX_SELECT || m_vMoveDrawedEvent)
	{
		if(m_oSelectionRect.IsValid())
		{
			if(m_eDisplayMode != CE_DISPLAY_MODE_DEFAULT)
			{
				/*
				if(m_eDisplayMode == CE_DISPLAY_MODE_MIDI_VELOCITY)
				{
					list<EMGClip*>::iterator oClipIterator = m_oClipRepository.begin();
					while(oClipIterator != m_oClipRepository.end())
					{
						EMGRubberBand* opRubberBand = static_cast<MIDIGClip*>(*oClipIterator) -> GetVelocityRubberBand();
						if((vModifiers & EM_SHIFT_KEY) == EM_SHIFT_KEY)
							opRubberBand -> SetSelected(opRubberBand -> GetNodes(m_oSelectionRect), false);
						else
							opRubberBand -> SetSelected(opRubberBand -> GetNodes(m_oSelectionRect), true);

						oClipIterator++;
					}
				}
				else// if(m_eDisplayMode != CE_DISPLAY_MODE_DEFAULT)*/
				{
					if((vModifiers & EM_SHIFT_KEY) == EM_SHIFT_KEY)
						m_opRubberBand -> SetSelected(m_opRubberBand -> GetNodes(m_oSelectionRect), false);	
					else
						m_opRubberBand -> SetSelected(m_opRubberBand -> GetNodes(m_oSelectionRect), true);
				}

				m_oSelectedMIDIEventFrame = CalculateEventFrame(&m_oSelectedMIDIEvents);

				m_oSelectionRect = EMRect();
				m_opView -> Invalidate(Frame() & m_opView -> Bounds());
				DrawSelection(EMRect(), p_opView);

				return;
			}

			EMRect oOldSelectedMIDIEventFrame = m_oSelectedMIDIEventFrame;

			list<EMGClip*>::iterator oClipIterator = m_oClipRepository.begin();
			while(oClipIterator != m_oClipRepository.end())
			{
				if(m_oSelectionRect.m_vLeft < m_vMouseDownXPos / m_vTimeZoomScale)
					static_cast<MIDIGClip*>(*oClipIterator) -> GetMIDIEvents(m_oSelectionRect, true, &m_oSelectedMIDIEvents);
				else
					static_cast<MIDIGClip*>(*oClipIterator) -> GetMIDIEvents(m_oSelectionRect, false, &m_oSelectedMIDIEvents);
				oClipIterator++;
			}

			list<EMMediaMIDIEvent*>::iterator oEventIterator = m_oSelectedMIDIEvents.begin();
			while(oEventIterator != m_oSelectedMIDIEvents.end())
			{
eo << "Selected MIDI event - base type: " << (*oEventIterator) -> m_vBaseType << ", position: " << (double) *((*oEventIterator) -> m_vpStart) << ", note off pointer: " << (int32) (*oEventIterator) -> m_opNoteOff << ", data1: " << (*oEventIterator) -> m_vData1 << ", data2: " << (*oEventIterator) -> m_vData2 << ", address: " << (int32) &(*oEventIterator) << ef;
				(*oEventIterator) -> m_vIsSelected = true;
				oEventIterator++;
			}

			m_oSelectedMIDIEventFrame = CalculateEventFrame(&m_oSelectedMIDIEvents);

			m_oSelectionRect = EMRect();
			m_opView -> Invalidate(oOldSelectedMIDIEventFrame | m_oSelectedMIDIEventFrame);
			DrawSelection(EMRect(), p_opView);
	
			char vMessage[80];
			sprintf(vMessage, "%ld events selected", m_oSelectedMIDIEvents.size());
			TrackWindowStatusBar::Instance() -> WriteMessage(string(vMessage));
		}

		else if(m_oMovedRect.IsValid())
		{
			if(m_oMovedRect != m_oSelectedMIDIEventFrame)
			{
				int64 vMoveOffset = m_vMovedPosition - *(m_oSelectedMIDIEvents.front() -> m_vpStart);
				int32 vKeyOffset = (m_oSelectedMIDIEventFrame.m_vBottom - m_oMovedRect.m_vBottom) / (MIDI_KEY_HEIGHT - 1);

				list<EMMediaMIDIEvent*> oMIDIEventsToMove;
				list<EMMediaMIDIEvent*>::iterator oEventIterator = m_oSelectedMIDIEvents.begin();
				while(oEventIterator != m_oSelectedMIDIEvents.end())
				{
					oMIDIEventsToMove.push_back(*oEventIterator);
					oMIDIEventsToMove.push_back((*oEventIterator) -> m_opNoteOff);
					oEventIterator++;
				}

				if((vModifiers & EM_CONTROL_KEY) == EM_CONTROL_KEY)
				{
					int64 vpParameters[4];
					vpParameters[0] = GetTrack() -> GetTrackInfo() -> GetMediaTrackID();
					vpParameters[1] = -1; // Let the media engine select a clip
					vpParameters[2] = vKeyOffset;
					vpParameters[3] = vMoveOffset;
					list<EMMediaMIDIEvent*>* opClonedEvents = static_cast<list<EMMediaMIDIEvent*>*>(EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_CLONE_MIDI_EVENTS, vpParameters, this, &oMIDIEventsToMove));
					DeselectAll(this);
					list<EMMediaMIDIEvent*>::iterator oIterator = opClonedEvents -> begin();
					while(oIterator != opClonedEvents -> end())
					{
						if((*oIterator) -> GetBaseEventType() == EM_MIDI_EVENT_TYPE_NOTE_ON)
						{
							(*oIterator) -> m_vIsSelected = true;
							m_oSelectedMIDIEvents.push_back(*oIterator);
						}
						oIterator++;
					}
				}
				else
					EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_MOVE_MIDI_EVENTS, &vKeyOffset, &vMoveOffset, &oMIDIEventsToMove);

				//UpdateEventsInClips();
				m_oSelectedMIDIEventFrame = CalculateEventFrame(&m_oSelectedMIDIEvents);

p_opView -> Invalidate(Frame() & p_opView -> Bounds()); // Needed?
			}
			m_oMovedRect = EMRect();
		}

		m_vMoveDrawedEvent = false;
		m_opMouseDownEvent = NULL;
	}
	
	else if(eToolboxMode == TOOLBOX_RESIZE)
	{
		if(m_vHasResized)
		{
			int64 vpOriginalValues[2];
			vpOriginalValues[0] = m_vOriginalPosition;
			vpOriginalValues[1] = m_vOriginalLength;
			EMCommandRepository::Instance() -> ExecuteCommand(UNDO_COMMAND_RESIZE_MIDI_EVENT, this, m_opMouseDownEvent, vpOriginalValues);
		}

		m_opMouseDownEvent = NULL;
	}

/*
	else if(eToolboxMode == TOOLBOX_DRAW)
	{
		m_vMoveDrawedEvent = false;
		m_opMouseDownEvent = NULL;
	}
*/
/*	
	if(p_eEditMode == EM_CLIP_EDIT_MODE)
	{
		if(m_eDisplayMode == CE_DISPLAY_MODE_MIDI_VELOCITY)
		{
			if(m_opMouseDownClip == NULL)
				return;

			m_opMouseDownClip -> MouseUp(p_opView, p_vX, p_vY, false, false, false);
		}
		if(m_eDisplayMode != CE_DISPLAY_MODE_DEFAULT)
			m_opRubberBand -> MouseUp(p_vX, p_vY);	
		else if(m_opClipContainerWithSelection == this)
		{
			if(m_oSelectionRect.IsValid())
			{
				m_oSelectedMIDIEvents.clear();
				list<EMGClip*>::iterator oIterator = m_oClipRepository.begin();
				while(oIterator != m_oClipRepository.end())
				{
					static_cast<MIDIGClip*>(*oIterator) -> GetMIDIEvents(m_oSelectionRect, true, &m_oSelectedMIDIEvents);
//					m_oSelectedMIDIEvents.merge(*(static_cast<MIDIGClip*>(*oIterator) -> SetSelected(m_oSelectionRect, true)));
					oIterator++;
				}
				m_oSelectedMIDIEventFrame = CalculateEventFrame(&m_oSelectedMIDIEvents);
				m_oSelectionRect = EMRect();
			}
			//DrawSelection(EMRect(), m_opView);
			m_opView -> Invalidate(Frame() & (m_opView -> Bounds()));
		}
	}
*/
}

void MIDIGClipContainer::RemoveClip(EMGClip* p_opClip)
{
	EMGClipContainer::RemoveClip(p_opClip);

	if(m_opClipContainerWithSelection == this)
		DeselectAll(NULL);
}

void MIDIGClipContainer::Scroll(int p_vLowestKey)
{
	m_oSelectedMIDIEventFrame = CalculateEventFrame(&m_oSelectedMIDIEvents);
	if(m_opView != NULL)
		m_opView -> Invalidate(Frame() & (m_opView -> Bounds()));
}

bool MIDIGClipContainer::SetFrame(EMRect p_oFrame)
{
	if(EMGClipContainer::SetFrame(p_oFrame))
	{
		if(m_opRubberBand != NULL)
			m_opRubberBand -> SetFrame(EMRect(p_oFrame.m_vLeft, p_oFrame.m_vTop, p_oFrame.m_vRight, p_oFrame.m_vBottom - LOWER_BORDER_SIZE));

		m_oSelectedMIDIEventFrame = CalculateEventFrame(&m_oSelectedMIDIEvents);

		return true;
	}

	return false;
}

void MIDIGClipContainer::SetTrack(EMGTrack* p_opTrack)
{
	EMGClipContainer::SetTrack(p_opTrack);
	m_opSelectionButton = dynamic_cast<GTrackInfo*>(p_opTrack -> GetTrackInfo()) -> GetSelectionButton();
	m_opSelectionButton -> AddListener(this);
	Scroll(m_opSelectionButton -> GetLowestKey());
}

void MIDIGClipContainer::UpdateEventsInClips()
{
	list<EMGClip*>::iterator oIterator = m_oClipRepository.begin();
	while(oIterator != m_oClipRepository.end())
	{
		static_cast<MIDIGClip*>(*oIterator) -> UpdateContent();
static_cast<MIDIGClip*>(*oIterator) -> UpdateFrame();
		oIterator++;
	}
}

void MIDIGClipContainer::UpdateSelection()
{
	m_oSelectedMIDIEventFrame = CalculateEventFrame(&m_oSelectedMIDIEvents);
}

void MIDIGClipContainer::UpdateTimeScale(int64 p_vTimeScale)
{
	EMGClipContainer::UpdateTimeScale(p_vTimeScale);
	if(m_opRubberBand != NULL)
		m_opRubberBand -> SetTimeZoomScale(p_vTimeScale);
	m_vTimeZoomScale = p_vTimeScale;

	m_oSelectedMIDIEventFrame = CalculateEventFrame(&m_oSelectedMIDIEvents);
}




