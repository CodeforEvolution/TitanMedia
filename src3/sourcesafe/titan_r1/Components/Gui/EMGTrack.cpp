#include "EMGTrack.h"

#include "EMGTrackRepository.h"
#include "EMGUIGlobals.h"
#include "EMView.h"
#include "EMSettingsRepository.h"
#include "EMSettingIDs.h"

EMGTrack::EMGTrack(EMGTrackInfo* p_opTrackInfo, EMGClipContainer* p_opClipContainer) :
m_eEditZoomScale(EM_HUGE),
m_eZoomScale(EM_NORMAL),
m_opClipContainer(p_opClipContainer),
m_opTrackInfo(p_opTrackInfo)//,
//m_vHidden(false)
{
}

EMGTrack::~EMGTrack()
{
	;//cout_commented_out_4_release << "EMGTrack::~EMGTrack()" << endl;
	delete m_opClipContainer;
	delete m_opTrackInfo;
}

EMGTrack* EMGTrack::CreateTrack(string p_oTrackName, EMGTrackInfo* p_opTrackInfo, EMGClipContainer* p_opClipContainer)
{
	;//cout_commented_out_4_release << "EMGTrack::CreateTrack" << endl;
	EMGTrack* opTrack = EM_new EMGTrack(p_opTrackInfo, p_opClipContainer);
	p_opTrackInfo -> SetTrack(opTrack);
	p_opClipContainer -> SetTrack(opTrack);
	p_opTrackInfo -> SetTrackName(p_oTrackName);
	return opTrack;
}

// Zoom out
bool EMGTrack::DecreaseZoomScale()
{
	if(*(static_cast<bool*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_EDIT_MODE))))
	{
		switch(m_eEditZoomScale)
		{
		case EM_TINY:
			return false;
		case EM_SMALL:
			m_eEditZoomScale = EM_TINY;
			EMGTrackRepository::Instance() -> TrackZoomHasChanged();
			return true;
		case EM_NORMAL:
			m_eEditZoomScale = EM_SMALL;
			EMGTrackRepository::Instance() -> TrackZoomHasChanged();
			return true;
		case EM_LARGE:
			m_eEditZoomScale = EM_NORMAL;
			EMGTrackRepository::Instance() -> TrackZoomHasChanged();
			return true;
		case EM_HUGE:
			m_eEditZoomScale = EM_LARGE;
			EMGTrackRepository::Instance() -> TrackZoomHasChanged();
			return true;
		case EM_COLOSSAL:
			m_eEditZoomScale = EM_HUGE;
			EMGTrackRepository::Instance() -> TrackZoomHasChanged();
			return true;
		case EM_LARGEST:
			m_eEditZoomScale = EM_COLOSSAL;
			EMGTrackRepository::Instance() -> TrackZoomHasChanged();
			return true;
		default:
			return false;
		}
	}
	else
	{
		switch(m_eZoomScale)
		{
		case EM_TINY:
			return false;
		case EM_SMALL:
			m_eZoomScale = EM_TINY;
			EMGTrackRepository::Instance() -> TrackZoomHasChanged();
			return true;
		case EM_NORMAL:
			m_eZoomScale = EM_SMALL;
			EMGTrackRepository::Instance() -> TrackZoomHasChanged();
			return true;
		case EM_LARGE:
			m_eZoomScale = EM_NORMAL;
			EMGTrackRepository::Instance() -> TrackZoomHasChanged();
			return true;
		case EM_HUGE:
			m_eZoomScale = EM_LARGE;
			EMGTrackRepository::Instance() -> TrackZoomHasChanged();
			return true;
		case EM_COLOSSAL:
			m_eZoomScale = EM_HUGE;
			EMGTrackRepository::Instance() -> TrackZoomHasChanged();
			return true;
		case EM_LARGEST:
			m_eZoomScale = EM_COLOSSAL;
			EMGTrackRepository::Instance() -> TrackZoomHasChanged();
			return true;
		default:
			return false;
		}
	}
}
		
/*
void EMGTrack::Draw(EMRect p_oClippingRect, EMView* p_opView)
{
	m_opClipContainer -> Draw(p_oClippingRect, p_opView);
	m_opTrackInfo -> Draw(p_oClippingRect, p_opView);
}
*/

EMGClipContainer* EMGTrack::GetClipContainer()
{
	return m_opClipContainer;
}

EMGTrackInfo* EMGTrack::GetTrackInfo()
{
	return m_opTrackInfo;
}

EMMediaType EMGTrack::GetType()
{
	return m_opTrackInfo -> GetType();
}

EMZoomScale EMGTrack::GetZoomScale()
{
	if(*(static_cast<bool*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_EDIT_MODE))) == EM_CLIP_EDIT_MODE)
		return m_eEditZoomScale;
	else
		return m_eZoomScale;
}

// Zoom out
bool EMGTrack::IncreaseZoomScale()
{
	if(*(static_cast<bool*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_EDIT_MODE))))
	{
		switch(m_eEditZoomScale)
		{
		case EM_TINY:
			m_eEditZoomScale = EM_SMALL;
			EMGTrackRepository::Instance() -> TrackZoomHasChanged();
			return true;
		case EM_SMALL:
			m_eEditZoomScale = EM_NORMAL;
			EMGTrackRepository::Instance() -> TrackZoomHasChanged();
			return true;
		case EM_NORMAL:
			m_eEditZoomScale = EM_LARGE;
			EMGTrackRepository::Instance() -> TrackZoomHasChanged();
			return true;
		case EM_LARGE:
			m_eEditZoomScale = EM_HUGE;
			EMGTrackRepository::Instance() -> TrackZoomHasChanged();
			return true;
		case EM_HUGE:
			m_eEditZoomScale = EM_COLOSSAL;
			EMGTrackRepository::Instance() -> TrackZoomHasChanged();
			return true;
		case EM_COLOSSAL:
			m_eEditZoomScale = EM_LARGEST;
			EMGTrackRepository::Instance() -> TrackZoomHasChanged();
			return true;
		case EM_LARGEST:
			return false;
		default:
			return false;
		}
	}
	else
	{
		switch(m_eZoomScale)
		{
		case EM_TINY:
			m_eZoomScale = EM_SMALL;
			EMGTrackRepository::Instance() -> TrackZoomHasChanged();
			return true;
		case EM_SMALL:
			m_eZoomScale = EM_NORMAL;
			EMGTrackRepository::Instance() -> TrackZoomHasChanged();
			return true;
		case EM_NORMAL:
			m_eZoomScale = EM_LARGE;
			EMGTrackRepository::Instance() -> TrackZoomHasChanged();
			return true;
		case EM_LARGE:
			m_eZoomScale = EM_HUGE;
			EMGTrackRepository::Instance() -> TrackZoomHasChanged();
			return true;
		case EM_HUGE:
			m_eZoomScale = EM_COLOSSAL;
			EMGTrackRepository::Instance() -> TrackZoomHasChanged();
			return true;
		case EM_COLOSSAL:
			m_eZoomScale = EM_LARGEST;
			EMGTrackRepository::Instance() -> TrackZoomHasChanged();
			return true;
		case EM_LARGEST:
			return false;
		default:
			return false;
		}
	}
}
		
void EMGTrack::SetZoomScale(EMZoomScale p_eZoomScale)
{
//	if(*(static_cast<bool*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_EDIT_MODE))))
	if(static_cast<EMEditMode>(*(static_cast<int*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_EDIT_MODE)))) == EM_CLIP_EDIT_MODE)
	{
		if(p_eZoomScale != m_eEditZoomScale)
		{
			m_eEditZoomScale = p_eZoomScale;
			EMGTrackRepository::Instance() -> TrackZoomHasChanged();
		}
	}
	else
	{
		if(p_eZoomScale != m_eZoomScale)
		{
			m_eZoomScale = p_eZoomScale;
			EMGTrackRepository::Instance() -> TrackZoomHasChanged();
		}
	}
//	m_opClipContainer -> ZoomScaleChanged(m_eZoomScale);
//	m_opTrackInfo -> ZoomScaleChanged(m_eZoomScale);
}

