#include "SoloButton.h"

#include "EMGTrack.h"
#include "EMGTrackRepository.h"
#include "EMKeyboard.h"
#include "EMKeyboardGlobals.h"
#include "EMView.h"
#include "GTrackInfo.h"
#include "MuteButton.h"

#define COLOR_ON_BACKGROUND EMColor(200, 200, 250, 255)
#define COLOR_ON_HIGHLIGHT EMColor(230, 230, 255, 255)
#define COLOR_ON_SHADOW EMColor(150, 150, 200, 255)
#define COLOR_OFF_BACKGROUND EMColor(150, 150, 200, 255)
#define COLOR_OFF_HIGHLIGHT EMColor(200, 200, 250, 255)
#define COLOR_OFF_SHADOW EMColor(100, 100, 150, 255)

SoloButton::SoloButton(EMView* p_opView, EMRect p_oFrame, string* p_opOnString, 
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
		p_opDisabledOnBitmap, p_opDisabledOffBitmap)
{
}

// Returns true if it should be invalidated
bool SoloButton::MouseDown(int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree, EMGTrack* p_opTrack)
{
	if(!m_vEnabled || !m_vVisible)
		return false;
	if(!m_oFrame.Contains(p_vX, p_vY))
		return false;

	if(m_vOn && m_eMode == EM_SLIM_BUTTON_RADIO)
		return false;
		
	if(m_eMode == EM_SLIM_BUTTON_DRAW_BLANK || m_eMode == EM_SLIM_BUTTON_DRAW_LABEL)
		return false;

	GTrackInfo* opTrackInfo;
	int vNumberOfSolos = 0;

	if(p_vButtonTwo)
	{
		for(uint32 vIndex = 0; vIndex < EMGTrackRepository::Instance() -> GetNrTracks(); vIndex++)
		{
			opTrackInfo = dynamic_cast<GTrackInfo*>(EMGTrackRepository::Instance() -> GetTrack(vIndex) -> GetTrackInfo());
			if(opTrackInfo -> GetMuteButton() -> GetOn())
			{
				if(opTrackInfo -> GetMuteButton() -> GetOnBySolo())
				{
					opTrackInfo -> GetMuteButton() -> SetOnBySolo(false);
					m_opView -> Invalidate(opTrackInfo -> GetMuteButton() -> Frame());
				}
				else
					opTrackInfo -> GetMuteButton() -> ExecuteCommand();
			}
			if(opTrackInfo -> GetSoloButton() -> GetOn())
			{
				opTrackInfo -> GetSoloButton() -> SetOn(false);
				m_opView -> Invalidate(opTrackInfo -> GetSoloButton() -> Frame());
			}
		}
	}

	else if(!m_vOn)
	{
		for(uint32 vIndex = 0; vIndex < EMGTrackRepository::Instance() -> GetNrTracks(); vIndex++)
		{
			opTrackInfo = dynamic_cast<GTrackInfo*>(EMGTrackRepository::Instance() -> GetTrack(vIndex) -> GetTrackInfo());
			if(opTrackInfo -> GetSoloButton() -> GetOn())
				vNumberOfSolos++;
		}
		if(vNumberOfSolos == 0)
		{
			for(uint32 vIndex = 0; vIndex < EMGTrackRepository::Instance() -> GetNrTracks(); vIndex++)
			{
				opTrackInfo = dynamic_cast<GTrackInfo*>(EMGTrackRepository::Instance() -> GetTrack(vIndex) -> GetTrackInfo());
				if(opTrackInfo != p_opTrack -> GetTrackInfo())
					if(!(opTrackInfo -> GetSoloButton() -> GetOn()) && !(opTrackInfo -> GetMuteButton() -> GetOn()))
					{
						opTrackInfo -> GetMuteButton() -> SetOnBySolo(true);
						m_opView -> Invalidate(opTrackInfo -> GetMuteButton() -> Frame());
					}
			}
		}
		if(dynamic_cast<GTrackInfo*>(p_opTrack -> GetTrackInfo()) -> GetMuteButton() -> GetOnBySolo())
		{
			dynamic_cast<GTrackInfo*>(p_opTrack -> GetTrackInfo()) -> GetMuteButton() -> SetOnBySolo(false);
			m_opView -> Invalidate(dynamic_cast<GTrackInfo*>(p_opTrack -> GetTrackInfo()) -> GetMuteButton() -> Frame());
		}
		else if(dynamic_cast<GTrackInfo*>(p_opTrack -> GetTrackInfo()) -> GetMuteButton() -> GetOn())
			dynamic_cast<GTrackInfo*>(p_opTrack -> GetTrackInfo()) -> GetMuteButton() -> ExecuteCommand();
		SetOn(true);
	}
	
	else if(m_vOn)
	{
		for(uint32 vIndex = 0; vIndex < EMGTrackRepository::Instance() -> GetNrTracks(); vIndex++)
		{
			opTrackInfo = dynamic_cast<GTrackInfo*>(EMGTrackRepository::Instance() -> GetTrack(vIndex) -> GetTrackInfo());
			if(opTrackInfo -> GetSoloButton() -> GetOn())
				vNumberOfSolos++;
		}
		if(vNumberOfSolos == 1) // Only this solo button left on
		{
			for(uint32 vIndex = 0; vIndex < EMGTrackRepository::Instance() -> GetNrTracks(); vIndex++)
			{
				opTrackInfo = dynamic_cast<GTrackInfo*>(EMGTrackRepository::Instance() -> GetTrack(vIndex) -> GetTrackInfo());
				{
					opTrackInfo -> GetMuteButton() -> SetOnBySolo(false);
					m_opView -> Invalidate(opTrackInfo -> GetMuteButton() -> Frame());
				}
			}
			if(dynamic_cast<GTrackInfo*>(p_opTrack -> GetTrackInfo()) -> GetMuteButton() -> GetOn())
				dynamic_cast<GTrackInfo*>(p_opTrack -> GetTrackInfo()) -> GetMuteButton() -> ExecuteCommand();
		}
		else // There are more solo buttons than this left on
		{
			if(!(dynamic_cast<GTrackInfo*>(p_opTrack -> GetTrackInfo()) -> GetMuteButton() -> GetOn()))
				dynamic_cast<GTrackInfo*>(p_opTrack -> GetTrackInfo()) -> GetMuteButton() -> SetOnBySolo(true);
			else
				dynamic_cast<GTrackInfo*>(p_opTrack -> GetTrackInfo()) -> GetMuteButton() -> ExecuteCommand();
		}
		SetOn(false);
	}
	
	return true;
}

