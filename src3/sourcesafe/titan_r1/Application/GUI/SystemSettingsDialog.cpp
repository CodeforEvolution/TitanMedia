#include "SystemSettingsDialog.h"

#include "CommandIDs.h"
#include "EMButton.h"
#include "EMCheckBox.h"
#include "EMComboBox.h"
#include "EMCommandRepository.h"
#include "EMFactory.h"
#include "EMGroupBox.h"
#include "EMLabel.h"
#include "EMMediaGlobals.h"
#include "EMMessageBox.h"
#include "EMMessages.h"
#include "EMNetworkEngine.h"
#include "EMRadioButton.h"
#include "EMSettingIDs.h"
#include "EMSettingsRepository.h"
#include "EMTabControl.h"
#include "EMTextControl.h"
#include "EMView.h"
#include "EMWindow.h"

#ifdef PLATFORM_WINDOWS
#include <windows.h> // To get modal dialog to work.
#include "EMWinWindow.h" // To get modal dialog to work.
#endif

const uint32 OK_BUTTON = 1;
const uint32 CANCEL_BUTTON = 2;
const uint32 SELECT_USER_BUTTON = 3;
const uint32 READ_AHEAD_TEXT_CONTROL_CHANGED = 4;
const uint32 BUFFER_SIZE_TEXT_CONTROL_CHANGED = 5;
const uint32 SNAP_THRESHOLD_TEXT_CONTROL_CHANGED = 6;
const uint32 USER_ID_TEXT_CONTROL_CHANGED = 7;

SystemSettingsDialog::SystemSettingsDialog() :
// Main View
m_opDialogWindow(EMFactory::Instance() -> CreateEMWindow(EMRect(0, 0, 300, 313), "System Settings", EM_WINDOW_DIALOG)),
m_opMainView(EMFactory::Instance() -> CreateView(EMRect(0, 0, 300, 313), 0)),
m_opTabControl(EMFactory::Instance() -> CreateTabControl(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
																EM_METRICS_NATIVE_DIALOG_MARGIN_TOP,
																300 - EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 1,
																313 - (EM_METRICS_NATIVE_DIALOG_MARGIN_BOTTOM + EM_METRICS_NATIVE_BUTTON_HEIGHT + EM_METRICS_NATIVE_BUTTON_SPACE)),
														 "System Tab", true, 0, 0, 0)),
m_opOK_Button(EMFactory::Instance() -> CreateButton(EMRect(300 - (EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + EM_METRICS_NATIVE_BUTTON_WIDTH * 2 + EM_METRICS_NATIVE_BUTTON_SPACE),
														   313 - (EM_METRICS_NATIVE_DIALOG_MARGIN_BOTTOM + EM_METRICS_NATIVE_BUTTON_HEIGHT),
														   300 - (EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + EM_METRICS_NATIVE_BUTTON_WIDTH + EM_METRICS_NATIVE_BUTTON_SPACE),
														   313 - (EM_METRICS_NATIVE_DIALOG_MARGIN_BOTTOM)),
													"OK", OK_BUTTON, 0, 0, NULL, true)),
m_opCancelButton(EMFactory::Instance() -> CreateButton(EMRect(300 - (EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + EM_METRICS_NATIVE_BUTTON_WIDTH),
															  313 - (EM_METRICS_NATIVE_DIALOG_MARGIN_BOTTOM + EM_METRICS_NATIVE_BUTTON_HEIGHT),
															  300 - (EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE),
															  313 - (EM_METRICS_NATIVE_DIALOG_MARGIN_BOTTOM)),
													   "Cancel", CANCEL_BUTTON, 0, 0)),
// Tabs
m_opNetworkView(EMFactory::Instance() -> CreateView(EMRect(0, 0, 0, 0), 0)), // TabControl take care of the size
m_vDecimalPosition(-1),
m_vSelectionStartBeforeDecimal(false),
m_opAudioView(EMFactory::Instance() -> CreateView(EMRect(0, 0, 0, 0), 0)), // TabControl take care of the size
m_opGUIView(EMFactory::Instance() -> CreateView(EMRect(0, 0, 0, 0), 0)), // TabControl take care of the size
//m_opMetronomeView(EMFactory::Instance() -> CreateView(EMRect(0, 0, 0, 0), 0)), // TabControl take care of the size
// Variables
m_vAlreadyChanged(false)
{
// Main View
	m_opDialogWindow -> AddChild(m_opMainView);
	m_opMainView -> AddChild(m_opTabControl);
	m_opMainView -> AddChild(m_opOK_Button);
	m_opMainView -> AddChild(m_opCancelButton);

// Network Tab
	m_opTabControl -> AddChild(m_opNetworkView);
	m_opTabControl -> AddTab("Network", m_opNetworkView);
//	m_opTabControl -> AddTab("Test", m_opTestView);

	EMFactory* oFactory = EMFactory::Instance();

	// Network Tab -> Host
	m_opHostGroupBox = oFactory -> CreateGroupBox(EMRect(EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_SIDE,
														EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_TOP,
														m_opNetworkView -> Frame().GetWidth() - EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_SIDE,
														EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_TOP + 90),
												 "Host",0);
	m_opHostView = oFactory -> CreateView(EMRect(EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_SIDE,
												EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_TOP,
												m_opNetworkView -> Frame().GetWidth() - EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_SIDE,
												EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_TOP + 90),
										 0);
	m_opHostLabel = oFactory -> CreateLabel(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
												  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 2,
												  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 30,
												  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 2 + EM_METRICS_NATIVE_LABEL_HEIGHT),
										   "Host:", 0);
	m_opHostTextControl = oFactory -> CreateTextControl(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 35,
															  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP,
															  m_opHostView -> Frame().GetWidth() - EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
															  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + EM_METRICS_NATIVE_TEXT_CONTROL_HEIGHT),
													   "server.elegant-media.com", 0, 0, false);
	m_opProtTCP_RadioButton = oFactory -> CreateRadioButton(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
																  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 27,
																  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 45,
																  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 27 + EM_METRICS_NATIVE_RADIO_BUTTON_HEIGHT),
														   "TCP", 0, 0, 0, false);
	m_opProtUDP_RadioButton = oFactory -> CreateRadioButton(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
																  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 49,
																  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 45,
																  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 49 + EM_METRICS_NATIVE_RADIO_BUTTON_HEIGHT),
														   "UDP", 0, 0, 0, true);
	m_opProtTCP_PortLabel = oFactory -> CreateLabel(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 55,
														  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 28,
														  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 55 + 20,
														  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 28 + EM_METRICS_NATIVE_LABEL_HEIGHT),
												   "Port:", 0);
	m_opProtUDP_PortLabel = oFactory -> CreateLabel(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 55,
														  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 50,
														  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 55 + 20,
														  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 50 + EM_METRICS_NATIVE_LABEL_HEIGHT),
												   "Port:", 0);
	m_opProtTCP_PortTextControl = oFactory ->CreateTextControl(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 80,
																	  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 25,
																	  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 80 + 36,
																	  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 25 + EM_METRICS_NATIVE_TEXT_CONTROL_HEIGHT),
															   "12345", 0, 0, false, EM_ALIGNMENT_RIGHT, true);
	m_opProtUDP_PortTextControl = oFactory ->CreateTextControl(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 80,
																	  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 47,
																	  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 80 + 36,
																	  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 47 + EM_METRICS_NATIVE_TEXT_CONTROL_HEIGHT),
															   "12345", 0, 0, false, EM_ALIGNMENT_RIGHT, true);
	m_opInternetRadioButton = oFactory -> CreateRadioButton(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 160,
																  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 27,
																  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 160 + 70,
																  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 27 + EM_METRICS_NATIVE_RADIO_BUTTON_HEIGHT),
														   "Internet", 0, 0, 0, false);
	m_opIntranetRadioButton = oFactory -> CreateRadioButton(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 160,
																  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 49,
																  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 160 + 70,
																  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 49 + EM_METRICS_NATIVE_RADIO_BUTTON_HEIGHT),
														   "Intranet", 0, 0, 0, true);

	m_opNetworkView -> AddChild(m_opHostGroupBox);
	m_opNetworkView -> AddChild(m_opHostView);
	m_opHostView -> AddChild(m_opHostLabel);
	m_opHostView -> AddChild(m_opHostTextControl);
	m_opHostView -> AddChild(m_opProtTCP_RadioButton);
	m_opHostView -> AddChild(m_opProtUDP_RadioButton);
	m_opHostView -> AddChild(m_opProtTCP_PortLabel);
	m_opHostView -> AddChild(m_opProtUDP_PortLabel);
	m_opHostView -> AddChild(m_opProtTCP_PortTextControl);
	m_opHostView -> AddChild(m_opProtUDP_PortTextControl);
	m_opHostView -> AddChild(m_opInternetRadioButton);
	m_opHostView -> AddChild(m_opIntranetRadioButton);

	// Network Tab -> User
	m_opUserGroupBox = oFactory -> CreateGroupBox(EMRect(EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_SIDE,
														EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_TOP + m_opHostGroupBox -> Frame().GetHeight() + 5,
														m_opNetworkView -> Frame().GetWidth() - EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_SIDE,
														EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_TOP + m_opHostGroupBox -> Frame().GetHeight() + 95),
												 "User",0);
	m_opUserView = oFactory -> CreateView(EMRect(EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_SIDE,
												EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_TOP + m_opHostGroupBox -> Frame().GetHeight() + 5,
												m_opNetworkView -> Frame().GetWidth() - EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_SIDE,
												EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_TOP + m_opHostGroupBox -> Frame().GetHeight() + 95),
										 0);
	m_opUserIDLabel = oFactory -> CreateLabel(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
													EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 3,
													EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 50,
													EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 3 + EM_METRICS_NATIVE_LABEL_HEIGHT),
											 "User ID:", 0);
	m_opUserIDTextControl = oFactory -> CreateTextControl(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 55,
																EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP,
																EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 55 + 100,
																EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + EM_METRICS_NATIVE_TEXT_CONTROL_HEIGHT),
														 "", USER_ID_TEXT_CONTROL_CHANGED, 0, false, EM_ALIGNMENT_RIGHT);
	m_opSelectUserButton = oFactory -> CreateButton(EMRect(m_opUserView -> Frame().GetWidth() - (EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + EM_METRICS_NATIVE_BUTTON_WIDTH),
														  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP - 1,
														  m_opUserView -> Frame().GetWidth() - 1 - EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
														  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + EM_METRICS_NATIVE_BUTTON_HEIGHT),
												   "Select User", SELECT_USER_BUTTON, 0, 0);
	m_opPasswordLabel = oFactory -> CreateLabel(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
													  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 28,
													  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 50,
													  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 28 + EM_METRICS_NATIVE_LABEL_HEIGHT),
											   "Password:", 0);
	m_opPasswordTextControl = oFactory -> CreateTextControl(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 55,
																  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 25,
																  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 55 + 100,
																  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 25 + EM_METRICS_NATIVE_TEXT_CONTROL_HEIGHT),
														   "", 0, 0, false, EM_ALIGNMENT_LEFT, false, true);
	m_opSavePasswordCheckBox = oFactory -> CreateCheckBox(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
																 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 50,
																 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 100,
																 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 50 + EM_METRICS_NATIVE_CHECKBOX_HEIGHT),
														  "Save Password", 0, 0, 0);

	m_opNetworkView -> AddChild(m_opUserGroupBox);
	m_opNetworkView -> AddChild(m_opUserView);
	m_opUserView -> AddChild(m_opUserIDLabel);
	m_opUserView -> AddChild(m_opUserIDTextControl);
//	m_opUserView -> AddChild(m_opSelectUserButton);
	m_opUserView -> AddChild(m_opPasswordLabel);
	m_opUserView -> AddChild(m_opPasswordTextControl);
	m_opUserView -> AddChild(m_opSavePasswordCheckBox);

// Audio Tab
	m_opTabControl -> AddChild(m_opAudioView);
	m_opTabControl -> AddTab("Audio", m_opAudioView);

	m_opAudioReadAheadLabel = oFactory -> CreateLabel(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
															 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 3,
															 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 63,
															 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 3 + EM_METRICS_NATIVE_LABEL_HEIGHT),
													  "Read Ahead:", 0);
	m_opAudioReadAheadTextControl = oFactory -> CreateTextControl(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 68,
																		 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP,
																		 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 68 + 45,
																		 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + EM_METRICS_NATIVE_TEXT_CONTROL_HEIGHT),
																  "50", READ_AHEAD_TEXT_CONTROL_CHANGED, 0, false, EM_ALIGNMENT_RIGHT, true);
	m_opAudioReadAheadSuffixLabel = oFactory -> CreateLabel(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 118,
																   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 3,
																   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 118 + 120,
																   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 3 + EM_METRICS_NATIVE_LABEL_HEIGHT),
															"milliseconds  (50-1000)", 0);
	m_opAudioBufferSizeLabel = oFactory -> CreateLabel(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
															  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 28,
															  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 63,
															 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 28 + EM_METRICS_NATIVE_LABEL_HEIGHT),
													   "Buffer Size:", 0);
	m_opAudioBufferSizeTextControl = oFactory -> CreateTextControl(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 68,
																		  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 25,
																		  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 68 + 45,
																		  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 25 + EM_METRICS_NATIVE_TEXT_CONTROL_HEIGHT),
																	   "65535", BUFFER_SIZE_TEXT_CONTROL_CHANGED, 0, false, EM_ALIGNMENT_RIGHT, true);
	m_opAudioBufferSizeSuffixLabel = oFactory -> CreateLabel(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 118,
																	EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 28,
																	EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 118 + 100,
																	EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 28 + EM_METRICS_NATIVE_LABEL_HEIGHT),
															 "bytes  (128-65535)", 0);


	m_opAudioView -> AddChild(m_opAudioReadAheadLabel);
	m_opAudioView -> AddChild(m_opAudioReadAheadTextControl);
	m_opAudioView -> AddChild(m_opAudioReadAheadSuffixLabel);
	m_opAudioView -> AddChild(m_opAudioBufferSizeLabel);
	m_opAudioView -> AddChild(m_opAudioBufferSizeTextControl);
	m_opAudioView -> AddChild(m_opAudioBufferSizeSuffixLabel);

// GUI Tab
	m_opTabControl -> AddChild(m_opGUIView);
	m_opTabControl -> AddTab("GUI", m_opGUIView);

	m_opGUISnapThresholdLabel = oFactory -> CreateLabel(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
															   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 3,
															   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 78,
															   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 3 + EM_METRICS_NATIVE_LABEL_HEIGHT),
														"Snap Threshold:", 0);
	m_opGUISnapThresholdTextControl = oFactory -> CreateTextControl(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 83,
																		   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP,
																		   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 83 + 20,
																		   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + EM_METRICS_NATIVE_TEXT_CONTROL_HEIGHT),
																	"10", SNAP_THRESHOLD_TEXT_CONTROL_CHANGED, 0, false, EM_ALIGNMENT_RIGHT, true);
	m_opGUISnapThresholdSuffixLabel = oFactory -> CreateLabel(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 108,
																	 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 3,
																	 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 108 + 100,
																	 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 3 + EM_METRICS_NATIVE_LABEL_HEIGHT),
															  "pixels  (1-50)", 0);

	m_opGUIView -> AddChild(m_opGUISnapThresholdLabel);
	m_opGUIView -> AddChild(m_opGUISnapThresholdTextControl);
	m_opGUIView -> AddChild(m_opGUISnapThresholdSuffixLabel);
/*
// Metronome Tab
	m_opTabControl -> AddChild(m_opMetronomeView);
	m_opTabControl -> AddTab("Metronome", m_opMetronomeView);

	m_opMetronomeUsePCSpeakerCheckBox = oFactory -> CreateCheckBox(EMRect(EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_SIDE,
																		  EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_TOP + 3,
																		  EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_SIDE + 100,
																		  EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_TOP + 3 + EM_METRICS_NATIVE_CHECKBOX_HEIGHT),
																   "Use PC Speaker", 0, 0, 0);
	m_opMetronomeUseMIDINoteCheckBox = oFactory -> CreateCheckBox(EMRect(EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_SIDE,
																		  EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_TOP + 28,
																		  EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_SIDE + 100,
																		  EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_TOP + 28 + EM_METRICS_NATIVE_CHECKBOX_HEIGHT),
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
	m_opMetronomeView -> AddChild(m_opMetronomeCountInLabel);
	m_opMetronomeView -> AddChild(m_opMetronomeCountInComboBox);
	
	// Metronome Tab -> Midi Note
	m_opMetronomeMidiNoteGroupBox = oFactory -> CreateGroupBox(EMRect(EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_SIDE,
																	  EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_SIDE + 50,
																	  m_opMetronomeView -> Frame().GetWidth() - EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_SIDE,
																	  EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_SIDE + 50 + 175),
															  "Midi Note", 0);
	m_opMetronomeMidiNoteView = oFactory -> CreateView(EMRect(EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_SIDE,
															  EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_SIDE + 50,
															  m_opMetronomeView -> Frame().GetWidth() - EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_SIDE,
															  EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_SIDE + 50 + 175),
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
	m_opMetronomeFirstBeatKeyComboBox = oFactory -> CreateComboBox(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 50,
																	 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP,
																	 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 50 + 50,
																	 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 100),
															  0, 0, false);
	m_opMetronomeFirstBeatVelocityLabel = oFactory -> CreateLabel(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
																	  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 28,
																	  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 45,
																	  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 28 + EM_METRICS_NATIVE_LABEL_HEIGHT),
															   "Velocity:", 0);
	m_opMetronomeFirstBeatVelocityComboBox = oFactory -> CreateComboBox(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 50,
																	 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 25,
																	 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 50 + 50,
																	 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 25 + 100),
															  0, 0, false);
	m_opMetronomeFirstBeatDurationLabel = oFactory -> CreateLabel(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
																	  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 53,
																	  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 45,
																	  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 53 + EM_METRICS_NATIVE_LABEL_HEIGHT),
															   "Duration:", 0);
	m_opMetronomeFirstBeatDurationComboBox = oFactory -> CreateComboBox(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 50,
																	 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 50,
																	 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 50 + 50,
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
															  "First Beat", 0);
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
	m_opMetronomeOtherBeatKeyComboBox = oFactory -> CreateComboBox(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 50,
																	 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP,
																	 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 50 + 50,
																	 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 100),
															  0, 0, false);
	m_opMetronomeOtherBeatVelocityLabel = oFactory -> CreateLabel(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
																	  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 28,
																	  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 45,
																	  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 28 + EM_METRICS_NATIVE_LABEL_HEIGHT),
															   "Velocity:", 0);
	m_opMetronomeOtherBeatVelocityComboBox = oFactory -> CreateComboBox(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 50,
																	 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 25,
																	 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 50 + 50,
																	 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 25 + 100),
															  0, 0, false);
	m_opMetronomeOtherBeatDurationLabel = oFactory -> CreateLabel(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
																	  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 53,
																	  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 45,
																	  EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 53 + EM_METRICS_NATIVE_LABEL_HEIGHT),
															   "Duration:", 0);
	m_opMetronomeOtherBeatDurationComboBox = oFactory -> CreateComboBox(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 50,
																	 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 50,
																	 EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 50 + 50,
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
*/
}

SystemSettingsDialog::~SystemSettingsDialog()
{
	// Delete topmost first
	// Network Tab
	delete m_opHostLabel;
	delete m_opHostTextControl;
	delete m_opProtTCP_RadioButton;
	delete m_opProtUDP_RadioButton;
	delete m_opProtTCP_PortLabel;
	delete m_opProtUDP_PortLabel;
	delete m_opProtTCP_PortTextControl;
	delete m_opProtUDP_PortTextControl;
	delete m_opInternetRadioButton;
	delete m_opIntranetRadioButton;
	delete m_opHostGroupBox;
	delete m_opHostView;

	delete m_opUserIDLabel;
	delete m_opUserIDTextControl;
	delete m_opSelectUserButton;
	delete m_opPasswordLabel;
	delete m_opPasswordTextControl;
	delete m_opSavePasswordCheckBox;
	delete m_opUserGroupBox;
	delete m_opUserView;

	delete m_opNetworkView;

	// Audio Tab
	delete m_opAudioReadAheadLabel;
	delete m_opAudioReadAheadTextControl;
	delete m_opAudioReadAheadSuffixLabel;
	delete m_opAudioBufferSizeLabel;
	delete m_opAudioBufferSizeTextControl;
	delete m_opAudioBufferSizeSuffixLabel;

	delete m_opAudioView;

	// GUI Tab
	delete m_opGUISnapThresholdLabel;
	delete m_opGUISnapThresholdTextControl;
	delete m_opGUISnapThresholdSuffixLabel;

	delete m_opGUIView;

/*	// Metronome Tab
	delete m_opMetronomeUsePCSpeakerCheckBox;
	delete m_opMetronomeUseMIDINoteCheckBox;
	delete m_opMetronomeCountInLabel;

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
*/
	// Main View
	delete m_opDialogWindow;
	delete m_opMainView;
	delete m_opTabControl;
	delete m_opOK_Button;
	delete m_opCancelButton;
}

uint64 SystemSettingsDialog::ConvertToID_Uint64(const char* p_vpUserID)
{
	string oFirstUserID = p_vpUserID;
	string oSecondUserID = p_vpUserID;
	uint32 vUserID[2];

	if(oFirstUserID.find_first_of('.') == oFirstUserID.npos)
		return 0;

	oFirstUserID.erase(oFirstUserID.find_first_of('.'), oFirstUserID.length());
	oSecondUserID.erase(0, oSecondUserID.find_first_of('.') + 1);

	vUserID[0] = atoi(oSecondUserID.c_str());
	vUserID[1] = atoi(oFirstUserID.c_str());
	
	return *reinterpret_cast<uint64*>(vUserID);
}
void SystemSettingsDialog::ConvertToID_String(uint64 p_vUserID, char* p_vpUserID)
{
	uint32* vUserID = new uint32[2];
	string oUserID;
	char vTemp[40];

	memcpy(vUserID, &p_vUserID, sizeof(p_vUserID));
	//vUserID = reinterpret_cast<uint32*>(&p_vUserID);
	oUserID.append(itoa(vUserID[1], vTemp, 10));
	oUserID.append(".");
	oUserID.append(itoa(vUserID[0], vTemp, 10));

	strcpy(p_vpUserID, oUserID.c_str());
	delete [] vUserID;
}

void SystemSettingsDialog::DisplayDialog()
{
	// Getting Settings
	char vTemp[40];
	EMSettingsRepository* oSettings = EMSettingsRepository::Instance();
	// Network Settings
	m_opHostTextControl -> SetText(static_cast<string*>(oSettings -> GetSetting(SETTING_NETWORK_HOST)) -> c_str());
	if(*static_cast<bool*>(oSettings ->GetSetting(SETTING_NETWORK_USE_TCP)))
	{
		m_opProtTCP_RadioButton -> Check(true);
		m_opProtUDP_RadioButton -> Check(false);
	}
	else
	{
		m_opProtTCP_RadioButton -> Check(false);
		m_opProtUDP_RadioButton -> Check(true);
	}
	m_opProtTCP_PortTextControl -> SetText(itoa(*static_cast<int32*>(oSettings -> GetSetting(SETTING_NETWORK_TCP_PORT)), vTemp, 10));
	m_opProtUDP_PortTextControl -> SetText(itoa(*static_cast<int32*>(oSettings -> GetSetting(SETTING_NETWORK_UDP_PORT)), vTemp, 10));
	if(*static_cast<bool*>(oSettings ->GetSetting(SETTING_NETWORK_USE_INTERNET)))
	{
		m_opInternetRadioButton -> Check(true);
		m_opIntranetRadioButton -> Check(false);
	}
	else
	{
		m_opInternetRadioButton -> Check(false);
		m_opIntranetRadioButton -> Check(true);
	}
	ConvertToID_String(*static_cast<int64*>(oSettings -> GetSetting(SETTING_NETWORK_USER_ID)), vTemp);
	m_opUserIDTextControl -> SetText(vTemp);
	if(*static_cast<bool*>(oSettings ->GetSetting(SETTING_NETWORK_SAVE_PASSWORD)))
	{
		m_opPasswordTextControl -> SetText(static_cast<string*>(oSettings -> GetSetting(SETTING_NETWORK_USER_PASSWORD)) -> c_str());
		m_opSavePasswordCheckBox -> Check(true);
	}
	else
	{
		m_opPasswordTextControl -> SetText("");
		m_opSavePasswordCheckBox -> Check(false);
	}

	// Audio Settings
	m_opAudioReadAheadTextControl -> SetText(itoa(*static_cast<int32*>(oSettings -> GetSetting(SETTING_AUDIO_PROCESSING_AHEAD)), vTemp, 10));
	m_opAudioBufferSizeTextControl -> SetText(itoa(*static_cast<int32*>(oSettings -> GetSetting(SETTING_AUDIO_BUFFER_SIZE)), vTemp, 10));

	// GUI Settings
	m_opGUISnapThresholdTextControl -> SetText(itoa(*static_cast<int32*>(oSettings -> GetSetting(SETTING_SNAP_THRESHOLD)), vTemp, 10));

/*	// Metronome Settings
	m_opMetronomeUsePCSpeakerCheckBox -> Check(*static_cast<bool*>(oSettings -> GetSetting(SETTING_MIDI_METRONOME_USE_PC_SPEAKER)));
	m_opMetronomeUseMIDINoteCheckBox -> Notify(USE_MIDI_NOTE_CHECKBOX_CHANGED);
*/
	if(m_opDialogWindow -> Lock())
	{
		m_opDialogWindow -> Show();
#ifdef PLATFORM_WINDOWS
		EnableWindow(dynamic_cast<EMWinWindow*>(m_opDialogWindow) -> GetMainWindowHandle(), FALSE);
		SetFocus(static_cast<HWND>(m_opDialogWindow -> GetNativeView()));
#endif
		m_opDialogWindow -> Unlock();
	}
}
/*
void SystemSettingsDialog::EnableMidiNote(bool p_vEnable)
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
*/
void SystemSettingsDialog::HideDialog()
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

void SystemSettingsDialog::Init()
{
	m_opDialogWindow -> AddListener(this);
	m_opMainView -> AddListener(this);
	m_opNetworkView -> AddListener(this);
	m_opHostView -> AddListener(this);
	m_opUserView -> AddListener(this);
	m_opAudioView -> AddListener(this);
	m_opGUIView -> AddListener(this);
//	m_opMetronomeView -> AddListener(this);
//	m_opMetronomeMidiNoteView -> AddListener(this);
//	m_opMetronomeFirstBeatView -> AddListener(this);
//	m_opMetronomeOtherBeatView -> AddListener(this);
	if(m_opDialogWindow -> Lock())
	{
		//m_opMainView -> SetViewColor(EMColor(0, 255, 255, 255));
		m_opMainView -> SetDefaultColor();
		//m_opNetworkView -> SetViewColor(EMColor(0, 255, 0, 255));
		m_opNetworkView -> SetDefaultColor();
		//m_opHostView -> SetViewColor(EMColor(255, 255, 0, 255));
		m_opHostView -> SetDefaultColor();
		//m_opUserView -> SetViewColor(EMColor(255, 255, 0, 255));
		m_opUserView -> SetDefaultColor();
		//m_opAudioView -> SetViewColor(EMColor(255, 255, 0, 255));
		m_opAudioView -> SetDefaultColor();
		//m_opGUIView -> SetViewColor(EMColor(255, 255, 0, 255));
		m_opGUIView -> SetDefaultColor();
		//m_opMetronomeView -> SetViewColor(EMColor(255, 255, 0, 255));
//		m_opMetronomeView -> SetDefaultColor();
		//m_opMetronomeMidiNoteView -> SetViewColor(EMColor(255, 0, 0, 255));
//		m_opMetronomeMidiNoteView -> SetDefaultColor();
		//m_opMetronomeFirstBeatView -> SetViewColor(EMColor(0, 255, 0, 255));
//		m_opMetronomeFirstBeatView -> SetDefaultColor();
		//m_opMetronomeOtherBeatView -> SetViewColor(EMColor(0, 0, 255, 255));
//		m_opMetronomeOtherBeatView -> SetDefaultColor();
		m_opDialogWindow -> Unlock();
	}
	// Network
	m_opHostTextControl -> AddListener(this);
	m_opProtTCP_RadioButton -> AddListener(this);
	m_opProtUDP_RadioButton -> AddListener(this);
	m_opProtTCP_PortTextControl -> AddListener(this);
	m_opProtUDP_PortTextControl -> AddListener(this);
	m_opInternetRadioButton -> AddListener(this);
	m_opIntranetRadioButton -> AddListener(this);
	m_opUserIDTextControl -> AddListener(this);
	m_opPasswordTextControl -> AddListener(this);
	m_opSavePasswordCheckBox -> AddListener(this);
	m_opSelectUserButton -> AddListener(this);
	//Audio
	m_opAudioReadAheadTextControl -> AddListener(this);
	m_opAudioBufferSizeTextControl -> AddListener(this);
	// GUI
	m_opGUISnapThresholdTextControl -> AddListener(this);
/*	// Metronome
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

	SetNumberInComboBox(m_opMetronomeCountInComboBox, 10);
	m_opMetronomeCountInComboBox -> SelectItem(0);

	EMMediaType eMidiType = EM_TYPE_MIDI;
	list<int32>* opMidiOutputIDs = static_cast<list<int32>*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_OUTPUTS, &eMidiType));
	if(opMidiOutputIDs -> size() == 0)
		m_opMetronomeMidiNoteDeviceComboBox -> AddString("No Midi Output found");
	else
	{
		m_opMetronomeMidiNoteDeviceComboBox -> AddString("Not implemented yet");
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
*/	
	// MainView
	m_opTabControl -> AddListener(this);
	m_opOK_Button -> AddListener(this);
	m_opCancelButton -> AddListener(this);

	// TabOrder
	// Network
	m_opDialogWindow -> AddTabComponent(m_opHostTextControl);
	m_opDialogWindow -> AddTabComponent(m_opProtTCP_RadioButton);
	m_opDialogWindow -> AddTabComponent(m_opProtUDP_RadioButton);
	m_opDialogWindow -> AddTabComponent(m_opProtTCP_PortTextControl);
	m_opDialogWindow -> AddTabComponent(m_opProtUDP_PortTextControl);
	m_opDialogWindow -> AddTabComponent(m_opInternetRadioButton);
	m_opDialogWindow -> AddTabComponent(m_opIntranetRadioButton);
	m_opDialogWindow -> AddTabComponent(m_opUserIDTextControl);
	m_opDialogWindow -> AddTabComponent(m_opPasswordTextControl);
	m_opDialogWindow -> AddTabComponent(m_opSavePasswordCheckBox);
	m_opDialogWindow -> AddTabComponent(m_opSelectUserButton);
	// Audio
	m_opDialogWindow -> AddTabComponent(m_opAudioReadAheadTextControl);
	m_opDialogWindow -> AddTabComponent(m_opAudioBufferSizeTextControl);
	// GUI
	m_opDialogWindow -> AddTabComponent(m_opGUISnapThresholdTextControl);
/*	// Metronome
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
*/	// MainView
	m_opDialogWindow -> AddTabComponent(m_opOK_Button);
	m_opDialogWindow -> AddTabComponent(m_opCancelButton);
	m_opDialogWindow -> AddTabComponent(m_opTabControl);

/*	// Getting Settings
	char vTemp[40];
	EMSettingsRepository* oSettings = EMSettingsRepository::Instance();
	m_opHostTextControl -> SetText(static_cast<string*>(oSettings -> GetSetting(SETTING_NETWORK_HOST)) -> c_str());
	if(*static_cast<bool*>(oSettings ->GetSetting(SETTING_NETWORK_USE_TCP)))
	{
		m_opProtTCP_RadioButton -> Check(true);
		m_opProtUDP_RadioButton -> Check(false);
	}
	else
	{
		m_opProtTCP_RadioButton -> Check(false);
		m_opProtUDP_RadioButton -> Check(true);
	}
	m_opProtTCP_PortTextControl -> SetText(itoa(*static_cast<int32*>(oSettings -> GetSetting(SETTING_NETWORK_TCP_PORT)), vTemp, 10));
	m_opProtUDP_PortTextControl -> SetText(itoa(*static_cast<int32*>(oSettings -> GetSetting(SETTING_NETWORK_UDP_PORT)), vTemp, 10));
	if(*static_cast<bool*>(oSettings ->GetSetting(SETTING_NETWORK_USE_INTERNET)))
	{
		m_opInternetRadioButton -> Check(true);
		m_opIntranetRadioButton -> Check(false);
	}
	else
	{
		m_opInternetRadioButton -> Check(false);
		m_opIntranetRadioButton -> Check(true);
	}
	m_opUserIDTextControl -> SetText(_i64toa(*static_cast<int64*>(oSettings -> GetSetting(SETTING_NETWORK_USER_ID)), vTemp, 10));
	m_opPasswordTextControl -> SetText(static_cast<string*>(oSettings -> GetSetting(SETTING_NETWORK_USER_PASSWORD)) -> c_str());
*/
}

bool SystemSettingsDialog::MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage)
{
	switch(p_vMessage)
	{
	case EM_GUI_COMPONENT_ENTER_PRESSED:
	case OK_BUTTON:
		eo << "EM_BUTTON_OK" << ef;
		SaveSettings();
		if(EMNetworkEngine::Instance() != NULL)
		{
			bool vUseTCP = *static_cast<bool*>(EMSettingsRepository::Instance() ->GetSetting(SETTING_NETWORK_USE_TCP));
			uint32 vPort(0);
			if(vUseTCP)
				vPort = *static_cast<uint32*>(EMSettingsRepository::Instance() ->GetSetting(SETTING_NETWORK_TCP_PORT));
			else
				vPort = *static_cast<uint32*>(EMSettingsRepository::Instance() ->GetSetting(SETTING_NETWORK_UDP_PORT));
			EMNetworkEngine::Instance() -> Initialize(static_cast<string*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_NETWORK_HOST)) -> c_str(),
													  vPort,
													  vUseTCP,
													  *static_cast<bool*>(EMSettingsRepository::Instance() ->GetSetting(SETTING_NETWORK_USE_INTERNET)));
		}
		HideDialog();
		return true;
	case EM_GUI_COMPONENT_ESC_PRESSED:
	case CANCEL_BUTTON:
		eo << "EM_BUTTON_CANCEL" << ef;
		HideDialog();
		return true;
	case SELECT_USER_BUTTON:
		eo << "SELECT_USER_BUTTON" << ef;
		EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_DISPLAY_SEARCH_USER_DIALOG, m_opDialogWindow, this);
		return true;
	case EM_TEXT_CONTROL_LEFT_MOUSE_BUTTON_DOWN:
	case EM_TEXT_CONTROL_UP_ARROW_PRESSED:
	case EM_TEXT_CONTROL_DOWN_ARROW_PRESSED:
	case EM_TEXT_CONTROL_LEFT_ARROW_PRESSED:
	case EM_TEXT_CONTROL_RIGHT_ARROW_PRESSED:
		if(p_opSender == m_opUserIDTextControl)
		{
			eo << "EM_TEXT_CONTROL_RIGHT_ARROW_PRESSED" << ef;
			int32 vSelectionStart(0);
			int32 vSelectionEnd(0);
			m_opUserIDTextControl -> GetSelection(vSelectionStart, vSelectionEnd);
			switch(p_vMessage)
			{
			case EM_TEXT_CONTROL_UP_ARROW_PRESSED:
			case EM_TEXT_CONTROL_LEFT_ARROW_PRESSED:
				if(vSelectionStart != 0)
					vSelectionStart--;
				break;
			case EM_TEXT_CONTROL_DOWN_ARROW_PRESSED:
			case EM_TEXT_CONTROL_RIGHT_ARROW_PRESSED:
				if(vSelectionStart != strlen(m_opUserIDTextControl -> Text()))
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
	case USER_ID_TEXT_CONTROL_CHANGED:
		eo << "FRAMERATE_TEXT_CONTROL_CHANGED" << ef;
		if(m_vAlreadyChanged == false)
		{
			//m_vFrameRateChanged = true;
			string oFrameRate(m_opUserIDTextControl -> Text());
			int32 vSelectionStart(0);
			int32 vSelectionEnd(0);
			int vDecimalPosition(-1);
			bool vErased(false);

			// Check after non numeric characters and delete them
			for(int vCounter = 0; vCounter < oFrameRate.size(); vCounter++)
			{
				if(oFrameRate[vCounter] < 48 || oFrameRate[vCounter] > 57)
				{
					if(oFrameRate[vCounter] == '.')
						continue;
					oFrameRate.erase(vCounter, 1);
					vErased = true;
					break;
				}
			}
			// Check if character was added/deleted before the decimal character and move decimal position
			m_opUserIDTextControl -> GetSelection(vSelectionStart, vSelectionEnd);
			if(m_vSelectionStartBeforeDecimal/*vSelectionStart <= m_vDecimalPosition + 1*/ && m_vDecimalPosition != -1)
			{
				if(m_vFrameRateLength < oFrameRate.size())
					m_vDecimalPosition++;
				else if(m_vFrameRateLength > oFrameRate.size())
					m_vDecimalPosition--;
			}

			// Setting the new decimal position if needed
			vDecimalPosition = oFrameRate.find_first_of(".");
			if(vDecimalPosition == string::npos)
				m_vDecimalPosition = -1;
			else
			{
				if(vDecimalPosition == m_vDecimalPosition)
				{
					vDecimalPosition = oFrameRate.find_first_of(".", m_vDecimalPosition + 1);
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
		//	if(oFrameRate.size() - m_vDecimalPosition > NUMBER_OF_DECIMALS && m_vDecimalPosition != -1)
		//		oFrameRate.erase(oFrameRate.find_first_of(".") + NUMBER_OF_DECIMALS);

			// Set the new value in the textcontrol
			if(oFrameRate.size() != 0)
			{
				m_vAlreadyChanged = true;
				m_opUserIDTextControl -> SetText(oFrameRate.c_str());
				if(vErased)
					m_opUserIDTextControl -> SetSelection(vSelectionStart - 1, vSelectionEnd - 1);
				else
					m_opUserIDTextControl -> SetSelection(vSelectionStart, vSelectionEnd);
			}
			else // if no value exist set value to 0.0
			{
				m_vAlreadyChanged = true;
				m_opUserIDTextControl -> SetText("0.0");
				m_opUserIDTextControl -> SelectAll();
			}
			m_vFrameRateLength = oFrameRate.size();
			m_opUserIDTextControl -> Notify(EM_TEXT_CONTROL_LEFT_MOUSE_BUTTON_DOWN); // Temp fix. Use another id
		}
		else
			m_vAlreadyChanged = false;
		return true;
	case READ_AHEAD_TEXT_CONTROL_CHANGED:
		if(!m_vAlreadyChanged)
		{
			char* vpTemp;
			int32 vStart(0);
			int32 vEnd(0);

			vpTemp = const_cast<char*>(m_opAudioReadAheadTextControl -> Text());
			vpTemp = SetValidValue(vpTemp, 0, 1000);
			m_opAudioReadAheadTextControl -> GetSelection(vStart, vEnd);
			m_opAudioReadAheadTextControl -> SetText(vpTemp);
			m_opAudioReadAheadTextControl -> SetSelection(vStart, vEnd);
		}
		else
			m_vAlreadyChanged = false;
		return true;
	case BUFFER_SIZE_TEXT_CONTROL_CHANGED:
		if(!m_vAlreadyChanged)
		{
			char* vpTemp;
			int32 vStart(0);
			int32 vEnd(0);
	
			vpTemp = const_cast<char*>(m_opAudioBufferSizeTextControl -> Text());
			vpTemp = SetValidValue(vpTemp, 0, 65535);
			m_opAudioBufferSizeTextControl -> GetSelection(vStart, vEnd);
			m_opAudioBufferSizeTextControl -> SetText(vpTemp);
			m_opAudioBufferSizeTextControl -> SetSelection(vStart, vEnd);
		}
		else
			m_vAlreadyChanged = false;
		return true;
	case SNAP_THRESHOLD_TEXT_CONTROL_CHANGED:
		if(!m_vAlreadyChanged)
		{
			char* vpTemp;
			int32 vStart(0);
			int32 vEnd(0);
	
			vpTemp = const_cast<char*>(m_opGUISnapThresholdTextControl -> Text());
			vpTemp = SetValidValue(vpTemp, 1, 50);
			m_opGUISnapThresholdTextControl -> GetSelection(vStart, vEnd);
			m_opGUISnapThresholdTextControl -> SetText(vpTemp);
			m_opGUISnapThresholdTextControl -> SetSelection(vStart, vEnd);
		}
		else
			m_vAlreadyChanged = false;
		return true;
/*	case USE_MIDI_NOTE_CHECKBOX_CHANGED:
		if(p_opSender == m_opMetronomeUseMIDINoteCheckBox)
		{
			if(m_opMetronomeUseMIDINoteCheckBox -> IsChecked())
				EnableMidiNote(true);
			else
				EnableMidiNote(false);
		}
		return true;
*/	// Tab-Key
	case EM_GUI_COMPONENT_TAB_PRESSED:
		m_opDialogWindow -> NextTab();
		return true;
	case EM_GUI_COMPONENT_SHIFT_TAB_PRESSED:
		m_opDialogWindow ->PreviousTab();
		return true;
	case EM_GUI_COMPONENT_GOT_FOCUS:
		eo << "EM_GUI_COMPONENT_GOT_FOCUS" << ef;
		if(p_opSender == m_opDialogWindow)
			m_opTabControl -> SetFocus();
		if(p_opSender != m_opAudioReadAheadTextControl)
		{
			char* vpTemp;
			int32 vStart(0);
			int32 vEnd(0);

			vpTemp = const_cast<char*>(m_opAudioReadAheadTextControl -> Text());
			vpTemp = SetValidValue(vpTemp, 50, 1000);
			m_opAudioReadAheadTextControl -> GetSelection(vStart, vEnd);
			m_opAudioReadAheadTextControl -> SetText(vpTemp);
			m_opAudioReadAheadTextControl -> SetSelection(vStart, vEnd);
		}
		if(p_opSender != m_opAudioBufferSizeTextControl)
		{	
			char* vpTemp;
			int32 vStart(0);
			int32 vEnd(0);
			vpTemp = const_cast<char*>(m_opAudioBufferSizeTextControl -> Text());
			vpTemp = SetValidValue(vpTemp, 128, 65535);
			m_opAudioBufferSizeTextControl -> GetSelection(vStart, vEnd);
			m_opAudioBufferSizeTextControl -> SetText(vpTemp);
			m_opAudioBufferSizeTextControl -> SetSelection(vStart, vEnd);
		}
		m_opDialogWindow -> SetCurrentFocus();
		return true;
	// Close Dialog
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

void SystemSettingsDialog::MoveDialog()
{
	EMRect oWindowRect(m_opParentWindow -> Frame());
	EMRect oDialogRect(m_opDialogWindow -> Frame());
	float vX(oWindowRect.m_vRight - oWindowRect.GetWidth() / 2 - oDialogRect.GetWidth() / 2);
	float vY(oWindowRect.m_vBottom - oWindowRect.GetHeight() / 2 - oDialogRect.GetHeight() / 2);

	MoveDialog(vX, vY);
}

void SystemSettingsDialog::MoveDialog(float p_vX, float p_vY)
{
	m_opDialogWindow -> MoveTo(p_vX, p_vY);
}

void SystemSettingsDialog::SaveSettings()
{
	string oStringSetting;
	int64 vInt64Setting;
	int32 vInt32Setting;
	bool vBoolSetting;
	EMSettingsRepository* oSettings = EMSettingsRepository::Instance();

	// Network settings
	oStringSetting = m_opHostTextControl -> Text();
	oSettings -> SetSetting(SETTING_NETWORK_HOST, EM_SETTING_STRING, &oStringSetting, false, true);
	
	if(m_opProtTCP_RadioButton -> IsChecked())
		vBoolSetting = true;
	else
		vBoolSetting = false;
	oSettings -> SetSetting(SETTING_NETWORK_USE_TCP, EM_SETTING_BOOL, &vBoolSetting, false, true);
	
	vInt32Setting = atoi(m_opProtTCP_PortTextControl -> Text());
	oSettings -> SetSetting(SETTING_NETWORK_TCP_PORT, EM_SETTING_INT32, &vInt32Setting, false, true);
	vInt32Setting = atoi(m_opProtUDP_PortTextControl -> Text());
	oSettings -> SetSetting(SETTING_NETWORK_UDP_PORT, EM_SETTING_INT32, &vInt32Setting, false, true);
	
	if(m_opInternetRadioButton -> IsChecked())
		vBoolSetting = true;
	else
		vBoolSetting = false;
	oSettings -> SetSetting(SETTING_NETWORK_USE_INTERNET, EM_SETTING_BOOL, &vBoolSetting, false, true);
	
	vInt64Setting = ConvertToID_Uint64(m_opUserIDTextControl -> Text());
	oSettings -> SetSetting(SETTING_NETWORK_USER_ID, EM_SETTING_INT64, &vInt64Setting, false, true);
	
	if(m_opSavePasswordCheckBox -> IsChecked())
	{
		oStringSetting = m_opPasswordTextControl -> Text();
		vBoolSetting = true;
	}
	else
	{
		oStringSetting = "";
		vBoolSetting = false;
	}
	oSettings -> SetSetting(SETTING_NETWORK_USER_PASSWORD, EM_SETTING_STRING, &oStringSetting, false, true);
	oSettings -> SetSetting(SETTING_NETWORK_SAVE_PASSWORD, EM_SETTING_BOOL, &vBoolSetting, false, true);

	// Audio Settings
	vInt32Setting = atoi(m_opAudioReadAheadTextControl -> Text());
	oSettings -> SetSetting(SETTING_AUDIO_PROCESSING_AHEAD,	EM_SETTING_INT32, &vInt32Setting, false, true);
	vInt32Setting = atoi(m_opAudioBufferSizeTextControl -> Text());
	oSettings -> SetSetting(SETTING_AUDIO_BUFFER_SIZE, EM_SETTING_INT32, &vInt32Setting, false, true);

	// GUI Settings
	vInt32Setting = atoi(m_opGUISnapThresholdTextControl -> Text());
	oSettings -> SetSetting(SETTING_SNAP_THRESHOLD, EM_SETTING_INT32, &vInt32Setting, true, true);// Last setting notifies system
	
	// Saving settings to disk
	if(!oSettings -> SaveSystemSettings())
		EMMessageBox::Instance() -> DisplayDialog(m_opDialogWindow -> GetNativeView(), "Could not save System settings!", "Error!", EM_DIALOG_BUTTON_OK | EM_DIALOG_ICON_ERROR);
}
/*
void SystemSettingsDialog::SetMidiKeyInComboBox(EMComboBox* p_opComboBox)
{
	int vOctaveCounter = 10;
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
			vKey[0] = 'A' + vKeyCounter;
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
			vKey[0] = 'A' + vKeyCounter;
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
}

void SystemSettingsDialog::SetNumberInComboBox(EMComboBox* p_opComboBox, int16 p_vNumberOfNumbers, int16 p_vStartValue)
{
	char vTemp[6];
	for(int vCounter = 0; vCounter < p_vNumberOfNumbers; vCounter++)
		p_opComboBox -> AddString(itoa(vCounter + p_vStartValue, vTemp, 10));
}
*/
void SystemSettingsDialog::SetParent(EMWindow* p_opWindow)
{
	m_opParentWindow = p_opWindow;
}

void SystemSettingsDialog::SetUserIDString(char* p_vpUserID)
{
	m_opUserIDTextControl -> SetText(p_vpUserID);
}

char* SystemSettingsDialog::SetValidValue(char* p_vpValue, int32 p_vMin, int32 p_vMax)
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