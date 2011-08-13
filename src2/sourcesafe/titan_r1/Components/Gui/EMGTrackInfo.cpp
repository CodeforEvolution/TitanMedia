#include "EMGTrackInfo.h"

#include "EMGTrack.h"

EMGTrackInfo::EMGTrackInfo() :
m_oUpdateRect(EMRect()),
//m_vClipEdit(false),
m_vHidden(false),
m_vMediaTrackID(-1)
{
}
/*
EMGTrackInfo::~EMGTrackInfo()
{
}
*/
bool EMGTrackInfo::Contains(int32 p_vX, int32 p_vY)
{
	return m_oFrame.Contains(p_vX, p_vY);
}

void EMGTrackInfo::EditModeChanged(EMEditMode p_eEditMode)
{
}

EMRect EMGTrackInfo::Frame()
{
	return m_oFrame;
}

int32 EMGTrackInfo::GetMediaTrackID() const
{
	return m_vMediaTrackID;
}

EMGTrack* EMGTrackInfo::GetTrack()
{
	return m_opTrack;
}

EMRect EMGTrackInfo::GetUpdateRect()
{
	EMRect oRect = m_oUpdateRect;
	m_oUpdateRect = EMRect();
	return oRect;
}

bool EMGTrackInfo::Intersects(EMRect p_oFrame)
{
	return m_oFrame.Intersects(p_oFrame);
}
/*
bool EMGTrackInfo::IsClipEdit()
{
	return m_vClipEdit;
}
*/
/*
bool EMGTrackInfo::IsHidden()
{
	return m_vHidden;
}
*/

void EMGTrackInfo::OffsetFrame(int32 p_vX, int32 p_vY)
{
	m_oFrame.OffsetTo(p_vX, p_vY);
}

void EMGTrackInfo::SetFrame(EMRect p_oFrame)
{
	m_oFrame = p_oFrame;
}
/*
void EMGTrackInfo::SetHidden(bool p_vHidden)
{
	m_vHidden = p_vHidden;
//	GetTrack() -> GetClipContainer() -> HideClips(m_vHidden);
}
*/
void EMGTrackInfo::SetMediaTrackID(int32 p_vMediaTrackID)
{
	m_vMediaTrackID = p_vMediaTrackID;
}

void EMGTrackInfo::SetTrack(EMGTrack* p_opTrack)
{
	m_opTrack = p_opTrack;
}



