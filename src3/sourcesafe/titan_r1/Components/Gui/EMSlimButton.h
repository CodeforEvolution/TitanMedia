/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_SLIM_BUTTON
#define __EM_SLIM_BUTTON

#include "EMGUIGlobals.h"
#include "EMRect.h"
#include "EMThreadListener.h"

#include <string>

class EMBitmap;
class EMToolTip;
class EMView;

enum EMSlimButtonMode
{
	EM_SLIM_BUTTON_DRAW_BLANK,
	EM_SLIM_BUTTON_STAY_DOWN, // Standard button behavior
	EM_SLIM_BUTTON_FOLLOW_MOUSE, // Button is only on when mouse button is pressed
	EM_SLIM_BUTTON_FOLLOW_MOUSE_REPEAT_COMMAND, // As above, and is continously sending commands when on
	EM_SLIM_BUTTON_RADIO, // Can only be set to on (not off) by the user
	EM_SLIM_BUTTON_DRAW_LABEL, // Draws itself as a label
};

class EMSlimButton : public EMThreadListener
{
public:
	EMSlimButton(EMView* p_opView, EMRect p_oFrame, string* p_opOnString, 
				string* p_opOffString, uint32 p_vCommandID = 0, 
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
				EMBitmap* p_opDisabledOffBitmap = NULL,
				const char* p_vpToolTipLabel = NULL
				);
	virtual ~EMSlimButton();
//	EMRect Bounds();
	bool Contains(int32 p_vX, int32 p_vY);
	bool Intersects(EMRect p_oRect);
	virtual void Draw(EMRect p_oClippingRect, EMView* p_opView);
	virtual void ExecuteCommand();
	void Flash();
	EMRect Frame();
	bool GetEnabled();
	bool GetOn();
	bool GetVisible();
	virtual bool MouseDown(int32 p_vX, int32 p_vY);
	virtual bool MouseUp(int32 p_vX, int32 p_vY);
	void SetBitmaps(EMBitmap* p_opOnBitmap, EMBitmap* p_opOffBitmap, EMBitmap* p_opDisabledOnBitmap = NULL, EMBitmap* p_opDisabledOffBitmap = NULL);
	void SetColors(EMColor p_oOnColor, EMColor p_oOffColor = EM_COLOR_BUTTON_DEFAULT_OFF);
	void SetCommand(uint32 p_vCommandID, void* p_opCommandParameterOne = NULL, void* p_opCommandParameterTwo = NULL, void* p_opCommandParameterThree = NULL);
	void SetEnabled(bool p_vEnabled);
	void SetFrame(EMRect p_oFrame);
	void SetMode(EMSlimButtonMode p_eMode);
	bool SetOn(bool p_vOn);
	void SetOnString(string* p_opString);
	void SetOffString(string* p_opString);
	void SetVisible(bool p_vVisible);
	void ThreadRun(EMThread* p_opThread);
	void ToggleOn();

protected:
	void CalculatePositions();

protected:
	EMSlimButtonMode m_eMode;
	EMRect m_oFrame;
	EMRect m_oInsideBorder;
	EMColor m_oOffColor;
	void* m_opCommandParameterOne;
	void* m_opCommandParameterTwo;
	void* m_opCommandParameterThree;
	EMBitmap* m_opDisabledOffBitmap;
	EMBitmap* m_opDisabledOnBitmap;
	EMBitmap* m_opOffBitmap;
	string* m_opOffString;
	EMBitmap* m_opOnBitmap;
	EMColor m_oOnColor;
	string* m_opOnString;
	EMToolTip* m_opToolTip;
	EMView* m_opView;
	bool m_vBottomBorder;
	uint32 m_vCommandID;
	bool m_vEnabled;
	bool m_vFlash;
	int m_vFlashCounter;
	bool m_vLeftBorder;
	bool m_vMousePressed;
	int32 m_vOffStringLeft;
	int32 m_vOffStringTop;
	bool m_vOn;
	int32 m_vOnStringLeft;
	int32 m_vOnStringTop;
	bool m_vRightBorder;
	bool m_vTopBorder;
	bool m_vVisible;
};

#endif