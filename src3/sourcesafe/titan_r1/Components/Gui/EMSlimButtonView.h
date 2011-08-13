/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_SLIM_BUTTON_VIEW
#define __EM_SLIM_BUTTON_VIEW

#include "EMGUIGlobals.h"
#include "EMListener.h"
//#include "EMSlimButton.h"
#include "EMRect.h"

#include <string>

class EMBitmap;
class EMSlimButton;
class EMView;

class EMSlimButtonView : public EMListener
{
public:
	EMSlimButtonView(EMView* p_opView, EMRect p_oFrame, 
				uint32 p_vResizingMode, 
				string* p_opOnString, 
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
				EMBitmap* p_opDisabledOffBitmap = NULL,
				const char* p_vpToolTipLabel = NULL
				);
	~EMSlimButtonView();
	void Draw(EMRect p_oClippingRect);
	EMSlimButton* GetButton();
	EMView* GetView();
	void InvalidateButton();
	bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage);
	void SetFrame(EMRect p_oFrame);
	
private:
	EMSlimButton* m_opButton;
	EMView* m_opView;
};

#endif