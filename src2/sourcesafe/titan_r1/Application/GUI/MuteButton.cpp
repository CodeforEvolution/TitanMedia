#include "MuteButton.h"

#include "CommandIDs.h"
#include "EMCommandRepository.h"
#include "EMGTrack.h"
#include "EMGTrackRepository.h"
#include "EMKeyboard.h"
#include "EMKeyboardGlobals.h"
#include "EMMediaGlobals.h"
#include "EMView.h"
#include "GTrackInfo.h"
#include "SoloButton.h"

#define COLOR_ON_BACKGROUND EMColor(200, 200, 250, 255)
#define COLOR_ON_HIGHLIGHT EMColor(230, 230, 255, 255)
#define COLOR_ON_SHADOW EMColor(150, 150, 200, 255)
#define COLOR_OFF_BACKGROUND EMColor(150, 150, 200, 255)
#define COLOR_OFF_HIGHLIGHT EMColor(200, 200, 250, 255)
#define COLOR_OFF_SHADOW EMColor(100, 100, 150, 255)

MuteButton::MuteButton(EMView* p_opView, EMRect p_oFrame, string* p_opOnString, 
		string* p_opOffString, uint32 p_vCommandID, 
		void* p_opCommandParameterOne, 
		void* p_opCommandParameterTwo,
		void* p_opCommandParameterThree,
		bool p_vLeftBorder,
		bool p_vTopBorder , bool p_vRightBorder, 
		bool p_vBottomBorder, 
		EMColor p_oOnByUserColor,
		EMColor p_oOnBySoloColor,
		EMColor p_oOffColor, 
		EMBitmap* p_opOnBitmap, EMBitmap* p_opOffBitmap,
		EMBitmap* p_opDisabledOnBitmap,
		EMBitmap* p_opDisabledOffBitmap
		) :
EMSlimButton(p_opView, p_oFrame, p_opOnString, p_opOffString, p_vCommandID, 
		p_opCommandParameterOne, p_opCommandParameterTwo, p_opCommandParameterThree,
		p_vLeftBorder, p_vTopBorder, p_vRightBorder, p_vBottomBorder,
		p_oOnByUserColor, p_oOffColor, p_opOnBitmap, p_opOffBitmap,
		p_opDisabledOnBitmap, p_opDisabledOffBitmap),
m_oOnBySoloColor(p_oOnBySoloColor),
m_oOnByUserColor(p_oOnByUserColor),
m_vOnBySolo(false)
{
}

void MuteButton::ExecuteCommand()
{
	EMMuteState eMuteState;
	if(GetOnBySolo())
	{
eo << "Setting mute state for track with ID " << m_vMediaTrackID << " to EM_MUTE_ON_BY_SOLO" << ef;
		eMuteState = EM_MUTE_ON_BY_SOLO;
	}
	else if(GetOn())
	{
eo << "Setting mute state for track with ID " << m_vMediaTrackID << " to EM_MUTE_ON" << ef;
		eMuteState = EM_MUTE_ON;
	}
	else
	{ 
eo << "Setting mute state for track with ID " << m_vMediaTrackID << " to EM_MUTE_OFF" << ef;
		eMuteState = EM_MUTE_OFF;
	}
	EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_SET_TRACK_MUTE_STATE, &m_vMediaTrackID, &eMuteState);
}

bool MuteButton::GetOnBySolo()
{
	return m_vOnBySolo;
}

// Returns true if it should be invalidated
bool MuteButton::MouseDown(int32 p_vX, int32 p_vY, EMGTrack* p_opTrack)
{
	if(!m_vEnabled || !m_vVisible)
		return false;
	if(!m_oFrame.Contains(p_vX, p_vY))
		return false;

	if(m_vOn && m_eMode == EM_SLIM_BUTTON_RADIO)
		return false;
		
	if(m_eMode == EM_SLIM_BUTTON_DRAW_BLANK || m_eMode == EM_SLIM_BUTTON_DRAW_LABEL)
		return false;

	EMGTrackInfo* opTrackInfo;
	int vNumberOfSolos = 0;

	for(uint32 vIndex = 0; vIndex < EMGTrackRepository::Instance() -> GetNrTracks(); vIndex++)
	{
		opTrackInfo = EMGTrackRepository::Instance() -> GetTrack(vIndex) -> GetTrackInfo();
		if(dynamic_cast<GTrackInfo*>(opTrackInfo) -> GetSoloButton() -> GetOn())
			vNumberOfSolos++;
	}
	if(vNumberOfSolos > 0)
		return false;

	EMMuteState eMuteState;
	if(!GetOn())
	{
eo << "Setting mute state for track with ID " << m_vMediaTrackID << " to EM_MUTE_ON" << ef;
		eMuteState = EM_MUTE_ON;
	}
	else
	//if(!GetOn())
	{ 
eo << "Setting mute state for track with ID " << m_vMediaTrackID << " to EM_MUTE_OFF" << ef;
		eMuteState = EM_MUTE_OFF;
	}
	EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_SET_TRACK_MUTE_STATE, &m_vMediaTrackID, &eMuteState);

/*
	if(GetOn() && vNumberOfSolos > 0)
	{
		m_vOnBySolo = false;
		SetOn(false);
		p_opTrack -> GetTrackInfo() -> GetSoloButton() -> SetOn(true);
//		m_opView -> Invalidate(p_opTrack -> GetTrackInfo() -> GetSoloButton() -> Frame());
		// Do not forget to also invalidate the solo button as the caller
		ExecuteCommand();
		return true;
	}
	else if(!GetOn() && vNumberOfSolos > 0)
	{
		if(vNumberOfSolos == 1 && p_opTrack -> GetTrackInfo() -> GetSoloButton() -> GetOn())
			for(uint32 vIndex = 0; vIndex < EMGTrackRepository::Instance() -> GetNrTracks(); vIndex++)
			{
				opTrackInfo = EMGTrackRepository::Instance() -> GetTrack(vIndex) -> GetTrackInfo();
				if(opTrackInfo != p_opTrack -> GetTrackInfo())
					if(opTrackInfo -> GetMuteButton() -> GetOn())
					{
						opTrackInfo -> GetMuteButton() -> SetOnBySolo(false);
						m_opView -> Invalidate(opTrackInfo -> GetMuteButton() -> Frame());
					}
			}		
		m_vOnBySolo = false;
		SetOn(true);
		p_opTrack -> GetTrackInfo() -> GetSoloButton() -> SetOn(false);
//		m_opView -> Invalidate(p_opTrack -> GetTrackInfo() -> GetSoloButton() -> Frame());
		// Do not forget to also invalidate the solo button as the caller
		ExecuteCommand();
		return true;
	}
//	SetColors(m_oOnByUserColor);
*/
	return EMSlimButton::MouseDown(p_vX, p_vY);
}

bool MuteButton::SetOnBySolo(bool p_vOn)
{
/*
	if(GetOn() && !m_vOnBySolo) // This is if the track is muted and soloed and then solo button is pressed
	{
		ExecuteCommand();
		return false;
	}
	else if(GetOn() && m_vOnBySolo) // This is if the track is muted and not soloed and then solo button is pressed
	{
		ExecuteCommand();
		return false;
	}
*/
	if(m_vOnBySolo == p_vOn)
		return false;

	m_vOnBySolo = p_vOn;
	if(m_vOnBySolo)
	{
		SetColors(m_oOnBySoloColor);
		SetOn(true);
		ExecuteCommand();
		return true;
	}
	SetOn(false);
	SetColors(m_oOnByUserColor);

	EMMuteState eMuteState;
	if(GetOn())
	{
eo << "Setting mute state for track with ID " << m_vMediaTrackID << " to EM_MUTE_ON_BY_SOLO" << ef;
		eMuteState = EM_MUTE_ON_BY_SOLO;
	}
	else
	{
eo << "Setting mute state for track with ID " << m_vMediaTrackID << " to EM_MUTE_OFF" << ef;
		eMuteState = EM_MUTE_OFF;
	}
	EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_SET_TRACK_MUTE_STATE, &m_vMediaTrackID, &eMuteState);

//	ExecuteCommand();
	return false;
}

void MuteButton::SetMediaTrackID(int32 p_vMediaTrackID)
{
	m_vMediaTrackID = p_vMediaTrackID;
}



