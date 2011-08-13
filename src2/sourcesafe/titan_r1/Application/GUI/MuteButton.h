/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __MUTE_BUTTON
#define __MUTE_BUTTON

#include "EMSlimButton.h"

class EMGTrack;

class MuteButton : public EMSlimButton
{
public:
	MuteButton(EMView* p_opView, EMRect p_oFrame, string* p_opOnString, 
				string* p_opOffString, uint32 p_vCommandID, 
				void* p_opCommandParameterOne = NULL, 
				void* p_opCommandParameterTwo = NULL,
				void* p_opCommandParameterThree = NULL,
				bool p_vLeftBorder = true,
				bool p_vTopBorder = true, bool p_vRightBorder = true, 
				bool p_vBottomBorder = true, 
				EMColor p_oOnByUserColor = EM_COLOR_BUTTON_DEFAULT_ON,
				EMColor p_oOnBySoloColor = EM_COLOR_BUTTON_DEFAULT_ON,
				EMColor p_oOffColor = EM_COLOR_BUTTON_DEFAULT_OFF, 
				EMBitmap* p_opOnBitmap = NULL, EMBitmap* p_opOffBitmap = NULL,
				EMBitmap* p_opDisabledOnBitmap = NULL,
				EMBitmap* p_opDisabledOffBitmap = NULL
				);
	void ExecuteCommand();
	bool GetOnBySolo();
	bool MouseDown(int32 p_vX, int32 p_vY, EMGTrack* p_opTrack);
	bool SetOnBySolo(bool p_vOn);
	void SetMediaTrackID(int32 p_vMediaTrackID);

private:
	EMColor m_oOnBySoloColor;
	EMColor m_oOnByUserColor;
	bool m_vOnBySolo;
	int32 m_vMediaTrackID;
};

#endif