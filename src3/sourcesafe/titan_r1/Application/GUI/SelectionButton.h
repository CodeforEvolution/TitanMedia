/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* Contains some native code
*******************************************************/

#include "EMGlobals.h"

#ifndef __SELECTION_BUTTON
#define __SELECTION_BUTTON

#include "EMListener.h"
#include "EMListenerRepository.h"
#include "EMMessages.h"
#include "EMSlimButton.h"

enum SelectionButtonMode
{
	SELECTION_BUTTON_MODE_NORMAL,
	SELECTION_BUTTON_MODE_MIDI_KEYS,
	SELECTION_BUTTON_MODE_AUDIO,
	SELECTION_BUTTON_MODE_VIDEO,
	SELECTION_BUTTON_MODE_TRANSITION
};

const uint32 SELECTION_BUTTON_LOWEST_KEY_CHANGED = EM_MESSAGE_BASE_OFFSET + 1;
const uint32 SELECTION_BUTTON_DISPLAY_MODE_CHANGED = EM_MESSAGE_BASE_OFFSET + 2;

enum ClipEditDisplayMode
{
	CE_DISPLAY_MODE_DEFAULT = 1,
	CE_DISPLAY_MODE_MIDI_PITCH_BEND = 2,
	CE_DISPLAY_MODE_MIDI_VELOCITY = 3,
	CE_DISPLAY_MODE_MIDI_MODULATION_WHEEL = 4,
	CE_DISPLAY_MODE_MIDI_AFTERTOUCH = 5,
	CE_DISPLAY_MODE_AUDIO_VOLUME = 6,
	CE_DISPLAY_MODE_AUDIO_PAN = 7
};

class EMGTrack;
class EMPopUpMenu;

class SelectionButton : public EMSlimButton, public EMListener, public EMListenerRepository
{
public:
	SelectionButton(EMView* p_opView, EMRect p_oFrame, string* p_opOnString, 
				string* p_opOffString, uint32 p_vCommandID, 
				void* p_opCommandParameterOne = NULL, 
				void* p_opCommandParameterTwo = NULL,
				void* p_opCommandParameterThree = NULL,
				bool p_vLeftBorder = true,
				bool p_vTopBorder = true, bool p_vRightBorder = true, 
				bool p_vBottomBorder = true, 
				EMColor p_oOnColor = EM_COLOR_BUTTON_DEFAULT_ON,
				EMColor p_oOffColor = EM_COLOR_BUTTON_DEFAULT_OFF, 
				EMBitmap* p_opOnBitmap = NULL, EMBitmap* p_opOffBitmap = NULL,
				EMBitmap* p_opDisabledOnBitmap = NULL,
				EMBitmap* p_opDisabledOffBitmap = NULL
				);
	virtual ~SelectionButton();
	void Draw(EMRect p_oClippingRect, EMView* p_opView);
	ClipEditDisplayMode GetDisplayMode();
	int GetLowestKey();
	bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage);
	bool MouseDown(int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree, EMGTrack* p_opTrack);
	bool MouseMoved(int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree);
	bool MouseUp(int32 p_vX, int32 p_vY);
	void SetMode(SelectionButtonMode p_eMode);

private:
	ClipEditDisplayMode m_eDisplayMode;
	SelectionButtonMode m_eMode;
	static EMPopUpMenu* m_opPopUpMenu;
	EMView* m_opView;
	int m_vKeyBelowMouse;
	int m_vLowestKey; // The lowest displayed key, goes from 0 to 127 (C0 to G10)
	bool m_vMouseDown;
	int m_vMouseDownKey;
	int32 m_vMouseDownX;
	int32 m_vMouseDownY;
};

#endif