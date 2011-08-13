#include "TrackView.h"

#include "AudioGClipContainer.h"
#include "CommandIDs.h"
#include "EMCommandRepository.h"
#include "EMDragDrop.h"
#include "EMImageBufferRepository.h"
#include "EMFactory.h"
#include "EMGClip.h"
#include "EMGClipRepository.h"
#include "EMGTrack.h"
#include "EMGTrackRepository.h"
#include "EMGUIGlobals.h"
#include "EMGUIUtilities.h"
#include "EMKeyboard.h" // Debug?
#include "EMKeyboardGlobals.h" // Debug?
#include "EMMediaMIDIEvent.h"
#include "EMMenuBar.h"
#include "EMMessageBox.h"
#include "EMMessages.h"
#include "EMResourceRepository.h"
#include "EMScrollbar.h"
#include "EMSettingIDs.h"
#include "EMSettingsRepository.h"
#include "EMThreadRepository.h"
#include "EMToolTip.h"
#include "EMVideoPreview.h" // Debug
#include "EMView.h"
#ifdef PLATFORM_WINDOWS
//#include "EMWinDropTarget.h"
#include "EMDragDrop.h"
#endif
#include "GUIGlobals.h"
#include "MediaPoolView.h"
#include "Messages.h"
#include "MIDIGClipContainer.h"
#include "ResourceIDs.h"
#include "SettingIDs.h"
#include "TrackInfoButtonBar.h"
#include "TrackWindowStatusBar.h"
#include "VideoGClip.h"
#include "VideoGClipContainer.h"
 
const int MOUSE_SCROLL_THRESHOLD = 20;
const uint32 DRAW_SIMPLE_THRESHOLD = 20;
const int32 GRID_MINIMUM_SPACING = 10;
const int FX_EDIT_MODE_SCROLLABLE_WIDTH = 3000;
const int64 MAX_SCROLLABLE_TIME = 0x861c46800L; // 10 hours

TrackView::TrackView() :
m_opMouseDownTrack(NULL),
m_opScrollbarHorizontal(NULL),
m_opScrollbarVertical(NULL),
m_opSnappingMode(NULL),
m_opView(EMFactory::Instance() -> CreateView(EMRect(), EM_FOLLOW_LEFT_RIGHT | EM_FOLLOW_TOP_BOTTOM, true)),
m_opToolTip(EMFactory::Instance() -> CreateToolTip(EMRect(), "Gronk!", m_opView, false)),
m_oRectEmpty(0, 0, MAX_VIEW_PIXEL_LIMIT, MAX_VIEW_PIXEL_LIMIT),
m_vClipsHaveMoved(false),
m_vDrawClipContent(false),
m_vMouseButtonPressed(false),
m_vOldPosition(0),//(int32) 0xFFFFFFFF),
//m_vOldThenPosition(0),//(int32) 0xFFFFFFFF),
m_vPosition(0),
m_vScrolledOffsetX(0),
m_vScrolledOffsetY(0),
m_vSelectionBoxExists(false)
{
;//cout_commented_out_4_release << "TrackView::TrackView" << endl;
#ifdef PLATFORM_WINDOWS
//	m_opIDropTarget = EM_new EMWinDropTarget(m_opView, VIEW);
//	RegisterDragDrop(static_cast<HWND>(m_opView -> GetNativeView()), m_opIDropTarget);
	EMDragDrop::Instance() -> RegisterTarget(m_opView, EM_VIEW);
#endif
}

TrackView::~TrackView()
{
	delete m_opToolTip;
	EMThread* opThread = EMThreadRepository::Instance() -> GetThread(GUI_SERVICE_THREAD_NAME);
	if(opThread)
		opThread -> RemoveListener(this);
	delete m_opView;
#ifdef PLATFORM_WINDOWS
//	delete m_opIDropTarget;
#endif
}

// Makes sure that moved clips end up in the right place
// Clip snapping takes place here and other evil stuff
void TrackView::AdjustMoveOffset()
{
	GetView() -> AssertWindowLocked();

	EMRect oMovableRect = m_oRectTracks;
	EMRect oSelectedRect = EMGClipRepository::Instance() -> SelectedFrame();

	int64 vEarliestSelectedPosition = EMGClipRepository::Instance() -> GetEarliestSelectedPosition();
	int64 vLatestSelectedPosition = EMGClipRepository::Instance() -> GetLatestSelectedPosition();
	int64 vMovedPosition = vEarliestSelectedPosition + static_cast<int64>(m_vMouseOffsetX * m_vTimeZoomScale);
	int64 vMovedPositionEnd = vLatestSelectedPosition + static_cast<int64>(m_vMouseOffsetX * m_vTimeZoomScale);

	if(oSelectedRect.m_vLeft + m_vMouseOffsetX < 0)
	{
		vMovedPosition = -1 * vEarliestSelectedPosition;
		vMovedPositionEnd = vLatestSelectedPosition - vEarliestSelectedPosition;
		m_vMouseOffsetX = -1 * oSelectedRect.m_vLeft;
	}
	if(m_vMouseDownY + m_vMouseOffsetY < 0)
		m_vMouseOffsetY = -1 * m_vMouseDownY;
	else if(m_vMouseDownY + m_vMouseOffsetY > oMovableRect.m_vBottom)
		m_vMouseOffsetY = oMovableRect.m_vBottom - m_vMouseDownY;
	
	/*
	int32 vSnapThreshold = *(static_cast<int32*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_SNAP_THRESHOLD)));
	if(vMovedPosition / m_vTimeZoomScale < vSnapThreshold) // Always snap to 0
	{
		m_vMouseOffsetX = -1 * oSelectedRect.m_vLeft;
		m_vPositionMovedClipOffset = -1 * vEarliestSelectedPosition;
	}
	else if(abs((vMovedPosition / m_vTimeZoomScale) - (vEarliestSelectedPosition / m_vTimeZoomScale)) < vSnapThreshold) // Always snap to original position
	{
		m_vMouseOffsetX = 0;
		m_vPositionMovedClipOffset = 0;
	}
	else if(abs((vMovedPosition / m_vTimeZoomScale) - (m_vPosition / m_vTimeZoomScale)) < vSnapThreshold) // Always snap to position pointer
	{
		m_vMouseOffsetX = (m_vPosition / m_vTimeZoomScale) - oSelectedRect.m_vLeft;
		m_vPositionMovedClipOffset = m_vPosition - vEarliestSelectedPosition;
	}

	else if(m_vSnapping)
	{
		// First, try snapping at the start of the selected clips
		if(EMGUIUtilities::Instance() -> GetSnapPosition(&vMovedPosition, true, true))
		{
			m_vMouseOffsetX = vMovedPosition / m_vTimeZoomScale - oSelectedRect.m_vLeft;
			m_vPositionMovedClipOffset = vMovedPosition - vEarliestSelectedPosition;
		}
		// If the above fails, try snapping to the end of the selected clips
		else if(EMGUIUtilities::Instance() -> GetSnapPosition(&vMovedPositionEnd, true, false))
		{
			m_vMouseOffsetX = vMovedPositionEnd / m_vTimeZoomScale - oSelectedRect.m_vRight;
			m_vPositionMovedClipOffset = vMovedPositionEnd - vLatestSelectedPosition;
		}
	}
	*/

	// First, try snapping at the start of the selected clips
	if(EMGUIUtilities::Instance() -> GetSnapPosition(&vMovedPosition, true, true, NULL, true, true, vEarliestSelectedPosition, true))
	{
		m_vMouseOffsetX = vMovedPosition / m_vTimeZoomScale - oSelectedRect.m_vLeft;
		m_vPositionMovedClipOffset = vMovedPosition - vEarliestSelectedPosition;
	}
	// If the above fails, try snapping to the end of the selected clips
	else if(EMGUIUtilities::Instance() -> GetSnapPosition(&vMovedPositionEnd, true, false, NULL, false, true))
	{
		m_vMouseOffsetX = vMovedPositionEnd / m_vTimeZoomScale - oSelectedRect.m_vRight;
		m_vPositionMovedClipOffset = vMovedPositionEnd - vLatestSelectedPosition;
	}

	EMGTrackRepository* opTrackRepository = EMGTrackRepository::Instance();

	EMRect vSelectedFrame = EMGClipRepository::Instance() -> SelectedFrame();
	int32 vUpperTrackIndex = opTrackRepository -> GetTrackIndex(opTrackRepository -> GetTrack(0, vSelectedFrame.m_vTop));
	int32 vLowerTrackIndex = opTrackRepository -> GetTrackIndex(opTrackRepository -> GetTrack(0, vSelectedFrame.m_vBottom));
	m_vTrackOffset = opTrackRepository -> GetTrackIndex(opTrackRepository
				 -> GetTrack(m_vMouseOffsetX + m_vMouseDownX, 
				 m_vMouseOffsetY + m_vMouseDownY)) - 
				 opTrackRepository -> GetTrackIndex(m_opMouseDownTrack);
	if(vUpperTrackIndex + m_vTrackOffset < 0)
		m_vTrackOffset = -1 * vUpperTrackIndex;
	else if(vLowerTrackIndex + m_vTrackOffset >  static_cast<int32>(opTrackRepository -> GetNrTracks()) - 1)
		m_vTrackOffset = opTrackRepository -> GetNrTracks() - vLowerTrackIndex - 1;
	EMGClipRepository::Instance() -> CalculateMovedFrame(m_vTrackOffset, m_vPositionMovedClipOffset);
eo << "TrackView::AdjustMoveOffset: m_vPositionMovedClipOffset: " << (int32) m_vPositionMovedClipOffset << ", vMovedPosition: " << (int32) vMovedPosition << ef;
} 

void TrackView::CalculateAndUpdateAll()
{
m_opView -> Invalidate(CalculateAndSetTrackFrames());
m_opView -> Invalidate(CalculateViewFrames());
return;
//		EMRect oOldTrackFrames= m_oRectFull;
		/*m_opView -> Invalidate(*/CalculateAndSetTrackFrames();// & m_opView -> Bounds();//);
//		EMRect oNewTrackFrames = m_oRectFull;
//oOldTrackFrames.Print();
//oNewTrackFrames.Print();
//		m_opView -> Invalidate(oNewTrackFrames);
//		if(!oNewTrackFrames.Contains(oOldTrackFrames))
//			m_opView -> Invalidate(oNewTrackFrames & oOldTrackFrames);
//		m_opView -> Invalidate(CalculateAndSetTrackFrames() & m_opView -> Bounds());
		/*m_opView -> Invalidate(*/CalculateViewFrames();// & m_opView -> Bounds();//);
		// Let's be lazy and invalidate the whole enchilada
		m_opView -> Invalidate(m_opView -> Bounds());
}
/*
void TrackView::CalculateSnappingMode()
{
	list<string>* opModes = EMResourceRepository::GetStringList(RES_TB_SNAPPING_MODES);
	int vIndex = 0;
	list<string>::iterator oIterator = opModes -> begin();
	while(oIterator != opModes -> end())
	{
		if(*oIterator == *m_opSnappingMode)
		{
			m_eSnappingMode = static_cast<SnappingMode>(vIndex);
			return;
		}
		oIterator++;
		vIndex++;
	}
	EMDebugger("Could not resolve snapping mode");
}
*/

// Calculates the following frames:
//		m_oRectFull
//		m_oRectVisible
//		m_oRectEmpty
// Also calls CalculateAndSetScrollableFrame to calculate m_oRectScrollable
EMRect TrackView::CalculateViewFrames()
{
	EMRect oUpdateRect;

	GetView() -> AssertWindowLocked();
	m_oRectVisible = EMRect(m_vScrolledOffsetX, m_vScrolledOffsetY, 
				m_vScrolledOffsetX + (GetView() -> Bounds()).GetWidth(), 
				m_vScrolledOffsetY + (GetView() -> Bounds()).GetHeight());
//;//cout_commented_out_4_release << "TrackView::CalculateViewFrames: m_vScrolledOffsetX: " << m_vScrolledOffsetX << endl;
//m_oRectVisible.Print();	

	int32 vBottom = -1;
	EMGTrack* opTrack;
	for(uint32 vIndex = 0; vIndex < EMGTrackRepository::Instance() -> GetNrTracks(); vIndex++)
	{
		opTrack = EMGTrackRepository::Instance() -> GetTrack(vIndex);
		if(!(opTrack -> GetTrackInfo() -> IsHidden(m_eEditMode == EM_CLIP_EDIT_MODE)))
			vBottom = (opTrack -> GetClipContainer() -> Frame()).m_vBottom;
	}
	m_oRectTracks = EMRect(0, 0, MAX_VIEW_PIXEL_LIMIT, vBottom);


//	m_oRectTracks = EMRect();
//	for(uint32 vIndex = 0; vIndex < EMGTrackRepository::Instance() -> GetNrTracks(); vIndex++)
//		m_oRectTracks = m_oRectTracks | EMGTrackRepository::Instance() -> GetTrack(vIndex) -> GetClipContainer() -> Frame();
	EMRect oOldRectFull = m_oRectFull;
	m_oRectFull = m_oRectTracks & m_oRectVisible;
	
	// Convenient place (but perhaps not a proper design) to configure the image buffer repository
	if(m_oRectFull != oOldRectFull)
	{
		////Releasing window semaphore since calling a semaphore protected method
		//m_opView -> UnlockWindow();
//;//cout_commented_out_4_release << "EMImageBufferRepository::Instance() -> SetVisibleArea: ";
//m_oRectFull.Print();
		EMImageBufferRepository::Instance() -> SetVisibleArea(m_oRectFull);
		//m_opView -> LockWindow();
	}

	EMRect oRectEmpty = m_oRectVisible;
	if(m_oRectFull.IsValid())
		oRectEmpty.m_vTop = m_oRectFull.m_vBottom + 1;
	else
		oRectEmpty = m_oRectVisible;
	if(oRectEmpty != m_oRectEmpty)
	{
		if(!m_oRectEmpty.Contains(oRectEmpty))
			oUpdateRect = oUpdateRect | (m_oRectEmpty ^ oRectEmpty) | (oRectEmpty ^ m_oRectEmpty);
		m_oRectEmpty = oRectEmpty;
	}

	CalculateAndSetScrollableFrame();
/*
	eo << "-------------------Calculate bounds---------------------" << ef;
	eo << "m_oRectVisible: ";
	m_oRectVisible.Print();
	eo << "m_oRectTracks: ";
	m_oRectTracks.Print();
	eo << "m_oRectFull: ";
	m_oRectFull.Print();
	eo << "m_oRectEmpty: ";
	m_oRectEmpty.Print();
	eo << "Selected rect: ";
	(EMGClipRepository::Instance() -> SelectedFrame()).Print();
	eo << "m_oRectScrollable: ";
	m_oRectScrollable.Print();
	eo << "--------------------------------------------------------" << ef;
*/
/*
		if(m_opPositionPointer != NULL)
			m_opPositionPointer -> UpdateBounds();	
		//EMGClipRepository::Instance() -> MovedBounds() = EMGClipRepository::Instance() -> SelectedBounds();
*/
	return oUpdateRect;
}

EMRect TrackView::CalculateTrackFrame(EMGTrack* p_opTrack)
{
/*
	// FIXME: Add support for effect and transition types here
	if((!(p_opTrack -> GetTrackInfo() -> IsSelected()) && m_vClipEdit) ||
		(p_opTrack -> GetType() == EM_TYPE_ANY_AUDIO && !(TrackInfoButtonBar::Instance() -> GetAudioTrackFilterOn())) ||
		(p_opTrack -> GetType() == EM_TYPE_ANY_VIDEO && !(TrackInfoButtonBar::Instance() -> GetVideoTrackFilterOn())) ||
		(p_opTrack -> GetType() == EM_TYPE_MIDI && !(TrackInfoButtonBar::Instance() -> GetMIDITrackFilterOn())))
		return EMRect();
*/

	int32 vTop = 0;
	EMGTrack* opTrack;
	int32 vTrackIndex = EMGTrackRepository::Instance() -> GetTrackIndex(p_opTrack);
	for(int32 vIndex = 0; vIndex < vTrackIndex; vIndex++)
	{
		opTrack = EMGTrackRepository::Instance() -> GetTrack(vIndex);
//		opTrack -> GetTrackInfo() -> CalculateAndSetHidden(m_vClipEdit);
		if(!(opTrack -> GetTrackInfo() -> IsHidden(m_eEditMode == EM_CLIP_EDIT_MODE)))
			vTop = (opTrack -> GetClipContainer() -> Frame()).m_vBottom + 1;
	}
	EMRect oFrame(0, vTop, MAX_VIEW_PIXEL_LIMIT, -1);
	
/*	
	EMRect oFrame;
	int32 vIndex = EMGTrackRepository::Instance() -> GetTrackIndex(p_opTrack);
	if(vIndex == 0)
		oFrame = EMRect(0, 0, MAX_VIEW_PIXEL_LIMIT, -1);
	else if(m_opView -> LockWindow())
	{
		while(true)
		{
			vIndex--;
			opTrack = EMGTrackRepository::Instance() -> GetTrack(vIndex);
			if(vIndex < 0)
			{
				oFrame = EMRect(0, 0, MAX_VIEW_PIXEL_LIMIT, -1);
				break;
			}
//			else if(!(opTrack -> GetTrackInfo() -> IsHidden()))
			else if(!((opTrack -> GetTrackInfo() -> Frame()).IsValid()))
			{
				oFrame = EMRect(0, (opTrack -> GetClipContainer() -> Frame()).m_vBottom + 1, MAX_VIEW_PIXEL_LIMIT, -1);
				break;
			}
		}
		m_opView -> UnlockWindow();
	}
*/

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
//	p_opTrack -> GetClipContainer() -> SetFrame(oFrame);
	return oFrame;
}

void TrackView::CalculateAndSetScrollableFrame(int32 p_vMinimumX)
{
	m_opView -> AssertWindowLocked();
	int32 vPreviousRight = m_oRectScrollable.m_vRight;
	m_oRectScrollable = m_oRectTracks;//EMGTrackRepository::Instance() -> ClipContainerFrame();
	m_oRectScrollable.m_vRight = 1; // Should be smaller than the visible width
	if(m_oRectScrollable.m_vBottom < 1)
		m_oRectScrollable.m_vBottom = 1;
	if((EMGClipRepository::Instance() -> Frame()).IsValid())
		m_oRectScrollable = m_oRectScrollable | EMGClipRepository::Instance() -> Frame();
	if(m_vMouseButtonPressed)
		m_oRectScrollable = m_oRectScrollable | EMGClipRepository::Instance() -> MovedFrame();
	// Scrolls a bit furter than the end of the last clip
	m_oRectScrollable.m_vRight += m_oRectVisible.GetWidth() / 2;
	// To prevent violent left scrolls, keep rightmost right border when mouse button is pressed
	if(m_vMouseButtonPressed && (vPreviousRight > m_oRectScrollable.m_vRight))
		m_oRectScrollable.m_vRight = vPreviousRight;

	if(m_eEditMode == EM_FX_EDIT_MODE)
		m_oRectScrollable.m_vRight = FX_EDIT_MODE_SCROLLABLE_WIDTH;
	else if(m_oRectScrollable.m_vRight * m_vTimeZoomScale > MAX_SCROLLABLE_TIME)
		m_oRectScrollable.m_vRight = MAX_SCROLLABLE_TIME / m_vTimeZoomScale;

	if(p_vMinimumX > m_oRectScrollable.m_vRight)
		m_oRectScrollable.m_vRight = p_vMinimumX;

	// Check if the scroll offsets (currently only checks the Y offset) need to be updated
	if(m_oRectVisible.m_vBottom > m_oRectScrollable.m_vBottom)
	{
		EMRect oOffsetRect = m_oRectVisible;
		oOffsetRect.OffsetBy(0, -1 * (m_oRectVisible.m_vBottom - m_oRectScrollable.m_vBottom));
		if(oOffsetRect.m_vTop < 0)
			oOffsetRect.OffsetBy(0, -1 * oOffsetRect.m_vTop);
		int32 vScrolledOffsetY = oOffsetRect.m_vTop;
		if(vScrolledOffsetY != m_vScrolledOffsetY)
		{
			m_vScrolledOffsetY = vScrolledOffsetY;
			CalculateViewFrames();
			Scroll();
		}
	}

	// Temporarily unlock the window since the native scrollbars may need to send some messages of their own
	m_opView -> UnlockWindow();
	if(m_opScrollbarHorizontal != NULL)
	{
		m_opScrollbarHorizontal -> SetPageAndRange(m_oRectVisible.GetWidth(), 0, m_oRectScrollable.GetWidth());
		m_opScrollbarHorizontal -> SetSteps(static_cast<int>(m_oRectVisible.GetWidth() / 10), static_cast<int>(m_oRectVisible.GetWidth()));
		if(m_opScrollbarHorizontal -> GetValue() != m_vScrolledOffsetX)
			m_opScrollbarHorizontal -> SetValue(m_vScrolledOffsetX);
	}
	if(m_opScrollbarVertical != NULL)
	{
		m_opScrollbarVertical -> SetPageAndRange(m_oRectVisible.GetHeight(), 0, m_oRectScrollable.GetHeight());
		m_opScrollbarVertical -> SetSteps(static_cast<int>(m_oRectVisible.GetHeight() / 10), static_cast<int>(m_oRectVisible.GetHeight()));
		if(m_opScrollbarVertical -> GetValue() != m_vScrolledOffsetY)
			m_opScrollbarVertical -> SetValue(m_vScrolledOffsetY);
	}
	m_opView -> LockWindow();
}

EMRect TrackView::CalculateAndSetTrackFrames()
{
	EMRect oUpdateRect;
	EMGTrack* opTrack;
	EMRect oCalculatedRect;

	for(uint32 vIndex = 0; vIndex < EMGTrackRepository::Instance() -> GetNrTracks(); vIndex++)
	{
		opTrack = EMGTrackRepository::Instance() -> GetTrack(vIndex);
		if(!(opTrack -> GetTrackInfo() -> IsHidden(m_eEditMode == EM_CLIP_EDIT_MODE)))
		{
			oCalculatedRect = CalculateTrackFrame(opTrack);
			if(opTrack -> GetClipContainer() -> SetFrame(oCalculatedRect))
				oUpdateRect = oUpdateRect | oCalculatedRect;
		}
	}

	if(oUpdateRect != EMRect())
	{
		// If the track frames have changed, it is likely that the clip frames have too, so update the clip repository
		EMGClipRepository::Instance() -> CalculateFrame();
		EMGClipRepository::Instance() -> CalculateSelectedFrame();

		Notify(EM_TRACK_FRAME_CHANGED);
	}

	return oUpdateRect;
}

void TrackView::ChangeEditMode(EMEditMode p_eNewEditMode)
{
	bool vScrollNeeded(false);

	if(p_eNewEditMode == EM_FX_EDIT_MODE)
	{
		m_vSavedScrollPosition = m_vScrolledOffsetX * m_vTimeZoomScale;
		m_vScrolledOffsetX = 0;
		vScrollNeeded = true;
	}
	else if(m_eEditMode == EM_FX_EDIT_MODE)
	{
		m_vScrolledOffsetX = static_cast<int32>(m_vSavedScrollPosition / m_vTimeZoomScale);
		vScrollNeeded = true;
	}
	
	m_eEditMode = p_eNewEditMode;

	//m_opView -> Invalidate(m_opView -> Bounds()); // Needed?
	CalculateAndUpdateAll();

	EMGClipRepository::Instance() -> CalculateFrame();
	EMGClipRepository::Instance() -> CalculateSelectedFrame();

	CalculateAndUpdateAll(); // One more time can't hurt (it's actually needed since clip sizes affect the scrollable frame calculated above)

	if(vScrollNeeded)
		Scroll();
}

bool TrackView::ClearData()
{
	m_oRectEmpty = m_opView -> Bounds();
	return true;
}

void TrackView::Draw(EMRect p_oClippingRect)
{
//;//cout_commented_out_4_release << "TrackView::Draw: ";
//p_oClippingRect.Print();
	m_opView -> LockBuffer();

	// This is a debug thingie to detect redraw problems
	/*
	uint32 vModifiers = EMKeyboard::Instance() -> GetModifiers();
	if((vModifiers & EM_SCROLL_LOCK) == EM_SCROLL_LOCK)
	{
		m_opView -> SetHighColor(EMColor(0, 255, 255, 255));
		m_opView -> FillRect(p_oClippingRect);
		m_opView -> DrawAndUnlockBuffer(p_oClippingRect);
		return;
	}
	*/
	// End of debug thingie

	// Draws the clip containers, the vertical grid and the clips within the clip containers
   	if(m_oRectFull.Intersects(p_oClippingRect))
   	{
		EMGTrack* opTrack;
		uint32 vIndex;
		for(vIndex = 0; vIndex < EMGTrackRepository::Instance() -> GetNrTracks(); vIndex++)
		{
			opTrack = EMGTrackRepository::Instance() -> GetTrack(vIndex);
			if(!(opTrack -> GetTrackInfo() -> IsHidden(m_eEditMode == EM_CLIP_EDIT_MODE)))
				opTrack -> GetClipContainer() -> DrawTrack(p_oClippingRect, m_opView, m_eEditMode);
			/*
			if(m_eEditMode == EM_CLIP_EDIT_MODE)
			{
				if(!(opTrack -> GetTrackInfo() -> IsHidden(m_eEditMode == EM_CLIP_EDIT_MODE)))
					opTrack -> GetClipContainer() -> DrawTrack(p_oClippingRect, m_opView, EM_CLIP_EDIT_MODE);
			}
			else if(m_eEditMode == EM_TRACK_EDIT_MODE)
			{
				if(!(opTrack -> GetTrackInfo() -> IsHidden(m_eEditMode == EM_CLIP_EDIT_MODE)))
					opTrack -> GetClipContainer() -> DrawTrack(p_oClippingRect, m_opView, EM_TRACK_EDIT_MODE);
			}
			else
			{
				if(!(opTrack -> GetTrackInfo() -> IsHidden(m_eEditMode == EM_CLIP_EDIT_MODE)))
					opTrack -> GetClipContainer() -> DrawTrack(p_oClippingRect, m_opView, EM_FX_EDIT_MODE);
			}
			*/
		}
		if(m_eEditMode != EM_FX_EDIT_MODE)
		{
			DrawVerticalGrid(p_oClippingRect);

			for(vIndex = 0; vIndex < EMGTrackRepository::Instance() -> GetNrTracks(); vIndex++)
			{
				opTrack = EMGTrackRepository::Instance() -> GetTrack(vIndex);
				if(!(opTrack -> GetTrackInfo() -> IsHidden(m_eEditMode == EM_CLIP_EDIT_MODE)) && (opTrack -> GetClipContainer() -> Frame()).Intersects(p_oClippingRect))
					EMGClipRepository::Instance() -> DrawClips(opTrack -> GetClipContainer() -> GetClips(), p_oClippingRect, m_opView, m_eEditMode == EM_CLIP_EDIT_MODE, m_vTimeZoomScale, m_vDrawClipContent);
			}
		}
		for(vIndex = 0; vIndex < EMGTrackRepository::Instance() -> GetNrTracks(); vIndex++)
		{
			opTrack = EMGTrackRepository::Instance() -> GetTrack(vIndex);
			if(!(opTrack -> GetTrackInfo() -> IsHidden(m_eEditMode == EM_CLIP_EDIT_MODE)))
				opTrack -> GetClipContainer() -> DrawAfterClips(p_oClippingRect, m_opView, m_eEditMode);
		}
	}
	
	// Draws the current region
	if(m_vRegionBegin != -1 && m_vRegionEnd != -1 && m_eEditMode == EM_TRACK_EDIT_MODE)
	{
		EMRect oRegionRect = EMRect(m_vRegionBegin / m_vTimeZoomScale, 0, m_vRegionEnd / m_vTimeZoomScale, (m_opView -> Bounds()).m_vBottom);
		if(oRegionRect.Intersects(p_oClippingRect))
		{
			m_opView -> SetDrawingMode(EM_OP_ALPHA);
			m_opView -> SetHighColor(EMColor(0, 0, 0, 50));
			m_opView -> FillRect(oRegionRect & p_oClippingRect);
			m_opView -> SetDrawingMode(EM_OP_COPY);
		}
	}

	// Draws the empty space below the existing clip containers
	if(m_oRectEmpty.Intersects(p_oClippingRect))
	{
		m_opView -> SetHighColor(EM_COLOR_VIEW_BACKGROUND);
		m_opView -> FillRect(m_oRectEmpty & p_oClippingRect);
	}

	// Draws the border around the selected clips
	if(m_eEditMode == EM_TRACK_EDIT_MODE && (EMGClipRepository::Instance() -> SelectedFrame()).Intersects(p_oClippingRect) && EMGClipRepository::Instance() -> GetNrSelectedClips() > 0)
	{
		m_opView -> SetHighColor(EM_COLOR_CLIP_SELECTION_BOX);
//		m_opView -> StrokeRect(EMGClipRepository::Instance() -> SelectedFrame());
		EMRect oSelectedFrame(EMGClipRepository::Instance() -> SelectedFrame());
		EMRect oDrawFrame = oSelectedFrame & p_oClippingRect;

		m_opView -> StrokeLine(oDrawFrame.m_vLeft, oSelectedFrame.m_vTop, oDrawFrame.m_vRight, oSelectedFrame.m_vTop);
		m_opView -> StrokeLine(oDrawFrame.m_vLeft, oSelectedFrame.m_vBottom, oDrawFrame.m_vRight, oSelectedFrame.m_vBottom);
		m_opView -> StrokeLine(oSelectedFrame.m_vLeft, oSelectedFrame.m_vTop, oSelectedFrame.m_vLeft, oSelectedFrame.m_vBottom);
		m_opView -> StrokeLine(oSelectedFrame.m_vRight, oSelectedFrame.m_vTop, oSelectedFrame.m_vRight, oSelectedFrame.m_vBottom);
	}
	
	// Draws the border around the moved clips
	if(m_eEditMode == EM_TRACK_EDIT_MODE && EMGClipRepository::Instance() -> MovedFrame().Intersects(p_oClippingRect) && m_vMouseButtonPressed && m_vClipsHaveMoved)
	{
		m_opView -> SetHighColor(EM_COLOR_CLIP_MOVE_BOX);
//		m_opView -> StrokeRect(EMGClipRepository::Instance() -> MovedFrame());
		EMRect oMovedFrame(EMGClipRepository::Instance() -> MovedFrame());
		EMRect oDrawFrame = oMovedFrame & p_oClippingRect;

		m_opView -> StrokeLine(oDrawFrame.m_vLeft, oMovedFrame.m_vTop, oDrawFrame.m_vRight, oMovedFrame.m_vTop);
		m_opView -> StrokeLine(oDrawFrame.m_vLeft, oMovedFrame.m_vBottom, oDrawFrame.m_vRight, oMovedFrame.m_vBottom);
		m_opView -> StrokeLine(oMovedFrame.m_vLeft, oMovedFrame.m_vTop, oMovedFrame.m_vLeft, oMovedFrame.m_vBottom);
		m_opView -> StrokeLine(oMovedFrame.m_vRight, oMovedFrame.m_vTop, oMovedFrame.m_vRight, oMovedFrame.m_vBottom);
	}
	
	// Draws the moved clips
	if(m_eEditMode == EM_TRACK_EDIT_MODE && m_vMouseButtonPressed && m_vClipsHaveMoved && EMGClipRepository::Instance() -> GetNrSelectedClips() < DRAW_SIMPLE_THRESHOLD)
		EMGClipRepository::Instance() -> DrawMovedClips(EMGClipRepository::Instance() -> GetSelectedClips(), p_oClippingRect, m_opView, m_vPositionMovedClipOffset, m_vTimeZoomScale, m_vDrawClipContent);

	// Draws the selection box
	if(m_eEditMode == EM_TRACK_EDIT_MODE && m_vSelectionBoxExists)
		DrawSelectionBox(p_oClippingRect, true);

	// Draws the position pointer
	if(m_eEditMode != EM_FX_EDIT_MODE)
		DrawPosition(p_oClippingRect);

	// Draws the ghost clip
	if(m_oGhostClipRect.IsValid())
		DrawGhostClip(p_oClippingRect);

	// Draws the selection made in a track in clip edit mode
	if(m_eEditMode == EM_CLIP_EDIT_MODE && EMGClipContainer::GetClipContainerWithSelection() != NULL && !(EMGClipContainer::GetClipContainerWithSelection() -> GetTrack() -> GetTrackInfo() -> IsHidden(m_eEditMode == EM_CLIP_EDIT_MODE)))
		EMGClipContainer::GetClipContainerWithSelection() -> DrawSelection(p_oClippingRect, m_opView);
	
	m_opView -> DrawAndUnlockBuffer(p_oClippingRect);
}

void TrackView::DrawVerticalGrid(EMRect p_oClippingRect)
{
	int64 vGridSize = EMGUIUtilities::Instance() -> GetGridSize();

	if(vGridSize > 1)
	{
		int64 vMeasureSize = EMGUIUtilities::Instance() -> GetMeasureSize();
		int vGridMeasureDivider = EMGUIUtilities::Instance() -> GetGridMeasureDivider();
		bool vDrawMinor(true);

		if(vGridSize / m_vTimeZoomScale < GRID_MINIMUM_SPACING)
			vDrawMinor = false;
		while(vMeasureSize / m_vTimeZoomScale < GRID_MINIMUM_SPACING)
		{
			vGridMeasureDivider *= 2;
			vMeasureSize *= 2;
		}
		int64 vGridNr = static_cast<int64>(p_oClippingRect.m_vLeft * m_vTimeZoomScale / vGridSize);
		int vMajorGridCounter = vGridMeasureDivider - (vGridNr % vGridMeasureDivider);
		if(vMajorGridCounter == vGridMeasureDivider)
			vMajorGridCounter = 0;
		int64 vStartPosition = vGridNr * vGridSize;
//eo << "DrawVerticalGrid - vGridNr: " << vGridNr << ", vGridSize: " << vGridSize << ", p_oClippingRect.m_vRight * m_vTimeZoomScale: " << p_oClippingRect.m_vRight * m_vTimeZoomScale << ", p_oClippingRect.m_vLeft: " << p_oClippingRect.m_vLeft << ", vStartPosition: " << vStartPosition << ef;
		int64 vPosition = vStartPosition;
		m_opView -> SetHighColor(EMColor(200, 200, 230, 255));
		while(vPosition <= (p_oClippingRect.m_vRight + 2) * m_vTimeZoomScale) // "+2" is because of rounding errors and that 2 lines are drawn in some cases
		{
			if(vDrawMinor || vMajorGridCounter == 0)
				m_opView -> StrokeLine(vPosition / m_vTimeZoomScale, p_oClippingRect.m_vTop, vPosition / m_vTimeZoomScale, p_oClippingRect.m_vBottom);
			if(vMajorGridCounter == 0)
			{
				m_opView -> StrokeLine((vPosition / m_vTimeZoomScale) + 1, p_oClippingRect.m_vTop, (vPosition / m_vTimeZoomScale) + 1, p_oClippingRect.m_vBottom);
				vMajorGridCounter = vGridMeasureDivider - 1;
			}
			else
				vMajorGridCounter--;
			vPosition += vGridSize;
		}
	}
}

void TrackView::DrawGhostClip(EMRect p_oClippingRect)
{
	EMRect oDrawRect;

	if(p_oClippingRect.IsValid()) // If normal paint caused by an invalidate
		oDrawRect = m_oGhostClipRect & p_oClippingRect;
	else
		oDrawRect = m_oGhostClipRect | m_oOldGhostClipRect;
	
	if(oDrawRect.IsValid())
	{
		if(!(p_oClippingRect.IsValid()))
		{
			m_opView -> LockBuffer();
			m_opView -> BeginPaint();
		}
		m_opView -> SetDrawingMode(EM_OP_ALPHA);
//	m_opView -> EnableDoubleBuffering(false);
		m_opView -> SetHighColor(EMColor(0, 0, 0, 100));
		m_opView -> FillRect(oDrawRect);
		m_opView -> SetDrawingMode(EM_OP_COPY);
//	m_opView -> EnableDoubleBuffering(true);
		if(!(p_oClippingRect.IsValid()))
		{
			m_opView -> DrawAndUnlockBuffer(oDrawRect);
			m_opView -> EndPaint();
		}
	}
}

void TrackView::DrawPosition(EMRect p_oClippingRect)
{

	if(p_oClippingRect.IsValid())
	{
		if((p_oClippingRect.m_vLeft <= m_vOldPosition && p_oClippingRect.m_vRight >= m_vOldPosition))
		{
			m_opView -> SetDrawingMode(EM_OP_INVERT);
			m_opView -> StrokeLine(m_vOldPosition, p_oClippingRect.m_vTop, m_vOldPosition, p_oClippingRect.m_vBottom);
			m_opView -> SetDrawingMode(EM_OP_COPY);
		}
/*
// Debug stuff
m_opView -> SetDrawingMode(EM_OP_INVERT);
m_opView -> StrokeLine(m_vOldThenPosition, p_oClippingRect.m_vTop, m_vOldThenPosition, p_oClippingRect.m_vBottom);
m_opView -> SetDrawingMode(EM_OP_COPY);
*/
		return;
	}
	
	m_opView -> LockBuffer();
	m_opView -> EnableDoubleBuffering(false);
	m_opView -> SetDrawingMode(EM_OP_INVERT);
	
	int32 vPosition = m_vPosition / m_vTimeZoomScale;

	if(m_vOldPosition >= m_oRectVisible.m_vLeft && m_vOldPosition <= m_oRectVisible.m_vRight)
		m_opView -> StrokeLine(m_vOldPosition, 0, m_vOldPosition, (m_opView -> Bounds()).m_vBottom);
	if(vPosition >= m_oRectVisible.m_vLeft && vPosition <= m_oRectVisible.m_vRight)
		m_opView -> StrokeLine(vPosition, 0, vPosition, (m_opView -> Bounds()).m_vBottom);
	m_vOldPosition = vPosition;
/*
// Debug stuff
int32 vThenPosition = m_vThenPosition / m_vTimeZoomScale;
m_opView -> StrokeLine(vThenPosition, 0, vThenPosition, (m_opView -> Bounds()).m_vBottom);
m_opView -> StrokeLine(m_vOldThenPosition, 0, m_vOldThenPosition, (m_opView -> Bounds()).m_vBottom);
m_vOldThenPosition = vThenPosition;
*/
	m_opView -> SetDrawingMode(EM_OP_COPY);
	m_opView -> EnableDoubleBuffering(true);
	m_opView -> UnlockBuffer();
}

void TrackView::DrawSelectionBox(EMRect p_oUpdateRect, bool p_vTriggeredByInvalidate)
{
	if(p_vTriggeredByInvalidate)
	{
		if(p_oUpdateRect.Intersects(m_oSelectionBox))
		{
			m_opView -> SetDrawingMode(EM_OP_INVERT);
			if(m_oSelectionBox.IsValid())
				m_opView -> StrokeRect(m_oSelectionBox);
			m_opView -> SetDrawingMode(EM_OP_COPY);
		}
	}
	else
	{
		m_opView -> BeginPaint();
		m_opView -> EnableDoubleBuffering(false);
		m_opView -> SetDrawingMode(EM_OP_INVERT);
		if(m_oOldSelectionBox.IsValid())
			m_opView -> StrokeRect(m_oOldSelectionBox);
		if(m_oSelectionBox.IsValid())
			m_opView -> StrokeRect(m_oSelectionBox);
		m_oOldSelectionBox = m_oSelectionBox;
		m_opView -> SetDrawingMode(EM_OP_COPY);
		m_opView -> EnableDoubleBuffering(true);
		m_opView -> EndPaint();
	}
eo << "Selection box: ";
m_oSelectionBox.Print();
	char vMessage[80];
	if(m_oSelectionBox.IsValid())
	{
		string oLeftTime = EMGUIUtilities::Instance() -> GetTimeString(m_oSelectionBox.m_vLeft * m_vTimeZoomScale, EM_SMPTE);
		string oRightTime = EMGUIUtilities::Instance() -> GetTimeString(m_oSelectionBox.m_vRight * m_vTimeZoomScale, EM_SMPTE);
		sprintf(vMessage, "Selection: (%s, %ld; %s, %ld)", oLeftTime.c_str(), 
							EMGTrackRepository::Instance() -> GetTrackIndex(EMGTrackRepository::Instance() -> GetTrack(0, m_oSelectionBox.m_vTop)),
							oRightTime.c_str(), 
							EMGTrackRepository::Instance() -> GetTrackIndex(EMGTrackRepository::Instance() -> GetTrack(0, m_oSelectionBox.m_vBottom)));						
//		sprintf(vMessage, "(%lld, %ld; %lld, %ld)", static_cast<int64>(m_oSelectionBox.m_vLeft * m_vTimeZoomScale / 1000), 
//							EMGTrackRepository::Instance() -> GetTrackIndex(EMGTrackRepository::Instance() -> GetTrack(0, m_oSelectionBox.m_vTop)),
//							static_cast<int64>(m_oSelectionBox.m_vRight * m_vTimeZoomScale / 1000), 
//							EMGTrackRepository::Instance() -> GetTrackIndex(EMGTrackRepository::Instance() -> GetTrack(0, m_oSelectionBox.m_vBottom)));						
		TrackWindowStatusBar::Instance() -> WriteMessage(vMessage);
	}
}

// Should be called whenever it is suspected that the enabled state of menus
// in response to changes in TrackView has happened
void TrackView::EnableMenuItems()
{
	EMMenuBar::Instance() -> SetEnabled(RES_TM_TRANSPORT, RES_TM_LOOP, m_vRegionBegin >= 0 && m_vRegionEnd > m_vRegionBegin/* && m_eEditMode == EM_TRACK_EDIT_MODE*/);

	int vNrClips = EMGClipRepository::Instance() -> GetNrClips();
	EMMenuBar::Instance() -> SetEnabled(RES_TM_CLIP, RES_TM_SELECT_ALL_CLIPS, vNrClips > 0 && m_eEditMode == EM_TRACK_EDIT_MODE);
	EMMenuBar::Instance() -> SetEnabled(RES_TM_CLIP, RES_TM_SELECT_NO_CLIPS, vNrClips > 0 && m_eEditMode == EM_TRACK_EDIT_MODE);
	EMMenuBar::Instance() -> SetEnabled(RES_TM_CLIP, RES_TM_INVERT_CLIP_SELECTION, vNrClips > 0 && m_eEditMode == EM_TRACK_EDIT_MODE);

	int vNrSelectedClips = EMGClipRepository::Instance() -> GetNrSelectedClips();
	EMMenuBar::Instance() -> SetEnabled(RES_TM_CLIP, RES_TM_CLIP_PROPERTIES, vNrSelectedClips > 0 && m_eEditMode == EM_TRACK_EDIT_MODE);
	EMMenuBar::Instance() -> SetEnabled(RES_TM_CLIP, RES_TM_DELETE_CLIPS, vNrSelectedClips > 0 && m_eEditMode == EM_TRACK_EDIT_MODE);
	EMMenuBar::Instance() -> SetEnabled(RES_TM_CLIP, RES_TM_GROUP_CLIPS, vNrSelectedClips > 1 && m_eEditMode == EM_TRACK_EDIT_MODE);
	EMMenuBar::Instance() -> SetEnabled(RES_TM_CLIP, RES_TM_UNGROUP_CLIPS, vNrSelectedClips > 1 && m_eEditMode == EM_TRACK_EDIT_MODE);

	EMMenuBar::Instance() -> SetEnabled(RES_TM_FILE, RES_TM_EXPORT_MEDIA, EMGClipRepository::Instance() -> GetNrClips() > 0);

	bool vMIDIClipsAreSelected(false); // Clips or events
	bool vAudioClipsAreSelected(false); // Clips or parts of clips
	bool vVideoClipsAreSelected(false); // Clips or parts of clips
	uint32 vNrSelectedVideoClips(0);

	if(m_eEditMode == EM_TRACK_EDIT_MODE)
	{
		list<EMGClip*>::iterator oIterator = EMGClipRepository::Instance() -> GetSelectedClips() -> begin();
		while(oIterator != EMGClipRepository::Instance() -> GetSelectedClips() -> end())
		{
			if((*oIterator) -> GetType() == EM_TYPE_MIDI)
				vMIDIClipsAreSelected = true;
			else if((*oIterator) -> GetType() == EM_TYPE_ANY_AUDIO)
				vAudioClipsAreSelected = true;
			else if((*oIterator) -> GetType() == EM_TYPE_ANY_VIDEO)
			{
				vVideoClipsAreSelected = true;
				vNrSelectedVideoClips++;
			}

			//if(vAudioClipsAreSelected && vMIDIClipsAreSelected)
			//	break; // No need to look further

			oIterator++;
		}
	}

	else if((!vMIDIClipsAreSelected || !vAudioClipsAreSelected) && m_eEditMode == EM_CLIP_EDIT_MODE)
	{
		EMGClipContainer* opContainer = EMGClipContainer::GetClipContainerWithSelection();

		if(opContainer != NULL)
		{
			if(!(opContainer -> GetTrack() -> GetTrackInfo() -> IsHidden(m_eEditMode == EM_CLIP_EDIT_MODE)))
			{
				if(dynamic_cast<MIDIGClipContainer*>(opContainer))
				{
					MIDIGClipContainer* opMIDIContainer = static_cast<MIDIGClipContainer*>(opContainer);
					ClipEditDisplayMode eDisplayMode = opMIDIContainer -> GetDisplayMode();
					if(eDisplayMode == CE_DISPLAY_MODE_DEFAULT) // If MIDI events are affected
					{
						list<EMMediaMIDIEvent*>* opEvents = opMIDIContainer -> GetSelectedMIDIEvents();
						if(opEvents -> size() > 0)
							vMIDIClipsAreSelected = true;
					}
				}
				else if(dynamic_cast<AudioGClipContainer*>(opContainer))
				{
					AudioGClipContainer* opAudioContainer = static_cast<AudioGClipContainer*>(opContainer);
					ClipEditDisplayMode eDisplayMode = opAudioContainer -> GetDisplayMode();
					if(eDisplayMode == CE_DISPLAY_MODE_DEFAULT) // If MIDI events are affected
					{
						int64 vSelectionPos;
						list<AudioGClip*>* opAudioClips = opAudioContainer -> GetSelectedClips(&vSelectionPos, &vSelectionPos);
						if(opAudioClips -> size() > 0)
							vAudioClipsAreSelected = true;
					}
				}
			}
		}
	}

	EMMenuBar::Instance() -> SetEnabled(RES_TM_MIDI, RES_TM_QUICK_QUANTIZE, vMIDIClipsAreSelected);
	EMMenuBar::Instance() -> SetEnabled(RES_TM_MIDI, RES_TM_QUANTIZE, vMIDIClipsAreSelected);
	EMMenuBar::Instance() -> SetEnabled(RES_TM_MIDI, RES_TM_FIXED_LENGTH, vMIDIClipsAreSelected);
	EMMenuBar::Instance() -> SetEnabled(RES_TM_MIDI, RES_TM_SET_VELOCITY, vMIDIClipsAreSelected);
	EMMenuBar::Instance() -> SetEnabled(RES_TM_MIDI, RES_TM_TRANSPOSE, vMIDIClipsAreSelected);

	EMMenuBar::Instance() -> SetEnabled(RES_TM_AUDIO, RES_TM_REVERSE_AUDIO, vAudioClipsAreSelected);
	EMMenuBar::Instance() -> SetEnabled(RES_TM_AUDIO, RES_TM_CROP_AUDIO_SELECTION, vAudioClipsAreSelected);
	EMMenuBar::Instance() -> SetEnabled(RES_TM_AUDIO, RES_TM_SILENCE_AUDIO, vAudioClipsAreSelected);
	EMMenuBar::Instance() -> SetEnabled(RES_TM_AUDIO, RES_TM_NORMALIZE_AUDIO, vAudioClipsAreSelected);
	EMMenuBar::Instance() -> SetEnabled(RES_TM_AUDIO, RES_TM_FADE_IN_LINEAR, vAudioClipsAreSelected);
	EMMenuBar::Instance() -> SetEnabled(RES_TM_AUDIO, RES_TM_FADE_OUT_LINEAR, vAudioClipsAreSelected);
	EMMenuBar::Instance() -> SetEnabled(RES_TM_AUDIO, RES_TM_AUDIO_PLUGIN, vAudioClipsAreSelected);

	//EMMenuBar::Instance() -> SetEnabled(RES_TM_VIDEO, RES_TM_CHANGE_PLAYBACK_SPEED, vVideoClipsAreSelected && vNrSelectedVideoClips == 1);
}

EMView* TrackView::GetView()
{
	return m_opView;
}

void TrackView::Init()
{
	EMSettingsRepository::Instance() -> AddListener(this);
	m_opView -> AddListener(this);
	m_opMouseDownClip = NULL;
	EMThreadRepository::Instance() -> GetThread(GUI_SERVICE_THREAD_NAME) -> AddListener(this);
	m_vTimeZoomScale = *(static_cast<int64*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_TIME_ZOOM_SCALE)));
	m_vTempo = *(static_cast<float*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_TEMPO)));
	m_vRelative = *(static_cast<bool*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_RELATIVE)));
//	m_opSnappingMode = static_cast<string*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_SNAPPING_MODE));
	m_vSnapping = *(static_cast<bool*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_SNAPPING)));
//	CalculateSnappingMode();
	m_opSignature = static_cast<string*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_SIGNATURE));
	sscanf(m_opSignature -> c_str(), "%d/%d", &m_vSignatureMajor, &m_vSignatureMinor);
	m_eEditMode = static_cast<EMEditMode>(*(static_cast<int*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_EDIT_MODE))));
	m_vRegionBegin = *(static_cast<int64*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_REGION_BEGIN)));
	m_vRegionEnd = *(static_cast<int64*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_REGION_END)));
	m_eToolboxMode = static_cast<ToolboxMode>(*(static_cast<int*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_TOOLBOX_MODE))));
	m_vFramerate = *(static_cast<float*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_VIDEO_FRAMERATE)));
	m_oToolbox.Init();
}

/*
void TrackView::ListenToClipContainers()
{
	for(uint32 vIndex = 0; vIndex < EMGTrackRepository::Instance() -> GetNrTracks(); vIndex++)
		EMGTrackRepository::Instance() -> GetTrack(vIndex) -> GetClipContainer() -> AddListener(this);
}
*/

void TrackView::LoadComplete()
{
	// The tempo is set here to prevent clips to be moved when a new tempo is loaded (MoveClipsToTempo will not be called)
	m_vTempo = *(static_cast<float*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_TEMPO)));
}

bool TrackView::MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage)
{
	int32 vX;
	int32 vY;
	bool vButtonOne;
	bool vButtonTwo;
	bool vButtonThree;
	EMGClipContainer* opClipContainer;
	EMGClipRepository* opClipRepository;
	int64 vTimeZoomScale;
	string* opSignature;
	EMEditMode eEditMode;
	
	switch(p_vMessage)
	{
	case EM_DRAW:
		Draw(m_opView -> GetClippingRect());
		return true;
	case EM_CLIP_REPOSITORY_UPDATED:
eo << "EM_CLIP_REPOSITORY_UPDATED - frame: ";
(EMGClipRepository::Instance() -> Frame()).Print();
		CalculateAndUpdateAll();
		EnableMenuItems();
		return true;
	case BUTTONBAR_CHANGED:
		CalculateAndUpdateAll();
		return true;
	case TRACK_FILTERS_CHANGED:
		EMGClipRepository::Instance() -> ClearSelectedClips();
		CalculateAndUpdateAll();
		EnableMenuItems();
		m_opView -> Invalidate(m_opView -> Bounds());
		return true;
	case EM_TRACK_REPOSITORY_UPDATED:
		if(m_vScrolledOffsetY > 0)
		{
			m_vScrolledOffsetY = 0;
			m_opScrollbarVertical -> SetValue(m_vScrolledOffsetY);
			Scroll();
		}
		CalculateAndUpdateAll();
		// If something has happened with the tracks, a complete redraw is not unthinkable
		m_opView -> Invalidate(m_opView -> Bounds());
		return true;
	case EM_SCROLLBAR_CHANGED:
		if(m_opScrollbarHorizontal -> GetValue() == m_vScrolledOffsetX && 
			m_opScrollbarVertical -> GetValue() == m_vScrolledOffsetY)
			return true;
		m_vScrolledOffsetX = m_opScrollbarHorizontal -> GetValue();
		m_vScrolledOffsetY = m_opScrollbarVertical -> GetValue();
		CalculateViewFrames();
		Scroll();
		return true;
	case EM_VIEW_BOUNDS_CHANGED: // Most often due to the user resizing the window
		m_opView -> UpdateBufferFrame();
		CalculateAndUpdateAll();
		return true;
	case EM_UPDATE_NEEDED:
		if((opClipContainer = dynamic_cast<EMGClipContainer*>(p_opSender)) != 0)
		{
			m_opView -> Invalidate((opClipContainer-> GetAndClearUpdateRect()) & (m_opView -> Bounds()));
			return true;
		}
		else if((opClipRepository = dynamic_cast<EMGClipRepository*>(p_opSender)) != 0)
		{
			m_opView -> Invalidate(opClipRepository-> GetAndClearUpdateRect());
			return true;
		}
		return false;
	case EM_MOUSE_DOWN:
{
		m_opView -> GetMouse(vX, vY, vButtonOne, vButtonTwo, vButtonThree);
/*
char xxx[30];
sprintf(xxx, "(%d, %d)", vX, vY);
m_opToolTip -> SetLabel(xxx);
m_opToolTip -> SetPosition(vX, vY);
m_opToolTip -> Show();
*/
		MouseDown(vX, vY, vButtonOne, vButtonTwo, vButtonThree);
		EnableMenuItems();
		return true;
}
	case EM_MOUSE_UP:
{
		m_opView -> GetMouse(vX, vY, vButtonOne, vButtonTwo, vButtonThree);
/*
char xxx[30];
sprintf(xxx, "(%d, %d)", vX, vY);
m_opToolTip -> SetLabel(xxx);
m_opToolTip -> SetPosition(vX, vY);
m_opToolTip -> Show();
*/
		MouseUp(vX, vY, vButtonOne, vButtonTwo, vButtonThree);
		EnableMenuItems();
		return true;
}
	case EM_MOUSE_MOVED:
{
		m_opView -> GetMouse(vX, vY, vButtonOne, vButtonTwo, vButtonThree);
char xxx[30];
sprintf(xxx, "(%d, %d)", vX, vY);
m_opToolTip -> SetLabel(xxx);
m_opToolTip -> SetPosition(vX, vY);
m_opToolTip -> Show();
		MouseMoved(vX, vY, vButtonOne, vButtonTwo, vButtonThree);
		return true;
}
	case EM_DOUBLE_CLICK:
		m_opView -> GetMouse(vX, vY, vButtonOne, vButtonTwo, vButtonThree);
		MouseDoubleClicked(vX, vY, vButtonOne, vButtonTwo, vButtonThree);
		return true;
	case EM_SETTING_UPDATED:
eo << "TrackView: EM_SETTING_UPDATED" << ef;
		if(m_opView -> LockWindow())
		{
			float vTempo = *(static_cast<float*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_TEMPO)));
			if(vTempo != m_vTempo)
			{
				list<EMGClip*> oClips(*(EMGClipRepository::Instance() -> GetClips(EM_TYPE_MIDI)));
				if(*(static_cast<bool*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_AUDIO_LOCKED_TO_TEMPO))))
				{
					list<EMGClip*> oAudioClips(*(EMGClipRepository::Instance() -> GetClips(EM_TYPE_ANY_AUDIO)));
					oClips.merge(oAudioClips);
				}
				EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_SET_PROJECT_BPM, &vTempo);
				EMGClipRepository::Instance() -> MoveClipsToTempo(&oClips, m_vTempo, vTempo);
				m_vTempo = vTempo;
				m_opView -> Invalidate(m_opView -> Bounds());
			}

			vTimeZoomScale = *(static_cast<int64*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_TIME_ZOOM_SCALE)));
			if(vTimeZoomScale != m_vTimeZoomScale)
			{
				m_vOldTimeZoomScale = m_vTimeZoomScale;
				m_vTimeZoomScale = vTimeZoomScale;
				UpdateTimeScale();
			}

			m_vRelative = *(static_cast<bool*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_RELATIVE)));

			m_vSnapping = *(static_cast<bool*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_SNAPPING)));

			string* opSnappingMode = static_cast<string*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_SNAPPING_MODE));
			if(opSnappingMode != m_opSnappingMode)
			{
				m_opSnappingMode = opSnappingMode;
				m_opView -> Invalidate(m_opView -> Bounds());
			}

			opSignature = static_cast<string*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_SIGNATURE));
			if(opSignature != m_opSignature)
			{
				m_opSignature = opSignature;
				sscanf(m_opSignature -> c_str(), "%d/%d", &m_vSignatureMajor, &m_vSignatureMinor);
			}

			eEditMode = static_cast<EMEditMode>(*(static_cast<int*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_EDIT_MODE))));
			if(eEditMode != m_eEditMode)
			{
				ChangeEditMode(eEditMode);
				m_opView -> Invalidate(m_opView -> Bounds());
			}

			int64 vRegionBegin = *(static_cast<int64*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_REGION_BEGIN)));
			int64 vRegionEnd = *(static_cast<int64*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_REGION_END)));
			if(vRegionBegin != m_vRegionBegin || vRegionEnd != m_vRegionEnd)
			{
				m_opView -> Invalidate(EMRect(m_vRegionBegin / m_vTimeZoomScale, 0, m_vRegionEnd / m_vTimeZoomScale, (m_opView -> Bounds()).m_vBottom) | EMRect(vRegionBegin / m_vTimeZoomScale, 0, vRegionEnd / m_vTimeZoomScale, (m_opView -> Bounds()).m_vBottom));
				m_vRegionBegin = vRegionBegin;
				m_vRegionEnd = vRegionEnd;
;//cout_commented_out_4_release << "m_vRegionBegin: " << m_vRegionBegin << ", m_vRegionEnd: " << m_vRegionEnd << endl;
			}

			m_eToolboxMode = static_cast<ToolboxMode>(*(static_cast<int*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_TOOLBOX_MODE))));

			float vFramerate = *(static_cast<float*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_VIDEO_FRAMERATE)));
			if(vFramerate != m_vFramerate)
			{
				m_vFramerate = vFramerate;
				// Update all video clips with the EM_new framerate
				list<EMGClip*>* opClips = EMGClipRepository::Instance() -> GetClips(EM_TYPE_ANY_VIDEO);
				list<EMGClip*>::iterator oIterator = opClips -> begin();
				while(oIterator != opClips -> end())
				{
					(dynamic_cast<VideoGClip*>(*oIterator)) -> SetFramerate(vFramerate);
					oIterator++;
				}
				m_opView -> Invalidate(m_opView -> Bounds());
			}

			bool vDrawClipContent = *(static_cast<bool*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_VIEW_CLIP_CONTENT_ENABLED)));
			if(vDrawClipContent != m_vDrawClipContent)
			{
				m_vDrawClipContent = vDrawClipContent;
				m_opView -> Invalidate(m_opView -> Bounds());
			}
			
			EnableMenuItems();

			m_opView -> UnlockWindow();
		}
		return true;
	case EM_DROP:
		if(p_opSender == m_opView)
		{
			EMDragDropData* opData = EMDragDrop::Instance() -> GetData();
			if(m_opView ->LockWindow())
			{
				int32 vAudioTrackIndex = -1;
				int32 vVideoTrackIndex = -1;
				EMGTrack* opAudioTrack  = NULL;
				EMGTrack* opVideoTrack  = NULL;
				int32 vAudioMediaClipID = NULL;
				int32 vVideoMediaClipID = NULL;
				int32 vEntryIDs[2];

				string oFullPath = opData -> m_opString.c_str();
				if(oFullPath.find_first_of('|') != -1)
				{
					oFullPath.erase(0, oFullPath.find_first_of('|') + 1);
					EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_GET_ENTRIES_FROM_MEDIA_POOL_VIEW, const_cast<char*>(oFullPath.c_str()), vEntryIDs, reinterpret_cast<void*>(false));
				}
				else
				{
					if(*static_cast<bool*>(EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_GET_ENTRIES_FROM_MEDIA_POOL_VIEW, const_cast<char*>(oFullPath.c_str()), vEntryIDs, reinterpret_cast<void*>(true))))
						EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_ADD_TO_MEDIA_POOL_VIEW, const_cast<char*>(oFullPath.c_str()), vEntryIDs, NULL);
				}
				int64 vPos((*static_cast<int64*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_TIME_ZOOM_SCALE)) * opData -> m_vMouseX));

				if(vEntryIDs[0] != -1)
				{
					//EMImageBufferRepository::Instance() -> AudioFileAdded(vEntryIDs[0]);
					opAudioTrack = EMGTrackRepository::Instance() -> GetClosestTrack(opData -> m_vMouseX, opData -> m_vMouseY, EM_TYPE_ANY_AUDIO, 0, 0);
					if(opAudioTrack == NULL)
					{
						EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_ADD_AUDIO_TRACK);
						opAudioTrack = EMGTrackRepository::Instance() -> GetClosestTrack(opData -> m_vMouseX, opData -> m_vMouseY, EM_TYPE_ANY_AUDIO, 0, 0);
					}
					vAudioTrackIndex = EMGTrackRepository::Instance() -> GetTrackIndex(opAudioTrack);
		
					int32 vAudioMediaTrackID = opAudioTrack -> GetTrackInfo() -> GetMediaTrackID();
					if(vEntryIDs[1] != -1)
						vPos = EMGUIUtilities::Instance() -> TruncateFrame(vPos);
					vAudioMediaClipID = *static_cast<int32*>(EMCommandRepository::Instance() ->ExecuteCommand(MEDIA_COMMAND_DROP_MEDIA, &vEntryIDs[0],  &vAudioMediaTrackID, &vPos));
					if(vAudioMediaClipID == -1)
						EMDebugger("Could not drop audio clip");
					else
					{
						EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_ADD_G_AUDIO_CLIP, &vAudioMediaClipID, &vAudioTrackIndex, &vPos);
						EMGClipRepository::Instance() -> Notify(EM_CLIP_REPOSITORY_UPDATED);
						MediaPoolView::Instance() -> ChangeInstances(MediaPoolView::Instance() -> GetIndexID(static_cast<const char*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_ENTRY_FILENAME, &vEntryIDs[0]))));
					}
				}
				if(vEntryIDs[1] != -1)
				{
//					int64 vPos((*static_cast<int64*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_TIME_ZOOM_SCALE)) * opData -> m_vMouseX));
					int64 vLength(*(static_cast<int64*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_ENTRY_DURATION, &vEntryIDs[1]))));
					//EMImageBufferRepository::Instance() -> VideoFileAdded(vEntryIDs[1]);
					opVideoTrack = EMGTrackRepository::Instance() -> GetClosestTrack(opData -> m_vMouseX, opData -> m_vMouseY, EM_TYPE_ANY_VIDEO, vPos, vLength);
					if(opVideoTrack == NULL)
					{
						eo << "No track found or not enough space to drop clip. First time" << ef;
						EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_ADD_VIDEO_TRACK);
						opVideoTrack = EMGTrackRepository::Instance() -> GetClosestTrack(opData -> m_vMouseX, opData -> m_vMouseY, EM_TYPE_ANY_VIDEO, vPos, vLength);
					}
					vVideoTrackIndex = EMGTrackRepository::Instance() -> GetTrackIndex(opVideoTrack);
		
					int32 vVideoMediaTrackID = opVideoTrack -> GetTrackInfo() -> GetMediaTrackID();

					vVideoMediaClipID = *static_cast<int32*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_DROP_MEDIA, &vEntryIDs[1],  &vVideoMediaTrackID, &vPos));
					if(vVideoMediaClipID == -1)
						EMDebugger("Could not drop video clip");
					else
					{
						if(EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_ADD_G_VIDEO_CLIP, &vVideoMediaClipID, &vVideoTrackIndex, &vPos) == NULL)
						{
							EMDebugger("How did you get here? Better do something about it.");
						/*	eo << "No track found or not enough space to drop clip. Second time" << ef;
							EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_ADD_VIDEO_TRACK);
							opVideoTrack = EMGTrackRepository::Instance() -> GetClosestTrack(vMouseX, vMouseY, EM_TYPE_ANY_VIDEO, vPos, vLength);
							vVideoTrackIndex = EMGTrackRepository::Instance() -> GetTrackIndex(opVideoTrack);
							EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_ADD_G_VIDEO_CLIP, &vVideoMediaClipID, &vVideoTrackIndex, &vPos);
							*/
						}
						EMGClipRepository::Instance() -> Notify(EM_CLIP_REPOSITORY_UPDATED);
						MediaPoolView::Instance() -> ChangeInstances(MediaPoolView::Instance() -> GetIndexID(static_cast<const char*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_ENTRY_FILENAME, &vEntryIDs[1]))));
					}
				}
				if(opAudioTrack && opVideoTrack)
				{
					list<EMGClip*> oClipsToGroup;
					oClipsToGroup.push_back(EMGClipRepository::Instance() -> GetClip(vAudioMediaClipID));
					oClipsToGroup.push_back(EMGClipRepository::Instance() -> GetClip(vVideoMediaClipID));
					EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_GROUP_CLIPS_FROM_LIST, &oClipsToGroup, NULL, NULL);
				}
				m_opView -> UnlockWindow();
			}
			return true;
		}
		else
			return false;
	default:
		return false;
	}
}

void TrackView::MouseDoubleClicked(int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree)
{
	GetView() -> AssertWindowLocked();

	if(m_eToolboxMode != TOOLBOX_SELECT)
		return;

	EMGTrack* opMouseDownTrack = EMGTrackRepository::Instance() -> GetTrack(p_vX, p_vY);
	if(opMouseDownTrack == NULL)
		return;

	EMGClip* opClip = opMouseDownTrack -> GetClipContainer() -> GetClip(p_vX, p_vY);

	if(m_eEditMode == EM_TRACK_EDIT_MODE)
	{
		if(opClip != NULL)
		{
			uint32 vModifiers = EMKeyboard::Instance() -> GetModifiers();
			if(opClip -> GetType() == EM_TYPE_TRANSITION)
			{
				m_opView -> Notify(EM_MOUSE_UP);
				EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_DISPLAY_TRANSITION_DIALOG, opClip);
			}
			else if((vModifiers & EM_SHIFT_KEY) == EM_SHIFT_KEY) // Jumps into FX edit mode
			{
				if(opClip -> GetType() == EM_TYPE_ANY_AUDIO)
					dynamic_cast<AudioGClipContainer*>(opMouseDownTrack -> GetClipContainer()) -> SetCurrentFXChain((opClip -> GetLabel()).c_str());
				else
					dynamic_cast<VideoGClipContainer*>(opMouseDownTrack -> GetClipContainer()) -> SetCurrentFXChain((opClip -> GetLabel()).c_str());
				EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_SET_FX_EDIT);
			}
			else // Jumps into clip edit mode
			{
				if(!(opMouseDownTrack -> GetTrackInfo() -> IsSelected()))
					opMouseDownTrack -> GetTrackInfo() -> SetSelected(true);
				EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_SET_CLIP_EDIT);
			}
		}
	}

	else// if(m_eEditMode == EM_CLIP_EDIT_MODE)
	{
//		if(!(opMouseDownTrack -> GetTrackInfo() -> IsHidden(m_eEditMode == EM_CLIP_EDIT_MODE)))
			opMouseDownTrack -> GetClipContainer() -> MouseDoubleClicked(m_opView, p_vX, p_vY, p_vButtonOne, p_vButtonTwo, p_vButtonThree, m_eEditMode, opClip);
	}
}

void TrackView::MouseDown(int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree)
{
	if(!m_oRectVisible.Contains(p_vX, p_vY))
		return;

	if(p_vButtonTwo && m_eEditMode != EM_FX_EDIT_MODE)
	{
		m_oToolbox.Show(m_opView);
		return;
	}
	
	m_vMouseButtonPressed = true;
	
	bool vSomethingChanged = false;

	m_vMouseDownX = p_vX;
	m_vMouseDownY = p_vY;
	m_vMousePosX = p_vX;
	m_vMousePosY = p_vY;
	
	m_opMouseDownTrack = EMGTrackRepository::Instance() -> GetTrack(p_vX, p_vY);
	if(m_opMouseDownTrack != NULL)
	{
		m_opMouseDownClip = m_opMouseDownTrack -> GetClipContainer() -> GetClip(p_vX, p_vY);
//if(m_opMouseDownClip != NULL)
//eo << "m_opMouseDownClip != NULL" << ef;
	}
	else
		m_opMouseDownClip = NULL;

	if(m_opMouseDownClip != NULL)
	{
		if(m_opMouseDownClip -> GetFirstInGroup() != NULL)
		{
			bool vHiddenClipsExist;
			EMGUIUtilities::Instance() -> GetClipGroup(m_opMouseDownClip, &vHiddenClipsExist);
			if(vHiddenClipsExist && (m_eToolboxMode == TOOLBOX_SELECT || m_eToolboxMode == TOOLBOX_ERASE))
			{
				EMMessageBox* opMessageBox = EMFactory::Instance() -> CreateMessageBox();
				opMessageBox -> DisplayDialog(NULL, "This clip can not be selected or deleted since\nit is grouped with one or more hidden clips.", "Hidden Clips In Group", EM_DIALOG_BUTTON_OK);
				m_opView -> Notify(EM_MOUSE_UP);
				return;
			}
		}
	}

	if(m_opMouseDownTrack != NULL)
	{
		if(m_eToolboxMode == TOOLBOX_RENAME)
		{
			if(m_opMouseDownClip != NULL)
			{
				EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_DISPLAY_CHANGE_CLIP_NAME_DIALOG, m_opMouseDownClip, NULL, NULL);
				m_opView -> Invalidate(m_opMouseDownClip -> Frame());
			}

			return;
		}

		if(m_eToolboxMode == TOOLBOX_ERASE && (m_eEditMode == EM_TRACK_EDIT_MODE || (m_opMouseDownTrack -> GetTrackInfo() -> GetType() != EM_TYPE_MIDI && 
			m_opMouseDownTrack -> GetTrackInfo() -> GetType() != EM_TYPE_ANY_AUDIO)))
		{
			if(m_opMouseDownClip != NULL)
			{
				list<EMGClip*> oClipList;
				if(m_opMouseDownClip -> GetFirstInGroup() != NULL)
				{
					EMGClip* opClip = m_opMouseDownClip; // Have to do this, since m_opMouseDownClip is set to NULL when m_opView -> Notify(EM_MOUSE_UP)
					if(m_eEditMode == EM_CLIP_EDIT_MODE)
					{
						EMMessageBox* opMessageBox = EMFactory::Instance() -> CreateMessageBox();
						EM_MESSAGE_BOX_RESULT eResult = static_cast<EM_MESSAGE_BOX_RESULT>(opMessageBox ->DisplayDialog(NULL, "You are trying to delete a clip that is grouped with other clips.\nDo you want to proceed and delete all clips in the group?", "Delete Grouped Clips", EM_DIALOG_BUTTON_OK_CANCEL));
						m_opView -> Notify(EM_MOUSE_UP);
						if(eResult == EM_DIALOG_CANCEL_BUTTON)
							return;
					}
					bool vHiddenClipsExist;
					oClipList = EMGUIUtilities::Instance() -> GetClipGroup(opClip, &vHiddenClipsExist);
				}
				else
					oClipList.push_back(m_opMouseDownClip);
				EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_DELETE_CLIPS_FROM_LIST, &oClipList);
			}
			return;
		}
		
		else if(m_eToolboxMode == TOOLBOX_CUT_PASTE && m_opMouseDownTrack -> GetTrackInfo() -> GetType() != EM_TYPE_MIDI && m_opMouseDownTrack -> GetTrackInfo() -> GetType() != EM_TYPE_TRANSITION)
		{
			if(m_opMouseDownClip != NULL)
			{
				EMRect oClipRect = m_opMouseDownClip -> Frame();
				list<EMGClip*> oClips;
				oClips.push_back(m_opMouseDownClip);
				int64 vPosition = static_cast<int64>(p_vX * m_vTimeZoomScale);
				EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_CUT_CLIPS, &oClips, &vPosition);
				m_opView -> Invalidate(oClipRect);
			}
			return;
		}
	}

	// If not in track edit mode, the tracks and clips contain all logic for all mouse operations
	if(m_eEditMode != EM_TRACK_EDIT_MODE)//== EM_FX_EDIT_MODE)
	{
		if(m_opMouseDownTrack == NULL)
			return;
//		if(!(opMouseDownTrack -> GetTrackInfo() -> IsHidden(m_eEditMode == EM_CLIP_EDIT_MODE)))
			m_opMouseDownTrack -> GetClipContainer() -> MouseDown(m_opView, p_vX, p_vY, p_vButtonOne, p_vButtonTwo, p_vButtonThree, m_eEditMode);
		return;
	}
	
	if(m_eToolboxMode == TOOLBOX_SELECT)
	{
		/*
		if(m_eEditMode == EM_CLIP_EDIT_MODE)
		{
			if(m_opMouseDownClip != NULL)
				m_opMouseDownClip -> MouseDown(m_opView, p_vX, p_vY, p_vButtonOne, p_vButtonTwo, p_vButtonThree);
			return;
		}
		*/
		uint32 vModifiers = EMKeyboard::Instance() -> GetModifiers();

		if(m_opMouseDownClip == NULL || (vModifiers & EM_SHIFT_KEY) == EM_SHIFT_KEY)
		{
			m_vSelectionBoxExists = true;
			m_oSelectionBox = EMRect(m_vMouseDownX, m_vMouseDownY, m_vMouseDownX, m_vMouseDownY);
			m_oOldSelectionBox = EMRect();
			DrawSelectionBox(EMRect(), false);
//			return;
		}
/*
		EMGTrack* opTrack = EMGTrackRepository::Instance() -> GetTrack(p_vX, p_vY);
		EMGClip* opClip = NULL;
		if(opTrack != NULL)
			opClip = opTrack -> GetClip(p_vX, p_vY);
*/
		if((vModifiers & EM_COMMAND_KEY) == EM_COMMAND_KEY)
		{
//			int64 vPosition = TranslateToTics(p_vX);
//			EMMediaEngine::Instance() -> MediaClipCommand(EM_SEEK, vPosition);
			return;
		}

		EMRect oOldBounds = EMGClipRepository::Instance() -> SelectedFrame();

		if((vModifiers & EM_SHIFT_KEY) == EM_SHIFT_KEY)
		{
			if(m_opMouseDownClip == NULL)
				return;
//			EMCommandRepository::Instance() -> ExecuteCommand(UNDO_COMMAND_CHANGE_CLIP_SELECTION, EMGClipRepository::Instance() -> GetSelectedClips(), NULL, NULL);

			bool vHiddenClipsExist;
			list<EMGClip*> oClips = EMGUIUtilities::Instance() -> GetClipGroup(m_opMouseDownClip, &vHiddenClipsExist);
			/*
			if(vHiddenClipsExist)
			{
				EMFactory::Instance() -> CreateMessageBox() ->DisplayDialog(NULL, "This clip is grouped with at least one clip from a hidden track. It cannot be selected unless that track is un-hidden.", "Clip Selection Information", EM_DIALOG_BUTTON_OK | EM_DIALOG_ICON_INFOMATION);
				return;
			}
			*/
			if(m_opMouseDownClip -> GetSelected())
				EMGClipRepository::Instance() -> SetSelected(&oClips, false);
			else
				EMGClipRepository::Instance() -> SetSelected(&oClips, true);

			char vMessage[80];
			sprintf(vMessage, "%ld clips selected", EMGClipRepository::Instance() -> GetNrSelectedClips());
			TrackWindowStatusBar::Instance() -> WriteMessage(string(vMessage));
			vSomethingChanged = true;
		}		

		else if(m_opMouseDownClip == NULL)
		{
			if(EMGClipRepository::Instance() -> GetNrSelectedClips() > 0)
			{
				vSomethingChanged = true;
//				EMCommandRepository::Instance() -> ExecuteCommand(UNDO_COMMAND_CHANGE_CLIP_SELECTION, EMGClipRepository::Instance() -> GetSelectedClips(), NULL, NULL);
				EMGClipRepository::Instance() -> ClearSelectedClips();
				char vMessage[80];
				sprintf(vMessage, "No clips selected");
				TrackWindowStatusBar::Instance() -> WriteMessage(string(vMessage));
			}
		}		
	
		else if(!(m_opMouseDownClip -> GetSelected()))
		{
//			EMCommandRepository::Instance() -> ExecuteCommand(UNDO_COMMAND_CHANGE_CLIP_SELECTION, EMGClipRepository::Instance() -> GetSelectedClips(), NULL, NULL);
			EMGClipRepository::Instance() -> ClearSelectedClips();

			bool vHiddenClipsExist;
			list<EMGClip*> oClips = EMGUIUtilities::Instance() -> GetClipGroup(m_opMouseDownClip, &vHiddenClipsExist);
			/*
			if(vHiddenClipsExist)
			{
				EMFactory::Instance() -> CreateMessageBox() ->DisplayDialog(NULL, "This clip is grouped with at least one clip from a hidden track. It cannot be selected unless that track is un-hidden.", "Clip Selection Information", EM_DIALOG_BUTTON_OK | EM_DIALOG_ICON_INFOMATION);
				return;
			}
			*/
			EMGClipRepository::Instance() -> SetSelected(&oClips, true);
			char vMessage[80];
			sprintf(vMessage, "%ld clips selected", EMGClipRepository::Instance() -> GetNrSelectedClips());
			TrackWindowStatusBar::Instance() -> WriteMessage(string(vMessage));
			vSomethingChanged = true;
		}	

//		EMGClipRepository::Instance() -> MovedBounds() = EMGClipRepository::Instance() -> SelectedBounds();

		if(!vSomethingChanged)
			return; // Nothing needs to be redrawn
		
		if(!(oOldBounds.IsValid()))
			m_opView -> Invalidate(EMGClipRepository::Instance() -> SelectedFrame());
		else if(!(EMGClipRepository::Instance() -> SelectedFrame().IsValid()))
			m_opView -> Invalidate(oOldBounds);
		else
			m_opView -> Invalidate(oOldBounds | EMGClipRepository::Instance() -> SelectedFrame());
	}
/*	
	else if(m_eToolboxMode == TOOLBOX_RENAME)
	{
		if(m_opMouseDownClip != NULL)
		{
			EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_DISPLAY_CHANGE_CLIP_NAME_DIALOG, m_opMouseDownClip, NULL, NULL);
			m_opView -> Invalidate(m_opMouseDownClip -> Frame());
		}
		return;
	}

	else if(m_eToolboxMode == TOOLBOX_ERASE)
	{
		if(m_opMouseDownClip != NULL)
		{
			list<EMGClip*>* opClipList = EM_new list<EMGClip*>;
			opClipList -> push_back(m_opMouseDownClip);
			EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_DELETE_CLIPS_FROM_LIST, opClipList);
		}
		return;
	}
*/	
	else if(m_eToolboxMode == TOOLBOX_RESIZE)
	{
		if(m_opMouseDownClip != NULL)
			m_opMouseDownClip -> MouseDown(m_opView, p_vX, p_vY, p_vButtonOne, p_vButtonTwo, p_vButtonThree);
		return;
	}
	
	else if(m_eToolboxMode == TOOLBOX_CUT_PASTE)
	{
		if(m_opMouseDownClip != NULL)
		{
			if(m_opMouseDownClip -> GetType() == EM_TYPE_MIDI || m_opMouseDownClip -> GetType() == EM_TYPE_TRANSITION)
				return;

			EMRect oClipRect = m_opMouseDownClip -> Frame();
			list<EMGClip*> oClips;
			oClips.push_back(m_opMouseDownClip);
			int64 vPosition = static_cast<int64>(p_vX * m_vTimeZoomScale);
			EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_CUT_CLIPS, &oClips, &vPosition);
			m_opView -> Invalidate(oClipRect);
		}
		return;
	}
	/*
	else if(m_eToolboxMode == TOOLBOX_MUTE)
	{
		if(m_opMouseDownClip != NULL)
		{
			list<EMGClip*> oClips;
			oClips.push_back(m_opMouseDownClip);
			EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_MUTE_CLIPS, &oClips);
		}
		return;
	}
	*/
	else if(m_eToolboxMode == TOOLBOX_DRAW)
	{
		if(m_opMouseDownTrack != NULL && m_opMouseDownClip == NULL)
			if(m_opMouseDownTrack -> GetTrackInfo() -> GetType() == EM_TYPE_TRANSITION)
			{
				m_vSnappedMouseDownPos = m_vMouseDownX * m_vTimeZoomScale;
				EMGUIUtilities::Instance() -> GetSnapPosition(&m_vSnappedMouseDownPos, true, true, NULL, true, true);

				EMRect oTrackFrame = m_opMouseDownTrack -> GetClipContainer() -> Frame();
				m_oGhostClipRect = EMRect(m_vSnappedMouseDownPos / m_vTimeZoomScale, oTrackFrame.m_vTop, m_vSnappedMouseDownPos / m_vTimeZoomScale, oTrackFrame.m_vBottom - m_opMouseDownTrack -> GetClipContainer() -> GetLowerBorderSize());
			}
	}

	// This tool is temporarily used as a clip info tool for debugging
	else if(m_eToolboxMode == TOOLBOX_SCRUB)
	{
		if(m_opMouseDownClip == NULL)
		{
eo << "-----------------------------------------------------------------------------------------------------------" << ef;
eo << "  Mouse down (X): " << m_vMouseDownX << " us, snapped position: " << (double) m_vSnappedMouseDownPos << ef;
eo << "  Mouse down (Y): " << m_vMouseDownY << " pixels" << ef;
if(m_opMouseDownTrack != NULL)
{
int32 vMediaTrackID = m_opMouseDownTrack -> GetTrackInfo() -> GetMediaTrackID();
if(m_opMouseDownTrack -> GetTrackInfo() -> GetType() == EM_TYPE_ANY_AUDIO || m_opMouseDownTrack -> GetTrackInfo() -> GetType() == EM_TYPE_ANY_VIDEO)
{
int32 vEffectTrackID = *(static_cast<int32*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_EFFECT_TRACK_ID, &vMediaTrackID)));
eo << "  Track: \"" << (m_opMouseDownTrack -> GetTrackInfo() -> GetTrackName()).c_str() << "\" with ID: " << vMediaTrackID << ", fx track ID: " << vEffectTrackID << ef;
}
else
eo << "  Track: \"" << (m_opMouseDownTrack -> GetTrackInfo() -> GetTrackName()).c_str() << "\" with ID: " << vMediaTrackID << ef;
}
eo << "-----------------------------------------------------------------------------------------------------------" << ef;
			return;
		}
eo << "-----------------------------------------------------------------------------------------------------------" << ef;
eo << "  Clip info for clip: " << (m_opMouseDownClip -> GetLabel()).c_str() << ", pointer: " << (uint32) m_opMouseDownClip << ef;
eo << "  Type: ";
if(m_opMouseDownClip -> GetType() == EM_TYPE_ANY_AUDIO)
eo << "Audio" << ef;
else if(m_opMouseDownClip -> GetType() == EM_TYPE_ANY_VIDEO)
eo << "Video" << ef;
else
eo << "Unknown" << ef;
int32 vMediaID = m_opMouseDownClip -> GetMediaClipID();
eo << "  Meida clip ID: " << vMediaID << ef;
int64 vClipMarkIn = *(static_cast<int64*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_CLIP_MARK_IN, &vMediaID)));
int64 vClipMarkOut = *(static_cast<int64*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_CLIP_MARK_OUT, &vMediaID)));
int64 vClipPosition = *(static_cast<int64*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_CLIP_POSITION, &vMediaID)));
int64 vClipLength = *(static_cast<int64*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_CLIP_DURATION, &vMediaID)));
if(vClipPosition != m_opMouseDownClip -> GetPosition())
eo << "  Clip position uncertain: media command position: " << (int32) vClipPosition << ", cached position: " << (int32) m_opMouseDownClip -> GetPosition() << ef;
if(vClipLength != m_opMouseDownClip -> GetLength())
eo << "  Clip length uncertain: media command length: " << (int32) vClipLength << ", cached length: " << (int32) m_opMouseDownClip -> GetLength() << ef;
else
eo << "  Position: " << (double) m_opMouseDownClip -> GetPosition() << ef;
eo << "  Position (int32): " << (int32) m_opMouseDownClip -> GetPosition() << ef;
eo << "  Length: " << (double) m_opMouseDownClip -> GetLength() << ef;
eo << "  Length (int32): " << (int32) m_opMouseDownClip -> GetLength() << ef;
eo << "  End pos: " << (double) m_opMouseDownClip -> GetPosition() + m_opMouseDownClip -> GetLength() - 1 << ef;
eo << "  End pos (int32): " << (int32) (m_opMouseDownClip -> GetPosition() + m_opMouseDownClip -> GetLength() - 1) << ef;
eo << "  Mark in: " << (double) vClipMarkIn << ef;
eo << "  Mark out: " << (double) vClipMarkOut << ef;
if(m_opMouseDownClip -> GetType() == EM_TYPE_ANY_AUDIO || m_opMouseDownClip -> GetType() == EM_TYPE_ANY_VIDEO)
{
EMImageFileInfo oInfo = EMImageBufferRepository::Instance() -> GetFileInfo(m_opMouseDownClip);
eo << "  Media file ID: " << oInfo.m_vFileID << ef;
eo << "  Media file length: " << (double) oInfo.m_vFileLength << ef;
eo << "  Number of chunks in file: " << (double) oInfo.m_vNrChunks << ef;
}
eo << "  Frame: ";
(m_opMouseDownClip -> Frame()).Print();
eo << "  Visible frame: ";
(m_opMouseDownClip -> Frame() & m_oRectVisible).Print();
eo << "  Selected: " << m_opMouseDownClip -> GetSelected() << ef;
EMGTrack* opTrack = m_opMouseDownClip -> GetTrack();
eo << "  Belongs to track \"" << (opTrack -> GetTrackInfo() -> GetTrackName()).c_str() << "\" with ID: " << opTrack -> GetTrackInfo() -> GetMediaTrackID() << ef;
eo << "First in group: " << (uint32) m_opMouseDownClip -> GetFirstInGroup() << ", next in group: " << (uint32) m_opMouseDownClip -> GetNextInGroup() << ef;
eo << "-----------------------------------------------------------------------------------------------------------" << ef;

return;
	}	
}

void TrackView::MouseMoved(int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree)
{
//eo << "TrackView::MouseMoved: x: " << p_vX << ", y: " << p_vY << ", buttons: " << p_vButtonOne << p_vButtonTwo << p_vButtonThree << ef;
	GetView() -> AssertWindowLocked();

	if(m_vMouseButtonPressed != p_vButtonOne) // Probably a pop-up window that interrupted a mouse down
	{
		MouseUp(p_vX, p_vY, p_vButtonOne, p_vButtonTwo, p_vButtonThree);
	}

	if(p_vX < 0)
		p_vX = 0;
	else if(p_vX * m_vTimeZoomScale > MAX_SCROLLABLE_TIME) // This should not really happen
		p_vX = MAX_SCROLLABLE_TIME / m_vTimeZoomScale;

	m_vMousePosX = p_vX;
	m_vMousePosY = p_vY;
	int64 vMouseXPos = p_vX * m_vTimeZoomScale;

	if(m_eToolboxMode == TOOLBOX_ERASE)
	{
		m_opView -> SetMouseCursor(EM_MOUSE_CURSOR_DELETE); // FIXME: Other object may set this cursor as well to the same type (unnecessary)

		EMGTrack* opTrack = EMGTrackRepository::Instance() -> GetTrack(p_vX, p_vY);
		if(opTrack != NULL)
			if(m_eEditMode == EM_TRACK_EDIT_MODE || (opTrack -> GetTrackInfo() -> GetType() != EM_TYPE_MIDI && 
				opTrack -> GetTrackInfo() -> GetType() != EM_TYPE_ANY_AUDIO))
			{
				EMGClip* opClip = opTrack -> GetClipContainer() -> GetClip(p_vX, p_vY);
				if(opClip != NULL)
				{
//eo << "TrackView::MouseMoved - clip found: " << opClip -> GetMediaClipID() << ef;
					if(!m_vMouseButtonPressed)
						return;

					list<EMGClip*> oClipList;
					if(opClip -> GetFirstInGroup != NULL)
					{
						if(m_eEditMode == EM_CLIP_EDIT_MODE)
						{
							EMMessageBox* opMessageBox = EMFactory::Instance() -> CreateMessageBox();
							EM_MESSAGE_BOX_RESULT eResult = static_cast<EM_MESSAGE_BOX_RESULT>(opMessageBox -> DisplayDialog(NULL, "You are trying to delete a clip that is grouped with other clips.\nDo you want to proceed and delete all clips in the group?", "Delete Grouped Clips", EM_DIALOG_BUTTON_OK_CANCEL));
							m_opView -> Notify(EM_MOUSE_UP);
							if(eResult == EM_DIALOG_CANCEL_BUTTON)
								return;
						}
						bool vHiddenClipsExist;
						oClipList = EMGUIUtilities::Instance() -> GetClipGroup(opClip, &vHiddenClipsExist);
					}
					else
						oClipList.push_back(opClip);

					EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_DELETE_CLIPS_FROM_LIST, &oClipList);
					return;
				}
			}
	}
	
	if(m_eEditMode != EM_TRACK_EDIT_MODE)
	{
		EMGTrack* opTrack;
		for(uint32 vIndex = 0; vIndex < EMGTrackRepository::Instance() -> GetNrTracks(); vIndex++)
		{
			opTrack = EMGTrackRepository::Instance() -> GetTrack(vIndex);
			if(!(opTrack -> GetTrackInfo() -> IsHidden(m_eEditMode == EM_CLIP_EDIT_MODE)))
				opTrack -> GetClipContainer() -> MouseMoved(m_opView, p_vX, p_vY, p_vButtonOne, p_vButtonTwo, p_vButtonThree, m_eEditMode);
		}

		return;
	}
	
	if(p_vY < 0)
	{
		p_vY = 0;
		m_vMousePosY = p_vY;
	}

	if(m_eToolboxMode == TOOLBOX_SELECT)
	{
		if(!m_vMouseButtonPressed)
			return;

		if(m_vMouseButtonPressed && m_vSelectionBoxExists)
		{
			if(p_vX < m_vMouseDownX)
			{
				if(p_vY < m_vMouseDownY)
					m_oSelectionBox = EMRect(p_vX, p_vY, m_vMouseDownX, m_vMouseDownY);
				else
					m_oSelectionBox = EMRect(p_vX, m_vMouseDownY, m_vMouseDownX, p_vY);
			}
			else
			{
				if(p_vY < m_vMouseDownY)
					m_oSelectionBox = EMRect(m_vMouseDownX, p_vY, p_vX, m_vMouseDownY);
				else
					m_oSelectionBox = EMRect(m_vMouseDownX, m_vMouseDownY, p_vX, p_vY);
			}
			m_oSelectionBox = m_oSelectionBox & m_oRectScrollable;
			DrawSelectionBox(EMRect(), false);
			return;
		}
		
		if(m_opMouseDownClip == NULL) // The mouse cursor is not positioned over a clip
			return;

		if(p_vButtonOne) // If button one is pressed
		{
			//if(p_vX < 0)
			//	m_vMouseOffsetX = -1 * m_vMouseDownX;
			//else
				m_vMouseOffsetX = p_vX - m_vMouseDownX;
			//if(p_vY < 0)
			//	m_vMouseOffsetY = -1 * m_vMouseDownY;
			//else
				m_vMouseOffsetY = p_vY - m_vMouseDownY;

			m_vMouseButtonPressed = true;
		
			EMRect oOldMovedRect = EMGClipRepository::Instance() -> MovedFrame();
			m_vPositionMovedClipOffset = static_cast<int64>(m_vMouseOffsetX) * m_vTimeZoomScale;
			AdjustMoveOffset();
			if(m_vMouseOffsetX != 0 || m_vTrackOffset != 0 || m_vClipsHaveMoved)//EMGClipRepository::Instance() -> MovedFrame() != oOldMovedRect) // Position has changed
			{
				m_vClipsHaveMoved = true;
				m_opView -> Invalidate(oOldMovedRect | EMGClipRepository::Instance() -> MovedFrame());
			}
		
			CalculateAndSetScrollableFrame();
		}

		if(m_vClipsHaveMoved)
		{
			char vMessage[80];
			sprintf(vMessage, "Move offset: %s, %ld", EMGUIUtilities::Instance() -> GetTimeString(m_vPositionMovedClipOffset, EM_SMPTE), m_vTrackOffset);
			TrackWindowStatusBar::Instance() -> WriteMessage(string(vMessage));
		}
	}
	/*
	else if(m_eToolboxMode == TOOLBOX_ERASE)
	{
		m_opView -> SetMouseCursor(EM_MOUSE_CURSOR_DELETE);

		if(!m_vMouseButtonPressed)
			return;
		EMGTrack* opTrack = EMGTrackRepository::Instance() -> GetTrack(p_vX, p_vY);
		EMGClip* opClip = NULL;
		if(opTrack != NULL)
			opClip = opTrack -> GetClipContainer() -> GetClip(p_vX, p_vY);
		if(opClip != NULL)
		{
			list<EMGClip*>* opClipList = EM_new list<EMGClip*>;
			opClipList -> push_back(opClip);
			EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_DELETE_CLIPS_FROM_LIST, opClipList);
		}
		
		return;
	}
	*/

	else if(m_eToolboxMode == TOOLBOX_RESIZE)
	{
		if(m_opMouseDownClip != NULL)
			m_opMouseDownClip -> MouseMoved(m_opView, p_vX, p_vY, p_vButtonOne, p_vButtonTwo, p_vButtonThree);

		else
		{
			EMGTrack* opTrack = EMGTrackRepository::Instance() -> GetTrack(p_vX, p_vY);
			EMGClip* opClip = NULL;
			if(opTrack != NULL)
				opClip = opTrack -> GetClipContainer() -> GetClip(p_vX, p_vY);
			if(opClip != NULL)
				opClip -> MouseMoved(m_opView, p_vX, p_vY, p_vButtonOne, p_vButtonTwo, p_vButtonThree);
			else
				m_opView -> SetMouseCursor(EM_MOUSE_CURSOR_RESIZE_CLIP);
		}
	}

	else if(m_eToolboxMode == TOOLBOX_DRAW)
	{
		m_opView -> SetMouseCursor(EM_MOUSE_CURSOR_DRAW);

		if(!m_vMouseButtonPressed)
			return;

		if(m_oGhostClipRect.IsValid())
		{
			EMGUIUtilities::Instance() -> GetSnapPosition(&vMouseXPos, true, true, NULL, true, true);
			int64 vSnappedMouseMovedX = vMouseXPos / m_vTimeZoomScale;

			m_oOldGhostClipRect = m_oGhostClipRect;
			if(vSnappedMouseMovedX > m_vSnappedMouseDownPos / m_vTimeZoomScale)
			{
				m_oGhostClipRect.m_vLeft = m_vSnappedMouseDownPos / m_vTimeZoomScale;
				m_oGhostClipRect.m_vRight = vSnappedMouseMovedX;
			}
			else
			{
				m_oGhostClipRect.m_vRight = m_vSnappedMouseDownPos / m_vTimeZoomScale;
				m_oGhostClipRect.m_vLeft = vSnappedMouseMovedX;
			}

			m_opView -> Invalidate(m_oGhostClipRect | m_oOldGhostClipRect);
		}
	}

	else if(m_eToolboxMode == TOOLBOX_RENAME)
		m_opView -> SetMouseCursor(EM_MOUSE_CURSOR_RENAME);

	else if(m_eToolboxMode == TOOLBOX_CUT_PASTE)
		m_opView -> SetMouseCursor(EM_MOUSE_CURSOR_CUT_PASTE);
}

void TrackView::MouseUp(int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree)
{
//eo << "TrackView::MouseUp" << ef;
	m_vMouseButtonPressed = false;

	if(p_vX < 0)
		p_vX = 0;
	else if(p_vX * m_vTimeZoomScale > MAX_SCROLLABLE_TIME) // This should not really happen
		p_vX = MAX_SCROLLABLE_TIME / m_vTimeZoomScale;

	if(p_vY < 0)
		p_vY = 0;

	int64 vMouseXPos = p_vX * m_vTimeZoomScale;

	if(m_eEditMode != EM_TRACK_EDIT_MODE)// == EM_FX_EDIT_MODE)
	{
		EMGTrack* opTrack;
		for(uint32 vIndex = 0; vIndex < EMGTrackRepository::Instance() -> GetNrTracks(); vIndex++)
		{
			opTrack = EMGTrackRepository::Instance() -> GetTrack(vIndex);
			opTrack -> GetClipContainer() -> MouseUp(m_opView, p_vX, p_vY, m_eEditMode);
		}

		m_opMouseDownTrack = NULL;
		m_opMouseDownClip = NULL;

		return;
	}
	
	GetView() -> AssertWindowLocked();

	if(m_eToolboxMode == TOOLBOX_SELECT)
	{
		if(m_vSelectionBoxExists)
		{
			list<EMGClip*>* opSelectedClips = EMGClipRepository::Instance() -> GetSelectedClips();
			EMRect oSelectedFrame = EMGClipRepository::Instance() -> SelectedFrame();
			EMRect oUpdateRect = EMGClipRepository::Instance() -> SelectedFrame();
			if(m_oSelectionBox.m_vLeft < m_vMouseDownX)
				EMGClipRepository::Instance() -> SetSelected(m_oSelectionBox, true);
			else
				EMGClipRepository::Instance() -> SetSelected(m_oSelectionBox, false);
			list<EMGClip*> oNewlySelectedClips;
			if(oSelectedFrame != EMGClipRepository::Instance() -> SelectedFrame() || opSelectedClips -> size() != EMGClipRepository::Instance() -> GetSelectedClips() -> size())
			{
//				EMCommandRepository::Instance() -> ExecuteCommand(UNDO_COMMAND_CHANGE_CLIP_SELECTION, EMGClipRepository::Instance() -> GetSelectedClips(), NULL, NULL);
				bool vHiddenClipsExist;
				oNewlySelectedClips = EMGUIUtilities::Instance() -> GetClipGroup(EMGClipRepository::Instance() -> GetSelectedClips(), &vHiddenClipsExist);

				if(vHiddenClipsExist)
				{
					EMMessageBox* opMessageBox = EMFactory::Instance() -> CreateMessageBox();
					opMessageBox ->DisplayDialog(NULL, "These clips can not be selected since\nthey are grouped with one or more hidden clips.", "Hidden Clips In Group", EM_DIALOG_BUTTON_OK);
					m_opView -> Notify(EM_MOUSE_UP);
					EMGClipRepository::Instance() -> ClearSelectedClips();
					m_opView -> Invalidate(m_opView -> Bounds());
				}

				else if(oNewlySelectedClips.size() != EMGClipRepository::Instance() -> GetSelectedClips() -> size())
					EMGClipRepository::Instance() -> SetSelected(&oNewlySelectedClips, true);
			}
			oUpdateRect = oUpdateRect | EMGClipRepository::Instance() -> SelectedFrame();
			m_opView -> Invalidate(oUpdateRect);
			m_vSelectionBoxExists = false;
			m_oSelectionBox = EMRect();
			DrawSelectionBox(EMRect(), false);		
	
			char vMessage[80];
			sprintf(vMessage, "%ld clip(s) selected", EMGClipRepository::Instance() -> GetNrSelectedClips());
			TrackWindowStatusBar::Instance() -> WriteMessage(string(vMessage));
		}

		if(m_vClipsHaveMoved)
		{
			if(!(EMGClipRepository::Instance() -> OkToDrop(EMGClipRepository::Instance() -> GetSelectedClips())))
			{
				;//cout_commented_out_4_release << "Cannot drop here" << endl;
				m_opView -> Invalidate(EMGClipRepository::Instance() -> MovedFrame());
				//EMGClipRepository::Instance() -> MovedBounds() = EMRect();//opClipRepository -> SelectedBounds(); // Needed?
				EMGClipRepository::Instance() -> ClearMovedFrame();
				CalculateAndSetScrollableFrame();
				m_vClipsHaveMoved = false;

				TrackWindowStatusBar::Instance() -> WriteMessage(string("Operation failed: Illegal clip location"));
			}
			else
			{
;//cout_commented_out_4_release << "m_vPositionMovedClipOffset : " << m_vPositionMovedClipOffset << endl;
				uint32 vModifiers = EMKeyboard::Instance() -> GetModifiers();
				if((vModifiers & EM_CONTROL_KEY) == EM_CONTROL_KEY)
					EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_CLONE_CLIPS, &m_vTrackOffset, &m_vPositionMovedClipOffset, EMGClipRepository::Instance() -> GetSelectedClips());
				else	
					EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_MOVE_CLIPS, &m_vTrackOffset, &m_vPositionMovedClipOffset);
				EMGClipRepository::Instance() -> ClearMovedFrame();
				CalculateAndSetScrollableFrame();
				m_vClipsHaveMoved = false;
			}
		}
	}

	else if(m_eToolboxMode == TOOLBOX_RESIZE)
	{
;//cout_commented_out_4_release << "mouse up, TOOLBOX_RESIZE" << endl;
		if(m_opMouseDownClip != NULL)
			m_opMouseDownClip -> MouseUp(m_opView, p_vX, p_vY, p_vButtonOne, p_vButtonTwo, p_vButtonThree);
	}
	
	else if(m_eToolboxMode == TOOLBOX_DRAW)
	{
		if(m_oGhostClipRect.IsValid())
		{
			EMGUIUtilities::Instance() -> GetSnapPosition(&vMouseXPos, true, true, NULL, true, true);
			int64 vSnappedMouseUpX = vMouseXPos / m_vTimeZoomScale;

			m_opView -> Invalidate(m_oGhostClipRect | m_oOldGhostClipRect);
			m_oOldGhostClipRect = EMRect();//m_oGhostClipRect;
			m_oGhostClipRect = EMRect();

			int32 vMediaTrackID = m_opMouseDownTrack -> GetTrackInfo() -> GetMediaTrackID();
			int64 vPos;
			int64 vEndPos;
			if(m_vSnappedMouseDownPos / m_vTimeZoomScale < vSnappedMouseUpX)
			{
				vPos = m_vSnappedMouseDownPos;
				vEndPos = vMouseXPos;
			}
			else
			{
				vPos = vMouseXPos;
				vEndPos = m_vSnappedMouseDownPos;
			}

			int32 vMediaClipID = *(static_cast<int32*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_CREATE_VIDEO_TRANSITION_CLIP, &vMediaTrackID, &vPos)));
			uint32 vTrackIndex = EMGTrackRepository::Instance() -> GetTrackIndex(m_opMouseDownTrack);
//eo << "Resize transition clip: " << vPos << ", " << vEndPos << ef;
			EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_RESIZE_VIDEO_TRANSITION_CLIP, &vMediaClipID, &vPos, &vEndPos);
			EMGClip* opClip = static_cast<EMGClip*>(EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_ADD_G_TRANSITION_CLIP, &vMediaClipID, &vTrackIndex, &vPos));
			EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_DISPLAY_TRANSITION_DIALOG, opClip);

			m_oGhostClipRect = EMRect();
		}
	}

	m_opMouseDownTrack = NULL;
	m_opMouseDownClip = NULL;	
}

void TrackView::ReadyForUse()
{
	if(m_opView -> LockWindow())
	{
		CalculateAndUpdateAll();
//CalculateAndSetScrollableFrame(); // Needed?
		m_opView -> Invalidate(m_opView -> Bounds());
		m_opView -> UnlockWindow();
	}
}

void TrackView::Scroll()
{
	if(GetView() -> LockBuffer())
	{
//eo << "TrackView::Scroll: m_vScrolledOffsetX: " << m_vScrolledOffsetX << ", m_vScrolledOffsetY: " << m_vScrolledOffsetY << ef;
		GetView() -> ScrollTo(m_vScrolledOffsetX, m_vScrolledOffsetY);
		GetView() -> UnlockBuffer();
	}
}

void TrackView::SetScrollbars(EMScrollbar* p_opScrollbarHorizontal, EMScrollbar* p_opScrollbarVertical)
{
	m_opScrollbarHorizontal = p_opScrollbarHorizontal;
	m_opScrollbarVertical = p_opScrollbarVertical;
}

void TrackView::ThreadRun(EMThread* p_opThread)
{
//EMVideoPreview::Instance() -> ShowPreview();


	// This code section handles the position pointer
	m_vPosition = *(static_cast<int64*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_CURRENT_PROJECT_POSITION, NULL)));//InstanceMediaTimer() -> NowTime();
//;//cout_commented_out_4_release << "m_vPosition: " << m_vPosition << endl;

	// This is a debug thingie (draws a second position pointer)

	uint32 vModifiers = EMKeyboard::Instance() -> GetModifiers();
	if((vModifiers & EM_CAPS_LOCK) == EM_CAPS_LOCK && (vModifiers & EM_SHIFT_KEY) == EM_SHIFT_KEY)
		m_vThenPosition = *(static_cast<int64*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_CURRENT_VIDEO_PROCESSING_POSITION, NULL)));//InstanceMediaTimer() -> VideoThenTime();
	else if((vModifiers & EM_CAPS_LOCK) == EM_CAPS_LOCK)
		m_vThenPosition = *(static_cast<int64*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_CURRENT_AUDIO_PROCESSING_POSITION, NULL)));//InstanceMediaTimer() -> AudioThenTime();
	else
		m_vThenPosition = UINT64_MAX;

	if(static_cast<int>(m_vPosition / m_vTimeZoomScale) != static_cast<int>(m_vOldPosition))
		if(m_opView -> LockWindow())
		{
;//cout_commented_out_4_release << "TrackView::ThreadRun: position: " << m_vPosition << endl;
			if(m_eEditMode != EM_FX_EDIT_MODE)
			{
				if(!m_oRectVisible.Contains(m_vPosition / m_vTimeZoomScale, m_oRectVisible.m_vTop))
				{
					m_vScrolledOffsetX = static_cast<int32>(m_vPosition / m_vTimeZoomScale);
					CalculateViewFrames();
					Scroll();
				}
				m_opView -> BeginPaint();
				DrawPosition(EMRect());
				m_opView -> EndPaint();
			}

			m_opView -> UnlockWindow();
		}

	if(!m_vMouseButtonPressed)
		return;

	// This code section handles automatic scrolling
	bool vScroll = false;
	if(m_opView -> LockWindow())
	{
		if(EMSlimValueBox::ValueBoxIsActive()) // If the user is changing a value in a value box
		{
			m_opView -> UnlockWindow();
			return;
		}

		if(m_vMousePosX - m_oRectVisible.m_vLeft < MOUSE_SCROLL_THRESHOLD && m_vScrolledOffsetX > 0)
		{
			// Scroll horizontally towards the beginning of the project
			m_vScrolledOffsetX -= MOUSE_SCROLL_THRESHOLD - (m_vMousePosX - m_oRectVisible.m_vLeft);
			m_vMousePosX -= MOUSE_SCROLL_THRESHOLD - (m_vMousePosX - m_oRectVisible.m_vLeft);
			if(m_vMousePosX < 0 && m_vScrolledOffsetX > 0)
			{
				m_vScrolledOffsetX -= m_vMousePosX;
				m_vMousePosX -= m_vMousePosX;
			}
			m_opScrollbarHorizontal -> SetValue(m_vScrolledOffsetX);
			vScroll = true;
		}
		else if(m_oRectVisible.m_vRight - m_vMousePosX < MOUSE_SCROLL_THRESHOLD && m_oRectVisible.m_vRight < m_oRectScrollable.m_vRight)
		{
			// Scroll horizontally towards the end of the project
			m_vScrolledOffsetX += MOUSE_SCROLL_THRESHOLD - (m_oRectVisible.m_vRight - m_vMousePosX);
			m_vMousePosX += MOUSE_SCROLL_THRESHOLD - (m_oRectVisible.m_vRight - m_vMousePosX);
eo << "Autoscroll " << m_vMousePosX << ef;
m_oRectScrollable.Print();
CalculateAndSetScrollableFrame(m_vMousePosX);
m_oRectScrollable.Print();
			m_opScrollbarHorizontal -> SetValue(m_vScrolledOffsetX);
			vScroll = true;
		}
		else if(m_vMousePosY - m_oRectVisible.m_vTop < MOUSE_SCROLL_THRESHOLD && m_vScrolledOffsetY > 0)
		{
			// Scroll vertically towards the first track
			m_vScrolledOffsetY -= MOUSE_SCROLL_THRESHOLD - (m_vMousePosY - m_oRectVisible.m_vTop);
			m_vMousePosY -= MOUSE_SCROLL_THRESHOLD - (m_vMousePosY - m_oRectVisible.m_vTop);
			if(m_vMousePosY < 0 && m_vScrolledOffsetY > 0)
			{
				m_vScrolledOffsetY -= m_vMousePosY;
				m_vMousePosY -= m_vMousePosY;
			}
			m_opScrollbarVertical -> SetValue(m_vScrolledOffsetY);
			vScroll = true;
		}
		else if(m_oRectVisible.m_vBottom - m_vMousePosY < MOUSE_SCROLL_THRESHOLD)
		{
			// Scroll vertically towards the last track
//eo << "m_vScrolledOffsetY: " << m_vScrolledOffsetY << ", m_oRectScrollable.GetHeight(): " << m_oRectScrollable.GetHeight() << ", m_oRectVisible.GetHeight(): " << m_oRectVisible.GetHeight() << ef;
//			if(m_vScrolledOffsetY == m_oRectScrollable.GetHeight() - m_oRectVisible.GetHeight())
			if(m_oRectScrollable.GetHeight() - m_vScrolledOffsetY <= m_oRectVisible.GetHeight())
			{
				m_opView -> UnlockWindow();
				return;
			}
			m_vScrolledOffsetY += MOUSE_SCROLL_THRESHOLD - (m_oRectVisible.m_vBottom - m_vMousePosY);
			m_vMousePosY += MOUSE_SCROLL_THRESHOLD - (m_oRectVisible.m_vBottom - m_vMousePosY);
			m_opScrollbarVertical -> SetValue(m_vScrolledOffsetY);
			vScroll = true;
		}

		if(vScroll)
		{
			CalculateViewFrames();
			MouseMoved(m_vMousePosX, m_vMousePosY, true, false, false); // Ok with these buttons?
			Scroll();
		}
		m_opView -> UnlockWindow();
	}
}

void TrackView::UpdateTimeScale()
{
	int64 vMiddleOfScreenPosition = *(static_cast<int64*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_CURRENT_PROJECT_POSITION, NULL)));//InstanceMediaTimer() -> NowTime();
 	for(uint32 vIndex = 0; vIndex < EMGTrackRepository::Instance() -> GetNrTracks(); vIndex++)
		EMGTrackRepository::Instance() -> GetTrack(vIndex) -> GetClipContainer() -> UpdateTimeScale(m_vTimeZoomScale);
	EMGClipRepository::Instance() -> CalculateFrame();
	EMGClipRepository::Instance() -> CalculateSelectedFrame();
	CalculateViewFrames();
	m_vScrolledOffsetX = (vMiddleOfScreenPosition / m_vTimeZoomScale) - (m_oRectVisible.GetWidth() / 2);
	if(m_vScrolledOffsetX < 0)
		m_vScrolledOffsetX = 0;
	if(m_opScrollbarHorizontal != NULL)
		m_opScrollbarHorizontal -> SetValue(m_vScrolledOffsetX);
	CalculateViewFrames();
	Scroll();
	m_opView -> Invalidate(m_opView -> Bounds());
}




