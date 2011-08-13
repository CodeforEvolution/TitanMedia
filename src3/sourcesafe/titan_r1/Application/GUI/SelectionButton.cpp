#include "SelectionButton.h"

#include "EMFactory.h"
#include "EMGTrack.h"
#include "EMGTrackRepository.h"
#include "EMKeyboard.h"
#include "EMKeyboardGlobals.h"
#include "EMMenuItem.h"
#include "EMPopUpMenu.h"
#include "EMResourceRepository.h"
#include "EMView.h"
#include "ResourceIDs.h"

#include "GUIGlobals.h"

#define COLOR_ON_BACKGROUND EMColor(200, 200, 250, 255)
#define COLOR_ON_HIGHLIGHT EMColor(230, 230, 255, 255)
#define COLOR_ON_SHADOW EMColor(150, 150, 200, 255)
#define COLOR_OFF_BACKGROUND EMColor(150, 150, 200, 255)
#define COLOR_OFF_HIGHLIGHT EMColor(200, 200, 250, 255)
#define COLOR_OFF_SHADOW EMColor(100, 100, 150, 255)

const float STRIPED_PROPORTION = 0.25;
const int32 STRIPED_SPACING = 2;
const int32 STRIPED_X_INSET = 3;

EMPopUpMenu* SelectionButton::m_opPopUpMenu = NULL;

SelectionButton::SelectionButton(EMView* p_opView, EMRect p_oFrame, string* p_opOnString, 
		string* p_opOffString, uint32 p_vCommandID, 
		void* p_opCommandParameterOne, 
		void* p_opCommandParameterTwo,
		void* p_opCommandParameterThree,
		bool p_vLeftBorder,
		bool p_vTopBorder , bool p_vRightBorder, 
		bool p_vBottomBorder, 
		EMColor p_oOnColor,
		EMColor p_oOffColor, 
		EMBitmap* p_opOnBitmap, EMBitmap* p_opOffBitmap,
		EMBitmap* p_opDisabledOnBitmap,
		EMBitmap* p_opDisabledOffBitmap
		) :
EMSlimButton(p_opView, p_oFrame, p_opOnString, p_opOffString, p_vCommandID, 
		p_opCommandParameterOne, p_opCommandParameterTwo, p_opCommandParameterThree,
		p_vLeftBorder, p_vTopBorder, p_vRightBorder, p_vBottomBorder,
		p_oOnColor, p_oOffColor, p_opOnBitmap, p_opOffBitmap,
		p_opDisabledOnBitmap, p_opDisabledOffBitmap),
m_eDisplayMode(CE_DISPLAY_MODE_DEFAULT),
m_eMode(SELECTION_BUTTON_MODE_NORMAL),
//m_opPopUpMenu(NULL),
m_opView(p_opView),
m_vKeyBelowMouse(0),
m_vLowestKey(36),
m_vMouseDown(false)
{
}

SelectionButton::~SelectionButton()
{
	delete m_opPopUpMenu;
	m_opPopUpMenu = NULL;
}
/*
// FIXME: Move these to GUIGlobals.h
const int MIDI_KEY_HEIGHT = 9; // Should be an odd number
const int MIDI_MAX_NR_KEYS = 127;
const int MIDI_BLACK_KEY_WIDTH = 8; // How far black keys "stretches out" towards the pianist
const int MIDI_KEYS_IN_OCTAVE = 12;
*/
void SelectionButton::Draw(EMRect p_oClippingRect, EMView* p_opView)
{
	if(!m_vVisible)
		return;
	
	if(!m_oFrame.Intersects(p_oClippingRect))
		return;
	
	if(m_eMode == SELECTION_BUTTON_MODE_MIDI_KEYS)
	{
		// Let's draw ourselves a piano
		EMRect oFrame = Frame();

		p_opView -> SetHighColor(EM_COLOR_VIEW_BACKGROUND_HIGHLIGHT);
		p_opView -> FillRect(oFrame & p_oClippingRect);

		int vCurrentKey(m_vLowestKey);
		int32 vCurrentPos = Frame().m_vBottom;

		p_opView -> SetHighColor(EM_COLOR_VIEW_BACKGROUND_SHADOW);
		int vKeyInOctave;
		char vOctaveNumber[4];
		while((vCurrentKey < MIDI_MAX_NR_KEYS - 1) && (vCurrentPos > Frame().m_vTop))
		{
			vKeyInOctave = (vCurrentKey + MIDI_KEYS_IN_OCTAVE) % MIDI_KEYS_IN_OCTAVE;

			if(vKeyInOctave == 0 || vKeyInOctave == 5) // The "C" and "G" key
			{
				p_opView -> StrokeLine(oFrame.m_vLeft, vCurrentPos, oFrame.m_vRight, vCurrentPos);

				if(vKeyInOctave == 0) // The "C" key
				{
					sprintf(vOctaveNumber, "C%d\0", (vCurrentKey / MIDI_KEYS_IN_OCTAVE));
					p_opView -> SetHighColor(EM_COLOR_TEXT);
					p_opView -> ConstrainClippingRect(oFrame);
					p_opView -> DrawString(oFrame.m_vLeft + 1, vCurrentPos, vOctaveNumber);
					p_opView -> ConstrainClippingRect(EMRect());
					p_opView -> SetHighColor(EM_COLOR_VIEW_BACKGROUND_SHADOW);
				}
/*
				if(vCurrentKey == m_vKeyBelowMouse && m_eDisplayMode == CE_DISPLAY_MODE_DEFAULT)
				{
					p_opView -> SetHighColor(EM_COLOR_BUTTON_DEFAULT_ON);
					p_opView -> FillRect(EMRect(oFrame.m_vRight - 7, vCurrentPos - 8, oFrame.m_vRight - 2, vCurrentPos - 1));
					p_opView -> SetHighColor(EM_COLOR_VIEW_BACKGROUND_SHADOW);
				}
*/
			}
			else if(vKeyInOctave == 1 || vKeyInOctave == 3 || vKeyInOctave == 6 || vKeyInOctave == 8 || vKeyInOctave == 10) // The "C#", "D#", "G#", "A#" and "B" key
			{
				p_opView -> FillRect(EMRect(oFrame.m_vLeft, vCurrentPos - (MIDI_KEY_HEIGHT - 2), oFrame.m_vLeft + MIDI_BLACK_KEY_WIDTH - 1, vCurrentPos - 1));
				p_opView -> StrokeLine(oFrame.m_vLeft + MIDI_BLACK_KEY_WIDTH - 1, vCurrentPos - (static_cast<int>(MIDI_KEY_HEIGHT / 2) + 0), 
								oFrame.m_vRight, vCurrentPos - (static_cast<int>(MIDI_KEY_HEIGHT / 2) + 0));
/*
				if(vCurrentKey == m_vKeyBelowMouse && m_eDisplayMode == CE_DISPLAY_MODE_DEFAULT)
				{
					p_opView -> SetHighColor(EM_COLOR_BUTTON_DEFAULT_ON);
//					p_opView -> FillRect(oFrame.m_vLeft, vCurrentPos, oFrame.m_vLeft + MIDI_BLACK_KEY_WIDTH - 1, vCurrentPos - (MIDI_KEY_HEIGHT - 1)));//EMRect(oFrame.m_vLeft + MIDI_BLACK_KEY_WIDTH - 7, vCurrentPos + 6, oFrame.m_vLeft + MIDI_BLACK_KEY_WIDTH - 2, vCurrentPos - 1));
					p_opView -> SetHighColor(EM_COLOR_VIEW_BACKGROUND_SHADOW);
				}
*/
			}
/*
			else if(vCurrentKey == m_vKeyBelowMouse && m_eDisplayMode == CE_DISPLAY_MODE_DEFAULT)
			{
				p_opView -> SetHighColor(EM_COLOR_BUTTON_DEFAULT_ON);
				p_opView -> FillRect(EMRect(oFrame.m_vRight - 7, vCurrentPos - 8, oFrame.m_vRight - 2, vCurrentPos - 1));
				p_opView -> SetHighColor(EM_COLOR_VIEW_BACKGROUND_SHADOW);
			}
*/
			vCurrentKey++;
			vCurrentPos -= MIDI_KEY_HEIGHT - 1;
		}

		p_opView -> SetHighColor(EM_COLOR_VIEW_BACKGROUND_ULTRA_HIGHLIGHT);
		p_opView -> StrokeLine(oFrame.m_vLeft, oFrame.m_vTop, oFrame.m_vLeft, oFrame.m_vBottom);
		p_opView -> SetHighColor(EM_COLOR_VIEW_BACKGROUND_SHADOW);
		p_opView -> StrokeLine(oFrame.m_vRight, oFrame.m_vTop, oFrame.m_vRight, oFrame.m_vBottom);

		return;
	}

	else if(m_eMode == SELECTION_BUTTON_MODE_AUDIO)
	{
		// Let's draw ourselves some kind of audio signal scale
		EMRect oFrame = Frame();

		p_opView -> SetHighColor(EM_COLOR_VIEW_BACKGROUND_HIGHLIGHT);
		p_opView -> FillRect(oFrame & p_oClippingRect);

		p_opView -> SetHighColor(EM_COLOR_VIEW_BACKGROUND_SHADOW);
		int32 vButtonHeight = oFrame.GetHeight();
		p_opView -> StrokeLine(oFrame.m_vLeft + 1, oFrame.m_vTop + vButtonHeight / 2, oFrame.m_vRight - 1, oFrame.m_vTop + vButtonHeight / 2);
		p_opView -> StrokeLine(oFrame.m_vLeft + 1, oFrame.m_vTop + vButtonHeight / 4, oFrame.m_vRight - 1, oFrame.m_vTop + vButtonHeight / 4);
		p_opView -> StrokeLine(oFrame.m_vLeft + 1, oFrame.m_vTop + ((vButtonHeight * 3) / 4), oFrame.m_vRight - 1, oFrame.m_vTop + ((vButtonHeight * 3) / 4));

		p_opView -> SetHighColor(EM_COLOR_VIEW_BACKGROUND_ULTRA_HIGHLIGHT);
		p_opView -> StrokeLine(oFrame.m_vLeft, oFrame.m_vTop, oFrame.m_vLeft, oFrame.m_vBottom);
		p_opView -> SetHighColor(EM_COLOR_VIEW_BACKGROUND_SHADOW);
		p_opView -> StrokeLine(oFrame.m_vRight, oFrame.m_vTop, oFrame.m_vRight, oFrame.m_vBottom);

		return;
	}

	else if(m_eMode == SELECTION_BUTTON_MODE_VIDEO || m_eMode == SELECTION_BUTTON_MODE_TRANSITION)
	{
		// Let's draw ourselves a blank button
		EMRect oFrame = Frame();

		p_opView -> SetHighColor(EM_COLOR_VIEW_BACKGROUND_HIGHLIGHT);
		p_opView -> FillRect(oFrame & p_oClippingRect);

		p_opView -> SetHighColor(EM_COLOR_VIEW_BACKGROUND_ULTRA_HIGHLIGHT);
		p_opView -> StrokeLine(oFrame.m_vLeft, oFrame.m_vTop, oFrame.m_vLeft, oFrame.m_vBottom);
		p_opView -> SetHighColor(EM_COLOR_VIEW_BACKGROUND_SHADOW);
		p_opView -> StrokeLine(oFrame.m_vRight, oFrame.m_vTop, oFrame.m_vRight, oFrame.m_vBottom);

		return;
	}

	
	EMColor oBackgroundColor;
	EMColor oShadowColor;
	EMColor oHighlightColor;

	if(GetOn())
	{
		oBackgroundColor = COLOR_ON_BACKGROUND;
		oShadowColor = COLOR_ON_SHADOW;
		oHighlightColor = COLOR_ON_HIGHLIGHT;
	}
	else
	{
		oBackgroundColor = COLOR_OFF_BACKGROUND;
		oShadowColor = COLOR_OFF_SHADOW;
		oHighlightColor = COLOR_OFF_HIGHLIGHT;
	}

	p_opView -> SetHighColor(oShadowColor);
	p_opView -> StrokeLine(Frame().m_vLeft, Frame().m_vBottom, Frame().m_vRight, Frame().m_vBottom);
	p_opView -> StrokeLine(Frame().m_vRight, Frame().m_vTop, Frame().m_vRight, Frame().m_vBottom);
	p_opView -> SetHighColor(oHighlightColor);
	p_opView -> StrokeLine(Frame().m_vLeft, Frame().m_vTop, Frame().m_vRight, Frame().m_vTop);
	p_opView -> StrokeLine(Frame().m_vLeft, Frame().m_vTop, Frame().m_vLeft, Frame().m_vBottom);
	p_opView -> SetHighColor(oBackgroundColor);
	p_opView -> FillRect(EMRect(Frame().m_vLeft + 1, Frame().m_vTop + 1, Frame().m_vRight - 1, Frame().m_vBottom - 1)/* & p_oClippingRect*/);
	
	int32 vCurrentYPosition = Frame().m_vTop + (Frame().GetHeight() * STRIPED_PROPORTION);
	int32 vEndYPosition = Frame().m_vBottom - (Frame().GetHeight() * STRIPED_PROPORTION) + 1;
	int32 vXBegin = Frame().m_vLeft + STRIPED_X_INSET;
	int32 vXEnd = Frame().m_vRight - STRIPED_X_INSET;
	while(vCurrentYPosition < vEndYPosition)
	{
		p_opView -> SetHighColor(oHighlightColor);
		p_opView -> StrokeLine(vXBegin, vCurrentYPosition, vXEnd, vCurrentYPosition);
		vCurrentYPosition += 1;
		p_opView -> SetHighColor(oShadowColor);
		p_opView -> StrokeLine(vXBegin, vCurrentYPosition, vXEnd, vCurrentYPosition);
		vCurrentYPosition += STRIPED_SPACING;
	}
}

ClipEditDisplayMode SelectionButton::GetDisplayMode()
{
	return m_eDisplayMode;
}

int SelectionButton::GetLowestKey()
{
	return m_vLowestKey;
}

bool SelectionButton::MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage)
{
	// Assume that messages only come from a pop-up menu
	m_eDisplayMode = static_cast<ClipEditDisplayMode>(p_vMessage);
	Notify(SELECTION_BUTTON_DISPLAY_MODE_CHANGED);
	return true;
}

// Returns true if it should be invalidated
bool SelectionButton::MouseDown(int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree, EMGTrack* p_opTrack)
{
	if(!m_vEnabled || !m_vVisible)
		return false;
	if(!m_oFrame.Contains(p_vX, p_vY))
		return false;

	if(m_eMode == SELECTION_BUTTON_MODE_MIDI_KEYS)
	{
		if(p_vButtonTwo)
		{
			delete m_opPopUpMenu;
			m_opPopUpMenu = EMFactory::Instance() -> CreatePopUpMenu();
			m_opPopUpMenu -> AddListener(this);

			list<string*>* opDisplayModes = EMResourceRepository::GetStringList(RES_SB_MIDI_DISPLAY_MODES);
			list<string*>::iterator oIterator = opDisplayModes -> begin();
				

			EMMenuItem* opItem;
			opItem = EMFactory::Instance() -> CreateMenuItem(*oIterator, CE_DISPLAY_MODE_DEFAULT);//EM_new string("Note on/off"), CE_DISPLAY_MODE_DEFAULT); // FIXME: Leaks memory, use resource instead
			m_opPopUpMenu -> AddItem(opItem);
			if(m_eDisplayMode == CE_DISPLAY_MODE_DEFAULT)
				opItem -> SetMarked(true);
			oIterator++;

			opItem = EMFactory::Instance() -> CreateMenuItem(*oIterator, CE_DISPLAY_MODE_MIDI_VELOCITY);//EM_new string("Velocity"), CE_DISPLAY_MODE_MIDI_VELOCITY); // FIXME: Leaks memory, use resource instead
			m_opPopUpMenu -> AddItem(opItem);
			if(m_eDisplayMode == CE_DISPLAY_MODE_MIDI_VELOCITY)
				opItem -> SetMarked(true);
			oIterator++;

			opItem = EMFactory::Instance() -> CreateMenuItem(*oIterator, CE_DISPLAY_MODE_MIDI_MODULATION_WHEEL);//EM_new string("Modulation Wheel"), CE_DISPLAY_MODE_MODULATION_WHEEL); // FIXME: Leaks memory, use resource instead
			m_opPopUpMenu -> AddItem(opItem);
			if(m_eDisplayMode == CE_DISPLAY_MODE_MIDI_MODULATION_WHEEL)
				opItem -> SetMarked(true);
			oIterator++;

			opItem = EMFactory::Instance() -> CreateMenuItem(*oIterator, CE_DISPLAY_MODE_MIDI_PITCH_BEND);//EM_new string("Pitch Bend"), CE_DISPLAY_MODE_MIDI_PITCH_BEND); // FIXME: Leaks memory, use resource instead
			m_opPopUpMenu -> AddItem(opItem);
			if(m_eDisplayMode == CE_DISPLAY_MODE_MIDI_PITCH_BEND)
				opItem -> SetMarked(true);
			oIterator++;

			opItem = EMFactory::Instance() -> CreateMenuItem(*oIterator, CE_DISPLAY_MODE_MIDI_AFTERTOUCH);//EM_new string("Aftertouch"), CE_DISPLAY_MODE_MIDI_AFTERTOUCH); // FIXME: Leaks memory, use resource instead
			m_opPopUpMenu -> AddItem(opItem);
			if(m_eDisplayMode == CE_DISPLAY_MODE_MIDI_AFTERTOUCH)
				opItem -> SetMarked(true);

			int32 vNewX = p_vX;
			int32 vNewY = p_vY;
			m_opView -> ConvertToScreenCoordinates(vNewX, vNewY);
			m_opPopUpMenu -> PopUp(vNewX, vNewY, m_opView);
		}

		else if(p_vButtonOne)
		{
			m_vMouseDown = true;
			m_vMouseDownX = p_vX;
			m_vMouseDownY = p_vY;
			m_vMouseDownKey = m_vLowestKey;
		}

		return false;
	}

	else if(m_eMode == SELECTION_BUTTON_MODE_AUDIO)
	{
		if(p_vButtonTwo)
		{
			delete m_opPopUpMenu;
			m_opPopUpMenu = EMFactory::Instance() -> CreatePopUpMenu();
			m_opPopUpMenu -> AddListener(this);

			list<string*>* opDisplayModes = EMResourceRepository::GetStringList(RES_SB_AUDIO_DISPLAY_MODES);
			list<string*>::iterator oIterator = opDisplayModes -> begin();
				

			EMMenuItem* opItem;
			opItem = EMFactory::Instance() -> CreateMenuItem(*oIterator, CE_DISPLAY_MODE_DEFAULT);//EM_new string("Note on/off"), CE_DISPLAY_MODE_DEFAULT); // FIXME: Leaks memory, use resource instead
			m_opPopUpMenu -> AddItem(opItem);
			if(m_eDisplayMode == CE_DISPLAY_MODE_DEFAULT)
				opItem -> SetMarked(true);
			oIterator++;

			opItem = EMFactory::Instance() -> CreateMenuItem(*oIterator, CE_DISPLAY_MODE_AUDIO_VOLUME);//EM_new string("Velocity"), CE_DISPLAY_MODE_MIDI_VELOCITY); // FIXME: Leaks memory, use resource instead
			m_opPopUpMenu -> AddItem(opItem);
			if(m_eDisplayMode == CE_DISPLAY_MODE_AUDIO_VOLUME)
				opItem -> SetMarked(true);
			oIterator++;

			opItem = EMFactory::Instance() -> CreateMenuItem(*oIterator, CE_DISPLAY_MODE_AUDIO_PAN);//EM_new string("Modulation Wheel"), CE_DISPLAY_MODE_MODULATION_WHEEL); // FIXME: Leaks memory, use resource instead
			m_opPopUpMenu -> AddItem(opItem);
			if(m_eDisplayMode == CE_DISPLAY_MODE_AUDIO_PAN)
				opItem -> SetMarked(true);
			oIterator++;

			int32 vNewX = p_vX;
			int32 vNewY = p_vY;
			m_opView -> ConvertToScreenCoordinates(vNewX, vNewY);
			m_opPopUpMenu -> PopUp(vNewX, vNewY, m_opView);
		}

		else if(p_vButtonOne)
		{
			m_vMouseDown = true;
			m_vMouseDownX = p_vX;
			m_vMouseDownY = p_vY;
			m_vMouseDownKey = m_vLowestKey;
		}

		return false;
	}

	uint32 vModifiers = EMKeyboard::Instance() -> GetModifiers();

	bool vShift = (vModifiers & EM_SHIFT_KEY) == EM_SHIFT_KEY;
	bool vControl = (vModifiers & EM_CONTROL_KEY) == EM_CONTROL_KEY;
	
	if(m_vOn && !vControl)
		return false;
	if(vControl)
		p_opTrack -> GetTrackInfo() -> SetSelected(!(p_opTrack -> GetTrackInfo() -> IsSelected()));
	else if(!m_vOn && !vShift && !vControl)
	{
		for(uint32 vIndex = 0; vIndex < EMGTrackRepository::Instance() -> GetNrTracks(); vIndex++)
			if(EMGTrackRepository::Instance() -> GetTrack(vIndex) != p_opTrack)
				EMGTrackRepository::Instance() -> GetTrack(vIndex) -> GetTrackInfo() -> SetSelected(false);
		p_opTrack -> GetTrackInfo() -> SetSelected(!(p_opTrack -> GetTrackInfo() -> IsSelected()));
	}
	else if(!m_vOn && vShift)
	{
		uint32 vStartIndex = 0;
		uint32 vEndIndex = 0;
		bool vFoundSelection = false;
		uint32 vMyIndex = EMGTrackRepository::Instance() -> GetTrackIndex(p_opTrack);
		if(vMyIndex > 0)
		{
			for(int32 vIndex = vMyIndex; vIndex > -1; vIndex--)
				if(EMGTrackRepository::Instance() -> GetTrack(vIndex) -> GetTrackInfo() -> IsSelected())
				{
					vFoundSelection = true;
					vStartIndex = vIndex;
					vEndIndex = vMyIndex + 1;
					break;
				}
		}
		if(!vFoundSelection)
			if(vMyIndex < EMGTrackRepository::Instance() -> GetNrTracks() - 1)
			{
				for(uint32 vIndex = vMyIndex + 1; vIndex < EMGTrackRepository::Instance() -> GetNrTracks(); vIndex++)
					if(EMGTrackRepository::Instance() -> GetTrack(vIndex) -> GetTrackInfo() -> IsSelected())
					{
						vFoundSelection = true;
						vStartIndex = vMyIndex;
						vEndIndex = vIndex + 1;
						break;
					}
			}
		if(vFoundSelection)
			for(uint32 vIndex = vStartIndex; vIndex < vEndIndex; vIndex++)
				EMGTrackRepository::Instance() -> GetTrack(vIndex) -> GetTrackInfo() -> SetSelected(true);
	}

	EMGTrackRepository::Instance() -> TrackSelectionHasChanged();

	return true;
}

bool SelectionButton::MouseMoved(int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree)
{
	if(!m_vMouseDown)
		return false;

	int vOldKey = m_vLowestKey;
	m_vLowestKey = m_vMouseDownKey - static_cast<int>((m_vMouseDownY - p_vY) / (MIDI_KEY_HEIGHT - 1));
	if(m_vLowestKey < 0)
		m_vLowestKey = 0;
	else if(Frame().GetHeight() / (MIDI_KEY_HEIGHT - 1) > 128 - m_vLowestKey)
		m_vLowestKey = 128 - Frame().GetHeight() / (MIDI_KEY_HEIGHT - 1);

	if(m_vLowestKey != vOldKey)
	{
		Notify(SELECTION_BUTTON_LOWEST_KEY_CHANGED);
		return true;
	}

	return false;
}

bool SelectionButton::MouseUp(int32 p_vX, int32 p_vY)
{
	EMSlimButton::MouseUp(p_vX, p_vY);
	m_vMouseDown = false;
	return false;
}

void SelectionButton::SetMode(SelectionButtonMode p_eMode)
{
	if(p_eMode == m_eMode)
		return;

	m_eMode = p_eMode;

	// Count this as a change of display mode
	Notify(SELECTION_BUTTON_DISPLAY_MODE_CHANGED);
}