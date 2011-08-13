#include "TransitionGClip.h"

#include "CommandIDs.h"
#include "EMCommandRepository.h"
#include "EMGClipRepository.h"
#include "EMGTrack.h"
#include "EMGTrackRepository.h"
#include "EMGUIGlobals.h"
#include "EMGUIUtilities.h"
#include "EMMediaGlobals.h"
#include "EMSettingIDs.h"
#include "EMSettingsRepository.h"
#include "EMView.h"
#include "GUIGlobals.h"
#include "SettingIDs.h"

// Fixme: Fix the text placement calculations
const int32 NAME_X_OFFSET = 3;
const int32 NAME_Y_OFFSET = -2;

TransitionGClip::TransitionGClip(string p_oClipName, int64 p_vPosition, int64 p_vLength, EMGTrack* p_opTrack, int32 p_vMediaClipID, EMView* p_opView) :
GClip(p_oClipName, p_vPosition, p_vLength, p_opTrack, p_vMediaClipID, p_opView),
m_vIsInitialized(false),
m_vNamePositionUpdated(false)
{
	m_vFileLength = 100000000000000000;
}

TransitionGClip::~TransitionGClip()
{
}
/*
EMRect TransitionGClip::CalculateFrame(uint32 p_vTrackOffset, int64 p_vClipOffsetX)
{
//	p_vClipOffsetX = EMGUIUtilities::Instance() -> TruncateSample(p_vClipOffsetX);
//	return EMGClip::CalculateFrame(p_vTrackOffset, p_vClipOffsetX);
	return EMRect();
}
*/
void TransitionGClip::CalculateNamePosition(EMView* p_opView)
{
	/*
	int32 vAscent;
	int32 vDescent;
	p_opView -> CalculateStringHeight(m_oName.c_str(), vAscent, vDescent);
	m_vStringTop = vAscent + NAME_Y_OFFSET;
	m_vStringLeft = NAME_X_OFFSET;
	m_vNamePositionUpdated = true;
	*/

	int32 vAscent;
	int32 vDescent;
	if(p_opView -> LockWindow())
	{
		p_opView -> SetFont(EM_PLAIN);
		p_opView -> CalculateStringHeight(m_oName.c_str(), vAscent, vDescent);
		m_vStringTop = vAscent + NAME_Y_OFFSET;
		m_vStringLeft = NAME_X_OFFSET;
		int vLength = p_opView -> CalculateStringWidth(m_oName.c_str());
		m_oLabelBackground = EMRect(2, 2, 2 + vLength, 12);
		m_vNamePositionUpdated = true;

		p_opView -> UnlockWindow();
	}
}

EMRect TransitionGClip::CalculateFrame(uint32 p_vTrackOffset, int64 p_vClipOffsetX)
{
	p_vClipOffsetX = EMGUIUtilities::Instance() -> TruncateFrame(p_vClipOffsetX);
	return EMGClip::CalculateFrame(p_vTrackOffset, p_vClipOffsetX);
}

EMGClip* TransitionGClip::CloneClip(int32 p_vTrackOffset, int64 p_vPositionOffset)
{
	EMGTrackRepository* opTrackRepository = EMGTrackRepository::Instance();
	EMGClipRepository* opClipRepository = EMGClipRepository::Instance();
	EMGTrack* opNewTrack = opTrackRepository -> GetTrack(opTrackRepository -> GetTrackIndex(GetTrack()) + p_vTrackOffset);
	//p_vPositionOffset = EMGUIUtilities::Instance() -> TruncateFrame(p_vPositionOffset);
	int64 vNewPosition = GetPosition() + p_vPositionOffset;
	vNewPosition = EMGUIUtilities::Instance() -> TruncateFrame(vNewPosition);


	int32 vTargetMediaTrackID = opNewTrack -> GetTrackInfo() -> GetMediaTrackID();
	int32* vpMediaClipID = static_cast<int32*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_CLONE_CLIP, &m_vMediaClipID, &vNewPosition, &vTargetMediaTrackID));
	int32 vMediaClipID = *vpMediaClipID;
	EMGClip* opNewClip = opClipRepository -> CreateClip(GetLabel(), GetType(), vNewPosition, GetLength()/*, sNewClipInfo*/, opNewTrack, vMediaClipID, m_opView);

	return opNewClip;
}

EMGClip* TransitionGClip::CreateInstance(string p_oClipName, int64 p_vPosition, int64 p_vLength, EMGTrack* p_opTrack, int32 p_vMediaClipID, EMView* p_opView)
{
	return EM_new TransitionGClip(EMGUIUtilities::Instance() -> GetUniqueClipName(p_oClipName.c_str(), NULL), p_vPosition, p_vLength, p_opTrack, p_vMediaClipID, p_opView);
}

void TransitionGClip::DrawClip(EMRect p_oClippingRect, EMView* p_opView, bool p_vClipEditMode, int64 p_vZoomScale, bool p_vDrawContent)
{
	if(!p_oClippingRect.Intersects(m_oFrame))
		return;

	if(!m_vNamePositionUpdated)
		CalculateNamePosition(p_opView);

	p_opView -> ConstrainClippingRect(m_oFrame & p_oClippingRect);

	EMRect oDrawRect(m_oFrame & p_oClippingRect);

	if(p_vClipEditMode)
	{
		p_opView -> SetHighColor(EMColor(0, 0, 0, 255));
		p_opView -> StrokeLine(oDrawRect.m_vLeft, m_oFrame.m_vTop, oDrawRect.m_vRight, m_oFrame.m_vTop);
		p_opView -> StrokeLine(oDrawRect.m_vLeft, m_oFrame.m_vBottom, oDrawRect.m_vRight, m_oFrame.m_vBottom);
		if(m_oFrame.m_vLeft >= oDrawRect.m_vLeft)
			p_opView -> StrokeLine(m_oFrame.m_vLeft, m_oFrame.m_vTop, m_oFrame.m_vLeft, m_oFrame.m_vBottom);
		if(m_oFrame.m_vRight <= oDrawRect.m_vRight)
			p_opView -> StrokeLine(m_oFrame.m_vRight, m_oFrame.m_vTop, m_oFrame.m_vRight, m_oFrame.m_vBottom);

		p_opView -> SetHighColor(EM_COLOR_TEXT);
		p_opView -> SetLowColor(EMColor(200, 200, 200, 255));
		p_opView -> DrawString(m_oFrame.m_vLeft + m_vStringLeft, m_oFrame.m_vTop + m_vStringTop, m_oName.c_str());
	}

	else
	{
		if(m_vSelected)
		{
			p_opView -> SetHighColor(EM_COLOR_SELECTED_TRANSITION_VIEW_BACKGROUND);
			p_opView -> FillRect(m_oFrame & p_oClippingRect);

			p_opView -> SetHighColor(EM_COLOR_SELECTED_TRANSITION_VIEW_SHADOW);
			p_opView -> StrokeLine(oDrawRect.m_vLeft, m_oFrame.m_vBottom, oDrawRect.m_vRight, m_oFrame.m_vBottom);
			p_opView -> StrokeLine(oDrawRect.m_vLeft, m_oFrame.m_vBottom - 1, oDrawRect.m_vRight, m_oFrame.m_vBottom - 1);

			p_opView -> SetHighColor(EM_COLOR_SELECTED_TRANSITION_VIEW_HIGHLIGHT);
			p_opView -> StrokeLine(oDrawRect.m_vLeft, m_oFrame.m_vTop, oDrawRect.m_vRight, m_oFrame.m_vTop);
			p_opView -> StrokeLine(oDrawRect.m_vLeft, m_oFrame.m_vTop + 1, oDrawRect.m_vRight, m_oFrame.m_vTop + 1);

			if(m_oFrame.m_vLeft >= oDrawRect.m_vLeft)
				p_opView -> StrokeLine(m_oFrame.m_vLeft, m_oFrame.m_vTop, m_oFrame.m_vLeft, m_oFrame.m_vBottom);
			if(m_oFrame.m_vLeft + 1 >= oDrawRect.m_vLeft)
				p_opView -> StrokeLine(m_oFrame.m_vLeft + 1, m_oFrame.m_vTop, m_oFrame.m_vLeft + 1, m_oFrame.m_vBottom - 1);

			p_opView -> SetHighColor(EM_COLOR_SELECTED_TRANSITION_VIEW_SHADOW);
			if(m_oFrame.m_vRight <= oDrawRect.m_vRight)
				p_opView -> StrokeLine(m_oFrame.m_vRight, m_oFrame.m_vTop, m_oFrame.m_vRight, m_oFrame.m_vBottom);
			if(m_oFrame.m_vRight - 1 <= oDrawRect.m_vRight)
				p_opView -> StrokeLine(m_oFrame.m_vRight - 1, m_oFrame.m_vTop + 1, m_oFrame.m_vRight - 1, m_oFrame.m_vBottom);

			/*
			p_opView -> SetHighColor(EM_COLOR_TEXT);
			p_opView -> SetLowColor(EM_COLOR_SELECTED_TRANSITION_VIEW_BACKGROUND);
			p_opView -> DrawString(m_oFrame.m_vLeft + m_vStringLeft, m_oFrame.m_vTop + m_vStringTop, m_oName.c_str());
			*/
			EMRect oLabelBackground(m_oLabelBackground);
			oLabelBackground.OffsetBy(m_oFrame.m_vLeft, m_oFrame.m_vTop);
			if(oLabelBackground.Intersects(p_oClippingRect))
			{
				m_opView -> SetHighColor(EM_COLOR_SELECTED_TRANSITION_VIEW_SHADOW);
				p_opView -> FillRect(oLabelBackground);
				p_opView -> SetHighColor(EMColor(255, 255, 255, 255));
				//p_opView -> SetLowColor(EM_COLOR_SELECTED_TRANSITION_VIEW_BACKGROUND);
				p_opView -> DrawString(m_oFrame.m_vLeft + m_vStringLeft, m_oFrame.m_vTop + m_vStringTop, m_oName.c_str());
			}
		}
		else
		{
			p_opView -> SetHighColor(EM_COLOR_TRANSITION_VIEW_BACKGROUND);
			p_opView -> FillRect(m_oFrame & p_oClippingRect);

			p_opView -> SetHighColor(EM_COLOR_TRANSITION_VIEW_SHADOW);
			p_opView -> StrokeLine(oDrawRect.m_vLeft, m_oFrame.m_vBottom, oDrawRect.m_vRight, m_oFrame.m_vBottom);
			p_opView -> StrokeLine(oDrawRect.m_vLeft, m_oFrame.m_vBottom - 1, oDrawRect.m_vRight, m_oFrame.m_vBottom - 1);

			p_opView -> SetHighColor(EM_COLOR_TRANSITION_VIEW_HIGHLIGHT);
			p_opView -> StrokeLine(oDrawRect.m_vLeft, m_oFrame.m_vTop, oDrawRect.m_vRight, m_oFrame.m_vTop);
			p_opView -> StrokeLine(oDrawRect.m_vLeft, m_oFrame.m_vTop + 1, oDrawRect.m_vRight, m_oFrame.m_vTop + 1);

			if(m_oFrame.m_vLeft >= oDrawRect.m_vLeft)
				p_opView -> StrokeLine(m_oFrame.m_vLeft, m_oFrame.m_vTop, m_oFrame.m_vLeft, m_oFrame.m_vBottom);
			if(m_oFrame.m_vLeft + 1 >= oDrawRect.m_vLeft)
				p_opView -> StrokeLine(m_oFrame.m_vLeft + 1, m_oFrame.m_vTop, m_oFrame.m_vLeft + 1, m_oFrame.m_vBottom - 1);

			p_opView -> SetHighColor(EM_COLOR_TRANSITION_VIEW_SHADOW);
			if(m_oFrame.m_vRight <= oDrawRect.m_vRight)
				p_opView -> StrokeLine(m_oFrame.m_vRight, m_oFrame.m_vTop, m_oFrame.m_vRight, m_oFrame.m_vBottom);
			if(m_oFrame.m_vRight - 1 <= oDrawRect.m_vRight)
				p_opView -> StrokeLine(m_oFrame.m_vRight - 1, m_oFrame.m_vTop + 1, m_oFrame.m_vRight - 1, m_oFrame.m_vBottom);

			/*
			p_opView -> SetHighColor(EM_COLOR_TEXT);
			p_opView -> SetLowColor(EM_COLOR_TRANSITION_VIEW_BACKGROUND);
			p_opView -> DrawString(m_oFrame.m_vLeft + m_vStringLeft, m_oFrame.m_vTop + m_vStringTop, m_oName.c_str());
			*/
			EMRect oLabelBackground(m_oLabelBackground);
			oLabelBackground.OffsetBy(m_oFrame.m_vLeft, m_oFrame.m_vTop);
			if(oLabelBackground.Intersects(p_oClippingRect))
			{
				m_opView -> SetHighColor(EM_COLOR_SELECTED_TRANSITION_VIEW_SHADOW);
				p_opView -> FillRect(oLabelBackground);
				p_opView -> SetHighColor(EMColor(255, 255, 255, 255));
				//p_opView -> SetLowColor(EM_COLOR_SELECTED_TRANSITION_VIEW_BACKGROUND);
				p_opView -> DrawString(m_oFrame.m_vLeft + m_vStringLeft, m_oFrame.m_vTop + m_vStringTop, m_oName.c_str());
			}
		}	
		/*
		if(GetMuted())
		{
			p_opView -> SetHighColor(EM_COLOR_SELECTED_TRANSITION_VIEW_SHADOW);
			p_opView -> StrokeLine(m_oFrame.m_vLeft, m_oFrame.m_vTop, m_oFrame.m_vRight, m_oFrame.m_vBottom);
			p_opView -> StrokeLine(m_oFrame.m_vLeft, m_oFrame.m_vBottom, m_oFrame.m_vRight, m_oFrame.m_vTop);
		}
		*/
	}

	p_opView -> ConstrainClippingRect(EMRect());
}

void TransitionGClip::DrawMovedClip(EMRect p_oClippingRect, EMView* p_opView, int64 p_vZoomScale, bool p_vDrawContent)
{
	if(!p_oClippingRect.Intersects(m_oMovedFrame))
		return;

	p_opView -> ConstrainClippingRect(m_oMovedFrame & p_oClippingRect);

	EMRect oDrawRect(m_oMovedFrame & p_oClippingRect);

	p_opView -> SetDrawingMode(EM_OP_ALPHA);
	p_opView -> SetHighColor(EM_COLOR_MOVED_TRANSITION_VIEW_BACKGROUND);
	p_opView -> FillRect(m_oMovedFrame);
	
	p_opView -> SetHighColor(EM_COLOR_MOVED_TRANSITION_VIEW_SHADOW);
	p_opView -> StrokeLine(oDrawRect.m_vLeft, m_oMovedFrame.m_vBottom, oDrawRect.m_vRight, m_oMovedFrame.m_vBottom);
	if(m_oMovedFrame.m_vRight <= oDrawRect.m_vRight)
		p_opView -> StrokeLine(m_oMovedFrame.m_vRight, m_oMovedFrame.m_vTop, m_oMovedFrame.m_vRight, m_oMovedFrame.m_vBottom);
	p_opView -> SetHighColor(EM_COLOR_MOVED_TRANSITION_VIEW_HIGHLIGHT);
	p_opView -> StrokeLine(oDrawRect.m_vLeft, m_oMovedFrame.m_vTop, oDrawRect.m_vRight, m_oMovedFrame.m_vTop);
	if(m_oMovedFrame.m_vLeft >= oDrawRect.m_vLeft)
		p_opView -> StrokeLine(m_oMovedFrame.m_vLeft, m_oMovedFrame.m_vTop, m_oMovedFrame.m_vLeft, m_oMovedFrame.m_vBottom);

	p_opView -> SetHighColor(EM_COLOR_TEXT);
	p_opView -> SetLowColor(EM_COLOR_MOVED_TRANSITION_VIEW_BACKGROUND);
	p_opView -> DrawString(m_oMovedFrame.m_vLeft + m_vStringLeft, m_oMovedFrame.m_vTop + m_vStringTop, m_oName.c_str());
	p_opView -> SetDrawingMode(EM_OP_COPY);
	/*
	if(GetMuted())
	{
		p_opView -> SetHighColor(EM_COLOR_SELECTED_TRANSITION_VIEW_SHADOW);
		p_opView -> StrokeLine(m_oFrame.m_vLeft, m_oFrame.m_vTop, m_oFrame.m_vRight, m_oFrame.m_vBottom);
		p_opView -> StrokeLine(m_oFrame.m_vLeft, m_oFrame.m_vBottom, m_oFrame.m_vRight, m_oFrame.m_vTop);
	}
	*/
	p_opView -> ConstrainClippingRect(EMRect());
}

EMMediaType TransitionGClip::GetType()
{
	return EM_TYPE_TRANSITION;
}

bool TransitionGClip::MoveClip(int32 p_vTrackOffset, int64 p_vPositionOffset)
{
	//p_vPositionOffset = EMGUIUtilities::Instance() -> TruncateFrame(p_vPositionOffset);
	return EMGClip::MoveClip(p_vTrackOffset, p_vPositionOffset);
}

int64 TransitionGClip::GetLength()
{
	int32 vClipID = GetMediaClipID();
	return *(static_cast<int64*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_CLIP_DURATION, &vClipID)));
}
/*
void TransitionGClip::MouseDown(EMView* p_opView, int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree)
{
}

void TransitionGClip::MouseMoved(EMView* p_opView, int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree)
{
}

void TransitionGClip::MouseUp(EMView* p_opView, int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree)
{
}
*/

// FIXME: p_opView is no longer needed
void TransitionGClip::SetLabel(char* p_oLabel, EMView* p_opView)
{
	EMGClip::SetLabel(p_oLabel, m_opView);
	m_vNamePositionUpdated = false;
}

EMGClip* TransitionGClip::SplitClip(int64 p_vPosition, int64 p_vTimeZoomScale)
{
	EMDebugger("Transition clips cannot split");
	return NULL;
}

void TransitionGClip::UpdateContent()
{
}

