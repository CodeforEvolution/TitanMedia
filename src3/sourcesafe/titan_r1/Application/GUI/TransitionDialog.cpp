#include "TransitionDialog.h"

#include "CommandIDs.h"
#include "EMButton.h"
#include "EMCheckBox.h"
#include "EMComboBox.h"
#include "EMCommandRepository.h"
#include "EMFactory.h"
#include "EMGClip.h"
#include "EMGroupBox.h"
#include "EMLabel.h"
#include "EMMessages.h"
#include "EMRadioButton.h"
#include "EMTextControl.h"
#include "EMTrackbarControl.h"
#include "EMView.h"
#include "EMWindow.h"

#ifdef PLATFORM_WINDOWS
#include <windows.h> // To get modal dialog to work.
#include "EMWinWindow.h" // To get modal dialog to work.
#endif

//Horizontal start right opposite matrix

const uint32 BUTTON_OK = 1;
const uint32 BUTTON_CANCEL = 2;
const uint32 COMBO_BOX_TRANSITIONS = 3;
const uint32 BUTTON_DEFAULT = 4;
const uint32 TEXT_CONTROL_CHANGED = 5;
const uint32 TRACKBAR_CHANGED = 6;
const uint32 COLOR_KEY_RADIO_BUTTON_CHANGED = 7;
const uint32 COLOR_KEY_INVERT_CHECKBOX = 8;
const uint32 SWAP_CHECKBOX = 9;

TransitionDialog::TransitionDialog() :
m_opDialogWindow(EMFactory::Instance() -> CreateEMWindow(EMRect(0, 0, 300, 385), "Transition", EM_WINDOW_DIALOG)), 
m_opMainView(EMFactory::Instance() -> CreateView(EMRect(0, 0, 300, 385), false)),
m_opComboBox(EMFactory::Instance() -> CreateComboBox(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
														  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP,
														  300 - EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
														  385),
												   COMBO_BOX_TRANSITIONS, 0, true)),
m_opSettingsGroupBox(EMFactory::Instance() -> CreateGroupBox(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
																	EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 30,
																	235 - EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
																	385 - EM_METRICS_NATIVE_DIALOG_MARGIN_BOTTOM - EM_METRICS_NATIVE_BUTTON_HEIGHT - EM_METRICS_NATIVE_BUTTON_SPACE),
															 "Settings", 0)),
m_opSettingsView(EMFactory::Instance() -> CreateView(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
																	EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 30,
																	235 - EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
																	273 - EM_METRICS_NATIVE_DIALOG_MARGIN_BOTTOM - EM_METRICS_NATIVE_BUTTON_HEIGHT - EM_METRICS_NATIVE_BUTTON_SPACE), false)),
m_opBorderGroupBox(EMFactory::Instance() -> CreateGroupBox(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
																  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP,
																  215 - EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
																  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 68),
														   "Border", 0)),
m_opBorderView(EMFactory::Instance() -> CreateView(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
														  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP,
														  215 - EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
														  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 68),
												   false)),
m_opColorLabel(EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
														   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 2,
														   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 30,
														   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 2 + EM_METRICS_NATIVE_LABEL_HEIGHT),
													"Color:", 0)),
m_opSoftnessLabel(EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
														   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 27,
														   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 45,
														   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 27 + EM_METRICS_NATIVE_LABEL_HEIGHT),
													"Softness:", 0)),
m_opWidthLabel(EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 115,
														   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 27,
														   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 115 + 32,
														   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 27 + EM_METRICS_NATIVE_LABEL_HEIGHT),
													"Width:", 0)),
m_opRLabel(EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 40,
														   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 2,
														   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 40 + 12,
														   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 2 + EM_METRICS_NATIVE_LABEL_HEIGHT),
													"R:", 0)),
m_opRTextControl(EMFactory::Instance() -> CreateTextControl(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 55,
																   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP,
																   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 55 + 25,
																   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + EM_METRICS_NATIVE_TEXT_CONTROL_HEIGHT),
															"0", 0, 0, false, EM_ALIGNMENT_RIGHT, true)),
m_opGLabel(EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 90,
														   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 2,
														   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 90 + 12,
														   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 2 + EM_METRICS_NATIVE_LABEL_HEIGHT),
													"G:", 0)),
m_opGTextControl(EMFactory::Instance() -> CreateTextControl(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 105,
																   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP,
																   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 105 + 25,
																   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + EM_METRICS_NATIVE_TEXT_CONTROL_HEIGHT),
															"0", 0, 0, false, EM_ALIGNMENT_RIGHT, true)),
m_opBLabel(EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 140,
														   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 2,
														   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 140 + 12,
														   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 2 + EM_METRICS_NATIVE_LABEL_HEIGHT),
													"B:", 0)),
m_opBTextControl(EMFactory::Instance() -> CreateTextControl(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 155,
																   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP,
																   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 155 + 25,
																   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + EM_METRICS_NATIVE_TEXT_CONTROL_HEIGHT),
															"0", 0, 0, false, EM_ALIGNMENT_RIGHT, true)),
m_opSoftnessTextControl(EMFactory::Instance() -> CreateTextControl(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 50,
																   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 25,
																   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 50 + 30,
																   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 25 + EM_METRICS_NATIVE_TEXT_CONTROL_HEIGHT),
															"0", 0, 0, false, EM_ALIGNMENT_RIGHT, true)),
m_opWidthTextControl(EMFactory::Instance() -> CreateTextControl(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 150,
																   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 25,
																   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 150 + 30,
																   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 25 + EM_METRICS_NATIVE_TEXT_CONTROL_HEIGHT),
															"0", 0, 0, false, EM_ALIGNMENT_RIGHT, true)),

m_opOffsetGroupBox(EMFactory::Instance() -> CreateGroupBox(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
																  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 73,
																  215 / 2 - 3,
																  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 73 + 68),
														   "Offset", 0)),
m_opOffsetView(EMFactory::Instance() -> CreateView(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
														  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 73,
														  215 / 2 - 3,
														  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 73 + 68),
												   false)),
m_opOffXLabel(EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
														   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 2,
														   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 12,
														   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 2 + EM_METRICS_NATIVE_LABEL_HEIGHT),
													"X:", 0)),
m_opOffXTextControl(EMFactory::Instance() -> CreateTextControl(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 50,
																   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP,
																   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 50 + 30,
																   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + EM_METRICS_NATIVE_TEXT_CONTROL_HEIGHT),
															"0", 0, 0, false, EM_ALIGNMENT_RIGHT, true)),
m_opOffYLabel(EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
														   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 27,
														   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 12,
														   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 27 + EM_METRICS_NATIVE_LABEL_HEIGHT),
													"Y:", 0)),
m_opOffYTextControl(EMFactory::Instance() -> CreateTextControl(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 50,
																   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 25,
																   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 50 + 30,
																   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 25 + EM_METRICS_NATIVE_TEXT_CONTROL_HEIGHT),
															"0", 0, 0, false, EM_ALIGNMENT_RIGHT, true)),

m_opReplicateGroupBox(EMFactory::Instance() -> CreateGroupBox(EMRect(215 / 2 + 3,
																  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 73,
																  215 - EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
																  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 73 + 68),
														   "Replicate", 0)),
m_opReplicateView(EMFactory::Instance() -> CreateView(EMRect(215 / 2 + 3,
														  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 73,
														  215 - EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
														  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 73 + 68),
												   false)),
m_opRepXLabel(EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
														   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 2,
														   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 12,
														   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 2 + EM_METRICS_NATIVE_LABEL_HEIGHT),
													"X:", 0)),
m_opRepXTextControl(EMFactory::Instance() -> CreateTextControl(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 50,
																   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP,
																   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 50 + 30,
																   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + EM_METRICS_NATIVE_TEXT_CONTROL_HEIGHT),
															"0", 0, 0, false, EM_ALIGNMENT_RIGHT, true)),
m_opRepYLabel(EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
														   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 27,
														   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 12,
														   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 27 + EM_METRICS_NATIVE_LABEL_HEIGHT),
													"Y:", 0)),
m_opRepYTextControl(EMFactory::Instance() -> CreateTextControl(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 50,
																   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 25,
																   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 50 + 30,
																   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 25 + EM_METRICS_NATIVE_TEXT_CONTROL_HEIGHT),
															"0", 0, 0, false, EM_ALIGNMENT_RIGHT, true)),

m_opSwapCheckBox(EMFactory::Instance() -> CreateCheckBox(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
																EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 73 + 68 + 6,
																EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 95,
																EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 73 + 68 + 6 + EM_METRICS_NATIVE_CHECKBOX_HEIGHT),
																"Swap Direction", SWAP_CHECKBOX, 0, 0)),
m_opOK_Button(EMFactory::Instance() ->CreateButton(EMRect(300 - EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE - EM_METRICS_NATIVE_BUTTON_WIDTH * 2 - EM_METRICS_NATIVE_BUTTON_SPACE,
														   385 - EM_METRICS_NATIVE_DIALOG_MARGIN_BOTTOM - EM_METRICS_NATIVE_BUTTON_HEIGHT,
														   300 - EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE - EM_METRICS_NATIVE_BUTTON_WIDTH - EM_METRICS_NATIVE_BUTTON_SPACE,
														   385 - EM_METRICS_NATIVE_DIALOG_MARGIN_BOTTOM),
													"OK", BUTTON_OK, 0, 0, NULL, true)),
m_opCancelButton(EMFactory::Instance() -> CreateButton(EMRect(300 - EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE - EM_METRICS_NATIVE_BUTTON_WIDTH,
															  385 - EM_METRICS_NATIVE_DIALOG_MARGIN_BOTTOM - EM_METRICS_NATIVE_BUTTON_HEIGHT,
															  300 - EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
															  385- EM_METRICS_NATIVE_DIALOG_MARGIN_BOTTOM),
													   "Cancel", BUTTON_CANCEL, 0, 0)),
// EM_ColorKey Settings
m_opColorKeySettingsView(EMFactory::Instance() -> CreateView(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
																	EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 30,
																	300 - EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
																	385 - EM_METRICS_NATIVE_DIALOG_MARGIN_BOTTOM - EM_METRICS_NATIVE_BUTTON_HEIGHT - EM_METRICS_NATIVE_BUTTON_SPACE), false)),
m_opColorKeyUseColorRadioButton(EMFactory::Instance() -> CreateRadioButton(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
																				  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP,
																				  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 95,
																				  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + EM_METRICS_NATIVE_RADIO_BUTTON_HEIGHT),
																			"Use Color", COLOR_KEY_RADIO_BUTTON_CHANGED, 0, 0, false)),
m_opColorKeyUseHueRadioButton(EMFactory::Instance() -> CreateRadioButton(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 100,
																				  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP,
																				  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 100 + 95,
																				  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + EM_METRICS_NATIVE_RADIO_BUTTON_HEIGHT),
																			"Use Hue", COLOR_KEY_RADIO_BUTTON_CHANGED, 0, 0, false)),
m_opColorKeyUseLuminanceRadioButton(EMFactory::Instance() -> CreateRadioButton(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
																				  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + EM_METRICS_NATIVE_RADIO_BUTTON_HEIGHT + 1,
																				  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 95,
																				  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP  + EM_METRICS_NATIVE_RADIO_BUTTON_HEIGHT + 1 + EM_METRICS_NATIVE_RADIO_BUTTON_HEIGHT),
																			"Use Luminance", COLOR_KEY_RADIO_BUTTON_CHANGED, 0, 0, false)),
m_opColorKeyUseBlueAndGreenScreenRadioButton(EMFactory::Instance() -> CreateRadioButton(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 100,
																				  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + EM_METRICS_NATIVE_RADIO_BUTTON_HEIGHT + 1,
																				  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 100 + 160,
																				  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + EM_METRICS_NATIVE_RADIO_BUTTON_HEIGHT + 1 + EM_METRICS_NATIVE_RADIO_BUTTON_HEIGHT),
																			"Use Blue- and Green- screen", COLOR_KEY_RADIO_BUTTON_CHANGED, 0, 0, true)),
m_vAlreadyChanged(false)
{
	m_opDefaultButton = EMFactory::Instance() -> CreateButton(EMRect(m_opSettingsView -> Frame().GetWidth() - EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE - EM_METRICS_NATIVE_BUTTON_WIDTH,
																m_opSettingsView -> Frame().GetHeight() - EM_METRICS_NATIVE_GROUP_BOX_MARGIN_B0TTOM - EM_METRICS_NATIVE_BUTTON_HEIGHT,
																m_opSettingsView -> Frame().GetWidth() - EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
																m_opSettingsView -> Frame().GetHeight() - EM_METRICS_NATIVE_GROUP_BOX_MARGIN_B0TTOM),
														 "Default", BUTTON_DEFAULT, 0, 0);
	m_opDialogWindow -> AddChild(m_opMainView);
	m_opMainView -> AddChild(m_opComboBox);
	m_opMainView -> AddChild(m_opSettingsGroupBox);
	m_opMainView -> AddChild(m_opSettingsView);
	m_opMainView -> AddChild(m_opOK_Button);
	m_opMainView -> AddChild(m_opCancelButton);

	m_opSettingsView -> AddChild(m_opBorderGroupBox);
	m_opSettingsView -> AddChild(m_opBorderView);
	m_opSettingsView -> AddChild(m_opOffsetGroupBox);
	m_opSettingsView -> AddChild(m_opOffsetView);
	m_opSettingsView -> AddChild(m_opReplicateGroupBox);
	m_opSettingsView -> AddChild(m_opReplicateView);
	m_opSettingsView -> AddChild(m_opSwapCheckBox);
	m_opSettingsView -> AddChild(m_opDefaultButton);

	m_opBorderView -> AddChild(m_opColorLabel);
	m_opBorderView -> AddChild(m_opSoftnessLabel);
	m_opBorderView -> AddChild(m_opWidthLabel);
	m_opBorderView -> AddChild(m_opRLabel);
	m_opBorderView -> AddChild(m_opRTextControl);
	m_opBorderView -> AddChild(m_opGLabel);
	m_opBorderView -> AddChild(m_opGTextControl);
	m_opBorderView -> AddChild(m_opBLabel);
	m_opBorderView -> AddChild(m_opBTextControl);
	m_opBorderView -> AddChild(m_opSoftnessTextControl);
	m_opBorderView -> AddChild(m_opWidthTextControl);

	m_opOffsetView -> AddChild(m_opOffXLabel);
	m_opOffsetView -> AddChild(m_opOffXTextControl);
	m_opOffsetView -> AddChild(m_opOffYLabel);
	m_opOffsetView -> AddChild(m_opOffYTextControl);

	m_opReplicateView -> AddChild(m_opRepXLabel);
	m_opReplicateView -> AddChild(m_opRepXTextControl);
	m_opReplicateView -> AddChild(m_opRepYLabel);
	m_opReplicateView -> AddChild(m_opRepYTextControl);
	
	// EM_Color Settings
	m_opMainView -> AddChild(m_opColorKeySettingsView);
	m_opColorKeySettingsView -> AddChild(m_opColorKeyUseColorRadioButton);
	m_opColorKeySettingsView -> AddChild(m_opColorKeyUseHueRadioButton);
	m_opColorKeySettingsView -> AddChild(m_opColorKeyUseLuminanceRadioButton);
	m_opColorKeySettingsView -> AddChild(m_opColorKeyUseBlueAndGreenScreenRadioButton);

	// Color
	m_opColorKeyColorGroupBox = EMFactory::Instance() -> CreateGroupBox(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
																				 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 35,
																				 m_opColorKeySettingsView -> Frame().GetWidth() - EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
																				 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 35 + 92),
																		  "Color", 0);
	m_opColorKeyColorView = EMFactory::Instance() -> CreateView(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
																		 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 35,
																		 m_opColorKeySettingsView -> Frame().GetWidth() - EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
																		 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 35 + 92),
																  0);
	m_opColorKeyColorRedLabel = EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
																			  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 3,
																			  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 35,
																			  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 3 + EM_METRICS_NATIVE_LABEL_HEIGHT),
																	   "Red:", 0);
	m_opColorKeyColorGreenLabel = EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
																				EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 28,
																				EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 35,
																				EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 28 + EM_METRICS_NATIVE_LABEL_HEIGHT),
																		 "Green:", 0);
	m_opColorKeyColorBlueLabel = EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
																			   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 53,
																			   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 35,
																			   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 53 + EM_METRICS_NATIVE_LABEL_HEIGHT),
																		"Blue:", 0);
	m_opColorKeyColorRedTrackbarControl = EMFactory::Instance() -> CreateTrackbarControl(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 40,
																								  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP,
																								  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 40 + 100,
																								  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + EM_METRICS_NATIVE_TRACKBAR_CONTROL_HEIGHT),
																						   "RedTrackbar", TRACKBAR_CHANGED, 0, 0);
	m_opColorKeyColorGreenTrackbarControl = EMFactory::Instance() -> CreateTrackbarControl(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 40,
																								  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 25,
																								  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 40 + 100,
																								  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 25 + EM_METRICS_NATIVE_TRACKBAR_CONTROL_HEIGHT),
																						   "GreenTrackbar", TRACKBAR_CHANGED, 0, 0);
	m_opColorKeyColorBlueTrackbarControl = EMFactory::Instance() -> CreateTrackbarControl(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 40,
																								  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 50,
																								  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 40 + 100,
																								  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 50 + EM_METRICS_NATIVE_TRACKBAR_CONTROL_HEIGHT),
																						   "BlueTrackbar", TRACKBAR_CHANGED, 0, 0);
	m_opColorKeyColorRedTextControl = EMFactory::Instance() -> CreateTextControl(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 150,
																						  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP,
																						  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 150 + 25,
																						  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + EM_METRICS_NATIVE_TEXT_CONTROL_HEIGHT),
																				   "0", TEXT_CONTROL_CHANGED, 0, false, EM_ALIGNMENT_RIGHT, true);
	m_opColorKeyColorGreenTextControl = EMFactory::Instance() -> CreateTextControl(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 150,
																							EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 25,
																							EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 150 + 25,
																							EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 25 + EM_METRICS_NATIVE_TEXT_CONTROL_HEIGHT),
																					 "0", TEXT_CONTROL_CHANGED, 0, false, EM_ALIGNMENT_RIGHT, true);
	m_opColorKeyColorBlueTextControl = EMFactory::Instance() -> CreateTextControl(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 150,
																						   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 50,
																						   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 150 + 25,
																						   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 50 + EM_METRICS_NATIVE_TEXT_CONTROL_HEIGHT),
																					"0", TEXT_CONTROL_CHANGED, 0, false, EM_ALIGNMENT_RIGHT, true);
	m_opColorKeyColorPreviewView = EMFactory::Instance() -> CreateView(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 190,
																				EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP,
																				m_opColorKeyColorView -> Frame().GetWidth() - EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
																				m_opColorKeyColorView -> Frame().GetHeight() - EM_METRICS_NATIVE_GROUP_BOX_MARGIN_B0TTOM),
																		 0);

	m_opColorKeySettingsView -> AddChild(m_opColorKeyColorGroupBox);
	m_opColorKeySettingsView -> AddChild(m_opColorKeyColorView);
	m_opColorKeyColorView -> AddChild(m_opColorKeyColorRedLabel);
	m_opColorKeyColorView -> AddChild(m_opColorKeyColorGreenLabel);
	m_opColorKeyColorView -> AddChild(m_opColorKeyColorBlueLabel);
	m_opColorKeyColorView -> AddChild(m_opColorKeyColorRedTrackbarControl);
	m_opColorKeyColorView -> AddChild(m_opColorKeyColorGreenTrackbarControl);
	m_opColorKeyColorView -> AddChild(m_opColorKeyColorBlueTrackbarControl);
	m_opColorKeyColorView -> AddChild(m_opColorKeyColorRedTextControl);
	m_opColorKeyColorView -> AddChild(m_opColorKeyColorGreenTextControl);
	m_opColorKeyColorView -> AddChild(m_opColorKeyColorBlueTextControl);
	m_opColorKeyColorView -> AddChild(m_opColorKeyColorPreviewView);

	m_opColorKeyHueGroupBox = EMFactory::Instance() -> CreateGroupBox(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
																				 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 131,
																				 m_opColorKeySettingsView -> Frame().GetWidth() - EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
																				 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 131 + 43),
																		  "Hue", 0);
	m_opColorKeyHueView = EMFactory::Instance() -> CreateView(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
																		 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 131,
																		 m_opColorKeySettingsView -> Frame().GetWidth() - EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
																		 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 131 + 43),
																  0);
	m_opColorKeyHueLabel = EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
																			  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 3,
																			  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 35,
																			  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 3 + EM_METRICS_NATIVE_LABEL_HEIGHT),
																	   "Value:", 0);
	m_opColorKeyHueTrackbarControl = EMFactory::Instance() -> CreateTrackbarControl(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 40,
																								  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP,
																								  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 40 + 100,
																								  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + EM_METRICS_NATIVE_TRACKBAR_CONTROL_HEIGHT),
																						   "Trackbar", TRACKBAR_CHANGED, 0, 0);
	m_opColorKeyHueTextControl = EMFactory::Instance() -> CreateTextControl(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 150,
																						  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP,
																						  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 150 + 25,
																						  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + EM_METRICS_NATIVE_TEXT_CONTROL_HEIGHT),
																				   "0", TEXT_CONTROL_CHANGED, 0, false, EM_ALIGNMENT_RIGHT, true);
	m_opColorKeyHuePreviewView = EMFactory::Instance() -> CreateView(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 190,
																				EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP,
																				m_opColorKeyHueView -> Frame().GetWidth() - EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
																				m_opColorKeyHueView -> Frame().GetHeight() - EM_METRICS_NATIVE_GROUP_BOX_MARGIN_B0TTOM),
																		 0);
	m_opColorKeySettingsView -> AddChild(m_opColorKeyHueGroupBox);
	m_opColorKeySettingsView -> AddChild(m_opColorKeyHueView);
	m_opColorKeyHueView -> AddChild(m_opColorKeyHueLabel);
	m_opColorKeyHueView -> AddChild(m_opColorKeyHueTrackbarControl);
	m_opColorKeyHueView -> AddChild(m_opColorKeyHueTextControl);
	m_opColorKeyHueView -> AddChild(m_opColorKeyHuePreviewView);

	m_opColorKeyLuminanceGroupBox = EMFactory::Instance() -> CreateGroupBox(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
																				 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 178,
																				 m_opColorKeySettingsView -> Frame().GetWidth() - EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
																				 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 178 + 43),
																		  "Luminance", 0);
	m_opColorKeyLuminanceView = EMFactory::Instance() -> CreateView(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
																		 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 178,
																		 m_opColorKeySettingsView -> Frame().GetWidth() - EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
																		 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 178 + 43),
																  0);
	m_opColorKeyLuminanceLabel = EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
																			  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 3,
																			  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 35,
																			  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 3 + EM_METRICS_NATIVE_LABEL_HEIGHT),
																	   "Value:", 0);
	m_opColorKeyLuminanceTrackbarControl = EMFactory::Instance() -> CreateTrackbarControl(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 40,
																								  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP,
																								  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 40 + 100,
																								  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + EM_METRICS_NATIVE_TRACKBAR_CONTROL_HEIGHT),
																						   "Trackbar", TRACKBAR_CHANGED, 0, 0);
	m_opColorKeyLuminanceTextControl = EMFactory::Instance() -> CreateTextControl(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 150,
																						  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP,
																						  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 150 + 25,
																						  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + EM_METRICS_NATIVE_TEXT_CONTROL_HEIGHT),
																				   "0", TEXT_CONTROL_CHANGED, 0, false, EM_ALIGNMENT_RIGHT, true);
	m_opColorKeySettingsView -> AddChild(m_opColorKeyLuminanceGroupBox);
	m_opColorKeySettingsView -> AddChild(m_opColorKeyLuminanceView);
	m_opColorKeyLuminanceView -> AddChild(m_opColorKeyLuminanceLabel);
	m_opColorKeyLuminanceView -> AddChild(m_opColorKeyLuminanceTrackbarControl);
	m_opColorKeyLuminanceView -> AddChild(m_opColorKeyLuminanceTextControl);

	m_opColorKeyToleranceLabel = EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
																			  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 228,
																			  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 50,
																			  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 228 + EM_METRICS_NATIVE_LABEL_HEIGHT),
																	   "Tolerance:", 0);
	m_opColorKeyToleranceTrackbarControl = EMFactory::Instance() -> CreateTrackbarControl(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 55,
																								  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 225,
																								  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 55 + 100,
																								  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 225 + EM_METRICS_NATIVE_TRACKBAR_CONTROL_HEIGHT),
																						   "Trackbar", TRACKBAR_CHANGED, 0, 0);
	m_opColorKeyToleranceTextControl = EMFactory::Instance() -> CreateTextControl(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 165,
																						  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 225,
																						  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 165 + 25,
																						  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 225 + EM_METRICS_NATIVE_TEXT_CONTROL_HEIGHT),
																				   "0", TEXT_CONTROL_CHANGED, 0, false, EM_ALIGNMENT_RIGHT, true);
	m_opColorKeyInvertCheckBox = EMFactory::Instance() ->CreateCheckBox(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
																					  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 250,
																					  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 95,
																					  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 250 + EM_METRICS_NATIVE_CHECKBOX_HEIGHT),
																				"Invert", COLOR_KEY_INVERT_CHECKBOX, 0, 0);
	m_opColorKeySwapCheckBox = EMFactory::Instance() -> CreateCheckBox(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
																					  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 250 + EM_METRICS_NATIVE_CHECKBOX_HEIGHT + 1,
																					  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 95,
																					  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 250 + EM_METRICS_NATIVE_CHECKBOX_HEIGHT + 1 + EM_METRICS_NATIVE_CHECKBOX_HEIGHT),
																				"Swap Direction", SWAP_CHECKBOX, 0, 0);
	m_opColorKeyDefaultButton = EMFactory::Instance() -> CreateButton(EMRect(m_opColorKeySettingsView -> Frame().GetWidth() - EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE - EM_METRICS_NATIVE_BUTTON_WIDTH,
																m_opColorKeySettingsView -> Frame().GetHeight() - EM_METRICS_NATIVE_GROUP_BOX_MARGIN_B0TTOM - EM_METRICS_NATIVE_BUTTON_HEIGHT,
																m_opColorKeySettingsView -> Frame().GetWidth() - EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
																m_opColorKeySettingsView -> Frame().GetHeight() - EM_METRICS_NATIVE_GROUP_BOX_MARGIN_B0TTOM),
														 "Default", BUTTON_DEFAULT, 0, 0);

	m_opColorKeySettingsView -> AddChild(m_opColorKeyToleranceLabel);
	m_opColorKeySettingsView -> AddChild(m_opColorKeyToleranceTrackbarControl);
	m_opColorKeySettingsView -> AddChild(m_opColorKeyToleranceTextControl);
	m_opColorKeySettingsView -> AddChild(m_opColorKeyInvertCheckBox);
	m_opColorKeySettingsView -> AddChild(m_opColorKeySwapCheckBox);
	m_opColorKeySettingsView -> AddChild(m_opColorKeyDefaultButton);

}
TransitionDialog::~TransitionDialog()
{
	delete m_opDialogWindow;
	delete m_opMainView;
	delete m_opComboBox;
	delete m_opDefaultButton;
	delete m_opOK_Button;
	delete m_opCancelButton;

	delete m_opBorderGroupBox;
	delete m_opBorderView;
	delete m_opSettingsView;
	delete m_opOffsetGroupBox;
	delete m_opOffsetView;
	delete m_opReplicateGroupBox;
	delete m_opReplicateView;
	delete m_opSwapCheckBox;

	delete m_opColorLabel;
	delete m_opSoftnessLabel;
	delete m_opWidthLabel;
	delete m_opRLabel;
	delete m_opRTextControl;
	delete m_opGLabel;
	delete m_opGTextControl;
	delete m_opBLabel;
	delete m_opBTextControl;
	delete m_opSoftnessTextControl;
	delete m_opWidthTextControl;

	delete m_opOffXLabel;
	delete m_opOffXTextControl;
	delete m_opOffYLabel;
	delete m_opOffYTextControl;

	delete m_opRepXLabel;
	delete m_opRepXTextControl;
	delete m_opRepYLabel;
	delete m_opRepYTextControl;

	delete m_opColorKeySettingsView;
	delete m_opColorKeyUseColorRadioButton;
	delete m_opColorKeyUseHueRadioButton;
	delete m_opColorKeyUseLuminanceRadioButton;
	delete m_opColorKeyUseBlueAndGreenScreenRadioButton;
	delete m_opColorKeyColorGroupBox;
	delete m_opColorKeyColorView;
	delete m_opColorKeyColorRedLabel;
	delete m_opColorKeyColorGreenLabel;
	delete m_opColorKeyColorBlueLabel;
	delete m_opColorKeyColorRedTrackbarControl;
	delete m_opColorKeyColorGreenTrackbarControl;
	delete m_opColorKeyColorBlueTrackbarControl;
	delete m_opColorKeyColorRedTextControl;
	delete m_opColorKeyColorGreenTextControl;
	delete m_opColorKeyColorBlueTextControl;
	delete m_opColorKeyColorPreviewView;

	delete m_opColorKeyHueGroupBox;
	delete m_opColorKeyHueView;
	delete m_opColorKeyHueLabel;
	delete m_opColorKeyHueTrackbarControl;
	delete m_opColorKeyHueTextControl;
	delete m_opColorKeyHuePreviewView;

	delete m_opColorKeyLuminanceGroupBox;
	delete m_opColorKeyLuminanceView;
	delete m_opColorKeyLuminanceLabel;
	delete m_opColorKeyLuminanceTrackbarControl;
	delete m_opColorKeyLuminanceTextControl;

	delete m_opColorKeyToleranceLabel;
	delete m_opColorKeyToleranceTrackbarControl;
	delete m_opColorKeyToleranceTextControl;
	delete m_opColorKeyInvertCheckBox;
	delete m_opColorKeySwapCheckBox;
	delete m_opColorKeyDefaultButton;
}

void TransitionDialog::AddList(list<string*>* p_opTransitionList)
{
	list<string*>::iterator oIterator = p_opTransitionList -> begin();

	while(oIterator != p_opTransitionList -> end())
	{
		m_opComboBox -> AddString((*oIterator) -> c_str());
		oIterator++;
	}
}

void TransitionDialog::DisplayDialog()
{
//	char vTemp[6];
	int32 vClipID(m_opTransitionClip -> GetMediaClipID());
	m_vTransitionID = *static_cast<int32*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_VIDEO_TRANSITION, &vClipID, NULL, NULL));
	m_opComboBox -> SelectItem(m_opComboBox -> FindString(m_oTransitionIDStringMap[m_vTransitionID].c_str()));
	m_opComboBox -> Notify(COMBO_BOX_TRANSITIONS);
	if(m_opDialogWindow -> Lock())
	{
		m_opDialogWindow -> Show();
#ifdef PLATFORM_WINDOWS
		EnableWindow(dynamic_cast<EMWinWindow*>(m_opDialogWindow) -> GetMainWindowHandle(), FALSE);
		SetFocus(static_cast<HWND>(m_opComboBox -> GetNativeView()));
#endif
		m_opDialogWindow -> Unlock();
	}
}

void TransitionDialog::EnableColorKeyColorSettings(bool p_vEnable)
{
	m_opColorKeyColorGroupBox			  -> Enable(p_vEnable);
	m_opColorKeyColorRedLabel			  -> Enable(p_vEnable);
	m_opColorKeyColorGreenLabel			  -> Enable(p_vEnable);
	m_opColorKeyColorBlueLabel			  -> Enable(p_vEnable);
	m_opColorKeyColorRedTrackbarControl	  -> Enable(p_vEnable);
	m_opColorKeyColorGreenTrackbarControl -> Enable(p_vEnable);
	m_opColorKeyColorBlueTrackbarControl  -> Enable(p_vEnable);
	m_opColorKeyColorRedTextControl		  -> Enable(p_vEnable);
	m_opColorKeyColorGreenTextControl	  -> Enable(p_vEnable);
	m_opColorKeyColorBlueTextControl	  -> Enable(p_vEnable);
}

void TransitionDialog::EnableColorKeyHueSettings(bool p_vEnable)
{
	m_opColorKeyHueGroupBox			-> Enable(p_vEnable);
	m_opColorKeyHueLabel			-> Enable(p_vEnable);
	m_opColorKeyHueTrackbarControl	-> Enable(p_vEnable);
	m_opColorKeyHueTextControl		-> Enable(p_vEnable);
}

void TransitionDialog::EnableColorKeyLuminanceSettings(bool p_vEnable)
{
	m_opColorKeyLuminanceGroupBox		 -> Enable(p_vEnable);
	m_opColorKeyLuminanceLabel			 -> Enable(p_vEnable);
	m_opColorKeyLuminanceTrackbarControl -> Enable(p_vEnable);
	m_opColorKeyLuminanceTextControl	 -> Enable(p_vEnable);
}

void TransitionDialog::EnableColorKeyToleranceSettings(bool p_vEnable)
{
	m_opColorKeyToleranceLabel			 -> Enable(p_vEnable);
	m_opColorKeyToleranceTrackbarControl -> Enable(p_vEnable);
	m_opColorKeyToleranceTextControl	 -> Enable(p_vEnable);
}

void TransitionDialog::EnableColorKeySettings(bool p_vEnable)
{
	EnableColorKeyColorSettings(p_vEnable);
	EnableColorKeyHueSettings(p_vEnable);
	EnableColorKeyLuminanceSettings(p_vEnable);
	EnableColorKeyToleranceSettings(p_vEnable);
//	m_opColorKeyInvertCheckBox			 -> Enable(p_vEnable);
}

void TransitionDialog::EnableSettings(bool p_vEnable)
{
	m_opBorderGroupBox		-> Enable(p_vEnable);
	m_opColorLabel			-> Enable(p_vEnable);
	m_opRLabel				-> Enable(p_vEnable);
	m_opRTextControl		-> Enable(p_vEnable);
	m_opGLabel				-> Enable(p_vEnable);
	m_opGTextControl		-> Enable(p_vEnable);
	m_opBLabel				-> Enable(p_vEnable);
	m_opBTextControl		-> Enable(p_vEnable);
	m_opSoftnessLabel		-> Enable(p_vEnable);
	m_opSoftnessTextControl -> Enable(p_vEnable);
	m_opWidthLabel			-> Enable(p_vEnable);
	m_opWidthTextControl	-> Enable(p_vEnable);
	m_opOffsetGroupBox		-> Enable(p_vEnable);
	m_opOffXLabel			-> Enable(p_vEnable);
	m_opOffXTextControl		-> Enable(p_vEnable);
	m_opOffYLabel			-> Enable(p_vEnable);
	m_opOffYTextControl		-> Enable(p_vEnable);
	m_opReplicateGroupBox	-> Enable(p_vEnable);
	m_opRepXLabel			-> Enable(p_vEnable);
	m_opRepXTextControl		-> Enable(p_vEnable);
	m_opRepYLabel			-> Enable(p_vEnable);
	m_opRepYTextControl		-> Enable(p_vEnable);
//	m_opSwapCheckBox		-> Enable(p_vEnable);
}

void TransitionDialog::HideDialog()
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

void TransitionDialog::Init()
{
	m_opDialogWindow -> AddListener(this);
	m_opMainView -> AddListener(this);
	m_opSettingsView -> AddListener(this);
	m_opBorderView -> AddListener(this);
	m_opOffsetView -> AddListener(this);
	m_opReplicateView -> AddListener(this);
	m_opColorKeySettingsView -> AddListener(this);
	m_opColorKeyColorView -> AddListener(this);
	m_opColorKeyHueView -> AddListener(this);
	m_opColorKeyLuminanceView -> AddListener(this);
//	m_opColorKeyHuePreviewView -> AddListener(this);

	if(m_opDialogWindow -> Lock())
	{
		//m_opMainView -> SetViewColor(EMColor(192, 192, 192, 255));
		m_opMainView -> SetDefaultColor();
		//m_opMainView -> SetViewColor(EMColor(192, 192, 192, 255));
		m_opSettingsView -> SetDefaultColor();
		//m_opBorderView -> SetViewColor(EMColor(192, 192, 192, 255));
		m_opBorderView -> SetDefaultColor();
		//m_opOffsetView -> SetViewColor(EMColor(192, 192, 192, 255));
		m_opOffsetView -> SetDefaultColor();
		//m_opReplicateView -> SetViewColor(EMColor(192, 192, 192, 255));
		m_opReplicateView -> SetDefaultColor();
		//m_opColorKeySettingsView -> SetViewColor(EMColor(0, 192, 0, 255));
		m_opColorKeySettingsView -> SetDefaultColor();
		//m_opColorKeyColorView -> SetViewColor(EMColor(192, 192, 0, 255));
		m_opColorKeyColorView -> SetDefaultColor();
		//m_opColorKeyHueView -> SetViewColor(EMColor(0, 0, 192, 255));
		m_opColorKeyHueView -> SetDefaultColor();
		//m_opColorKeyLuminanceView -> SetViewColor(EMColor(0, 192, 192, 255));
		m_opColorKeyLuminanceView -> SetDefaultColor();

		m_opDialogWindow -> Unlock();
	}
	m_opComboBox -> AddListener(this);
	m_opDefaultButton -> AddListener(this);
	m_opOK_Button -> AddListener(this);
	m_opCancelButton -> AddListener(this);

	m_opRTextControl		-> AddListener(this);
	m_opGTextControl		-> AddListener(this);
	m_opBTextControl		-> AddListener(this);
	m_opSoftnessTextControl -> AddListener(this);
	m_opWidthTextControl	-> AddListener(this);
	m_opOffXTextControl		-> AddListener(this);
	m_opOffYTextControl		-> AddListener(this);
	m_opRepXTextControl		-> AddListener(this);
	m_opRepYTextControl		-> AddListener(this);
	m_opSwapCheckBox		-> AddListener(this);

	m_opColorKeyUseColorRadioButton				 -> AddListener(this);
	m_opColorKeyUseHueRadioButton				 -> AddListener(this);
	m_opColorKeyUseLuminanceRadioButton			 -> AddListener(this);
	m_opColorKeyUseBlueAndGreenScreenRadioButton -> AddListener(this);
	m_opColorKeyColorRedTrackbarControl			 -> AddListener(this);
	m_opColorKeyColorGreenTrackbarControl		 -> AddListener(this);
	m_opColorKeyColorBlueTrackbarControl		 -> AddListener(this);
	m_opColorKeyColorRedTextControl				 -> AddListener(this);
	m_opColorKeyColorGreenTextControl			 -> AddListener(this);
	m_opColorKeyColorBlueTextControl			 -> AddListener(this);
	m_opColorKeyColorPreviewView				 -> AddListener(this);
	m_opColorKeyHueTrackbarControl				 -> AddListener(this);
	m_opColorKeyHueTextControl					 -> AddListener(this);
	m_opColorKeyLuminanceTrackbarControl		 -> AddListener(this);
	m_opColorKeyLuminanceTextControl			 -> AddListener(this);
	m_opColorKeyToleranceTrackbarControl		 -> AddListener(this);
	m_opColorKeyToleranceTextControl			 -> AddListener(this);
	m_opColorKeyInvertCheckBox					 -> AddListener(this);
	m_opColorKeySwapCheckBox					 -> AddListener(this);
	m_opColorKeyDefaultButton					 -> AddListener(this);

	//Setting TabOrder
	m_opDialogWindow -> AddTabComponent(m_opComboBox);
	m_opDialogWindow -> AddTabComponent(m_opRTextControl);
	m_opDialogWindow -> AddTabComponent(m_opGTextControl);
	m_opDialogWindow -> AddTabComponent(m_opBTextControl);
	m_opDialogWindow -> AddTabComponent(m_opSoftnessTextControl);
	m_opDialogWindow -> AddTabComponent(m_opWidthTextControl);
	m_opDialogWindow -> AddTabComponent(m_opOffXTextControl);
	m_opDialogWindow -> AddTabComponent(m_opOffYTextControl);
	m_opDialogWindow -> AddTabComponent(m_opRepXTextControl);
	m_opDialogWindow -> AddTabComponent(m_opRepYTextControl);
	m_opDialogWindow -> AddTabComponent(m_opSwapCheckBox);
	m_opDialogWindow -> AddTabComponent(m_opDefaultButton);

	m_opDialogWindow -> AddTabComponent(m_opColorKeyUseColorRadioButton);
	m_opDialogWindow -> AddTabComponent(m_opColorKeyUseHueRadioButton);
	m_opDialogWindow -> AddTabComponent(m_opColorKeyUseLuminanceRadioButton);
	m_opDialogWindow -> AddTabComponent(m_opColorKeyUseBlueAndGreenScreenRadioButton);
	m_opDialogWindow -> AddTabComponent(m_opColorKeyColorRedTrackbarControl);
	m_opDialogWindow -> AddTabComponent(m_opColorKeyColorGreenTrackbarControl);
	m_opDialogWindow -> AddTabComponent(m_opColorKeyColorBlueTrackbarControl);
	m_opDialogWindow -> AddTabComponent(m_opColorKeyColorRedTextControl);
	m_opDialogWindow -> AddTabComponent(m_opColorKeyColorGreenTextControl);
	m_opDialogWindow -> AddTabComponent(m_opColorKeyColorBlueTextControl);
	m_opDialogWindow -> AddTabComponent(m_opColorKeyHueTrackbarControl);
	m_opDialogWindow -> AddTabComponent(m_opColorKeyHueTextControl);
	m_opDialogWindow -> AddTabComponent(m_opColorKeyLuminanceTrackbarControl);
	m_opDialogWindow -> AddTabComponent(m_opColorKeyLuminanceTextControl);
	m_opDialogWindow -> AddTabComponent(m_opColorKeyToleranceTrackbarControl);
	m_opDialogWindow -> AddTabComponent(m_opColorKeyInvertCheckBox);
	m_opDialogWindow -> AddTabComponent(m_opColorKeySwapCheckBox);
	m_opDialogWindow -> AddTabComponent(m_opColorKeyToleranceTextControl);
	m_opDialogWindow -> AddTabComponent(m_opColorKeyDefaultButton);

	m_opDialogWindow -> AddTabComponent(m_opOK_Button);
	m_opDialogWindow -> AddTabComponent(m_opCancelButton);

	m_opSettingsView -> Hide();
	m_opColorKeySettingsView -> Hide();


	int32 vNumberOfTransitions(*static_cast<int32*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_NUMBER_OF_VIDEO_TRANSITIONS, NULL, NULL, NULL)));
	uint32* vpTransitionIDs = EM_new uint32[sizeof(uint32) * vNumberOfTransitions];
	EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_VIDEO_TRANSITION_IDS, vpTransitionIDs, NULL, NULL);
	
	for(int i = 0; i < vNumberOfTransitions; i++)
	{
		string* opTempString = static_cast<string*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_VIDEO_TRANSITION_NAME, &vpTransitionIDs[i]));
		m_opComboBox -> AddString(opTempString -> c_str());
		m_oTransitionStringIDMap[opTempString -> c_str()] = vpTransitionIDs[i];
		m_oTransitionIDStringMap[vpTransitionIDs[i]] = opTempString -> c_str();
	}
	m_opComboBox -> SelectItem(0);
	m_vTransitionID = m_oTransitionStringIDMap[m_opComboBox -> GetSelection().c_str()];
	delete [] vpTransitionIDs;

	m_opColorKeyColorRedTrackbarControl -> SetRange(0, 255);
	m_opColorKeyColorGreenTrackbarControl -> SetRange(0, 255);
	m_opColorKeyColorBlueTrackbarControl -> SetRange(0, 255);
	m_opColorKeyHueTrackbarControl -> SetRange(0, 360);
	m_opColorKeyLuminanceTrackbarControl -> SetRange(0, 100);
	m_opColorKeyToleranceTrackbarControl -> SetRange(0, 100);
}

bool TransitionDialog::MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage)
{
	switch(p_vMessage)
	{
/*	case EM_LIST_BOX_DOUBLE_CLICKED:
		if(p_opSender != m_opComboBox)
			return false;
		eo << "DoubleClicked ListBox" << ef;
*/	case BUTTON_DEFAULT:
		{
			eo << "BUTTON_DEFAULT" << ef;
//			char vTemp[6];
			EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_DEFAULT_TRANSITION_SETTINGS, &m_vTransitionID, &m_oTransitionParameters, NULL);

			switch(m_oTransitionParameters.m_vID)
			{
			case EM_TRANSITION_DX:
			case EM_TRANSITION_FADER:
				m_opColorKeySettingsView -> Hide();
				m_opSettingsGroupBox -> SetFrame(m_opSettingsView -> Frame());
				EnableSettings(false);
				m_opSettingsView -> Show();
				m_opSwapCheckBox -> Check(m_oTransitionParameters.m_vSwap);
				break;
			case EM_TRANSITION_WIPE:
				{
					char vTemp[6];
					m_opColorKeySettingsView -> Hide();
					m_opSettingsGroupBox -> SetFrame(m_opSettingsView -> Frame());
					m_opRTextControl		-> SetText(itoa(m_oTransitionParameters.m_vParams.m_vWipe.m_vR, vTemp, 10));
					m_opGTextControl		-> SetText(itoa(m_oTransitionParameters.m_vParams.m_vWipe.m_vG, vTemp, 10));
					m_opBTextControl		-> SetText(itoa(m_oTransitionParameters.m_vParams.m_vWipe.m_vB, vTemp, 10));
					m_opSoftnessTextControl -> SetText(itoa(m_oTransitionParameters.m_vParams.m_vWipe.m_vSoftness, vTemp, 10));
					m_opWidthTextControl	-> SetText(itoa(m_oTransitionParameters.m_vParams.m_vWipe.m_vWidth, vTemp, 10));
					m_opOffXTextControl		-> SetText(itoa(m_oTransitionParameters.m_vParams.m_vWipe.m_vOffsetX, vTemp, 10));
					m_opOffYTextControl		-> SetText(itoa(m_oTransitionParameters.m_vParams.m_vWipe.m_vOffsetY, vTemp, 10));
					m_opRepXTextControl		-> SetText(itoa(m_oTransitionParameters.m_vParams.m_vWipe.m_vReplicateX, vTemp, 10));
					m_opRepYTextControl		-> SetText(itoa(m_oTransitionParameters.m_vParams.m_vWipe.m_vReplicateY, vTemp, 10));
					m_opSwapCheckBox -> Check(m_oTransitionParameters.m_vSwap);
					EnableSettings(true);
					m_opSettingsView -> Show();
					break;
				}
			case EM_TRANSITION_KEY:
				m_opSettingsView -> Hide();
				m_opSettingsGroupBox -> SetFrame(m_opColorKeySettingsView -> Frame());
				switch(m_oTransitionParameters.m_vParams.m_vKey.m_vColorID)
				{
				case EM_KEY_COLOR:
					m_opColorKeyUseColorRadioButton -> Check(true);
					m_opColorKeyUseHueRadioButton -> Check(false);
					m_opColorKeyUseLuminanceRadioButton -> Check(false);
					m_opColorKeyUseBlueAndGreenScreenRadioButton -> Check(false);
					break;
				case EM_KEY_HUE:
					m_opColorKeyUseColorRadioButton -> Check(false);
					m_opColorKeyUseHueRadioButton -> Check(true);
					m_opColorKeyUseLuminanceRadioButton -> Check(false);
					m_opColorKeyUseBlueAndGreenScreenRadioButton -> Check(false);
					break;
				case EM_KEY_LUMINANCE:
					m_opColorKeyUseColorRadioButton -> Check(false);
					m_opColorKeyUseHueRadioButton -> Check(false);
					m_opColorKeyUseLuminanceRadioButton -> Check(true);
					m_opColorKeyUseBlueAndGreenScreenRadioButton -> Check(false);
					break;
				case EM_KEY_BLUE:
					m_opColorKeyUseColorRadioButton -> Check(false);
					m_opColorKeyUseHueRadioButton -> Check(false);
					m_opColorKeyUseLuminanceRadioButton -> Check(false);
					m_opColorKeyUseBlueAndGreenScreenRadioButton -> Check(true);
					break;
				case EM_KEY_ALPHA:
				default:
					EMDebugger("This ColorID is not implemented yet!");

				}
				m_opColorKeyUseColorRadioButton -> Notify(COLOR_KEY_RADIO_BUTTON_CHANGED);
				m_opColorKeyColorRedTrackbarControl -> SetPosition(m_oTransitionParameters.m_vParams.m_vKey.m_vR);
				m_opColorKeyColorGreenTrackbarControl -> SetPosition(m_oTransitionParameters.m_vParams.m_vKey.m_vG);
				m_opColorKeyColorBlueTrackbarControl -> SetPosition(m_oTransitionParameters.m_vParams.m_vKey.m_vB);
				m_opColorKeyHueTrackbarControl -> SetPosition(m_oTransitionParameters.m_vParams.m_vKey.m_vHue);
				m_opColorKeyLuminanceTrackbarControl -> SetPosition(m_oTransitionParameters.m_vParams.m_vKey.m_vLuminance);
				m_opColorKeyToleranceTrackbarControl -> SetPosition(m_oTransitionParameters.m_vParams.m_vKey.m_vTolerance);
				m_opColorKeyInvertCheckBox -> Check(m_oTransitionParameters.m_vParams.m_vKey.m_vInvert);
				m_opColorKeySwapCheckBox -> Check(m_oTransitionParameters.m_vSwap);
				m_opColorKeySettingsView -> Show();
				break;
			}
			return true;
		}
	case EM_GUI_COMPONENT_ENTER_PRESSED:
	case BUTTON_OK:
		{
			eo << "OK button pressed" << ef;

			uint32 vMediaClipID = m_opTransitionClip -> GetMediaClipID();
			
			switch(m_oTransitionParameters.m_vID)
			{
			case EM_TRANSITION_DX:
			case EM_TRANSITION_FADER:
				m_oTransitionParameters.m_vSwap = m_opSwapCheckBox -> IsChecked();
				break;
			case EM_TRANSITION_WIPE:
				m_oTransitionParameters.m_vParams.m_vWipe.m_vR = atoi(m_opRTextControl -> Text());
				m_oTransitionParameters.m_vParams.m_vWipe.m_vG = atoi(m_opGTextControl -> Text());	
				m_oTransitionParameters.m_vParams.m_vWipe.m_vB = atoi(m_opBTextControl -> Text());
				m_oTransitionParameters.m_vParams.m_vWipe.m_vSoftness = atoi(m_opSoftnessTextControl -> Text());
				m_oTransitionParameters.m_vParams.m_vWipe.m_vWidth = atoi(m_opWidthTextControl -> Text());
				m_oTransitionParameters.m_vParams.m_vWipe.m_vOffsetX = atoi(m_opOffXTextControl -> Text());
				m_oTransitionParameters.m_vParams.m_vWipe.m_vOffsetY = atoi(m_opOffYTextControl -> Text());
				m_oTransitionParameters.m_vParams.m_vWipe.m_vReplicateX = atoi(m_opRepXTextControl -> Text());
				m_oTransitionParameters.m_vParams.m_vWipe.m_vReplicateY = atoi(m_opRepYTextControl -> Text());
				m_oTransitionParameters.m_vSwap = m_opSwapCheckBox -> IsChecked();
				break;
			case EM_TRANSITION_KEY:
				if(m_opColorKeyUseColorRadioButton -> IsChecked())
					m_oTransitionParameters.m_vParams.m_vKey.m_vColorID = EM_KEY_COLOR;
				else if(m_opColorKeyUseHueRadioButton -> IsChecked())
					m_oTransitionParameters.m_vParams.m_vKey.m_vColorID = EM_KEY_HUE;
				else if(m_opColorKeyUseLuminanceRadioButton -> IsChecked())
					m_oTransitionParameters.m_vParams.m_vKey.m_vColorID = EM_KEY_LUMINANCE;
				else if(m_opColorKeyUseBlueAndGreenScreenRadioButton -> IsChecked())
					m_oTransitionParameters.m_vParams.m_vKey.m_vColorID = EM_KEY_BLUE;
				m_oTransitionParameters.m_vParams.m_vKey.m_vR = m_opColorKeyColorRedTrackbarControl -> GetPosition();
				m_oTransitionParameters.m_vParams.m_vKey.m_vG = m_opColorKeyColorGreenTrackbarControl -> GetPosition();
				m_oTransitionParameters.m_vParams.m_vKey.m_vB = m_opColorKeyColorBlueTrackbarControl -> GetPosition();
				m_oTransitionParameters.m_vParams.m_vKey.m_vHue = m_opColorKeyHueTrackbarControl -> GetPosition();
				m_oTransitionParameters.m_vParams.m_vKey.m_vLuminance = m_opColorKeyLuminanceTrackbarControl -> GetPosition();
				m_oTransitionParameters.m_vParams.m_vKey.m_vTolerance = m_opColorKeyToleranceTrackbarControl -> GetPosition();
				m_oTransitionParameters.m_vParams.m_vKey.m_vInvert = m_opColorKeyInvertCheckBox -> IsChecked();
				m_oTransitionParameters.m_vSwap = m_opColorKeySwapCheckBox -> IsChecked();
				m_opColorKeySettingsView -> Show();
				break;
			}
		
			string oSelection = m_opComboBox -> GetSelection().c_str();
			int32 vTransitionID = m_oTransitionStringIDMap[oSelection.c_str()];
			EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_SET_VIDEO_TRANSITION, &vMediaClipID, &vTransitionID);
			EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_SET_PERSIST_TRANSITION_SETTINGS, &vMediaClipID, &m_oTransitionParameters, NULL);
			m_opTransitionClip -> SetLabel(const_cast<char*>(oSelection.c_str()), m_opTrackView);
			if(m_opTrackView -> LockWindow())
			{
				m_opTrackView -> Invalidate(m_opTransitionClip -> Frame());
				m_opTrackView -> UnlockWindow();
			}
			HideDialog();
			return true;
		}
	case EM_GUI_COMPONENT_ESC_PRESSED:
	case BUTTON_CANCEL:
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
			m_opComboBox -> SetFocus();
		m_opDialogWindow -> SetCurrentFocus();
		return true;
	case COMBO_BOX_TRANSITIONS:
		{
			eo << "EM_LIST_BOX_TRANSITIONS: Selection made" << ef;
			eo << (char*)m_opComboBox -> GetSelection().c_str() << ef;
			uint32 vMediaClipID = m_opTransitionClip -> GetMediaClipID();
			m_vTransitionID = m_oTransitionStringIDMap[m_opComboBox -> GetSelection().c_str()];
			if(m_vTransitionID == *static_cast<uint32*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_VIDEO_TRANSITION, &vMediaClipID)))
				EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_PERSIST_TRANSITION_SETTINGS, &vMediaClipID, &m_oTransitionParameters, NULL);
			else
				EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_DEFAULT_TRANSITION_SETTINGS, &m_vTransitionID, &m_oTransitionParameters, NULL);

			switch(m_oTransitionParameters.m_vID)
			{
			case EM_TRANSITION_DX:
			case EM_TRANSITION_FADER:
				m_opColorKeySettingsView -> Hide();
				m_opSettingsGroupBox -> SetFrame(m_opSettingsView -> Frame());
				EnableSettings(false);
				m_opSettingsView -> Show();
				m_opSwapCheckBox -> Check(m_oTransitionParameters.m_vSwap);
				break;
			case EM_TRANSITION_WIPE:
				{
					char vTemp[6];
					m_opColorKeySettingsView -> Hide();
					m_opSettingsGroupBox -> SetFrame(m_opSettingsView -> Frame());
					m_opRTextControl		-> SetText(itoa(m_oTransitionParameters.m_vParams.m_vWipe.m_vR, vTemp, 10));
					m_opGTextControl		-> SetText(itoa(m_oTransitionParameters.m_vParams.m_vWipe.m_vG, vTemp, 10));
					m_opBTextControl		-> SetText(itoa(m_oTransitionParameters.m_vParams.m_vWipe.m_vB, vTemp, 10));
					m_opSoftnessTextControl -> SetText(itoa(m_oTransitionParameters.m_vParams.m_vWipe.m_vSoftness, vTemp, 10));
					m_opWidthTextControl	-> SetText(itoa(m_oTransitionParameters.m_vParams.m_vWipe.m_vWidth, vTemp, 10));
					m_opOffXTextControl		-> SetText(itoa(m_oTransitionParameters.m_vParams.m_vWipe.m_vOffsetX, vTemp, 10));
					m_opOffYTextControl		-> SetText(itoa(m_oTransitionParameters.m_vParams.m_vWipe.m_vOffsetY, vTemp, 10));
					m_opRepXTextControl		-> SetText(itoa(m_oTransitionParameters.m_vParams.m_vWipe.m_vReplicateX, vTemp, 10));
					m_opRepYTextControl		-> SetText(itoa(m_oTransitionParameters.m_vParams.m_vWipe.m_vReplicateY, vTemp, 10));
					m_opSwapCheckBox -> Check(m_oTransitionParameters.m_vSwap);
					EnableSettings(true);
					m_opSettingsView -> Show();
					break;
				}
			case EM_TRANSITION_KEY:
				m_opSettingsView -> Hide();
				m_opSettingsGroupBox -> SetFrame(m_opColorKeySettingsView -> Frame());
				switch(m_oTransitionParameters.m_vParams.m_vKey.m_vColorID)
				{
				case EM_KEY_COLOR:
					m_opColorKeyUseColorRadioButton -> Check(true);
					m_opColorKeyUseHueRadioButton -> Check(false);
					m_opColorKeyUseLuminanceRadioButton -> Check(false);
					m_opColorKeyUseBlueAndGreenScreenRadioButton -> Check(false);
					break;
				case EM_KEY_HUE:
					m_opColorKeyUseColorRadioButton -> Check(false);
					m_opColorKeyUseHueRadioButton -> Check(true);
					m_opColorKeyUseLuminanceRadioButton -> Check(false);
					m_opColorKeyUseBlueAndGreenScreenRadioButton -> Check(false);
					break;
				case EM_KEY_LUMINANCE:
					m_opColorKeyUseColorRadioButton -> Check(false);
					m_opColorKeyUseHueRadioButton -> Check(false);
					m_opColorKeyUseLuminanceRadioButton -> Check(true);
					m_opColorKeyUseBlueAndGreenScreenRadioButton -> Check(false);
					break;
				case EM_KEY_BLUE:
					m_opColorKeyUseColorRadioButton -> Check(false);
					m_opColorKeyUseHueRadioButton -> Check(false);
					m_opColorKeyUseLuminanceRadioButton -> Check(false);
					m_opColorKeyUseBlueAndGreenScreenRadioButton -> Check(true);
					break;
				case EM_KEY_ALPHA:
					EMDebugger("This ColorID \"EM_KEY_ALPHA\" is not implemented yet!");
					break;
				default:
					EMDebugger("This ColorID \"Unknown\" is not implemented yet!");

				}
				m_opColorKeyUseColorRadioButton -> Notify(COLOR_KEY_RADIO_BUTTON_CHANGED);
				m_opColorKeyColorRedTrackbarControl -> SetPosition(m_oTransitionParameters.m_vParams.m_vKey.m_vR);
				m_opColorKeyColorGreenTrackbarControl -> SetPosition(m_oTransitionParameters.m_vParams.m_vKey.m_vG);
				m_opColorKeyColorBlueTrackbarControl -> SetPosition(m_oTransitionParameters.m_vParams.m_vKey.m_vB);
				m_opColorKeyHueTrackbarControl -> SetPosition(m_oTransitionParameters.m_vParams.m_vKey.m_vHue);
				m_opColorKeyLuminanceTrackbarControl -> SetPosition(m_oTransitionParameters.m_vParams.m_vKey.m_vLuminance);
				m_opColorKeyToleranceTrackbarControl -> SetPosition(m_oTransitionParameters.m_vParams.m_vKey.m_vTolerance);
				m_opColorKeyInvertCheckBox -> Check(m_oTransitionParameters.m_vParams.m_vKey.m_vInvert);
				m_opColorKeySwapCheckBox -> Check(m_oTransitionParameters.m_vSwap);
				m_opColorKeySettingsView -> Show();
				break;
			}

/*			if(m_vTransitionID == *static_cast<uint32*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_VIDEO_TRANSITION, &vMediaClipID)))
				EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_PERSIST_TRANSITION_SETTINGS, &vMediaClipID, m_vpTransitionValues, NULL);
			else
				EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_DEFAULT_TRANSITION_SETTINGS, &m_vTransitionID, m_vpTransitionValues, NULL);

	//		if(()) == NULL)
	//			EnableSettings(false);
	//		else
	//		{
				char vTemp[6];
				m_opRTextControl		-> SetText(itoa(m_vpTransitionValues[0], vTemp, 10));
				if(m_vpTransitionValues[0] != -1)    // FIXME: Only enable
					EnableSettings(true);
				else
					EnableSettings(false);
				m_opGTextControl		-> SetText(itoa(m_vpTransitionValues[1], vTemp, 10));
				m_opBTextControl		-> SetText(itoa(m_vpTransitionValues[2], vTemp, 10));
				m_opSoftnessTextControl -> SetText(itoa(m_vpTransitionValues[3], vTemp, 10));
				m_opWidthTextControl	-> SetText(itoa(m_vpTransitionValues[4], vTemp, 10));
				m_opOffXTextControl		-> SetText(itoa(m_vpTransitionValues[5], vTemp, 10));
				m_opOffYTextControl		-> SetText(itoa(m_vpTransitionValues[6], vTemp, 10));
				m_opRepXTextControl		-> SetText(itoa(m_vpTransitionValues[7], vTemp, 10));
				m_opRepYTextControl		-> SetText(itoa(m_vpTransitionValues[8], vTemp, 10));
				if(/*m_vpTransitionValues[7] != -1 && *m_vpTransitionValues[8] == -1)
				{
					m_opSettingsView -> Hide();
					m_opSettingsGroupBox -> SetFrame(m_opColorKeySettingsView -> Frame());
					m_opColorKeySettingsView -> Show();
				}
				else
				{
					m_opColorKeySettingsView -> Hide();
					m_opSettingsGroupBox -> SetFrame(m_opSettingsView -> Frame());
					m_opSettingsView -> Show();
				}
				if(m_vpTransitionValues[9] > 0)
					m_opSwapCheckBox -> Check(true);
				else
					m_opSwapCheckBox -> Check(false);
*///				EnableSettings(true);
	//		}
			return true;
		}
	case COLOR_KEY_RADIO_BUTTON_CHANGED:
		if(m_opColorKeyUseColorRadioButton -> IsChecked())
		{
			EnableColorKeySettings(true);
			EnableColorKeyHueSettings(false);
			EnableColorKeyLuminanceSettings(false);
		}
		else if(m_opColorKeyUseHueRadioButton -> IsChecked())
		{
			EnableColorKeySettings(true);
			EnableColorKeyColorSettings(false);
			EnableColorKeyLuminanceSettings(false);
			EnableColorKeyToleranceSettings(false);
		}
		else if(m_opColorKeyUseLuminanceRadioButton -> IsChecked())
		{
			EnableColorKeySettings(true);
			EnableColorKeyColorSettings(false);
			EnableColorKeyHueSettings(false);
			EnableColorKeyToleranceSettings(false);
		}
		else if(m_opColorKeyUseBlueAndGreenScreenRadioButton -> IsChecked())
		{
			EnableColorKeySettings(false);
			EnableColorKeyToleranceSettings(true);
		}
		return true;
	case COLOR_KEY_INVERT_CHECKBOX:
	case SWAP_CHECKBOX:
	case TEXT_CONTROL_CHANGED:
		if(m_vAlreadyChanged == false)
		{
			char* vpTemp;
			int32 vStart(0);
			int32 vEnd(0);
			if(p_opSender == m_opColorKeyColorRedTextControl)
			{
				vpTemp = const_cast<char*>(m_opColorKeyColorRedTextControl -> Text());
				vpTemp = SetValidValue(vpTemp, 0, 255);
				m_opColorKeyColorRedTextControl -> GetSelection(vStart, vEnd);
				m_opColorKeyColorRedTextControl -> SetText(vpTemp);
				m_opColorKeyColorRedTrackbarControl -> SetPosition(atoi(vpTemp));
				m_opColorKeyColorRedTextControl -> SetSelection(vStart, vEnd);
				if(m_opDialogWindow -> Lock())
				{
					m_opColorKeyColorPreviewView -> SetViewColor(EMColor(atoi(m_opColorKeyColorRedTextControl -> Text()), atoi(m_opColorKeyColorGreenTextControl -> Text()), atoi(m_opColorKeyColorBlueTextControl -> Text()), 255));
					m_opColorKeyColorPreviewView -> Invalidate(m_opColorKeyColorPreviewView -> Bounds());
					m_opDialogWindow -> Unlock();
				}
//				return true;
			}
			if(p_opSender == m_opColorKeyColorGreenTextControl)
			{
				vpTemp = const_cast<char*>(m_opColorKeyColorGreenTextControl -> Text());
				vpTemp = SetValidValue(vpTemp, 0, 255);
				m_opColorKeyColorGreenTextControl -> GetSelection(vStart, vEnd);
				m_opColorKeyColorGreenTextControl -> SetText(vpTemp);
				m_opColorKeyColorGreenTrackbarControl -> SetPosition(atoi(vpTemp));
				m_opColorKeyColorGreenTextControl -> SetSelection(vStart, vEnd);
				if(m_opDialogWindow -> Lock())
				{
					m_opColorKeyColorPreviewView -> SetViewColor(EMColor(atoi(m_opColorKeyColorRedTextControl -> Text()), atoi(m_opColorKeyColorGreenTextControl -> Text()), atoi(m_opColorKeyColorBlueTextControl -> Text()), 255));
					m_opColorKeyColorPreviewView -> Invalidate(m_opColorKeyColorPreviewView -> Bounds());
					m_opDialogWindow -> Unlock();
				}
//				return true;
			}
			if(p_opSender == m_opColorKeyColorBlueTextControl)
			{
				vpTemp = const_cast<char*>(m_opColorKeyColorBlueTextControl -> Text());
				vpTemp = SetValidValue(vpTemp, 0, 255);
				m_opColorKeyColorBlueTextControl -> GetSelection(vStart, vEnd);
				m_opColorKeyColorBlueTextControl -> SetText(vpTemp);
				m_opColorKeyColorBlueTrackbarControl -> SetPosition(atoi(vpTemp));
				m_opColorKeyColorBlueTextControl -> SetSelection(vStart, vEnd);
				if(m_opDialogWindow -> Lock())
				{
					m_opColorKeyColorPreviewView -> SetViewColor(EMColor(atoi(m_opColorKeyColorRedTextControl -> Text()), atoi(m_opColorKeyColorGreenTextControl -> Text()), atoi(m_opColorKeyColorBlueTextControl -> Text()), 255));
					m_opColorKeyColorPreviewView -> Invalidate(m_opColorKeyColorPreviewView -> Bounds());
					m_opDialogWindow -> Unlock();
				}
//				return true;
			}
			if(p_opSender == m_opColorKeyHueTextControl)
			{
				vpTemp = const_cast<char*>(m_opColorKeyHueTextControl -> Text());
				vpTemp = SetValidValue(vpTemp, 0, 360);
				m_opColorKeyHueTextControl -> GetSelection(vStart, vEnd);
				m_opColorKeyHueTextControl -> SetText(vpTemp);
				m_opColorKeyHueTrackbarControl -> SetPosition(atoi(vpTemp));
				m_opColorKeyHueTextControl -> SetSelection(vStart, vEnd);
				float vHue(static_cast<float>(static_cast<float>(atoi(vpTemp)) / static_cast<float>(60)));
				unsigned char vRGB[3];
				HueToRGB(&vHue, vRGB);
				if(m_opDialogWindow -> Lock())
				{
					m_opColorKeyHuePreviewView -> SetViewColor(EMColor(vRGB[0], vRGB[1], vRGB[2], 255));
					m_opColorKeyHuePreviewView -> Invalidate(m_opColorKeyHuePreviewView -> Bounds());
					m_opDialogWindow -> Unlock();
				}
//				return true;
			}
			if(p_opSender == m_opColorKeyLuminanceTextControl)
			{
				vpTemp = const_cast<char*>(m_opColorKeyLuminanceTextControl -> Text());
				vpTemp = SetValidValue(vpTemp, 0, 100);
				m_opColorKeyLuminanceTextControl -> GetSelection(vStart, vEnd);
				m_opColorKeyLuminanceTextControl -> SetText(vpTemp);
				m_opColorKeyLuminanceTrackbarControl -> SetPosition(atoi(vpTemp));
				m_opColorKeyLuminanceTextControl -> SetSelection(vStart, vEnd);
/*				if(m_opDialogWindow -> Lock())
				{
					m_opColorKeyLuminancePreviewView -> SetViewColor(EMColor(atoi(m_opColorKeyLuminanceTextControl -> Text()), atoi(m_opColorKeyLuminanceGreenTextControl -> Text()), atoi(m_opColorKeyLuminanceBlueTextControl -> Text()), 255));
					m_opColorKeyLuminancePreviewView -> Invalidate(m_opColorKeyLuminancePreviewView -> Bounds());
					m_opDialogWindow -> Unlock();
				}
*///				return true;
			}
			if(p_opSender == m_opColorKeyToleranceTextControl)
			{
				vpTemp = const_cast<char*>(m_opColorKeyToleranceTextControl -> Text());
				vpTemp = SetValidValue(vpTemp, 0, 360);
				m_opColorKeyToleranceTextControl -> GetSelection(vStart, vEnd);
				m_opColorKeyToleranceTextControl -> SetText(vpTemp);
				m_opColorKeyToleranceTrackbarControl -> SetPosition(atoi(vpTemp));
				m_opColorKeyToleranceTextControl -> SetSelection(vStart, vEnd);
/*				if(m_opDialogWindow -> Lock())
				{
					m_opColorKeyTolerancePreviewView -> SetViewColor(EMColor(atoi(m_opColorKeyToleranceTextControl -> Text()), atoi(m_opColorKeyToleranceGreenTextControl -> Text()), atoi(m_opColorKeyToleranceBlueTextControl -> Text()), 255));
					m_opColorKeyTolerancePreviewView -> Invalidate(m_opColorKeyTolerancePreviewView -> Bounds());
					m_opDialogWindow -> Unlock();
				}
*/	//			return true;
			}
// FIXME: Put this code in a method. Its the same code as in OK_BUTTON
			uint32 vMediaClipID = m_opTransitionClip -> GetMediaClipID();
			
			switch(m_oTransitionParameters.m_vID)
			{
			case EM_TRANSITION_DX:
			case EM_TRANSITION_FADER:
				break;
			case EM_TRANSITION_WIPE:
				m_oTransitionParameters.m_vParams.m_vWipe.m_vR = atoi(m_opRTextControl -> Text());
				m_oTransitionParameters.m_vParams.m_vWipe.m_vG = atoi(m_opGTextControl -> Text());	
				m_oTransitionParameters.m_vParams.m_vWipe.m_vB = atoi(m_opBTextControl -> Text());
				m_oTransitionParameters.m_vParams.m_vWipe.m_vSoftness = atoi(m_opSoftnessTextControl -> Text());
				m_oTransitionParameters.m_vParams.m_vWipe.m_vWidth = atoi(m_opWidthTextControl -> Text());
				m_oTransitionParameters.m_vParams.m_vWipe.m_vOffsetX = atoi(m_opOffXTextControl -> Text());
				m_oTransitionParameters.m_vParams.m_vWipe.m_vOffsetY = atoi(m_opOffYTextControl -> Text());
				m_oTransitionParameters.m_vParams.m_vWipe.m_vReplicateX = atoi(m_opRepXTextControl -> Text());
				m_oTransitionParameters.m_vParams.m_vWipe.m_vReplicateY = atoi(m_opRepYTextControl -> Text());
				m_oTransitionParameters.m_vSwap = m_opSwapCheckBox -> IsChecked();
				break;
			case EM_TRANSITION_KEY:
				if(m_opColorKeyUseColorRadioButton -> IsChecked())
					m_oTransitionParameters.m_vParams.m_vKey.m_vColorID = EM_KEY_COLOR;
				else if(m_opColorKeyUseHueRadioButton -> IsChecked())
					m_oTransitionParameters.m_vParams.m_vKey.m_vColorID = EM_KEY_HUE;
				else if(m_opColorKeyUseLuminanceRadioButton -> IsChecked())
					m_oTransitionParameters.m_vParams.m_vKey.m_vColorID = EM_KEY_LUMINANCE;
				else if(m_opColorKeyUseBlueAndGreenScreenRadioButton -> IsChecked())
					m_oTransitionParameters.m_vParams.m_vKey.m_vColorID = EM_KEY_BLUE;
				m_oTransitionParameters.m_vParams.m_vKey.m_vR = m_opColorKeyColorRedTrackbarControl -> GetPosition();
				m_oTransitionParameters.m_vParams.m_vKey.m_vG = m_opColorKeyColorGreenTrackbarControl -> GetPosition();
				m_oTransitionParameters.m_vParams.m_vKey.m_vB = m_opColorKeyColorBlueTrackbarControl -> GetPosition();
				m_oTransitionParameters.m_vParams.m_vKey.m_vHue = m_opColorKeyHueTrackbarControl -> GetPosition();
				m_oTransitionParameters.m_vParams.m_vKey.m_vLuminance = m_opColorKeyLuminanceTrackbarControl -> GetPosition();
				m_oTransitionParameters.m_vParams.m_vKey.m_vTolerance = m_opColorKeyToleranceTrackbarControl -> GetPosition();
				m_oTransitionParameters.m_vParams.m_vKey.m_vInvert = m_opColorKeyInvertCheckBox -> IsChecked();
				m_oTransitionParameters.m_vSwap = m_opColorKeySwapCheckBox -> IsChecked();
				m_opColorKeySettingsView -> Show();
				break;
			}
		
			string oSelection = m_opComboBox -> GetSelection().c_str();
			int32 vTransitionID = m_oTransitionStringIDMap[oSelection.c_str()];
			EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_SET_VIDEO_TRANSITION, &vMediaClipID, &vTransitionID);
			EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_SET_PERSIST_TRANSITION_SETTINGS, &vMediaClipID, &m_oTransitionParameters, NULL);
			EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_REFRESH_PREVIEW);
			eo << "Uppdate Preview" << ef;
			return true;
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
			if(p_opSender == m_opColorKeyColorRedTrackbarControl)
			{
				m_opColorKeyColorRedTextControl -> SetText(itoa(m_opColorKeyColorRedTrackbarControl -> GetPosition(), vTemp, 10));
				eo << "Red Position = " << m_opColorKeyColorRedTrackbarControl -> GetPosition() << ef;
			}
			else if(p_opSender == m_opColorKeyColorGreenTrackbarControl)
				m_opColorKeyColorGreenTextControl -> SetText(itoa(m_opColorKeyColorGreenTrackbarControl -> GetPosition(), vTemp, 10));
			else if(p_opSender == m_opColorKeyColorBlueTrackbarControl)
				m_opColorKeyColorBlueTextControl -> SetText(itoa(m_opColorKeyColorBlueTrackbarControl -> GetPosition(), vTemp, 10));
			else if(p_opSender == m_opColorKeyHueTrackbarControl)
				m_opColorKeyHueTextControl -> SetText(itoa(m_opColorKeyHueTrackbarControl -> GetPosition(), vTemp, 10));
			else if(p_opSender == m_opColorKeyLuminanceTrackbarControl)
				m_opColorKeyLuminanceTextControl -> SetText(itoa(m_opColorKeyLuminanceTrackbarControl -> GetPosition(), vTemp, 10));
			else if(p_opSender == m_opColorKeyToleranceTrackbarControl)
				m_opColorKeyToleranceTextControl -> SetText(itoa(m_opColorKeyToleranceTrackbarControl -> GetPosition(), vTemp, 10));
			else
				return false;
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

void TransitionDialog::MoveDialog()
{
	EMRect oWindowRect(m_opParentWindow -> Frame());
	EMRect oDialogRect(m_opDialogWindow -> Frame());
	float vX(oWindowRect.m_vRight - oWindowRect.GetWidth() / 2 - oDialogRect.GetWidth() / 2);
	float vY(oWindowRect.m_vBottom - oWindowRect.GetHeight() / 2 - oDialogRect.GetHeight() / 2);

	MoveDialog(vX, vY);
}

void TransitionDialog::MoveDialog(float p_vX, float p_vY)
{
	m_opDialogWindow -> MoveTo(p_vX, p_vY);
}

void TransitionDialog::SetParent(EMWindow* p_opWindow)
{
	m_opParentWindow = p_opWindow;
}

void TransitionDialog::SetTrackView(EMView* m_opView)
{
	m_opTrackView = m_opView;
}

void TransitionDialog::SetTransitionClip(EMGClip* p_opClip)
{
	m_opTransitionClip = p_opClip;
}

char* TransitionDialog::SetValidValue(char* p_vpValue, int32 p_vMin, int32 p_vMax)
{
	eo << p_vpValue << ef;
	m_vAlreadyChanged = true;

	int vValue(atoi(p_vpValue));
	if(vValue > p_vMax)
		vValue = p_vMax;
	if(vValue < p_vMin)
		vValue = p_vMin;
	return itoa(vValue, m_vTemp, 10);

}

// Made by: Martin Johansson
void TransitionDialog::HueToRGB(float* p_vpHue, unsigned char* p_vpRGB)
{
	float r;
	float g;
	float b;

	float h = static_cast<float>(*p_vpHue);

	h += 2.0f;

	h = 6.0 - h;

	if(h >= 6.0f)
		h -= 6.0f;

	if(h < 0.0f)
		h += 6.0f;

	float k = static_cast<int>(h);

	if(k>h)
		--k;

	//if(k<0.0f)
	//	k += 6.0f;

	eo << "h = " << h << " k = " << k << " h-k = " << h-k << ef;

	float dk = h - k;

	switch(static_cast<int>(k))
	{
	case 0:
		r = 255;
		g = 255 - 255 * (1.0 - dk);
		b = 0;
		break;
	case 1:
		r = 255 - 255 * dk;
		g = 255;
		b = 0;
		break;
	case 2:
		r = 0;
		g = 255;
		b = 255 - 255 * (1.0 - dk);
		break;
	case 3:
		r = 0;
		g = 255 - 255 * dk;
		b = 255;
		break;
	case 4:
		r = 255 - 255 * (1.0 - dk);
		g = 0;
		b = 255;
		break;
	case 5:
		r = 255;
		g = 0;
		b = 255 - 255 * dk;
		break;
	}

	*p_vpRGB = static_cast<unsigned char>(b + 0.5);
	*(p_vpRGB + 1) = static_cast<unsigned char>(g + 0.5);
	*(p_vpRGB + 2) = static_cast<unsigned char>(r + 0.5);
}