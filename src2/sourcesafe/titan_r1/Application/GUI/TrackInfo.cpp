#include "TrackInfo.h"

#include "CommandIDs.h"
#include "EMBitmap.h"
#include "EMCommandRepository.h"
#include "EMFactory.h"
#include "EMGClipRepository.h"
#include "EMGTrack.h"
#include "EMGTrackRepository.h"
#include "EMGUIGlobals.h"
#include "EMKeyboard.h" // Debug
#include "EMKeyboardGlobals.h" // Debug
#include "EMListenerRepository.h"
#include "EMMenuBar.h"
#include "EMMessages.h"
#include "EMResourceRepository.h"
#include "EMScrollbar.h"
#include "EMSettingIDs.h"
#include "EMSettingsRepository.h"
#include "EMThreadRepository.h"
#include "EMView.h"
#include "GTrackInfo.h"
#include "GUIGlobals.h"
#include "Messages.h"
#include "ResourceIDs.h"
#include "SettingIDs.h"
#include "TrackInfoButtonBar.h"
#include "VideoGTrackInfo.h"

TrackInfo::TrackInfo() :
m_oEmptySpace(0, 0, MAX_VIEW_PIXEL_LIMIT, MAX_VIEW_PIXEL_LIMIT),
m_opMouseDownTrack(NULL),
m_opView(EMFactory::Instance() -> CreateView(EMRect(), EM_FOLLOW_TOP_BOTTOM, false)),
m_vDrawedOnButtonBar(false)
//m_vIsDirty(false)
{
	// Initializing the signal meter value array + matrix
	int32 vIndexOne;
	int vIndexTwo;
	for(vIndexOne = 0; vIndexOne < 100; vIndexOne++) // FIXME: Replace the magic values with constants
	{
		m_vMIDITrackSignalValues[vIndexOne] = 0;

		for(vIndexTwo = 0; vIndexTwo < 5; vIndexTwo++)
			m_vAudioTrackSignalValues[vIndexOne][vIndexTwo] = 0;
	}
}

TrackInfo::~TrackInfo()
{
	delete m_opView;
}

void TrackInfo::CalculateAndSetTrackInfos()
{
//eo << "CalculateAndSetTrackInfos:" << ef;
//eo << "Audio: " << ef;
	m_opView -> AssertWindowLocked();
	m_oAudioTrackInfos.clear();
	list<int32>* opMediaIDs = static_cast<list<int32>* >(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_AUDIO_TRACK_IDS, NULL, NULL, NULL));
	EMGTrackInfo* opTrackInfo;
	list<int32>::iterator oIterator = opMediaIDs -> begin();
	while(oIterator != opMediaIDs -> end())
	{
//eo << " Looking for match for media track id: " << *oIterator << ef;
		for(uint32 vIndex = 0; vIndex < EMGTrackRepository::Instance() -> GetNrTracks(); vIndex++)
		{
			opTrackInfo = EMGTrackRepository::Instance() -> GetTrack(vIndex) -> GetTrackInfo();
			if(opTrackInfo -> GetMediaTrackID() == *oIterator)
			{
//eo << "  Found match with track with index " << vIndex << ef;
				m_oAudioTrackInfos.push_back(opTrackInfo);
				break;
			}
		}
		oIterator++;
	}

//eo << "Video: " << ef;
	m_oMIDITrackInfos.clear();
	opMediaIDs = static_cast<list<int32>* >(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_MIDI_TRACK_IDS, NULL, NULL, NULL));
	oIterator = opMediaIDs -> begin();
	while(oIterator != opMediaIDs -> end())
	{
//eo << " Looking for match for media track id: " << *oIterator << ef;
		for(uint32 vIndex = 0; vIndex < EMGTrackRepository::Instance() -> GetNrTracks(); vIndex++)
		{
			opTrackInfo = EMGTrackRepository::Instance() -> GetTrack(vIndex) -> GetTrackInfo();
			if(opTrackInfo -> GetMediaTrackID() == *oIterator)
			{
//eo << "  Found match with track with index " << vIndex << ef;
				m_oMIDITrackInfos.push_back(opTrackInfo);
				break;
			}
		}
		oIterator++;
	}
}

EMRect TrackInfo::CalculateTrackInfoFrame(EMGTrack* p_opTrack)
{
	int32 vTop = 0;
	EMGTrack* opTrack;
	int32 vTrackIndex = EMGTrackRepository::Instance() -> GetTrackIndex(p_opTrack);
	for(int32 vIndex = 0; vIndex < vTrackIndex; vIndex++)
	{
		opTrack = EMGTrackRepository::Instance() -> GetTrack(vIndex);
//		opTrack -> GetTrackInfo() -> CalculateAndSetHidden(m_vClipEdit);
		if(!(opTrack -> GetTrackInfo() -> IsHidden(m_eEditMode == EM_CLIP_EDIT_MODE)))
			vTop = (opTrack -> GetTrackInfo() -> Frame()).m_vBottom + 1;
	}
	EMRect oFrame(0, vTop, m_opView -> Frame().GetWidth(), -1);

	switch(p_opTrack -> GetZoomScale())
	{
	case EM_TINY:
		oFrame.m_vBottom = oFrame.m_vTop + EM_TINY_TRACK_HEIGHT - 1;
		break;
	case EM_SMALL:
		oFrame.m_vBottom = oFrame.m_vTop + EM_SMALL_TRACK_HEIGHT - 1;
		break;
	case EM_NORMAL:
		oFrame.m_vBottom = oFrame.m_vTop + EM_NORMAL_TRACK_HEIGHT - 1;
		break;
	case EM_LARGE:
		oFrame.m_vBottom = oFrame.m_vTop + EM_LARGE_TRACK_HEIGHT - 1;
		break;
	case EM_HUGE:
		oFrame.m_vBottom = oFrame.m_vTop + EM_HUGE_TRACK_HEIGHT - 1;
		break;
	case EM_COLOSSAL:
		oFrame.m_vBottom = oFrame.m_vTop + EM_COLOSSAL_TRACK_HEIGHT - 1;
		break;
	case EM_LARGEST:
		oFrame.m_vBottom = oFrame.m_vTop + EM_LARGEST_TRACK_HEIGHT - 1;
		break;
	default:
		EMDebugger("Illegal track height detected");
		break;
	}
	p_opTrack -> GetTrackInfo() -> SetFrame(oFrame);
	return oFrame;
}

// Calculates the frames for p_opTrack, all following tracks and the
// remaining empty space, then invalidates that region and sends a
// message
void TrackInfo::CalculateTrackInfoFramesAndUpdate(EMGTrack* p_opTrack)
{
	m_opView -> AssertWindowLocked();
//	if(m_opView -> LockWindow())
	{
		uint32 vIndex;
		if(EMGTrackRepository::Instance() -> GetNrTracks() == 0 || p_opTrack == NULL)
			vIndex = 0;
		else
			vIndex = EMGTrackRepository::Instance() -> GetTrackIndex(p_opTrack);
		for(; vIndex < EMGTrackRepository::Instance() -> GetNrTracks(); vIndex++)
			CalculateTrackInfoFrame(EMGTrackRepository::Instance() -> GetTrack(vIndex));
		if(EMGTrackRepository::Instance() -> GetNrTracks() == 0)
			m_oEmptySpace = EMRect(0, 0, MAX_VIEW_PIXEL_LIMIT, MAX_VIEW_PIXEL_LIMIT);//GetView() -> Bounds();
		else
		{
			int32 vEmptySpaceTop = 0;
			EMGTrack* opTrack;
			for(uint32 vIndex = 0; vIndex < EMGTrackRepository::Instance() -> GetNrTracks(); vIndex++)
			{
				opTrack = EMGTrackRepository::Instance() -> GetTrack(vIndex);
				if(!(opTrack -> GetTrackInfo() -> IsHidden(m_eEditMode == EM_CLIP_EDIT_MODE)))
					vEmptySpaceTop = (opTrack -> GetTrackInfo() -> Frame()).m_vBottom + 1;
			}
			m_oEmptySpace = EMRect(0, 
								vEmptySpaceTop,//(EMGTrackRepository::Instance() -> GetTrack(vIndex - 1) -> GetTrackInfo() -> Frame()).m_vBottom + 1, 
								MAX_VIEW_PIXEL_LIMIT, 
								MAX_VIEW_PIXEL_LIMIT);
//			m_opView -> Invalidate(EMRect(0, (p_opTrack -> GetTrackInfo() -> Frame()).m_vTop, (m_opView -> Frame()).m_vRight,
//								(EMGTrackRepository::Instance() -> GetTrack(vIndex - 1) -> GetTrackInfo() -> Frame()).m_vBottom));
//;//cout_commented_out_4_release << "TrackInfo::CalculateTrackInfoFrameAndUpdate: ";
//EMRect(0, (p_opTrack -> GetTrackInfo() -> Frame()).m_vTop, (m_opView -> Frame()).m_vRight,
//(EMGTrackRepository::Instance() -> GetTrack(vIndex - 1) -> GetTrackInfo() -> Frame()).m_vBottom).Print();
		}

		for(vIndex = 0; vIndex < EMGTrackRepository::Instance() -> GetNrTracks(); vIndex++)
			EMGTrackRepository::Instance() -> GetTrack(vIndex) -> GetTrackInfo() -> LayoutComponents();

		m_opView -> Invalidate(GetView() -> Bounds());
	}
	// Does not send a EM_TRACK_BOUNDS_CHANGED message, 
	// that is carried out by TrackView
;//cout_commented_out_4_release << "TrackInfo::CalculateTrackInfoFramesAndUpdate: empty space: ";
//m_oEmptySpace.Print();
}

void TrackInfo::ChangeEditMode()
{
	CalculateTrackInfoFramesAndUpdate(NULL);

	EMGTrackInfo* opTrackInfo;
	for(uint32 vIndex = 0; vIndex < EMGTrackRepository::Instance() -> GetNrTracks(); vIndex++)
	{
		opTrackInfo = EMGTrackRepository::Instance() -> GetTrack(vIndex) -> GetTrackInfo();
		opTrackInfo -> EditModeChanged(m_eEditMode);
	}
}

bool TrackInfo::ClearData()
{
	m_oAudioTrackInfos.clear();
	m_oMIDITrackInfos.clear();
	m_oEmptySpace = m_opView -> Bounds();
	return true;
}

void TrackInfo::Draw(EMRect p_oClippingRect)
{
	/*
	// This is a debug thingie to detect redraw problems
	uint32 vModifiers = EMKeyboard::Instance() -> GetModifiers();
	if((vModifiers & EM_SCROLL_LOCK) == EM_SCROLL_LOCK)
	{
		m_opView -> SetHighColor(EMColor(0, 255, 255, 255));
		m_opView -> FillRect(p_oClippingRect);
		return;
	}
	*/
	// End of debug thingie

//	;//cout_commented_out_4_release << "TrackInfo::Draw" << endl;
	EMGTrackInfo* opTrackInfo;
	for(uint32 vIndex = 0; vIndex < EMGTrackRepository::Instance() -> GetNrTracks(); vIndex++)
	{
		opTrackInfo = EMGTrackRepository::Instance() -> GetTrack(vIndex) -> GetTrackInfo();
		if(!(opTrackInfo -> IsHidden(m_eEditMode == EM_CLIP_EDIT_MODE)))
			opTrackInfo -> Draw(p_oClippingRect, m_opView);
	}
	if(m_oEmptySpace.Intersects(p_oClippingRect))
	{
		m_opView -> SetHighColor(EM_COLOR_VIEW_BACKGROUND);
		m_opView -> FillRect(m_oEmptySpace & p_oClippingRect);
//eo << "Empty space: ";
//m_oEmptySpace.Print();
//m_opView -> SetHighColor(EMColor(0, 255, 0, 255));
//EMRect oRect = m_oEmptySpace & m_opView -> Bounds();
//oRect.Print();
/*
if(oRect.IsValid())
{
(m_opView -> Bounds()).Print();
m_opView -> StrokeRect(oRect);
m_opView -> StrokeLine(oRect.m_vLeft, oRect.m_vTop, oRect.m_vRight, oRect.m_vBottom);
m_opView -> StrokeLine(oRect.m_vRight, oRect.m_vTop, oRect.m_vLeft, oRect.m_vBottom);
}
*/
	}
}

// Should be called whenever it is suspected that the enabled state of menus
// in response to changes in TrackInfo has happened
void TrackInfo::EnableMenuItems()
{
	int vArmedTracks(0);
	int vTracks;
	int vSelectedTracks(0);
	int vSelectedVideoTracks(0);

	vector<EMGTrack*>* opTracks = EMGTrackRepository::Instance() -> GetTracks();
	vTracks = opTracks -> size();
	for(int vIndex = 0; vIndex < opTracks -> size(); vIndex++)
	{
		if((*opTracks)[vIndex] -> GetTrackInfo() -> IsSelected())
		{
			vSelectedTracks++;
			if((*opTracks)[vIndex] -> GetTrackInfo() -> GetType() == EM_TYPE_ANY_VIDEO)
				vSelectedVideoTracks++;
		}
		if(dynamic_cast<GTrackInfo*>((*opTracks)[vIndex] -> GetTrackInfo()) -> GetArmButton() -> GetOn())
			vArmedTracks++;
	}

	EMMenuBar::Instance() -> SetEnabled(RES_TM_TRANSPORT, RES_TM_RECORD, vArmedTracks > 0);
	if(*(static_cast<bool*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_VIDEO_ENABLED))))
		EMMenuBar::Instance() -> SetEnabled(RES_TM_TRACK, RES_TM_CREATE_TRANSITION_TRACK, vSelectedVideoTracks == 1 && vSelectedTracks == 1);
	EMMenuBar::Instance() -> SetEnabled(RES_TM_TRACK, RES_TM_DELETE_TRACK, vSelectedTracks > 0);
	EMMenuBar::Instance() -> SetEnabled(RES_TM_TRACK, RES_TM_SELECT_ALL_TRACKS, vTracks > 0);
	EMMenuBar::Instance() -> SetEnabled(RES_TM_TRACK, RES_TM_SELECT_NO_TRACKS, vTracks > 0);
	EMMenuBar::Instance() -> SetEnabled(RES_TM_TRACK, RES_TM_INVERT_TRACK_SELECTION, vTracks > 0);
}

EMView* TrackInfo::GetView()
{
	return m_opView;
}

void TrackInfo::Init()
{
	m_opView -> AddListener(this);
	EMSettingsRepository::Instance() -> AddListener(this);
	EMGClipRepository::Instance() -> AddListener(this);
	m_eEditMode = static_cast<EMEditMode>(*(static_cast<int*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_EDIT_MODE))));
	m_vSignal = TrackInfoButtonBar::Instance() -> GetSignalOn();
	m_vExpertMode = *(static_cast<bool*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_EXPERT_MODE)));
	SetUserMode();
	EMThreadRepository::Instance() -> GetThread(GUI_SERVICE_THREAD_NAME) -> AddListener(this);
//	EnableMenuItems();
}
/*
bool TrackInfo::IsDirty()
{
	return m_vIsDirty;
}
*/
void TrackInfo::LoadComplete()
{
}

bool TrackInfo::MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage)
{
	int32 vX;
	int32 vY;
	bool vButtonOne;
	bool vButtonTwo;
	bool vButtonThree;
	EMGTrackInfo* opInfo;
	EMEditMode eEditMode;
	bool vExpertMode;
	
	switch(p_vMessage)
	{
	case EM_DRAW:
		Draw(m_opView -> GetClippingRect());
		return true;
	case BUTTONBAR_CHANGED:
		m_vSignal = TrackInfoButtonBar::Instance() -> GetSignalOn();
		CalculateTrackInfoFramesAndUpdate(EMGTrackRepository::Instance() -> GetTrack(0));
		return true;
	case TRACK_FILTERS_CHANGED:
		{
			vector<EMGTrack*>* opTracks = EMGTrackRepository::Instance() -> GetTracks();

			for(uint32 vIndex = 0; vIndex < EMGTrackRepository::Instance() -> GetNrTracks(); vIndex++)
				if((*opTracks)[vIndex] -> GetTrackInfo() -> IsHidden(m_eEditMode == EM_CLIP_EDIT_MODE))
				{
					if(dynamic_cast<GTrackInfo*>((*opTracks)[vIndex] -> GetTrackInfo()) -> GetArmButton() -> GetOn())
					{
						dynamic_cast<GTrackInfo*>((*opTracks)[vIndex] -> GetTrackInfo()) -> GetArmButton() -> SetOn(false);
						dynamic_cast<GTrackInfo*>((*opTracks)[vIndex] -> GetTrackInfo()) -> GetArmButton() -> ExecuteCommand();
					}
					(*opTracks)[vIndex] -> GetTrackInfo() -> SetSelected(false);
				}
			CalculateTrackInfoFramesAndUpdate(EMGTrackRepository::Instance() -> GetTrack(0));
			EnableMenuItems();
			return true;
		}
	case EM_TRACK_REPOSITORY_UPDATED:
		{
			for(uint32 vIndex = 0; vIndex < EMGTrackRepository::Instance() -> GetNrTracks(); vIndex++)
				EMGTrackRepository::Instance() -> GetTrack(vIndex) -> GetTrackInfo() -> AddListener(this);
			CalculateTrackInfoFramesAndUpdate(EMGTrackRepository::Instance() -> GetTrack(0));
			CalculateAndSetTrackInfos();
			SetUserMode();
			EnableMenuItems();
			return true;
		}
// Is this one needed?
//	case EM_CLIP_REPOSITORY_UPDATED:
//		CalculateAndSetTrackInfos();
//		return true;
	case EM_MOUSE_DOWN:
		m_opView -> GetMouse(vX, vY, vButtonOne, vButtonTwo, vButtonThree);
//eo << "TrackInfo: EM_MOUSE_DOWN: " << vButtonOne << ", " << vButtonTwo << ", " << vButtonThree << ef;
		MouseDown(vX, vY, vButtonOne, vButtonTwo, vButtonThree);
		EnableMenuItems();
		return true;
	case EM_MOUSE_MOVED:
		m_opView -> GetMouse(vX, vY, vButtonOne, vButtonTwo, vButtonThree);
		MouseMoved(vX, vY, vButtonOne, vButtonTwo, vButtonThree);
		return true;
	case EM_MOUSE_UP:
		m_opView -> GetMouse(vX, vY, vButtonOne, vButtonTwo, vButtonThree);
		MouseUp(vX, vY);
		return true;
	case EM_DOUBLE_CLICK:
		m_opView -> GetMouse(vX, vY, vButtonOne, vButtonTwo, vButtonThree);
		MouseDoubleClick(vX, vY);
		return true;
	case EM_UPDATE_NEEDED:
		if((opInfo = dynamic_cast<EMGTrackInfo*>(p_opSender)))
		{
			if(m_opView -> LockWindow())
			{
				m_opView -> Invalidate(opInfo -> GetUpdateRect());
				m_opView -> UnlockWindow();
			}
			return true;
		}
		return false;
	case EM_SCROLLBAR_CHANGED:
		m_vScrolledOffsetY = m_opScrollbarVertical -> GetValue();
		m_opView -> ScrollTo(0, m_vScrolledOffsetY);
		return true;
	case EM_SETTING_UPDATED:
eo << "TrackInfo: EM_SETTING_UPDATED" << ef;
		if(m_opView -> LockWindow())
		{
			eEditMode = static_cast<EMEditMode>(*(static_cast<int*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_EDIT_MODE))));
			if(eEditMode != m_eEditMode)
			{
				m_eEditMode = eEditMode;
				ChangeEditMode();
			}
			vExpertMode = *(static_cast<bool*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_EXPERT_MODE)));
			if(vExpertMode != m_vExpertMode)
			{
				m_vExpertMode = vExpertMode;
				SetUserMode();
				m_opView -> Invalidate(m_opView -> Bounds());
			}
			m_opView -> UnlockWindow();
		}
		return true;
	default:
		return false;
	}
}

void TrackInfo::MouseDown(int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree)
{
	EMRect oFrame;
	EMGTrack* opTrack;
	
	m_opMouseDownTrack = NULL;
	m_vMouseDownY = p_vY;
	for(uint32 vIndex = 0; vIndex < EMGTrackRepository::Instance() -> GetNrTracks(); vIndex++)
	{
		opTrack = EMGTrackRepository::Instance() -> GetTrack(vIndex);
		if(!(opTrack -> GetTrackInfo() -> IsHidden(m_eEditMode == EM_CLIP_EDIT_MODE)))
		{
			opTrack -> GetTrackInfo() -> MouseDown(p_vX, p_vY, p_vButtonOne, p_vButtonTwo, p_vButtonThree, m_opView);
			oFrame = EMGTrackRepository::Instance() -> GetTrack(vIndex) -> GetTrackInfo() -> GetUpdateRect();
			if(oFrame != EMRect())
				m_opView -> Invalidate(oFrame);
	
			GTrackInfo* opTrackInfo = dynamic_cast<GTrackInfo*>(opTrack -> GetTrackInfo());
			
			if(opTrackInfo)
				if((opTrackInfo -> GetSelectionButton() -> Frame()).Contains(p_vX, p_vY))
				{
					if(p_vButtonTwo && m_eEditMode != EM_CLIP_EDIT_MODE)
					{
						EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_TRACKS_SELECT_NONE);
						return;
					}
					if(opTrackInfo -> GetSelectionButton() -> GetOn() && m_eEditMode != EM_CLIP_EDIT_MODE)
						m_opMouseDownTrack = opTrack;
				}
		}
	}

	if(m_oEmptySpace.Contains(p_vX, p_vY) && m_eEditMode != EM_CLIP_EDIT_MODE)
		EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_TRACKS_SELECT_NONE);
}

void TrackInfo::MouseDoubleClick(int32 p_vX, int32 p_vY)
{
	EMRect oFrame;
eo << "TrackInfo::MouseDoubleClick - p_vX: " << p_vX << ", p_vY: " << p_vY << ef;
	for(uint32 vIndex = 0; vIndex < EMGTrackRepository::Instance() -> GetNrTracks(); vIndex++)
	{
		EMGTrackRepository::Instance() -> GetTrack(vIndex) -> GetTrackInfo() -> MouseDoubleClick(p_vX, p_vY, m_opView);
		oFrame = EMGTrackRepository::Instance() -> GetTrack(vIndex) -> GetTrackInfo() -> GetUpdateRect();
		if(oFrame != EMRect())
			m_opView -> Invalidate(oFrame);
	}
}

void TrackInfo::MouseMoved(int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree)
{
	EMRect oFrame;
	bool vWillDrawOnButtonBar(false);
	
	if(m_eEditMode == EM_CLIP_EDIT_MODE)
		for(uint32 vIndex = 0; vIndex < EMGTrackRepository::Instance() -> GetNrTracks(); vIndex++)
		{
			EMGTrackRepository::Instance() -> GetTrack(vIndex) -> GetTrackInfo() -> MouseMoved(p_vX, p_vY, p_vButtonOne, p_vButtonTwo, p_vButtonThree);
			oFrame = EMGTrackRepository::Instance() -> GetTrack(vIndex) -> GetTrackInfo() -> GetUpdateRect();
			if(oFrame != EMRect())
				m_opView -> Invalidate(oFrame);
		}

	else if(m_opMouseDownTrack != NULL)
	{
//		if(m_opView -> LockWindow())
		{
			uint32 vIndexOne = EMGTrackRepository::Instance() -> GetTrackIndex(m_opMouseDownTrack);
			EMGTrack* opTrackTwo = EMGTrackRepository::Instance() -> GetTrack(p_vX, p_vY);
			bool vHasMoved(false);
			EMBitmap* opBitmap = EMResourceRepository::GetBitmap(RES_MOVE_TRACKS_CURSOR);
			if(opTrackTwo == NULL)
			{
				if(p_vY < 0)// && vIndexOne > 0)
				{
					vWillDrawOnButtonBar = true;
					vHasMoved = true;
					oFrame = EMRect(0,
								(m_opView -> Bounds()).m_vTop - 3,
								(m_opView -> Bounds()).m_vRight,
								(m_opView -> Bounds()).m_vTop);
				}
				else if(p_vY >= m_oEmptySpace.m_vTop)// && vIndexOne < EMGTrackRepository::Instance() -> GetNrTracks() - 1)
				{
					vHasMoved = true;
					oFrame = EMRect(0,
								m_oEmptySpace.m_vTop - 3,
								(m_opView -> Bounds()).m_vRight,
								m_oEmptySpace.m_vTop - 3 + (opBitmap -> Bounds()).GetHeight());
				}
			}
			else
			{
				uint32 vIndexTwo = EMGTrackRepository::Instance() -> GetTrackIndex(opTrackTwo);
				
				if(vIndexTwo < vIndexOne && opTrackTwo -> GetTrackInfo() -> GetType() == EM_TYPE_TRANSITION) // Cannot move tracks in-between a video track and its transition track
				{
					vIndexTwo--;
					opTrackTwo = EMGTrackRepository::Instance() -> GetTrack(vIndexTwo);
				}
				else if(vIndexTwo > vIndexOne && opTrackTwo -> GetTrackInfo() -> GetType() == EM_TYPE_ANY_VIDEO) // Cannot move tracks in-between a video track and its transition track
				{
					if(static_cast<VideoGTrackInfo*>(opTrackTwo -> GetTrackInfo()) -> GetTransitionTrack() != NULL)
					{
						vIndexTwo++;
						opTrackTwo = EMGTrackRepository::Instance() -> GetTrack(vIndexTwo);
					}
				}

				if(vIndexOne != vIndexTwo)
				{
					vHasMoved = true;
					if(vIndexTwo == 0)
						vWillDrawOnButtonBar = true;
					else if(vIndexTwo < vIndexOne)
						oFrame = EMRect(0,
									(opTrackTwo -> GetTrackInfo() -> Frame()).m_vTop - 3,
									(m_opView -> Bounds()).m_vRight,
									(opTrackTwo -> GetTrackInfo() -> Frame()).m_vTop - 3 + (opBitmap -> Bounds()).GetHeight());
					else
						oFrame = EMRect(0,
									(opTrackTwo -> GetTrackInfo() -> Frame()).m_vBottom - 3,
									(m_opView -> Bounds()).m_vRight,
									(opTrackTwo -> GetTrackInfo() -> Frame()).m_vBottom - 3 + (opBitmap -> Bounds()).GetHeight());
				}
				else
					oFrame = EMRect();
			}
//;//cout_commented_out_4_release << "TrackInfo::MouseMoved: " << vIndexOne << ", " << vIndexTwo << endl;
			if(m_vDrawedOnButtonBar && !vWillDrawOnButtonBar)
			{
				TrackInfoButtonBar::Instance() -> GetView() -> Invalidate(TrackInfoButtonBar::Instance() -> GetView() -> Bounds());
				m_vDrawedOnButtonBar = false;
			}
			else if(m_oPreviousSelectRect != oFrame)
					m_opView -> Invalidate(m_oPreviousSelectRect);
			if(vHasMoved)
			{
				if(vWillDrawOnButtonBar)
				{
					EMRect oOtherViewBounds(TrackInfoButtonBar::Instance() -> GetView() -> Bounds());
					TrackInfoButtonBar::Instance() -> GetView() -> BeginPaint();
					TrackInfoButtonBar::Instance() -> GetView() -> DrawBitmap(EMResourceRepository::GetBitmap(RES_MOVE_TRACKS_CURSOR), 0, oOtherViewBounds.m_vBottom - 3);
					TrackInfoButtonBar::Instance() -> GetView() -> EndPaint();
					m_vDrawedOnButtonBar = true;
				}
				else
				{
					m_opView -> BeginPaint();
					m_opView -> DrawBitmap(EMResourceRepository::GetBitmap(RES_MOVE_TRACKS_CURSOR), 0, oFrame.m_vTop);
					m_opView -> EndPaint();
				}
			}
			m_oPreviousSelectRect = oFrame;
//oFrame.Print();
//			m_opView -> UnlockWindow();
		}
	}
	else
	{
		for(uint32 vIndex = 0; vIndex < EMGTrackRepository::Instance() -> GetNrTracks(); vIndex++)
		{
			EMGTrackRepository::Instance() -> GetTrack(vIndex) -> GetTrackInfo() -> MouseMoved(p_vX, p_vY, p_vButtonOne, p_vButtonTwo, p_vButtonThree);
			oFrame = EMGTrackRepository::Instance() -> GetTrack(vIndex) -> GetTrackInfo() -> GetUpdateRect();
			if(oFrame != EMRect())
				m_opView -> Invalidate(oFrame);
		}
	}
}

void TrackInfo::MouseUp(int32 p_vX, int32 p_vY)
{
	if(m_eEditMode == EM_CLIP_EDIT_MODE)
	{
		for(uint32 vIndex = 0; vIndex < EMGTrackRepository::Instance() -> GetNrTracks(); vIndex++)
			EMGTrackRepository::Instance() -> GetTrack(vIndex) -> GetTrackInfo() -> MouseUp(p_vX, p_vY);

		m_opMouseDownTrack = NULL;
	}

	else if(m_opMouseDownTrack != NULL)
	{
		if(m_vDrawedOnButtonBar)
		{
			TrackInfoButtonBar::Instance() -> GetView() -> Invalidate(TrackInfoButtonBar::Instance() -> GetView() -> Bounds());
			m_vDrawedOnButtonBar = false;
		}
		if(m_opMouseDownTrack == EMGTrackRepository::Instance() -> GetTrack(p_vX, p_vY))
		{
			m_opMouseDownTrack = NULL;
			return;
		}
		m_opMouseDownTrack = NULL;
		uint32 vNewIndex;
		if(p_vY < 0)
			vNewIndex = 0;
		else if(m_oEmptySpace.Contains(p_vX, p_vY))
			vNewIndex = EMGTrackRepository::Instance() -> GetNrTracks() - 1;
		else
			vNewIndex = EMGTrackRepository::Instance() -> GetTrackIndex(EMGTrackRepository::Instance() -> GetTrack(p_vX, p_vY));

		uint32 vBeforeIndex;
		uint32 vAfterIndex;
		if(p_vY < m_vMouseDownY)
		{
			if(vNewIndex == 0)
				vBeforeIndex = 0;//EM_new uint32(0);
			else
				vBeforeIndex = vNewIndex - 1;//EM_new uint32(vNewIndex - 1);
			vAfterIndex = vNewIndex;//EM_new uint32(vNewIndex);
			EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_MOVE_TRACKS, &vBeforeIndex, &vAfterIndex);
			m_oPreviousSelectRect = EMRect();
		}
		else
		{
			vBeforeIndex = vNewIndex;//EM_new uint32(vNewIndex);
//			if(vNewIndex == EMGTrackRepository::Instance() -> GetNrTracks() - 1)
//				pvAfterIndex = EM_new uint32(vNewIndex);
//			else
				vAfterIndex = vNewIndex + 1;//EM_new uint32(vNewIndex + 1);
			EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_MOVE_TRACKS, &vBeforeIndex, &vAfterIndex);
			m_oPreviousSelectRect = EMRect();
		}
	}
	else
	{
		EMRect oFrame;
		for(uint32 vIndex = 0; vIndex < EMGTrackRepository::Instance() -> GetNrTracks(); vIndex++)
		{
			EMGTrackRepository::Instance() -> GetTrack(vIndex) -> GetTrackInfo() -> MouseUp(p_vX, p_vY);
			oFrame = EMGTrackRepository::Instance() -> GetTrack(vIndex) -> GetTrackInfo() -> GetUpdateRect();
			if(oFrame != EMRect())
				m_opView -> Invalidate(oFrame);
		}
	}	
}

void TrackInfo::PrepareToClearData()
{
	//m_opView -> UnlockWindow(); // Temporary unlock to prevent deadlock
	EMThread* opThread = EMThreadRepository::Instance() -> GetThread(GUI_SERVICE_THREAD_NAME);
	if(opThread != NULL)
		opThread -> Suspend();//RemoveListener(this);

	//m_opView -> LockWindow(); 
}

void TrackInfo::ReadyForUse()
{
eo << "TrackInfo::ReadyForUse" << ef;
	if(m_opView -> LockWindow())
	{
		CalculateTrackInfoFramesAndUpdate(NULL);
		CalculateAndSetTrackInfos();
		EMThreadRepository::Instance() -> GetThread(GUI_SERVICE_THREAD_NAME) -> Resume();//Start();//AddListener(this);
eo << "TrackInfo::ReadyForUse - thread resumed" << ef;
		SetUserMode(); // After a project load, solo and mute button display modes must be set for all tracks
		EnableMenuItems();

		EMGTrackInfo* opTrackInfo;
		for(uint32 vIndex = 0; vIndex < EMGTrackRepository::Instance() -> GetNrTracks(); vIndex++)
		{
			opTrackInfo = EMGTrackRepository::Instance() -> GetTrack(vIndex) -> GetTrackInfo();
			opTrackInfo -> EditModeChanged(m_eEditMode);
		}

		m_opView -> UnlockWindow();
	}
}
/*
void TrackInfo::SaveComplete()
{
	EMThreadRepository::Instance() -> GetThread(GUI_SERVICE_THREAD_NAME) -> Start();//AddListener(this);
}
*/
void TrackInfo::SetScrollbar(EMScrollbar* p_opScrollbarVertical)
{
	m_opScrollbarVertical = p_opScrollbarVertical;
}

/*
void TrackInfo::SetFrame(EMRect p_oFrame)
{
	m_opView -> SetFrame(p_oFrame);
	for(uint32 vIndex = 0; vIndex < EMGTrackRepository::Instance() -> GetNrTracks(); vIndex++)
		EMGTrackRepository::Instance() -> GetTrack(vIndex) -> GetTrackInfo() -> SetFrame();
*/

void TrackInfo::SetUserMode()
{
	for(uint32 vIndex = 0; vIndex < EMGTrackRepository::Instance() -> GetNrTracks(); vIndex++)
		if(dynamic_cast<GTrackInfo*>(EMGTrackRepository::Instance() -> GetTrack(vIndex) -> GetTrackInfo()))
			dynamic_cast<GTrackInfo*>(EMGTrackRepository::Instance() -> GetTrack(vIndex) -> GetTrackInfo()) -> SetUserMode(m_vExpertMode);
}

void TrackInfo::ThreadRun(EMThread* p_opThread)
{
//eo << "TrackInfo::ThreadRun" << ef;
	if(!m_vSignal)
		return;

//eo << "TrackInfo::ThreadRun - getting signal meter values" << ef;
	// This call is made outside LockWindow to prevent deadlocks (e.g. when the media engine creates an audio/video clip at the same time)
	EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_MIDI_SIGNAL_METER_VALUE, &m_vMIDITrackSignalValues);
	EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_SIGNAL_METER_VALUE, &m_vAudioTrackSignalValues);

	if(m_opView -> LockWindow())
	{
		list<EMGTrackInfo*>::iterator oTrackInfoIterator;
		int32 vCounter;
		GTrackInfo* opTrackInfo;

		// Audio signal meter stuff
		oTrackInfoIterator = m_oAudioTrackInfos.begin();
		float* vpSignal;
		vCounter = 0;
		while(oTrackInfoIterator != m_oAudioTrackInfos.end())
		{
			vpSignal = m_vAudioTrackSignalValues[vCounter];
			opTrackInfo = static_cast<GTrackInfo*>(*oTrackInfoIterator);
			if(!(opTrackInfo -> IsHidden(m_eEditMode == EM_CLIP_EDIT_MODE)))
				opTrackInfo -> SetSignal(vpSignal);
//if(vpSignal[0] > 0)
//eo << "Got signal from array index: " << vCounter << ef;

			oTrackInfoIterator++;
			vCounter++;
		}

		// MIDI signal meter stuff
		oTrackInfoIterator = m_oMIDITrackInfos.begin();
		float vSignal;
		vCounter = 0;
		while(oTrackInfoIterator != m_oMIDITrackInfos.end())
		{
			vSignal = m_vMIDITrackSignalValues[vCounter];
			opTrackInfo = static_cast<GTrackInfo*>(*oTrackInfoIterator);
			if(!(opTrackInfo -> IsHidden(m_eEditMode == EM_CLIP_EDIT_MODE)))
				opTrackInfo -> SetSignal(vSignal);

			oTrackInfoIterator++;
			vCounter++;
		}		

		m_opView -> UnlockWindow();
	}
}


