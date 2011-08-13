/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __SOLO_BUTTON
#define __SOLO_BUTTON

#include "EMSlimButton.h"

class EMGTrack;

class SoloButton : public EMSlimButton
{
public:
	SoloButton(EMView* p_opView, EMRect p_oFrame, string* p_opOnString, 
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
	bool MouseDown(int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree, EMGTrack* p_opTrack);
};

#endif