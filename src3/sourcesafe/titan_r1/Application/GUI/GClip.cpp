#include "GClip.h"

#include "CommandIDs.h"
#include "EMCommandRepository.h"
#include "EMGClipRepository.h"
#include "EMGTrack.h"
#include "EMGUIUtilities.h"
#include "EMMediaGlobals.h"
#include "EMSettingIDs.h"
#include "EMSettingsRepository.h"
#include "EMView.h"
#include "SettingIDs.h"

#include <list>

GClip::GClip(string p_oClipName, int64 p_vPosition, int64 p_vLength, EMGTrack* p_opTrack, int32 p_vMediaClipID, EMView* p_opView) :
EMGClip(p_oClipName, p_vPosition, p_vLength, p_opTrack, p_vMediaClipID, p_opView),
m_opNextClip(NULL),
m_opPreviousClip(NULL),
m_vMouseIsDown(false),
m_vResizeLeft(false),
m_vResizeRight(false)
{
/*
	if(p_opTrack != NULL)
	{
		m_vClipMarkIn = *(static_cast<int64*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_CLIP_MARK_IN, &p_vMediaClipID)));
		m_vClipMarkOut = *(static_cast<int64*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_CLIP_MARK_OUT, &p_vMediaClipID)));
	}
*/
}
/*
int64 GClip::GetLength()
{
	// FIXME: Cache this value
	return *(static_cast<int64*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_CLIP_DURATION, &m_vMediaClipID)));
}
*/
bool GClip::MouseDown(EMView* p_opView, int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree)
{
	m_vMouseIsDown = true;
	m_vMouseDownX = p_vX;
	m_vMouseDownY = p_vY;

	ToolboxMode eToolboxMode = static_cast<ToolboxMode>(*(static_cast<int*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_TOOLBOX_MODE))));

	switch(eToolboxMode)
	{
	case TOOLBOX_ERASE:
		{
			break;
		}
	case TOOLBOX_RESIZE:
		{
			if(GetType() == EM_TYPE_MIDI)
			{
				m_vMouseIsDown = false;
				return false;
			}

			m_vTimeZoomScale = *(static_cast<int64*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_TIME_ZOOM_SCALE)));
			if(p_vX > m_oFrame.m_vLeft + (m_oFrame.GetWidth() / 2))
				m_vResizeRight = true;
			else
				m_vResizeLeft = true;
			m_vOriginalLength = GetLength();
			m_vOriginalPosition = GetPosition();			
			int32 vMediaClipID = GetMediaClipID();
			m_vOriginalMarkIn = *(static_cast<int64*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_CLIP_MARK_IN, &vMediaClipID)));
			m_vOriginalMarkOut = *(static_cast<int64*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_CLIP_MARK_OUT, &vMediaClipID)));
			m_oOriginalFrame = Frame();

			// Calculate previous and next clips
			list<EMGClip*>* opClips = m_opTrack -> GetClipContainer() -> GetClips();
			list<EMGClip*>::iterator oIterator = opClips -> begin();
			while(oIterator != opClips -> end())
			{
				if((*oIterator) == this)
					break;
				oIterator++;
			}
			if(oIterator == opClips -> begin())
				m_opPreviousClip = NULL;
			else
			{
				oIterator--;
				m_opPreviousClip = *oIterator;
				oIterator++;
			}
			oIterator++;
			if(oIterator == opClips -> end())
				m_opNextClip = NULL;
			else
				m_opNextClip = *oIterator;

			break;
		}
	default:
		break;
	}

	return true;
}

bool GClip::MouseMoved(EMView* p_opView, int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree)
{
	ToolboxMode eToolboxMode = static_cast<ToolboxMode>(*(static_cast<int*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_TOOLBOX_MODE))));

	switch(eToolboxMode)
	{
	case TOOLBOX_RESIZE:
		{
			if(GetType() == EM_TYPE_MIDI)
			{
				m_opView -> SetMouseCursor(EM_MOUSE_CURSOR_RESIZE_CLIP);
				return true;
			}

			if(!m_vMouseIsDown)
			{
				if(p_vX < m_oFrame.m_vLeft + (m_oFrame.GetWidth() / 2))
					m_opView -> SetMouseCursor(EM_MOUSE_CURSOR_RESIZE_CLIP_LEFT);
				else
					m_opView -> SetMouseCursor(EM_MOUSE_CURSOR_RESIZE_CLIP_RIGHT);
			
				break;
			}

			int32 vMediaClipID = GetMediaClipID();
			int64 vClipMarkIn = *(static_cast<int64*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_CLIP_MARK_IN, &vMediaClipID)));
			int64 vClipMarkOut = *(static_cast<int64*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_CLIP_MARK_OUT, &vMediaClipID)));
			int64 vNewLength;
			int64 vNewPosition;
			EMRect oOldFrame = Frame();
			EMRect oOldSelectedFrame = EMGClipRepository::Instance() -> SelectedFrame();
			//int64 vPosition = *(static_cast<int64*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_CURRENT_PROJECT_POSITION, NULL)));
			//int32 vSnapThreshold = *(static_cast<int32*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_SNAP_THRESHOLD)));
			if(p_vX < 0)
				p_vX = 0;

			if(m_vResizeRight)
			{
;//cout_commented_out_4_release << "m_vClipMarkOut: " << vClipMarkOut << endl;
				m_opView -> SetMouseCursor(EM_MOUSE_CURSOR_RESIZE_CLIP_RIGHT);

				int64 vMovedPosition = m_vOriginalPosition + m_vOriginalLength - 1 + static_cast<int64>(p_vX - m_vMouseDownX) * m_vTimeZoomScale;
				/*
				if((abs(vMovedPosition - (m_vOriginalPosition + m_vOriginalLength))) / m_vTimeZoomScale < vSnapThreshold)
					vMovedPosition = m_vOriginalPosition + m_vOriginalLength;
				else if(abs((vMovedPosition / m_vTimeZoomScale) - (vPosition / m_vTimeZoomScale)) < vSnapThreshold)
					vMovedPosition = vPosition;
				else
				*/
				EMGUIUtilities::Instance() -> GetSnapPosition(&vMovedPosition, true, false, this, true, true, m_vOriginalPosition + m_vOriginalLength - 1, true);

				if(GetType() == EM_TYPE_ANY_VIDEO) // Adjust to a whole frame
					vMovedPosition = EMGUIUtilities::Instance() -> TruncateFrame(vMovedPosition);//(static_cast<int64>((vMovedPosition * m_vFramerate) / EM_TICKS_PER_SECOND)) * static_cast<int64>(EM_TICKS_PER_SECOND / m_vFramerate);
				else if(GetType() == EM_TYPE_ANY_AUDIO) // Adjust to whole sample
					vMovedPosition = EMGUIUtilities::Instance() -> TruncateSample(vMovedPosition);

				// Video and transition clips cannot overlap
				if((GetType() == EM_TYPE_ANY_VIDEO || GetType() == EM_TYPE_TRANSITION) && m_opNextClip != NULL)
					if(vMovedPosition > m_opNextClip -> GetPosition())
						vMovedPosition = m_opNextClip -> GetPosition() - 1;

				vNewLength = vMovedPosition - m_vOriginalPosition;
				int64 vNewClipMarkOut = vClipMarkIn + vNewLength;
				if(vNewClipMarkOut < vClipMarkIn + 1000)
					vNewClipMarkOut = vClipMarkIn + 1000; // FIXME: should perhaps have other minimum length (one sample?)
				else if(vNewClipMarkOut > m_vFileLength)
					vNewClipMarkOut = m_vFileLength;

				int32 vClipID = GetMediaClipID();
eo << "Setting clip mark out: " << (int32) vNewClipMarkOut << " (m_vClipMarkIn: " << (int32) vClipMarkIn << ")" << ", vNewLength: " << (int32) vNewLength << ", vMovedPosition: " << (int32) vMovedPosition << ", m_vOriginalPosition: " << (int32) m_vOriginalPosition << ef;
				EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_SET_CLIP_MARK_OUT, &vClipID, &vNewClipMarkOut);
				SetFrame(EMRect(oOldFrame.m_vLeft, oOldFrame.m_vTop, oOldFrame.m_vLeft + ((vClipMarkOut - vClipMarkIn) / m_vTimeZoomScale), oOldFrame.m_vBottom));
			}

			else if(m_vResizeLeft)
			{
				m_opView -> SetMouseCursor(EM_MOUSE_CURSOR_RESIZE_CLIP_LEFT);

				int64 vMovedPosition = m_vOriginalPosition + static_cast<int64>(p_vX - m_vMouseDownX) * m_vTimeZoomScale;
				/*
				if((abs(vMovedPosition - m_vOriginalPosition)) / m_vTimeZoomScale < vSnapThreshold)
					vMovedPosition = m_vOriginalPosition;
				else if(abs((vMovedPosition / m_vTimeZoomScale) - (vPosition / m_vTimeZoomScale)) < vSnapThreshold)
					vMovedPosition = vPosition;
				else if(vMovedPosition / m_vTimeZoomScale < vSnapThreshold) // Snap to 0
					vMovedPosition = 0;
				else
				*/
				EMGUIUtilities::Instance() -> GetSnapPosition(&vMovedPosition, true, true, this, true, true, m_vOriginalPosition, true);

				if(GetType() == EM_TYPE_ANY_VIDEO) // Adjust to a whole frame
					vMovedPosition = EMGUIUtilities::Instance() -> TruncateFrame(vMovedPosition);//(static_cast<int64>((vMovedPosition * m_vFramerate) / EM_TICKS_PER_SECOND)) * static_cast<int64>(EM_TICKS_PER_SECOND / m_vFramerate);
				else if(GetType() == EM_TYPE_ANY_AUDIO) // Adjust to whole sample
					vMovedPosition = EMGUIUtilities::Instance() -> TruncateSample(vMovedPosition);

				// Video and transition clips cannot overlap
				if((GetType() == EM_TYPE_ANY_VIDEO || GetType() == EM_TYPE_TRANSITION) && m_opPreviousClip != NULL)
					if(vMovedPosition < m_opPreviousClip -> GetPosition() + m_opPreviousClip -> GetLength() - 1)
						vMovedPosition = m_opPreviousClip -> GetPosition() + m_opPreviousClip -> GetLength();

				vNewPosition = vMovedPosition;
				int64 vNewClipMarkIn = vClipMarkOut  - m_vOriginalLength + vNewPosition - m_vOriginalPosition;
				if(vNewClipMarkIn > vClipMarkOut - 1000)
				{
					vNewPosition -= vNewClipMarkIn - (vClipMarkOut - 1000);
					vNewClipMarkIn -= vNewClipMarkIn - (vClipMarkOut - 1000); // FIXME: should perhaps have other minimum length (one sample?)
				}
				else if(vNewClipMarkIn < 0)
				{
					vNewPosition -= vNewClipMarkIn;
					vNewClipMarkIn -= vNewClipMarkIn;
				}
//eo << "vNewClipMarkIn: " << vNewClipMarkIn << ", m_vClipMarkOut: " << m_vClipMarkOut << ", vNewPosition: " << vNewPosition << ef;

				int32 vClipID = GetMediaClipID();
eo << "Setting clip mark in: " << (int32) vNewClipMarkIn << " (m_vClipMarkOut: " << (int32) vClipMarkOut << ")" << ", vNewPosition: " << (int32) vNewPosition << ", vMovedPosition - m_vOriginalPosition: " << (int32) vMovedPosition - m_vOriginalPosition << ef;
				EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_SET_CLIP_MARK_IN, &vClipID, &vNewClipMarkIn);
				MoveClip(0, vNewPosition - GetPosition());
			}

			if(GetSelected())
			{
				EMGClipRepository::Instance() -> CalculateSelectedFrame();
				if(EMGClipRepository::Instance() -> SelectedFrame() != oOldSelectedFrame)
					p_opView -> Invalidate(oOldFrame | EMGClipRepository::Instance() -> SelectedFrame());
				else
					p_opView -> Invalidate(oOldFrame | Frame());
			}
			else
				p_opView -> Invalidate(oOldFrame | Frame());
		}
		break;
	case TOOLBOX_CUT_PASTE:
		m_opView -> SetMouseCursor(EM_MOUSE_CURSOR_CUT_PASTE);
		break;
	case TOOLBOX_RENAME:
		m_opView -> SetMouseCursor(EM_MOUSE_CURSOR_RENAME);
		break;
	case TOOLBOX_ERASE:
		m_opView -> SetMouseCursor(EM_MOUSE_CURSOR_DELETE);
		break;
	case TOOLBOX_DRAW:
		m_opView -> SetMouseCursor(EM_MOUSE_CURSOR_DRAW);
		break;
	default:
		break;
	}

	return true;
}

bool GClip::MouseUp(EMView* p_opView, int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree)
{
	if(!m_vMouseIsDown)
		return true;

	int32 vMediaClipID = GetMediaClipID();
	int64 vClipMarkIn = *(static_cast<int64*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_CLIP_MARK_IN, &vMediaClipID)));
	int64 vClipMarkOut = *(static_cast<int64*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_CLIP_MARK_OUT, &vMediaClipID)));

	// Update the cached length
	m_vLength = *(static_cast<int64*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_CLIP_DURATION, &m_vMediaClipID)));

	if(vClipMarkIn != m_vOriginalMarkIn || vClipMarkOut != m_vOriginalMarkOut)
	{
		list<int64> oParameterList;
		oParameterList.push_back(m_vOriginalPosition);
		oParameterList.push_back(m_vOriginalMarkIn);
		oParameterList.push_back(m_vOriginalMarkOut);
		EMCommandRepository::Instance() -> ExecuteCommand(UNDO_COMMAND_CHANGE_CLIP_RESIZE, this, &oParameterList, &m_oOriginalFrame);
	}

	m_vMouseIsDown = false;
	m_vResizeLeft = false;
	m_vResizeRight = false;

	return true;
}

EMGClip* GClip::SplitClip(int64 p_vPosition, int64 p_vTimeZoomScale)
{
	/*
	int64 vPosition = *(static_cast<int64*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_CURRENT_PROJECT_POSITION, NULL)));
	int32 vSnapThreshold = *(static_cast<int32*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_SNAP_THRESHOLD)));
	m_vTimeZoomScale = *(static_cast<int64*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_TIME_ZOOM_SCALE)));
	if(abs((p_vPosition / m_vTimeZoomScale) - (vPosition / m_vTimeZoomScale)) < vSnapThreshold)
		p_vPosition = vPosition;
	else
	*/
	EMGUIUtilities::Instance() -> GetSnapPosition(&p_vPosition, true, false, this, true, true);
	if(GetType() == EM_TYPE_ANY_VIDEO) // Adjust to a whole frame
		p_vPosition = EMGUIUtilities::Instance() -> TruncateFrame(p_vPosition);
	else
		p_vPosition = EMGUIUtilities::Instance() -> TruncateSample(p_vPosition);
	EMGClip* opNewClip = CloneClip(0, p_vPosition - GetPosition());

	int32 vOldClipMediaID = GetMediaClipID();
	int32 vNewClipMediaID = opNewClip -> GetMediaClipID();

	EMRect oOldClipFrame(Frame());
	oOldClipFrame.m_vRight = (p_vPosition - 1) / p_vTimeZoomScale;
	SetFrame(oOldClipFrame);
	EMRect oNewClipFrame(opNewClip -> Frame());
	oNewClipFrame.m_vRight = (GetPosition() + GetLength() - 1) / p_vTimeZoomScale;
	opNewClip -> SetFrame(oNewClipFrame);

	int64 vClipMarkIn = *(static_cast<int64*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_CLIP_MARK_IN, &vOldClipMediaID)));

	// The old clip needs a new mark out
	int64 vOldClipMarkOut = vClipMarkIn + p_vPosition - 1 - GetPosition();
	// The new clip needs a new mark in
	int64 vNewClipMarkIn = vClipMarkIn + p_vPosition - GetPosition();
//;//cout_commented_out_4_release << "GClip::SplitClip: vNewClipMarkIn: " << vNewClipMarkIn << endl;
	EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_SET_CLIP_MARK_OUT, &vOldClipMediaID, &vOldClipMarkOut);
	EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_SET_CLIP_MARK_IN, &vNewClipMediaID, &vNewClipMarkIn);

	UpdateFrame();
	opNewClip -> UpdateFrame();

	return opNewClip;
}

void GClip::UpdateFrame()
{
	m_vPosition = *(static_cast<int64*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_CLIP_POSITION, &m_vMediaClipID)));
	m_vLength = *(static_cast<int64*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_CLIP_DURATION, &m_vMediaClipID)));
	SetFrame(m_opTrack -> GetClipContainer() -> CalculateClipFrame(this));
}



