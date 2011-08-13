#include "MIDIGClip.h"

#include "CommandIDs.h"
#include "EMCommandRepository.h"
#include "EMGClipRepository.h"
#include "EMGTrack.h"
#include "EMGTrackRepository.h"
#include "EMGUIGlobals.h"
#include "EMGUIUtilities.h"
#include "EMMediaGlobals.h"
#include "EMMediaMIDIEvent.h"
#include "EMSettingIDs.h"
#include "EMSettingsRepository.h"
#include "EMView.h"
#include "GUIGlobals.h"
#include "MIDIGClipContainer.h"
#include "SettingIDs.h"

#include <vector>

// FIXME: Fix the text placement calculations
const int32 NAME_X_OFFSET = 3;
const int32 NAME_Y_OFFSET = -2;

MIDIGClip::MIDIGClip(string p_oClipName, int64 p_vPosition, int64 p_vLength, EMGTrack* p_opTrack, int32 p_vMediaClipID, EMView* p_opView) :
GClip(p_oClipName, p_vPosition, p_vLength, p_opTrack, p_vMediaClipID, p_opView),
//m_oVelocityRubberBand(p_opView, 127, 1, 127, EM_RUBBERBAND_DRAW_BARS),
m_vIsInitialized(false),
m_vNamePositionUpdated(false)
{
	//m_oVelocityRubberBand.SetPermissions(false, false, true);
	//m_oVelocityRubberBand.AddListener(this);

	if(p_vMediaClipID > 0)
		UpdateContent();
}

MIDIGClip::~MIDIGClip()
{
}
/*
EMRect MIDIGClip::CalculateFrame(uint32 p_vTrackOffset, int64 p_vClipOffsetX)
{
	p_vClipOffsetX = EMGUIUtilities::Instance() -> TruncateSample(p_vClipOffsetX);
	return EMGClip::CalculateFrame(p_vTrackOffset, p_vClipOffsetX);
}
*/
void MIDIGClip::CalculateNamePosition(EMView* p_opView)
{
	/*
	int32 vAscent;
	int32 vDescent;
	p_opView -> SetFont(EM_PLAIN);
	p_opView -> CalculateStringHeight(m_oName.c_str(), vAscent, vDescent);
	m_vStringTop = vAscent + NAME_Y_OFFSET;
	m_vStringLeft = NAME_X_OFFSET;
	m_vNamePositionUpdated = true;
	*/

	int32 vAscent;
	int32 vDescent;
	if(p_opView -> LockWindow())
	{
		p_opView -> SetFont(EM_PLAIN);
		p_opView -> CalculateStringHeight(m_oName.c_str(), vAscent, vDescent);
		m_vStringTop = vAscent + NAME_Y_OFFSET;
		m_vStringLeft = NAME_X_OFFSET;
		int vLength = p_opView -> CalculateStringWidth(m_oName.c_str());
		m_oLabelBackground = EMRect(2, 2, 2 + vLength, 12);
		m_vNamePositionUpdated = true;

		p_opView -> UnlockWindow();
	}
}

EMRect MIDIGClip::CalculateFrame(uint32 p_vTrackOffset, int64 p_vClipOffsetX)
{
	p_vClipOffsetX = EMGUIUtilities::Instance() -> TruncateSample(p_vClipOffsetX);
	return EMGClip::CalculateFrame(p_vTrackOffset, p_vClipOffsetX);
}

EMGClip* MIDIGClip::CloneClip(int32 p_vTrackOffset, int64 p_vPositionOffset)
{
	EMGTrackRepository* opTrackRepository = EMGTrackRepository::Instance();
	EMGClipRepository* opClipRepository = EMGClipRepository::Instance();
	EMGTrack* opNewTrack = opTrackRepository -> GetTrack(opTrackRepository -> GetTrackIndex(GetTrack()) + p_vTrackOffset);
	//p_vPositionOffset = EMGUIUtilities::Instance() -> TruncateSample(p_vPositionOffset);
	int64 vNewPosition = GetPosition() + p_vPositionOffset;
	vNewPosition = EMGUIUtilities::Instance() -> TruncateSample(vNewPosition);


	int32 vTargetMediaTrackID = opNewTrack -> GetTrackInfo() -> GetMediaTrackID();
	int32* vpMediaClipID = static_cast<int32*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_CLONE_CLIP, &m_vMediaClipID, &vNewPosition, &vTargetMediaTrackID));
	int32 vMediaClipID = *vpMediaClipID;
	EMGClip* opNewClip = opClipRepository -> CreateClip(GetLabel(), GetType(), vNewPosition, GetLength()/*, sNewClipInfo*/, opNewTrack, vMediaClipID, m_opView);

	return opNewClip;
}

EMGClip* MIDIGClip::CreateInstance(string p_oClipName, int64 p_vPosition, int64 p_vLength, EMGTrack* p_opTrack, int32 p_vMediaClipID, EMView* p_opView)
{
	return EM_new MIDIGClip(EMGUIUtilities::Instance() -> GetUniqueClipName(p_oClipName.c_str(), NULL), p_vPosition, p_vLength, p_opTrack, p_vMediaClipID, p_opView);
}

void MIDIGClip::DeselectAll()
{
	//m_oVelocityRubberBand.DeselectAllNodes();
}

void MIDIGClip::DrawClip(EMRect p_oClippingRect, EMView* p_opView, bool p_vClipEditMode, int64 p_vZoomScale, bool p_vDrawContent)
{
	if(!p_oClippingRect.Intersects(m_oFrame))
		return;

//eo << "Drawing MIDI clip with ID: " << GetMediaClipID() << ", and nr events: " << m_opMidiEvents -> size() << ", pointer to events: " << (uint32) m_opMidiEvents << ef;

	if(!m_vNamePositionUpdated)
		CalculateNamePosition(p_opView);

	p_opView -> ConstrainClippingRect(m_oFrame & p_oClippingRect);

	EMRect oDrawRect(m_oFrame & p_oClippingRect);

	if(p_vClipEditMode)
	{
		p_opView -> SetHighColor(EMColor(0, 0, 0, 255));
		p_opView -> StrokeLine(oDrawRect.m_vLeft, m_oFrame.m_vTop, oDrawRect.m_vRight, m_oFrame.m_vTop);
		p_opView -> StrokeLine(oDrawRect.m_vLeft, m_oFrame.m_vBottom, oDrawRect.m_vRight, m_oFrame.m_vBottom);
		if(m_oFrame.m_vLeft >= oDrawRect.m_vLeft)
			p_opView -> StrokeLine(m_oFrame.m_vLeft, m_oFrame.m_vTop, m_oFrame.m_vLeft, m_oFrame.m_vBottom);
		if(m_oFrame.m_vRight <= oDrawRect.m_vRight)
			p_opView -> StrokeLine(m_oFrame.m_vRight, m_oFrame.m_vTop, m_oFrame.m_vRight, m_oFrame.m_vBottom);

		p_opView -> SetHighColor(EM_COLOR_TEXT);
		p_opView -> SetLowColor(EMColor(200, 200, 200, 255));
		p_opView -> DrawString(m_oFrame.m_vLeft + m_vStringLeft, m_oFrame.m_vTop + m_vStringTop, m_oName.c_str());

		// Let's draw the events
		int vLowestKey = dynamic_cast<MIDIGClipContainer*>(m_opTrack -> GetClipContainer()) -> GetLowestKey();
		ClipEditDisplayMode eDisplayMode = dynamic_cast<MIDIGClipContainer*>(m_opTrack -> GetClipContainer()) -> GetDisplayMode();
		uint8 vKey;
		int64 vLeftPosition;
		int64 vRightPosition;
		EMRect oEventFrame;
		list<EMMediaMIDIEvent*>::iterator oIterator = m_opMidiEvents -> begin();
//eo << "Clip frame: ";
//m_oFrame.Print();

		// Let's draw the MIDI events
		while(oIterator != m_opMidiEvents -> end())
		{
//eo << "MIDI event - base type: " << (*oIterator) -> m_vBaseType << ", position: " << (double) *((*oIterator) -> m_vpStart) << ", note off pointer: " << (int32) (*oIterator) -> m_opNoteOff << ", data1: " << (*oIterator) -> m_vData1 << ", data2: " << (*oIterator) -> m_vData2 << ", address: " << (int32) &(*oIterator) << ef;
			if((*oIterator) -> m_vBaseType == EM_MIDI_EVENT_TYPE_NOTE_ON) // FIXME: Use a constant for note ons and other events
			{
				vLeftPosition = *((*oIterator) -> m_vpStart);
				oEventFrame.m_vLeft = vLeftPosition / p_vZoomScale;
				vRightPosition = *((*oIterator) -> m_opNoteOff -> m_vpStart);
				oEventFrame.m_vRight = vRightPosition / p_vZoomScale;

				if(oEventFrame.m_vLeft <= p_oClippingRect.m_vRight && oEventFrame.m_vRight >= p_oClippingRect.m_vLeft)
				{
					vKey = (*oIterator) -> m_vData1;
					oEventFrame.m_vTop = m_oFrame.m_vBottom - ((MIDI_KEY_HEIGHT - 1) * (vKey - vLowestKey + 1)) + 1;
					oEventFrame.m_vBottom = m_oFrame.m_vBottom - ((MIDI_KEY_HEIGHT - 1) * (vKey - vLowestKey)) - 1;

					if(p_oClippingRect.Intersects(oEventFrame))
					{
						if(eDisplayMode == CE_DISPLAY_MODE_DEFAULT)
						{
							if((*oIterator) -> m_vIsSelected)
								p_opView -> SetHighColor(EM_COLOR_MIDI_VIEW_SHADOW);
							else
								p_opView -> SetHighColor(EM_COLOR_MIDI_VIEW_BACKGROUND);
							//if(oDrawRect.Contains(oEventFrame)) // If the whole event is visible, we can do this a bit quicker
							{
								p_opView -> FillRect(oEventFrame);
								p_opView -> SetHighColor(EMColor(0, 0, 0, 255));
								p_opView -> StrokeRect(oEventFrame);
							}
							/*
							else // Now we must concider that coordinates may wrap, update: maybe the framework resolves this wrapping issue now
							{
								EMRect oVisibleEventRect(oEventFrame & p_oClippingRect);

								p_opView -> FillRect(oVisibleEventRect);
								p_opView -> SetHighColor(EMColor(0, 0, 0, 255));

								if(oEventFrame.m_vLeft < p_oClippingRect.m_vLeft && oEventFrame.m_vRight > p_oClippingRect.m_vRight)
								{
									p_opView -> StrokeLine(p_oClippingRect.m_vLeft, oEventFrame.m_vTop, p_oClippingRect.m_vRight, oEventFrame.m_vTop);
									p_opView -> StrokeLine(p_oClippingRect.m_vLeft, oEventFrame.m_vBottom, p_oClippingRect.m_vRight, oEventFrame.m_vBottom);
								}
								else if(oEventFrame.m_vLeft < p_oClippingRect.m_vLeft)
								{
									p_opView -> StrokeLine(p_oClippingRect.m_vLeft, oEventFrame.m_vTop, oEventFrame.m_vRight, oEventFrame.m_vTop);
									p_opView -> StrokeLine(p_oClippingRect.m_vLeft, oEventFrame.m_vBottom, oEventFrame.m_vRight, oEventFrame.m_vBottom);
								}
								else
								{
									p_opView -> StrokeLine(oEventFrame.m_vLeft, oEventFrame.m_vTop, p_oClippingRect.m_vRight, oEventFrame.m_vTop);
									p_opView -> StrokeLine(oEventFrame.m_vLeft, oEventFrame.m_vBottom, p_oClippingRect.m_vRight, oEventFrame.m_vBottom);
								}

								if(oEventFrame.m_vLeft >= p_oClippingRect.m_vLeft)
									p_opView -> StrokeLine(oEventFrame.m_vLeft, oEventFrame.m_vTop, oEventFrame.m_vLeft, oEventFrame.m_vBottom);
								if(oEventFrame.m_vRight <= p_oClippingRect.m_vRight)
									p_opView -> StrokeLine(oEventFrame.m_vRight, oEventFrame.m_vTop, oEventFrame.m_vRight, oEventFrame.m_vBottom);
							}
							*/
						}
						else
						{
							p_opView -> SetHighColor(EM_COLOR_VIEW_BACKGROUND_HIGHLIGHT);
							p_opView -> FillRect(oEventFrame & p_oClippingRect);
						}
					}
				}
			}
			oIterator++;
		}
		/*
		if(eDisplayMode == CE_DISPLAY_MODE_MIDI_VELOCITY)
		{
			m_oVelocityRubberBand.SetTimeZoomScale(p_vZoomScale);
			p_opView -> ConstrainClippingRect(EMRect());
			m_oVelocityRubberBand.Draw(p_oClippingRect);
			p_opView -> ConstrainClippingRect(m_oFrame);
		}
		*/
	}

	else
	{
		if(m_vSelected)
		{
			p_opView -> SetHighColor(EM_COLOR_SELECTED_MIDI_VIEW_BACKGROUND);
			p_opView -> FillRect(m_oFrame & p_oClippingRect);

			p_opView -> SetHighColor(EM_COLOR_SELECTED_MIDI_VIEW_SHADOW);
			p_opView -> StrokeLine(oDrawRect.m_vLeft, m_oFrame.m_vBottom, oDrawRect.m_vRight, m_oFrame.m_vBottom);
			p_opView -> StrokeLine(oDrawRect.m_vLeft, m_oFrame.m_vBottom - 1, oDrawRect.m_vRight, m_oFrame.m_vBottom - 1);

			p_opView -> SetHighColor(EM_COLOR_SELECTED_MIDI_VIEW_HIGHLIGHT);
			p_opView -> StrokeLine(oDrawRect.m_vLeft, m_oFrame.m_vTop, oDrawRect.m_vRight, m_oFrame.m_vTop);
			p_opView -> StrokeLine(oDrawRect.m_vLeft, m_oFrame.m_vTop + 1, oDrawRect.m_vRight, m_oFrame.m_vTop + 1);

			if(m_oFrame.m_vLeft >= oDrawRect.m_vLeft)
				p_opView -> StrokeLine(m_oFrame.m_vLeft, m_oFrame.m_vTop, m_oFrame.m_vLeft, m_oFrame.m_vBottom);
			if(m_oFrame.m_vLeft + 1 >= oDrawRect.m_vLeft)
				p_opView -> StrokeLine(m_oFrame.m_vLeft + 1, m_oFrame.m_vTop, m_oFrame.m_vLeft + 1, m_oFrame.m_vBottom - 1);

			p_opView -> SetHighColor(EM_COLOR_SELECTED_MIDI_VIEW_SHADOW);
			if(m_oFrame.m_vRight <= oDrawRect.m_vRight)
				p_opView -> StrokeLine(m_oFrame.m_vRight, m_oFrame.m_vTop, m_oFrame.m_vRight, m_oFrame.m_vBottom);
			if(m_oFrame.m_vRight - 1 <= oDrawRect.m_vRight)
				p_opView -> StrokeLine(m_oFrame.m_vRight - 1, m_oFrame.m_vTop + 1, m_oFrame.m_vRight - 1, m_oFrame.m_vBottom);

			EMRect oLabelBackground(m_oLabelBackground);
			oLabelBackground.OffsetBy(m_oFrame.m_vLeft, m_oFrame.m_vTop);
			if(oLabelBackground.Intersects(p_oClippingRect))
			{
				m_opView -> SetHighColor(EM_COLOR_SELECTED_MIDI_VIEW_SHADOW);
				p_opView -> FillRect(oLabelBackground);
				p_opView -> SetHighColor(EMColor(255, 255, 255, 255));
				//p_opView -> SetLowColor(EM_COLOR_SELECTED_MIDI_VIEW_BACKGROUND);
				p_opView -> DrawString(m_oFrame.m_vLeft + m_vStringLeft, m_oFrame.m_vTop + m_vStringTop, m_oName.c_str());
			}
			/*
			p_opView -> SetHighColor(EM_COLOR_TEXT);
			p_opView -> SetLowColor(EM_COLOR_SELECTED_MIDI_VIEW_BACKGROUND);
			p_opView -> DrawString(m_oFrame.m_vLeft + m_vStringLeft, m_oFrame.m_vTop + m_vStringTop, m_oName.c_str());
			*/
		}
		else
		{
			p_opView -> SetHighColor(EM_COLOR_MIDI_VIEW_BACKGROUND);
			p_opView -> FillRect(m_oFrame & p_oClippingRect);

			p_opView -> SetHighColor(EM_COLOR_MIDI_VIEW_SHADOW);
			p_opView -> StrokeLine(oDrawRect.m_vLeft, m_oFrame.m_vBottom, oDrawRect.m_vRight, m_oFrame.m_vBottom);
			p_opView -> StrokeLine(oDrawRect.m_vLeft, m_oFrame.m_vBottom - 1, oDrawRect.m_vRight, m_oFrame.m_vBottom - 1);

			p_opView -> SetHighColor(EM_COLOR_MIDI_VIEW_HIGHLIGHT);
			p_opView -> StrokeLine(oDrawRect.m_vLeft, m_oFrame.m_vTop, oDrawRect.m_vRight, m_oFrame.m_vTop);
			p_opView -> StrokeLine(oDrawRect.m_vLeft, m_oFrame.m_vTop + 1, oDrawRect.m_vRight, m_oFrame.m_vTop + 1);

			if(m_oFrame.m_vLeft >= oDrawRect.m_vLeft)
				p_opView -> StrokeLine(m_oFrame.m_vLeft, m_oFrame.m_vTop, m_oFrame.m_vLeft, m_oFrame.m_vBottom);
			if(m_oFrame.m_vLeft + 1 >= oDrawRect.m_vLeft)
				p_opView -> StrokeLine(m_oFrame.m_vLeft + 1, m_oFrame.m_vTop, m_oFrame.m_vLeft + 1, m_oFrame.m_vBottom - 1);

			p_opView -> SetHighColor(EM_COLOR_MIDI_VIEW_SHADOW);
			if(m_oFrame.m_vRight <= oDrawRect.m_vRight)
				p_opView -> StrokeLine(m_oFrame.m_vRight, m_oFrame.m_vTop, m_oFrame.m_vRight, m_oFrame.m_vBottom);
			if(m_oFrame.m_vRight - 1 <= oDrawRect.m_vRight)
				p_opView -> StrokeLine(m_oFrame.m_vRight - 1, m_oFrame.m_vTop + 1, m_oFrame.m_vRight - 1, m_oFrame.m_vBottom);

			/*
			p_opView -> SetHighColor(EM_COLOR_TEXT);
			p_opView -> SetLowColor(EM_COLOR_MIDI_VIEW_BACKGROUND);
			p_opView -> DrawString(m_oFrame.m_vLeft + m_vStringLeft, m_oFrame.m_vTop + m_vStringTop, m_oName.c_str());
			*/
			EMRect oLabelBackground(m_oLabelBackground);
			oLabelBackground.OffsetBy(m_oFrame.m_vLeft, m_oFrame.m_vTop);
			if(oLabelBackground.Intersects(p_oClippingRect))
			{
				m_opView -> SetHighColor(EM_COLOR_SELECTED_MIDI_VIEW_SHADOW);
				p_opView -> FillRect(oLabelBackground);
				p_opView -> SetHighColor(EMColor(255, 255, 255, 255));
				//p_opView -> SetLowColor(EM_COLOR_SELECTED_MIDI_VIEW_BACKGROUND);
				p_opView -> DrawString(m_oFrame.m_vLeft + m_vStringLeft, m_oFrame.m_vTop + m_vStringTop, m_oName.c_str());
			}
		}	
		/*
		if(GetMuted())
		{
			p_opView -> SetHighColor(EM_COLOR_SELECTED_MIDI_VIEW_SHADOW);
			p_opView -> StrokeLine(m_oFrame.m_vLeft, m_oFrame.m_vTop, m_oFrame.m_vRight, m_oFrame.m_vBottom);
			p_opView -> StrokeLine(m_oFrame.m_vLeft, m_oFrame.m_vBottom, m_oFrame.m_vRight, m_oFrame.m_vTop);
		}
		*/
	}

	p_opView -> ConstrainClippingRect(EMRect());
}

void MIDIGClip::DrawMovedClip(EMRect p_oClippingRect, EMView* p_opView, int64 p_vZoomScale, bool p_vDrawContent)
{
	if(!p_oClippingRect.Intersects(m_oMovedFrame))
		return;

	p_opView -> ConstrainClippingRect(m_oMovedFrame & p_oClippingRect);

	EMRect oDrawRect(m_oMovedFrame & p_oClippingRect);

	p_opView -> SetDrawingMode(EM_OP_ALPHA);
	p_opView -> SetHighColor(EM_COLOR_MOVED_MIDI_VIEW_BACKGROUND);
	p_opView -> FillRect(oDrawRect);
	
	p_opView -> SetHighColor(EM_COLOR_MOVED_MIDI_VIEW_SHADOW);
	p_opView -> StrokeLine(oDrawRect.m_vLeft, m_oMovedFrame.m_vBottom, oDrawRect.m_vRight, m_oMovedFrame.m_vBottom);
	if(m_oMovedFrame.m_vRight <= oDrawRect.m_vRight)
		p_opView -> StrokeLine(m_oMovedFrame.m_vRight, m_oMovedFrame.m_vTop, m_oMovedFrame.m_vRight, m_oMovedFrame.m_vBottom);
	p_opView -> SetHighColor(EM_COLOR_MOVED_MIDI_VIEW_HIGHLIGHT);
	p_opView -> StrokeLine(oDrawRect.m_vLeft, m_oMovedFrame.m_vTop, oDrawRect.m_vRight, m_oMovedFrame.m_vTop);
	if(m_oMovedFrame.m_vLeft >= oDrawRect.m_vLeft)
		p_opView -> StrokeLine(m_oMovedFrame.m_vLeft, m_oMovedFrame.m_vTop, m_oMovedFrame.m_vLeft, m_oMovedFrame.m_vBottom);

	p_opView -> SetHighColor(EM_COLOR_TEXT);
	p_opView -> SetLowColor(EM_COLOR_MOVED_MIDI_VIEW_BACKGROUND);
	p_opView -> DrawString(m_oMovedFrame.m_vLeft + m_vStringLeft, m_oMovedFrame.m_vTop + m_vStringTop, m_oName.c_str());
	p_opView -> SetDrawingMode(EM_OP_COPY);
	/*
	if(GetMuted())
	{
		p_opView -> SetHighColor(EM_COLOR_SELECTED_MIDI_VIEW_SHADOW);
		p_opView -> StrokeLine(m_oFrame.m_vLeft, m_oFrame.m_vTop, m_oFrame.m_vRight, m_oFrame.m_vBottom);
		p_opView -> StrokeLine(m_oFrame.m_vLeft, m_oFrame.m_vBottom, m_oFrame.m_vRight, m_oFrame.m_vTop);
	}
	*/
	p_opView -> ConstrainClippingRect(EMRect());
}

EMMediaMIDIEvent* MIDIGClip::GetMIDIEvent(int32 p_vX, int32 p_vY)
{
	int64 vTimeZoomScale = *(static_cast<int64*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_TIME_ZOOM_SCALE)));
	int vLowestKey = dynamic_cast<MIDIGClipContainer*>(m_opTrack -> GetClipContainer()) -> GetLowestKey();

	EMRect oEventFrame;
	int vKey;
	int64 vLeftPosition;
	int64 vRightPosition;
	list<EMMediaMIDIEvent*>::iterator oIterator = m_opMidiEvents -> begin();
	while(oIterator != m_opMidiEvents -> end())
	{
		if((*oIterator) -> m_vBaseType == EM_MIDI_EVENT_TYPE_NOTE_ON) // FIXME: Use a constant for note ons and other events
		{
			vKey = (*oIterator) -> m_vData1;
			vLeftPosition = *((*oIterator) -> m_vpStart);// + m_vPosition;
			vRightPosition = *((*oIterator) -> m_opNoteOff -> m_vpStart);

			oEventFrame.m_vLeft = vLeftPosition / vTimeZoomScale;
			oEventFrame.m_vRight = vRightPosition / vTimeZoomScale;
			if(oEventFrame.m_vLeft <= p_vX && oEventFrame.m_vRight >= p_vX)
			{
				oEventFrame.m_vTop = m_oFrame.m_vBottom - ((MIDI_KEY_HEIGHT - 1) * (vKey - vLowestKey + 1)) + 1;
				oEventFrame.m_vBottom = m_oFrame.m_vBottom - ((MIDI_KEY_HEIGHT - 1) * (vKey - vLowestKey)) - 1;
				if(oEventFrame.Contains(p_vX, p_vY))
					return *oIterator;
			}
		}
		oIterator++;
	}

	return NULL;
}

list<EMMediaMIDIEvent*>* MIDIGClip::GetMIDIEvents()
{
	return &m_oMidiEvents;
}

void MIDIGClip::GetMIDIEvents(EMRect p_oRect, bool p_vIntersectOK, list<EMMediaMIDIEvent*>* p_opEvents)
{
	int64 vTimeZoomScale = *(static_cast<int64*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_TIME_ZOOM_SCALE)));
	int vLowestKey = dynamic_cast<MIDIGClipContainer*>(m_opTrack -> GetClipContainer()) -> GetLowestKey();

	EMRect oEventFrame;
	int vKey;
	int64 vPosition;
//	list<EMMediaMIDIEvent*> oEvents;
//	m_oSelectedEvents.clear();
	list<EMMediaMIDIEvent*>::iterator oIterator = m_opMidiEvents -> begin();
	while(oIterator != m_opMidiEvents -> end())
	{
		if((*oIterator) -> m_vBaseType == EM_MIDI_EVENT_TYPE_NOTE_ON) // FIXME: Use a constant for note ons and other events
		{
			vKey = (*oIterator) -> m_vData1;
			vPosition = *((*oIterator) -> m_vpStart);// + m_vPosition;

			oEventFrame.m_vLeft = vPosition / vTimeZoomScale;
			oEventFrame.m_vTop = m_oFrame.m_vBottom - ((MIDI_KEY_HEIGHT - 1) * (vKey - vLowestKey + 1)) + 1;
			oEventFrame.m_vRight = *((*oIterator) -> m_opNoteOff -> m_vpStart) / vTimeZoomScale;
			oEventFrame.m_vBottom = m_oFrame.m_vBottom - ((MIDI_KEY_HEIGHT - 1) * (vKey - vLowestKey)) - 1;
			if(p_vIntersectOK)
			{
				if(oEventFrame.Intersects(p_oRect))
				{
//					(*oIterator) -> m_vIsSelected = true;
					p_opEvents -> push_back(*oIterator);
				}
//				else
//					(*oIterator) -> m_vIsSelected = false;
			}
			else
			{
				if(p_oRect.Contains(oEventFrame))
				{
//					(*oIterator) -> m_vIsSelected = true;
					p_opEvents -> push_back(*oIterator);
				}
//				else
//					(*oIterator) -> m_vIsSelected = false;
			}
		}
		oIterator++;
	}

//	return &oEvents;
}

EMMediaType MIDIGClip::GetType()
{
	return EM_TYPE_MIDI;
}

/*
EMGRubberBand* MIDIGClip::GetVelocityRubberBand()
{
	return &m_oVelocityRubberBand;
}

bool MIDIGClip::MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage)
{
	if(p_vMessage == EM_RUBBERBAND_POSITIONS_CHANGED)
	{
		list<EMGRubberBandNode*>* opAffectedNodes = m_oVelocityRubberBand.GetAffectedNodes();
		list<EMGRubberBandNode*>::iterator oIterator = opAffectedNodes -> begin();
		while(oIterator != opAffectedNodes -> end())
		{
eo << "New rubberband node value: " << (*oIterator) -> m_vValue << ef;
			static_cast<EMMediaMIDIEvent*>((*oIterator) -> m_opDataLink) -> m_vData2 = (*oIterator) -> m_vValue;
			oIterator++;
		}

		return true;
	}

	return false;
}

bool MIDIGClip::MouseDown(EMView* p_opView, int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree)
{
eo << "MIDIGClip::MouseDown" << ef;

	ToolboxMode eToolboxMode = static_cast<ToolboxMode>(*(static_cast<int*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_TOOLBOX_MODE))));
	EMEditMode eEditMode = static_cast<EMEditMode>(*(static_cast<int*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_EDIT_MODE))));
	ClipEditDisplayMode eDisplayMode = dynamic_cast<MIDIGClipContainer*>(m_opTrack -> GetClipContainer()) -> GetDisplayMode();

	if(eEditMode == EM_CLIP_EDIT_MODE && eDisplayMode == CE_DISPLAY_MODE_MIDI_VELOCITY)
		return m_oVelocityRubberBand.MouseDown(p_vX, p_vY, p_vButtonOne, p_vButtonTwo, p_vButtonThree, eToolboxMode);
	else
		return GClip::MouseDown(p_opView, p_vX, p_vY, p_vButtonOne, p_vButtonTwo, p_vButtonThree);
}

bool MIDIGClip::MouseMoved(EMView* p_opView, int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree)
{
	ToolboxMode eToolboxMode = static_cast<ToolboxMode>(*(static_cast<int*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_TOOLBOX_MODE))));
	EMEditMode eEditMode = static_cast<EMEditMode>(*(static_cast<int*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_EDIT_MODE))));
	ClipEditDisplayMode eDisplayMode = dynamic_cast<MIDIGClipContainer*>(m_opTrack -> GetClipContainer()) -> GetDisplayMode();

	if(eEditMode == EM_CLIP_EDIT_MODE && eDisplayMode == CE_DISPLAY_MODE_MIDI_VELOCITY)
		return m_oVelocityRubberBand.MouseMoved(p_vX, p_vY, p_vButtonOne, p_vButtonTwo, p_vButtonThree, eToolboxMode);
	else
		return GClip::MouseMoved(p_opView, p_vX, p_vY, p_vButtonOne, p_vButtonTwo, p_vButtonThree);
}

bool MIDIGClip::MouseUp(EMView* p_opView, int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree)
{
	ToolboxMode eToolboxMode = static_cast<ToolboxMode>(*(static_cast<int*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_TOOLBOX_MODE))));
	EMEditMode eEditMode = static_cast<EMEditMode>(*(static_cast<int*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_EDIT_MODE))));
	ClipEditDisplayMode eDisplayMode = dynamic_cast<MIDIGClipContainer*>(m_opTrack -> GetClipContainer()) -> GetDisplayMode();

	if(eEditMode == EM_CLIP_EDIT_MODE && eDisplayMode == CE_DISPLAY_MODE_MIDI_VELOCITY)
		return m_oVelocityRubberBand.MouseUp(p_vX, p_vY, eToolboxMode);
	else
		return GClip::MouseUp(p_opView, p_vX, p_vY, p_vButtonOne, p_vButtonTwo, p_vButtonThree);
}
*/
bool MIDIGClip::MoveClip(int32 p_vTrackOffset, int64 p_vPositionOffset)
{
	//p_vPositionOffset = EMGUIUtilities::Instance() -> TruncateSample(p_vPositionOffset);
	if(EMGClip::MoveClip(p_vTrackOffset, p_vPositionOffset))
	{
		UpdateContent();
		return true;
	}

	return false;
}
/*
void MIDIGClip::SetFrame(EMRect p_oFrame)
{
	m_oVelocityRubberBand.SetFrame(p_oFrame);
	EMGClip::SetFrame(p_oFrame);
}
*/

// FIXME: p_opView is no longer needed
void MIDIGClip::SetLabel(char* p_oLabel, EMView* p_opView)
{
	EMGClip::SetLabel(p_oLabel, m_opView);
	m_vNamePositionUpdated = false;
}

/*
// Deselects all events not included in p_opEvents
void MIDIGClip::SetSelected(list<EMMediaMIDIEvent*>* p_opEvents)
{
	list<EMMediaMIDIEvent*>::iterator oIterator = m_oSelectedEvents.begin();
	while(oIterator != m_oSelectedEvents.end())
	{
		(*oIterator) -> m_vIsSelected = false;
		oIterator++;
	}

	oIterator = p_opEvents -> begin();
	while(oIterator != p_opEvents -> end())
	{
		(*oIterator) -> m_vIsSelected = true;
		oIterator++;
	}
	m_oSelectedEvents = *p_opEvents;
}
*/

void MIDIGClip::UpdateContent()
{
	// Retrieving the events from the media engine
	m_oMidiEvents = *(static_cast<list<EMMediaMIDIEvent*>*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_MIDI_EVENTS_FOR_CLIP, &m_vMediaClipID)));
	m_opMidiEvents = &m_oMidiEvents;
	/*
	// Building the rubberband values
	m_oVelocityRubberBand.ClearNodes();

	list<EMMediaMIDIEvent*>::iterator oIterator = m_opMidiEvents -> begin();
	while(oIterator != m_opMidiEvents -> end())
	{
		if((*oIterator) -> m_vBaseType == EM_MIDI_EVENT_TYPE_NOTE_ON)
			m_oVelocityRubberBand.CreateNode(*((*oIterator) -> m_vpStart), (*oIterator) -> m_vData2, *oIterator, false, false);
		oIterator++;
	}
	*/
}

