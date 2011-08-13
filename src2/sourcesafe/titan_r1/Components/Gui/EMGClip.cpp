#include "EMGClip.h"

#include "CommandIDs.h" // FIXME: An application file in the FW, not good
#include "EMCommandRepository.h"
#include "EMGUIUtilities.h"
#include "EMGTrack.h"
#include "EMGTrackInfo.h"
#include "EMGClipContainer.h"
#include "EMGClipRepository.h"
#include "EMGTrackRepository.h"
#include "EMImageBufferRepository.h"

EMGClip::EMGClip(string p_oClipName, int64 p_vPosition, int64 p_vLength, EMGTrack* p_opTrack, int32 p_vMediaClipID, EMView* p_opView) :
//m_oName(p_oClipName),
m_opFirstInGroup(NULL),
m_opNextInGroup(NULL),
m_opTrack(p_opTrack),
m_opView(p_opView),
m_vHidden(false),
m_vLength(p_vLength),
m_vMediaClipID(p_vMediaClipID),
m_vMuted(false),
m_vPosition(p_vPosition),
m_vSelected(false)
{
	if(m_vMediaClipID != -1)
		SetLabel(const_cast<char*>(p_oClipName.c_str()), m_opView);

	if(p_opTrack != NULL)
		int32 vMediaTrackID = p_opTrack -> GetTrackInfo() -> GetMediaTrackID();
}

EMGClip::~EMGClip()
{
}

EMRect EMGClip::CalculateFrame(uint32 p_vTrackOffset, int64 p_vClipOffsetX)
{
	EMGTrack* opTrack = EMGTrackRepository::Instance() -> GetTrack(EMGTrackRepository::Instance() ->  GetTrackIndex(GetTrack()) + p_vTrackOffset);

	return opTrack -> GetClipContainer() -> CalculateClipFrame(this, p_vClipOffsetX);
}

void EMGClip::CalculateOkToDrop()
{
	EMGTrack* opTrack = EMGTrackRepository::Instance() -> GetTrack(EMGTrackRepository::Instance() -> GetTrackIndex(m_opTrack) + m_vTrackOffset);

	if(!(opTrack -> GetClipContainer() -> OkToDrop(this, GetPosition() + m_vPositionMovedOffset, GetLength())))
		m_vOkToDrop = false;
	else
		m_vOkToDrop = true;
}

EMRect EMGClip::Frame()
{
	return m_oFrame;
}

EMGClip* EMGClip::GetFirstInGroup()
{
	return m_opFirstInGroup;
}

string EMGClip::GetLabel()
{
	return m_oName;
}

int64 EMGClip::GetLength()
{
	return m_vLength;
}

int32 EMGClip::GetMediaClipID() const
{
	return m_vMediaClipID;
}

bool EMGClip::GetMuted()
{
	return m_vMuted;
}

EMGClip* EMGClip::GetNextInGroup()
{
	return m_opNextInGroup;
}

// Returns the position in microseconds
int64 EMGClip::GetPosition()
{
	return m_vPosition;
}

bool EMGClip::GetSelected() const
{
	return m_vSelected;
}

EMGTrack* EMGClip::GetTrack()
{
	return m_opTrack;
}
/*
bool EMGClip::IsHidden()
{
	return m_vHidden;
}
*/
// Calling this directly prevents EMGClipRepository from sorting its clips (probably bad)
bool EMGClip::MoveClip(int32 p_vTrackOffset, int64 p_vPositionOffset)
{
	EMGTrackRepository* opTrackRepository = EMGTrackRepository::Instance();
	EMGTrack* opNewTrack = opTrackRepository -> GetTrack(opTrackRepository -> GetTrackIndex(GetTrack()) + p_vTrackOffset);
	// Note: This does not trigger a EM_CLIP_FRAME_CHANGED message notify
	int64 vNewPosition = GetPosition() + p_vPositionOffset;
	vNewPosition = EMGUIUtilities::Instance() -> TruncateSample(vNewPosition);
	EMGClipRepository::Instance() -> SetClipPosition(this, vNewPosition, false);
	int32 vNewMediaTrackID = -1;

	if(m_opTrack != opNewTrack)
	{
		vNewMediaTrackID = opNewTrack -> GetTrackInfo() -> GetMediaTrackID();
		EMGClipRepository::Instance() -> SetClipTrack(this, opNewTrack);
	}
	EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_MOVE_CLIP, &m_vMediaClipID, &vNewPosition, &vNewMediaTrackID);
	//SetPosition(vNewPosition);
//int64 vNewMediaPosition = *(static_cast<int64*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_CLIP_POSITION, &m_vMediaClipID)));
//eo << "EMGClip::MoveClip: Intended position: " << (int32) vNewPosition << ", actual position: " << (int32) vNewMediaPosition << ef;
	//m_vPosition = vNewPosition;
	UpdateFrame();
	return true;
}

EMRect EMGClip::MovedFrame() const
{
	return m_oMovedFrame;
}

bool EMGClip::OkToDrop()
{
	return m_vOkToDrop;
}

void EMGClip::SetFirstInGroup(EMGClip* p_opClip)
{
	m_opFirstInGroup = p_opClip;
}

void EMGClip::SetFrame(EMRect p_oFrame)
{
	m_oFrame = p_oFrame;
}
/*
void EMGClip::SetMediaClipID(int32 p_vMediaClipID)
{
	m_vMediaClipID = p_vMediaClipID;
}
*/
/*
void EMGClip::SetHidden(bool p_vHidden)
{
	m_vHidden = p_vHidden;
}
*/
void EMGClip::SetLabel(char* p_oLabel, EMView* p_opView)
{
	m_oName = EMGUIUtilities::Instance() -> GetUniqueClipName(p_oLabel, this);
	EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_SET_CLIP_NAME, &m_vMediaClipID, p_oLabel);
}

void EMGClip::SetMovedFrame(EMRect p_oFrame, int64 p_vPositionMovedOffset, int32 p_vTrackOffset)
{
	m_oMovedFrame = p_oFrame;
	m_vPositionMovedOffset = p_vPositionMovedOffset;
	m_vTrackOffset = p_vTrackOffset;
}

void EMGClip::SetMuted(bool p_vMuted)
{
	if(p_vMuted == m_vMuted)
		return;
	m_vMuted = p_vMuted;
//	EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_TOGGLE_CLIP_MUTE, &m_vMediaClipID); FIXME: Call this when the media engine supports it
}

void EMGClip::SetNextInGroup(EMGClip* p_opClip)
{
	m_opNextInGroup = p_opClip;
}

void EMGClip::SetSelected(bool p_vSelected)
{
	if(p_vSelected == m_vSelected)
		return;
	m_vSelected = p_vSelected;
}

void EMGClip::SetTrack(EMGTrack* p_opTrack)
{
	m_opTrack = p_opTrack;
}
