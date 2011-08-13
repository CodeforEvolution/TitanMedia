/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __TRACK_VIEW
#define __TRACK_VIEW

#include "EMGUIGlobals.h"
#include "EMListener.h"
#include "EMListenerRepository.h"
#include "EMProjectDataContainer.h"
#include "EMRect.h"
#include "EMThreadListener.h"
#include "GUIGlobals.h"
#include "Toolbox.h"

class EMGClip;
class EMGTrack;
class EMScrollbar;
class EMToolTip;
class EMView;
//#ifdef PLATFORM_WINDOWS
//class EMWinDropTarget;
//#endif

class TrackView : public EMThreadListener, public EMListener, public EMListenerRepository, public EMProjectDataContainer
{
public:
	TrackView();
	~TrackView();
	void CalculateAndUpdateAll();
//	void CalculateTrackFramesAndUpdate(EMGTrack* p_opTrack);
	bool ClearData();
	void Draw(EMRect p_oClippingRect);
	void EnableMenuItems();
	EMView* GetView();
	void Init();
	bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage);
	void ReadyForUse();
	void SetScrollbars(EMScrollbar* p_opScrollbarHorizontal, EMScrollbar* p_opScrollbarVertical);
	void ThreadRun(EMThread* p_opThread);
	
private:
	void AdjustMoveOffset();
	void CalculateAndSetScrollableFrame(int32 p_vMinimumX = 0);
	EMRect CalculateAndSetTrackFrames();
//	void CalculateSnappingMode();
	EMRect CalculateTrackFrame(EMGTrack* p_opTrack);
	EMRect CalculateViewFrames();
	void ChangeEditMode(EMEditMode p_eNewEditMode);
	void DrawGhostClip(EMRect p_oClippingRect);
	void DrawVerticalGrid(EMRect p_oClippingRect);
	void DrawPosition(EMRect p_oClippingRect);
	void DrawSelectionBox(EMRect p_oUpdateRect, bool p_vTriggeredByInvalidate);
//	void ListenToClipContainers();
	void LoadComplete();
	void MouseDoubleClicked(int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree);
	void MouseDown(int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree);
	void MouseMoved(int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree);
	void MouseUp(int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree);
	/*
	void CalculateBounds();
	void CalculateScrollableBounds();
	*/
	void Scroll();
	void UpdateTimeScale();

private:
	EMEditMode m_eEditMode; // Cached setting
//	SnappingMode m_eSnappingMode; // Cached setting
	ToolboxMode m_eToolboxMode; // Cached setting
	EMRect m_oGhostClipRect;
	EMRect m_oOldGhostClipRect;
	EMRect m_oOldSelectionBox;
//#ifdef PLATFORM_WINDOWS
//	EMWinDropTarget* m_opIDropTarget;
//#endif
	EMGClip* m_opMouseDownClip;
	EMGTrack* m_opMouseDownTrack;
	EMScrollbar* m_opScrollbarHorizontal;	
	EMScrollbar* m_opScrollbarVertical;
	string* m_opSignature; // Cached setting
//	string* m_opSnappingMode; // Cached setting
	EMToolTip* m_opToolTip;
	EMView* m_opView;
	EMRect m_oRectEmpty;  // The visible rect not occupied by tracks
	EMRect m_oRectFull; // The visible rect occupied by tracks
	EMRect m_oRectScrollable; // The rect that can be scrolled through
	EMRect m_oRectTracks; // The rect that the tracks occupy
	EMRect m_oRectVisible; // The rect currently visible on screen
	EMRect m_oSelectionBox;
	string* m_opSnappingMode;
	Toolbox m_oToolbox;
	bool m_vClipsHaveMoved;
	bool m_vDrawClipContent; // Cached setting
	float m_vFramerate; // Cached setting
	bool m_vMouseButtonPressed;
	int32 m_vMouseDownX; // The X coordinate where the user last clicked
	int32 m_vMouseDownY; // The Y coordinate where the user last clicked
	int32 m_vMouseOffsetX;
	int32 m_vMouseOffsetY;
	int32 m_vMousePosX;
	int32 m_vMousePosY;
	int32 m_vOldPosition;
int64 m_vOldThenPosition; // Used for Richards twisted debugging purposes
	int64 m_vOldTimeZoomScale;
	int64 m_vPosition;
	int64 m_vPositionMovedClipOffset; // The offset from selected clips to moved clips
	int64 m_vRegionBegin; // Cached setting
	int64 m_vRegionEnd; // Cached setting
	bool m_vRelative; // Cached setting
	int64 m_vSavedScrollPosition;
	int32 m_vScrolledOffsetX; // Used for scrolling the display
	int32 m_vScrolledOffsetY; // Used for scrolling the display
	bool m_vSelectionBoxExists;
	int m_vSignatureMajor; // Cached setting
	int m_vSignatureMinor; // Cached setting
	int64 m_vSnappedMouseDownPos;
	bool m_vSnapping;
	float m_vTempo; // Cached setting
int64 m_vThenPosition; // Used for Richards twisted debugging purposes
	int64 m_vTimeZoomScale; // Cached setting
	int32 m_vTrackOffset;
};
 
#endif

