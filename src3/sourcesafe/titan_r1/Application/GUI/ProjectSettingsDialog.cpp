#include "ProjectSettingsDialog.h"

#include "CommandIDs.h"
#include "EMButton.h"
#include "EMCheckBox.h"
#include "EMComboBox.h"
#include "EMCommandRepository.h"
#include "EMFactory.h"
#include "EMGroupBox.h"
#include "EMLabel.h"
#include "EMMediaGlobals.h"
#include "EMMessages.h"
#include "EMRadioButton.h"
#include "EMSettingIDs.h"
#include "EMSettingsRepository.h"
#include "EMTabControl.h"
#include "EMTextControl.h"
#include "EMTrackbarControl.h"
#include "EMView.h"
#include "EMWindow.h"

//#include <string>

#ifdef PLATFORM_WINDOWS
#include <windows.h> // To get modal dialog to work.
#include "EMWinWindow.h" // To get modal dialog to work.
#endif

const uint32 NUMBER_OF_DECIMALS = 3; // Count the decimal-character too. I.e. 0,01 -> NUMBER_OF_DECIMALS = 3

// Local MessageIDs
const uint32 OK_BUTTON = 1;
const uint32 CANCEL_BUTTON = 2;
const uint32 DIMENSION_DEFINE_RADIO_BUTTON = 3;
const uint32 WIDTH_TEXT_CONTROL_CHANGED = 4;
const uint32 HEIGHT_TEXT_CONTROL_CHANGED = 5;
const uint32 ASPECT_RATIOS_TOGGLE = 6;
const uint32 ASPECT_RATIOS_COMBO_BOX_CHANGED = 7;
const uint32 FRAMERATE_DEFINE_RADIO_BUTTON = 8;
const uint32 FRAMERATE_TEXT_CONTROL_CHANGED = 9;
const uint32 TEXT_CONTROL_CHANGED = 10;
const uint32 TRACKBAR_CHANGED = 11;
const uint32 USE_MIDI_NOTE_CHECKBOX_CHANGED = 12;
const uint32 USE_PC_SPEAKER_CHECKBOX_CHANGED = 13;
//const uint32 USE_DURING_RADIO_BUTTON_CHANGED = 13;


ProjectSettingsDialog::ProjectSettingsDialog() :
m_opDialogWindow(EMFactory::Instance() -> CreateEMWindow(EMRect(0, 0, 326, 352), "Project Settings", EM_WINDOW_DIALOG)),
m_opMainView(EMFactory::Instance() -> CreateView(EMRect(0, 0, 326, 352), 0)),
m_opTabControl(EMFactory::Instance() -> CreateTabControl(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
																EM_METRICS_NATIVE_DIALOG_MARGIN_TOP,
																326 - EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 1,
																352 - (EM_METRICS_NATIVE_DIALOG_MARGIN_BOTTOM + EM_METRICS_NATIVE_BUTTON_HEIGHT + EM_METRICS_NATIVE_BUTTON_SPACE)),
														 "Project Tab", true, 0, 0, 0)),
m_opOK_Button(EMFactory::Instance() -> CreateButton(EMRect(326 - (EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + EM_METRICS_NATIVE_BUTTON_WIDTH * 2 + EM_METRICS_NATIVE_BUTTON_SPACE),
														   352 - (EM_METRICS_NATIVE_DIALOG_MARGIN_BOTTOM + EM_METRICS_NATIVE_BUTTON_HEIGHT),
														   326 - (EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + EM_METRICS_NATIVE_BUTTON_WIDTH + EM_METRICS_NATIVE_BUTTON_SPACE),
														   352 - (EM_METRICS_NATIVE_DIALOG_MARGIN_BOTTOM)),
													"OK", OK_BUTTON, 0, 0, NULL, true)),
m_opCancelButton(EMFactory::Instance() -> CreateButton(EMRect(326 - (EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + EM_METRICS_NATIVE_BUTTON_WIDTH),
															  352 - (EM_METRICS_NATIVE_DIALOG_MARGIN_BOTTOM + EM_METRICS_NATIVE_BUTTON_HEIGHT),
															  326 - (EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE),
															  352 - (EM_METRICS_NATIVE_DIALOG_MARGIN_BOTTOM)),
													   "Cancel", CANCEL_BUTTON, 0, 0)),
// Video Tab
m_opVideoView(EMFactory::Instance() -> CreateView(EMRect(0, 0, 0, 0), 0)), // TabControl take care of the size
m_opMetronomeView(EMFactory::Instance() -> CreateView(EMRect(0, 0, 0, 0), 0)), // TabControl take care of the size
m_vDecimalPosition(-1),
m_vSelectionStart(0),
m_vSelectionStartBeforeDecimal(false),
m_vFrameRateChanged(false)
{
	// Main View
	m_opDialogWindow -> AddChild(m_opMainView);
	m_opMainView -> AddChild(m_opTabControl);
	m_opMainView -> AddChild(m_opOK_Button);
	m_opMainView -> AddChild(m_opCancelButton);

	// Video Tab
	m_opTabControl -> AddChild(m_opVideoView);
	m_opTabControl -> AddTab("Video", m_opVideoView);
	// Dimension GroupBox
	m_opDimensionGroupBox = EMFactory::Instance() -> CreateGroupBox(EMRect(EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_SIDE,
																		   EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_TOP,
																		   m_opVideoView -> Frame().GetWidth() - EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_SIDE,
																		   EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_TOP + 94),
																	"Dimension", 0);
	m_opDimensionView = EMFactory::Instance() -> CreateView(EMRect(EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_SIDE,
																		   EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_TOP,
																		   m_opVideoView -> Frame().GetWidth() - EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_SIDE,
																		   EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_TOP + 94),
																   0);

	m_opDimensionPreDefineRadioButton = EMFactory::Instance() -> CreateRadioButton(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
																						  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 3,
																						  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 80,
																						  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 3 + EM_METRICS_NATIVE_RADIO_BUTTON_HEIGHT),
																				   "Pre-Define", DIMENSION_DEFINE_RADIO_BUTTON, 0, 0, false);
	m_opDimensionUserDefineRadioButton = EMFactory::Instance() -> CreateRadioButton(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
																						   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 27,
																						   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 80,
																						   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 27 + EM_METRICS_NATIVE_RADIO_BUTTON_HEIGHT),
																					"User-Define", DIMENSION_DEFINE_RADIO_BUTTON, 0, 0, true);
	m_opWidthLabel = EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 97,
																 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 28,
																 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 97 + 30,
																 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 28 + EM_METRICS_NATIVE_LABEL_HEIGHT),
														  "Width:", 0);
	m_opWidthTextControl = EMFactory::Instance() -> CreateTextControl(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 132,
																			 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 25,
																			 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 132 + 30,
																			 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 25 + EM_METRICS_NATIVE_TEXT_CONTROL_HEIGHT),
																	  "0", WIDTH_TEXT_CONTROL_CHANGED, 0, false, EM_ALIGNMENT_RIGHT, true);
	m_opHeightLabel = EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 177,
																  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 28,
																  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 177 + 35,
																  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 28 + EM_METRICS_NATIVE_LABEL_HEIGHT),
														   "Heigth:", 0);  
	m_opHeightTextControl = EMFactory::Instance() -> CreateTextControl(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 217,
																			  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 25,
																			  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 217 + 30,
																			  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 25 + EM_METRICS_NATIVE_TEXT_CONTROL_HEIGHT),
																	   "0", HEIGHT_TEXT_CONTROL_CHANGED, 0, false, EM_ALIGNMENT_RIGHT, true);
	m_opDimensionComboBox = EMFactory::Instance() -> CreateComboBox(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 97,
																		   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP,
																		   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 97 + 150,
																		   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 100),
																	0, 0, false);
	m_opVideoAspectRatiosCheckBox = EMFactory::Instance() -> CreateCheckBox(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
																				   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 53,
																				   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 90,
																				   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 53 + EM_METRICS_NATIVE_CHECKBOX_HEIGHT),
																			"Aspect Ratios", ASPECT_RATIOS_TOGGLE, 0, 0);
	m_opVideoAspectRatiosComboBox = EMFactory::Instance() -> CreateComboBox(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 97,
																				   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 50,
																				   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 97 + 150,
																				   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 50 + 100),
																			ASPECT_RATIOS_COMBO_BOX_CHANGED, 0, false);


	m_opVideoView -> AddChild(m_opDimensionGroupBox);
	m_opVideoView -> AddChild(m_opDimensionView);
	m_opDimensionView -> AddChild(m_opDimensionPreDefineRadioButton);
	m_opDimensionView -> AddChild(m_opDimensionUserDefineRadioButton);
	m_opDimensionView -> AddChild(m_opWidthLabel);
	m_opDimensionView -> AddChild(m_opWidthTextControl);
	m_opDimensionView -> AddChild(m_opHeightLabel);
	m_opDimensionView -> AddChild(m_opHeightTextControl);
	m_opDimensionView -> AddChild(m_opDimensionComboBox);
	m_opDimensionView -> AddChild(m_opVideoAspectRatiosCheckBox);
	m_opDimensionView -> AddChild(m_opVideoAspectRatiosComboBox);

	// Frame Rate GroupBox
	m_opFrameRateGroupBox = EMFactory::Instance() -> CreateGroupBox(EMRect(EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_SIDE,
																		   EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_TOP + m_opDimensionGroupBox -> Frame().GetHeight() + 5,
																		   m_opVideoView -> Frame().GetWidth() - EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_SIDE,
																		   EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_TOP + m_opDimensionGroupBox -> Frame().GetHeight() + 5 + 67),
																	"Frame Rate", 0);
	m_opFrameRateView = EMFactory::Instance() -> CreateView(EMRect(EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_SIDE,
																   EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_TOP + m_opDimensionGroupBox -> Frame().GetHeight() + 5,
																   m_opVideoView -> Frame().GetWidth() - EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_SIDE,
																   EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_TOP + m_opDimensionGroupBox -> Frame().GetHeight() + 5 + 67),
															0);
	m_opFrameRatePreDefineRadioButton = EMFactory::Instance() -> CreateRadioButton(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
																						  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 3,
																						  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 80,
																						  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 3 + EM_METRICS_NATIVE_RADIO_BUTTON_HEIGHT),
																				   "Pre-Define", FRAMERATE_DEFINE_RADIO_BUTTON, 0, 0, false);
	m_opFrameRateUserDefineRadioButton = EMFactory::Instance() -> CreateRadioButton(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
																						   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 27,
																						   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 80,
																						   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 27 + EM_METRICS_NATIVE_RADIO_BUTTON_HEIGHT),
																					"User-Define", FRAMERATE_DEFINE_RADIO_BUTTON, 0, 0, true);
	m_opFrameRateComboBox = EMFactory::Instance() -> CreateComboBox(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 97,
																		   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP,
																		   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 97 + 150,
																		   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 100),
																	0, 0, false);
	m_opFrameRateTextControl = EMFactory::Instance() -> CreateTextControl(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 97,
																				 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 25,
																				 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 97 + 32,
																				 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 25 + EM_METRICS_NATIVE_TEXT_CONTROL_HEIGHT),
																		  "25,00", FRAMERATE_TEXT_CONTROL_CHANGED, 0, false, EM_ALIGNMENT_RIGHT);
	
	m_opVideoView -> AddChild(m_opFrameRateGroupBox);
	m_opVideoView -> AddChild(m_opFrameRateView);
	m_opFrameRateView -> AddChild(m_opFrameRatePreDefineRadioButton);
	m_opFrameRateView -> AddChild(m_opFrameRateUserDefineRadioButton);
	m_opFrameRateView -> AddChild(m_opFrameRateComboBox);
	m_opFrameRateView -> AddChild(m_opFrameRateTextControl);

	// Background Color
	m_opBackgroundColorGroupBox = EMFactory::Instance() -> CreateGroupBox(EMRect(EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_SIDE,
																				 EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_TOP + m_opDimensionGroupBox -> Frame().GetHeight() + 5 + m_opFrameRateGroupBox -> Frame().GetHeight() + 5,
																				 m_opVideoView -> Frame().GetWidth() - EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_SIDE,
																				 EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_TOP + m_opDimensionGroupBox -> Frame().GetHeight() + 5 + m_opFrameRateGroupBox -> Frame().GetHeight() + 5 + 92),
																		  "Background Color", 0);
	m_opBackgroundColorView = EMFactory::Instance() -> CreateView(EMRect(EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_SIDE,
																		 EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_TOP + m_opDimensionGroupBox -> Frame().GetHeight() + 5 + m_opFrameRateGroupBox -> Frame().GetHeight() + 5,
																		 m_opVideoView -> Frame().GetWidth() - EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_SIDE,
																		 EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_TOP + m_opDimensionGroupBox -> Frame().GetHeight() + 5 + m_opFrameRateGroupBox -> Frame().GetHeight() + 5 + 92),
																  0);
	m_opBackgroundColorRedLabel = EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
																			  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 3,
																			  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 35,
																			  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 3 + EM_METRICS_NATIVE_LABEL_HEIGHT),
																	   "Red:", 0);
	m_opBackgroundColorGreenLabel = EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
																				EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 28,
																				EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 35,
																				EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 28 + EM_METRICS_NATIVE_LABEL_HEIGHT),
																		 "Green:", 0);
	m_opBackgroundColorBlueLabel = EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
																			   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 53,
																			   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 35,
																			   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 53 + EM_METRICS_NATIVE_LABEL_HEIGHT),
																		"Blue:", 0);
	m_opBackgroundColorRedTrackbarControl = EMFactory::Instance() -> CreateTrackbarControl(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 40,
																								  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP,
																								  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 40 + 100,
																								  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + EM_METRICS_NATIVE_TRACKBAR_CONTROL_HEIGHT),
																						   "RedTrackbar", TRACKBAR_CHANGED, 0, 0);
	m_opBackgroundColorGreenTrackbarControl = EMFactory::Instance() -> CreateTrackbarControl(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 40,
																								  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 25,
																								  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 40 + 100,
																								  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 25 + EM_METRICS_NATIVE_TRACKBAR_CONTROL_HEIGHT),
																						   "GreenTrackbar", TRACKBAR_CHANGED, 0, 0);
	m_opBackgroundColorBlueTrackbarControl = EMFactory::Instance() -> CreateTrackbarControl(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 40,
																								  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 50,
																								  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 40 + 100,
																								  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 50 + EM_METRICS_NATIVE_TRACKBAR_CONTROL_HEIGHT),
																						   "BlueTrackbar", TRACKBAR_CHANGED, 0, 0);
	m_opBackgroundColorRedTextControl = EMFactory::Instance() -> CreateTextControl(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 150,
																						  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP,
																						  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 150 + 25,
																						  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + EM_METRICS_NATIVE_TEXT_CONTROL_HEIGHT),
																				   "0", TEXT_CONTROL_CHANGED, 0, false, EM_ALIGNMENT_RIGHT, true);
	m_opBackgroundColorGreenTextControl = EMFactory::Instance() -> CreateTextControl(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 150,
																							EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 25,
																							EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 150 + 25,
																							EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 25 + EM_METRICS_NATIVE_TEXT_CONTROL_HEIGHT),
																					 "0", TEXT_CONTROL_CHANGED, 0, false, EM_ALIGNMENT_RIGHT, true);
	m_opBackgroundColorBlueTextControl = EMFactory::Instance() -> CreateTextControl(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 150,
																						   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 50,
																						   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 150 + 25,
																						   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 50 + EM_METRICS_NATIVE_TEXT_CONTROL_HEIGHT),
																					"0", TEXT_CONTROL_CHANGED, 0, false, EM_ALIGNMENT_RIGHT, true);
	m_opBackgroundColorPreviewView = EMFactory::Instance() -> CreateView(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 190,
																				EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP,
																				m_opBackgroundColorView -> Frame().GetWidth() - EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
																				m_opBackgroundColorView -> Frame().GetHeight() - EM_METRICS_NATIVE_GROUP_BOX_MARGIN_B0TTOM),
																		 0);

	m_opVideoView -> AddChild(m_opBackgroundColorGroupBox);
	m_opVideoView -> AddChild(m_opBackgroundColorView);
	m_opBackgroundColorView -> AddChild(m_opBackgroundColorRedLabel);
	m_opBackgroundColorView -> AddChild(m_opBackgroundColorGreenLabel);
	m_opBackgroundColorView -> AddChild(m_opBackgroundColorBlueLabel);
	m_opBackgroundColorView -> AddChild(m_opBackgroundColorRedTrackbarControl);
	m_opBackgroundColorView -> AddChild(m_opBackgroundColorGreenTrackbarControl);
	m_opBackgroundColorView -> AddChild(m_opBackgroundColorBlueTrackbarControl);
	m_opBackgroundColorView -> AddChild(m_opBackgroundColorRedTextControl);
	m_opBackgroundColorView -> AddChild(m_opBackgroundColorGreenTextControl);
	m_opBackgroundColorView -> AddChild(m_opBackgroundColorBlueTextControl);
	m_opBackgroundColorView -> AddChild(m_opBackgroundColorPreviewView);

// Metronome Tab
	m_opTabControl -> AddChild(m_opMetronomeView);
	m_opTabControl -> AddTab("Metronome", m_opMetronomeView);

	EMFactory* oFactory = EMFactory::Instance();

	m_opMetronomeUsePCSpeakerCheckBox = oFactory -> CreateCheckBox(EMRect(EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_SIDE,
																		  EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_TOP + 53,
																		  EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_SIDE + 100,
																		  EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_TOP + 53 + EM_METRICS_NATIVE_CHECKBOX_HEIGHT),
																   "Use PC Speaker", 0	, 0, 0);
	m_opMetronomeUseMIDINoteCheckBox = oFactory -> CreateCheckBox(EMRect(EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_SIDE,
																		  EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_TOP + 73,
																		  EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_SIDE + 100,
																		  EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_TOP + 73 + EM_METRICS_NATIVE_CHECKBOX_HEIGHT),
																   "Use MIDI Note", USE_MIDI_NOTE_CHECKBOX_CHANGED, 0, 0);
	m_opMetronomeCountInLabel = oFactory -> CreateLabel(EMRect(EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_SIDE + 130,
															   EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_SIDE + 3,
															   EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_SIDE + 130 + 45,
															   EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_TOP + 3 + EM_METRICS_NATIVE_LABEL_HEIGHT),
														"Count In:", 0);
	m_opMetronomeCountInComboBox = oFactory -> CreateComboBox(EMRect(EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_SIDE + 130 + 45 + 5,
																	 EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_TOP,
																	 EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_SIDE + 130 + 45 + 5 + 40,
																	 EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_TOP + 100),
															  0, 0, false);

	m_opMetronomeView -> AddChild(m_opMetronomeUsePCSpeakerCheckBox);
	m_opMetronomeView -> AddChild(m_opMetronomeUseMIDINoteCheckBox);
//	m_opMetronomeView -> AddChild(m_opMetronomeCountInLabel);
//	m_opMetronomeView -> AddChild(m_opMetronomeCountInComboBox);

	// Metrinome Tab -> Use During
	m_opMetronomeUseDuringGroupBox = oFactory -> CreateGroupBox(EMRect(EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_SIDE,
																	  EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_TOP,
																	  EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_SIDE + 80,
																	  EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_TOP + 65),
															  "Use During", 0);
	m_opMetronomeUseDuringView = oFactory -> CreateView(EMRect(EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_SIDE,
															  EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_TOP,
															  EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_SIDE + 80,
															  EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_TOP + 65),
													   0);
	m_opMetronomeUseDuringPlaybackRadioButton = oFactory -> CreateRadioButton(EMRect(EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_SIDE,
																					 EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_TOP,
																					 EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_SIDE + 70,
																					 EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_TOP + EM_METRICS_NATIVE_RADIO_BUTTON_HEIGHT),
																			  "Playback", 0, 0, 0, false);
	m_opMetronomeUseDuringRecordingRadioButton = oFactory -> CreateRadioButton(EMRect(EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_SIDE,
																					  EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_TOP + 16 ,
																					  EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_SIDE + 70,
																					  EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_TOP + 16 + EM_METRICS_NATIVE_RADIO_BUTTON_HEIGHT),
																			   "Recording", 0, 0, 0, false);
	m_opMetronomeUseDuringBothRadioButton = oFactory -> CreateRadioButton(EMRect(EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_SIDE,
																				 EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_TOP + 32,
																				 EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_SIDE + 70,
																				 EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_TOP + 32 + EM_METRICS_NATIVE_RADIO_BUTTON_HEIGHT),
																		  "Both", 0, 0, 0, true);

//	m_opMetronomeView -> AddChild(m_opMetronomeUseDuringGroupBox);
//	m_opMetronomeView -> AddChild(m_opMetronomeUseDuringView);
	m_opMetronomeView -> AddChild(m_opMetronomeUseDuringPlaybackRadioButton);
	m_opMetronomeView -> AddChild(m_opMetronomeUseDuringRecordingRadioButton);
	m_opMetronomeView -> AddChild(m_opMetronomeUseDuringBothRadioButton);
	
	// Metronome Tab -> Midi Note
	m_opMetronomeMidiNoteGroupBox = oFactory -> CreateGroupBox(EMRect(EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_SIDE,
																	  EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_TOP + 90,
																	  m_opMetronomeView -> Frame().GetWidth() - EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_SIDE,
																	  EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_TOP + 90 + 175),
															  "Midi Note", 0);
	m_opMetronomeMidiNoteView = oFactory -> CreateView(EMRect(EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_SIDE,
															  EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_TOP + 90,
															  m_opMetronomeView -> Frame().GetWidth() - EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_SIDE,
															  EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_TOP + 90 + 175),
													   0);
	m_opMetronomeMidiNoteDeviceLabel = oFactory -> CreateLabel(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
																	  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 3,
																	  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 45,
																	  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 3 + EM_METRICS_NATIVE_LABEL_HEIGHT),
															   "Device:", 0);
	m_opMetronomeMidiNoteDeviceComboBox = oFactory -> CreateComboBox(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 50,
																	 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP,
																	 m_opMetronomeMidiNoteView -> Frame().GetWidth() - EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
																	 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 100),
															  0, 0, false);
	m_opMetronomeMidiNoteChannelLabel = oFactory -> CreateLabel(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
																	  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 28,
																	  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 45,
																	  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 28 + EM_METRICS_NATIVE_LABEL_HEIGHT),
															   "Channel:", 0);
	m_opMetronomeMidiNoteChannelComboBox = oFactory -> CreateComboBox(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 50,
																	 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 25,
																	 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 50 + 40,
																	 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 25 + 100),
															  0, 0, false);


	m_opMetronomeView -> AddChild(m_opMetronomeMidiNoteGroupBox);
	m_opMetronomeView -> AddChild(m_opMetronomeMidiNoteView);
	m_opMetronomeMidiNoteView -> AddChild(m_opMetronomeMidiNoteDeviceLabel);
	m_opMetronomeMidiNoteView -> AddChild(m_opMetronomeMidiNoteDeviceComboBox);
	m_opMetronomeMidiNoteView -> AddChild(m_opMetronomeMidiNoteChannelLabel);
	m_opMetronomeMidiNoteView -> AddChild(m_opMetronomeMidiNoteChannelComboBox);

	// Metronome Tab -> Midi Note -> First Beat

	m_opMetronomeFirstBeatGroupBox = oFactory -> CreateGroupBox(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
																	  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 55,
																	  m_opMetronomeMidiNoteView -> Frame().GetWidth() / 2 - 3,
																	  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 55 + 95),
															  "First Beat", 0);
	m_opMetronomeFirstBeatView = oFactory -> CreateView(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
															  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 55,
															  m_opMetronomeMidiNoteView -> Frame().GetWidth() / 2 - 3,
															  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 55 + 95),
													   0);
	m_opMetronomeFirstBeatKeyLabel = oFactory -> CreateLabel(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
																	  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 3,
																	  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 45,
																	  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 3 + EM_METRICS_NATIVE_LABEL_HEIGHT),
															   "Key:", 0);
	m_opMetronomeFirstBeatKeyComboBox = oFactory -> CreateComboBox(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 70,
																	 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP,
																	 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 70 + 50,
																	 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 100),
															  0, 0, false);
	m_opMetronomeFirstBeatVelocityLabel = oFactory -> CreateLabel(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
																	  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 28,
																	  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 45,
																	  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 28 + EM_METRICS_NATIVE_LABEL_HEIGHT),
															   "Velocity:", 0);
	m_opMetronomeFirstBeatVelocityComboBox = oFactory -> CreateComboBox(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 70,
																	 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 25,
																	 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 70 + 50,
																	 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 25 + 100),
															  0, 0, false);
	m_opMetronomeFirstBeatDurationLabel = oFactory -> CreateLabel(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
																	  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 53,
																	  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 65,
																	  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 53 + EM_METRICS_NATIVE_LABEL_HEIGHT),
															   "Duration (ms):", 0);
	m_opMetronomeFirstBeatDurationComboBox = oFactory -> CreateComboBox(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 70,
																	 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 50,
																	 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 70 + 50,
																	 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 50 + 100),
															  0, 0, false);
	
	m_opMetronomeMidiNoteView -> AddChild(m_opMetronomeFirstBeatGroupBox);
	m_opMetronomeMidiNoteView -> AddChild(m_opMetronomeFirstBeatView);
	m_opMetronomeFirstBeatView -> AddChild(m_opMetronomeFirstBeatKeyLabel);
	m_opMetronomeFirstBeatView -> AddChild(m_opMetronomeFirstBeatKeyComboBox);
	m_opMetronomeFirstBeatView -> AddChild(m_opMetronomeFirstBeatVelocityLabel);
	m_opMetronomeFirstBeatView -> AddChild(m_opMetronomeFirstBeatVelocityComboBox);
	m_opMetronomeFirstBeatView -> AddChild(m_opMetronomeFirstBeatDurationLabel);
	m_opMetronomeFirstBeatView -> AddChild(m_opMetronomeFirstBeatDurationComboBox);

	// Metronome Tab -> Midi Note -> Other Beat

	m_opMetronomeOtherBeatGroupBox = oFactory -> CreateGroupBox(EMRect(m_opMetronomeMidiNoteView -> Frame().GetWidth() / 2 + 3,
																	  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 55,
																	  m_opMetronomeMidiNoteView -> Frame().GetWidth() - EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
																	  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 55 + 95),
															  "Other Beats", 0);
	m_opMetronomeOtherBeatView = oFactory -> CreateView(EMRect(m_opMetronomeMidiNoteView -> Frame().GetWidth() / 2 + 3,
															  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 55,
															  m_opMetronomeMidiNoteView -> Frame().GetWidth() - EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
															  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 55 + 95),
													   0);
	m_opMetronomeOtherBeatKeyLabel = oFactory -> CreateLabel(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
																	  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 3,
																	  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 45,
																	  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 3 + EM_METRICS_NATIVE_LABEL_HEIGHT),
															   "Key:", 0);
	m_opMetronomeOtherBeatKeyComboBox = oFactory -> CreateComboBox(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 70,
																	 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP,
																	 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 70 + 50,
																	 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 100),
															  0, 0, false);
	m_opMetronomeOtherBeatVelocityLabel = oFactory -> CreateLabel(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
																	  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 28,
																	  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 45,
																	  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 28 + EM_METRICS_NATIVE_LABEL_HEIGHT),
															   "Velocity:", 0);
	m_opMetronomeOtherBeatVelocityComboBox = oFactory -> CreateComboBox(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 70,
																	 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 25,
																	 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 70 + 50,
																	 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 25 + 100),
															  0, 0, false);
	m_opMetronomeOtherBeatDurationLabel = oFactory -> CreateLabel(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
																	  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 53,
																	  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 65,
																	  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 53 + EM_METRICS_NATIVE_LABEL_HEIGHT),
															   "Duration (ms):", 0);
	m_opMetronomeOtherBeatDurationComboBox = oFactory -> CreateComboBox(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 70,
																	 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 50,
																	 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 70 + 50,
																	 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 50 + 100),
															  0, 0, false);
	
	m_opMetronomeMidiNoteView -> AddChild(m_opMetronomeOtherBeatGroupBox);
	m_opMetronomeMidiNoteView -> AddChild(m_opMetronomeOtherBeatView);
	m_opMetronomeOtherBeatView -> AddChild(m_opMetronomeOtherBeatKeyLabel);
	m_opMetronomeOtherBeatView -> AddChild(m_opMetronomeOtherBeatKeyComboBox);
	m_opMetronomeOtherBeatView -> AddChild(m_opMetronomeOtherBeatVelocityLabel);
	m_opMetronomeOtherBeatView -> AddChild(m_opMetronomeOtherBeatVelocityComboBox);
	m_opMetronomeOtherBeatView -> AddChild(m_opMetronomeOtherBeatDurationLabel);
	m_opMetronomeOtherBeatView -> AddChild(m_opMetronomeOtherBeatDurationComboBox);

}

ProjectSettingsDialog::~ProjectSettingsDialog()
{
	// Delete topmost first
	// Video Tab - Dimension
	delete m_opDimensionPreDefineRadioButton;
	delete m_opDimensionUserDefineRadioButton;
	delete m_opWidthLabel;
	delete m_opWidthTextControl;
	delete m_opHeightLabel;
	delete m_opHeightTextControl;
	delete m_opDimensionComboBox;
	delete m_opVideoAspectRatiosCheckBox;
	delete m_opVideoAspectRatiosComboBox;
	// Video Tab - FrameRate
	delete m_opFrameRatePreDefineRadioButton;
	delete m_opFrameRateUserDefineRadioButton;
	delete m_opFrameRateComboBox;
	delete m_opFrameRateTextControl;
	// Video Tab - Background Color
	delete m_opBackgroundColorGroupBox;
	delete m_opBackgroundColorView;
	delete m_opBackgroundColorRedLabel;
	delete m_opBackgroundColorGreenLabel;
	delete m_opBackgroundColorBlueLabel;
	delete m_opBackgroundColorRedTrackbarControl;
	delete m_opBackgroundColorGreenTrackbarControl;
	delete m_opBackgroundColorBlueTrackbarControl;
	delete m_opBackgroundColorRedTextControl;
	delete m_opBackgroundColorGreenTextControl;
	delete m_opBackgroundColorBlueTextControl;
	delete m_opBackgroundColorPreviewView;
	// Video Tab
	delete m_opVideoView;

	// Metronome Tab
	delete m_opMetronomeUsePCSpeakerCheckBox;
	delete m_opMetronomeUseMIDINoteCheckBox;
	delete m_opMetronomeCountInLabel;

	delete m_opMetronomeUseDuringPlaybackRadioButton;
	delete m_opMetronomeUseDuringRecordingRadioButton;
	delete m_opMetronomeUseDuringBothRadioButton;
	delete m_opMetronomeUseDuringGroupBox;
	delete m_opMetronomeUseDuringView;

	delete m_opMetronomeMidiNoteDeviceLabel;
	delete m_opMetronomeMidiNoteDeviceComboBox;
	delete m_opMetronomeMidiNoteChannelLabel;
	delete m_opMetronomeMidiNoteChannelComboBox;
	delete m_opMetronomeMidiNoteGroupBox;
	delete m_opMetronomeMidiNoteView;

	delete m_opMetronomeFirstBeatKeyLabel;
	delete m_opMetronomeFirstBeatKeyComboBox;
	delete m_opMetronomeFirstBeatVelocityLabel;
	delete m_opMetronomeFirstBeatVelocityComboBox;
	delete m_opMetronomeFirstBeatDurationLabel;
	delete m_opMetronomeFirstBeatDurationComboBox;
	delete m_opMetronomeFirstBeatGroupBox;
	delete m_opMetronomeFirstBeatView;

	delete m_opMetronomeOtherBeatKeyLabel;
	delete m_opMetronomeOtherBeatKeyComboBox;
	delete m_opMetronomeOtherBeatVelocityLabel;
	delete m_opMetronomeOtherBeatVelocityComboBox;
	delete m_opMetronomeOtherBeatDurationLabel;
	delete m_opMetronomeOtherBeatDurationComboBox;
	delete m_opMetronomeOtherBeatGroupBox;
	delete m_opMetronomeOtherBeatView;

	delete m_opMetronomeView;

	// Main View
	delete m_opDialogWindow;
	delete m_opMainView;
	delete m_opTabControl;
	delete m_opOK_Button;
	delete m_opCancelButton;
}

void ProjectSettingsDialog::DisplayDialog()
{
	char vTemp[40];
	EMSettingsRepository* oSettings = EMSettingsRepository::Instance();

	// Video Dimension Settings
	if(*static_cast<bool*>(oSettings -> GetSetting(SETTING_USE_PREDEFINED_VIDEO_FORMAT)))
	{
		m_opDimensionPreDefineRadioButton -> Check(true);
		m_opDimensionUserDefineRadioButton -> Check(false);
	}
	else
	{
		m_opDimensionPreDefineRadioButton -> Check(false);
		m_opDimensionUserDefineRadioButton -> Check(true);
	}
	m_opDimensionPreDefineRadioButton -> Notify(DIMENSION_DEFINE_RADIO_BUTTON);
	
	int vWidth(*static_cast<int*>(oSettings -> GetSetting(SETTING_VIDEO_FORMAT_WIDTH)));
	int vHeight(*static_cast<int*>(oSettings -> GetSetting(SETTING_VIDEO_FORMAT_HEIGHT)));
	ResolutionPair oDimension(vWidth, vHeight);
	bool vNotSet(true);
	map<int, ResolutionPair>::iterator oIterator = m_oResolutionMap.begin();
	while(oIterator != m_oResolutionMap.end())
	{
		if((*oIterator).second == oDimension/*(*oIterator).second.first == oDimension.first && (*oIterator).second.second == oDimension.second*/)
		{
			m_opDimensionComboBox -> SelectItem((*oIterator).first);
			vNotSet = false;
			break;
		}
		oIterator++;
	}
	if(vNotSet)
		m_opDimensionComboBox -> SelectItem(0);
	m_opWidthTextControl -> SetText(itoa(vWidth, vTemp, 10));
	m_opHeightTextControl -> SetText(itoa(vHeight, vTemp, 10));

	if(*static_cast<bool*>(oSettings -> GetSetting(SETTING_USE_ASPECT_RATIOS)))
		m_opVideoAspectRatiosCheckBox -> Check(true);
	else
		m_opVideoAspectRatiosCheckBox -> Check(false);
	m_opVideoAspectRatiosComboBox -> SelectItem(*static_cast<int*>(oSettings -> GetSetting(SETTING_ASPECT_RATIOS)));

	// Video Framerate Settings
	if(*static_cast<bool*>(oSettings -> GetSetting(SETTING_USE_PREDEFINED_VIDEO_FRAMERATE)))
	{
		m_opFrameRatePreDefineRadioButton -> Check(true);
		m_opFrameRateUserDefineRadioButton -> Check(false);
	}
	else
	{
		m_opFrameRatePreDefineRadioButton -> Check(false);
		m_opFrameRateUserDefineRadioButton -> Check(true);
	}
	m_opFrameRatePreDefineRadioButton -> Notify(FRAMERATE_DEFINE_RADIO_BUTTON);

	vNotSet = true;
	float vFramerate(*static_cast<float*>(oSettings -> GetSetting(SETTING_VIDEO_FRAMERATE)));
	map<int, float>::iterator oFramerateIterator = m_oFrameRateMap.begin();
	while(oFramerateIterator != m_oFrameRateMap.end())
	{
		if((*oFramerateIterator).second == vFramerate)
		{
			m_opFrameRateComboBox -> SelectItem((*oFramerateIterator).first);
			vNotSet = false;
			break;
		}
		oFramerateIterator++;
	}
	if(vNotSet)
		m_opFrameRateComboBox -> SelectItem(0);
	double vDoubleFramerate(vFramerate + 0.005f);
	sprintf(vTemp, "%f\n", vDoubleFramerate);
	string oTemp = vTemp;
	oTemp.replace(oTemp.find_first_of("."), 1, ",");
	oTemp.erase(oTemp.find_first_of(",") + NUMBER_OF_DECIMALS);
	m_opFrameRateTextControl -> SetText(oTemp.c_str());
	m_vFrameRateLength = oTemp.size();
	m_vDecimalPosition = -1;

	int32 vColor = *static_cast<int32*>(oSettings -> GetSetting(SETTING_VIDEO_BACKGROUND_COLOR));
//	m_opBackgroundColorRedTextControl   -> SetText(itoa((vColor & 0x00FF0000) >> 16, vTemp, 10));
//	m_opBackgroundColorGreenTextControl -> SetText(itoa((vColor & 0x0000FF00) >> 8, vTemp, 10));
//	m_opBackgroundColorBlueTextControl  -> SetText(itoa((vColor & 0x000000FF), vTemp, 10));
	// Will set the value in the text controls too.
	m_opBackgroundColorRedTrackbarControl   -> SetPosition((vColor & 0x00FF0000) >> 16);
	m_opBackgroundColorGreenTrackbarControl -> SetPosition((vColor & 0x0000FF00) >> 8);
	m_opBackgroundColorBlueTrackbarControl  -> SetPosition((vColor & 0x000000FF));

	// Metronome Settings
	bool vDuringPlay(*static_cast<bool*>(oSettings -> GetSetting(SETTING_MIDI_METRONOME_DURING_PLAYBACK)));
	bool vDuringRec(*static_cast<bool*>(oSettings -> GetSetting(SETTING_MIDI_METRONOME_DURING_RECORDING)));
	if(vDuringPlay == true && vDuringRec == false)
	{
		m_opMetronomeUseDuringPlaybackRadioButton -> Check(true);
		m_opMetronomeUseDuringRecordingRadioButton -> Check(false);
		m_opMetronomeUseDuringBothRadioButton -> Check(false);
	}
	else if(vDuringPlay == false && vDuringRec == true)
	{
		m_opMetronomeUseDuringPlaybackRadioButton -> Check(false);
		m_opMetronomeUseDuringRecordingRadioButton -> Check(true);
		m_opMetronomeUseDuringBothRadioButton -> Check(false);
	}
	else if(vDuringPlay == true && vDuringRec == true)
	{
		m_opMetronomeUseDuringPlaybackRadioButton -> Check(false);
		m_opMetronomeUseDuringRecordingRadioButton -> Check(false);
		m_opMetronomeUseDuringBothRadioButton -> Check(true);
	}
	m_opMetronomeUsePCSpeakerCheckBox -> Check(*static_cast<bool*>(oSettings -> GetSetting(SETTING_MIDI_METRONOME_USE_PC_SPEAKER)));
	m_opMetronomeUseMIDINoteCheckBox -> Check(*static_cast<bool*>(oSettings -> GetSetting(SETTING_MIDI_METRONOME_USE_MIDI_NOTE)));
	m_opMetronomeUseMIDINoteCheckBox -> Notify(USE_MIDI_NOTE_CHECKBOX_CHANGED);
	m_opMetronomeCountInComboBox -> SelectItem(*static_cast<int32*>(oSettings -> GetSetting(SETTING_MIDI_METRONOME_COUNT_IN)));
	string oMidiDevice = static_cast<string*>(oSettings -> GetSetting(SETTING_MIDI_METRONOME_MIDI_DEVICE)) -> c_str();
	if(oMidiDevice.size() != 0)
		m_opMetronomeMidiNoteDeviceComboBox -> SelectItem(m_oMidiDeviceMap[oMidiDevice.c_str()]);
	m_opMetronomeMidiNoteChannelComboBox -> SelectItem(*static_cast<int32*>(oSettings -> GetSetting(SETTING_MIDI_METRONOME_MIDI_CHANNEL)));
	m_opMetronomeFirstBeatKeyComboBox -> SelectItem(*static_cast<int32*>(oSettings -> GetSetting(SETTING_MIDI_METRONOME_MIDI_NOTE_HIGH_KEY)));
	m_opMetronomeFirstBeatVelocityComboBox -> SelectItem(*static_cast<int32*>(oSettings -> GetSetting(SETTING_MIDI_METRONOME_MIDI_NOTE_HIGH_VELOCITY)));
	m_opMetronomeFirstBeatDurationComboBox -> SelectItem(*static_cast<int32*>(oSettings -> GetSetting(SETTING_MIDI_METRONOME_MIDI_NOTE_HIGH_DURATION)));
	m_opMetronomeOtherBeatKeyComboBox -> SelectItem(*static_cast<int32*>(oSettings -> GetSetting(SETTING_MIDI_METRONOME_MIDI_NOTE_LOW_KEY)));
	m_opMetronomeOtherBeatVelocityComboBox -> SelectItem(*static_cast<int32*>(oSettings -> GetSetting(SETTING_MIDI_METRONOME_MIDI_NOTE_LOW_VELOCITY)));
	m_opMetronomeOtherBeatDurationComboBox -> SelectItem(*static_cast<int32*>(oSettings -> GetSetting(SETTING_MIDI_METRONOME_MIDI_NOTE_LOW_DURATION)));


	if(m_opDialogWindow -> Lock())
	{
		m_opBackgroundColorPreviewView -> SetViewColor(EMColor(atoi(m_opBackgroundColorRedTextControl -> Text()), atoi(m_opBackgroundColorGreenTextControl -> Text()), atoi(m_opBackgroundColorBlueTextControl -> Text()), 255));
		m_opDialogWindow -> Show();
#ifdef PLATFORM_WINDOWS
		EnableWindow(dynamic_cast<EMWinWindow*>(m_opDialogWindow) -> GetMainWindowHandle(), FALSE);
		SetFocus(static_cast<HWND>(m_opTabControl -> GetNativeView()));
#endif
		m_opDialogWindow -> Unlock();
	}
}

void ProjectSettingsDialog::EnableMidiNote(bool p_vEnable)
{
	m_opMetronomeMidiNoteGroupBox -> Enable(p_vEnable);
	m_opMetronomeMidiNoteDeviceLabel -> Enable(p_vEnable);
	m_opMetronomeMidiNoteDeviceComboBox -> Enable(p_vEnable);
	m_opMetronomeMidiNoteChannelLabel -> Enable(p_vEnable);
	m_opMetronomeMidiNoteChannelComboBox -> Enable(p_vEnable);
	m_opMetronomeFirstBeatGroupBox -> Enable(p_vEnable);
	m_opMetronomeFirstBeatKeyLabel -> Enable(p_vEnable);
	m_opMetronomeFirstBeatKeyComboBox -> Enable(p_vEnable);
	m_opMetronomeFirstBeatVelocityLabel -> Enable(p_vEnable);
	m_opMetronomeFirstBeatVelocityComboBox -> Enable(p_vEnable);
	m_opMetronomeFirstBeatDurationLabel -> Enable(p_vEnable);
	m_opMetronomeFirstBeatDurationComboBox -> Enable(p_vEnable);
	m_opMetronomeOtherBeatGroupBox -> Enable(p_vEnable);
	m_opMetronomeOtherBeatKeyLabel -> Enable(p_vEnable);
	m_opMetronomeOtherBeatKeyComboBox -> Enable(p_vEnable);
	m_opMetronomeOtherBeatVelocityLabel -> Enable(p_vEnable);
	m_opMetronomeOtherBeatVelocityComboBox -> Enable(p_vEnable);
	m_opMetronomeOtherBeatDurationLabel -> Enable(p_vEnable);
	m_opMetronomeOtherBeatDurationComboBox -> Enable(p_vEnable);

}

void ProjectSettingsDialog::HideDialog()
{
	if(m_opDialogWindow -> Lock())
	{
#ifdef PLATFORM_WINDOWS
		EnableWindow(dynamic_cast<EMWinWindow*>(m_opDialogWindow) -> GetMainWindowHandle(), TRUE);
		SetFocus(dynamic_cast<EMWinWindow*>(m_opDialogWindow) -> GetMainWindowHandle());
#endif
		m_opDialogWindow -> Hide();
		m_opDialogWindow -> Unlock();
	}
}

void ProjectSettingsDialog::Init()
{
	m_opDialogWindow -> AddListener(this);
	m_opMainView -> AddListener(this);
	m_opMetronomeView -> AddListener(this);
	m_opMetronomeMidiNoteView -> AddListener(this);
	m_opMetronomeFirstBeatView -> AddListener(this);
	m_opMetronomeOtherBeatView -> AddListener(this);
	m_opMetronomeUseDuringView -> AddListener(this);
	if(m_opDialogWindow -> Lock())
	{
		//m_opMetronomeView -> SetViewColor(EMColor(0, 255, 0, 255));
		m_opMetronomeView -> SetDefaultColor();
		//m_opMetronomeMidiNoteView -> SetViewColor(EMColor(255, 0, 0, 255));
		m_opMetronomeMidiNoteView -> SetDefaultColor();
		//m_opMetronomeFirstBeatView -> SetViewColor(EMColor(0, 255, 0, 255));
		m_opMetronomeFirstBeatView -> SetDefaultColor();
		//m_opMetronomeOtherBeatView -> SetViewColor(EMColor(0, 0, 255, 255));
		m_opMetronomeOtherBeatView -> SetDefaultColor();
		//m_opMetronomeUseDuringView -> SetViewColor(EMColor(0, 195, 0, 255));
		m_opMetronomeUseDuringView -> SetDefaultColor();
		m_opMainView -> SetDefaultColor();
		m_opDialogWindow -> Unlock();
	}
	m_opTabControl -> AddListener(this);
	m_opOK_Button -> AddListener(this);
	m_opCancelButton -> AddListener(this);

// Video Tab
	m_opDimensionPreDefineRadioButton -> AddListener(this);
	m_opDimensionUserDefineRadioButton -> AddListener(this);
	m_opVideoAspectRatiosCheckBox -> AddListener(this);
	m_opDimensionComboBox -> AddListener(this);
	m_opWidthTextControl -> AddListener(this);
	m_opHeightTextControl -> AddListener(this);
	m_opVideoAspectRatiosComboBox -> AddListener(this);
	m_opFrameRatePreDefineRadioButton -> AddListener(this);
	m_opFrameRateUserDefineRadioButton -> AddListener(this);
	m_opFrameRateComboBox -> AddListener(this);
	m_opFrameRateTextControl -> AddListener(this);
	m_opBackgroundColorRedTrackbarControl -> AddListener(this);
	m_opBackgroundColorGreenTrackbarControl -> AddListener(this);
	m_opBackgroundColorBlueTrackbarControl -> AddListener(this);
	m_opBackgroundColorRedTextControl -> AddListener(this);
	m_opBackgroundColorGreenTextControl -> AddListener(this);
	m_opBackgroundColorBlueTextControl -> AddListener(this);
	m_opBackgroundColorPreviewView -> AddListener(this);


	m_opDimensionComboBox -> AddString("DV-PAL (720x576)");
	m_oResolutionMap[0] = ResolutionPair(720, 576);
	m_opDimensionComboBox -> AddString("DV-NTSC (720x480)");
	m_oResolutionMap[1] = ResolutionPair(720, 480);
	m_opDimensionComboBox -> AddString("MPEG-PAL (352x288)");
	m_oResolutionMap[2] = ResolutionPair(352, 288);
	m_opDimensionComboBox -> AddString("MPEG-NTSC (320x240)");
	m_oResolutionMap[3] = ResolutionPair(320, 240);
	m_opDimensionComboBox -> AddString("VideoCD (352x240)");
	m_oResolutionMap[4] = ResolutionPair(352, 240);
	m_opDimensionComboBox -> SelectItem(0);

	m_opVideoAspectRatiosComboBox -> AddString("[4:3] European & Asian theatre");
	m_oAspectRatiosMap[0] = ResolutionPair(4, 3);
	m_opVideoAspectRatiosComboBox -> AddString("[16:9] North American theatre");
	m_oAspectRatiosMap[1] = ResolutionPair(16, 9);
	m_opVideoAspectRatiosComboBox -> AddString("[12:5] Worldwide WideScreen");
	m_oAspectRatiosMap[2] = ResolutionPair(12, 5);
	m_opVideoAspectRatiosComboBox -> SelectItem(0);

	m_opFrameRateComboBox -> AddString("29,97 (TV-NTSC)");
	m_oFrameRateMap[0] = 29.97f;
	m_opFrameRateComboBox -> AddString("25 (TV-PAL)");
	m_oFrameRateMap[1] = 25.0f;
	m_opFrameRateComboBox -> AddString("24 (FILM)");
	m_oFrameRateMap[2] = 24.0f;
	m_opFrameRateComboBox -> AddString("15 (Cinepack)");
	m_oFrameRateMap[3] = 15.0f;
	m_opFrameRateComboBox -> SelectItem(0);

	m_opBackgroundColorRedTrackbarControl -> SetRange(0, 255);
	m_opBackgroundColorGreenTrackbarControl -> SetRange(0, 255);
	m_opBackgroundColorBlueTrackbarControl -> SetRange(0, 255);

	// Metronome
	m_opMetronomeUsePCSpeakerCheckBox -> AddListener(this);
	m_opMetronomeUseMIDINoteCheckBox -> AddListener(this);
	m_opMetronomeCountInComboBox -> AddListener(this);
	m_opMetronomeMidiNoteDeviceComboBox -> AddListener(this);
	m_opMetronomeMidiNoteChannelComboBox -> AddListener(this);
	m_opMetronomeFirstBeatKeyComboBox -> AddListener(this);
	m_opMetronomeFirstBeatVelocityComboBox -> AddListener(this);
	m_opMetronomeFirstBeatDurationComboBox -> AddListener(this);
	m_opMetronomeOtherBeatKeyComboBox -> AddListener(this);
	m_opMetronomeOtherBeatVelocityComboBox -> AddListener(this);
	m_opMetronomeOtherBeatDurationComboBox -> AddListener(this);

	SetNumberInComboBox(m_opMetronomeCountInComboBox, 33);
	m_opMetronomeCountInComboBox -> SelectItem(0);

	EMMediaType eMidiType = EM_TYPE_MIDI;
	list<int32>* opMidiOutputIDs = static_cast<list<int32>*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_OUTPUTS, &eMidiType));
	if(opMidiOutputIDs -> size() == 0)
		m_opMetronomeMidiNoteDeviceComboBox -> AddString("No Midi Output found");
	else
	{
		int vCounter(0);
		for(list<int32>::iterator oMidiOutputIterator = opMidiOutputIDs -> begin(); oMidiOutputIterator != opMidiOutputIDs -> end(); oMidiOutputIterator++, vCounter++)
		{
			m_opMetronomeMidiNoteDeviceComboBox -> AddString(static_cast<const char*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_OUTPUT_NAME, &(*oMidiOutputIterator))));
			m_oMidiDeviceMap[static_cast<const char*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_OUTPUT_NAME, &(*oMidiOutputIterator)))] = vCounter;
		}
	}

	SetNumberInComboBox(m_opMetronomeMidiNoteChannelComboBox, 16, 1);
	m_opMetronomeMidiNoteChannelComboBox -> SelectItem(0);

	SetMidiKeyInComboBox(m_opMetronomeFirstBeatKeyComboBox);
	m_opMetronomeFirstBeatKeyComboBox -> SelectItem(0);
	SetNumberInComboBox(m_opMetronomeFirstBeatVelocityComboBox, 128);
	m_opMetronomeFirstBeatVelocityComboBox -> SelectItem(0);
	SetNumberInComboBox(m_opMetronomeFirstBeatDurationComboBox, 1000, 1);
	m_opMetronomeFirstBeatDurationComboBox -> SelectItem(0);

	SetMidiKeyInComboBox(m_opMetronomeOtherBeatKeyComboBox);
	m_opMetronomeOtherBeatKeyComboBox -> SelectItem(0);
	SetNumberInComboBox(m_opMetronomeOtherBeatVelocityComboBox, 128);
	m_opMetronomeOtherBeatVelocityComboBox -> SelectItem(0);
	SetNumberInComboBox(m_opMetronomeOtherBeatDurationComboBox, 1000, 1);
	m_opMetronomeOtherBeatDurationComboBox -> SelectItem(0);

// Setting Tab order
	// TabControl
	m_opDialogWindow -> AddTabComponent(m_opTabControl);
	// Video Tab
	m_opDialogWindow -> AddTabComponent(m_opDimensionPreDefineRadioButton);
	m_opDialogWindow -> AddTabComponent(m_opDimensionUserDefineRadioButton);
	m_opDialogWindow -> AddTabComponent(m_opVideoAspectRatiosCheckBox);
	m_opDialogWindow -> AddTabComponent(m_opDimensionComboBox);
	m_opDialogWindow -> AddTabComponent(m_opWidthTextControl);
	m_opDialogWindow -> AddTabComponent(m_opHeightTextControl);
	m_opDialogWindow -> AddTabComponent(m_opVideoAspectRatiosComboBox);
	m_opDialogWindow -> AddTabComponent(m_opFrameRatePreDefineRadioButton);
	m_opDialogWindow -> AddTabComponent(m_opFrameRateUserDefineRadioButton);
	m_opDialogWindow -> AddTabComponent(m_opFrameRateComboBox);
	m_opDialogWindow -> AddTabComponent(m_opFrameRateTextControl);
	m_opDialogWindow -> AddTabComponent(m_opBackgroundColorRedTrackbarControl);
	m_opDialogWindow -> AddTabComponent(m_opBackgroundColorGreenTrackbarControl);
	m_opDialogWindow -> AddTabComponent(m_opBackgroundColorBlueTrackbarControl);
	m_opDialogWindow -> AddTabComponent(m_opBackgroundColorRedTextControl);
	m_opDialogWindow -> AddTabComponent(m_opBackgroundColorGreenTextControl);
	m_opDialogWindow -> AddTabComponent(m_opBackgroundColorBlueTextControl);
	// Metronome
	m_opDialogWindow -> AddTabComponent(m_opMetronomeUsePCSpeakerCheckBox);
	m_opDialogWindow -> AddTabComponent(m_opMetronomeUseMIDINoteCheckBox);
	m_opDialogWindow -> AddTabComponent(m_opMetronomeCountInComboBox);
	m_opDialogWindow -> AddTabComponent(m_opMetronomeMidiNoteDeviceComboBox);
	m_opDialogWindow -> AddTabComponent(m_opMetronomeMidiNoteChannelComboBox);
	m_opDialogWindow -> AddTabComponent(m_opMetronomeFirstBeatKeyComboBox);
	m_opDialogWindow -> AddTabComponent(m_opMetronomeFirstBeatVelocityComboBox);
	m_opDialogWindow -> AddTabComponent(m_opMetronomeFirstBeatDurationComboBox);
	m_opDialogWindow -> AddTabComponent(m_opMetronomeOtherBeatKeyComboBox);
	m_opDialogWindow -> AddTabComponent(m_opMetronomeOtherBeatVelocityComboBox);
	m_opDialogWindow -> AddTabComponent(m_opMetronomeOtherBeatDurationComboBox);
	// Main View
	m_opDialogWindow -> AddTabComponent(m_opOK_Button);
	m_opDialogWindow -> AddTabComponent(m_opCancelButton);

}

bool ProjectSettingsDialog::MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage)
{
	switch(p_vMessage)
	{
	case EM_GUI_COMPONENT_ENTER_PRESSED:
	case OK_BUTTON:
		eo << "EM_BUTTON_OK" << ef;
		SaveSettings();
	case EM_GUI_COMPONENT_ESC_PRESSED:
	case CANCEL_BUTTON:
		eo << "EM_BUTTON_CANCEL" << ef;
		HideDialog();
		return true;
	case DIMENSION_DEFINE_RADIO_BUTTON:
		eo << "Pre-Define checkbox checked/unchecked" << ef;
		if(m_opDimensionPreDefineRadioButton -> IsChecked())
		{
			eo << "m_opPreDefineRadioButton -> IsChecked()" << ef;
//			if(m_opVideoGroupBox -> IsEnabled())
				m_opDimensionComboBox -> Enable(true);
/*			else
				m_opDimensionComboBox -> Enable(false);
*/			m_opWidthLabel -> Enable(false);
			m_opWidthTextControl -> Enable(false);
			m_opHeightLabel -> Enable(false);
			m_opHeightTextControl -> Enable(false);
			m_opVideoAspectRatiosCheckBox -> Enable(false);
			m_opVideoAspectRatiosComboBox -> Enable(false);
		}
		else if(m_opDimensionUserDefineRadioButton -> IsChecked())
		{
			eo << "m_opUserDefineRadioButton -> IsChecked()" << ef;
			m_opDimensionComboBox -> Enable(false);
//			if(m_opVideoGroupBox -> IsEnabled())
//			{
				m_opWidthLabel -> Enable(true);
				m_opWidthTextControl -> Enable(true);
				m_opHeightLabel -> Enable(true);
				m_opHeightTextControl -> Enable(true);
				m_opVideoAspectRatiosCheckBox -> Enable(true);
				m_opVideoAspectRatiosCheckBox -> Notify(ASPECT_RATIOS_TOGGLE);
/*			}
			else
			{
				m_opWidthLabel -> Enable(false);
				m_opWidthTextControl -> Enable(false);
				m_opHeightLabel -> Enable(false);
				m_opHeightTextControl -> Enable(false);
				m_opVideoAspectRatiosCheckBox -> Enable(false);
				m_opVideoAspectRatiosComboBox -> Enable(false);
			}
*/		}
		return true;
	case ASPECT_RATIOS_TOGGLE:
		eo << "ASPECT_RATIOS_TOGGLE" << ef;
		if(m_opVideoAspectRatiosCheckBox -> IsChecked())
		{
			m_vAspectRatiosIsChecked = true;
//			if(m_opVideoGroupBox -> IsEnabled())
//			{
				m_opVideoAspectRatiosComboBox -> Enable(true);
				m_opVideoAspectRatiosComboBox -> Notify(ASPECT_RATIOS_COMBO_BOX_CHANGED);
/*			}
			else
				m_opVideoAspectRatiosComboBox -> Enable(false);
*/		}
		else
		{
			m_vAspectRatiosIsChecked = false;
			m_opVideoAspectRatiosComboBox -> Enable(false);
		}
		return true;
	case ASPECT_RATIOS_COMBO_BOX_CHANGED:
		eo << "ASPECT_RATIOS_COMBO_BOX_CHANGED" << ef;
		if(m_vAspectRatiosIsChecked)
		{
			uint16 vWidth(atoi(m_opWidthTextControl -> Text()));
			uint16 vHeight(static_cast<double>(vWidth * (static_cast<double>(m_oAspectRatiosMap[m_opVideoAspectRatiosComboBox -> GetSelectionIndex()].second) / static_cast<double>(m_oAspectRatiosMap[m_opVideoAspectRatiosComboBox -> GetSelectionIndex()].first))) + 0.5f);
			eo << "Aspect Ratios = " << (static_cast<double>(m_oAspectRatiosMap[m_opVideoAspectRatiosComboBox -> GetSelectionIndex()].second) / static_cast<double>(m_oAspectRatiosMap[m_opVideoAspectRatiosComboBox -> GetSelectionIndex()].first)) << ef;
			eo << "Width Aspect Ratios = " << static_cast<double>(vWidth * (static_cast<double>(m_oAspectRatiosMap[m_opVideoAspectRatiosComboBox -> GetSelectionIndex()].second) / static_cast<double>(m_oAspectRatiosMap[m_opVideoAspectRatiosComboBox -> GetSelectionIndex()].first))) + 0.5f << ef;
			char vTemp[5];
			m_opHeightTextControl -> SetText(itoa(vHeight, vTemp, 10));
		}
		return true;
	case WIDTH_TEXT_CONTROL_CHANGED:
		if(m_vAspectRatiosIsChecked && m_vHeightChanged == false)
		{
			m_vWidthChanged = true;
			m_opVideoAspectRatiosComboBox -> Notify(ASPECT_RATIOS_COMBO_BOX_CHANGED);
		}
		else
			m_vHeightChanged = false;
		return true;
	case HEIGHT_TEXT_CONTROL_CHANGED:
		if(m_vAspectRatiosIsChecked && m_vWidthChanged == false)
		{
			m_vHeightChanged = true;
			uint16 vHeight(atoi(m_opHeightTextControl -> Text()));
			uint16 vWidth(static_cast<double>(vHeight * (static_cast<double>(m_oAspectRatiosMap[m_opVideoAspectRatiosComboBox -> GetSelectionIndex()].first) / static_cast<double>(m_oAspectRatiosMap[m_opVideoAspectRatiosComboBox -> GetSelectionIndex()].second))) + 0.5f);
			eo << "Aspect Ratios = " << (static_cast<double>(m_oAspectRatiosMap[m_opVideoAspectRatiosComboBox -> GetSelectionIndex()].first) / static_cast<double>(m_oAspectRatiosMap[m_opVideoAspectRatiosComboBox -> GetSelectionIndex()].second)) << ef;
			eo << "Height Aspect Ratios = " << static_cast<double>(vHeight * (static_cast<double>(m_oAspectRatiosMap[m_opVideoAspectRatiosComboBox -> GetSelectionIndex()].first) / static_cast<double>(m_oAspectRatiosMap[m_opVideoAspectRatiosComboBox -> GetSelectionIndex()].second))) + 0.5f << ef;
			char vTemp[5];
			m_opWidthTextControl -> SetText(itoa(vWidth, vTemp, 10));
		}
		else
			m_vWidthChanged = false;
		return true;
	case FRAMERATE_DEFINE_RADIO_BUTTON:
		eo << "Pre-Define checkbox checked/unchecked" << ef;
		if(m_opFrameRatePreDefineRadioButton -> IsChecked())
		{
			eo << "m_opPreDefineRadioButton -> IsChecked()" << ef;
//			if(m_opVideoGroupBox -> IsEnabled())
				m_opFrameRateComboBox -> Enable(true);
//			else
//				m_opFrameRateComboBox -> Enable(false);
			m_opFrameRateTextControl -> Enable(false);
		}
		else if(m_opFrameRateUserDefineRadioButton -> IsChecked())
		{
			eo << "m_opUserDefineRadioButton -> IsChecked()" << ef;
			m_opFrameRateComboBox -> Enable(false);
//			if(m_opVideoGroupBox -> IsEnabled())
				m_opFrameRateTextControl -> Enable(true);
//			else
//				m_opFrameRateTextControl -> Enable(false);
		}
		return true;
	case EM_TEXT_CONTROL_LEFT_MOUSE_BUTTON_DOWN:
	case EM_TEXT_CONTROL_UP_ARROW_PRESSED:
	case EM_TEXT_CONTROL_DOWN_ARROW_PRESSED:
	case EM_TEXT_CONTROL_LEFT_ARROW_PRESSED:
	case EM_TEXT_CONTROL_RIGHT_ARROW_PRESSED:
		if(p_opSender == m_opFrameRateTextControl)
		{
			eo << "EM_TEXT_CONTROL_RIGHT_ARROW_PRESSED" << ef;
			int32 vSelectionStart(0);
			int32 vSelectionEnd(0);
			m_opFrameRateTextControl -> GetSelection(vSelectionStart, vSelectionEnd);
			switch(p_vMessage)
			{
			case EM_TEXT_CONTROL_UP_ARROW_PRESSED:
			case EM_TEXT_CONTROL_LEFT_ARROW_PRESSED:
				if(vSelectionStart != 0)
					vSelectionStart--;
				break;
			case EM_TEXT_CONTROL_DOWN_ARROW_PRESSED:
			case EM_TEXT_CONTROL_RIGHT_ARROW_PRESSED:
				if(vSelectionStart != strlen(m_opFrameRateTextControl -> Text()))
					vSelectionStart++;
				break;
			default:
				break;
			}
			eo << "vSelectionStart = " << vSelectionStart << ef;
			eo << "m_vDecimalPosition = " << m_vDecimalPosition << ef;
			if(vSelectionStart > m_vDecimalPosition)
				m_vSelectionStartBeforeDecimal = false;
			else
				m_vSelectionStartBeforeDecimal = true;
		}
		return false; // We want the control to move the "TextCursor" so we return false. If we return true, we have to move the "TextCursor".
	case FRAMERATE_TEXT_CONTROL_CHANGED:
		eo << "FRAMERATE_TEXT_CONTROL_CHANGED" << ef;
		if(m_vFrameRateChanged == false)
		{
			//m_vFrameRateChanged = true;
			string oFrameRate(m_opFrameRateTextControl -> Text());
			float vFrameRate(0);
			int32 vSelectionStart(0);
			int32 vSelectionEnd(0);
			int vDecimalPosition(-1);
			bool vErased(false);

			// Check after non numeric characters and delete them
			for(int vCounter = 0; vCounter < oFrameRate.size(); vCounter++)
			{
				if(oFrameRate[vCounter] < 48 || oFrameRate[vCounter] > 57)
				{
					if(oFrameRate[vCounter] == ',')
						continue;
					oFrameRate.erase(vCounter, 1);
					vErased = true;
					break;
				}
			}
			// Check if character was added/deleted before the decimal character and move decimal position
			m_opFrameRateTextControl -> GetSelection(vSelectionStart, vSelectionEnd);
			if(m_vSelectionStartBeforeDecimal /*vSelectionStart <= m_vDecimalPosition*/ && m_vDecimalPosition != -1)
			{
				if(m_vFrameRateLength < oFrameRate.size())
					m_vDecimalPosition++;
				else if(m_vFrameRateLength > oFrameRate.size())
					m_vDecimalPosition--;
			}

			// Setting the new decimal position if needed
			vDecimalPosition = oFrameRate.find_first_of(",");
			if(vDecimalPosition == string::npos)
				m_vDecimalPosition = -1;
			else
			{
				if(vDecimalPosition == m_vDecimalPosition)
				{
					vDecimalPosition = oFrameRate.find_first_of(",", m_vDecimalPosition + 1);
					if(vDecimalPosition != string::npos)
					{
						oFrameRate.erase(m_vDecimalPosition, 1);
						vErased = true;
						m_vDecimalPosition = vDecimalPosition - 1;
					}
				}
				else
				{
					if(m_vDecimalPosition == -1)
						m_vDecimalPosition = vDecimalPosition;
					else
					{
						oFrameRate.erase(m_vDecimalPosition, 1);
						m_vDecimalPosition = vDecimalPosition;
					}
				}
			}

			// Setting number of decimals
			if(oFrameRate.size() - m_vDecimalPosition > NUMBER_OF_DECIMALS && m_vDecimalPosition != -1)
				oFrameRate.erase(oFrameRate.find_first_of(",") + NUMBER_OF_DECIMALS);

			// Set the new value in the textcontrol
			if(oFrameRate.size() != 0)
			{
				m_vFrameRateChanged = true;
				m_opFrameRateTextControl -> SetText(oFrameRate.c_str());
				if(vErased)
					m_opFrameRateTextControl -> SetSelection(vSelectionStart - 1, vSelectionEnd - 1);
				else
					m_opFrameRateTextControl -> SetSelection(vSelectionStart, vSelectionEnd);
			}
			else // if no value exist set value to 1
			{
				m_vFrameRateChanged = true;
				m_opFrameRateTextControl -> SetText("1");
				m_opFrameRateTextControl -> SelectAll();
			}
			m_vFrameRateLength = oFrameRate.size();
			m_opFrameRateTextControl -> Notify(EM_TEXT_CONTROL_LEFT_MOUSE_BUTTON_DOWN);
		}
		else
			m_vFrameRateChanged = false;
		return true;
		/*	bool vStringOK(false);
			int vCounter(0);
			bool vDecimalExist(false);

			m_opFrameRateTextControl -> GetSelection(vSelectionStart, vSelectionEnd);
			if(vSelectionStart <= m_vDecimalPosition)
			{
				if(m_vFrameRateLength < strlen(m_opFrameRateTextControl -> Text()))
					m_vDecimalPosition++;
				else if(m_vFrameRateLength > strlen(m_opFrameRateTextControl -> Text()))
					m_vDecimalPosition--;
			}

			if(oFrameRate.find_first_of(',') == oFrameRate.find_last_of(','))
			{
				if(m_vDecimalExist == true)
					m_vNoNeedToCheck = true;
				else
					m_vNoNeedToCheck =false;
			}
			else
				m_vNoNeedToCheck = false;
			
			while(vStringOK == false)
			{
				vDecimalExist = false;
				for(vCounter = 0; vCounter < oFrameRate.size(); vCounter++)
				{
					if(oFrameRate[vCounter] == ',' && m_vNoNeedToCheck == false)
					{
						vDecimalExist = true;
						if(m_vDecimalExist == false)
						{
							m_vDecimalExist = true;
							m_vDecimalPosition = vCounter;
							vDecimalPosition = m_vDecimalPosition;
							vStringOK = true;
							continue;
						}
						else
						{
							if(m_vDecimalPosition == vCounter)
							{
								oFrameRate.erase(vCounter, 1);
								m_vFrameRateChanged = true;
								m_opFrameRateTextControl -> GetSelection(vSelectionStart, vSelectionEnd);
								m_opFrameRateTextControl -> SetText(oFrameRate.c_str());
								m_opFrameRateTextControl -> SetSelection(vSelectionStart - 1, vSelectionEnd - 1);
								if(vDecimalPosition == -1)
								{
									vStringOK = false;
									vDecimalExist = false;
								}
								else
									vStringOK = true;
							}
							else
							{
								vDecimalPosition = vCounter;
								continue;
							}
							break;
						}
					}
					else if(oFrameRate[vCounter] < 48 || oFrameRate[vCounter] > 57)
					{
						if(oFrameRate[vCounter] == ',')
							continue;
						vStringOK = false;
						oFrameRate.erase(vCounter, 1);
						m_vFrameRateChanged = true;
						m_opFrameRateTextControl -> GetSelection(vSelectionStart, vSelectionEnd);
						m_opFrameRateTextControl -> SetText(oFrameRate.c_str());
						m_opFrameRateTextControl -> SetSelection(vSelectionStart - 1, vSelectionEnd - 1);
						break;
					}
					else
						vStringOK = true;
				}
				if(oFrameRate.size() == 0)
					vStringOK = true;
				if(vDecimalExist == false)
				{
					m_vDecimalExist = false;
					m_vDecimalPosition = -1;
				}
				else
					m_vDecimalPosition = vDecimalPosition;
			}
		}
		else
			m_vFrameRateChanged = false;
			
		return true;
*/
	case TEXT_CONTROL_CHANGED:
		if(m_vAlreadyChanged == false)
		{
			char* vpTemp;
			int32 vStart(0);
			int32 vEnd(0);
			if(p_opSender == m_opBackgroundColorRedTextControl)
			{
				vpTemp = const_cast<char*>(m_opBackgroundColorRedTextControl -> Text());
				vpTemp = SetValidValue(vpTemp);
				m_opBackgroundColorRedTextControl -> GetSelection(vStart, vEnd);
				m_opBackgroundColorRedTextControl -> SetText(vpTemp);
				m_opBackgroundColorRedTrackbarControl -> SetPosition(atoi(vpTemp));
				m_opBackgroundColorRedTextControl -> SetSelection(vStart, vEnd);
				if(m_opDialogWindow -> Lock())
				{
					m_opBackgroundColorPreviewView -> SetViewColor(EMColor(atoi(m_opBackgroundColorRedTextControl -> Text()), atoi(m_opBackgroundColorGreenTextControl -> Text()), atoi(m_opBackgroundColorBlueTextControl -> Text()), 255));
					m_opBackgroundColorPreviewView -> Invalidate(m_opBackgroundColorPreviewView -> Bounds());
					m_opDialogWindow -> Unlock();
				}
				return true;
			}
			if(p_opSender == m_opBackgroundColorGreenTextControl)
			{
				vpTemp = const_cast<char*>(m_opBackgroundColorGreenTextControl -> Text());
				vpTemp = SetValidValue(vpTemp);
				m_opBackgroundColorGreenTextControl -> GetSelection(vStart, vEnd);
				m_opBackgroundColorGreenTextControl -> SetText(vpTemp);
				m_opBackgroundColorGreenTrackbarControl -> SetPosition(atoi(vpTemp));
				m_opBackgroundColorGreenTextControl -> SetSelection(vStart, vEnd);
				if(m_opDialogWindow -> Lock())
				{
					m_opBackgroundColorPreviewView -> SetViewColor(EMColor(atoi(m_opBackgroundColorRedTextControl -> Text()), atoi(m_opBackgroundColorGreenTextControl -> Text()), atoi(m_opBackgroundColorBlueTextControl -> Text()), 255));
					m_opBackgroundColorPreviewView -> Invalidate(m_opBackgroundColorPreviewView -> Bounds());
					m_opDialogWindow -> Unlock();
				}
				return true;
			}
			if(p_opSender == m_opBackgroundColorBlueTextControl)
			{
				vpTemp = const_cast<char*>(m_opBackgroundColorBlueTextControl -> Text());
				vpTemp = SetValidValue(vpTemp);
				m_opBackgroundColorBlueTextControl -> GetSelection(vStart, vEnd);
				m_opBackgroundColorBlueTextControl -> SetText(vpTemp);
				m_opBackgroundColorBlueTrackbarControl -> SetPosition(atoi(vpTemp));
				m_opBackgroundColorBlueTextControl -> SetSelection(vStart, vEnd);
				if(m_opDialogWindow -> Lock())
				{
					m_opBackgroundColorPreviewView -> SetViewColor(EMColor(atoi(m_opBackgroundColorRedTextControl -> Text()), atoi(m_opBackgroundColorGreenTextControl -> Text()), atoi(m_opBackgroundColorBlueTextControl -> Text()), 255));
					m_opBackgroundColorPreviewView -> Invalidate(m_opBackgroundColorPreviewView -> Bounds());
					m_opDialogWindow -> Unlock();
				}
				return true;
			}
		}
		else
		{
			m_vAlreadyChanged = false;
			return true;
		}
		return false;
	case TRACKBAR_CHANGED:
		{
			char vTemp[5];
			if(p_opSender == m_opBackgroundColorRedTrackbarControl)
				m_opBackgroundColorRedTextControl -> SetText(itoa(m_opBackgroundColorRedTrackbarControl -> GetPosition(), vTemp, 10));
			else if(p_opSender == m_opBackgroundColorGreenTrackbarControl)
				m_opBackgroundColorGreenTextControl -> SetText(itoa(m_opBackgroundColorGreenTrackbarControl -> GetPosition(), vTemp, 10));
			else if(p_opSender == m_opBackgroundColorBlueTrackbarControl)
				m_opBackgroundColorBlueTextControl -> SetText(itoa(m_opBackgroundColorBlueTrackbarControl -> GetPosition(), vTemp, 10));
			else
				return false;
			return true;
		}
	case USE_MIDI_NOTE_CHECKBOX_CHANGED:
		if(p_opSender == m_opMetronomeUseMIDINoteCheckBox)
		{
			if(m_opMetronomeUseMIDINoteCheckBox -> IsChecked())
				EnableMidiNote(true);
			else
				EnableMidiNote(false);
		}
		return true;
	case EM_GUI_COMPONENT_TAB_PRESSED:
		m_opDialogWindow -> NextTab();
		return true;
	case EM_GUI_COMPONENT_SHIFT_TAB_PRESSED:
		m_opDialogWindow -> PreviousTab();
		return true;
	case EM_GUI_COMPONENT_GOT_FOCUS:
		if(p_opSender == m_opDialogWindow)
			m_opTabControl -> SetFocus();
		if(p_opSender != m_opFrameRateTextControl)
		{
			string oTemp(m_opFrameRateTextControl -> Text());
			if(oTemp.find_first_of(",") == oTemp.npos)
				if(atoi(m_opFrameRateTextControl -> Text()) == 0)
					m_opFrameRateTextControl -> SetText("1");
		}
		m_opDialogWindow -> SetCurrentFocus();
		return true;
	case EM_CLOSE_WINDOW:
		if(p_opSender == m_opDialogWindow)
		{
			eo << "EM_CLOSE_WINDOW" << ef;
			HideDialog();
			return true;
		}
		break;
	}
	return false;
}

void ProjectSettingsDialog::MoveDialog()
{
	EMRect oWindowRect(m_opParentWindow -> Frame());
	EMRect oDialogRect(m_opDialogWindow -> Frame());
	float vX(oWindowRect.m_vRight - oWindowRect.GetWidth() / 2 - oDialogRect.GetWidth() / 2);
	float vY(oWindowRect.m_vBottom - oWindowRect.GetHeight() / 2 - oDialogRect.GetHeight() / 2);

	MoveDialog(vX, vY);
}

void ProjectSettingsDialog::MoveDialog(float p_vX, float p_vY)
{
	m_opDialogWindow -> MoveTo(p_vX, p_vY);
}

void ProjectSettingsDialog::SaveSettings()
{
	string oStringSetting;
//	int64 vInt64Setting;
	int32 vInt32Setting;
	int vIntSetting;
	float vFloatSetting;
	bool vBoolSetting;
	EMSettingsRepository* oSettings = EMSettingsRepository::Instance();

	// Video Dimension Settings
	if(m_opDimensionPreDefineRadioButton -> IsChecked())
	{
		vBoolSetting = true;
		EMSettingsRepository::Instance() -> SetSetting(SETTING_USE_PREDEFINED_VIDEO_FORMAT, EM_SETTING_BOOL, &vBoolSetting, false);
		vIntSetting = m_oResolutionMap[m_opDimensionComboBox -> GetSelectionIndex()].first;
		EMSettingsRepository::Instance() -> SetSetting(SETTING_VIDEO_FORMAT_WIDTH, EM_SETTING_INT, &vIntSetting, false);
		vIntSetting = m_oResolutionMap[m_opDimensionComboBox -> GetSelectionIndex()].second;
		EMSettingsRepository::Instance() -> SetSetting(SETTING_VIDEO_FORMAT_HEIGHT, EM_SETTING_INT, &vIntSetting, false);
	
	}
	else
	{
		vBoolSetting = false;
		EMSettingsRepository::Instance() -> SetSetting(SETTING_USE_PREDEFINED_VIDEO_FORMAT, EM_SETTING_BOOL, &vBoolSetting, false);
		vIntSetting = atoi(m_opWidthTextControl -> Text());
		EMSettingsRepository::Instance() -> SetSetting(SETTING_VIDEO_FORMAT_WIDTH, EM_SETTING_INT, &vIntSetting, false);
		vIntSetting = atoi(m_opHeightTextControl -> Text());
		EMSettingsRepository::Instance() -> SetSetting(SETTING_VIDEO_FORMAT_HEIGHT, EM_SETTING_INT, &vIntSetting, false);
	}
	vBoolSetting = m_opVideoAspectRatiosCheckBox -> IsChecked();
	EMSettingsRepository::Instance() -> SetSetting(SETTING_USE_ASPECT_RATIOS, EM_SETTING_BOOL, &vBoolSetting, false);
	vIntSetting = m_opVideoAspectRatiosComboBox ->GetSelectionIndex();
	EMSettingsRepository::Instance() -> SetSetting(SETTING_ASPECT_RATIOS, EM_SETTING_INT, &vIntSetting, false);

	// Video Framerate Settings
	if(m_opFrameRatePreDefineRadioButton -> IsChecked())
	{
		vBoolSetting = true;
		vFloatSetting = m_oFrameRateMap[m_opFrameRateComboBox -> GetSelectionIndex()];
	}
	else
	{
		vBoolSetting = false;
		string oFrameRate(m_opFrameRateTextControl -> Text());
		if(m_vDecimalPosition != -1)
			oFrameRate.replace(m_vDecimalPosition, 1, ".");
		sscanf(oFrameRate.c_str(), "%f", &vFloatSetting);
		if(vFloatSetting == static_cast<float>(0))
			vFloatSetting = 1;
	}
	EMSettingsRepository::Instance() -> SetSetting(SETTING_USE_PREDEFINED_VIDEO_FRAMERATE, EM_SETTING_BOOL, &vBoolSetting, false);
	EMSettingsRepository::Instance() -> SetSetting(SETTING_VIDEO_FRAMERATE, EM_SETTING_FLOAT, &vFloatSetting, false);

	vInt32Setting =	atoi(m_opBackgroundColorRedTextControl -> Text());		// 0x00RRGGBB
	vInt32Setting = vInt32Setting << 8;
	vInt32Setting += atoi(m_opBackgroundColorGreenTextControl -> Text());
	vInt32Setting = vInt32Setting << 8;
	vInt32Setting += atoi(m_opBackgroundColorBlueTextControl -> Text());
	oSettings -> SetSetting(SETTING_VIDEO_BACKGROUND_COLOR, EM_SETTING_INT32, &vInt32Setting);

	// Metronome Settings
	if(m_opMetronomeUseDuringPlaybackRadioButton -> IsChecked())
	{
		vBoolSetting = true;
		oSettings -> SetSetting(SETTING_MIDI_METRONOME_DURING_PLAYBACK, EM_SETTING_BOOL, &vBoolSetting, false);
		vBoolSetting = false;
		oSettings -> SetSetting(SETTING_MIDI_METRONOME_DURING_RECORDING, EM_SETTING_BOOL, &vBoolSetting, false);
	}
	else if(m_opMetronomeUseDuringRecordingRadioButton -> IsChecked())
	{
		vBoolSetting = false;
		oSettings -> SetSetting(SETTING_MIDI_METRONOME_DURING_PLAYBACK, EM_SETTING_BOOL, &vBoolSetting, false);
		vBoolSetting = true;
		oSettings -> SetSetting(SETTING_MIDI_METRONOME_DURING_RECORDING, EM_SETTING_BOOL, &vBoolSetting, false);
	}
	else if(m_opMetronomeUseDuringBothRadioButton -> IsChecked())
	{
		vBoolSetting = true;
		oSettings -> SetSetting(SETTING_MIDI_METRONOME_DURING_PLAYBACK, EM_SETTING_BOOL, &vBoolSetting, false);
		vBoolSetting = true;
		oSettings -> SetSetting(SETTING_MIDI_METRONOME_DURING_RECORDING, EM_SETTING_BOOL, &vBoolSetting, false);
	}
	vBoolSetting = m_opMetronomeUsePCSpeakerCheckBox -> IsChecked();
	oSettings -> SetSetting(SETTING_MIDI_METRONOME_USE_PC_SPEAKER, EM_SETTING_BOOL, &vBoolSetting, false);
	vBoolSetting = m_opMetronomeUseMIDINoteCheckBox -> IsChecked();
	oSettings -> SetSetting(SETTING_MIDI_METRONOME_USE_MIDI_NOTE, EM_SETTING_BOOL, &vBoolSetting, false);
	vInt32Setting = m_opMetronomeCountInComboBox -> GetSelectionIndex();
	oSettings -> SetSetting(SETTING_MIDI_METRONOME_COUNT_IN, EM_SETTING_INT32, &vInt32Setting, false);
	oStringSetting = m_opMetronomeMidiNoteDeviceComboBox -> GetSelection();
	oSettings -> SetSetting(SETTING_MIDI_METRONOME_MIDI_DEVICE, EM_SETTING_STRING, &oStringSetting, false);
	vInt32Setting = m_opMetronomeMidiNoteChannelComboBox -> GetSelectionIndex();
	oSettings -> SetSetting(SETTING_MIDI_METRONOME_MIDI_CHANNEL, EM_SETTING_INT32, &vInt32Setting, false);
	vInt32Setting = m_opMetronomeFirstBeatKeyComboBox -> GetSelectionIndex();
	oSettings -> SetSetting(SETTING_MIDI_METRONOME_MIDI_NOTE_HIGH_KEY, EM_SETTING_INT32, &vInt32Setting, false);
	vInt32Setting = m_opMetronomeFirstBeatVelocityComboBox -> GetSelectionIndex();
	oSettings -> SetSetting(SETTING_MIDI_METRONOME_MIDI_NOTE_HIGH_VELOCITY, EM_SETTING_INT32, &vInt32Setting, false);
	vInt32Setting = m_opMetronomeFirstBeatDurationComboBox -> GetSelectionIndex();
	oSettings -> SetSetting(SETTING_MIDI_METRONOME_MIDI_NOTE_HIGH_DURATION, EM_SETTING_INT32, &vInt32Setting, false);
	vInt32Setting = m_opMetronomeOtherBeatKeyComboBox -> GetSelectionIndex();
	oSettings -> SetSetting(SETTING_MIDI_METRONOME_MIDI_NOTE_LOW_KEY, EM_SETTING_INT32, &vInt32Setting, false);
	vInt32Setting = m_opMetronomeOtherBeatVelocityComboBox -> GetSelectionIndex();
	oSettings -> SetSetting(SETTING_MIDI_METRONOME_MIDI_NOTE_LOW_VELOCITY, EM_SETTING_INT32, &vInt32Setting, false);
	vInt32Setting = m_opMetronomeOtherBeatDurationComboBox -> GetSelectionIndex();
	oSettings -> SetSetting(SETTING_MIDI_METRONOME_MIDI_NOTE_LOW_DURATION, EM_SETTING_INT32, &vInt32Setting);

}

void ProjectSettingsDialog::SetMidiKeyInComboBox(EMComboBox* p_opComboBox)
{
/*	int vOctaveCounter = 10;
	int vInOctaveCounter = 7;
	bool vBlackKey = false;
	int vKeyCounter = 6;
	char vKey[5];
	
	for(int vTotalCounter = 128; vTotalCounter > 0; vTotalCounter--, vInOctaveCounter--)
	{
		if(vInOctaveCounter == -1)
		{
			vOctaveCounter--;
			vInOctaveCounter = 11;
		}
		if(vKeyCounter == -1)
			vKeyCounter = 6;
				
		if(vBlackKey)
		{
			vKey[0] = 'a' + vKeyCounter;
			vKey[1] = '#';
			if(vOctaveCounter == 10)
			{
				vKey[2] = '1';
				vKey[3] = '0';
				vKey[4] = 0;
			}
			else
			{
				vKey[2] = vOctaveCounter + 48;
				vKey[3] = 0;
			}
			vBlackKey = false;
		}
		else
		{
			vKey[0] = 'a' + vKeyCounter;
			if(vOctaveCounter == 10)
			{
				vKey[1] = '1';
				vKey[2] = '0';
				vKey[3] = 0;
			}
			else
			{
				vKey[1] = vOctaveCounter + 48;
				vKey[2] = 0;
			}
			vKeyCounter--;
			vBlackKey = true;
		}
		if(vInOctaveCounter == 0 || vInOctaveCounter == 5)
		{
//			vKeyCounter--;
			vBlackKey = false;
		}

		p_opComboBox -> AddString(vKey);
	}
*/
	int vOctaveCounter = 0;
	int vInOctaveCounter = 0;
	bool vBlackKey = false;
	int vKeyCounter = 2;
	char vKey[5];
	
	for(int vTotalCounter = 0; vTotalCounter < 128; vTotalCounter++, vInOctaveCounter++)
	{
		if(vInOctaveCounter == 12)
		{
			vOctaveCounter++;
			vInOctaveCounter = 0;
		}
		if(vKeyCounter == 7)
			vKeyCounter = 0;
		
		if(vBlackKey)
		{
			vKey[0] = 'a' + vKeyCounter;
			vKey[1] = '#';
			if(vOctaveCounter == 10)
			{
				vKey[2] = '1';
				vKey[3] = '0';
				vKey[4] = 0;
			}
			else
			{
				vKey[2] = vOctaveCounter + 48;
				vKey[3] = 0;
			}
			vKeyCounter++;
			vBlackKey = false;
		}
		else
		{
			vKey[0] = 'a' + vKeyCounter;
			if(vOctaveCounter == 10)
			{
				vKey[1] = '1';
				vKey[2] = '0';
				vKey[3] = 0;
			}
			else
			{
				vKey[1] = vOctaveCounter + 48;
				vKey[2] = 0;
			}
			vBlackKey = true;
		}
		if(vInOctaveCounter == 4 || vInOctaveCounter == 11)
		{
			vKeyCounter++;
			vBlackKey = false;
		}

		p_opComboBox -> AddString(vKey);
	}

}

void ProjectSettingsDialog::SetNumberInComboBox(EMComboBox* p_opComboBox, int16 p_vNumberOfNumbers, int16 p_vStartValue)
{
	char vTemp[6] = {0,0,0,0,0,0};
	for(int vCounter = 0; vCounter < p_vNumberOfNumbers; vCounter++)
	{
		itoa(vCounter + p_vStartValue, vTemp, 10);
		p_opComboBox -> AddString(string(vTemp));
	}
}

void ProjectSettingsDialog::SetParent(EMWindow* p_opWindow)
{
	m_opParentWindow = p_opWindow;
}

char* ProjectSettingsDialog::SetValidValue(char* p_vpValue)
{
	eo << p_vpValue << ef;
	m_vAlreadyChanged = true;

	int vValue(atoi(p_vpValue));
	if(vValue > 255)
		vValue = 255;
	if(vValue < 0)
		vValue = 0;
	return itoa(vValue, m_vTemp, 10);

}