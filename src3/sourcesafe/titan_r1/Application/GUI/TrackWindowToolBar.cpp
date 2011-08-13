#include "TrackWindowToolBar.h"

#include "CommandIDs.h"
#include "EMCommandRepository.h"
#include "EMFactory.h"
#include "EMGUIGlobals.h"
#include "EMMessages.h"
//#include "EMProjectDataLoader.h"
//#include "EMProjectDataSaver.h"
#include "EMResourceRepository.h"
#include "EMSettingIDs.h"
#include "EMSettingsRepository.h"
#include "EMView.h"
#include "GUIGlobals.h"
#include "ResourceIDs.h"
#include "SettingIDs.h"

#include <string>
#include <vector>
#include <list>

const int32 TIME_DISPLAY_WIDTH = 75;
const int32 COMPONENT_SPACING = 2;
const int32 COMPONENT_INSET_Y = 4;
const int32 TRANSPORT_BUTTON_WIDTH = 24;
const int32 TRANSPORT_BUTTON_INSET_Y = 8;
const int32 TINY_BUTTON_HEIGHT = 10;

TrackWindowToolBar::TrackWindowToolBar() :
m_opView(EMFactory::Instance() -> CreateView(EMRect(), EM_FOLLOW_LEFT_RIGHT, false)),
/*
m_oAffectAllButton(m_opView, 
					EMRect(COMPONENT_SPACING * 6 + TIME_DISPLAY_WIDTH * 2 + EM_METRICS_MEDIUM_SLIM_BUTTON_WIDTH * 3,
						COMPONENT_INSET_Y + EM_METRICS_SLIM_BUTTON_HEIGHT + COMPONENT_SPACING,
						COMPONENT_SPACING * 6 + TIME_DISPLAY_WIDTH * 2 + EM_METRICS_MEDIUM_SLIM_BUTTON_WIDTH * 4 - 1,
						COMPONENT_INSET_Y + EM_METRICS_SLIM_BUTTON_HEIGHT * 2 + COMPONENT_SPACING - 1),
					EMResourceRepository::GetString(RES_TB_AFFECT_ALL),
					EMResourceRepository::GetString(RES_TB_AFFECT_ALL)),
*/
m_oAutoQuantizeButton(m_opView, EMRect(), NULL, NULL, COMMAND_TOGGLE_AUTO_QUANTIZE, NULL, NULL, NULL, true, true, true, true,
					EM_COLOR_BUTTON_DEFAULT_ON, EM_COLOR_BUTTON_DEFAULT_OFF,
					NULL, NULL, NULL, NULL, "Toggles Quantizing Of New MIDI Clips On/Off"),
m_oEditClipsButton(m_opView, 
					EMRect(COMPONENT_SPACING * 6 + TIME_DISPLAY_WIDTH * 2 + EM_METRICS_MEDIUM_SLIM_BUTTON_WIDTH * 2 + TRANSPORT_BUTTON_WIDTH * 11,
						COMPONENT_INSET_Y + EM_METRICS_LABEL_HEIGHT,
						COMPONENT_SPACING * 6 + TIME_DISPLAY_WIDTH * 2 + EM_METRICS_MEDIUM_SLIM_BUTTON_WIDTH * 2 + TRANSPORT_BUTTON_WIDTH * 12 - 1,
						COMPONENT_INSET_Y + EM_METRICS_SLIM_BUTTON_HEIGHT * 2 + COMPONENT_SPACING - 1),
					NULL, NULL, COMMAND_SET_CLIP_EDIT, NULL, NULL, NULL, false, false, false, false,
					EM_COLOR_BUTTON_DEFAULT_ON, EM_COLOR_BUTTON_DEFAULT_OFF,
					NULL, NULL, NULL, NULL, "Activate Clip Edit Mode (2)"),
m_oEditFXButton(m_opView, 
					EMRect(COMPONENT_SPACING * 6 + TIME_DISPLAY_WIDTH * 2 + EM_METRICS_MEDIUM_SLIM_BUTTON_WIDTH * 2 + TRANSPORT_BUTTON_WIDTH * 12,
						COMPONENT_INSET_Y + EM_METRICS_LABEL_HEIGHT,
						COMPONENT_SPACING * 6 + TIME_DISPLAY_WIDTH * 2 + EM_METRICS_MEDIUM_SLIM_BUTTON_WIDTH * 2 + TRANSPORT_BUTTON_WIDTH * 13 - 1,
						COMPONENT_INSET_Y + EM_METRICS_SLIM_BUTTON_HEIGHT * 2 + COMPONENT_SPACING - 1),
					NULL, NULL, COMMAND_SET_FX_EDIT, NULL, NULL, NULL, false, false, false, false,
					EM_COLOR_BUTTON_DEFAULT_ON, EM_COLOR_BUTTON_DEFAULT_OFF,
					NULL, NULL, NULL, NULL, "Activate FX Edit Mode (3)"),
m_oEditModeLabel(m_opView,
						EMRect(COMPONENT_SPACING * 6 + TIME_DISPLAY_WIDTH * 2 + EM_METRICS_MEDIUM_SLIM_BUTTON_WIDTH * 2 + TRANSPORT_BUTTON_WIDTH * 10,
							COMPONENT_INSET_Y,
							COMPONENT_SPACING * 6 + TIME_DISPLAY_WIDTH * 2 + EM_METRICS_MEDIUM_SLIM_BUTTON_WIDTH * 2 + TRANSPORT_BUTTON_WIDTH * 13 - 1,
							COMPONENT_INSET_Y + EM_METRICS_LABEL_HEIGHT - 1),
						NULL, EM_COLOR_VIEW_BACKGROUND, true),
m_oEditTracksButton(m_opView, 
					EMRect(COMPONENT_SPACING * 6 + TIME_DISPLAY_WIDTH * 2 + EM_METRICS_MEDIUM_SLIM_BUTTON_WIDTH * 2 + TRANSPORT_BUTTON_WIDTH * 10,
						COMPONENT_INSET_Y + EM_METRICS_LABEL_HEIGHT,
						COMPONENT_SPACING * 6 + TIME_DISPLAY_WIDTH * 2 + EM_METRICS_MEDIUM_SLIM_BUTTON_WIDTH * 2 + TRANSPORT_BUTTON_WIDTH * 11 - 1,
						COMPONENT_INSET_Y + EM_METRICS_SLIM_BUTTON_HEIGHT * 2 + COMPONENT_SPACING - 1),
					NULL, NULL, COMMAND_SET_TRACK_EDIT, NULL, NULL, NULL, false, false, false, false,
					EM_COLOR_BUTTON_DEFAULT_ON, EM_COLOR_BUTTON_DEFAULT_OFF,
					NULL, NULL, NULL, NULL, "Activate Track Edit Mode (1)"),
m_oMeasuresLabel(m_opView,
						EMRect(COMPONENT_SPACING * 2 + TIME_DISPLAY_WIDTH,
							COMPONENT_INSET_Y,
							COMPONENT_SPACING * 2 + TIME_DISPLAY_WIDTH * 2 - 1,
							COMPONENT_INSET_Y + EM_METRICS_LABEL_HEIGHT - 1),
						NULL, EM_COLOR_VIEW_BACKGROUND, true),
m_oPositionLabel(m_opView,
						EMRect(COMPONENT_SPACING,
							COMPONENT_INSET_Y,
							COMPONENT_SPACING + TIME_DISPLAY_WIDTH - 1,
							COMPONENT_INSET_Y + EM_METRICS_LABEL_HEIGHT - 1),
						NULL, EM_COLOR_VIEW_BACKGROUND, true),
m_oQuantizeLabel(m_opView, EMRect(), NULL, EM_COLOR_VIEW_BACKGROUND, true),
m_oQuantizeModeValueBox(m_opView, EMRect(), /*vector<string>(), */0, NULL, NULL, NULL, true, true, false, true, SETTING_MIDI_AUTO_QUANTIZE_MODE, false, "Displays The Current Quantize Mode"),
/*
m_oRelativeButton(m_opView, 
					EMRect(COMPONENT_SPACING * 6 + TIME_DISPLAY_WIDTH * 2 + EM_METRICS_MEDIUM_SLIM_BUTTON_WIDTH * 3,
						COMPONENT_INSET_Y,
						COMPONENT_SPACING * 6 + TIME_DISPLAY_WIDTH * 2 + EM_METRICS_MEDIUM_SLIM_BUTTON_WIDTH * 4 - 1,
						COMPONENT_INSET_Y + EM_METRICS_SLIM_BUTTON_HEIGHT - 1),
					EMResourceRepository::GetString(RES_TB_RELATIVE),
					EMResourceRepository::GetString(RES_TB_RELATIVE),
					COMMAND_TOGGLE_RELATIVE_MOVE),
*/
m_oSignatureLabel(m_opView,
						EMRect(COMPONENT_SPACING * 4 + TIME_DISPLAY_WIDTH * 2 + EM_METRICS_MEDIUM_SLIM_BUTTON_WIDTH,
							COMPONENT_INSET_Y,
							COMPONENT_SPACING * 4 + TIME_DISPLAY_WIDTH * 2 + EM_METRICS_MEDIUM_SLIM_BUTTON_WIDTH * 2 - 1,
							COMPONENT_INSET_Y + EM_METRICS_LABEL_HEIGHT - 1),
						NULL, EM_COLOR_VIEW_BACKGROUND, true),
m_oSignatureValueBox(m_opView, 
						EMRect(COMPONENT_SPACING * 4 + TIME_DISPLAY_WIDTH * 2 + EM_METRICS_MEDIUM_SLIM_BUTTON_WIDTH,
							COMPONENT_INSET_Y + EM_METRICS_LABEL_HEIGHT,
							COMPONENT_SPACING * 4 + TIME_DISPLAY_WIDTH * 2 + EM_METRICS_MEDIUM_SLIM_BUTTON_WIDTH * 2 - 1,
							COMPONENT_INSET_Y + EM_METRICS_SLIM_BUTTON_HEIGHT * 2 + COMPONENT_SPACING - 1),
//					vector<string>(),
					0, NULL, NULL, NULL, true, true, true, true, SETTING_SIGNATURE, false, "Displays The Current MIDI Signature"),
m_oSnapEnableButton(m_opView, EMRect(), NULL, NULL,	COMMAND_TOGGLE_SNAPPING, NULL, NULL, NULL, true, true, true, true,
					EM_COLOR_BUTTON_DEFAULT_ON, EM_COLOR_BUTTON_DEFAULT_OFF,
					NULL, NULL, NULL, NULL, "Toggles Clip Snapping On/Off (S)"),
m_oSnapLabel(m_opView, EMRect(), NULL, EM_COLOR_VIEW_BACKGROUND, true),
m_oSnappingModeValueBox(m_opView, EMRect(), /*vector<string>(), */0, NULL, NULL, NULL, true, true, false, true, SETTING_SNAPPING_MODE, false, "Displays The Current Clip Snap Mode"),
m_oTempoLabel(m_opView, EMRect(COMPONENT_SPACING * 3 + TIME_DISPLAY_WIDTH * 2,
						COMPONENT_INSET_Y,
						COMPONENT_SPACING * 3 + TIME_DISPLAY_WIDTH * 2 + EM_METRICS_MEDIUM_SLIM_BUTTON_WIDTH - 1,
						COMPONENT_INSET_Y + EM_METRICS_LABEL_HEIGHT - 1),
					NULL, EM_COLOR_VIEW_BACKGROUND, true),
m_oTempoValueBox(m_opView, EMRect(COMPONENT_SPACING * 3 + TIME_DISPLAY_WIDTH * 2,
						COMPONENT_INSET_Y + EM_METRICS_LABEL_HEIGHT,
						COMPONENT_SPACING * 3 + TIME_DISPLAY_WIDTH * 2 + EM_METRICS_MEDIUM_SLIM_BUTTON_WIDTH - 1,
						COMPONENT_INSET_Y + EM_METRICS_SLIM_BUTTON_HEIGHT * 2 + COMPONENT_SPACING - 1),
					30, 300, 120,
					0, NULL, NULL, NULL, true, true, true, true, SETTING_TEMPO, true, "Displays The Current Tempo In BPM"),
m_oTimeDisplayOne(m_opView, EMRect(COMPONENT_SPACING,
						COMPONENT_INSET_Y + EM_METRICS_LABEL_HEIGHT,
						COMPONENT_SPACING + TIME_DISPLAY_WIDTH - 1,
						COMPONENT_INSET_Y + EM_METRICS_SLIM_BUTTON_HEIGHT * 2 + COMPONENT_SPACING - 1),
						EM_SMPTE),
m_oTimeDisplayTwo(m_opView, EMRect(COMPONENT_SPACING * 2 + TIME_DISPLAY_WIDTH,
						COMPONENT_INSET_Y + EM_METRICS_LABEL_HEIGHT,
						COMPONENT_SPACING * 2 + TIME_DISPLAY_WIDTH * 2 - 1,
						COMPONENT_INSET_Y + EM_METRICS_SLIM_BUTTON_HEIGHT * 2 + COMPONENT_SPACING - 1),
						EM_MEASURES),
m_oTransportBeginningButton(m_opView, 
					EMRect(COMPONENT_SPACING * 5 + TIME_DISPLAY_WIDTH * 2 + EM_METRICS_MEDIUM_SLIM_BUTTON_WIDTH * 2 + TRANSPORT_BUTTON_WIDTH,
						COMPONENT_INSET_Y + EM_METRICS_LABEL_HEIGHT,
						COMPONENT_SPACING * 5 + TIME_DISPLAY_WIDTH * 2 + EM_METRICS_MEDIUM_SLIM_BUTTON_WIDTH * 2 + TRANSPORT_BUTTON_WIDTH * 2 - 1,
						COMPONENT_INSET_Y + EM_METRICS_SLIM_BUTTON_HEIGHT * 2 + COMPONENT_SPACING - 1),
					NULL, NULL, COMMAND_PROJECT_BEGIN, NULL, NULL, NULL, false, false, false, false,
					EM_COLOR_BUTTON_DEFAULT_ON, EM_COLOR_BUTTON_DEFAULT_OFF,
					NULL, NULL, NULL, NULL, "Sets The Current Position To The Beginning Of The Project (Home)"),
m_oTransportEndButton(m_opView, 
					EMRect(COMPONENT_SPACING * 5 + TIME_DISPLAY_WIDTH * 2 + EM_METRICS_MEDIUM_SLIM_BUTTON_WIDTH * 2 + TRANSPORT_BUTTON_WIDTH * 7,
						COMPONENT_INSET_Y + EM_METRICS_LABEL_HEIGHT,
						COMPONENT_SPACING * 5 + TIME_DISPLAY_WIDTH * 2 + EM_METRICS_MEDIUM_SLIM_BUTTON_WIDTH * 2 + TRANSPORT_BUTTON_WIDTH * 8 - 1,
						COMPONENT_INSET_Y + EM_METRICS_SLIM_BUTTON_HEIGHT * 2 + COMPONENT_SPACING - 1),
					NULL, NULL, COMMAND_PROJECT_END, NULL, NULL, NULL, false, false, false, false,
					EM_COLOR_BUTTON_DEFAULT_ON, EM_COLOR_BUTTON_DEFAULT_OFF,
					NULL, NULL, NULL, NULL, "Sets The Current Position To The End Of The Project (End)"),
m_oTransportFastForwardButton(m_opView, 
					EMRect(COMPONENT_SPACING * 5 + TIME_DISPLAY_WIDTH * 2 + EM_METRICS_MEDIUM_SLIM_BUTTON_WIDTH * 2 + TRANSPORT_BUTTON_WIDTH * 5,
						COMPONENT_INSET_Y + EM_METRICS_LABEL_HEIGHT,
						COMPONENT_SPACING * 5 + TIME_DISPLAY_WIDTH * 2 + EM_METRICS_MEDIUM_SLIM_BUTTON_WIDTH * 2 + TRANSPORT_BUTTON_WIDTH * 6 - 1,
						COMPONENT_INSET_Y + EM_METRICS_SLIM_BUTTON_HEIGHT * 2 + COMPONENT_SPACING - 1),
					NULL, NULL, COMMAND_FAST_FORWARD, NULL, NULL, NULL, false, false, false, false,
					EM_COLOR_BUTTON_DEFAULT_ON, EM_COLOR_BUTTON_DEFAULT_OFF,
					NULL, NULL, NULL, NULL, "Fast-Forwards The Current Position (Right Arrow)"),
m_oTransportLabel(m_opView, EMRect(COMPONENT_SPACING * 5 + TIME_DISPLAY_WIDTH * 2 + EM_METRICS_MEDIUM_SLIM_BUTTON_WIDTH * 2,
						COMPONENT_INSET_Y,
						COMPONENT_SPACING * 5 + TIME_DISPLAY_WIDTH * 2 + EM_METRICS_MEDIUM_SLIM_BUTTON_WIDTH * 2 + TRANSPORT_BUTTON_WIDTH * 10 - 1,
						COMPONENT_INSET_Y + EM_METRICS_LABEL_HEIGHT - 1),
					NULL, EM_COLOR_VIEW_BACKGROUND, true),
m_oTransportLoopButton(m_opView, 
					EMRect(COMPONENT_SPACING * 5 + TIME_DISPLAY_WIDTH * 2 + EM_METRICS_MEDIUM_SLIM_BUTTON_WIDTH * 2,
						COMPONENT_INSET_Y + EM_METRICS_LABEL_HEIGHT,
						COMPONENT_SPACING * 5 + TIME_DISPLAY_WIDTH * 2 + EM_METRICS_MEDIUM_SLIM_BUTTON_WIDTH * 2 + TRANSPORT_BUTTON_WIDTH - 1,
						COMPONENT_INSET_Y + EM_METRICS_SLIM_BUTTON_HEIGHT * 2 + COMPONENT_SPACING - 1),
					NULL, NULL, COMMAND_TOGGLE_LOOP, NULL, NULL, NULL, false, false, false, false,
					EM_COLOR_BUTTON_DEFAULT_ON, EM_COLOR_BUTTON_DEFAULT_OFF,
					NULL, NULL, NULL, NULL, "Toggles Looping Within The Selected Region On/Off (L)"),
m_oTransportPlayButton(m_opView, 
					EMRect(COMPONENT_SPACING * 5 + TIME_DISPLAY_WIDTH * 2 + EM_METRICS_MEDIUM_SLIM_BUTTON_WIDTH * 2 + TRANSPORT_BUTTON_WIDTH * 4,
						COMPONENT_INSET_Y + EM_METRICS_LABEL_HEIGHT,
						COMPONENT_SPACING * 5 + TIME_DISPLAY_WIDTH * 2 + EM_METRICS_MEDIUM_SLIM_BUTTON_WIDTH * 2 + TRANSPORT_BUTTON_WIDTH * 5 - 1,
						COMPONENT_INSET_Y + EM_METRICS_SLIM_BUTTON_HEIGHT * 2 + COMPONENT_SPACING - 1),
					NULL, NULL, COMMAND_PLAY, NULL, NULL, NULL, false, false, false, false,
					EM_COLOR_BUTTON_DEFAULT_ON, EM_COLOR_BUTTON_DEFAULT_OFF,
					NULL, NULL, NULL, NULL, "Starts Playback Of The Project (Space)"),
m_oTransportRecordButton(m_opView, 
					EMRect(COMPONENT_SPACING * 5 + TIME_DISPLAY_WIDTH * 2 + EM_METRICS_MEDIUM_SLIM_BUTTON_WIDTH * 2 + TRANSPORT_BUTTON_WIDTH * 9,
						COMPONENT_INSET_Y + EM_METRICS_LABEL_HEIGHT,
						COMPONENT_SPACING * 5 + TIME_DISPLAY_WIDTH * 2 + EM_METRICS_MEDIUM_SLIM_BUTTON_WIDTH * 2 + TRANSPORT_BUTTON_WIDTH * 10 - 1,
						COMPONENT_INSET_Y + EM_METRICS_SLIM_BUTTON_HEIGHT * 2 + COMPONENT_SPACING - 1),
					NULL, NULL, COMMAND_PREPARE_RECORDING, NULL, NULL, NULL, false, false, false, false,
					EM_COLOR_BUTTON_DEFAULT_ON, EM_COLOR_BUTTON_DEFAULT_OFF,
					NULL, NULL, NULL, NULL, "Starts Recording On Armed Tracks From The Current Position (*)"),
m_oTransportRegionStartButton(m_opView, 
					EMRect(COMPONENT_SPACING * 5 + TIME_DISPLAY_WIDTH * 2 + EM_METRICS_MEDIUM_SLIM_BUTTON_WIDTH * 2 + TRANSPORT_BUTTON_WIDTH * 2,
						COMPONENT_INSET_Y + EM_METRICS_LABEL_HEIGHT,
						COMPONENT_SPACING * 5 + TIME_DISPLAY_WIDTH * 2 + EM_METRICS_MEDIUM_SLIM_BUTTON_WIDTH * 2 + TRANSPORT_BUTTON_WIDTH * 3 - 1,
						COMPONENT_INSET_Y + EM_METRICS_SLIM_BUTTON_HEIGHT * 2 + COMPONENT_SPACING - 1),
					NULL, NULL, COMMAND_REGION_BEGIN, NULL, NULL, NULL, false, false, false, false,
					EM_COLOR_BUTTON_DEFAULT_ON, EM_COLOR_BUTTON_DEFAULT_OFF,
					NULL, NULL, NULL, NULL, "Sets The Current Position To The Beginning Of The Selected Region (Shift-Home)"),
m_oTransportRewindButton(m_opView, 
					EMRect(COMPONENT_SPACING * 5 + TIME_DISPLAY_WIDTH * 2 + EM_METRICS_MEDIUM_SLIM_BUTTON_WIDTH * 2 + TRANSPORT_BUTTON_WIDTH * 3,
						COMPONENT_INSET_Y + EM_METRICS_LABEL_HEIGHT,
						COMPONENT_SPACING * 5 + TIME_DISPLAY_WIDTH * 2 + EM_METRICS_MEDIUM_SLIM_BUTTON_WIDTH * 2 + TRANSPORT_BUTTON_WIDTH * 4 - 1,
						COMPONENT_INSET_Y + EM_METRICS_SLIM_BUTTON_HEIGHT * 2 + COMPONENT_SPACING - 1),
					NULL, NULL, COMMAND_REWIND, NULL, NULL, NULL, false, false, false, false,
					EM_COLOR_BUTTON_DEFAULT_ON, EM_COLOR_BUTTON_DEFAULT_OFF,
					NULL, NULL, NULL, NULL, "Rewinds The Current Position (Left Arrow)"),
m_oTransportRegionEndButton(m_opView, 
					EMRect(COMPONENT_SPACING * 5 + TIME_DISPLAY_WIDTH * 2 + EM_METRICS_MEDIUM_SLIM_BUTTON_WIDTH * 2 + TRANSPORT_BUTTON_WIDTH * 6,
						COMPONENT_INSET_Y + EM_METRICS_LABEL_HEIGHT,
						COMPONENT_SPACING * 5 + TIME_DISPLAY_WIDTH * 2 + EM_METRICS_MEDIUM_SLIM_BUTTON_WIDTH * 2 + TRANSPORT_BUTTON_WIDTH * 7 - 1,
						COMPONENT_INSET_Y + EM_METRICS_SLIM_BUTTON_HEIGHT * 2 + COMPONENT_SPACING - 1),
					NULL, NULL, COMMAND_REGION_END, NULL, NULL, NULL, false, false, false, false,
					EM_COLOR_BUTTON_DEFAULT_ON, EM_COLOR_BUTTON_DEFAULT_OFF,
					NULL, NULL, NULL, NULL, "Sets The Current Position To The End Of The Selected Region (Shift-End)"),
m_oTransportStopButton(m_opView, 
					EMRect(COMPONENT_SPACING * 5 + TIME_DISPLAY_WIDTH * 2 + EM_METRICS_MEDIUM_SLIM_BUTTON_WIDTH * 2 + TRANSPORT_BUTTON_WIDTH * 8,
						COMPONENT_INSET_Y + EM_METRICS_LABEL_HEIGHT,
						COMPONENT_SPACING * 5 + TIME_DISPLAY_WIDTH * 2 + EM_METRICS_MEDIUM_SLIM_BUTTON_WIDTH * 2 + TRANSPORT_BUTTON_WIDTH * 9 - 1,
						COMPONENT_INSET_Y + EM_METRICS_SLIM_BUTTON_HEIGHT * 2 + COMPONENT_SPACING - 1),
					NULL, NULL, COMMAND_STOP, NULL, NULL, NULL, false, false, false, false,
					EM_COLOR_BUTTON_DEFAULT_ON, EM_COLOR_BUTTON_DEFAULT_OFF,
					NULL, NULL, NULL, NULL, "Stops Playback (Space)"),
m_vLoopButtonEnabled(false)
{
}

TrackWindowToolBar::~TrackWindowToolBar()
{
	delete m_opView;
}

EMRect TrackWindowToolBar::CalculateMinimumBounds()
{
	return EMRect(0,
				0,
				COMPONENT_SPACING * 9 + TIME_DISPLAY_WIDTH * 2 + EM_METRICS_MEDIUM_SLIM_BUTTON_WIDTH * 2 + TRANSPORT_BUTTON_WIDTH * 13 + EM_METRICS_LARGE_SLIM_BUTTON_WIDTH * 2 - 1,
				COMPONENT_INSET_Y * 2 + TINY_BUTTON_HEIGHT + EM_METRICS_SLIM_BUTTON_HEIGHT + COMPONENT_SPACING - 1);
}

bool TrackWindowToolBar::ClearData()
{
	m_oAutoQuantizeButton.SetOn(false);
	m_oEditClipsButton.SetOn(true);
	m_oEditFXButton.SetOn(false);
	m_oEditTracksButton.SetOn(false);
	m_oSnapEnableButton.SetOn(false);
	m_oTransportBeginningButton.SetOn(false);
	m_oTransportEndButton.SetOn(false);
	m_oTransportFastForwardButton.SetOn(false);
	m_oTransportLoopButton.SetOn(false);
	m_oTransportPlayButton.SetOn(false);
	m_oTransportRecordButton.SetOn(false);
	m_oTransportRegionStartButton.SetOn(false);
	m_oTransportRewindButton.SetOn(false);
	m_oTransportRegionEndButton.SetOn(false);
	m_oTransportStopButton.SetOn(false);

	return true;
}

void TrackWindowToolBar::Draw(EMRect p_oClippingRect)
{
	m_opView -> SetHighColor(EM_COLOR_VIEW_BACKGROUND_HIGHLIGHT);
	m_opView -> StrokeLine(0, 0, m_opView -> Bounds().m_vRight, 0);
	m_opView -> SetHighColor(EM_COLOR_VIEW_BACKGROUND_SHADOW);
	m_opView -> StrokeLine(0, m_opView -> Bounds().m_vBottom, m_opView -> Bounds().m_vRight, m_opView -> Bounds().m_vBottom);
	m_opView -> SetHighColor(EM_COLOR_VIEW_BACKGROUND);
	m_opView -> FillRect(EMRect(0, 1, m_opView -> Bounds().m_vRight, m_opView -> Bounds().m_vBottom - 1));

//	m_oAffectAllButton.Draw(p_oClippingRect, m_opView);
	m_oAutoQuantizeButton.Draw(p_oClippingRect, m_opView);
	m_oEditClipsButton.Draw(p_oClippingRect, m_opView);
	m_oEditFXButton.Draw(p_oClippingRect, m_opView);
	m_oEditTracksButton.Draw(p_oClippingRect, m_opView);
	m_oQuantizeModeValueBox.Draw(p_oClippingRect);
//	m_oRelativeButton.Draw(p_oClippingRect, m_opView);
	m_oSignatureValueBox.Draw(p_oClippingRect);
	m_oSnapEnableButton.Draw(p_oClippingRect, m_opView);
	m_oSnappingModeValueBox.Draw(p_oClippingRect);
	m_oTempoValueBox.Draw(p_oClippingRect);
	m_oTimeDisplayOne.Draw(p_oClippingRect);
	m_oTimeDisplayTwo.Draw(p_oClippingRect);
	
	m_oTransportBeginningButton.Draw(p_oClippingRect, m_opView);
	m_oTransportEndButton.Draw(p_oClippingRect, m_opView);
	m_oTransportFastForwardButton.Draw(p_oClippingRect, m_opView);
	m_oTransportLoopButton.Draw(p_oClippingRect, m_opView);
	m_oTransportPlayButton.Draw(p_oClippingRect, m_opView);
	m_oTransportRecordButton.Draw(p_oClippingRect, m_opView);
	m_oTransportRegionStartButton.Draw(p_oClippingRect, m_opView);
	m_oTransportRewindButton.Draw(p_oClippingRect, m_opView);
	m_oTransportRegionEndButton.Draw(p_oClippingRect, m_opView);
	m_oTransportStopButton.Draw(p_oClippingRect, m_opView);	
	
	m_oMeasuresLabel.Draw(p_oClippingRect);
	m_oEditModeLabel.Draw(p_oClippingRect);
	m_oPositionLabel.Draw(p_oClippingRect);
	m_oTransportLabel.Draw(p_oClippingRect);	
	m_oQuantizeLabel.Draw(p_oClippingRect);
	m_oSignatureLabel.Draw(p_oClippingRect);
	m_oSnapLabel.Draw(p_oClippingRect);
	m_oTempoLabel.Draw(p_oClippingRect);
}

EMView* TrackWindowToolBar::GetView()
{
	return m_opView;
}

void TrackWindowToolBar::Init()
{
	m_oAutoQuantizeButton.SetOnString(EMResourceRepository::GetString(RES_TB_AUTO_QUANTIZE));
	m_oAutoQuantizeButton.SetOffString(EMResourceRepository::GetString(RES_TB_AUTO_QUANTIZE));
	m_oEditClipsButton.SetBitmaps(EMResourceRepository::GetBitmap(RES_EM_EDIT_CLIPS_ON), EMResourceRepository::GetBitmap(RES_EM_EDIT_CLIPS));
	m_oEditFXButton.SetBitmaps(EMResourceRepository::GetBitmap(RES_EM_EDIT_FX_ON), EMResourceRepository::GetBitmap(RES_EM_EDIT_FX));
	m_oEditModeLabel.SetLabel(EMResourceRepository::GetString(RES_TB_LABEL_EDIT_MODE));
	m_oEditTracksButton.SetBitmaps(EMResourceRepository::GetBitmap(RES_EM_EDIT_TRACKS_ON), EMResourceRepository::GetBitmap(RES_EM_EDIT_TRACKS));
	m_oMeasuresLabel.SetLabel(EMResourceRepository::GetString(RES_TB_LABEL_MEASURES));
	m_oPositionLabel.SetLabel(EMResourceRepository::GetString(RES_TB_LABEL_POSITION));
	m_oQuantizeLabel.SetLabel(EMResourceRepository::GetString(RES_TB_LABEL_QUANTIZE));
	m_oSignatureLabel.SetLabel(EMResourceRepository::GetString(RES_TB_LABEL_SIGNATURE));
	m_oSnapEnableButton.SetOnString(EMResourceRepository::GetString(RES_SR_ON));
	m_oSnapEnableButton.SetOffString(EMResourceRepository::GetString(RES_SR_ON));
	m_oSnapLabel.SetLabel(EMResourceRepository::GetString(RES_TB_LABEL_SNAP));
	m_oTempoLabel.SetLabel(EMResourceRepository::GetString(RES_TB_LABEL_TEMPO));
	m_oTransportBeginningButton.SetBitmaps(EMResourceRepository::GetBitmap(RES_TB_PROJ_BEGIN_ON), EMResourceRepository::GetBitmap(RES_TB_PROJ_BEGIN));
	m_oTransportEndButton.SetBitmaps(EMResourceRepository::GetBitmap(RES_TB_PROJ_END_ON), EMResourceRepository::GetBitmap(RES_TB_PROJ_END));
	m_oTransportFastForwardButton.SetBitmaps(EMResourceRepository::GetBitmap(RES_TB_FAST_FORWARD_ON), EMResourceRepository::GetBitmap(RES_TB_FAST_FORWARD));
	m_oTransportLabel.SetLabel(EMResourceRepository::GetString(RES_TB_LABEL_TRANSPORT));
	m_oTransportLoopButton.SetBitmaps(EMResourceRepository::GetBitmap(RES_TB_LOOP_ON), EMResourceRepository::GetBitmap(RES_TB_LOOP));
	m_oTransportPlayButton.SetBitmaps(EMResourceRepository::GetBitmap(RES_TB_PLAY_ON), EMResourceRepository::GetBitmap(RES_TB_PLAY));
	m_oTransportRecordButton.SetBitmaps(EMResourceRepository::GetBitmap(RES_TB_RECORD_ON), EMResourceRepository::GetBitmap(RES_TB_RECORD));
	m_oTransportRegionStartButton.SetBitmaps(EMResourceRepository::GetBitmap(RES_TB_REG_BEGIN_ON), EMResourceRepository::GetBitmap(RES_TB_REG_BEGIN));
	m_oTransportRewindButton.SetBitmaps(EMResourceRepository::GetBitmap(RES_TB_REWIND_ON), EMResourceRepository::GetBitmap(RES_TB_REWIND));
	m_oTransportRegionEndButton.SetBitmaps(EMResourceRepository::GetBitmap(RES_TB_REG_END_ON), EMResourceRepository::GetBitmap(RES_TB_REG_END));
	m_oTransportStopButton.SetBitmaps(EMResourceRepository::GetBitmap(RES_TB_STOP_ON), EMResourceRepository::GetBitmap(RES_TB_STOP));

	m_opView -> AddListener(this);
	
	m_oTimeDisplayOne.Init();
	m_oTimeDisplayTwo.Init();
/*	
	vector<string> oValueBoxValues;
	list<string*>* opValueBoxValues = EMResourceRepository::GetStringList(RES_TB_TIME_SIGNATURES);
	list<string*>::iterator oIterator = opValueBoxValues -> begin();
	while(oIterator != opValueBoxValues -> end())
	{
		oValueBoxValues.push_back(*oIterator);
		oIterator++;
	}
	m_oSignatureValueBox.SetValues(oValueBoxValues);
*/
	m_oSignatureValueBox.SetValues(EMResourceRepository::GetStringList(RES_TB_TIME_SIGNATURES), false);
	m_vSignature = static_cast<string*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_SIGNATURE));
	m_oSignatureValueBox.SetValue(*m_vSignature);
/*
	oValueBoxValues.clear();
	
	opValueBoxValues = EMResourceRepository::GetStringList(RES_TB_SNAPPING_MODES);
	oIterator = opValueBoxValues -> begin();
	while(oIterator != opValueBoxValues -> end())
	{
		oValueBoxValues.push_back(*oIterator);
		oIterator++;
	}
	m_oSnappingModeValueBox.SetValues(oValueBoxValues);
*/
	m_oSnappingModeValueBox.SetValues(EMResourceRepository::GetStringList(RES_TB_SNAPPING_MODES), false);
/*
	oValueBoxValues.clear();

	opValueBoxValues = EMResourceRepository::GetStringList(RES_TB_QUANTIZE_MODES);
	oIterator = opValueBoxValues -> begin();
	while(oIterator != opValueBoxValues -> end())
	{
		oValueBoxValues.push_back(*oIterator);
		oIterator++;
	}
	m_oQuantizeModeValueBox.SetValues(oValueBoxValues);
*/
	m_oQuantizeModeValueBox.SetValues(EMResourceRepository::GetStringList(RES_TB_QUANTIZE_MODES), false);

	m_vTempo = *(static_cast<float*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_TEMPO)));
	m_oTempoValueBox.SetValue(static_cast<int32>(m_vTempo));

//	m_oRelativeButton.SetOn(*(static_cast<bool*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_RELATIVE))));

	EMSettingsRepository::Instance() -> AddListener(this);

	m_oSnappingModeValueBox.SetValue(*(static_cast<string*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_SNAPPING_MODE))));
	m_oSnapEnableButton.SetOn(*(static_cast<bool*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_SNAPPING))));
	m_oTransportLoopButton.SetOn(*(static_cast<bool*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_LOOP))));
	EMEditMode eEditMode = static_cast<EMEditMode>(*(static_cast<int*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_EDIT_MODE))));
	if(eEditMode == EM_TRACK_EDIT_MODE)
		m_oEditTracksButton.SetOn(true);
	else if(eEditMode == EM_CLIP_EDIT_MODE)
		m_oEditClipsButton.SetOn(true);
	else
		m_oEditFXButton.SetOn(true);

	m_oTransportBeginningButton.SetMode(EM_SLIM_BUTTON_FOLLOW_MOUSE);
	m_oTransportEndButton.SetMode(EM_SLIM_BUTTON_FOLLOW_MOUSE);
	m_oTransportFastForwardButton.SetMode(EM_SLIM_BUTTON_FOLLOW_MOUSE_REPEAT_COMMAND);
	m_oTransportRegionStartButton.SetMode(EM_SLIM_BUTTON_FOLLOW_MOUSE);
	m_oTransportRewindButton.SetMode(EM_SLIM_BUTTON_FOLLOW_MOUSE_REPEAT_COMMAND);
	m_oTransportRegionEndButton.SetMode(EM_SLIM_BUTTON_FOLLOW_MOUSE);
	m_oTransportStopButton.SetMode(EM_SLIM_BUTTON_FOLLOW_MOUSE);
	
	m_oEditTracksButton.SetMode(EM_SLIM_BUTTON_RADIO);
	m_oEditClipsButton.SetMode(EM_SLIM_BUTTON_RADIO);
	m_oEditFXButton.SetMode(EM_SLIM_BUTTON_RADIO);
	
	LayoutComponents();

	m_vMIDIEnabled = *(static_cast<bool*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_MIDI_ENABLED)));
	SetMIDIEnabled();
}

void TrackWindowToolBar::LayoutComponents()
{
	EMRect oBounds = m_opView -> Bounds();
/*	
	if(oBounds.GetWidth() < COMPONENT_SPACING * 11 + TIME_DISPLAY_WIDTH * 2 + EM_METRICS_MEDIUM_SLIM_BUTTON_WIDTH * 5 + TRANSPORT_BUTTON_WIDTH * 9 + EM_METRICS_LARGE_SLIM_BUTTON_WIDTH * 2 - 1)
	{
		m_oTransportBeginningButton.SetVisible(false);
		m_oTransportEndButton.SetVisible(false);
		m_oTransportFastForwardButton.SetVisible(false);
		m_oTransportPlayButton.SetVisible(false);
		m_oTransportRecordButton.SetVisible(false);
		m_oTransportRegionStartButton.SetVisible(false);
		m_oTransportRewindButton.SetVisible(false);
		m_oTransportRegionEndButton.SetVisible(false);
		m_oTransportStopButton.SetVisible(false);
	}
	else
	{
		m_oTransportBeginningButton.SetVisible(true);
		m_oTransportEndButton.SetVisible(true);
		m_oTransportFastForwardButton.SetVisible(true);
		m_oTransportPlayButton.SetVisible(true);
		m_oTransportRecordButton.SetVisible(true);
		m_oTransportRegionStartButton.SetVisible(true);
		m_oTransportRewindButton.SetVisible(true);
		m_oTransportRegionEndButton.SetVisible(true);
		m_oTransportStopButton.SetVisible(true);
	}
*/
	m_oQuantizeLabel.SetFrame(
						EMRect(oBounds.m_vRight - COMPONENT_SPACING * 2 - EM_METRICS_LARGE_SLIM_BUTTON_WIDTH * 2 + 1,
								COMPONENT_INSET_Y,
								oBounds.m_vRight - COMPONENT_SPACING * 2 - EM_METRICS_LARGE_SLIM_BUTTON_WIDTH,
								COMPONENT_INSET_Y + EM_METRICS_LABEL_HEIGHT - 1));
	m_oQuantizeModeValueBox.SetFrame(
								EMRect(oBounds.m_vRight - COMPONENT_SPACING * 2 - EM_METRICS_LARGE_SLIM_BUTTON_WIDTH * 2 + 1,
										COMPONENT_INSET_Y + EM_METRICS_LABEL_HEIGHT,
										oBounds.m_vRight - COMPONENT_SPACING * 2 - EM_METRICS_LARGE_SLIM_BUTTON_WIDTH - EM_METRICS_SMALL_SLIM_BUTTON_WIDTH,
										COMPONENT_INSET_Y + EM_METRICS_SLIM_BUTTON_HEIGHT + COMPONENT_SPACING + EM_METRICS_SLIM_BUTTON_HEIGHT - 1));
	m_oAutoQuantizeButton.SetFrame(
								EMRect(oBounds.m_vRight - COMPONENT_SPACING * 2 - EM_METRICS_LARGE_SLIM_BUTTON_WIDTH - EM_METRICS_SMALL_SLIM_BUTTON_WIDTH + 1,
										COMPONENT_INSET_Y + EM_METRICS_LABEL_HEIGHT,
										oBounds.m_vRight - COMPONENT_SPACING * 2 - EM_METRICS_LARGE_SLIM_BUTTON_WIDTH,
										COMPONENT_INSET_Y + EM_METRICS_SLIM_BUTTON_HEIGHT + COMPONENT_SPACING + EM_METRICS_SLIM_BUTTON_HEIGHT - 1));
	m_oSnapLabel.SetFrame(
						EMRect(oBounds.m_vRight - COMPONENT_SPACING - EM_METRICS_LARGE_SLIM_BUTTON_WIDTH + 1,
								COMPONENT_INSET_Y,
								oBounds.m_vRight - COMPONENT_SPACING,
								COMPONENT_INSET_Y + EM_METRICS_LABEL_HEIGHT - 1));
	m_oSnappingModeValueBox.SetFrame(
								EMRect(oBounds.m_vRight - COMPONENT_SPACING - EM_METRICS_LARGE_SLIM_BUTTON_WIDTH + 1,
										COMPONENT_INSET_Y + EM_METRICS_LABEL_HEIGHT,
										oBounds.m_vRight - COMPONENT_SPACING - EM_METRICS_SMALL_SLIM_BUTTON_WIDTH,
										COMPONENT_INSET_Y + EM_METRICS_SLIM_BUTTON_HEIGHT + COMPONENT_SPACING + EM_METRICS_SLIM_BUTTON_HEIGHT - 1));
	m_oSnapEnableButton.SetFrame(
								EMRect(oBounds.m_vRight - COMPONENT_SPACING - EM_METRICS_SMALL_SLIM_BUTTON_WIDTH + 1,
										COMPONENT_INSET_Y + EM_METRICS_LABEL_HEIGHT,
										oBounds.m_vRight - COMPONENT_SPACING,
										COMPONENT_INSET_Y + EM_METRICS_SLIM_BUTTON_HEIGHT + COMPONENT_SPACING + EM_METRICS_SLIM_BUTTON_HEIGHT - 1));
}

void TrackWindowToolBar::LoadComplete()
{
//	m_oSnapEnableButton.SetOn(*(static_cast<bool*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_SNAPPING))));
//	m_oTransportLoopButton.SetOn(*(static_cast<bool*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_LOOP))));
//	m_oAutoQuantizeButton.SetOn(*(static_cast<bool*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_MIDI_AUTO_QUANTIZE_ENABLED))));
}
/*
bool TrackWindowToolBar::LoadData(void* p_upProjectDataLoader)
{
	EMProjectDataLoader* opLoader = static_cast<EMProjectDataLoader*>(p_upProjectDataLoader);
	m_oTransportRecordButton.SetOn(opLoader -> LoadBool());
	return true;
}
*/
bool TrackWindowToolBar::MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage)
{
	int32 vX;
	int32 vY;
	bool vButtonOne;
	bool vButtonTwo;
	bool vButtonThree;
//	bool vRelativeMove;
//	bool vSnapping;
	EMEditMode eEditMode;
	bool vLoop;
	bool vMIDIEnabled;

	switch(p_vMessage)
	{
	case EM_DRAW:
		Draw(m_opView -> GetClippingRect());
		return true;
	case EM_MOUSE_DOWN:
		m_opView -> GetMouse(vX, vY, vButtonOne, vButtonTwo, vButtonThree);
		MouseDown(vX, vY, vButtonOne, vButtonTwo, vButtonThree);
		return true;
	case EM_MOUSE_MOVED:
		m_opView -> GetMouse(vX, vY, vButtonOne, vButtonTwo, vButtonThree);
		MouseMoved(vX, vY, vButtonOne, vButtonTwo, vButtonThree);
		return true;
	case EM_MOUSE_UP:
		m_opView -> GetMouse(vX, vY, vButtonOne, vButtonTwo, vButtonThree);
		MouseUp(vX, vY);
		return true;
	case EM_VIEW_BOUNDS_CHANGED:
		LayoutComponents();
		m_opView -> Invalidate(EMRect(m_oEditFXButton.Frame().m_vRight + 1, 0, (m_opView -> Bounds()).m_vRight, (m_opView -> Bounds()).m_vBottom));
		return true;
	case EM_SETTING_UPDATED:
eo << "TrackWindowToolBar: EM_SETTING_UPDATED" << ef;
/*
		vRelativeMove = *(static_cast<bool*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_RELATIVE)));
		if(vRelativeMove != m_oRelativeButton.GetOn())
		{
			if(m_opView -> LockWindow())
			{
				if(EMCommandRepository::Instance() -> TriggeredByBinding())
					m_oRelativeButton.SetOn(vRelativeMove);
				m_opView -> Invalidate(m_oRelativeButton.Frame());
				m_opView -> UnlockWindow();
			}
		}
*/
		if(m_opView -> LockWindow())
		{
			m_oQuantizeModeValueBox.SetValue(*(static_cast<string*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_MIDI_AUTO_QUANTIZE_MODE))));

			m_oSnappingModeValueBox.SetValue(*(static_cast<string*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_SNAPPING_MODE))));
/*
			vSnapping = *(static_cast<bool*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_SNAPPING)));
			if(vSnapping != m_oSnapEnableButton.GetOn())
			{
				if(EMCommandRepository::Instance() -> TriggeredByBinding())
					m_oSnapEnableButton.SetOn(vSnapping);
				m_opView -> Invalidate(m_oSnapEnableButton.Frame());
			}
*/
			eEditMode = static_cast<EMEditMode>(*(static_cast<int*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_EDIT_MODE))));
			if((eEditMode == EM_TRACK_EDIT_MODE && !(m_oEditTracksButton.GetOn() && !m_oEditClipsButton.GetOn() && !m_oEditFXButton.GetOn())) ||
				(eEditMode == EM_CLIP_EDIT_MODE && !(!m_oEditTracksButton.GetOn() && m_oEditClipsButton.GetOn() && !m_oEditFXButton.GetOn())) ||
				(eEditMode == EM_FX_EDIT_MODE && !(!m_oEditTracksButton.GetOn() && !m_oEditClipsButton.GetOn() && m_oEditFXButton.GetOn())))
			{
				if(eEditMode == EM_TRACK_EDIT_MODE)
				{
					m_oEditTracksButton.SetOn(true);
					m_oEditClipsButton.SetOn(false);
					m_oEditFXButton.SetOn(false);
				}
				else if(eEditMode == EM_CLIP_EDIT_MODE)
				{
					m_oEditTracksButton.SetOn(false);
					m_oEditClipsButton.SetOn(true);
					m_oEditFXButton.SetOn(false);
				}
				else
				{
					m_oEditTracksButton.SetOn(false);
					m_oEditClipsButton.SetOn(false);
					m_oEditFXButton.SetOn(true);
				}
				m_opView -> Invalidate(m_oEditTracksButton.Frame() | m_oEditClipsButton.Frame() | m_oEditFXButton.Frame());
			}

			vLoop = *(static_cast<bool*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_LOOP)));
			if(vLoop != m_oTransportLoopButton.GetOn())
			{
//				if(EMCommandRepository::Instance() -> TriggeredByBinding())
					m_oTransportLoopButton.SetOn(vLoop);
				m_opView -> Invalidate(m_oTransportLoopButton.Frame());
			}

			int64 vRegionBegin = *(static_cast<int64*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_REGION_BEGIN)));
			int64 vRegionEnd = *(static_cast<int64*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_REGION_END)));
			if(vRegionBegin >= 0 && vRegionEnd > vRegionBegin)
				m_vLoopButtonEnabled = true;
			else
				m_vLoopButtonEnabled = false;

			vMIDIEnabled = *(static_cast<bool*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_MIDI_ENABLED)));
			if(vMIDIEnabled != m_vMIDIEnabled)
			{
				m_vMIDIEnabled = vMIDIEnabled;
				SetMIDIEnabled();
				m_opView -> Invalidate(m_opView -> Bounds());
			}

			bool vSnappingEnabled = *(static_cast<bool*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_SNAPPING)));
			if(vSnappingEnabled != m_oSnapEnableButton.GetOn())
			{
				m_oSnapEnableButton.SetOn(*(static_cast<bool*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_SNAPPING))));
				m_opView -> Invalidate(m_opView -> Bounds());
			}

			float vTempo = *(static_cast<float*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_TEMPO)));
			if(vTempo != *(static_cast<int32*>(m_oTempoValueBox.GetValue())))
			{
				m_oTempoValueBox.SetValue(vTempo);
				m_opView -> Invalidate(m_opView -> Bounds());
			}

			string* opSignature = static_cast<string*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_SIGNATURE));
			if(*opSignature != *(static_cast<string*>(m_oSignatureValueBox.GetValue())))
			{
				m_oSignatureValueBox.SetValue(*opSignature);
				m_opView -> Invalidate(m_opView -> Bounds());
			}

			bool vAQOn = *(static_cast<bool*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_MIDI_AUTO_QUANTIZE_ENABLED)));
			if(vAQOn != m_oAutoQuantizeButton.GetOn())
			{
eo << "SETTING_MIDI_AUTO_QUANTIZE_ENABLED: " << vAQOn << ef;
				m_oAutoQuantizeButton.SetOn(vAQOn);
				m_opView -> Invalidate(m_oAutoQuantizeButton.Frame());
			}

			m_opView -> UnlockWindow();
		}
		return true;
	default:
		return false;
	}
}

void TrackWindowToolBar::MouseDown(int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree)
{
	EMSlimButton* opButton = NULL;
	EMSlimValueBox* opValueBox = NULL;

//		if(m_oAffectAllButton.MouseDown(p_vX, p_vY))
//			m_oAffectAllButton.Draw(m_oAffectAllButton.Frame(), m_opView);
	if(m_oAutoQuantizeButton.MouseDown(p_vX, p_vY))
		opButton = &m_oAutoQuantizeButton;
	else if(m_oEditClipsButton.MouseDown(p_vX, p_vY))
		opButton = &m_oEditClipsButton;
	else if(m_oEditFXButton.MouseDown(p_vX, p_vY))
		opButton = &m_oEditFXButton;
	else if(m_oEditTracksButton.MouseDown(p_vX, p_vY))
		opButton = &m_oEditTracksButton;
//		else if(m_oRelativeButton.MouseDown(p_vX, p_vY))
//			m_oRelativeButton.Draw(m_oRelativeButton.Frame(), m_opView);
	else if(m_oSnapEnableButton.MouseDown(p_vX, p_vY))
		opButton = &m_oSnapEnableButton;

	else if(m_oQuantizeModeValueBox.MouseDown(p_vX, p_vY, p_vButtonOne, p_vButtonTwo, p_vButtonThree))
		opValueBox = &m_oQuantizeModeValueBox;
	else if(m_oSignatureValueBox.MouseDown(p_vX, p_vY, p_vButtonOne, p_vButtonTwo, p_vButtonThree))
		opValueBox = &m_oSignatureValueBox;
	else if(m_oSnappingModeValueBox.MouseDown(p_vX, p_vY, p_vButtonOne, p_vButtonTwo, p_vButtonThree))
		opValueBox = &m_oSnappingModeValueBox;
	else if(m_oTempoValueBox.MouseDown(p_vX, p_vY, p_vButtonOne, p_vButtonTwo, p_vButtonThree))
		opValueBox = &m_oTempoValueBox;

	else if(m_oTransportBeginningButton.MouseDown(p_vX, p_vY))
		opButton = &m_oTransportBeginningButton;
	else if(m_oTransportEndButton.MouseDown(p_vX, p_vY))
		opButton = &m_oTransportEndButton;
	else if(m_oTransportFastForwardButton.MouseDown(p_vX, p_vY))
		opButton = &m_oTransportFastForwardButton;
	else if(m_oTransportLoopButton.Frame().Contains(p_vX, p_vY) && m_vLoopButtonEnabled)		
	{
		if(m_oTransportLoopButton.MouseDown(p_vX, p_vY))
			opButton = &m_oTransportLoopButton;
	}
	else if(m_oTransportPlayButton.MouseDown(p_vX, p_vY))
		opButton = &m_oTransportPlayButton;
	else if(m_oTransportRecordButton.MouseDown(p_vX, p_vY))
	{
		m_oTransportRecordButton.SetOn(!m_oTransportLoopButton.GetOn()); // Quick'n dirty fix
		opButton = &m_oTransportRecordButton;
	}
	else if(m_oTransportRegionStartButton.MouseDown(p_vX, p_vY))
		opButton = &m_oTransportRegionStartButton;
	else if(m_oTransportRewindButton.MouseDown(p_vX, p_vY))
		opButton = &m_oTransportRewindButton;
	else if(m_oTransportRegionEndButton.MouseDown(p_vX, p_vY))
		opButton = &m_oTransportRegionEndButton;
	else if(m_oTransportStopButton.MouseDown(p_vX, p_vY))
		opButton = &m_oTransportStopButton;

	if(opButton != NULL || opValueBox != NULL)
	{
		m_opView -> BeginPaint();
		if(opButton != NULL)
			opButton -> Draw(opButton -> Frame(), m_opView);
		else if(opValueBox != NULL)
			opValueBox -> Draw(opValueBox -> Frame());
		m_opView -> EndPaint();
	}
}

void TrackWindowToolBar::MouseMoved(int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree)
{
	EMSlimValueBox* opValueBox = NULL;

	if(m_oQuantizeModeValueBox.MouseMoved(p_vX, p_vY, p_vButtonOne, p_vButtonTwo, p_vButtonThree))
		opValueBox = &m_oQuantizeModeValueBox;
	else if(m_oSignatureValueBox.MouseMoved(p_vX, p_vY, p_vButtonOne, p_vButtonTwo, p_vButtonThree))
		opValueBox = &m_oSignatureValueBox;
	else if(m_oSnappingModeValueBox.MouseMoved(p_vX, p_vY, p_vButtonOne, p_vButtonTwo, p_vButtonThree))
		opValueBox = &m_oSnappingModeValueBox;
	else if(m_oTempoValueBox.MouseMoved(p_vX, p_vY, p_vButtonOne, p_vButtonTwo, p_vButtonThree))
		opValueBox = &m_oTempoValueBox;

	if(opValueBox != NULL)
	{
		m_opView -> BeginPaint();
		opValueBox -> Draw(opValueBox -> Frame());
		m_opView -> EndPaint();
	}
}

void TrackWindowToolBar::MouseUp(int32 p_vX, int32 p_vY)
{
	EMSlimButton* opButton = NULL;
	EMSlimValueBox* opValueBox = NULL;

	if(m_oQuantizeModeValueBox.MouseUp(p_vX, p_vY))
		opValueBox = &m_oQuantizeModeValueBox;
	if(m_oSignatureValueBox.MouseUp(p_vX, p_vY))
		opValueBox = &m_oSignatureValueBox;
	if(m_oSnappingModeValueBox.MouseUp(p_vX, p_vY))
		opValueBox = &m_oSnappingModeValueBox;
	if(m_oTempoValueBox.MouseUp(p_vX, p_vY))
		opValueBox = &m_oTempoValueBox;
	if(m_oTransportBeginningButton.MouseUp(p_vX, p_vY))
		opButton = &m_oTransportBeginningButton;
	if(m_oTransportEndButton.MouseUp(p_vX, p_vY))
		opButton = &m_oTransportEndButton;
	if(m_oTransportFastForwardButton.MouseUp(p_vX, p_vY))
		opButton = &m_oTransportFastForwardButton;
	if(m_oTransportLoopButton.MouseUp(p_vX, p_vY))
		opButton = &m_oTransportLoopButton;
	if(m_oTransportRegionStartButton.MouseUp(p_vX, p_vY))
		opButton = &m_oTransportRegionStartButton;
	if(m_oTransportRewindButton.MouseUp(p_vX, p_vY))
		opButton = &m_oTransportRewindButton;
	if(m_oTransportRegionEndButton.MouseUp(p_vX, p_vY))
		opButton = &m_oTransportRegionEndButton;
	if(m_oTransportStopButton.MouseUp(p_vX, p_vY))
		opButton = &m_oTransportStopButton;

	if(opButton != NULL || opValueBox != NULL)
	{
		m_opView -> BeginPaint();
		if(opButton != NULL)
			opButton -> Draw(opButton -> Frame(), m_opView);
		else if(opValueBox != NULL)
			opValueBox -> Draw(opValueBox -> Frame());
		m_opView -> EndPaint();
	}
}

void TrackWindowToolBar::ReadyForUse()
{
	if(m_opView -> LockWindow())
	{
//		if(m_oTransportRecordButton.GetOn())
//			EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_PREPARE_RECORDING);
		m_opView -> Invalidate(m_opView -> Bounds());
		m_opView -> UnlockWindow();
	}
}
/*
bool TrackWindowToolBar::SaveData(void* p_upDataSaver)
{
	EMProjectDataSaver* opSaver = static_cast<EMProjectDataSaver*>(p_upDataSaver);
	opSaver -> SaveBool(m_oTransportRecordButton.GetOn());
	return true;
}
*/
void TrackWindowToolBar::SetFastForward()
{
;//cout_commented_out_4_release << "TrackWindowToolBar::SetFastForward" << endl;
	m_oTransportFastForwardButton.Flash();
}

void TrackWindowToolBar::SetMIDIEnabled()
{
	if(m_vMIDIEnabled)
	{
		m_oAutoQuantizeButton.SetMode(EM_SLIM_BUTTON_STAY_DOWN);
		m_oQuantizeLabel.SetMode(EM_SLIM_LABEL_DRAW_NORMAL);
		m_oQuantizeModeValueBox.SetMode(VALUE_BOX_DRAW_NORMAL);
		m_oSignatureLabel.SetMode(EM_SLIM_LABEL_DRAW_NORMAL);
		m_oSignatureValueBox.SetMode(VALUE_BOX_DRAW_NORMAL);
		m_oTempoLabel.SetMode(EM_SLIM_LABEL_DRAW_NORMAL);
		m_oTempoValueBox.SetMode(VALUE_BOX_DRAW_NORMAL);
		m_oTimeDisplayTwo.SetMode(TIME_DISPLAY_DRAW_NORMAL);
		m_oMeasuresLabel.SetMode(EM_SLIM_LABEL_DRAW_NORMAL);
	}
	else
	{
		m_oAutoQuantizeButton.SetMode(EM_SLIM_BUTTON_DRAW_BLANK);
		m_oQuantizeLabel.SetMode(EM_SLIM_LABEL_DRAW_BLANK);
		m_oQuantizeModeValueBox.SetMode(VALUE_BOX_DRAW_BLANK);
		m_oSignatureLabel.SetMode(EM_SLIM_LABEL_DRAW_BLANK);
		m_oSignatureValueBox.SetMode(VALUE_BOX_DRAW_BLANK);
		m_oTempoLabel.SetMode(EM_SLIM_LABEL_DRAW_BLANK);
		m_oTempoValueBox.SetMode(VALUE_BOX_DRAW_BLANK);
		m_oTimeDisplayTwo.SetMode(TIME_DISPLAY_DRAW_BLANK);
		m_oMeasuresLabel.SetMode(EM_SLIM_LABEL_DRAW_BLANK);
	}
}
/*
void TrackWindowToolBar::SetLoop()
{
//	m_oTransportLoopButton.SetOn(!m_oTransportLoopButton.GetOn());
	m_oTransportLoopButton.Draw(m_oTransportLoopButton.Frame(), m_opView);
}
*/
void TrackWindowToolBar::SetPlay()
{
	m_oTransportPlayButton.SetOn(true);
	bool vIsPainting(false);
	if(m_opView -> IsPainting())
		vIsPainting = true;
	else
		m_opView -> BeginPaint();
	m_oTransportPlayButton.Draw(m_oTransportPlayButton.Frame(), m_opView);
	if(!vIsPainting)
		m_opView -> EndPaint();
}

void TrackWindowToolBar::SetProjectBegin()
{
	m_oTransportBeginningButton.Flash();
}

void TrackWindowToolBar::SetProjectEnd()
{
	m_oTransportEndButton.Flash();
}

void TrackWindowToolBar::SetRecord()
{
	if(m_oTransportRecordButton.GetOn() == false)
		m_oTransportRecordButton.SetOn(true);
	else
		m_oTransportRecordButton.SetOn(false);
	bool vIsPainting(false);
	if(m_opView -> IsPainting())
		vIsPainting = true;
	else
		m_opView -> BeginPaint();
	m_oTransportRecordButton.Draw(m_oTransportRecordButton.Frame(), m_opView);
	if(!vIsPainting)
		m_opView -> EndPaint();
}

void TrackWindowToolBar::SetRegionBegin()
{
	m_oTransportRegionStartButton.Flash();
}

void TrackWindowToolBar::SetRegionEnd()
{
	m_oTransportRegionEndButton.Flash();
}

void TrackWindowToolBar::SetRewind()
{
;//cout_commented_out_4_release << "TrackWindowToolBar::SetRewind" << endl;
	m_oTransportRewindButton.Flash();
}

void TrackWindowToolBar::SetStop()
{
	bool vIsPainting(false);
	if(m_opView -> IsPainting())
		vIsPainting = true;
	else
		m_opView -> BeginPaint();
	if(m_oTransportRecordButton.GetOn() == true)
	{
		m_oTransportRecordButton.SetOn(false);
		m_oTransportRecordButton.Draw(m_oTransportRecordButton.Frame(), m_opView);
	}
	m_oTransportPlayButton.SetOn(false);
	m_oTransportPlayButton.Draw(m_oTransportPlayButton.Frame(), m_opView);
	if(!vIsPainting)
		m_opView -> EndPaint();
	m_oTransportStopButton.Flash();
}

