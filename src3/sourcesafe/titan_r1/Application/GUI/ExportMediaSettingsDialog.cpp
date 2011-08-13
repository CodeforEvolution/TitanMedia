#include "ExportMediaSettingsDialog.h"

#include "EMFactory.h"

#include "EMWindow.h"
#include "EMView.h"
#include "EMGroupBox.h"
#include "EMButton.h"
#include "EMLabel.h"
#include "EMCheckBox.h"
#include "EMComboBox.h"
#include "EMMediaGlobals.h"
#include "EMTextControl.h"
#include "EMRadioButton.h"
#include "EMCommandRepository.h"
#include "EMSettingIDs.h"
#include "EMSettingsRepository.h"
#include "CommandIDs.h"
#include "ExportMediaDialog.h"

#include "EMMessages.h"

#ifdef PLATFORM_WINDOWS
#include <windows.h> // To get modal dialog to work.
#include "EMWinWindow.h" // To get modal dialog to work.
#endif

char ExportMediaSettingsDialog::m_vFileExtension[5] = ".avi";

const uint32 NUMBER_OF_DECIMALS = 3; // Count the decimal-character too. I.e. 0,01 -> NUMBER_OF_DECIMALS = 3

const uint32 OK_BUTTON = 1;
const uint32 CANCEL_BUTTON = 2;
const uint32 AUDIO_SETTINGS_BUTTON = 3;
const uint32 DIMENSION_DEFINE_RADIO_BUTTON = 4;
const uint32 USER_DEFINE_RADIO_BUTTON = 5;
const uint32 VIDEO_SETTINGS_BUTTON = 6;
const uint32 AUDIO_ENCODER_CHANGED = 7;
const uint32 VIDEO_ENCODER_CHANGED = 8;
const uint32 FRAMERATE_DEFINE_RADIO_BUTTON = 9;
const uint32 ASPECT_RATIOS_TOGGLE = 10;
const uint32 ASPECT_RATIOS_COMBO_BOX_CHANGED = 11;
const uint32 WIDTH_TEXT_CONTROL_CHANGED = 12;
const uint32 HEIGHT_TEXT_CONTROL_CHANGED = 13;
const uint32 FRAMERATE_TEXT_CONTROL_CHANGED = 14;
const uint32 VIDEO_FAMILY_COMBO_BOX_CHANGED = 15;

//#define EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE 7
//#define EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP 15
//#define EM_METRICS_NATIVE_GROUP_BOX_MARGIN_B0TTOM 7

ExportMediaSettingsDialog::ExportMediaSettingsDialog() :
m_opDialogWindow(EMFactory::Instance() -> CreateEMWindow(EMRect(0, 0, 443, 348), "Export Media Settings", EM_WINDOW_DIALOG)), 
m_opMainView(EMFactory::Instance() -> CreateView(EMRect(0, 0, 443, 348), false)),

m_opAudioGroupBox(EMFactory::Instance() -> CreateGroupBox(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
																 EM_METRICS_NATIVE_DIALOG_MARGIN_TOP,
																 EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 423,
																 EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 70),
														  "Audio", 0)),
m_opAudioView(EMFactory::Instance() ->CreateView(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
														 EM_METRICS_NATIVE_DIALOG_MARGIN_TOP,
														 EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 423,
														 EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 70),
												  0)),
m_opAudioEncoderLabel(EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
															 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 2,
															 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 50,
															 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 2 + EM_METRICS_NATIVE_LABEL_HEIGHT),
													  "Encoder:", 0)),
m_opAudioFormatLabel(EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
																EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 27,
																EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 70,
																EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 27 + EM_METRICS_NATIVE_LABEL_HEIGHT),
														 "Format:", 0)),
/*
m_opBitDepthLabel(EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
															  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 52,
															  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 70,
															  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 52 + EM_METRICS_NATIVE_LABEL_HEIGHT),
													   "Bit Depth:", 0)),
m_opChannelsLabel(EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
															  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 77,
															  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 70,
															  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 77 + EM_METRICS_NATIVE_LABEL_HEIGHT),
													   "Channels:", 0)),
*/
m_opAudioEncoderComboBox(EMFactory::Instance() -> CreateComboBox(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 73,
																   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP,
																   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 73 + 250,
																   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 100),
															AUDIO_ENCODER_CHANGED, 0, true)),
m_opAudioFormatComboBox(EMFactory::Instance() -> CreateComboBox(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 73,
																	  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 25,
																	  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 73 + 250,
																	  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 25 + 100),
															   0, 0, true)),
/*
m_opBitDepthComboBox(EMFactory::Instance() -> CreateComboBox(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 73,
																	EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 50,
																	EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 73 + 250,
																	EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 50 + 100),
															 0, 0, false)),
m_opChannelsComboBox(EMFactory::Instance() -> CreateComboBox(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 73,
																	EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 75,
																	EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 73 + 250,
																	EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 75 + 100),
															 0, 0, false)),
*/
m_opAudioSettingsButton(EMFactory::Instance() -> CreateButton(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 333,
																EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP,
																EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 333 + EM_METRICS_NATIVE_BUTTON_WIDTH,
																EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + EM_METRICS_NATIVE_BUTTON_HEIGHT),
														 "Settings...", AUDIO_SETTINGS_BUTTON, 0, 0)),

m_opVideoGroupBox(EMFactory::Instance() -> CreateGroupBox(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
																 EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 75,
																 EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 423,
																 EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 75 + 220),
														  "Video", 0)),
m_opVideoView(EMFactory::Instance() -> CreateView(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
														 EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 75,
														 EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 423,
														 EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 75 + 220),
												  0)),
m_opDimensionLabel(EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
															   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 2,
															   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 55,
															   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 2 + EM_METRICS_NATIVE_LABEL_HEIGHT),
														"Dimension:", 0)),
m_opVideoFamilyLabel(EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
															 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 77,
															 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 70,
															 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 77 + EM_METRICS_NATIVE_LABEL_HEIGHT),
													  "Family:", 0)),
m_opVideoEncoderLabel(EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
															 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 102,
															 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 70,
															 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 102 + EM_METRICS_NATIVE_LABEL_HEIGHT),
													  "Encoder:", VIDEO_ENCODER_CHANGED)),
m_opFrameRateLabel(EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
															   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 127,
															   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 70,
															   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 127	+ EM_METRICS_NATIVE_LABEL_HEIGHT),
														"Framerate:", 0)),
m_opQualityLabel(EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
															 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 177,
															 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 70,
															 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 177 + EM_METRICS_NATIVE_LABEL_HEIGHT),
													  "Quality:", 0)),
m_opDimensionPreDefineRadioButton(EMFactory::Instance() -> CreateRadioButton(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 73,
																		   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 2,
																		   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 73 + 80,
																		   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 2 + EM_METRICS_NATIVE_RADIO_BUTTON_HEIGHT),
																	"Pre-Define", DIMENSION_DEFINE_RADIO_BUTTON, 0, 0, false)),
m_opDimensionUserDefineRadioButton(EMFactory::Instance() -> CreateRadioButton(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 73,
																			EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 27,
																			EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 73 + 80,
																			EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 27 + EM_METRICS_NATIVE_RADIO_BUTTON_HEIGHT),
																	 "User-Define", DIMENSION_DEFINE_RADIO_BUTTON, 0, 0, true)),
m_opWidthLabel(EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 173,
														   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 27,
														   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 173 + 30,
														   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 27 + EM_METRICS_NATIVE_LABEL_HEIGHT),
													"Width:", 0)),
m_opWidthTextControl(EMFactory::Instance() -> CreateTextControl(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 208,
																	   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 25,
																	   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 208 + 30,
																	   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 25 + EM_METRICS_NATIVE_TEXT_CONTROL_HEIGHT),
																"0", WIDTH_TEXT_CONTROL_CHANGED, 0, false, EM_ALIGNMENT_RIGHT, true)),
m_opHeightLabel(EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 253,
															EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 27,
															EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 253 + 35,
															EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 27 + EM_METRICS_NATIVE_LABEL_HEIGHT),
													 "Heigth:", 0)),
m_opHeightTextControl(EMFactory::Instance() -> CreateTextControl(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 293,
																		EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 25,
																		EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 293 + 30,
																		EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 25 + EM_METRICS_NATIVE_TEXT_CONTROL_HEIGHT),
																 "0", HEIGHT_TEXT_CONTROL_CHANGED, 0, false, EM_ALIGNMENT_RIGHT, true)),
m_opDimensionComboBox(EMFactory::Instance() -> CreateComboBox(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 173,
																	 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP,
																	 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 173 + 150,
																	 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 100),
															  0, 0, false)),
m_opVideoAspectRatiosCheckBox(EMFactory::Instance() -> CreateCheckBox(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 73,
																			 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 50,
																			 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 73 + 90,
																			 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 50 + EM_METRICS_NATIVE_CHECKBOX_HEIGHT),
																	  "Aspect Ratios", ASPECT_RATIOS_TOGGLE, 0, 0)),
m_opVideoAspectRatiosComboBox(EMFactory::Instance() -> CreateComboBox(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 173,
																			 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 50,
																			 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 173 + 150,
																			 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 50 + 100),
																	  ASPECT_RATIOS_COMBO_BOX_CHANGED, 0, false)),
m_opVideoFamilyComboBox(EMFactory::Instance() -> CreateComboBox(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 73,
																   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 75,
																   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 73 + 250,
																   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 57 + 100),
															VIDEO_FAMILY_COMBO_BOX_CHANGED, 0, false)),
m_opVideoEncoderComboBox(EMFactory::Instance() -> CreateComboBox(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 73,
																   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 100,
																   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 73 + 250,
																   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 100 + 100),
															VIDEO_ENCODER_CHANGED, 0, true)),
m_opFrameRatePreDefineRadioButton(EMFactory::Instance() -> CreateRadioButton(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 73,
																			 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 127,
																			 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 73 + 80,
																			 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 127 + EM_METRICS_NATIVE_RADIO_BUTTON_HEIGHT),
																	"Pre-Define", FRAMERATE_DEFINE_RADIO_BUTTON, 0, 0, false)),
m_opFrameRateUserDefineRadioButton(EMFactory::Instance() -> CreateRadioButton(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 73,
																			  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 152,
																			  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 73 + 80,
																			  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 152 + EM_METRICS_NATIVE_RADIO_BUTTON_HEIGHT),
																	 "User-Define", FRAMERATE_DEFINE_RADIO_BUTTON, 0, 0, true)),
m_opFrameRateComboBox(EMFactory::Instance() -> CreateComboBox(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 173,
																	 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 125,
																	 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 173 + 150,
																	 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 125 + 100),
															  0, 0, false)),
m_opFrameRateTextControl(EMFactory::Instance() -> CreateTextControl(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 173,
																		   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 150,
																		   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 173 + 32,
																		   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 150 + EM_METRICS_NATIVE_TEXT_CONTROL_HEIGHT),
																	"25,00", FRAMERATE_TEXT_CONTROL_CHANGED, 0, false, EM_ALIGNMENT_RIGHT)),
m_opQualityTextControl(EMFactory::Instance() -> CreateTextControl(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 73,
																   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 175,
																   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 73 + 25,
																   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 175 + EM_METRICS_NATIVE_TEXT_CONTROL_HEIGHT),
															"75", 0, 0, false, EM_ALIGNMENT_RIGHT, true)),
m_opProcentageLabel(EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 73 + 25 + 5,
															 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 178,
															 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 73 + 25 + 15,
															 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 178 + EM_METRICS_NATIVE_LABEL_HEIGHT),
													  "%", 0)),
m_opVideoSettingsButton(EMFactory::Instance() -> CreateButton(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 333,
																EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 100,
																EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 333 + EM_METRICS_NATIVE_BUTTON_WIDTH,
																EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 100 + EM_METRICS_NATIVE_BUTTON_HEIGHT),
														 "Settings...", VIDEO_SETTINGS_BUTTON, 0, 0)),

m_opOK_Button(EMFactory::Instance() -> CreateButton(EMRect(443 - (EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + EM_METRICS_NATIVE_BUTTON_WIDTH * 2 + EM_METRICS_NATIVE_BUTTON_SPACE),
														   348 - (EM_METRICS_NATIVE_DIALOG_MARGIN_BOTTOM + EM_METRICS_NATIVE_BUTTON_HEIGHT),
														   443 - (EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + EM_METRICS_NATIVE_BUTTON_WIDTH + EM_METRICS_NATIVE_BUTTON_SPACE),
														   348 - (EM_METRICS_NATIVE_DIALOG_MARGIN_BOTTOM)),
													"OK", OK_BUTTON, 0, 0, NULL, true)),
m_opCancelButton(EMFactory::Instance() -> CreateButton(EMRect(443 - (EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + EM_METRICS_NATIVE_BUTTON_WIDTH),
															  348 - (EM_METRICS_NATIVE_DIALOG_MARGIN_BOTTOM + EM_METRICS_NATIVE_BUTTON_HEIGHT),
															  443 - (EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE),
															  348 - (EM_METRICS_NATIVE_DIALOG_MARGIN_BOTTOM)),
													   "Cancel", CANCEL_BUTTON, 0, 0)),
m_vAudioEncoderOld(0),
m_vAudioFormatOld(0),
m_vDimensionDefineOld(true),
m_vDimensionPreDefineOld(0),
m_oDimensionUserDefineWidthOld("320"),
m_oDimensionUserDefineHeightOld("240"),
m_vVideoAspectRatiosToggleOld(false),
m_vAspectRatiosIsChecked(false),
m_vVideoAspectRatiosOld(0),
m_vVideoFrameRateDefineOld(true),
m_vVideoFrameRatePreDefineOld(0),
m_oVideoFrameRateUserDefineOld("25,00"),
m_vVideoFamilyOld(0),
m_vVideoEncoderOld(0),
m_oQualityOld("75"),
m_vWidthChanged(false),
m_vHeightChanged(false),
m_vFrameRateChanged(false),
//m_vDecimalExist(false),
m_vDecimalPosition(-1)
//m_vNoNeedToCheck(false)
{
	m_opDialogWindow -> AddChild(m_opMainView);
	
	m_opMainView -> AddChild(m_opAudioGroupBox);
	m_opMainView -> AddChild(m_opAudioView);
	m_opAudioView -> AddChild(m_opAudioEncoderLabel);
	m_opAudioView -> AddChild(m_opAudioFormatLabel);
//	m_opAudioView -> AddChild(m_opBitDepthLabel);
//	m_opAudioView -> AddChild(m_opChannelsLabel);
	m_opAudioView -> AddChild(m_opAudioEncoderComboBox);
	m_opAudioView -> AddChild(m_opAudioFormatComboBox);
//	m_opAudioView -> AddChild(m_opBitDepthComboBox);
//	m_opAudioView -> AddChild(m_opChannelsComboBox);
	m_opAudioView -> AddChild(m_opAudioSettingsButton);

	m_opMainView -> AddChild(m_opVideoGroupBox);
	m_opMainView -> AddChild(m_opVideoView);
	m_opVideoView -> AddChild(m_opDimensionLabel);
	m_opVideoView -> AddChild(m_opFrameRateLabel);
	m_opVideoView -> AddChild(m_opVideoFamilyLabel);
	m_opVideoView -> AddChild(m_opVideoEncoderLabel);
	m_opVideoView -> AddChild(m_opQualityLabel);
	m_opVideoView -> AddChild(m_opDimensionPreDefineRadioButton);
	m_opVideoView -> AddChild(m_opDimensionUserDefineRadioButton);
	m_opVideoView -> AddChild(m_opWidthLabel);
	m_opVideoView -> AddChild(m_opWidthTextControl);
	m_opVideoView -> AddChild(m_opHeightLabel);
	m_opVideoView -> AddChild(m_opHeightTextControl);
	m_opVideoView -> AddChild(m_opVideoAspectRatiosCheckBox);
	m_opVideoView -> AddChild(m_opVideoAspectRatiosComboBox);
	m_opVideoView -> AddChild(m_opDimensionComboBox);
	m_opVideoView -> AddChild(m_opVideoFamilyComboBox);
	m_opVideoView -> AddChild(m_opVideoEncoderComboBox);
	m_opVideoView -> AddChild(m_opFrameRatePreDefineRadioButton);
	m_opVideoView -> AddChild(m_opFrameRateUserDefineRadioButton);
	m_opVideoView -> AddChild(m_opFrameRateComboBox);
	m_opVideoView -> AddChild(m_opFrameRateTextControl);
	m_opVideoView -> AddChild(m_opQualityTextControl);
	m_opVideoView -> AddChild(m_opProcentageLabel);
	m_opVideoView -> AddChild(m_opVideoSettingsButton);

	m_opMainView -> AddChild(m_opOK_Button);
	m_opMainView -> AddChild(m_opCancelButton);
}

ExportMediaSettingsDialog::~ExportMediaSettingsDialog()
{
	delete m_opDialogWindow;
	delete m_opMainView;
	delete m_opAudioGroupBox;
	delete m_opAudioEncoderLabel;
	delete m_opAudioFormatLabel;
//	delete m_opBitDepthLabel;
//	delete m_opChannelsLabel;
	delete m_opAudioEncoderComboBox;
	delete m_opAudioFormatComboBox;
//	delete m_opBitDepthComboBox;
//	delete m_opChannelsComboBox;
	delete m_opAudioSettingsButton;

	delete m_opVideoGroupBox;
	delete m_opDimensionLabel;
	delete m_opFrameRateLabel;
	delete m_opVideoFamilyLabel;
	delete m_opVideoEncoderLabel;
	delete m_opQualityLabel;
	delete m_opDimensionPreDefineRadioButton;
	delete m_opDimensionUserDefineRadioButton;
	delete m_opWidthLabel;
	delete m_opWidthTextControl;
	delete m_opHeightLabel;
	delete m_opHeightTextControl;
	delete m_opVideoAspectRatiosCheckBox;
	delete m_opVideoAspectRatiosComboBox;
	delete m_opDimensionComboBox;
	delete m_opVideoFamilyComboBox;
	delete m_opVideoEncoderComboBox;
	delete m_opFrameRatePreDefineRadioButton;
	delete m_opFrameRateUserDefineRadioButton;
	delete m_opFrameRateComboBox;
	delete m_opFrameRateTextControl;
	delete m_opQualityTextControl;
	delete m_opProcentageLabel;
	delete m_opVideoSettingsButton;

	delete m_opOK_Button;
	delete m_opCancelButton;
}

void ExportMediaSettingsDialog::DisplayDialog()
{
	m_opAudioEncoderComboBox -> SelectItem(m_vAudioEncoderOld);
	m_opAudioEncoderComboBox -> Notify(AUDIO_ENCODER_CHANGED);
	m_opAudioFormatComboBox -> SelectItem(m_vAudioFormatOld);
	m_opVideoAspectRatiosCheckBox -> Check(m_vVideoAspectRatiosToggleOld);
	m_opVideoAspectRatiosComboBox -> SelectItem(m_vVideoAspectRatiosOld);
	if(m_vDimensionDefineOld)
	{
		m_opDimensionPreDefineRadioButton -> Check(true);
		m_opDimensionUserDefineRadioButton -> Check(false);
	}
	else
	{
		m_opDimensionPreDefineRadioButton -> Check(false);
		m_opDimensionUserDefineRadioButton -> Check(true);
		m_opVideoAspectRatiosCheckBox -> Notify(ASPECT_RATIOS_TOGGLE);
	}
	m_opDimensionPreDefineRadioButton -> Notify(DIMENSION_DEFINE_RADIO_BUTTON);
	m_opDimensionComboBox -> SelectItem(m_vDimensionPreDefineOld);
	m_opWidthTextControl -> SetText(m_oDimensionUserDefineWidthOld.c_str());
	m_opHeightTextControl -> SetText(m_oDimensionUserDefineHeightOld.c_str());
	m_opVideoFamilyComboBox -> SelectItem(m_vVideoFamilyOld);
	m_opVideoEncoderComboBox -> SelectItem(m_vVideoEncoderOld);
	m_opVideoEncoderComboBox -> Notify(VIDEO_ENCODER_CHANGED);
	if(m_vVideoFrameRateDefineOld)
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
	m_opFrameRateComboBox -> SelectItem(m_vVideoFrameRatePreDefineOld);
	m_opFrameRateTextControl -> SetText(m_oVideoFrameRateUserDefineOld.c_str());
	m_opQualityTextControl -> SetText(m_oQualityOld.c_str());

	m_vFrameRateLength = strlen(m_opFrameRateTextControl -> Text());

	if(m_opDialogWindow -> Lock())
	{
		m_opDialogWindow -> Show();
#ifdef PLATFORM_WINDOWS
		EnableWindow(static_cast<HWND>(m_opParentWindow -> GetNativeView()), FALSE);
		SetFocus(static_cast<HWND>(m_opDialogWindow -> GetNativeView()));
#endif
		m_opDialogWindow -> Unlock();
	}
}

void ExportMediaSettingsDialog::EnableAudioVideo(bool p_vEnableAudio, bool p_vEnableVideo)
{
	m_opAudioEncoderComboBox -> Enable(p_vEnableAudio);
	m_opAudioEncoderLabel -> Enable(p_vEnableAudio);
	m_opAudioFormatComboBox -> Enable(p_vEnableAudio);
	m_opAudioFormatLabel -> Enable(p_vEnableAudio);
	m_opAudioGroupBox -> Enable(p_vEnableAudio);

	m_opDimensionLabel -> Enable(p_vEnableVideo);
	m_opDimensionPreDefineRadioButton -> Enable(p_vEnableVideo);
	m_opDimensionUserDefineRadioButton -> Enable(p_vEnableVideo);
	m_opFrameRateLabel -> Enable(p_vEnableVideo);
	m_opFrameRateComboBox -> Enable(p_vEnableVideo);
	m_opFrameRateTextControl -> Enable(p_vEnableVideo);
	m_opFrameRatePreDefineRadioButton -> Enable(p_vEnableVideo);
	m_opFrameRateUserDefineRadioButton -> Enable(p_vEnableVideo);
	m_opHeightLabel -> Enable(p_vEnableVideo);
	m_opProcentageLabel -> Enable(p_vEnableVideo);
	m_opQualityLabel -> Enable(p_vEnableVideo);
	m_opQualityTextControl -> Enable(p_vEnableVideo);
	m_opVideoAspectRatiosCheckBox -> Enable(p_vEnableVideo);
	m_opVideoAspectRatiosComboBox -> Enable(p_vEnableVideo);
	m_opVideoEncoderComboBox -> Enable(p_vEnableVideo);
	m_opVideoEncoderLabel -> Enable(p_vEnableVideo);
	m_opVideoFamilyComboBox -> Enable(p_vEnableVideo);
	m_opVideoFamilyLabel -> Enable(p_vEnableVideo);
	m_opVideoGroupBox -> Enable(p_vEnableVideo);
	m_opWidthLabel -> Enable(p_vEnableVideo);
	
	if(p_vEnableAudio == false)
		m_opOK_Button -> Enable(true);
	else
		m_opAudioEncoderComboBox -> Notify(AUDIO_ENCODER_CHANGED);
	if(p_vEnableVideo == false)
	{
		m_opDimensionComboBox -> Enable(p_vEnableVideo);
		m_opHeightTextControl -> Enable(p_vEnableVideo);
		m_opWidthTextControl -> Enable(p_vEnableVideo);
		m_opVideoSettingsButton -> Enable(p_vEnableVideo);
	}
	else
	{
		m_opDimensionPreDefineRadioButton -> Notify(DIMENSION_DEFINE_RADIO_BUTTON);
		m_opFrameRatePreDefineRadioButton -> Notify(FRAMERATE_DEFINE_RADIO_BUTTON);
		m_opVideoEncoderComboBox -> Notify(VIDEO_ENCODER_CHANGED);
	}
}

void ExportMediaSettingsDialog::HideDialog()
{
	if(m_opDialogWindow -> Lock())
	{
#ifdef PLATFORM_WINDOWS
		EnableWindow(static_cast<HWND>(m_opParentWindow -> GetNativeView()), TRUE);
		SetFocus(static_cast<HWND>(m_opParentWindow -> GetNativeView()));
#endif
		m_opDialogWindow -> Hide();
		m_opDialogWindow -> Unlock();
	}
}

void ExportMediaSettingsDialog::Init()
{
	m_opDialogWindow -> AddListener(this);
	if(m_opDialogWindow -> Lock())
	{
		m_opMainView -> AddListener(this);
		//m_opMainView -> SetViewColor(EMColor(192, 192, 192, 255));
		m_opMainView -> SetDefaultColor();
		m_opAudioView -> AddListener(this);
		//m_opAudioView -> SetViewColor(EMColor(192, 192, 192, 255));
		m_opAudioView -> SetDefaultColor();
		m_opVideoView -> AddListener(this);
		//m_opVideoView -> SetViewColor(EMColor(192, 192, 192, 255));
		m_opVideoView -> SetDefaultColor();
		m_opDialogWindow -> Unlock();
	}
	m_opAudioEncoderComboBox -> AddListener(this);
	m_opAudioFormatComboBox -> AddListener(this);
	m_opAudioSettingsButton -> AddListener(this);
	m_opDimensionPreDefineRadioButton -> AddListener(this);
	m_opDimensionUserDefineRadioButton -> AddListener(this);
	m_opVideoAspectRatiosCheckBox -> AddListener(this);
	m_opDimensionComboBox -> AddListener(this);
	m_opWidthTextControl -> AddListener(this);
	m_opHeightTextControl -> AddListener(this);
	m_opVideoAspectRatiosComboBox -> AddListener(this);
	m_opVideoFamilyComboBox -> AddListener(this);
	m_opVideoEncoderComboBox -> AddListener(this);
	m_opVideoSettingsButton -> AddListener(this);
	m_opFrameRatePreDefineRadioButton -> AddListener(this);
	m_opFrameRateUserDefineRadioButton -> AddListener(this);
	m_opFrameRateComboBox -> AddListener(this);
	m_opFrameRateTextControl -> AddListener(this);
	m_opQualityTextControl -> AddListener(this);
	m_opOK_Button -> AddListener(this);
	m_opCancelButton -> AddListener(this);

	//Setting TabOrder
	m_opDialogWindow -> AddTabComponent(m_opAudioEncoderComboBox);
	m_opDialogWindow -> AddTabComponent(m_opAudioFormatComboBox);
	m_opDialogWindow -> AddTabComponent(m_opAudioSettingsButton);
	m_opDialogWindow -> AddTabComponent(m_opDimensionPreDefineRadioButton);
	m_opDialogWindow -> AddTabComponent(m_opDimensionUserDefineRadioButton);
	m_opDialogWindow -> AddTabComponent(m_opVideoAspectRatiosCheckBox);
	m_opDialogWindow -> AddTabComponent(m_opDimensionComboBox);
	m_opDialogWindow -> AddTabComponent(m_opWidthTextControl);
	m_opDialogWindow -> AddTabComponent(m_opHeightTextControl);
	m_opDialogWindow -> AddTabComponent(m_opVideoAspectRatiosComboBox);
	m_opDialogWindow -> AddTabComponent(m_opVideoFamilyComboBox);
	m_opDialogWindow -> AddTabComponent(m_opVideoEncoderComboBox);
	m_opDialogWindow -> AddTabComponent(m_opVideoSettingsButton);
	m_opDialogWindow -> AddTabComponent(m_opFrameRatePreDefineRadioButton);
	m_opDialogWindow -> AddTabComponent(m_opFrameRateUserDefineRadioButton);
	m_opDialogWindow -> AddTabComponent(m_opFrameRateComboBox);
	m_opDialogWindow -> AddTabComponent(m_opFrameRateTextControl);
	m_opDialogWindow -> AddTabComponent(m_opQualityTextControl);
	m_opDialogWindow -> AddTabComponent(m_opOK_Button);
	m_opDialogWindow -> AddTabComponent(m_opCancelButton);

	m_opDimensionPreDefineRadioButton -> Check(true);
	m_opFrameRatePreDefineRadioButton -> Check(true);
	m_opWidthLabel -> Enable(false);
	m_opWidthTextControl -> Enable(false);
	m_opHeightLabel -> Enable(false);
	m_opHeightTextControl -> Enable(false);

	m_opVideoFamilyComboBox -> AddString("AVI-Family Encoders (.avi)");
	m_opVideoFamilyComboBox -> AddString("QuickTime-Family Encoders (.mov)");
	m_opVideoFamilyComboBox -> SelectItem(0);
	m_opVideoFamilyComboBox -> Notify(VIDEO_FAMILY_COMBO_BOX_CHANGED);
/*
	int32 vNumberOfAudioEncoders(*static_cast<int32*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_NUMBER_AUDIO_ENCODERS, NULL, NULL, NULL)));
	uint32* vpAudioEncoderIDs = EM_new uint32[sizeof(uint32) * vNumberOfAudioEncoders];
	EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_AUDIO_ENCODERS, vpAudioEncoderIDs, NULL, NULL);

	for(int i = 0; i < vNumberOfAudioEncoders; i++)
	{
		string* opTempString = static_cast<string*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_ENCODER_NAME, &vpAudioEncoderIDs[i]));
		m_opAudioEncoderComboBox -> AddString(opTempString -> c_str());
		m_oAudioEncoderStringIDMap[opTempString -> c_str()] = vpAudioEncoderIDs[i];
	}
	m_opAudioEncoderComboBox -> SelectItem(0);
	m_opAudioEncoderComboBox -> Notify(AUDIO_ENCODER_CHANGED);
	delete [] vpAudioEncoderIDs;

	int32 vNumberOfVideoEncoders(*static_cast<int32*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_NUMBER_VIDEO_AVI_ENCODERS, NULL, NULL, NULL)));
	uint32* vpVideoEncoderIDs = EM_new uint32[sizeof(uint32) * vNumberOfVideoEncoders];
	EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_VIDEO_AVI_ENCODERS, vpVideoEncoderIDs, NULL, NULL);

	for(i = 0; i < vNumberOfVideoEncoders; i++)
	{
		string* opTempString = static_cast<string*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_ENCODER_NAME, &vpVideoEncoderIDs[i]));
		m_opVideoEncoderComboBox -> AddString(opTempString -> c_str());
		m_oVideoEncoderStringIDMap[opTempString -> c_str()] = vpVideoEncoderIDs[i];
	}
	m_opVideoEncoderComboBox -> SelectItem(0);
	m_opVideoEncoderComboBox -> Notify(VIDEO_ENCODER_CHANGED);
	delete [] vpVideoEncoderIDs;
*/
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

	m_opFrameRateComboBox -> AddString("29,97 (TV-NTSC)");
	m_oFrameRateMap[0] = 29.97f;
	m_opFrameRateComboBox -> AddString("25 (TV-PAL)");
	m_oFrameRateMap[1] = 25.0f;
	m_opFrameRateComboBox -> AddString("24 (FILM)");
	m_oFrameRateMap[2] = 24.0f;
	m_opFrameRateComboBox -> AddString("15 (Cinepack)");
	m_oFrameRateMap[3] = 15.0f;
	m_opFrameRateComboBox -> SelectItem(0);

	m_opVideoAspectRatiosComboBox -> AddString("[4:3] European & Asian theatre");
	m_oAspectRatiosMap[0] = ResolutionPair(4, 3);
	m_opVideoAspectRatiosComboBox -> AddString("[16:9] North American theatre");
	m_oAspectRatiosMap[1] = ResolutionPair(16, 9);
	m_opVideoAspectRatiosComboBox -> AddString("[12:5] Worldwide WideScreen");
	m_oAspectRatiosMap[2] = ResolutionPair(12, 5);
	m_opVideoAspectRatiosComboBox -> SelectItem(0);
}

bool ExportMediaSettingsDialog::MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage)
{
	switch(p_vMessage)
	{
/*	case EM_DRAW:
		if(p_opSender == m_opMainView)
		{
			eo << "EM_DRAW: m_oMainView" << ef;
			m_opMainView -> SetHighColor(EMColor(0, 192, 0, 255));
			m_opMainView -> FillRect(m_opMainView -> Bounds());
		}
		else
			eo << "EM_DRAW: <unknown> " << ef;
		return true;
*/	case EM_GUI_COMPONENT_ENTER_PRESSED:
		if(m_opOK_Button -> IsEnabled() == false)
			return true;
	case OK_BUTTON:
		{
			eo << "OK buttom pressed" << ef;
			eo << "Selection from AudioEncoder = " << (char*)m_opAudioEncoderComboBox -> GetSelection().c_str() << ef;
			eo << "Selection from AudioFormat = " << (char*)m_opAudioFormatComboBox -> GetSelection().c_str() << ef;
			if(m_opDimensionComboBox -> IsEnabled())
				eo << "Selection from Dimensions = " << m_oResolutionMap[m_opDimensionComboBox -> GetSelectionIndex()].first << "x" << m_oResolutionMap[m_opDimensionComboBox -> GetSelectionIndex()].second << ef;
			else
				eo << "Selection from Dimensions = " << const_cast<char*>(m_opWidthTextControl -> Text()) << "x" << const_cast<char*>(m_opHeightTextControl -> Text()) << ef;
			eo << "Selection from Framerate = " << const_cast<char*>(m_opFrameRateTextControl -> Text()) << ef;
			eo << "Selection from VideoEncoder = " << (char*)m_opVideoEncoderComboBox -> GetSelection().c_str() << ef;
			eo << "Selection from Quality = " << const_cast<char*>(m_opQualityTextControl -> Text()) << ef;

			// Saving settings for next time dialog is opened
			m_vAudioEncoderOld = m_opAudioEncoderComboBox -> GetSelectionIndex();
			m_vAudioFormatOld = m_opAudioFormatComboBox -> GetSelectionIndex();
			m_vDimensionDefineOld = m_opDimensionPreDefineRadioButton -> IsChecked();
			m_vDimensionPreDefineOld = m_opDimensionComboBox -> GetSelectionIndex();
			m_oDimensionUserDefineWidthOld = m_opWidthTextControl -> Text();
			m_oDimensionUserDefineHeightOld = m_opHeightTextControl -> Text();
			m_vVideoAspectRatiosOld = m_opVideoAspectRatiosComboBox -> GetSelectionIndex();
			m_vVideoAspectRatiosToggleOld = m_opVideoAspectRatiosCheckBox -> IsChecked();
			m_vVideoFrameRateDefineOld = m_opFrameRatePreDefineRadioButton -> IsChecked();
			m_vVideoFrameRatePreDefineOld = m_opFrameRateComboBox -> GetSelectionIndex();
			m_oVideoFrameRateUserDefineOld = m_opFrameRateTextControl -> Text();
			m_vVideoFamilyOld = m_opVideoFamilyComboBox -> GetSelectionIndex();
			m_vVideoEncoderOld = m_opVideoEncoderComboBox -> GetSelectionIndex();
			m_oQualityOld = m_opQualityTextControl -> Text();

			// Setting render settings
			uint32 vAudioEncoderID(m_oAudioEncoderStringIDMap[m_opAudioEncoderComboBox -> GetSelection()]);
			uint32 vAudioFormatID(m_oAudioFormatStringIDMap[m_opAudioFormatComboBox -> GetSelection()]);
			uint32 vVideoEncoderID(m_oVideoEncoderStringIDMap[m_opVideoEncoderComboBox -> GetSelection()]);
			EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_SET_AUDIO_ENCODER, &vAudioEncoderID, NULL, NULL);
			if(m_opDimensionComboBox -> IsEnabled())
			{
				EMSettingsRepository::Instance() -> SetSetting(SETTING_VIDEO_RENDER_FORMAT_WIDTH, EM_SETTING_INT, EM_new int(m_oResolutionMap[m_opDimensionComboBox -> GetSelectionIndex()].first), false);
				EMSettingsRepository::Instance() -> SetSetting(SETTING_VIDEO_RENDER_FORMAT_HEIGHT, EM_SETTING_INT, EM_new int(m_oResolutionMap[m_opDimensionComboBox -> GetSelectionIndex()].second), false);
			}
			else
			{
				int vAsciiInt = atoi(m_opWidthTextControl -> Text()); //Richard had to put this here because the goddamn thing kept giving me internal compiler errors.
				int* vpInt = EM_new int(vAsciiInt); //Richard had to put this here because the goddamn thing kept giving me internal compiler errors.
				EMSettingsRepository::Instance() -> SetSetting(SETTING_VIDEO_RENDER_FORMAT_WIDTH, EM_SETTING_INT, vpInt, false);

				vAsciiInt = atoi(m_opHeightTextControl -> Text()); //Richard had to put this here because the goddamn thing kept giving me internal compiler errors.
				vpInt = EM_new int(vAsciiInt); //Richard had to put this here because the goddamn thing kept giving me internal compiler errors.
				EMSettingsRepository::Instance() -> SetSetting(SETTING_VIDEO_RENDER_FORMAT_HEIGHT, EM_SETTING_INT, vpInt, false);
			}
			if(m_opFrameRateComboBox -> IsEnabled())
				EMSettingsRepository::Instance() -> SetSetting(SETTING_VIDEO_RENDER_FRAMERATE, EM_SETTING_FLOAT, EM_new float(m_oFrameRateMap[m_opFrameRateComboBox -> GetSelectionIndex()]), false);
			else
			{
				float vFrameRate;
				string oFrameRate(m_opFrameRateTextControl -> Text());
				oFrameRate.replace(oFrameRate.find_first_of(','), 1, ".");
				sscanf(oFrameRate.c_str(), "%f", &vFrameRate);
				EMSettingsRepository::Instance() -> SetSetting(SETTING_VIDEO_RENDER_FRAMERATE, EM_SETTING_FLOAT, EM_new float(vFrameRate), false);
			}
			EMSettingsRepository::Instance() -> GetSetting(SETTING_VIDEO_RENDER_FRAMERATE);
			EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_SET_ENCODER_PROPERTY, &vAudioEncoderID, &vAudioFormatID, NULL);
			EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_SET_VIDEO_ENCODER, &vVideoEncoderID, NULL, NULL);

			int vAsciiInt = atoi(m_opQualityTextControl -> Text()); //Richard had to put this here because the goddamn thing kept giving me internal compiler errors.
			int* vpInt = EM_new int(vAsciiInt); //Richard had to put this here because the goddamn thing kept giving me internal compiler errors.
			EMSettingsRepository::Instance() -> SetSetting(SETTING_VIDEO_RENDER_QUALITY, EM_SETTING_INT, vpInt, false);

			// Setting file-extension
			if(m_opVideoFamilyComboBox -> GetSelectionIndex() == 0) // == AVI-family
				strcpy(m_vFileExtension, ".avi");
			else  // == QuickTime-family
				strcpy(m_vFileExtension, ".mov");
			m_opExportMediaDialog -> GetExportTypeComboBox() -> Notify(EXPORT_TYPE_COMBO_BOX_CHANGED);


			HideDialog();
			return true;
		}
	case EM_GUI_COMPONENT_ESC_PRESSED:
	case CANCEL_BUTTON:
		eo << "Cancel button pressed" << ef;
		HideDialog();
		return true;
	case EM_GUI_COMPONENT_TAB_PRESSED:
		m_opDialogWindow -> NextTab();
		return true;
	case EM_GUI_COMPONENT_SHIFT_TAB_PRESSED:
		m_opDialogWindow -> PreviousTab();
		return true;
	case EM_GUI_COMPONENT_GOT_FOCUS:
		if(p_opSender == m_opDialogWindow)
		{
			if(m_opAudioGroupBox -> IsEnabled())
				m_opAudioEncoderComboBox -> SetFocus();
			else
				m_opDimensionPreDefineRadioButton -> SetFocus();
		}
		m_opDialogWindow -> SetCurrentFocus();
		return true;
	case AUDIO_SETTINGS_BUTTON:
		eo << "Audio Settings... button pressed" << ef;
		EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_ENCODER_PROPERTY_PAGE, &m_oAudioEncoderStringIDMap[m_opAudioEncoderComboBox -> GetSelection().c_str()], NULL, NULL);
		return true;
	case VIDEO_SETTINGS_BUTTON:
		eo << "Video Settings... button pressed" << ef;
		EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_ENCODER_PROPERTY_PAGE, &m_oVideoEncoderStringIDMap[m_opVideoEncoderComboBox -> GetSelection().c_str()], NULL, NULL);
		return true;
	case DIMENSION_DEFINE_RADIO_BUTTON:
		eo << "Pre-Define checkbox checked/unchecked" << ef;
		if(m_opDimensionPreDefineRadioButton -> IsChecked())
		{
			eo << "m_opPreDefineRadioButton -> IsChecked()" << ef;
			if(m_opVideoGroupBox -> IsEnabled())
				m_opDimensionComboBox -> Enable(true);
			else
				m_opDimensionComboBox -> Enable(false);
			m_opWidthLabel -> Enable(false);
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
			if(m_opVideoGroupBox -> IsEnabled())
			{
				m_opWidthLabel -> Enable(true);
				m_opWidthTextControl -> Enable(true);
				m_opHeightLabel -> Enable(true);
				m_opHeightTextControl -> Enable(true);
				m_opVideoAspectRatiosCheckBox -> Enable(true);
				m_opVideoAspectRatiosCheckBox -> Notify(ASPECT_RATIOS_TOGGLE);
			}
			else
			{
				m_opWidthLabel -> Enable(false);
				m_opWidthTextControl -> Enable(false);
				m_opHeightLabel -> Enable(false);
				m_opHeightTextControl -> Enable(false);
				m_opVideoAspectRatiosCheckBox -> Enable(false);
				m_opVideoAspectRatiosComboBox -> Enable(false);
			}
		}
		return true;
	case FRAMERATE_DEFINE_RADIO_BUTTON:
		eo << "Pre-Define checkbox checked/unchecked" << ef;
		if(m_opFrameRatePreDefineRadioButton -> IsChecked())
		{
			eo << "m_opPreDefineRadioButton -> IsChecked()" << ef;
			if(m_opVideoGroupBox -> IsEnabled())
				m_opFrameRateComboBox -> Enable(true);
			else
				m_opFrameRateComboBox -> Enable(false);
			m_opFrameRateTextControl -> Enable(false);
		}
		else if(m_opFrameRateUserDefineRadioButton -> IsChecked())
		{
			eo << "m_opUserDefineRadioButton -> IsChecked()" << ef;
			m_opFrameRateComboBox -> Enable(false);
			if(m_opVideoGroupBox -> IsEnabled())
				m_opFrameRateTextControl -> Enable(true);
			else
				m_opFrameRateTextControl -> Enable(false);
		}
		return true;
	case AUDIO_ENCODER_CHANGED:
		{
			eo << "AUDIO_ENCODER_CHANGED" << ef;
			m_opAudioFormatComboBox -> Clear();
			string* opFormats = static_cast<string*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_ENCODER_PROPERTIES, &m_oAudioEncoderStringIDMap[m_opAudioEncoderComboBox -> GetSelection().c_str()], NULL, NULL));
			if(strcmp(opFormats -> c_str(), "--- Not Available ---") == 0)  // FIXME: The string "--- Not Available ---" should be taken from the resoucefile.
			{
				m_opAudioFormatComboBox -> AddString(opFormats -> c_str());
				if(m_opAudioGroupBox -> IsEnabled())
					m_opOK_Button -> Enable(false);
				else
					m_opOK_Button -> Enable(true);
			}
			else
			{
				m_oAudioFormatStringIDMap.erase(m_oAudioFormatStringIDMap.begin(), m_oAudioFormatStringIDMap.end()); // Erase map contents
				for(int i = 1; strcmp(opFormats -> c_str(), "") != 0; i++)
				{
					m_opAudioFormatComboBox -> AddString(opFormats -> c_str());
					m_oAudioFormatStringIDMap[opFormats -> c_str()] = i;
					opFormats++;
				}
				m_opOK_Button -> Enable(true);
			}
			m_opAudioFormatComboBox -> SelectItem(0);

			if((*static_cast<bool*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_HAS_ENCODER_PROPERTY_PAGE, &m_oAudioEncoderStringIDMap[m_opAudioEncoderComboBox -> GetSelection().c_str()], NULL, NULL))) == true)
				m_opAudioSettingsButton -> Enable(true);
			else
				m_opAudioSettingsButton -> Enable(false);

			return true;
		}
	case VIDEO_ENCODER_CHANGED:
		eo << "VIDEO_ENCODER_CHANGED" << ef;
		EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_ENCODER_PROPERTIES, &m_oVideoEncoderStringIDMap[m_opVideoEncoderComboBox -> GetSelection().c_str()], NULL, NULL);
		if((*static_cast<bool*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_HAS_ENCODER_PROPERTY_PAGE, &m_oVideoEncoderStringIDMap[m_opVideoEncoderComboBox -> GetSelection().c_str()], NULL, NULL))) == true)
			m_opVideoSettingsButton -> Enable(true);
		else
			m_opVideoSettingsButton -> Enable(false);
		if((*static_cast<bool*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_HAS_QUALITY, &m_oVideoEncoderStringIDMap[m_opVideoEncoderComboBox -> GetSelection().c_str()], NULL, NULL))) == true)
			m_opQualityTextControl -> Enable(true);
		else
			m_opQualityTextControl -> Enable(false);
		return true;
	case ASPECT_RATIOS_TOGGLE:
		eo << "ASPECT_RATIOS_TOGGLE" << ef;
		if(m_opVideoAspectRatiosCheckBox -> IsChecked())
		{
			m_vAspectRatiosIsChecked = true;
			if(m_opVideoGroupBox -> IsEnabled())
			{
				m_opVideoAspectRatiosComboBox -> Enable(true);
				m_opVideoAspectRatiosComboBox -> Notify(ASPECT_RATIOS_COMBO_BOX_CHANGED);
			}
			else
				m_opVideoAspectRatiosComboBox -> Enable(false);
		}
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
			if(vSelectionStart <= m_vDecimalPosition + 1 && m_vDecimalPosition != -1)
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
		}
		else
			m_vFrameRateChanged = false;
		return true;
/*		if(m_vFrameRateChanged == false)
		{
			//m_vFrameRateChanged = true;
			string oFrameRate(m_opFrameRateTextControl -> Text());
			bool vStringOK(false);
			int vCounter(0);
			bool vDecimalExist(false);
			int vDecimalPosition(-1);
			int32 vSelectionStart(0);
			int32 vSelectionEnd(0);

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
			if(oFrameRate.size() == 0)
			{
				m_vFrameRateChanged = true;
				m_opFrameRateTextControl -> SetText("1");
				m_opFrameRateTextControl -> SelectAll();
			}
		}
		else
			m_vFrameRateChanged = false;
		return true;
*/
	case VIDEO_FAMILY_COMBO_BOX_CHANGED:
		{
			EMMediaFamily eMediaFamily;
			if(m_opVideoFamilyComboBox -> GetSelectionIndex() == 0) // == AVI-family
			{
//				strcpy(m_vFileExtension, ".avi");
				eMediaFamily = EM_FAMILY_AVI;
				EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_SET_ENCODER_FAMILY, &eMediaFamily, NULL, NULL);
			}
			else  // == QuickTime-family
			{
//				strcpy(m_vFileExtension, ".mov");
				eMediaFamily = EM_FAMILY_QUICKTIME;
				EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_SET_ENCODER_FAMILY, &eMediaFamily, NULL, NULL);
			}
//			m_opExportMediaDialog -> GetExportTypeComboBox() -> Notify(EXPORT_TYPE_COMBO_BOX_CHANGED);

			int32 vNumberOfAudioEncoders(*static_cast<int32*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_NUMBER_AUDIO_ENCODERS, NULL, NULL, NULL)));
			uint32* vpAudioEncoderIDs = EM_new uint32[sizeof(uint32) * vNumberOfAudioEncoders];
			EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_AUDIO_ENCODERS, vpAudioEncoderIDs, NULL, NULL);

			m_oAudioEncoderStringIDMap.erase(m_oAudioEncoderStringIDMap.begin(), m_oAudioEncoderStringIDMap.end());
			m_opAudioEncoderComboBox -> Clear();
			for(int i = 0; i < vNumberOfAudioEncoders; i++)
			{
				string* opTempString = static_cast<string*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_ENCODER_NAME, &vpAudioEncoderIDs[i]));
				m_opAudioEncoderComboBox -> AddString(opTempString -> c_str());
				m_oAudioEncoderStringIDMap[opTempString -> c_str()] = vpAudioEncoderIDs[i];
			}
			m_opAudioEncoderComboBox -> SelectItem(0);
			m_opAudioEncoderComboBox -> Notify(AUDIO_ENCODER_CHANGED);
			delete [] vpAudioEncoderIDs;

			int32 vNumberOfVideoEncoders(*static_cast<int32*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_NUMBER_VIDEO_AVI_ENCODERS, NULL, NULL, NULL)));
			uint32* vpVideoEncoderIDs = EM_new uint32[sizeof(uint32) * vNumberOfVideoEncoders];
			EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_VIDEO_AVI_ENCODERS, vpVideoEncoderIDs, NULL, NULL);

			m_oVideoEncoderStringIDMap.erase(m_oVideoEncoderStringIDMap.begin(), m_oVideoEncoderStringIDMap.end());
			m_opVideoEncoderComboBox -> Clear();
			for(i = 0; i < vNumberOfVideoEncoders; i++)
			{
				string* opTempString = static_cast<string*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_ENCODER_NAME, &vpVideoEncoderIDs[i]));
				m_opVideoEncoderComboBox -> AddString(opTempString -> c_str());
				m_oVideoEncoderStringIDMap[opTempString -> c_str()] = vpVideoEncoderIDs[i];
			}
			m_opVideoEncoderComboBox -> SelectItem(0);
			m_opVideoEncoderComboBox -> Notify(VIDEO_ENCODER_CHANGED);
			delete [] vpVideoEncoderIDs;

			return true;
		}
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

void ExportMediaSettingsDialog::MoveDialog()
{
	EMRect oWindowRect(m_opMainAppWindow -> Frame());
	EMRect oDialogRect(m_opDialogWindow -> Frame());
	float vX(oWindowRect.m_vRight - oWindowRect.GetWidth() / 2 - oDialogRect.GetWidth() / 2);
	float vY(oWindowRect.m_vBottom - oWindowRect.GetHeight() / 2 - oDialogRect.GetHeight() / 2);

	MoveDialog(vX, vY);
}

void ExportMediaSettingsDialog::MoveDialog(float p_vX, float p_vY)
{
	m_opDialogWindow -> MoveTo(p_vX, p_vY);
}

void ExportMediaSettingsDialog::SetExportMediaDialog(void* p_opExportMediaDialog)
{
	m_opExportMediaDialog = static_cast<ExportMediaDialog*>(p_opExportMediaDialog);
}

void ExportMediaSettingsDialog::SetMainWindow(EMWindow* p_opWindow)
{
	m_opMainAppWindow = p_opWindow;
}

void ExportMediaSettingsDialog::SetParent(EMWindow* p_opWindow)
{
	m_opParentWindow = p_opWindow;
}
