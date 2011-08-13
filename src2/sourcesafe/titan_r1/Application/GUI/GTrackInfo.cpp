#include "GTrackInfo.h"

#include "CommandIDs.h"
#include "EMCommandRepository.h"
#include "EMGTrack.h"
#include "EMGTrackRepository.h"
#include "EMMediaGlobals.h"
#include "EMMessages.h"
#include "EMProjectDataLoader.h"
#include "EMProjectDataSaver.h"
#include "EMResourceRepository.h"
#include "EMView.h"
#include "GUIGlobals.h"
#include "ResourceIDs.h"
#include "TrackInfoButtonBar.h"

#include <vector>
#include <string>

const uint32 MESSAGE_INPUT_CHANGED = 1;
const uint32 MESSAGE_OUTPUT_CHANGED = 2;

GTrackInfo::GTrackInfo(EMView* p_opView, TrackInfoButtonBar* p_opButtonBar) :
m_ePreviousZoomScale(EM_NORMAL),
m_oArmButton(p_opView, EMRect(), NULL, NULL, 0, NULL, NULL, NULL,
				true, true, true, true, EM_COLOR_ARM),
m_oEditLabel(EMRect(), EM_COLOR_TEXT_BACKGROUND, p_opView, true, true, true, true, 10),
m_oInputValueBox(p_opView, EMRect(), //vector<string>(),
				COMMAND_MESSAGE_SENDER, NULL, NULL, NULL, true, true, true, true),
m_oKeyOffsetValueBox(p_opView, EMRect(), -100, 100, 0,
				0, NULL, NULL, NULL, true, true, true, true),
m_oLockButton(p_opView, EMRect(), NULL, NULL, 0, NULL, NULL, NULL,
				true, true, true, true, EM_COLOR_LOCK),
m_oMuteButton(p_opView, EMRect(), NULL, NULL, 0, NULL, NULL, NULL,
				true, true, true, true, EM_COLOR_MUTE_USER, EM_COLOR_MUTE_SOLO),
m_oOutputValueBox(p_opView, EMRect(), //vector<string>(),
				0, NULL, NULL, NULL, true, true, true, true),
m_oPanValueBox(p_opView, EMRect(), -63, 63, 0,
				0, NULL, NULL, NULL, true, true, true, true),
m_opButtonBar(p_opButtonBar),
m_opView(p_opView),
m_oRenderButton(p_opView, EMRect(), NULL, NULL, 0, NULL, NULL, NULL,
				true, true, true, true, EM_COLOR_RENDER),
m_oSelectionButton(p_opView, EMRect(), NULL, NULL, 0, NULL, NULL, NULL,
				true, true, true, true),
m_oSignalMeter(EMRect(), p_opView),
m_oSoloButton(p_opView, EMRect(), NULL, NULL, 0, NULL, NULL, NULL,
				true, true, true, true, EM_COLOR_SOLO),
m_oTimeOffsetValueBox(p_opView, EMRect(), 1, 16, 1,//vector<string>(),
				0, NULL, NULL, NULL, true, true, true, true),
m_oVelocityOffsetValueBox(p_opView, EMRect(), -127, 127, 0,//vector<string>(),
				0, NULL, NULL, NULL, true, true, true, true),
m_oVolumeValueBox(p_opView, EMRect(), 0, 127, 100,//vector<string>(),
				0, NULL, NULL, NULL, true, true, true, true),
m_vPreviousSignalOn(true),
m_vUpdateAll(true),
m_vMessageInputChanged(MESSAGE_INPUT_CHANGED),
m_vMessageOutputChanged(MESSAGE_OUTPUT_CHANGED)
{
	m_oEditLabel.AddListener(this);
	m_oInputValueBox.SetCommand(COMMAND_MESSAGE_SENDER, &m_vMessageInputChanged, dynamic_cast<EMListener*>(this), NULL);
	m_oOutputValueBox.SetCommand(COMMAND_MESSAGE_SENDER, &m_vMessageOutputChanged, dynamic_cast<EMListener*>(this), NULL);	

	m_oLockButton.SetMode(EM_SLIM_BUTTON_DRAW_BLANK);
}

GTrackInfo::~GTrackInfo()
{
}

void GTrackInfo::Draw(EMRect p_oClippingRect, EMView* p_opView)
{
//	if(IsHidden(p_vClipEdit))
//	if(!(Frame().IsValid()))
//		return;
/*
	;//cout_commented_out_4_release << "AudioGTrackInfo::Draw" << endl;
	p_oClippingRect.Print();
	Frame().Print();
*/
	if(!(Frame().Intersects(p_oClippingRect)))
		return;
	/*
	p_opView -> SetHighColor(EMColor(255, 255, 150, 255));
	p_opView -> FillRect(Frame() & p_oClippingRect);
	*/
	
	m_oEditLabel.Draw(p_oClippingRect);
	
	m_oArmButton.Draw(p_oClippingRect, p_opView);
	m_oLockButton.Draw(p_oClippingRect, p_opView);
	m_oMuteButton.Draw(p_oClippingRect, p_opView);
	m_oRenderButton.Draw(p_oClippingRect, p_opView);
	m_oSoloButton.Draw(p_oClippingRect, p_opView);
	
	m_oSignalMeter.Draw(p_oClippingRect);
	
	m_oInputValueBox.Draw(p_oClippingRect);
	m_oKeyOffsetValueBox.Draw(p_oClippingRect);
	m_oTimeOffsetValueBox.Draw(p_oClippingRect);
	m_oOutputValueBox.Draw(p_oClippingRect);
	m_oPanValueBox.Draw(p_oClippingRect);
	m_oVelocityOffsetValueBox.Draw(p_oClippingRect);
	m_oVolumeValueBox.Draw(p_oClippingRect);
	
	m_oSelectionButton.Draw(p_oClippingRect, p_opView);
	
	p_opView -> SetHighColor(EM_COLOR_VIEW_BACKGROUND);
	if(m_oEmptySpaceOne.IsValid())
		p_opView -> FillRect(m_oEmptySpaceOne & p_oClippingRect);
	if(m_oEmptySpaceTwo.IsValid())
		p_opView -> FillRect(m_oEmptySpaceTwo & p_oClippingRect);

	p_opView -> SetHighColor(EM_COLOR_VIEW_BACKGROUND_HIGHLIGHT);
	p_opView -> StrokeLine(Frame().m_vLeft, Frame().m_vBottom - 2, Frame().m_vRight, Frame().m_vBottom - 2);
	p_opView -> SetHighColor(EM_COLOR_VIEW_BACKGROUND);
	p_opView -> StrokeLine(Frame().m_vLeft, Frame().m_vBottom - 1, Frame().m_vRight, Frame().m_vBottom - 1);
	p_opView -> SetHighColor(EM_COLOR_VIEW_BACKGROUND_SHADOW);
	p_opView -> StrokeLine(Frame().m_vLeft, Frame().m_vBottom, Frame().m_vRight, Frame().m_vBottom);
	
}

EMSlimButton* GTrackInfo::GetArmButton()
{
	return &m_oArmButton;
}

MuteButton* GTrackInfo::GetMuteButton()
{
	return &m_oMuteButton;
}

SelectionButton* GTrackInfo::GetSelectionButton()
{
	return &m_oSelectionButton;
}

SoloButton* GTrackInfo::GetSoloButton()
{
	return &m_oSoloButton;
}

string GTrackInfo::GetTrackName()
{
	return m_oEditLabel.GetLabel();
}

bool GTrackInfo::IsSelected()
{
	return m_oSelectionButton.GetOn();
}

void GTrackInfo::LayoutComponents()
{
	TrackInfoButtonBar* opButtonBar = TrackInfoButtonBar::Instance();
	EMZoomScale eCurrentZoomScale = GetTrack() -> GetZoomScale();
	bool vCurrentSignalOn = opButtonBar -> GetSignalOn();

	if(((eCurrentZoomScale == EM_SMALL && vCurrentSignalOn) ||
		eCurrentZoomScale == EM_TINY)
		&& (m_ePreviousZoomScale != eCurrentZoomScale || 
		m_vPreviousSignalOn != vCurrentSignalOn || m_vUpdateAll))
	{
		m_oArmButton.SetOnString(EMResourceRepository::GetString(RES_TI_ARM_SMALL));
		m_oArmButton.SetOffString(EMResourceRepository::GetString(RES_TI_ARM_SMALL));
		m_oLockButton.SetOnString(EMResourceRepository::GetString(RES_TI_LOCK_SMALL));
		m_oLockButton.SetOffString(EMResourceRepository::GetString(RES_TI_LOCK_SMALL));
		m_oMuteButton.SetOnString(EMResourceRepository::GetString(RES_TI_MUTE_SMALL));
		m_oMuteButton.SetOffString(EMResourceRepository::GetString(RES_TI_MUTE_SMALL));
		m_oRenderButton.SetOnString(EMResourceRepository::GetString(RES_TI_RENDER_SMALL));
		m_oRenderButton.SetOffString(EMResourceRepository::GetString(RES_TI_RENDER_SMALL));
		m_oSoloButton.SetOnString(EMResourceRepository::GetString(RES_TI_SOLO_SMALL));
		m_oSoloButton.SetOffString(EMResourceRepository::GetString(RES_TI_SOLO_SMALL));
	}
	else if(((eCurrentZoomScale == EM_SMALL && !(vCurrentSignalOn)) ||
			eCurrentZoomScale > EM_SMALL)
			&& (m_ePreviousZoomScale != eCurrentZoomScale ||
			m_vPreviousSignalOn != vCurrentSignalOn || m_vUpdateAll))
	{
		m_oArmButton.SetOnString(EMResourceRepository::GetString(RES_TI_ARM_LARGE));
		m_oArmButton.SetOffString(EMResourceRepository::GetString(RES_TI_ARM_LARGE));
		m_oLockButton.SetOnString(EMResourceRepository::GetString(RES_TI_LOCK_LARGE));
		m_oLockButton.SetOffString(EMResourceRepository::GetString(RES_TI_LOCK_LARGE));
		m_oMuteButton.SetOnString(EMResourceRepository::GetString(RES_TI_MUTE_LARGE));
		m_oMuteButton.SetOffString(EMResourceRepository::GetString(RES_TI_MUTE_LARGE));
		m_oRenderButton.SetOnString(EMResourceRepository::GetString(RES_TI_RENDER_LARGE));
		m_oRenderButton.SetOffString(EMResourceRepository::GetString(RES_TI_RENDER_LARGE));
		m_oSoloButton.SetOnString(EMResourceRepository::GetString(RES_TI_SOLO_LARGE));
		m_oSoloButton.SetOffString(EMResourceRepository::GetString(RES_TI_SOLO_LARGE));
	}

	if((eCurrentZoomScale > EM_SMALL || (eCurrentZoomScale == EM_SMALL && !vCurrentSignalOn))
			&& (m_ePreviousZoomScale != eCurrentZoomScale ||
			m_vPreviousSignalOn != vCurrentSignalOn || m_vUpdateAll))
	{
		m_oEditLabel.SetFrame(EMRect(Frame().m_vLeft, 
							Frame().m_vTop, 
							Frame().m_vLeft + (EM_METRICS_MEDIUM_SLIM_BUTTON_WIDTH * 5) - 1, 
							Frame().m_vTop + EM_METRICS_SLIM_EDIT_LABEL_HEIGHT - 1));
		m_oEditLabel.CalculatePositions();
	}
	else if((eCurrentZoomScale == EM_SMALL && vCurrentSignalOn)
			&& (m_ePreviousZoomScale != eCurrentZoomScale ||
			m_vPreviousSignalOn != vCurrentSignalOn || m_vUpdateAll))
	{
		m_oEditLabel.SetFrame(EMRect(Frame().m_vLeft, 
							Frame().m_vTop, 
							Frame().m_vLeft + (EM_METRICS_MEDIUM_SLIM_BUTTON_WIDTH * 5) - (EM_METRICS_TINY_SLIM_BUTTON_WIDTH * 5) - 1, 
							Frame().m_vTop + EM_METRICS_SLIM_EDIT_LABEL_HEIGHT - 1));
		m_oEditLabel.CalculatePositions();
	}
	else if(eCurrentZoomScale == EM_TINY
			&& (m_ePreviousZoomScale != eCurrentZoomScale ||
			m_vPreviousSignalOn != vCurrentSignalOn || m_vUpdateAll))
	{
		if(vCurrentSignalOn)
		{
			m_oEditLabel.SetFrame(EMRect(Frame().m_vLeft + TINY_SIGNAL_METER_WIDTH, 
								Frame().m_vTop, 
								Frame().m_vLeft + (EM_METRICS_MEDIUM_SLIM_BUTTON_WIDTH * 5) - (EM_METRICS_TINY_SLIM_BUTTON_WIDTH * 5) - 1, 
								Frame().m_vTop + EM_METRICS_SLIM_EDIT_LABEL_HEIGHT - 1));
			m_oEditLabel.CalculatePositions();
		}
		else
		{
			m_oEditLabel.SetFrame(EMRect(Frame().m_vLeft, 
								Frame().m_vTop, 
								Frame().m_vLeft + (EM_METRICS_MEDIUM_SLIM_BUTTON_WIDTH * 5) - (EM_METRICS_TINY_SLIM_BUTTON_WIDTH * 5) - 1, 
								Frame().m_vTop + EM_METRICS_SLIM_EDIT_LABEL_HEIGHT - 1));
			m_oEditLabel.CalculatePositions();
		}
	}

	if((eCurrentZoomScale == EM_TINY || (eCurrentZoomScale == EM_SMALL && vCurrentSignalOn))
			&& (m_ePreviousZoomScale != eCurrentZoomScale ||
			m_vPreviousSignalOn != vCurrentSignalOn || m_vUpdateAll))
	{	
		m_oMuteButton.SetFrame(EMRect(Frame().m_vLeft + (EM_METRICS_MEDIUM_SLIM_BUTTON_WIDTH * 5) - (EM_METRICS_TINY_SLIM_BUTTON_WIDTH * 5), 
							Frame().m_vTop,
							Frame().m_vLeft + (EM_METRICS_MEDIUM_SLIM_BUTTON_WIDTH * 5) - (EM_METRICS_TINY_SLIM_BUTTON_WIDTH * 4) - 1, 
							Frame().m_vTop + EM_METRICS_SLIM_BUTTON_HEIGHT - 1));
		m_oSoloButton.SetFrame(EMRect(Frame().m_vLeft + (EM_METRICS_MEDIUM_SLIM_BUTTON_WIDTH * 5) - (EM_METRICS_TINY_SLIM_BUTTON_WIDTH * 4), 
							Frame().m_vTop,
							Frame().m_vLeft + (EM_METRICS_MEDIUM_SLIM_BUTTON_WIDTH * 5) - (EM_METRICS_TINY_SLIM_BUTTON_WIDTH * 3) - 1, 
							Frame().m_vTop + EM_METRICS_SLIM_BUTTON_HEIGHT - 1));
		m_oRenderButton.SetFrame(EMRect(Frame().m_vLeft + (EM_METRICS_MEDIUM_SLIM_BUTTON_WIDTH * 5) - (EM_METRICS_TINY_SLIM_BUTTON_WIDTH * 3), 
							Frame().m_vTop,
							Frame().m_vLeft + (EM_METRICS_MEDIUM_SLIM_BUTTON_WIDTH * 5) - (EM_METRICS_TINY_SLIM_BUTTON_WIDTH * 2) - 1, 
							Frame().m_vTop + EM_METRICS_SLIM_BUTTON_HEIGHT - 1));
		m_oLockButton.SetFrame(EMRect(Frame().m_vLeft + (EM_METRICS_MEDIUM_SLIM_BUTTON_WIDTH * 5) - (EM_METRICS_TINY_SLIM_BUTTON_WIDTH * 2), 
							Frame().m_vTop,
							Frame().m_vLeft + (EM_METRICS_MEDIUM_SLIM_BUTTON_WIDTH * 5) - EM_METRICS_TINY_SLIM_BUTTON_WIDTH - 1, 
							Frame().m_vTop + EM_METRICS_SLIM_BUTTON_HEIGHT - 1));
		m_oArmButton.SetFrame(EMRect(Frame().m_vLeft + (EM_METRICS_MEDIUM_SLIM_BUTTON_WIDTH * 5) - EM_METRICS_TINY_SLIM_BUTTON_WIDTH, 
							Frame().m_vTop,
							Frame().m_vLeft + (EM_METRICS_MEDIUM_SLIM_BUTTON_WIDTH * 5) - 1, 
							Frame().m_vTop + EM_METRICS_SLIM_BUTTON_HEIGHT - 1));
		m_oEmptySpaceOne = EMRect();
		if(vCurrentSignalOn && eCurrentZoomScale == EM_TINY)
		{
			m_oSignalMeter.SetVisible(true);
			m_oSignalMeter.SetFrame(EMRect(Frame().m_vLeft,
								Frame().m_vTop,
								Frame().m_vLeft + TINY_SIGNAL_METER_WIDTH - 1,
								Frame().m_vTop + EM_METRICS_SIGNAL_METER_HEIGHT - 1));
		}
		else if(vCurrentSignalOn && eCurrentZoomScale == EM_SMALL)
		{
			m_oSignalMeter.SetVisible(true);
			m_oSignalMeter.SetFrame(EMRect(Frame().m_vLeft,
								Frame().m_vTop + EM_METRICS_SLIM_EDIT_LABEL_HEIGHT,
								Frame().m_vLeft + (EM_METRICS_MEDIUM_SLIM_BUTTON_WIDTH * 5),
								Frame().m_vTop + EM_METRICS_SLIM_EDIT_LABEL_HEIGHT + EM_METRICS_SIGNAL_METER_HEIGHT - 1));
		}
		else
			m_oSignalMeter.SetVisible(false);
	}
	else if((eCurrentZoomScale > EM_SMALL || (eCurrentZoomScale == EM_SMALL && !vCurrentSignalOn))
			&& (m_ePreviousZoomScale != eCurrentZoomScale ||
			m_vPreviousSignalOn != vCurrentSignalOn || m_vUpdateAll))
	{	
		m_oMuteButton.SetFrame(EMRect(Frame().m_vLeft, 
							Frame().m_vTop + EM_METRICS_SLIM_EDIT_LABEL_HEIGHT,
							Frame().m_vLeft + EM_METRICS_MEDIUM_SLIM_BUTTON_WIDTH - 1, 
							Frame().m_vTop + EM_METRICS_SLIM_EDIT_LABEL_HEIGHT + EM_METRICS_SLIM_BUTTON_HEIGHT - 1));
		m_oSoloButton.SetFrame(EMRect(Frame().m_vLeft + EM_METRICS_MEDIUM_SLIM_BUTTON_WIDTH, 
							Frame().m_vTop + EM_METRICS_SLIM_EDIT_LABEL_HEIGHT,
							Frame().m_vLeft + (EM_METRICS_MEDIUM_SLIM_BUTTON_WIDTH * 2) - 1,
							Frame().m_vTop + EM_METRICS_SLIM_EDIT_LABEL_HEIGHT + EM_METRICS_SLIM_BUTTON_HEIGHT - 1));
		m_oRenderButton.SetFrame(EMRect(Frame().m_vLeft + (EM_METRICS_MEDIUM_SLIM_BUTTON_WIDTH * 2), 
							Frame().m_vTop + EM_METRICS_SLIM_EDIT_LABEL_HEIGHT,
							Frame().m_vLeft + (EM_METRICS_MEDIUM_SLIM_BUTTON_WIDTH * 3) - 1,
							Frame().m_vTop + EM_METRICS_SLIM_EDIT_LABEL_HEIGHT + EM_METRICS_SLIM_BUTTON_HEIGHT - 1));
		m_oLockButton.SetFrame(EMRect(Frame().m_vLeft + (EM_METRICS_MEDIUM_SLIM_BUTTON_WIDTH * 3), 
							Frame().m_vTop + EM_METRICS_SLIM_EDIT_LABEL_HEIGHT,
							Frame().m_vLeft + (EM_METRICS_MEDIUM_SLIM_BUTTON_WIDTH * 4) - 1,
							Frame().m_vTop + EM_METRICS_SLIM_EDIT_LABEL_HEIGHT + EM_METRICS_SLIM_BUTTON_HEIGHT - 1));
		m_oArmButton.SetFrame(EMRect(Frame().m_vLeft + (EM_METRICS_MEDIUM_SLIM_BUTTON_WIDTH * 4), 
							Frame().m_vTop + EM_METRICS_SLIM_EDIT_LABEL_HEIGHT,
							Frame().m_vLeft + (EM_METRICS_MEDIUM_SLIM_BUTTON_WIDTH * 5) - 1,
							Frame().m_vTop + EM_METRICS_SLIM_EDIT_LABEL_HEIGHT + EM_METRICS_SLIM_BUTTON_HEIGHT - 1));	
		if(vCurrentSignalOn)
		{
			m_oSignalMeter.SetVisible(true);
			m_oSignalMeter.SetFrame(EMRect(Frame().m_vLeft,
								Frame().m_vTop + EM_METRICS_SLIM_EDIT_LABEL_HEIGHT + EM_METRICS_SLIM_BUTTON_HEIGHT,
								Frame().m_vLeft + (EM_METRICS_MEDIUM_SLIM_BUTTON_WIDTH * 5) - 1,
								Frame().m_vTop + EM_METRICS_SLIM_EDIT_LABEL_HEIGHT + EM_METRICS_SLIM_BUTTON_HEIGHT + EM_METRICS_SIGNAL_METER_HEIGHT - 1));
			m_oEmptySpaceOne = EMRect(Frame().m_vLeft,
									Frame().m_vTop + EM_METRICS_SLIM_EDIT_LABEL_HEIGHT + EM_METRICS_SLIM_BUTTON_HEIGHT + EM_METRICS_SIGNAL_METER_HEIGHT,
									Frame().m_vLeft + (EM_METRICS_MEDIUM_SLIM_BUTTON_WIDTH * 5) - 1,
									Frame().m_vBottom - BOTTOM_BORDER_SIZE);
		}
		else
		{
			m_oSignalMeter.SetVisible(false);
			m_oEmptySpaceOne = EMRect(Frame().m_vLeft,
									Frame().m_vTop + EM_METRICS_SLIM_EDIT_LABEL_HEIGHT + EM_METRICS_SLIM_BUTTON_HEIGHT,
									Frame().m_vLeft + (EM_METRICS_MEDIUM_SLIM_BUTTON_WIDTH * 5) - 1,
									Frame().m_vBottom - BOTTOM_BORDER_SIZE);
		}
	}

	m_oSelectionButton.SetFrame(EMRect(Frame().m_vRight - EM_METRICS_TINY_SLIM_BUTTON_WIDTH + 1, 
						Frame().m_vTop,
						Frame().m_vRight,
						Frame().m_vBottom - BOTTOM_BORDER_SIZE));

	if(eCurrentZoomScale <= EM_SMALL && 
		(m_ePreviousZoomScale > EM_TINY || m_vUpdateAll))
		m_oEmptySpaceTwo = EMRect();
	else if(eCurrentZoomScale > EM_SMALL && 
		(m_ePreviousZoomScale <= EM_SMALL || m_vUpdateAll))
		m_oEmptySpaceTwo = EMRect(Frame().m_vLeft + (EM_METRICS_MEDIUM_SLIM_BUTTON_WIDTH * 5),
									Frame().m_vTop + EM_METRICS_SLIM_EDIT_LABEL_HEIGHT + EM_METRICS_SLIM_BUTTON_HEIGHT,
									Frame().m_vRight - EM_METRICS_TINY_SLIM_BUTTON_WIDTH,
									Frame().m_vBottom);
	
	int32 vCurrentPosX = Frame().m_vLeft + (EM_METRICS_MEDIUM_SLIM_BUTTON_WIDTH * 5);
	
	// Checks if the update was triggered by a zoom that does not affect the frames
	if(eCurrentZoomScale >= EM_SMALL && 
		m_ePreviousZoomScale >= EM_SMALL && 
		(eCurrentZoomScale != m_ePreviousZoomScale) && !m_vUpdateAll)
	{
		m_ePreviousZoomScale = eCurrentZoomScale;
		m_vPreviousSignalOn = vCurrentSignalOn;
//		m_vUpdateAll = false;
		return;
	}

	// Checks if the update was triggered by the signal button (does not affect the following frames)
	if((m_vPreviousSignalOn != vCurrentSignalOn) && !m_vUpdateAll)
	{
		m_ePreviousZoomScale = eCurrentZoomScale;
		m_vPreviousSignalOn = vCurrentSignalOn;
//		m_vUpdateAll = false;
		return;
	}

	if(opButtonBar -> GetInputOn() || opButtonBar -> GetExpandOn())
	{
		m_oInputValueBox.SetVisible(true);
		if(eCurrentZoomScale == EM_TINY)
			m_oInputValueBox.SetFrame(EMRect(vCurrentPosX, 
									Frame().m_vTop,
									vCurrentPosX + EM_METRICS_MEDIUM_SLIM_BUTTON_WIDTH - 1,
									Frame().m_vTop + EM_METRICS_SLIM_BUTTON_HEIGHT - 1));
		else
			m_oInputValueBox.SetFrame(EMRect(vCurrentPosX, 
									Frame().m_vTop,
									vCurrentPosX + EM_METRICS_MEDIUM_SLIM_BUTTON_WIDTH - 1,
									Frame().m_vTop + EM_METRICS_SLIM_EDIT_LABEL_HEIGHT + EM_METRICS_SLIM_BUTTON_HEIGHT - 1));
		vCurrentPosX += EM_METRICS_MEDIUM_SLIM_BUTTON_WIDTH;
	}
	else
		m_oInputValueBox.SetVisible(false);
	
	if(opButtonBar -> GetOutputOn() || opButtonBar -> GetExpandOn())
	{
		m_oOutputValueBox.SetVisible(true);
		if(eCurrentZoomScale == EM_TINY)
			m_oOutputValueBox.SetFrame(EMRect(vCurrentPosX, 
									Frame().m_vTop,
									vCurrentPosX + EM_METRICS_MEDIUM_SLIM_BUTTON_WIDTH - 1,
									Frame().m_vTop + EM_METRICS_SLIM_BUTTON_HEIGHT- 1));
		else
			m_oOutputValueBox.SetFrame(EMRect(vCurrentPosX, 
									Frame().m_vTop,
									vCurrentPosX + EM_METRICS_MEDIUM_SLIM_BUTTON_WIDTH - 1,
									Frame().m_vTop + EM_METRICS_SLIM_EDIT_LABEL_HEIGHT + EM_METRICS_SLIM_BUTTON_HEIGHT- 1));
		vCurrentPosX += EM_METRICS_MEDIUM_SLIM_BUTTON_WIDTH;
	}
	else
		m_oOutputValueBox.SetVisible(false);
	
	if(opButtonBar -> GetVolumeOn() || opButtonBar -> GetExpandOn())
	{
		m_oVolumeValueBox.SetVisible(true);
		if(eCurrentZoomScale == EM_TINY)
			m_oVolumeValueBox.SetFrame(EMRect(vCurrentPosX, 
									Frame().m_vTop,
									vCurrentPosX + EM_METRICS_SMALL_SLIM_BUTTON_WIDTH - 1,
									Frame().m_vTop + EM_METRICS_SLIM_BUTTON_HEIGHT- 1));
		else
			m_oVolumeValueBox.SetFrame(EMRect(vCurrentPosX, 
									Frame().m_vTop,
									vCurrentPosX + EM_METRICS_SMALL_SLIM_BUTTON_WIDTH - 1,
									Frame().m_vTop + EM_METRICS_SLIM_EDIT_LABEL_HEIGHT + EM_METRICS_SLIM_BUTTON_HEIGHT- 1));
		vCurrentPosX += EM_METRICS_SMALL_SLIM_BUTTON_WIDTH;
	}
	else
		m_oVolumeValueBox.SetVisible(false);
	
	if(opButtonBar -> GetPanOn() || opButtonBar -> GetExpandOn())
	{
		m_oPanValueBox.SetVisible(true);
		if(eCurrentZoomScale == EM_TINY)
			m_oPanValueBox.SetFrame(EMRect(vCurrentPosX, 
									Frame().m_vTop,
									vCurrentPosX + EM_METRICS_SMALL_SLIM_BUTTON_WIDTH - 1,
									Frame().m_vTop + EM_METRICS_SLIM_BUTTON_HEIGHT- 1));
		else
			m_oPanValueBox.SetFrame(EMRect(vCurrentPosX, 
									Frame().m_vTop,
									vCurrentPosX + EM_METRICS_SMALL_SLIM_BUTTON_WIDTH - 1,
									Frame().m_vTop + EM_METRICS_SLIM_EDIT_LABEL_HEIGHT + EM_METRICS_SLIM_BUTTON_HEIGHT- 1));
		vCurrentPosX += EM_METRICS_SMALL_SLIM_BUTTON_WIDTH;
	}
	else
		m_oPanValueBox.SetVisible(false);
	
	if(opButtonBar -> GetTimeOffsetOn() || opButtonBar -> GetExpandOn())
	{
		m_oTimeOffsetValueBox.SetVisible(true);
		if(eCurrentZoomScale == EM_TINY)
			m_oTimeOffsetValueBox.SetFrame(EMRect(vCurrentPosX, 
									Frame().m_vTop,
									vCurrentPosX + EM_METRICS_SMALL_SLIM_BUTTON_WIDTH - 1,
									Frame().m_vTop + EM_METRICS_SLIM_BUTTON_HEIGHT- 1));
		else
			m_oTimeOffsetValueBox.SetFrame(EMRect(vCurrentPosX, 
									Frame().m_vTop,
									vCurrentPosX + EM_METRICS_SMALL_SLIM_BUTTON_WIDTH - 1,
									Frame().m_vTop + EM_METRICS_SLIM_EDIT_LABEL_HEIGHT + EM_METRICS_SLIM_BUTTON_HEIGHT- 1));
		vCurrentPosX += EM_METRICS_SMALL_SLIM_BUTTON_WIDTH;
	}
	else
		m_oTimeOffsetValueBox.SetVisible(false);
	
	if(opButtonBar -> GetVelocityOffsetOn() || opButtonBar -> GetExpandOn())
	{
		m_oVelocityOffsetValueBox.SetVisible(true);
		if(eCurrentZoomScale == EM_TINY)
			m_oVelocityOffsetValueBox.SetFrame(EMRect(vCurrentPosX, 
									Frame().m_vTop,
									vCurrentPosX + EM_METRICS_SMALL_SLIM_BUTTON_WIDTH - 1,
									Frame().m_vTop + EM_METRICS_SLIM_BUTTON_HEIGHT- 1));
		else
			m_oVelocityOffsetValueBox.SetFrame(EMRect(vCurrentPosX, 
									Frame().m_vTop,
									vCurrentPosX + EM_METRICS_SMALL_SLIM_BUTTON_WIDTH - 1,
									Frame().m_vTop + EM_METRICS_SLIM_EDIT_LABEL_HEIGHT + EM_METRICS_SLIM_BUTTON_HEIGHT- 1));
		vCurrentPosX += EM_METRICS_SMALL_SLIM_BUTTON_WIDTH;
	}
	else
		m_oVelocityOffsetValueBox.SetVisible(false);
	
	if(opButtonBar -> GetKeyOffsetOn() || opButtonBar -> GetExpandOn())
	{
		m_oKeyOffsetValueBox.SetVisible(true);
		if(eCurrentZoomScale == EM_TINY)
			m_oKeyOffsetValueBox.SetFrame(EMRect(vCurrentPosX, 
									Frame().m_vTop,
									vCurrentPosX + EM_METRICS_SMALL_SLIM_BUTTON_WIDTH - 1,
									Frame().m_vTop + EM_METRICS_SLIM_BUTTON_HEIGHT- 1));
		else
			m_oKeyOffsetValueBox.SetFrame(EMRect(vCurrentPosX, 
									Frame().m_vTop,
									vCurrentPosX + EM_METRICS_SMALL_SLIM_BUTTON_WIDTH - 1,
									Frame().m_vTop + EM_METRICS_SLIM_EDIT_LABEL_HEIGHT + EM_METRICS_SLIM_BUTTON_HEIGHT- 1));
		vCurrentPosX += EM_METRICS_SMALL_SLIM_BUTTON_WIDTH;
	}
	else
		m_oKeyOffsetValueBox.SetVisible(false);

	m_ePreviousZoomScale = eCurrentZoomScale;
	m_vPreviousSignalOn = vCurrentSignalOn;
//	m_vUpdateAll = false;
}

bool GTrackInfo::MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage)
{
	if(p_vMessage == EM_TEXT_CHANGED)
	{
		int32 vTrackID = GetMediaTrackID();
		string vName = GetTrackName();
		EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_SET_TRACK_NAME, &vTrackID, const_cast<char*>(vName.c_str())/*&vName*/);
		return true;
	}
	else if(p_vMessage == MESSAGE_INPUT_CHANGED)
	{
		string oInputName;
		list<int32> oInputIDs;
		EMMediaType eMediaType = GetType();
		oInputIDs = *(static_cast<list<int32>* >(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_INPUTS, &eMediaType, NULL, NULL)));
		list<int32>::iterator oIterator = oInputIDs.begin();
		int32 vMediaTrackID = GetMediaTrackID();
		while(oIterator != oInputIDs.end())
		{
			oInputName = string(static_cast<const char*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_INPUT_NAME, &(*oIterator))));
;//cout_commented_out_4_release << "opInputName: " << opInputName << ", name2: " << *static_cast<string*>(m_oInputValueBox.GetValue()) << endl;
			if(oInputName == *static_cast<string*>(m_oInputValueBox.GetValue()))
			{
				EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_SET_TRACK_INPUT, &vMediaTrackID, &(*oIterator));
				break;
			}
			oIterator++;
		}
		return true;
	}
	else if(p_vMessage == MESSAGE_OUTPUT_CHANGED)
	{
		string oOutputName;
		list<int32> oOutputIDs;
		EMMediaType eMediaType = GetType();
		oOutputIDs = *(static_cast<list<int32>* >(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_OUTPUTS, &eMediaType, NULL, NULL)));
		list<int32>::iterator oIterator = oOutputIDs.begin();
		int32 vMediaTrackID = GetMediaTrackID();
		while(oIterator != oOutputIDs.end())
		{
			oOutputName = string(static_cast<const char*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_OUTPUT_NAME, &(*oIterator))));
			if(oOutputName == *static_cast<string*>(m_oOutputValueBox.GetValue()))
			{
				if(eMediaType == EM_TYPE_ANY_AUDIO)
				{
					int32 vEffectTrackID = *(static_cast<int32*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_EFFECT_TRACK_ID, &vMediaTrackID)));
					EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_SET_EFFECT_TRACK_OUTPUT, &vEffectTrackID, &(*oIterator));
				}
				else
					EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_SET_TRACK_OUTPUT, &vMediaTrackID, &(*oIterator));
				break;
			}
			oIterator++;
		}
		return true;
	}
	return false;
}

void GTrackInfo::MouseDown(int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree, EMView* p_opView)
{
	if(!Frame().Contains(p_vX, p_vY))
		return;
	
	if(m_oSelectionButton.Frame().Contains(p_vX, p_vY))
	{
		if(m_oSelectionButton.MouseDown(p_vX, p_vY, p_vButtonOne, p_vButtonTwo, p_vButtonThree, GetTrack()))
			m_oUpdateRect = m_oUpdateRect | m_oSelectionButton.Frame();
	}
//	if(m_oEditLabel.MouseDown(p_vX, p_vY))
//		m_oUpdateRect = m_oUpdateRect | m_oArmButton.Frame();
	else if(m_oArmButton.MouseDown(p_vX, p_vY))
	{
//		m_oArmButton.ToggleOn();
		m_oUpdateRect = m_oUpdateRect | m_oArmButton.Frame();
	}
//	if(m_oLockButton.MouseDown(p_vX, p_vY))
//		m_oUpdateRect = m_oUpdateRect | m_oLockButton.Frame();
	else if(m_oMuteButton.MouseDown(p_vX, p_vY, GetTrack()))
	{
//		m_oMuteButton.ToggleOn();
		m_oUpdateRect = m_oUpdateRect | m_oMuteButton.Frame();
		m_oUpdateRect = m_oUpdateRect | m_oSoloButton.Frame();
	}
	else if(m_oRenderButton.MouseDown(p_vX, p_vY))
	{
		GTrackInfo* opTrackInfo;
		for(uint32 vIndex = 0; vIndex < EMGTrackRepository::Instance() -> GetNrTracks(); vIndex++)
		{
			opTrackInfo = dynamic_cast<GTrackInfo*>(EMGTrackRepository::Instance() -> GetTrack(vIndex) -> GetTrackInfo());
			opTrackInfo -> SetRenderButton(m_oRenderButton.GetOn());
			/*
			if(opTrackInfo -> GetMuteButton() -> GetOn())
			{
				if(opTrackInfo -> GetMuteButton() -> GetOnBySolo())
				{
					opTrackInfo -> GetMuteButton() -> SetOnBySolo(false);
					m_opView -> Invalidate(opTrackInfo -> GetMuteButton() -> Frame());
				}
				else
					opTrackInfo -> GetMuteButton() -> ExecuteCommand();
			}
			if(opTrackInfo -> GetSoloButton() -> GetOn())
			{
				opTrackInfo -> GetSoloButton() -> SetOn(false);
				m_opView -> Invalidate(opTrackInfo -> GetSoloButton() -> Frame());
			}
			*/
		}
	/*
		m_oUpdateRect = m_oUpdateRect | m_oRenderButton.Frame();
		int32 vTrackID = GetMediaTrackID();
		int32 vOutputID;
		SetRenderButton(m_oRenderButton.GetOn());
	*/
		
/*
		if(m_oRenderButton.GetOn())
		{
			// The rendering output should be active for this track
			if(GetType() == EM_TYPE_ANY_VIDEO)
				vOutputID = *(static_cast<int32*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_VIDEO_RENDER_OUTPUT, NULL, NULL)));
			else
				vOutputID = *(static_cast<int32*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_AUDIO_RENDER_OUTPUT, NULL, NULL)));
		}
		else
		{
			list<int32> oOutputIDs;
			EMMediaType* epMediaType = EM_new EMMediaType(GetType());
			EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_OUTPUTS, epMediaType, &oOutputIDs);
			vOutputID = oOutputIDs.front();
		}
;//cout_commented_out_4_release << "GTrackInfo::MouseDown: vTrackID: " << vTrackID << ", vOutputID: " << vOutputID << endl;
		EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_SET_TRACK_OUTPUT, &vTrackID, &vOutputID);
*/
	}
	else if(m_oSoloButton.MouseDown(p_vX, p_vY, p_vButtonOne, p_vButtonTwo, p_vButtonThree, GetTrack()))
	{
		int32 vTrackID = GetMediaTrackID();
		EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_TOGGLE_TRACK_SOLO, &vTrackID, NULL);
		m_oUpdateRect = m_oUpdateRect | m_oMuteButton.Frame();
		m_oUpdateRect = m_oUpdateRect | m_oSoloButton.Frame();
	}
	else if(m_oInputValueBox.MouseDown(p_vX, p_vY, p_vButtonOne, p_vButtonTwo, p_vButtonThree))
		m_oUpdateRect = m_oUpdateRect | m_oInputValueBox.Frame();
	else if(m_oKeyOffsetValueBox.MouseDown(p_vX, p_vY, p_vButtonOne, p_vButtonTwo, p_vButtonThree))
		m_oUpdateRect = m_oUpdateRect | m_oKeyOffsetValueBox.Frame();
	else if(m_oOutputValueBox.MouseDown(p_vX, p_vY, p_vButtonOne, p_vButtonTwo, p_vButtonThree))
		m_oUpdateRect = m_oUpdateRect | m_oOutputValueBox.Frame();
	else if(m_oPanValueBox.MouseDown(p_vX, p_vY, p_vButtonOne, p_vButtonTwo, p_vButtonThree))
		m_oUpdateRect = m_oUpdateRect | m_oPanValueBox.Frame();
	else if(m_oTimeOffsetValueBox.MouseDown(p_vX, p_vY, p_vButtonOne, p_vButtonTwo, p_vButtonThree))
		m_oUpdateRect = m_oUpdateRect | m_oTimeOffsetValueBox.Frame();
	else if(m_oVelocityOffsetValueBox.MouseDown(p_vX, p_vY, p_vButtonOne, p_vButtonTwo, p_vButtonThree))
		m_oUpdateRect = m_oUpdateRect | m_oVelocityOffsetValueBox.Frame();
	else if(m_oVolumeValueBox.MouseDown(p_vX, p_vY, p_vButtonOne, p_vButtonTwo, p_vButtonThree))
		m_oUpdateRect = m_oUpdateRect | m_oVolumeValueBox.Frame();
}

void GTrackInfo::MouseDoubleClick(int32 p_vX, int32 p_vY, EMView* p_opView)
{
	m_oEditLabel.MouseDoubleClick(p_vX, p_vY);
}

void GTrackInfo::MouseMoved(int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree)
{
	if(m_oInputValueBox.MouseMoved(p_vX, p_vY, p_vButtonOne, p_vButtonTwo, p_vButtonThree))
		m_oUpdateRect = m_oUpdateRect | m_oInputValueBox.Frame();
	if(m_oKeyOffsetValueBox.MouseMoved(p_vX, p_vY, p_vButtonOne, p_vButtonTwo, p_vButtonThree))
		m_oUpdateRect = m_oUpdateRect | m_oKeyOffsetValueBox.Frame();
	if(m_oOutputValueBox.MouseMoved(p_vX, p_vY, p_vButtonOne, p_vButtonTwo, p_vButtonThree))
		m_oUpdateRect = m_oUpdateRect | m_oOutputValueBox.Frame();
	if(m_oPanValueBox.MouseMoved(p_vX, p_vY, p_vButtonOne, p_vButtonTwo, p_vButtonThree))
		m_oUpdateRect = m_oUpdateRect | m_oPanValueBox.Frame();
	if(m_oTimeOffsetValueBox.MouseMoved(p_vX, p_vY, p_vButtonOne, p_vButtonTwo, p_vButtonThree))
		m_oUpdateRect = m_oUpdateRect | m_oTimeOffsetValueBox.Frame();
	if(m_oVelocityOffsetValueBox.MouseMoved(p_vX, p_vY, p_vButtonOne, p_vButtonTwo, p_vButtonThree))
		m_oUpdateRect = m_oUpdateRect | m_oVelocityOffsetValueBox.Frame();
	if(m_oVolumeValueBox.MouseMoved(p_vX, p_vY, p_vButtonOne, p_vButtonTwo, p_vButtonThree))
		m_oUpdateRect = m_oUpdateRect | m_oVolumeValueBox.Frame();
	if(m_oSelectionButton.MouseMoved(p_vX, p_vY, p_vButtonOne, p_vButtonTwo, p_vButtonThree))
		m_oUpdateRect = m_oUpdateRect | m_oSelectionButton.Frame();
}

void GTrackInfo::MouseUp(int32 p_vX, int32 p_vY)
{
//;//cout_commented_out_4_release << "AudioGTrackInfo::MouseUp" << endl;
	if(m_oInputValueBox.MouseUp(p_vX, p_vY))
		m_oUpdateRect = m_oUpdateRect | m_oInputValueBox.Frame();
	if(m_oKeyOffsetValueBox.MouseUp(p_vX, p_vY))
		m_oUpdateRect = m_oUpdateRect | m_oKeyOffsetValueBox.Frame();
	if(m_oOutputValueBox.MouseUp(p_vX, p_vY))
		m_oUpdateRect = m_oUpdateRect | m_oOutputValueBox.Frame();
	if(m_oPanValueBox.MouseUp(p_vX, p_vY))
		m_oUpdateRect = m_oUpdateRect | m_oPanValueBox.Frame();
	if(m_oTimeOffsetValueBox.MouseUp(p_vX, p_vY))
		m_oUpdateRect = m_oUpdateRect | m_oTimeOffsetValueBox.Frame();
	if(m_oVelocityOffsetValueBox.MouseUp(p_vX, p_vY))
		m_oUpdateRect = m_oUpdateRect | m_oVelocityOffsetValueBox.Frame();
	if(m_oVolumeValueBox.MouseUp(p_vX, p_vY))
		m_oUpdateRect = m_oUpdateRect | m_oVolumeValueBox.Frame();
	if(m_oSelectionButton.MouseUp(p_vX, p_vY))
		m_oUpdateRect = m_oUpdateRect | m_oSelectionButton.Frame();
}

// FIXME: A lot of stuff in here that should be moved to separate methods
void GTrackInfo::SetMediaTrackID(int32 p_vMediaTrackID)
{
	EMGTrackInfo::SetMediaTrackID(p_vMediaTrackID);

	EMMediaTrackInfo* opMediaTrackInfo = static_cast<EMMediaTrackInfo*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_TRACK_INFO, &p_vMediaTrackID, NULL));
	m_oArmButton.SetOn(opMediaTrackInfo -> m_vIsArmActive);
	m_oPanValueBox.SetValue(opMediaTrackInfo -> m_vPanLevel);
	m_oRenderButton.SetOn(opMediaTrackInfo -> m_vIsRenderActive);
	m_oSoloButton.SetOn(opMediaTrackInfo -> m_vIsSoloActive);
//	m_oTimeOffsetValueBox.SetValue(opMediaTrackInfo -> m_vStartOffset);
	m_oVolumeValueBox.SetValue(opMediaTrackInfo -> m_vFaderLevel);
	m_oMuteButton.SetMediaTrackID(p_vMediaTrackID);
	if(opMediaTrackInfo -> m_eMuteState == EM_MUTE_ON)
		m_oMuteButton.SetOn(true);
	else if(opMediaTrackInfo -> m_eMuteState == EM_MUTE_ON_BY_SOLO)
		m_oMuteButton.SetOnBySolo(true);
	else
		m_oMuteButton.SetOn(false);
	if(GetType() == EM_TYPE_MIDI)
	{
		m_oKeyOffsetValueBox.SetValue(opMediaTrackInfo -> m_vKeyOffset);
		m_oVelocityOffsetValueBox.SetValue(opMediaTrackInfo -> m_vVelocityOffset);
		m_oTimeOffsetValueBox.SetValue(opMediaTrackInfo -> m_vMIDIChannel);
	}

//eo << "GTrackInfo::SetMediaTrackID - arm: " << opMediaTrackInfo -> m_vIsArmActive << ", solo: " << opMediaTrackInfo -> m_vIsSoloActive << ", mute state: " << static_cast<int>(opMediaTrackInfo -> m_eMuteState) << ef;

	// If there's no output saved, set first available output (or render output)
	if(opMediaTrackInfo -> m_vOutputID == -1)
	{
		list<int32> oOutputIDs;
		int32 vOutputID;
		EMMediaType eMediaType = GetType();
		if(opMediaTrackInfo -> m_vIsRenderActive)
		{
			if(eMediaType == EM_TYPE_ANY_AUDIO)
				vOutputID = *(static_cast<int32*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_AUDIO_RENDER_OUTPUT, NULL, NULL, NULL)));
			else if(eMediaType == EM_TYPE_ANY_VIDEO)
				vOutputID = *(static_cast<int32*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_VIDEO_RENDER_OUTPUT, NULL, NULL, NULL)));
		}
		else
		{
			oOutputIDs = *(static_cast<list<int32>* >(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_OUTPUTS, &eMediaType, NULL, NULL)));
			if(oOutputIDs.size() == 0)
				vOutputID = -1;
			else
				vOutputID = oOutputIDs.front();
		}

		if(eMediaType == EM_TYPE_ANY_AUDIO || eMediaType == EM_TYPE_ANY_VIDEO)
		{
			int32 vEffectTrackID = *(static_cast<int32*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_CREATE_EFFECT_TRACK,NULL)));
			bool vIsEffect(true);
			EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_SET_TRACK_OUTPUT, &p_vMediaTrackID, &vEffectTrackID, &vIsEffect);
			if(vOutputID != -1)
				EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_SET_EFFECT_TRACK_OUTPUT, &vEffectTrackID, &vOutputID, NULL);
		}
		else if(vOutputID != -1)
			EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_SET_TRACK_OUTPUT, &p_vMediaTrackID, &vOutputID, NULL);

		opMediaTrackInfo = static_cast<EMMediaTrackInfo*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_TRACK_INFO, &p_vMediaTrackID, NULL));
	}

	int32 vOutputID;
	if(opMediaTrackInfo -> m_vOutputIsEffectTrack)
	{
		int32 vEffectTrackID = opMediaTrackInfo -> m_vOutputID;
		EMMediaEffectTrackInfo* opMediaEffectTrackInfo = static_cast<EMMediaEffectTrackInfo*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_EFFECT_TRACK_INFO, &vEffectTrackID));
		vOutputID = opMediaEffectTrackInfo -> m_vOutputID;
	}
	else
		vOutputID = opMediaTrackInfo -> m_vOutputID;

	// If there are any outputs at all (video has no "real" output)
	if(vOutputID != -1 && GetType() != EM_TYPE_ANY_VIDEO)
	{
		const char* vpOutputName = static_cast<const char*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_OUTPUT_NAME, &vOutputID));
		if(vpOutputName != NULL)
			m_oOutputValueBox.SetValue(string(vpOutputName));
		else
			EMDebugger("The track output has no name");
	}
	
	// If there's no input saved, set first available input
	if(opMediaTrackInfo -> m_vInputID == -1) 
	{
		list<int32> oInputIDs;
		EMMediaType eMediaType = GetType();
		oInputIDs = *(static_cast<list<int32>* >(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_INPUTS, &eMediaType, NULL, NULL)));
		if(oInputIDs.size() > 0)
		{
			EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_SET_TRACK_INPUT, &p_vMediaTrackID, &(oInputIDs.front()), NULL);
			// Refresh the track info object data
			opMediaTrackInfo = static_cast<EMMediaTrackInfo*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_TRACK_INFO, &p_vMediaTrackID, NULL));
		}
	}

	// If there are any inputs at all
	if(opMediaTrackInfo -> m_vInputID != -1)
	{
		const char* vpInputName = static_cast<const char*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_INPUT_NAME, &(opMediaTrackInfo -> m_vInputID)));
		if(vpInputName != NULL)
			m_oOutputValueBox.SetValue(string(vpInputName));
		else
			EMDebugger("The track input has no name");
	}
}

void GTrackInfo::SetRenderButton(bool p_vOn)
{
//		m_oUpdateRect = m_oUpdateRect | m_oRenderButton.Frame();
	int32 vTrackID = GetMediaTrackID();
	int32 vOutputID;
	m_oRenderButton.SetOn(p_vOn);
	if(m_oRenderButton.GetOn())
	{
		// The rendering output should be active for this track
		if(GetType() == EM_TYPE_ANY_VIDEO)
			vOutputID = *(static_cast<int32*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_VIDEO_RENDER_OUTPUT, NULL, NULL, NULL)));
		else if(GetType() == EM_TYPE_ANY_AUDIO)
			vOutputID = *(static_cast<int32*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_AUDIO_RENDER_OUTPUT, NULL, NULL, NULL)));
	}
	else
	{
		list<int32> oOutputIDs;
		EMMediaType* epMediaType = NULL;
		EMMediaType eType = GetType();
		epMediaType = EM_new EMMediaType(eType);
		oOutputIDs = *(static_cast<list<int32>* >(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_OUTPUTS, epMediaType, NULL, NULL)));
		vOutputID = oOutputIDs.front();
	}
	EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_SET_TRACK_OUTPUT, &vTrackID, &vOutputID);
	m_opView -> Invalidate(m_oRenderButton.Frame());
}

void GTrackInfo::SetSelected(bool p_vSelected, bool p_vNotify)
{
	if(m_oSelectionButton.SetOn(p_vSelected))
	{
		m_oUpdateRect = m_oUpdateRect | m_oSelectionButton.Frame();
		if(p_vNotify)
			Notify(EM_UPDATE_NEEDED);
	}
}

void GTrackInfo::SetSignal(float p_vSignal)
{
	m_oSignalMeter.SetSignal(p_vSignal);
}

void GTrackInfo::SetSignal(float* p_vpSignal)//list<int32>* p_vSignal)
{
	m_oSignalMeter.SetSignal(p_vpSignal);//p_vSignal);
}

void GTrackInfo::SetTrack(EMGTrack* p_opTrack)
{
	EMGTrackInfo::SetTrack(p_opTrack);
;//cout_commented_out_4_release << "MediaTrackID: " << p_opTrack -> GetTrackInfo() -> GetMediaTrackID() << endl;
//	int32* vpMediaTrackID = EM_new int32(p_opTrack -> GetTrackInfo() -> GetMediaTrackID());
//	m_oMuteButton.SetCommand(MEDIA_COMMAND_TOGGLE_TRACK_MUTE, vpMediaTrackID, NULL, NULL);
//	int32 vMediaTrackID = p_opTrack -> GetTrackInfo() -> GetMediaTrackID();
	m_oSoloButton.SetCommand(MEDIA_COMMAND_TOGGLE_TRACK_SOLO, &m_vMediaTrackID, NULL, NULL);
	m_oArmButton.SetCommand(MEDIA_COMMAND_TOGGLE_TRACK_ARM, &m_vMediaTrackID, NULL, NULL);
	m_oPanValueBox.SetCommand(MEDIA_COMMAND_SET_TRACK_PAN, &m_vMediaTrackID, NULL, NULL);
	m_oVolumeValueBox.SetCommand(MEDIA_COMMAND_SET_TRACK_FADER, &m_vMediaTrackID, NULL, NULL);
	m_oTimeOffsetValueBox.SetCommand(MEDIA_COMMAND_SET_TRACK_MIDI_CHANNEL, &m_vMediaTrackID, NULL, NULL);
	m_oVelocityOffsetValueBox.SetCommand(MEDIA_COMMAND_SET_TRACK_VELOCITY_OFFSET, &m_vMediaTrackID, NULL, NULL);
	m_oKeyOffsetValueBox.SetCommand(MEDIA_COMMAND_SET_TRACK_KEY_OFFSET, &m_vMediaTrackID, NULL, NULL);
}

void GTrackInfo::SetTrackName(string p_oName)
{
	m_oEditLabel.SetLabel(p_oName.c_str());
}

void GTrackInfo::SetUserMode(bool p_vExpertMode)
{
	if(p_vExpertMode)
	{
//		m_oLockButton.SetMode(EM_SLIM_BUTTON_STAY_DOWN);
		m_oMuteButton.SetMode(EM_SLIM_BUTTON_STAY_DOWN);
//		m_oRenderButton.SetMode(EM_SLIM_BUTTON_STAY_DOWN);
		m_oSoloButton.SetMode(EM_SLIM_BUTTON_STAY_DOWN);
	}
	else
	{
//		m_oLockButton.SetMode(EM_SLIM_BUTTON_DRAW_BLANK);
		m_oMuteButton.SetMode(EM_SLIM_BUTTON_DRAW_BLANK);
//		m_oRenderButton.SetMode(EM_SLIM_BUTTON_DRAW_BLANK);
		m_oSoloButton.SetMode(EM_SLIM_BUTTON_DRAW_BLANK);
	}
}


