/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __TRACK_WINDOW_TIMELINE
#define __TRACK_WINDOW_TIMELINE

#include "EMListener.h"
#include "EMProjectDataContainer.h"
#include "EMRect.h"
#include "EMSlimButton.h"
#include "EMThreadListener.h"

class EMListenerRepository;
class EMView;

enum DrawType
{
	DRAW_TIME_SAMPLES,
	DRAW_TIME_24_FRAMES,
	DRAW_TIME_25_FRAMES,
	DRAW_TIME_30_DROP_FRAMES,
	DRAW_TIME_30_FRAMES,
	DRAW_TIME_MEASURES,
	DRAW_MARKERS,
	DRAW_SIGNATURE,
	DRAW_TEMPO
};

enum DrawMode
{
	DRAW_MODE_1000,
	DRAW_MODE_1000_MIN,
	DRAW_MODE_100,
	DRAW_MODE_100_MIN,
	DRAW_MODE_10,
	DRAW_MODE_10_MIN,
	DRAW_MODE_1,
	DRAW_MODE_1_MIN,
	DRAW_MODE_10_SECONDS,
	DRAW_MODE_10_SECONDS_MIN,
	DRAW_MODE_SECONDS,
	DRAW_MODE_SECONDS_MIN
};	

class TrackWindowTimeLine : public EMListener, public EMThreadListener, public EMProjectDataContainer
{
public:
	TrackWindowTimeLine();
	~TrackWindowTimeLine();
	EMView* GetView();
	void Init();
	bool IsDirty();
	bool LoadData(void* p_upProjectDataLoader);
	bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage);
	EMRect PreferredBounds();
	void ReadyForUse();
	bool SaveData(void* p_upProjectDataSaver);;
	void SetExpand();
	void ThreadRun(EMThread* p_opThread);

private:
	void CalculateBounds();
	void CalculateFrames();
	void CalculateMetrics();
//	void CalculateSnappingMode();
	void Draw(EMRect p_oClippingRect);
	void DrawContentMeasures(EMRect p_oClippingRect, EMRect p_oDestination, DrawType p_eDrawType, EMColor p_opBackgroundColor);
	void DrawContentSMPTE(EMRect p_oClippingRect, EMRect p_oDestination, DrawType p_eDrawType, EMColor p_opBackgroundColor);
	void DrawPosition(EMRect p_oClippingRect);
	void DrawRegion(EMRect p_oClippingRect);
	void DrawScrolling(EMRect p_oClippingRect);
	void DrawMouse(EMRect p_oClippingRect);
	void LayoutButtons();
	void MouseDown(int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree);
	void MouseDownScrolling(int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree);
	void MouseMovedScrolling(int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree);
	void MouseUpScrolling(int32 p_vX, int32 p_vY);
	void SetUserMode();

private:
	EMRect m_oOldFrame;
	EMView* m_opScrollingView;
	string* m_opSignature; // Cached setting
	EMView* m_opView;
	EMRect m_oRegion;
	EMSlimButton m_oSignatureButton;
	EMRect m_oSignatureFrame;
	EMSlimButton m_oTempoButton;
	EMSlimButton m_oTimeOneButton;
	EMSlimButton m_oTimeTwoButton;
	EMRect m_oTempoFrame;
	EMRect m_oTimeOneFrame;
	EMRect m_oTimeTwoFrame;
	bool m_vExpertMode;
	float m_vFramerate; // Cached setting
	bool m_vHasSeeked;
	bool m_vIsDirty;
	bool m_vIsExpanded;
	bool m_vMIDIEnabled;
	bool m_vMouseButtonPressed;
	int32 m_vMouseDownX;
	int32 m_vMouseOffsetX;
	int32 m_vOldMouseX;
	int32 m_vOldPosition;
	int64 m_vPosition;
	int64 m_vRegionBegin;
	int64 m_vRegionEnd;
	bool m_vRegionExists;
	bool m_vRegionHasChanged;
	bool m_vResizeRegionLeft;
	bool m_vResizeRegionRight;
	int32 m_vScrollOffsetX;
	int m_vSignatureMajor; // Cached setting
	int m_vSignatureMinor; // Cached setting
	bool m_vSnapping;
	char m_vString[10];
	float m_vTempo; // Cached setting
	int64 m_vTimeZoomScale; // Cached setting

	bool m_vTempoTimelineDrawMajorText;
	bool m_vTempoTimelineDrawMinorText;
	int64 m_vTempoTimelineMajorNudgeSpacing;
	int64 m_vTempoTimelineMinorNudgeSpacing;
	int m_vTempoTimelineMajorNudgeCountdown;
	DrawMode m_eTempoTimelineDrawMode;

	bool m_vSMPTETimelineDrawMajorText;
	bool m_vSMPTETimelineDrawMinorText;
	int64 m_vSMPTETimelineMajorNudgeSpacing;
	int64 m_vSMPTETimelineMinorNudgeSpacing;
	int m_vSMPTETimelineMajorNudgeCountdown;
	DrawMode m_eSMPTETimelineDrawMode;
};

#endif

