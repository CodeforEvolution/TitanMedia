#include "TrackWindowTimeLine.h"

#include "CommandIDs.h"
#include "EMCommandRepository.h"
#include "EMFactory.h"
#include "EMGUIGlobals.h"
#include "EMGUIUtilities.h"
#include "EMKeyboard.h" // Debug
#include "EMKeyboardGlobals.h" // Debug
#include "EMMediaGlobals.h"
#include "EMMessages.h"
#include "EMProjectDataLoader.h"
#include "EMProjectDataSaver.h"
#include "EMResourceRepository.h"
#include "EMScrollbar.h"
#include "EMSettingIDs.h"
#include "EMSettingsRepository.h"
#include "EMThreadRepository.h"
#include "EMView.h"
#include "GUIGlobals.h"
#include "ResourceIDs.h"
#include "SettingIDs.h"

#define COLOR_TIME_ONE_BACKGROUND EMColor(200, 255, 200, 255)
#define COLOR_TIME_ONE_HIGHLIGHT EMColor(230, 255, 230, 255)
#define COLOR_TIME_ONE_SHADOW EMColor(120, 255, 120, 255)
#define COLOR_TIME_TWO_BACKGROUND EMColor(255, 200, 200, 255)
#define COLOR_TIME_TWO_HIGHLIGHT EMColor(255, 230, 230, 255)
#define COLOR_TIME_TWO_SHADOW EMColor(255, 150, 150, 255)
#define COLOR_SIGNATURE_BACKGROUND EMColor(255, 200, 255, 255)
#define COLOR_SIGNATURE_HIGHLIGHT EMColor(255, 230, 255, 255)
#define COLOR_SIGNATURE_SHADOW EMColor(255, 150, 255, 255)
#define COLOR_TEMPO_BACKGROUND EMColor(255, 255, 200, 255)
#define COLOR_TEMPO_HIGHLIGHT EMColor(255, 230, 230, 255)
#define COLOR_TEMPO_SHADOW EMColor(255, 150, 150, 255)
#define COLOR_TEXT EMColor(50, 50, 50, 255)
#define COLOR_NUDGES EMColor(150, 150, 150, 255)

const int32 MAJOR_NUDGE_SIZE = 10;
const int32 MINOR_NUDGE_SIZE = 2;
const int32 MINIMUM_MAJOR_NUDGE_SPACING = 50;
const int32 MINIMUM_MINOR_NUDGE_SPACING = 5;
const int32 MINIMUM_MAJOR_NUDGE_TEXT_SPACING = 30;
const int32 MINIMUM_MINOR_NUDGE_TEXT_SPACING = 30;
const int32 RESIZE_REGION_THRESHOLD = 5;
 
// The scrolling view is offset by one pixel to allow a vertical shadow 
// at the beginning of the view
const int CONTENT_START_PIXEL_OFFSET = -1;

TrackWindowTimeLine::TrackWindowTimeLine() :
m_opScrollingView(EMFactory::Instance() -> CreateView(EMRect(), EM_FOLLOW_LEFT_RIGHT | EM_FOLLOW_TOP, false)),
m_opView(EMFactory::Instance() -> CreateView(EMRect(), EM_FOLLOW_LEFT_RIGHT | EM_FOLLOW_TOP, false)),
m_oSignatureButton(m_opView, 
					EMRect(0, 0, EM_METRICS_TINY_SLIM_BUTTON_WIDTH - 1, EM_METRICS_SLIM_BUTTON_HEIGHT - 1),
					NULL, NULL, 0, NULL, NULL, NULL, true, true, true, true, 
					EM_COLOR_BUTTON_DEFAULT_ON,
					EM_COLOR_BUTTON_DEFAULT_OFF),
m_oTempoButton(m_opView, 
					EMRect(0, 0, EM_METRICS_TINY_SLIM_BUTTON_WIDTH - 1, EM_METRICS_SLIM_BUTTON_HEIGHT - 1),
					NULL, NULL, 0, NULL, NULL, NULL, true, true, true, true, 
					EM_COLOR_BUTTON_DEFAULT_ON,
					EM_COLOR_BUTTON_DEFAULT_OFF),
m_oTimeOneButton(m_opView, 
					EMRect(0, 0, EM_METRICS_TINY_SLIM_BUTTON_WIDTH - 1, EM_METRICS_SLIM_BUTTON_HEIGHT - 1),
					NULL, NULL, COMMAND_EXPAND_TIMELINE, NULL, NULL, NULL, true, true, true, true,
					EM_COLOR_BUTTON_DEFAULT_ON, EM_COLOR_BUTTON_DEFAULT_OFF,
					NULL, NULL, NULL, NULL, "Toggles Display Of The Measures Timeline On/Off"),
m_oTimeTwoButton(m_opView, 
					EMRect(0, 0, EM_METRICS_TINY_SLIM_BUTTON_WIDTH - 1, EM_METRICS_SLIM_BUTTON_HEIGHT - 1),
					NULL, NULL, 0, NULL, NULL, NULL, true, true, true, true, 
					EM_COLOR_BUTTON_DEFAULT_ON,
					EM_COLOR_BUTTON_DEFAULT_OFF),
m_vHasSeeked(false),
m_vIsDirty(false),
m_vIsExpanded(false),
m_vMouseButtonPressed(false),
m_vMouseDownX(0),
m_vMouseOffsetX(0),
m_vOldMouseX(-1),
m_vOldPosition(-1),
m_vPosition(0),
m_vRegionBegin(-1),
m_vRegionEnd(-1),
m_vRegionExists(false),
m_vRegionHasChanged(false),
m_vResizeRegionLeft(false),
m_vResizeRegionRight(false),
m_vScrollOffsetX(0)
{
}

TrackWindowTimeLine::~TrackWindowTimeLine()
{
//	EMThreadRepository::Instance() -> GetThread(GUI_SERVICE_THREAD_NAME) -> RemoveListener(this);
	delete m_opScrollingView;
	delete m_opView;
}

void TrackWindowTimeLine::CalculateBounds()
{
	m_opView -> AssertWindowLocked();

	EMRect oBounds = m_opView -> Bounds();
	
	m_opScrollingView -> SetFrame(EMRect(oBounds.m_vLeft + EM_METRICS_TINY_SLIM_BUTTON_WIDTH, 
							0, 
							oBounds.m_vRight - 1, 
							oBounds.m_vBottom - 3));
}

void TrackWindowTimeLine::CalculateFrames()
{
	EMRect oScrollingBounds = m_opScrollingView -> Bounds();
//eo << "TrackWindowTimeLine::CalculateFrames: new bounds: ";
//oScrollingBounds.Print();
	int32 vCurrentYPos = 0;
/*
	if(m_oSignatureButton.GetOn() || m_vIsExpanded)
	{
		m_oSignatureFrame = EMRect(0, 
								vCurrentYPos, 
								oScrollingBounds.m_vRight, 
								vCurrentYPos + EM_METRICS_SLIM_BUTTON_HEIGHT - 1);
		vCurrentYPos += EM_METRICS_SLIM_BUTTON_HEIGHT;
	}
	if(m_oTempoButton.GetOn() || m_vIsExpanded)
	{
		m_oTempoFrame = EMRect(0, 
							vCurrentYPos, 
							oScrollingBounds.m_vRight, 
							vCurrentYPos + EM_METRICS_SLIM_BUTTON_HEIGHT - 1);
		vCurrentYPos += EM_METRICS_SLIM_BUTTON_HEIGHT;
	}
*/
	if(m_oTimeTwoButton.GetOn() || m_vIsExpanded)
	{
		m_oTimeTwoFrame = EMRect(0, 
							vCurrentYPos, 
							oScrollingBounds.m_vRight, 
							vCurrentYPos + EM_METRICS_SLIM_BUTTON_HEIGHT - 1);
		vCurrentYPos += EM_METRICS_SLIM_BUTTON_HEIGHT;
	}
	m_oTimeOneFrame = EMRect(oScrollingBounds.m_vLeft, 
						vCurrentYPos, 
						oScrollingBounds.m_vRight, 
						vCurrentYPos + EM_METRICS_SLIM_BUTTON_HEIGHT - 1);
}

void TrackWindowTimeLine::CalculateMetrics()
{
	int64 vMajorNudgeSpacing;
	int64 vMinorNudgeSpacing;

	// The measures timeline
	vMajorNudgeSpacing = static_cast<int64>((m_vSignatureMajor * EM_TICKS_PER_SECOND * 240) / (m_vSignatureMinor * m_vTempo));
	
	if(vMajorNudgeSpacing / m_vTimeZoomScale >= MINIMUM_MAJOR_NUDGE_SPACING)
	{
		vMinorNudgeSpacing = vMajorNudgeSpacing / m_vSignatureMajor;
		if(vMinorNudgeSpacing / m_vTimeZoomScale >= MINIMUM_MINOR_NUDGE_SPACING)
		{
			if(vMinorNudgeSpacing / m_vTimeZoomScale >= MINIMUM_MINOR_NUDGE_TEXT_SPACING)
				m_vTempoTimelineDrawMinorText = true;
			else
				m_vTempoTimelineDrawMinorText = false;
			m_eTempoTimelineDrawMode = DRAW_MODE_1_MIN;
		}
		else
			m_eTempoTimelineDrawMode = DRAW_MODE_1;
		m_vTempoTimelineMajorNudgeSpacing = vMajorNudgeSpacing;
		m_vTempoTimelineMinorNudgeSpacing = vMinorNudgeSpacing;
	}

	else if(vMajorNudgeSpacing * 10 / m_vTimeZoomScale >= MINIMUM_MAJOR_NUDGE_SPACING)
	{
		vMinorNudgeSpacing = vMajorNudgeSpacing;
		if(vMinorNudgeSpacing / m_vTimeZoomScale >= MINIMUM_MINOR_NUDGE_SPACING)
		{
			if(vMinorNudgeSpacing / m_vTimeZoomScale >= MINIMUM_MINOR_NUDGE_TEXT_SPACING)
				m_vTempoTimelineDrawMinorText = true;
			else
				m_vTempoTimelineDrawMinorText = false;
			m_eTempoTimelineDrawMode = DRAW_MODE_10_MIN;
		}
		else
			m_eTempoTimelineDrawMode = DRAW_MODE_10;
		m_vTempoTimelineMajorNudgeSpacing = vMajorNudgeSpacing * 10;
		m_vTempoTimelineMinorNudgeSpacing = vMinorNudgeSpacing;
	}
	
	else if(vMajorNudgeSpacing * 100 / m_vTimeZoomScale >= MINIMUM_MAJOR_NUDGE_SPACING)
	{
		vMinorNudgeSpacing = vMajorNudgeSpacing * 10;
		if(vMinorNudgeSpacing / m_vTimeZoomScale >= MINIMUM_MINOR_NUDGE_SPACING)
		{
			if(vMinorNudgeSpacing / m_vTimeZoomScale >= MINIMUM_MINOR_NUDGE_TEXT_SPACING)
				m_vTempoTimelineDrawMinorText = true;
			else
				m_vTempoTimelineDrawMinorText = false;
			m_eTempoTimelineDrawMode = DRAW_MODE_100_MIN;
		}
		else
			m_eTempoTimelineDrawMode = DRAW_MODE_100;
		m_vTempoTimelineMajorNudgeSpacing = vMajorNudgeSpacing * 100;
		m_vTempoTimelineMinorNudgeSpacing = vMinorNudgeSpacing;
	}
	else
	{
		vMinorNudgeSpacing = vMajorNudgeSpacing * 100;
		if(vMinorNudgeSpacing / m_vTimeZoomScale >= MINIMUM_MINOR_NUDGE_SPACING)
		{
			if(vMinorNudgeSpacing / m_vTimeZoomScale >= MINIMUM_MINOR_NUDGE_TEXT_SPACING)
				m_vTempoTimelineDrawMinorText = true;
			else
				m_vTempoTimelineDrawMinorText = false;
			m_eTempoTimelineDrawMode = DRAW_MODE_1000_MIN;
		}
		else
		{
			if(vMajorNudgeSpacing * 1000 / m_vTimeZoomScale >= MINIMUM_MAJOR_NUDGE_TEXT_SPACING)
				m_vTempoTimelineDrawMajorText = true;
			else
				m_vTempoTimelineDrawMajorText = false;
			m_eTempoTimelineDrawMode = DRAW_MODE_1000;
		}
		m_vTempoTimelineMajorNudgeSpacing = vMajorNudgeSpacing * 1000;
		m_vTempoTimelineMinorNudgeSpacing = vMinorNudgeSpacing;
	}
	
	// The SMPTE timeline
	vMajorNudgeSpacing = EM_TICKS_PER_SECOND;
	
	if(vMajorNudgeSpacing / m_vTimeZoomScale >= MINIMUM_MAJOR_NUDGE_SPACING)
	{
		vMinorNudgeSpacing = static_cast<int64>(vMajorNudgeSpacing / m_vFramerate);
		if(vMinorNudgeSpacing / m_vTimeZoomScale >= MINIMUM_MINOR_NUDGE_SPACING)
		{
			if(vMinorNudgeSpacing / m_vTimeZoomScale >= MINIMUM_MINOR_NUDGE_TEXT_SPACING)
				m_vSMPTETimelineDrawMinorText = true;
			else
				m_vSMPTETimelineDrawMinorText = false;
			m_eSMPTETimelineDrawMode = DRAW_MODE_1_MIN;
		}
		else
			m_eSMPTETimelineDrawMode = DRAW_MODE_1;
		m_vSMPTETimelineMajorNudgeSpacing = vMajorNudgeSpacing;
		m_vSMPTETimelineMinorNudgeSpacing = vMinorNudgeSpacing;
	}

	else if(vMajorNudgeSpacing * 10 / m_vTimeZoomScale >= MINIMUM_MAJOR_NUDGE_SPACING)
	{
		vMinorNudgeSpacing = vMajorNudgeSpacing;
		if(vMinorNudgeSpacing / m_vTimeZoomScale >= MINIMUM_MINOR_NUDGE_SPACING)
		{
			if(vMinorNudgeSpacing / m_vTimeZoomScale >= MINIMUM_MINOR_NUDGE_TEXT_SPACING)
				m_vSMPTETimelineDrawMinorText = true;
			else
				m_vSMPTETimelineDrawMinorText = false;
			m_eSMPTETimelineDrawMode = DRAW_MODE_10_MIN;
		}
		else
			m_eSMPTETimelineDrawMode = DRAW_MODE_10;
		m_vSMPTETimelineMajorNudgeSpacing = vMajorNudgeSpacing * 10;
		m_vSMPTETimelineMinorNudgeSpacing = vMinorNudgeSpacing;
	}
	
	else if(vMajorNudgeSpacing * 60 / m_vTimeZoomScale >= MINIMUM_MAJOR_NUDGE_SPACING)
	{
		vMinorNudgeSpacing = vMajorNudgeSpacing * 10;
		if(vMinorNudgeSpacing / m_vTimeZoomScale >= MINIMUM_MINOR_NUDGE_SPACING)
		{
			if(vMinorNudgeSpacing / m_vTimeZoomScale >= MINIMUM_MINOR_NUDGE_TEXT_SPACING)
				m_vSMPTETimelineDrawMinorText = true;
			else
				m_vSMPTETimelineDrawMinorText = false;
			m_eSMPTETimelineDrawMode = DRAW_MODE_100_MIN;
		}
		else
			m_eSMPTETimelineDrawMode = DRAW_MODE_100;
		m_vSMPTETimelineMajorNudgeSpacing = vMajorNudgeSpacing * 60;
		m_vSMPTETimelineMinorNudgeSpacing = vMinorNudgeSpacing;
	}
	
	else if(vMajorNudgeSpacing * 1000 / m_vTimeZoomScale >= MINIMUM_MAJOR_NUDGE_SPACING)
	{
		vMinorNudgeSpacing = vMajorNudgeSpacing * 60;
		if(vMinorNudgeSpacing / m_vTimeZoomScale >= MINIMUM_MINOR_NUDGE_SPACING)
		{
			if(vMinorNudgeSpacing / m_vTimeZoomScale >= MINIMUM_MINOR_NUDGE_TEXT_SPACING)
				m_vSMPTETimelineDrawMinorText = true;
			else
				m_vSMPTETimelineDrawMinorText = false;
			m_eSMPTETimelineDrawMode = DRAW_MODE_1000_MIN;
		}
		else
			m_eSMPTETimelineDrawMode = DRAW_MODE_1000;
		m_vSMPTETimelineMajorNudgeSpacing = vMajorNudgeSpacing * 1000;
		m_vSMPTETimelineMinorNudgeSpacing = vMinorNudgeSpacing;
	}
}

void TrackWindowTimeLine::Draw(EMRect p_oClippingRect)
{
//	m_opScrollingView -> ScrollTo(m_vScrollOffsetX, 0); // FIXME: Temporary solution to shared double buffer scroll problem

	m_oSignatureButton.Draw(p_oClippingRect, m_opView);
	m_oTempoButton.Draw(p_oClippingRect, m_opView);
	m_oTimeOneButton.Draw(p_oClippingRect, m_opView);
	m_oTimeTwoButton.Draw(p_oClippingRect, m_opView);

	EMRect oBounds = m_opView -> Bounds();
	
	if(m_oSignatureButton.GetOn() || m_vIsExpanded)
	{
//		m_opView -> SetHighColor(COLOR_SIGNATURE_HIGHLIGHT);
//		m_opView -> StrokeLine(EM_METRICS_TINY_SLIM_BUTTON_WIDTH, m_oSignatureFrame.m_vTop, EM_METRICS_TINY_SLIM_BUTTON_WIDTH, m_oSignatureFrame.m_vBottom);
		m_opView -> SetHighColor(COLOR_SIGNATURE_SHADOW);
		m_opView -> StrokeLine(oBounds.m_vRight, m_oSignatureFrame.m_vTop, oBounds.m_vRight, m_oSignatureFrame.m_vBottom);
	}
	if(m_oTempoButton.GetOn() || m_vIsExpanded)
	{
//		m_opView -> SetHighColor(COLOR_TEMPO_HIGHLIGHT);
//		m_opView -> StrokeLine(EM_METRICS_TINY_SLIM_BUTTON_WIDTH, m_oTempoFrame.m_vTop, EM_METRICS_TINY_SLIM_BUTTON_WIDTH, m_oTempoFrame.m_vBottom);
		m_opView -> SetHighColor(COLOR_TEMPO_SHADOW);
		m_opView -> StrokeLine(oBounds.m_vRight, m_oTempoFrame.m_vTop, oBounds.m_vRight, m_oTempoFrame.m_vBottom);
	}
	if(m_oTimeTwoButton.GetOn() || m_vIsExpanded)
	{
//		m_opView -> SetHighColor(COLOR_TIME_TWO_HIGHLIGHT);
//		m_opView -> StrokeLine(EM_METRICS_TINY_SLIM_BUTTON_WIDTH, m_oTimeTwoFrame.m_vTop, EM_METRICS_TINY_SLIM_BUTTON_WIDTH, m_oTimeTwoFrame.m_vBottom);
		m_opView -> SetHighColor(COLOR_TIME_TWO_SHADOW);
		m_opView -> StrokeLine(oBounds.m_vRight, m_oTimeTwoFrame.m_vTop, oBounds.m_vRight, m_oTimeTwoFrame.m_vBottom);
	}
//	m_opView -> SetHighColor(COLOR_TIME_ONE_HIGHLIGHT);
//	m_opView -> StrokeLine(EM_METRICS_TINY_SLIM_BUTTON_WIDTH, m_oTimeOneFrame.m_vTop, EM_METRICS_TINY_SLIM_BUTTON_WIDTH, m_oTimeOneFrame.m_vBottom);
	m_opView -> SetHighColor(COLOR_TIME_ONE_SHADOW);
	m_opView -> StrokeLine(oBounds.m_vRight, m_oTimeOneFrame.m_vTop, oBounds.m_vRight, m_oTimeOneFrame.m_vBottom);

	m_opView -> SetHighColor(EM_COLOR_VIEW_BACKGROUND_HIGHLIGHT);
	m_opView -> StrokeLine(p_oClippingRect.m_vLeft, oBounds.m_vBottom - 2, p_oClippingRect.m_vRight, oBounds.m_vBottom - 2);
	m_opView -> SetHighColor(EM_COLOR_VIEW_BACKGROUND);
	m_opView -> StrokeLine(p_oClippingRect.m_vLeft, oBounds.m_vBottom - 1, p_oClippingRect.m_vRight, oBounds.m_vBottom - 1);
	m_opView -> SetHighColor(EM_COLOR_VIEW_BACKGROUND_SHADOW);
	m_opView -> StrokeLine(p_oClippingRect.m_vLeft, oBounds.m_vBottom, p_oClippingRect.m_vRight, oBounds.m_vBottom);
}

void TrackWindowTimeLine::DrawContentSMPTE(EMRect p_oClippingRect, EMRect p_oDestination, DrawType p_eDrawType, EMColor p_opBackgroundColor)
{
	int vMajorNudgeCountdown;
	int32 vCurrentXPos;
	int32 vIndex;
	int32 vStartNudge;
	int32 vEndNudge;
	int vMinutes;
	int vSeconds;

	m_opScrollingView -> SetLowColor(p_opBackgroundColor);
	m_opScrollingView -> SetHighColor(COLOR_NUDGES);

	switch(m_eSMPTETimelineDrawMode)
	{
	case DRAW_MODE_1000:
		vStartNudge = static_cast<int32>((p_oClippingRect.m_vLeft) * m_vTimeZoomScale / m_vSMPTETimelineMajorNudgeSpacing);
		vEndNudge = static_cast<int32>((p_oClippingRect.m_vRight) * m_vTimeZoomScale / m_vSMPTETimelineMajorNudgeSpacing);
		for(vIndex = vStartNudge; vIndex <= vEndNudge; vIndex++)
		{
			vCurrentXPos = vIndex * m_vSMPTETimelineMajorNudgeSpacing / m_vTimeZoomScale;
			m_opScrollingView -> StrokeLine(vCurrentXPos, p_oDestination.m_vBottom - MAJOR_NUDGE_SIZE, vCurrentXPos, p_oDestination.m_vBottom);
//			if(m_vSMPTETimelineDrawMajorText)
			{
				sprintf(m_vString, "%03ld:00:00", vIndex);
				m_opScrollingView -> SetHighColor(COLOR_TEXT);
				m_opScrollingView -> DrawString(vCurrentXPos + 2, p_oDestination.m_vTop + 8, m_vString);
				m_opScrollingView -> SetHighColor(COLOR_NUDGES);
			}
		}
		break;
	case DRAW_MODE_1000_MIN:
		vStartNudge = static_cast<int32>((p_oClippingRect.m_vLeft * m_vTimeZoomScale) / m_vSMPTETimelineMinorNudgeSpacing);
		// Jump back to nearest major nudge so that the text is properly drawn
		vStartNudge -= vStartNudge % 10; 
		vEndNudge = static_cast<int32>((p_oClippingRect.m_vRight) * m_vTimeZoomScale / m_vSMPTETimelineMinorNudgeSpacing);
		vMajorNudgeCountdown = 10 - (vStartNudge % 10);
		if(vMajorNudgeCountdown == 10)
			vMajorNudgeCountdown = 0;
		for(vIndex = vStartNudge; vIndex <= vEndNudge; vIndex++)
		{
			vCurrentXPos = vIndex * m_vSMPTETimelineMinorNudgeSpacing / m_vTimeZoomScale;
			if(vMajorNudgeCountdown == 0)
			{
				m_opScrollingView -> StrokeLine(vCurrentXPos, p_oDestination.m_vBottom - MAJOR_NUDGE_SIZE, vCurrentXPos, p_oDestination.m_vBottom);
				sprintf(m_vString, "%03ld:00:00", vIndex);
				m_opScrollingView -> SetHighColor(COLOR_TEXT);
				m_opScrollingView -> DrawString(vCurrentXPos + 2, p_oDestination.m_vTop + 8, m_vString);
				m_opScrollingView -> SetHighColor(COLOR_NUDGES);
				vMajorNudgeCountdown = 9;
			}
			else
			{
				m_opScrollingView -> StrokeLine(vCurrentXPos, p_oDestination.m_vBottom - MINOR_NUDGE_SIZE, vCurrentXPos, p_oDestination.m_vBottom);
				if(m_vSMPTETimelineDrawMinorText)
				{
					sprintf(m_vString, "%ld", (vIndex % 10));
					m_opScrollingView -> DrawString(vCurrentXPos + 2, p_oDestination.m_vTop + 10, m_vString);
				}
				vMajorNudgeCountdown--;
			}
		}
		break;
	case DRAW_MODE_100:
		vStartNudge = static_cast<int32>((p_oClippingRect.m_vLeft) * m_vTimeZoomScale / m_vSMPTETimelineMajorNudgeSpacing);
		vEndNudge = static_cast<int32>((p_oClippingRect.m_vRight) * m_vTimeZoomScale / m_vSMPTETimelineMajorNudgeSpacing);
		for(vIndex = vStartNudge; vIndex <= vEndNudge; vIndex++)
		{
			vCurrentXPos = vIndex * m_vSMPTETimelineMajorNudgeSpacing / m_vTimeZoomScale;
			m_opScrollingView -> StrokeLine(vCurrentXPos, p_oDestination.m_vBottom - MAJOR_NUDGE_SIZE, vCurrentXPos, p_oDestination.m_vBottom);
			vMinutes = static_cast<int>(vIndex);
			sprintf(m_vString, "%03d:00:00", vMinutes);
			m_opScrollingView -> SetHighColor(COLOR_TEXT);
			m_opScrollingView -> DrawString(vCurrentXPos + 2, p_oDestination.m_vTop + 8, m_vString);
			m_opScrollingView -> SetHighColor(COLOR_NUDGES);
		}
		break;
	case DRAW_MODE_100_MIN:
		vStartNudge = static_cast<int32>((p_oClippingRect.m_vLeft * m_vTimeZoomScale) / m_vSMPTETimelineMinorNudgeSpacing);
		// Jump back to nearest major nudge so that the text is properly drawn
		vStartNudge -= vStartNudge % 6; 
		vEndNudge = static_cast<int32>((p_oClippingRect.m_vRight) * m_vTimeZoomScale / m_vSMPTETimelineMinorNudgeSpacing);
		vMajorNudgeCountdown = 6 - (vStartNudge % 6);
		if(vMajorNudgeCountdown == 6)
			vMajorNudgeCountdown = 0;
		for(vIndex = vStartNudge; vIndex <= vEndNudge; vIndex++)
		{
			vCurrentXPos = vIndex * m_vSMPTETimelineMinorNudgeSpacing / m_vTimeZoomScale;
			if(vMajorNudgeCountdown == 0)
			{
				m_opScrollingView -> StrokeLine(vCurrentXPos, p_oDestination.m_vBottom - MAJOR_NUDGE_SIZE, vCurrentXPos, p_oDestination.m_vBottom);
				vMinutes = static_cast<int>(vIndex / 6);
				vSeconds = (vIndex - vMinutes * 6) * 10;
				sprintf(m_vString, "%03d:%02d:00", vMinutes, vSeconds);
				m_opScrollingView -> SetHighColor(COLOR_TEXT);
				m_opScrollingView -> DrawString(vCurrentXPos + 2, p_oDestination.m_vTop + 8, m_vString);
				m_opScrollingView -> SetHighColor(COLOR_NUDGES);
				vMajorNudgeCountdown = 5;
			}
			else
			{
				m_opScrollingView -> StrokeLine(vCurrentXPos, p_oDestination.m_vBottom - MINOR_NUDGE_SIZE, vCurrentXPos, p_oDestination.m_vBottom);
				if(m_vSMPTETimelineDrawMinorText)
				{
					sprintf(m_vString, "%ld", (vIndex % 10));
					m_opScrollingView -> DrawString(vCurrentXPos + 2, p_oDestination.m_vTop + 10, m_vString);
				}
				vMajorNudgeCountdown--;
			}
		}
		break;
	case DRAW_MODE_10:
		vStartNudge = static_cast<int32>((p_oClippingRect.m_vLeft) * m_vTimeZoomScale / m_vSMPTETimelineMajorNudgeSpacing);
		vEndNudge = static_cast<int32>((p_oClippingRect.m_vRight) * m_vTimeZoomScale / m_vSMPTETimelineMajorNudgeSpacing);
		for(vIndex = vStartNudge; vIndex <= vEndNudge; vIndex++)
		{
			vCurrentXPos = vIndex * m_vSMPTETimelineMajorNudgeSpacing / m_vTimeZoomScale;
			m_opScrollingView -> StrokeLine(vCurrentXPos, p_oDestination.m_vBottom - MAJOR_NUDGE_SIZE, vCurrentXPos, p_oDestination.m_vBottom);
			vMinutes = static_cast<int>(vIndex / 60);
			vSeconds = vIndex - vMinutes * 60;
			sprintf(m_vString, "%03d:%02d:00", vMinutes, vSeconds);
			m_opScrollingView -> SetHighColor(COLOR_TEXT);
			m_opScrollingView -> DrawString(vCurrentXPos + 2, p_oDestination.m_vTop + 8, m_vString);
			m_opScrollingView -> SetHighColor(COLOR_NUDGES);
		}
		break;
	case DRAW_MODE_10_MIN:
		vStartNudge = static_cast<int32>((p_oClippingRect.m_vLeft * m_vTimeZoomScale) / m_vSMPTETimelineMajorNudgeSpacing) * 10;
		// Jump back to nearest major nudge so that the text is properly drawn
		vStartNudge -= vStartNudge % 10; 
		vEndNudge = static_cast<int32>((p_oClippingRect.m_vRight) * m_vTimeZoomScale / m_vSMPTETimelineMinorNudgeSpacing);
		vMajorNudgeCountdown = 10 - (vStartNudge % 10);
		if(vMajorNudgeCountdown == 10)
			vMajorNudgeCountdown = 0;
		for(vIndex = vStartNudge; vIndex <= vEndNudge; vIndex++)
		{
			vCurrentXPos = vIndex * m_vSMPTETimelineMinorNudgeSpacing / m_vTimeZoomScale;
			if(vMajorNudgeCountdown == 0)
			{
				m_opScrollingView -> StrokeLine(vCurrentXPos, p_oDestination.m_vBottom - MAJOR_NUDGE_SIZE, vCurrentXPos, p_oDestination.m_vBottom);
				vMinutes = static_cast<int>(vIndex / 60);
				vSeconds = vIndex - vMinutes * 60;
				sprintf(m_vString, "%03d:%02d:00", vMinutes, vSeconds);
				m_opScrollingView -> SetHighColor(COLOR_TEXT);
				m_opScrollingView -> DrawString(vCurrentXPos + 2, p_oDestination.m_vTop + 8, m_vString);
				m_opScrollingView -> SetHighColor(COLOR_NUDGES);
				vMajorNudgeCountdown = 9;
			}
			else
			{
				m_opScrollingView -> StrokeLine(vCurrentXPos, p_oDestination.m_vBottom - MINOR_NUDGE_SIZE, vCurrentXPos, p_oDestination.m_vBottom);
				if(m_vSMPTETimelineDrawMinorText)
				{
					sprintf(m_vString, "%ld", (vIndex % 10));
					m_opScrollingView -> DrawString(vCurrentXPos + 2, p_oDestination.m_vTop + 10, m_vString);
				}
				vMajorNudgeCountdown--;
			}
		}
		break;
	case DRAW_MODE_1:
		vStartNudge = static_cast<int32>((p_oClippingRect.m_vLeft) * m_vTimeZoomScale / m_vSMPTETimelineMajorNudgeSpacing);
		vEndNudge = static_cast<int32>((p_oClippingRect.m_vRight) * m_vTimeZoomScale / m_vSMPTETimelineMajorNudgeSpacing);
		for(vIndex = vStartNudge; vIndex <= vEndNudge; vIndex++)
		{
			vCurrentXPos = vIndex * m_vSMPTETimelineMajorNudgeSpacing / m_vTimeZoomScale;
			m_opScrollingView -> StrokeLine(vCurrentXPos, p_oDestination.m_vBottom - MAJOR_NUDGE_SIZE, vCurrentXPos, p_oDestination.m_vBottom);
			vMinutes = static_cast<int>(vIndex / 60);
			vSeconds = vIndex - (vMinutes * 60);
			sprintf(m_vString, "%03d:%02d:00", vMinutes, vSeconds);
			m_opScrollingView -> SetHighColor(COLOR_TEXT);
			m_opScrollingView -> DrawString(vCurrentXPos + 2, p_oDestination.m_vTop + 8, m_vString);
			m_opScrollingView -> SetHighColor(COLOR_NUDGES);
		}
		break;
	case DRAW_MODE_1_MIN:
		if(m_vFramerate != 29.997) // FIXME: Support 29.997 (30 drop frame) (update: support any weird framerate)
		{
//			vStartNudge = static_cast<int32>((p_oClippingRect.m_vLeft * m_vTimeZoomScale) / m_vSMPTETimelineMajorNudgeSpacing) * 10;
			vStartNudge = static_cast<int32>((p_oClippingRect.m_vLeft * m_vTimeZoomScale) / m_vSMPTETimelineMinorNudgeSpacing);
			// Jump back to nearest major nudge so that the text is properly drawn
			vStartNudge -= vStartNudge % static_cast<int>(m_vFramerate); 
			vEndNudge = static_cast<int32>((p_oClippingRect.m_vRight) * m_vTimeZoomScale / m_vSMPTETimelineMinorNudgeSpacing);
			vMajorNudgeCountdown = static_cast<int>(m_vFramerate) - (vStartNudge % static_cast<int>(m_vFramerate));
			if(vMajorNudgeCountdown == m_vFramerate)
				vMajorNudgeCountdown = 0;
//			vCurrentXPos = vStartNudge
			for(vIndex = vStartNudge; vIndex <= vEndNudge; vIndex++)
			{
//eo << "gronk - vStartNudge: " << vStartNudge << ", vEndNudge: " << vEndNudge << ", vMajorNudgeCountdown: " << vMajorNudgeCountdown << ef;
//eo << "gronk2 - p_oClippingRect.m_vLeft: " << p_oClippingRect.m_vLeft << ", m_vSMPTETimelineMajorNudgeSpacing: " << (double) m_vSMPTETimelineMajorNudgeSpacing << ", m_vSMPTETimelineMinorNudgeSpacing: " << (double) m_vSMPTETimelineMinorNudgeSpacing << ef;
				vCurrentXPos = vIndex * m_vSMPTETimelineMinorNudgeSpacing / m_vTimeZoomScale;
				if(vMajorNudgeCountdown == 0)
				{
					m_opScrollingView -> StrokeLine(vCurrentXPos, p_oDestination.m_vBottom - MAJOR_NUDGE_SIZE, vCurrentXPos, p_oDestination.m_vBottom);
					vMinutes = static_cast<int>(vIndex / (m_vFramerate * 60));
					vSeconds = (vIndex - (vMinutes * static_cast<int>(m_vFramerate) * 60)) / static_cast<int>(m_vFramerate);
					sprintf(m_vString, "%03d:%02d:00", vMinutes, vSeconds);
					m_opScrollingView -> SetHighColor(COLOR_TEXT);
					m_opScrollingView -> DrawString(vCurrentXPos + 2, p_oDestination.m_vTop + 8, m_vString);
					m_opScrollingView -> SetHighColor(COLOR_NUDGES);
					vMajorNudgeCountdown = static_cast<int>(m_vFramerate - 1);
				}
				else
				{
					m_opScrollingView -> StrokeLine(vCurrentXPos, p_oDestination.m_vBottom - MINOR_NUDGE_SIZE, vCurrentXPos, p_oDestination.m_vBottom);
					if(m_vSMPTETimelineDrawMinorText)
					{
						sprintf(m_vString, "%ld", (vIndex % static_cast<int>(m_vFramerate)));
						m_opScrollingView -> DrawString(vCurrentXPos + 2, p_oDestination.m_vTop + 10, m_vString);
					}
					vMajorNudgeCountdown--;
				}
			}
		}
		break;
	default:
		break;
	}
}

void TrackWindowTimeLine::DrawContentMeasures(EMRect p_oClippingRect, EMRect p_oDestination, DrawType p_eDrawType, EMColor p_opBackgroundColor)
{
	int vMajorNudgeCountdown;
	int32 vCurrentXPos;
	int32 vIndex;
	int32 vStartNudge;
	int32 vEndNudge;

	m_opScrollingView -> SetLowColor(p_opBackgroundColor);
	m_opScrollingView -> SetHighColor(COLOR_NUDGES);

	switch(m_eTempoTimelineDrawMode)
	{
	case DRAW_MODE_1000:
		vStartNudge = static_cast<int32>((p_oClippingRect.m_vLeft) * m_vTimeZoomScale / m_vTempoTimelineMajorNudgeSpacing);
		vEndNudge = static_cast<int32>((p_oClippingRect.m_vRight) * m_vTimeZoomScale / m_vTempoTimelineMajorNudgeSpacing);
		for(vIndex = vStartNudge; vIndex <= vEndNudge; vIndex++)
		{
			vCurrentXPos = vIndex * m_vTempoTimelineMajorNudgeSpacing / m_vTimeZoomScale;
			m_opScrollingView -> StrokeLine(vCurrentXPos, p_oDestination.m_vBottom - MAJOR_NUDGE_SIZE, vCurrentXPos, p_oDestination.m_vBottom);
			if(m_vTempoTimelineDrawMajorText)
			{
				sprintf(m_vString, "%ldk", vIndex);
				m_opScrollingView -> SetHighColor(COLOR_TEXT);
				m_opScrollingView -> DrawString(vCurrentXPos + 2, p_oDestination.m_vTop + 8, m_vString);
				m_opScrollingView -> SetHighColor(COLOR_NUDGES);
			}
		}
		break;
	case DRAW_MODE_1000_MIN:
		vStartNudge = static_cast<int32>((p_oClippingRect.m_vLeft * m_vTimeZoomScale) / m_vTempoTimelineMajorNudgeSpacing) * 10;
		vEndNudge = static_cast<int32>((p_oClippingRect.m_vRight) * m_vTimeZoomScale / m_vTempoTimelineMinorNudgeSpacing);
		vMajorNudgeCountdown = 10 - (vStartNudge % 10);
		if(vMajorNudgeCountdown == 10)
			vMajorNudgeCountdown = 0;
		for(vIndex = vStartNudge; vIndex <= vEndNudge; vIndex++)
		{
			vCurrentXPos = vIndex * m_vTempoTimelineMinorNudgeSpacing / m_vTimeZoomScale;
			if(vMajorNudgeCountdown == 0)
			{
				m_opScrollingView -> StrokeLine(vCurrentXPos, p_oDestination.m_vBottom - MAJOR_NUDGE_SIZE, vCurrentXPos, p_oDestination.m_vBottom);
				sprintf(m_vString, "%ldk", (vIndex / 10));
				m_opScrollingView -> SetHighColor(COLOR_TEXT);
				m_opScrollingView -> DrawString(vCurrentXPos + 2, p_oDestination.m_vTop + 8, m_vString);
				m_opScrollingView -> SetHighColor(COLOR_NUDGES);
				vMajorNudgeCountdown = 9;
			}
			else
			{
				m_opScrollingView -> StrokeLine(vCurrentXPos, p_oDestination.m_vBottom - MINOR_NUDGE_SIZE, vCurrentXPos, p_oDestination.m_vBottom);
				if(m_vTempoTimelineDrawMinorText)
				{
					sprintf(m_vString, "%ld", (1 + ((vIndex % 10) * 100)));
					m_opScrollingView -> DrawString(vCurrentXPos + 2, p_oDestination.m_vTop + 10, m_vString);
				}
				vMajorNudgeCountdown--;
			}
		}
		break;
	case DRAW_MODE_100:
		vStartNudge = static_cast<int32>((p_oClippingRect.m_vLeft) * m_vTimeZoomScale / m_vTempoTimelineMajorNudgeSpacing);
		vEndNudge = static_cast<int32>((p_oClippingRect.m_vRight) * m_vTimeZoomScale / m_vTempoTimelineMajorNudgeSpacing);
		for(vIndex = vStartNudge; vIndex <= vEndNudge; vIndex++)
		{
			vCurrentXPos = vIndex * m_vTempoTimelineMajorNudgeSpacing / m_vTimeZoomScale;
			m_opScrollingView -> StrokeLine(vCurrentXPos, p_oDestination.m_vBottom - MAJOR_NUDGE_SIZE, vCurrentXPos, p_oDestination.m_vBottom);
			sprintf(m_vString, "%ld", (1 + vIndex * 100));
			m_opScrollingView -> SetHighColor(COLOR_TEXT);
			m_opScrollingView -> DrawString(vCurrentXPos + 2, p_oDestination.m_vTop + 8, m_vString);
			m_opScrollingView -> SetHighColor(COLOR_NUDGES);
		}
		break;
	case DRAW_MODE_100_MIN:
		vStartNudge = static_cast<int32>((p_oClippingRect.m_vLeft * m_vTimeZoomScale) / m_vTempoTimelineMajorNudgeSpacing) * 10;
		vEndNudge = static_cast<int32>((p_oClippingRect.m_vRight) * m_vTimeZoomScale / m_vTempoTimelineMinorNudgeSpacing);
		vMajorNudgeCountdown = 10 - (vStartNudge % 10);
		if(vMajorNudgeCountdown == 10)
			vMajorNudgeCountdown = 0;
		for(vIndex = vStartNudge; vIndex <= vEndNudge; vIndex++)
		{
			vCurrentXPos = vIndex * m_vTempoTimelineMinorNudgeSpacing / m_vTimeZoomScale;
			if(vMajorNudgeCountdown == 0)
			{
				m_opScrollingView -> StrokeLine(vCurrentXPos, p_oDestination.m_vBottom - MAJOR_NUDGE_SIZE, vCurrentXPos, p_oDestination.m_vBottom);
				sprintf(m_vString, "%ld", (1 + vIndex * 10));
				m_opScrollingView -> SetHighColor(COLOR_TEXT);
				m_opScrollingView -> DrawString(vCurrentXPos + 2, p_oDestination.m_vTop + 8, m_vString);
				m_opScrollingView -> SetHighColor(COLOR_NUDGES);
				vMajorNudgeCountdown = 9;
			}
			else
			{
				m_opScrollingView -> StrokeLine(vCurrentXPos, p_oDestination.m_vBottom - MINOR_NUDGE_SIZE, vCurrentXPos, p_oDestination.m_vBottom);
				if(m_vTempoTimelineDrawMinorText)
				{
					sprintf(m_vString, "%ld", (1 + ((vIndex % 10) * 10)));
					m_opScrollingView -> DrawString(vCurrentXPos + 2, p_oDestination.m_vTop + 10, m_vString);
				}
				vMajorNudgeCountdown--;
			}
		}
		break;
	case DRAW_MODE_10:
		vStartNudge = static_cast<int32>((p_oClippingRect.m_vLeft) * m_vTimeZoomScale / m_vTempoTimelineMajorNudgeSpacing);
		vEndNudge = static_cast<int32>((p_oClippingRect.m_vRight) * m_vTimeZoomScale / m_vTempoTimelineMajorNudgeSpacing);
		for(vIndex = vStartNudge; vIndex <= vEndNudge; vIndex++)
		{
			vCurrentXPos = vIndex * m_vTempoTimelineMajorNudgeSpacing / m_vTimeZoomScale;
			m_opScrollingView -> StrokeLine(vCurrentXPos, p_oDestination.m_vBottom - MAJOR_NUDGE_SIZE, vCurrentXPos, p_oDestination.m_vBottom);
			sprintf(m_vString, "%ld", (1 + vIndex * 10));
			m_opScrollingView -> SetHighColor(COLOR_TEXT);
			m_opScrollingView -> DrawString(vCurrentXPos + 2, p_oDestination.m_vTop + 8, m_vString);
			m_opScrollingView -> SetHighColor(COLOR_NUDGES);
		}
		break;
	case DRAW_MODE_10_MIN:
		vStartNudge = static_cast<int32>((p_oClippingRect.m_vLeft * m_vTimeZoomScale) / m_vTempoTimelineMajorNudgeSpacing) * 10;
		vEndNudge = static_cast<int32>((p_oClippingRect.m_vRight) * m_vTimeZoomScale / m_vTempoTimelineMinorNudgeSpacing);
		vMajorNudgeCountdown = 10 - (vStartNudge % 10);
		if(vMajorNudgeCountdown == 10)
			vMajorNudgeCountdown = 0;
		for(vIndex = vStartNudge; vIndex <= vEndNudge; vIndex++)
		{
			vCurrentXPos = vIndex * m_vTempoTimelineMinorNudgeSpacing / m_vTimeZoomScale;
			if(vMajorNudgeCountdown == 0)
			{
				m_opScrollingView -> StrokeLine(vCurrentXPos, p_oDestination.m_vBottom - MAJOR_NUDGE_SIZE, vCurrentXPos, p_oDestination.m_vBottom);
				sprintf(m_vString, "%ld", (1 + vIndex));
				m_opScrollingView -> SetHighColor(COLOR_TEXT);
				m_opScrollingView -> DrawString(vCurrentXPos + 2, p_oDestination.m_vTop + 8, m_vString);
				m_opScrollingView -> SetHighColor(COLOR_NUDGES);
				vMajorNudgeCountdown = 9;
			}
			else
			{
				m_opScrollingView -> StrokeLine(vCurrentXPos, p_oDestination.m_vBottom - MINOR_NUDGE_SIZE, vCurrentXPos, p_oDestination.m_vBottom);
				if(m_vTempoTimelineDrawMinorText)
				{
					sprintf(m_vString, "%ld", (1 + (vIndex % 10)));
					m_opScrollingView -> DrawString(vCurrentXPos + 2, p_oDestination.m_vTop + 10, m_vString);
				}
				vMajorNudgeCountdown--;
			}
		}
		break;
	case DRAW_MODE_1:
		vStartNudge = static_cast<int32>((p_oClippingRect.m_vLeft) * m_vTimeZoomScale / m_vTempoTimelineMajorNudgeSpacing);
		vEndNudge = static_cast<int32>((p_oClippingRect.m_vRight) * m_vTimeZoomScale / m_vTempoTimelineMajorNudgeSpacing);
		for(vIndex = vStartNudge; vIndex <= vEndNudge; vIndex++)
		{
			vCurrentXPos = vIndex * m_vTempoTimelineMajorNudgeSpacing / m_vTimeZoomScale;
			m_opScrollingView -> StrokeLine(vCurrentXPos, p_oDestination.m_vBottom - MAJOR_NUDGE_SIZE, vCurrentXPos, p_oDestination.m_vBottom);
			sprintf(m_vString, "%ld", (1 + vIndex));
			m_opScrollingView -> SetHighColor(COLOR_TEXT);
			m_opScrollingView -> DrawString(vCurrentXPos + 2, p_oDestination.m_vTop + 8, m_vString);
			m_opScrollingView -> SetHighColor(COLOR_NUDGES);
		}
		break;
	case DRAW_MODE_1_MIN:
		vStartNudge = static_cast<int32>((p_oClippingRect.m_vLeft * m_vTimeZoomScale) / m_vTempoTimelineMajorNudgeSpacing) * m_vSignatureMajor;
		vEndNudge = static_cast<int32>((p_oClippingRect.m_vRight) * m_vTimeZoomScale / m_vTempoTimelineMinorNudgeSpacing);
		vMajorNudgeCountdown = m_vSignatureMajor - (vStartNudge % m_vSignatureMajor);
		if(vMajorNudgeCountdown == m_vSignatureMajor)
			vMajorNudgeCountdown = 0;
		for(vIndex = vStartNudge; vIndex <= vEndNudge; vIndex++)
		{
			vCurrentXPos = vIndex * m_vTempoTimelineMinorNudgeSpacing / m_vTimeZoomScale;
			if(vMajorNudgeCountdown == 0)
			{
				m_opScrollingView -> StrokeLine(vCurrentXPos, p_oDestination.m_vBottom - MAJOR_NUDGE_SIZE, vCurrentXPos, p_oDestination.m_vBottom);
				sprintf(m_vString, "%ld", (1 + (vIndex / m_vSignatureMajor)));
				m_opScrollingView -> SetHighColor(COLOR_TEXT);
				m_opScrollingView -> DrawString(vCurrentXPos + 2, p_oDestination.m_vTop + 8, m_vString);
				m_opScrollingView -> SetHighColor(COLOR_NUDGES);
				vMajorNudgeCountdown = m_vSignatureMajor - 1;
			}
			else
			{
				m_opScrollingView -> StrokeLine(vCurrentXPos, p_oDestination.m_vBottom - MINOR_NUDGE_SIZE, vCurrentXPos, p_oDestination.m_vBottom);
				if(m_vTempoTimelineDrawMinorText)
				{
					sprintf(m_vString, "%ld", (1 + (vIndex % m_vSignatureMajor)));
					m_opScrollingView -> DrawString(vCurrentXPos + 2, p_oDestination.m_vTop + 10, m_vString);
				}
				vMajorNudgeCountdown--;
			}
		}
		break;
	default:
		break;
	}
}

void TrackWindowTimeLine::DrawMouse(EMRect p_oClippingRect)
{
	m_opView -> AssertWindowLocked();

//	m_opScrollingView -> EnableDoubleBuffering(false);

	m_opScrollingView -> SetDrawingMode(EM_OP_INVERT);
	m_opScrollingView -> SetHighColor(EM_COLOR_INVERTED_DRAWING);

	if(p_oClippingRect.IsValid())
	{
		if((p_oClippingRect.m_vLeft <= m_vOldMouseX && p_oClippingRect.m_vRight >= m_vOldMouseX))
			m_opScrollingView -> StrokeLine(m_vOldMouseX, p_oClippingRect.m_vTop, m_vOldMouseX, p_oClippingRect.m_vBottom);
	}
	else
	{
		int32 vMouseX;
		int32 vMouseY;
		bool vButton;
	
		m_opScrollingView -> GetMouse(vMouseX, vMouseY, vButton, vButton, vButton);
		EMRect oViewBounds = m_opScrollingView -> Bounds();
		if(m_vOldMouseX >= oViewBounds.m_vLeft && m_vOldMouseX <= oViewBounds.m_vRight)
			m_opScrollingView -> StrokeLine(m_vOldMouseX, 0, m_vOldMouseX, (m_opScrollingView -> Bounds()).m_vBottom);
		if(vMouseX >= oViewBounds.m_vLeft && vMouseX <= oViewBounds.m_vRight)
			m_opScrollingView -> StrokeLine(vMouseX, 0, vMouseX, (m_opScrollingView -> Bounds()).m_vBottom);
		//m_opScrollingView -> Sync();
		m_vOldMouseX = vMouseX;
	}

	m_opScrollingView -> SetDrawingMode(EM_OP_COPY);

//	m_opScrollingView -> EnableDoubleBuffering(true);
}

void TrackWindowTimeLine::DrawPosition(EMRect p_oClippingRect)
{
//	m_opScrollingView -> EnableDoubleBuffering(false);

	m_opScrollingView -> SetDrawingMode(EM_OP_INVERT);
	m_opScrollingView -> SetHighColor(EM_COLOR_INVERTED_DRAWING);
	
	int32 vPosition = m_vPosition / m_vTimeZoomScale;

	if(p_oClippingRect.IsValid())
	{
		if((p_oClippingRect.m_vLeft <= m_vOldPosition && p_oClippingRect.m_vRight >= m_vOldPosition))
			m_opScrollingView -> StrokeLine(m_vOldPosition, p_oClippingRect.m_vTop, m_vOldPosition, p_oClippingRect.m_vBottom);
	}
	else
	{
		EMRect oViewBounds = m_opScrollingView -> Bounds();
		if(m_vOldPosition >= oViewBounds.m_vLeft && m_vOldPosition <= oViewBounds.m_vRight)
			m_opScrollingView -> StrokeLine(m_vOldPosition, 0, m_vOldPosition, (m_opScrollingView -> Bounds()).m_vBottom);
		if(vPosition >= oViewBounds.m_vLeft && vPosition <= oViewBounds.m_vRight)
			m_opScrollingView -> StrokeLine(vPosition, 0, vPosition, (m_opScrollingView -> Bounds()).m_vBottom);
		//m_opScrollingView -> Sync();
		m_vOldPosition = vPosition;
	}

	m_opScrollingView -> SetDrawingMode(EM_OP_COPY);

//	m_opScrollingView -> EnableDoubleBuffering(true);
}

void TrackWindowTimeLine::DrawRegion(EMRect p_oClippingRect)
{
	if(m_vRegionExists)
	{
		m_opScrollingView -> SetDrawingMode(EM_OP_ALPHA);
		m_opScrollingView -> SetHighColor(EMColor(0, 0, 0, 100));
		m_opScrollingView -> FillRect(m_oRegion & p_oClippingRect);
		m_opScrollingView -> SetDrawingMode(EM_OP_COPY);
	}
}

void TrackWindowTimeLine::DrawScrolling(EMRect p_oClippingRect)
{
	/*
	uint32 vModifiers = EMKeyboard::Instance() -> GetModifiers();
	if((vModifiers & EM_SCROLL_LOCK) == EM_SCROLL_LOCK)
	{
		m_opScrollingView -> SetHighColor(EMColor(0, 255, 255, 255));
		m_opScrollingView -> FillRect(p_oClippingRect);
		return;
	}
	*/
	if((m_oSignatureButton.GetOn() || m_vIsExpanded) && p_oClippingRect.Intersects(m_oSignatureFrame))
	{
		m_opScrollingView -> SetHighColor(COLOR_SIGNATURE_HIGHLIGHT);
		m_opScrollingView -> StrokeLine(m_oSignatureFrame.m_vLeft, m_oSignatureFrame.m_vTop, m_oSignatureFrame.m_vRight, m_oSignatureFrame.m_vTop);
		m_opScrollingView -> SetHighColor(COLOR_SIGNATURE_SHADOW);
		m_opScrollingView -> StrokeLine(m_oSignatureFrame.m_vLeft, m_oSignatureFrame.m_vBottom, m_oSignatureFrame.m_vRight, m_oSignatureFrame.m_vBottom);
		m_opScrollingView -> SetHighColor(COLOR_SIGNATURE_BACKGROUND);
		m_opScrollingView -> FillRect(EMRect(m_oSignatureFrame.m_vLeft, m_oSignatureFrame.m_vTop + 1, m_oSignatureFrame.m_vRight, m_oSignatureFrame.m_vBottom - 1));
	}
	if((m_oTempoButton.GetOn() || m_vIsExpanded) && p_oClippingRect.Intersects(m_oTempoFrame))
	{
		m_opScrollingView -> SetHighColor(COLOR_TEMPO_HIGHLIGHT);
		m_opScrollingView -> StrokeLine(m_oTempoFrame.m_vLeft, m_oTempoFrame.m_vTop, m_oTempoFrame.m_vRight, m_oTempoFrame.m_vTop);
		m_opScrollingView -> SetHighColor(COLOR_TEMPO_SHADOW);
		m_opScrollingView -> StrokeLine(m_oTempoFrame.m_vLeft, m_oTempoFrame.m_vBottom, m_oTempoFrame.m_vRight, m_oTempoFrame.m_vBottom);
		m_opScrollingView -> SetHighColor(COLOR_TEMPO_BACKGROUND);
		m_opScrollingView -> FillRect(EMRect(m_oTempoFrame.m_vLeft, m_oTempoFrame.m_vTop + 1, m_oTempoFrame.m_vRight, m_oTempoFrame.m_vBottom - 1));
	}
	if((m_oTimeTwoButton.GetOn() || m_vIsExpanded) && p_oClippingRect.Intersects(m_oTimeTwoFrame))
	{
		m_opScrollingView -> SetHighColor(COLOR_TIME_TWO_HIGHLIGHT);
		m_opScrollingView -> StrokeLine(p_oClippingRect.m_vLeft, m_oTimeTwoFrame.m_vTop, p_oClippingRect.m_vRight, m_oTimeTwoFrame.m_vTop);
		m_opScrollingView -> SetHighColor(COLOR_TIME_TWO_SHADOW);
		m_opScrollingView -> StrokeLine(p_oClippingRect.m_vLeft, m_oTimeTwoFrame.m_vBottom, p_oClippingRect.m_vRight, m_oTimeTwoFrame.m_vBottom);
		m_opScrollingView -> SetHighColor(COLOR_TIME_TWO_BACKGROUND);
		m_opScrollingView -> FillRect(EMRect(p_oClippingRect.m_vLeft, m_oTimeTwoFrame.m_vTop + 1, p_oClippingRect.m_vRight, m_oTimeTwoFrame.m_vBottom - 1));
		EMRect oFrame = m_oTimeTwoFrame;
		oFrame.InsetBy(0, 1);
		DrawContentMeasures(p_oClippingRect, oFrame, DRAW_TIME_MEASURES, COLOR_TIME_TWO_BACKGROUND);
	}
	if(p_oClippingRect.Intersects(m_oTimeOneFrame))
	{
		m_opScrollingView -> SetHighColor(COLOR_TIME_ONE_HIGHLIGHT);
		m_opScrollingView -> StrokeLine(p_oClippingRect.m_vLeft, m_oTimeOneFrame.m_vTop, p_oClippingRect.m_vRight, m_oTimeOneFrame.m_vTop);
		m_opScrollingView -> SetHighColor(COLOR_TIME_ONE_SHADOW);
		m_opScrollingView -> StrokeLine(p_oClippingRect.m_vLeft, m_oTimeOneFrame.m_vBottom, p_oClippingRect.m_vRight, m_oTimeOneFrame.m_vBottom);
		m_opScrollingView -> SetHighColor(COLOR_TIME_ONE_BACKGROUND);
		m_opScrollingView -> FillRect(EMRect(p_oClippingRect.m_vLeft, m_oTimeOneFrame.m_vTop + 1, p_oClippingRect.m_vRight, m_oTimeOneFrame.m_vBottom - 1));
		EMRect oFrame = m_oTimeOneFrame;
		oFrame.InsetBy(0, 1);
		DrawContentSMPTE(p_oClippingRect, oFrame, DRAW_TIME_24_FRAMES, COLOR_TIME_ONE_BACKGROUND);
	}
}

EMView* TrackWindowTimeLine::GetView()
{
	return m_opView;
}

void TrackWindowTimeLine::Init()
{
	m_oTimeOneButton.SetBitmaps(EMResourceRepository::GetBitmap(RES_EB_EXPAND_VER_ON), EMResourceRepository::GetBitmap(RES_EB_EXPAND_VER));

	EMSettingsRepository::Instance() -> AddListener(this);
	m_opView -> AddListener(this);
	m_opScrollingView -> AddListener(this);
	LayoutButtons();
	CalculateFrames();
	m_opView -> AddChild(m_opScrollingView);
	m_oRegion.m_vTop = 0;
	m_oRegion.m_vBottom = (m_opScrollingView -> Bounds()).m_vBottom;

	m_vTimeZoomScale = *(static_cast<int64*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_TIME_ZOOM_SCALE)));
	m_vTempo = *(static_cast<float*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_TEMPO)));
	m_opSignature = static_cast<string*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_SIGNATURE));
	sscanf(m_opSignature -> c_str(), "%d/%d", &m_vSignatureMajor, &m_vSignatureMinor);
	m_vFramerate = *(static_cast<float*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_VIDEO_FRAMERATE)));
	m_vMIDIEnabled = *(static_cast<bool*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_MIDI_ENABLED)));
	m_vSnapping = *(static_cast<bool*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_SNAPPING)));
	CalculateMetrics();
	m_vExpertMode = *(static_cast<bool*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_EXPERT_MODE)));
	SetUserMode();
	EMThreadRepository::Instance() -> GetThread(GUI_SERVICE_THREAD_NAME) -> AddListener(this);
}

bool TrackWindowTimeLine::IsDirty()
{
	return m_vIsDirty;
}

void TrackWindowTimeLine::LayoutButtons()
{
	int32 vCurrentYPos = 0;
/*
	if(m_oSignatureButton.GetOn() || m_vIsExpanded)
	{
		m_oSignatureButton.SetVisible(true);
		m_oSignatureButton.SetFrame(EMRect(0, vCurrentYPos, EM_METRICS_TINY_SLIM_BUTTON_WIDTH - 1, vCurrentYPos + EM_METRICS_SLIM_BUTTON_HEIGHT - 1));
		vCurrentYPos += EM_METRICS_SLIM_BUTTON_HEIGHT;
	}
	else
		m_oSignatureButton.SetVisible(false);
		
	if(m_oTempoButton.GetOn() || m_vIsExpanded)
	{
		m_oTempoButton.SetVisible(true);
		m_oTempoButton.SetFrame(EMRect(0, vCurrentYPos, EM_METRICS_TINY_SLIM_BUTTON_WIDTH - 1, vCurrentYPos + EM_METRICS_SLIM_BUTTON_HEIGHT - 1));
		vCurrentYPos += EM_METRICS_SLIM_BUTTON_HEIGHT;
	}
	else
		m_oTempoButton.SetVisible(false);
*/		
	if(m_oTimeTwoButton.GetOn() || m_vIsExpanded)
	{
		m_oTimeTwoButton.SetVisible(true);
		m_oTimeTwoButton.SetFrame(EMRect(0, vCurrentYPos, EM_METRICS_TINY_SLIM_BUTTON_WIDTH - 1, vCurrentYPos + EM_METRICS_SLIM_BUTTON_HEIGHT - 1));
		vCurrentYPos += EM_METRICS_SLIM_BUTTON_HEIGHT;
	}
	else
		m_oTimeTwoButton.SetVisible(false);
/*		
	// FIXME: Temporary stuff
	m_oSignatureButton.SetVisible(false);
	m_oTempoButton.SetVisible(false);
*/
	m_oTimeOneButton.SetFrame(EMRect(0, vCurrentYPos, EM_METRICS_TINY_SLIM_BUTTON_WIDTH - 1, vCurrentYPos + EM_METRICS_SLIM_BUTTON_HEIGHT - 1));
}

bool TrackWindowTimeLine::LoadData(void* p_upProjectDataLoader)
{
	EMProjectDataLoader* opLoader = static_cast<EMProjectDataLoader*>(p_upProjectDataLoader);
	m_oTimeOneButton.SetOn(opLoader -> LoadBool());
	m_vIsExpanded = m_oTimeOneButton.GetOn();

//	m_vIsDirty = false;

	return true;
}

bool TrackWindowTimeLine::MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage)
{
	int32 vX;
	int32 vY;
	bool vButtonOne;
	bool vButtonTwo;
	bool vButtonThree;
	EMView* opView;
	int64 vTimeZoomScale;
	string* opSignature;
	float vTempo;
	bool vMetricsChanged;
//	string* opSnappingMode;
	bool vExpertMode;
	
	switch(p_vMessage)
	{
	case EM_DRAW:
		opView = dynamic_cast<EMView*>(p_opSender);
		if(opView == m_opView)
		{
			Draw(m_opView -> GetClippingRect());
			return true;
		}
		else if(opView == m_opScrollingView)
		{
//			m_opScrollingView -> LockBuffer();
			EMRect oRect = m_opScrollingView -> GetClippingRect();
			if(!(m_opScrollingView -> Bounds()).Contains(oRect))
			{
//(m_opScrollingView -> Bounds()).Print();
//oRect.Print();
				CalculateBounds();
				CalculateFrames();
//;//cout_commented_out_4_release << "Draw problems!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
//(m_opScrollingView -> Bounds()).Print();
			}
			DrawScrolling(oRect);
			DrawRegion(oRect);
//			m_opScrollingView -> DrawAndUnlockBuffer(oRect);
			DrawMouse(oRect);
			DrawPosition(oRect);
			return true;
		}
		return false;
	case EM_MOUSE_DOWN:
		opView = dynamic_cast<EMView*>(p_opSender);
		if(opView == m_opView)
		{
			m_vMouseButtonPressed = true;	
			m_opView -> GetMouse(vX, vY, vButtonOne, vButtonTwo, vButtonThree);
			MouseDown(vX, vY, vButtonOne, vButtonTwo, vButtonThree);
			return true;
		}
		else if(opView == m_opScrollingView)
		{
			m_vMouseButtonPressed = true;	
			m_opScrollingView -> GetMouse(vX, vY, vButtonOne, vButtonTwo, vButtonThree);
			MouseDownScrolling(vX, vY, vButtonOne, vButtonTwo, vButtonThree);
			return true;
		}
		return false;
	case EM_MOUSE_MOVED:
		m_opScrollingView -> BeginPaint();
		DrawMouse(EMRect());
		opView = dynamic_cast<EMView*>(p_opSender);
		if(opView == m_opScrollingView)// && m_vMouseButtonPressed)
		{
			m_opScrollingView -> GetMouse(vX, vY, vButtonOne, vButtonTwo, vButtonThree);
			MouseMovedScrolling(vX, vY, vButtonOne, vButtonTwo, vButtonThree);
			m_opScrollingView -> EndPaint();
			return true;
		}
		m_opScrollingView -> EndPaint();
		return false;
	case EM_MOUSE_UP:
		m_vMouseButtonPressed = false;	
		opView = dynamic_cast<EMView*>(p_opSender);
		if(opView == m_opScrollingView)
		{
			m_opScrollingView -> GetMouse(vX, vY, vButtonOne, vButtonTwo, vButtonThree);
			MouseUpScrolling(vX, vY);
			return true;
		}
		return false;		
	case EM_VIEW_BOUNDS_CHANGED:
		opView = dynamic_cast<EMView*>(p_opSender);
		if(opView == m_opView)
		{
//			if(m_opView -> LockWindow())
			{
				CalculateBounds();
//				m_opView -> UnlockWindow();
			}
			return true;
		}
		else if(opView == m_opScrollingView)
		{
//			if(m_opScrollingView -> LockWindow())
			{
				m_opScrollingView -> UpdateBufferFrame();
				CalculateFrames();
//m_opScrollingView -> Invalidate(m_opScrollingView -> Bounds()); // FIXME: Should not have to do this
//				m_opScrollingView -> UnlockWindow();
			}
			return true;
		}
		return false;
	case EM_SCROLLBAR_CHANGED:
//		if(m_opScrollingView -> LockWindow())
		{
			m_opScrollingView -> AssertWindowLocked();
			
			m_vScrollOffsetX = dynamic_cast<EMScrollbar*>(p_opSender) -> GetValue();
//			if(m_opScrollingView -> LockBuffer())
			{
				m_opScrollingView -> ScrollTo(m_vScrollOffsetX, 0);
//				m_opScrollingView -> UnlockBuffer();
			}
			CalculateFrames();
//			m_opScrollingView -> UnlockWindow();
		}
		return true;
	case EM_SETTING_UPDATED:
eo << "TrackWindowTimeLine: EM_SETTING_UPDATED" << ef;
		if(m_opScrollingView -> LockWindow())
		{
			vMetricsChanged = false;
			vTimeZoomScale = *(static_cast<int64*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_TIME_ZOOM_SCALE)));
			if(vTimeZoomScale != m_vTimeZoomScale)
			{
				m_vTimeZoomScale = vTimeZoomScale;
				m_oRegion = EMRect(m_vRegionBegin / m_vTimeZoomScale, 0, m_vRegionEnd / m_vTimeZoomScale, (m_opScrollingView -> Bounds()).m_vBottom);
				vMetricsChanged = true;
			}
			opSignature = static_cast<string*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_SIGNATURE));
			if(opSignature != m_opSignature)
			{
				m_opSignature = opSignature;
				sscanf(m_opSignature -> c_str(), "%d/%d", &m_vSignatureMajor, &m_vSignatureMinor);
				vMetricsChanged = true;
			}
			vTempo = *(static_cast<float*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_TEMPO)));
			if(vTempo != m_vTempo)
			{
				m_vTempo = vTempo;
				vMetricsChanged = true;
			}
			float vFramerate = *(static_cast<float*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_VIDEO_FRAMERATE)));
			if(vFramerate != m_vFramerate)
			{
				m_vFramerate = vFramerate;
				vMetricsChanged = true;
			}				
/*
			opSnappingMode = static_cast<string*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_SNAPPING_MODE));
			if(opSnappingMode != m_opSnappingMode)
			{
				m_opSnappingMode = opSnappingMode;
				CalculateSnappingMode();
			}
*/
			m_vSnapping = *(static_cast<bool*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_SNAPPING)));
			if(vMetricsChanged)
			{
				CalculateMetrics();
				m_opScrollingView -> Invalidate(m_opScrollingView -> Bounds());
			}

			vExpertMode = *(static_cast<bool*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_EXPERT_MODE)));
			if(vExpertMode != m_vExpertMode)
			{
				m_vExpertMode = vExpertMode;
				SetUserMode();
				m_opView -> Invalidate(m_opView -> Bounds());
			}

			bool vMIDIEnabled = *(static_cast<bool*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_MIDI_ENABLED)));
			if(vMIDIEnabled != m_vMIDIEnabled)
			{
				m_vMIDIEnabled = vMIDIEnabled;
				if(m_oTimeOneButton.GetOn() != m_vMIDIEnabled)
				{
					m_oTimeOneButton.SetOn(m_vMIDIEnabled);
					EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_EXPAND_TIMELINE);
				}
				m_opView -> Invalidate(m_opView -> Bounds());
			}

			int64 vRegionBegin = *(static_cast<int64*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_REGION_BEGIN)));
			int64 vRegionEnd = *(static_cast<int64*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_REGION_END)));
			if(vRegionBegin != m_vRegionBegin || vRegionEnd != m_vRegionEnd)
			{
				m_vRegionBegin = vRegionBegin;
				m_vRegionEnd = vRegionEnd;
				if(m_vRegionBegin != -1 && m_vRegionEnd != -1) // If a region exists
				{
					m_oRegion = EMRect(m_vRegionBegin / m_vTimeZoomScale, 0, m_vRegionEnd / m_vTimeZoomScale, (m_opScrollingView -> Bounds()).m_vBottom);
					m_vRegionExists = true;
				}
				else
					m_vRegionExists = false;
				m_opScrollingView -> Invalidate(m_opView -> Bounds());
			}

			m_opScrollingView -> UnlockWindow();
		}
		return true;
	default:
		return false;
	}
}

void TrackWindowTimeLine::MouseDown(int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree)
{
	if(m_oTimeOneButton.MouseDown(p_vX, p_vY))
	{
		m_opView -> Invalidate(m_oTimeOneButton.Frame());
		return;
	}

	if(m_vIsExpanded)
	{
		if(m_oSignatureButton.MouseDown(p_vX, p_vY))
			m_opView -> Invalidate(m_oSignatureButton.Frame());
		else if(m_oTempoButton.MouseDown(p_vX, p_vY))
			m_opView -> Invalidate(m_oTempoButton.Frame());
		else if(m_oTimeTwoButton.MouseDown(p_vX, p_vY))
			m_opView -> Invalidate(m_oTimeTwoButton.Frame());
	}
	else
	{
		m_oSignatureButton.MouseDown(p_vX, p_vY);
		m_oTempoButton.MouseDown(p_vX, p_vY);
		m_oTimeTwoButton.MouseDown(p_vX, p_vY);
		EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_EXPAND_TIMELINE);

		m_vIsDirty = true;
	}
}

void TrackWindowTimeLine::MouseDownScrolling(int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree)
{
//;//cout_commented_out_4_release << "TrackWindowTimeLine::MouseDownScrolling: " << p_vX << ", " << p_vY << ", position: " << p_vX * m_vTimeZoomScale << ", fps: " << m_vFramerate << endl;
	int32 vLeftRegionThreshold = abs(m_oRegion.m_vLeft - p_vX);
	int32 vRightRegionThreshold = abs(m_oRegion.m_vRight - p_vX);

	if(m_vRegionExists && (vLeftRegionThreshold < RESIZE_REGION_THRESHOLD || vRightRegionThreshold < RESIZE_REGION_THRESHOLD))
	{
		if(vLeftRegionThreshold < vRightRegionThreshold)
			m_vResizeRegionLeft = true;
		else
			m_vResizeRegionRight = true;
	}

	else if(p_vButtonOne)
	{
		//int64* vpPosition = EM_new int64(static_cast<int64>(p_vX) * m_vTimeZoomScale);
		int64 vPosition = p_vX * m_vTimeZoomScale;

		EMGUIUtilities::Instance() -> GetSnapPosition(&vPosition, false, false, NULL, true, true);

		// The window is unlocked temporarily to prevent deadlocks, FIXME: Is this really needed anymore?
		m_opView -> UnlockWindow();

		if(*(static_cast<bool*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_IS_PLAYING, NULL, NULL, NULL))))
		{
			EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_STOP_PLAYBACK);
			m_vHasSeeked = true;
		}
		EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_SEEK, &vPosition);
		EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_PLAY_BUFFER);

		m_opView -> LockWindow();
	}
	else if(p_vButtonTwo)
	{
		m_vRegionHasChanged = true;

		if(m_vRegionExists)
			m_opScrollingView -> Invalidate(m_oRegion);
		m_vRegionExists = false;
		m_vMouseDownX = p_vX;
		int64 vMovedPosition = static_cast<int64>(p_vX * m_vTimeZoomScale);
		if(EMGUIUtilities::Instance() -> GetSnapPosition(&vMovedPosition, true, false))
			m_vMouseDownX = vMovedPosition / m_vTimeZoomScale;
		m_vRegionBegin = vMovedPosition;
	}
}

void TrackWindowTimeLine::MouseMovedScrolling(int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree)
{
	if(m_vResizeRegionLeft || m_vResizeRegionRight || m_vRegionHasChanged || (!p_vButtonOne && (
		(m_vRegionExists && abs(m_oRegion.m_vLeft - p_vX) < RESIZE_REGION_THRESHOLD) || 
		(m_vRegionExists && abs(m_oRegion.m_vRight - p_vX) < RESIZE_REGION_THRESHOLD))))
	{
		m_opScrollingView -> SetMouseCursor(EM_MOUSE_CURSOR_H_ARROWS);
	}

	if(!p_vButtonOne && !p_vButtonTwo)
		return;

	if(m_vResizeRegionLeft || m_vResizeRegionRight)
	{
		m_vRegionHasChanged = true;

		if(p_vX < 0)
			p_vX = 0;

		int64 vPosition = p_vX * m_vTimeZoomScale;
		EMGUIUtilities::Instance() -> GetSnapPosition(&vPosition, true, false, NULL, true, true);

		EMRect oOldRegion = m_oRegion;

		if(m_vResizeRegionLeft)
		{
			m_vRegionBegin = vPosition;
			m_oRegion.m_vLeft = vPosition / m_vTimeZoomScale;
			if(m_oRegion.m_vLeft >= m_oRegion.m_vRight)
			{
				m_oRegion.m_vLeft = m_oRegion.m_vRight - 1;
				m_vRegionBegin = (m_oRegion.m_vRight - 1) * m_vTimeZoomScale;
			}
			
			if(m_oRegion.m_vLeft < oOldRegion.m_vLeft)
				m_opScrollingView -> Invalidate(EMRect(m_oRegion.m_vLeft, m_oRegion.m_vTop, oOldRegion.m_vLeft, m_oRegion.m_vBottom));
			else
				m_opScrollingView -> Invalidate(EMRect(oOldRegion.m_vLeft, m_oRegion.m_vTop, m_oRegion.m_vLeft, m_oRegion.m_vBottom));
		}
		else
		{
			m_vRegionEnd = vPosition;
			m_oRegion.m_vRight = vPosition / m_vTimeZoomScale;
			if(m_oRegion.m_vRight <= m_oRegion.m_vLeft)
			{
				m_oRegion.m_vRight = m_oRegion.m_vLeft + 1;
				m_vRegionEnd = (m_oRegion.m_vLeft + 1) * m_vTimeZoomScale;
			}

			if(m_oRegion.m_vRight < oOldRegion.m_vRight)
				m_opScrollingView -> Invalidate(EMRect(m_oRegion.m_vRight, m_oRegion.m_vTop, oOldRegion.m_vRight, m_oRegion.m_vBottom));
			else
				m_opScrollingView -> Invalidate(EMRect(oOldRegion.m_vRight, m_oRegion.m_vTop, m_oRegion.m_vRight, m_oRegion.m_vBottom));
		}
	}

	else if(p_vButtonOne)
	{
		if(p_vX < 0)
			p_vX = 0;

		int64 vPosition = p_vX * m_vTimeZoomScale;
		EMGUIUtilities::Instance() -> GetSnapPosition(&vPosition, false, false, NULL, true, false);

		// The window is unlocked temporarily to prevent deadlocks, FIXME: Is this needed anymore?
		m_opView -> UnlockWindow();

		EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_SEEK, &vPosition);
		EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_PLAY_BUFFER);

		m_opView -> LockWindow();
	}

	else if(p_vButtonTwo)
	{
		int64 vMovedPosition = static_cast<int64>(p_vX * m_vTimeZoomScale);
		if(vMovedPosition < 0)
			vMovedPosition = 0;
		if(m_vSnapping)
			EMGUIUtilities::Instance() -> GetSnapPosition(&vMovedPosition, true, false, NULL, true, true);
		
		int32 vNewX = vMovedPosition / m_vTimeZoomScale;
		
		EMRect oOldRegion = m_oRegion;
		if(vNewX != m_vMouseDownX)
			m_vRegionExists = true;
		else
		{
			m_vRegionExists = false;
			m_oRegion = EMRect();
		}
		
		if(vNewX < m_vMouseDownX)
			m_oRegion = EMRect(vNewX, 0, m_vMouseDownX, (m_opScrollingView -> Bounds()).m_vBottom);
		else
			m_oRegion = EMRect(m_vMouseDownX, 0, vNewX, (m_opScrollingView -> Bounds()).m_vBottom);
		m_vRegionEnd = vMovedPosition;

		// Determine what has to be redrawn
		EMRect oInvalidateRect(m_oRegion | oOldRegion);
		if(m_oRegion.m_vLeft == oOldRegion.m_vLeft)
		{
			if(m_oRegion.m_vRight > oOldRegion.m_vRight)
				oInvalidateRect.m_vLeft = oOldRegion.m_vRight + 1;
			else
				oInvalidateRect.m_vLeft = m_oRegion.m_vRight + 1;
		}
		else if(m_oRegion.m_vRight == oOldRegion.m_vRight)
		{
			if(m_oRegion.m_vLeft < oOldRegion.m_vLeft)
				oInvalidateRect.m_vRight = oOldRegion.m_vLeft - 1;
			else
				oInvalidateRect.m_vRight = m_oRegion.m_vLeft - 1;
		}
		// Else leave the invalidate rect unchanged
		m_opScrollingView -> Invalidate(oInvalidateRect);
	}
}

void TrackWindowTimeLine::MouseUpScrolling(int32 p_vX, int32 p_vY)
{
	if(m_vHasSeeked)
	{
		EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_START_PLAYBACK);
		m_vHasSeeked = false;
	}
/*
	else
	{
		if(p_vX < 0)
			p_vX = 0;
		int64* vpPosition = EM_new int64(static_cast<int64>(p_vX) * m_vTimeZoomScale);
		EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_SEEK, vpPosition);
		EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_PLAY_BUFFER);
	}
*/
	if(m_vRegionHasChanged)
	{
		if(m_vRegionExists)
		{
			if(m_vResizeRegionLeft)
				EMSettingsRepository::Instance() -> SetSetting(SETTING_REGION_BEGIN, EM_SETTING_INT64, &m_vRegionBegin);

			else if(m_vResizeRegionRight)
				EMSettingsRepository::Instance() -> SetSetting(SETTING_REGION_END, EM_SETTING_INT64, &m_vRegionEnd);

			else if(m_vRegionBegin < m_vRegionEnd)
			{
				if(m_vRegionBegin < 0)
					m_vRegionBegin = 0;
				EMSettingsRepository::Instance() -> SetSetting(SETTING_REGION_BEGIN, EM_SETTING_INT64, &m_vRegionBegin, false);
				EMSettingsRepository::Instance() -> SetSetting(SETTING_REGION_END, EM_SETTING_INT64, &m_vRegionEnd);
			}
			else
			{
				if(m_vRegionEnd < 0)
					m_vRegionEnd = 0;
				EMSettingsRepository::Instance() -> SetSetting(SETTING_REGION_BEGIN, EM_SETTING_INT64, &m_vRegionEnd, false);
				EMSettingsRepository::Instance() -> SetSetting(SETTING_REGION_END, EM_SETTING_INT64, &m_vRegionBegin);
			}
		}
		else
		{
			int64 vRange(-1);
			bool vLoop(false);
			EMSettingsRepository::Instance() -> SetSetting(SETTING_LOOP, EM_SETTING_BOOL, &vLoop, false);
			EMSettingsRepository::Instance() -> SetSetting(SETTING_REGION_BEGIN, EM_SETTING_INT64, &vRange, false);
			EMSettingsRepository::Instance() -> SetSetting(SETTING_REGION_END, EM_SETTING_INT64, &vRange);
		}

		m_vRegionHasChanged = false;
		m_vResizeRegionLeft = false;
		m_vResizeRegionRight = false;
	}
}

EMRect TrackWindowTimeLine::PreferredBounds()
{
	int32 vWidth = MAX_VIEW_PIXEL_LIMIT;
	int32 vHeight = EM_METRICS_SLIM_BUTTON_HEIGHT - 1 + 3;
/*
	if(m_oSignatureButton.GetVisible())
		vHeight += m_oSignatureButton.Frame().GetHeight() + 1;
	if(m_oTempoButton.GetVisible())
		vHeight += m_oTempoButton.Frame().GetHeight() + 1;
	if(m_oTimeTwoButton.GetVisible())
		vHeight += m_oTimeTwoButton.Frame().GetHeight() + 1;
*/
	if(m_oTimeOneButton.GetOn())
		vHeight += m_oTimeTwoButton.Frame().GetHeight() + 1; // Temporary
	return EMRect(0, 0, vWidth, vHeight);
}

void TrackWindowTimeLine::ReadyForUse()
{
	if(m_opView -> LockWindow())
	{
		CalculateFrames();
		LayoutButtons();
/*
		m_vRegionBegin = *(static_cast<int64*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_REGION_BEGIN)));
		m_vRegionEnd = *(static_cast<int64*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_REGION_END)));
		if(m_vRegionBegin != -1 && m_vRegionEnd != -1) // If a region exists
		{
			m_oRegion = EMRect(m_vRegionBegin / m_vTimeZoomScale, 0, m_vRegionEnd / m_vTimeZoomScale, m_oRegion.m_vBottom);
			m_vRegionExists = true;
		}
		else
			m_vRegionExists = false;

		m_opView -> Invalidate(m_opView -> Bounds());
*/
		m_opView -> UnlockWindow();
	}
}

bool TrackWindowTimeLine::SaveData(void* p_upDataSaver)
{
	EMProjectDataSaver* opSaver = static_cast<EMProjectDataSaver*>(p_upDataSaver);
	opSaver -> SaveBool(m_oTimeOneButton.GetOn()); // The expand button

//	m_vIsDirty = false;
	
	return true;
}

void TrackWindowTimeLine::SetExpand()
{
	if(EMCommandRepository::Instance() -> TriggeredByBinding())
		m_oTimeOneButton.ToggleOn();
	m_vIsExpanded = m_oTimeOneButton.GetOn();
	LayoutButtons();
	CalculateFrames();
	if(m_oRegion.IsValid())
		m_oRegion.m_vBottom = m_oTimeOneFrame.m_vBottom;
/*
	bool vMustUnlock(false);
	if(!m_opView -> WindowIsLocked())
	{
		vMustUnlock = true;
		m_opView -> LockWindow();
	}
*/
	m_opView -> Invalidate(m_opView -> Bounds());
	m_opScrollingView -> Invalidate(m_opScrollingView -> Bounds());

//	if(vMustUnlock)
//		m_opView -> UnlockWindow();
//	;//cout_commented_out_4_release << "TrackWindowTimeLine::SetExpand(), m_vIsExpanded: " << m_vIsExpanded << endl;
}

void TrackWindowTimeLine::SetUserMode()
{
	if(m_vExpertMode)
	{
		m_oSignatureButton.SetMode(EM_SLIM_BUTTON_STAY_DOWN);
		m_oTempoButton.SetMode(EM_SLIM_BUTTON_STAY_DOWN);
		m_oTimeTwoButton.SetMode(EM_SLIM_BUTTON_STAY_DOWN);
		m_oTimeOneButton.SetMode(EM_SLIM_BUTTON_STAY_DOWN);
	}
	else
	{
//		m_oSignatureButton.SetOn(false);
//		m_oTempoButton.SetOn(false);
//		m_oTimeTwoButton.SetOn(false);
		m_oSignatureButton.SetMode(EM_SLIM_BUTTON_DRAW_BLANK);
		m_oTempoButton.SetMode(EM_SLIM_BUTTON_DRAW_BLANK);
		m_oTimeTwoButton.SetMode(EM_SLIM_BUTTON_DRAW_BLANK);
		m_oTimeOneButton.SetMode(EM_SLIM_BUTTON_DRAW_BLANK);
		if(m_oTimeOneButton.GetOn() && !m_vMIDIEnabled)
		{
			m_oTimeOneButton.SetOn(false);
			m_oTimeOneButton.ExecuteCommand();
		}
		else if(!m_oTimeOneButton.GetOn() && m_vMIDIEnabled)
		{
			m_oTimeOneButton.SetOn(true);
			m_oTimeOneButton.ExecuteCommand();
		}
	}
// FIXME: Temporary
m_oSignatureButton.SetMode(EM_SLIM_BUTTON_DRAW_BLANK);
m_oTempoButton.SetMode(EM_SLIM_BUTTON_DRAW_BLANK);
m_oTimeTwoButton.SetMode(EM_SLIM_BUTTON_DRAW_BLANK);
}

void TrackWindowTimeLine::ThreadRun(EMThread* p_opThread)
{
	m_vPosition = *(static_cast<int64*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_CURRENT_PROJECT_POSITION, NULL)));
	if(m_vPosition / m_vTimeZoomScale != m_vOldPosition)
		if(m_opScrollingView -> LockWindow())
		{
//;//cout_commented_out_4_release << "m_vPosition / m_vTimeZoomScale: " << m_vPosition / m_vTimeZoomScale << ", m_vOldPosition: " << m_vOldPosition << endl;
//			m_opScrollingView -> LockBuffer();
			m_opScrollingView -> BeginPaint();
			DrawPosition(EMRect());
			m_opScrollingView -> EndPaint();
//			m_opScrollingView -> DrawAndUnlockBuffer();
			m_opScrollingView -> UnlockWindow();
		}
}
