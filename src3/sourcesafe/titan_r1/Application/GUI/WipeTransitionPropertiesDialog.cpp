#include "WipeTransitionPropertiesDialog.h"

#include "CommandIDs.h"
#include "EMButton.h"
#include "EMCheckBox.h"
#include "EMCommandRepository.h"
#include "EMFactory.h"
#include "EMGroupBox.h"
#include "EMLabel.h"
#include "EMMessages.h"
#include "EMTextControl.h"
#include "EMView.h"
#include "EMWindow.h"


#ifdef PLATFORM_WINDOWS
#include <windows.h> // To get modal dialog to work.
#include "EMWinWindow.h" // To get modal dialog to work.
#endif

const uint32 BUTTON_CANCEL = 1;
const uint32 BUTTON_OK = 2;
const uint32 BUTTON_DEFAULT = 3;

WipeTransitionPropertiesDialog::WipeTransitionPropertiesDialog() :
m_opDialogWindow(EMFactory::Instance() -> CreateEMWindow(EMRect(0, 0, 215, 220), "Settings - Wipe Transition", EM_WINDOW_DIALOG)), 
m_opMainView(EMFactory::Instance() -> CreateView(EMRect(0, 0, 215, 220), false)),
m_opBorderGroupBox(EMFactory::Instance() -> CreateGroupBox(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
																  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP,
																  215 - EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
																  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 68),
														   "Border", 0)),
m_opBorderView(EMFactory::Instance() ->CreateView(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
														  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP,
														  215 - EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
														  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 68),
												   false)),
m_opColorLabel(EMFactory::Instance() ->CreateLabel(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
														   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 2,
														   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 30,
														   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 2 + EM_METRICS_NATIVE_LABEL_HEIGHT),
													"Color:", 0)),
m_opSoftnessLabel(EMFactory::Instance() ->CreateLabel(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
														   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 27,
														   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 45,
														   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 27 + EM_METRICS_NATIVE_LABEL_HEIGHT),
													"Softness:", 0)),
m_opWidthLabel(EMFactory::Instance() ->CreateLabel(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 115,
														   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 27,
														   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 115 + 32,
														   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 27 + EM_METRICS_NATIVE_LABEL_HEIGHT),
													"Width:", 0)),
m_opRLabel(EMFactory::Instance() ->CreateLabel(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 40,
														   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 2,
														   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 40 + 12,
														   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 2 + EM_METRICS_NATIVE_LABEL_HEIGHT),
													"R:", 0)),
m_opRTextControl(EMFactory::Instance() ->CreateTextControl(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 55,
																   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP,
																   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 55 + 25,
																   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + EM_METRICS_NATIVE_TEXT_CONTROL_HEIGHT),
															"0", 0, 0, false, EM_ALIGNMENT_RIGHT, true)),
m_opGLabel(EMFactory::Instance() ->CreateLabel(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 90,
														   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 2,
														   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 90 + 12,
														   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 2 + EM_METRICS_NATIVE_LABEL_HEIGHT),
													"G:", 0)),
m_opGTextControl(EMFactory::Instance() ->CreateTextControl(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 105,
																   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP,
																   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 105 + 25,
																   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + EM_METRICS_NATIVE_TEXT_CONTROL_HEIGHT),
															"0", 0, 0, false, EM_ALIGNMENT_RIGHT, true)),
m_opBLabel(EMFactory::Instance() ->CreateLabel(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 140,
														   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 2,
														   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 140 + 12,
														   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 2 + EM_METRICS_NATIVE_LABEL_HEIGHT),
													"B:", 0)),
m_opBTextControl(EMFactory::Instance() ->CreateTextControl(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 155,
																   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP,
																   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 155 + 25,
																   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + EM_METRICS_NATIVE_TEXT_CONTROL_HEIGHT),
															"0", 0, 0, false, EM_ALIGNMENT_RIGHT, true)),
m_opSoftnessTextControl(EMFactory::Instance() ->CreateTextControl(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 50,
																   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 25,
																   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 50 + 30,
																   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 25 + EM_METRICS_NATIVE_TEXT_CONTROL_HEIGHT),
															"0", 0, 0, false, EM_ALIGNMENT_RIGHT, true)),
m_opWidthTextControl(EMFactory::Instance() ->CreateTextControl(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 150,
																   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 25,
																   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 150 + 30,
																   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 25 + EM_METRICS_NATIVE_TEXT_CONTROL_HEIGHT),
															"0", 0, 0, false, EM_ALIGNMENT_RIGHT, true)),

m_opOffsetGroupBox(EMFactory::Instance() -> CreateGroupBox(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
																  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 73,
																  215 / 2 - 3,
																  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 73 + 68),
														   "Offset", 0)),
m_opOffsetView(EMFactory::Instance() ->CreateView(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
														  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 73,
														  215 / 2 - 3,
														  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 73 + 68),
												   false)),
m_opOffXLabel(EMFactory::Instance() ->CreateLabel(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
														   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 2,
														   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 12,
														   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 2 + EM_METRICS_NATIVE_LABEL_HEIGHT),
													"X:", 0)),
m_opOffXTextControl(EMFactory::Instance() ->CreateTextControl(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 50,
																   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP,
																   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 50 + 30,
																   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + EM_METRICS_NATIVE_TEXT_CONTROL_HEIGHT),
															"0", 0, 0, false, EM_ALIGNMENT_RIGHT, true)),
m_opOffYLabel(EMFactory::Instance() ->CreateLabel(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
														   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 27,
														   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 12,
														   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 27 + EM_METRICS_NATIVE_LABEL_HEIGHT),
													"Y:", 0)),
m_opOffYTextControl(EMFactory::Instance() ->CreateTextControl(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 50,
																   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 25,
																   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 50 + 30,
																   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 25 + EM_METRICS_NATIVE_TEXT_CONTROL_HEIGHT),
															"0", 0, 0, false, EM_ALIGNMENT_RIGHT, true)),

m_opReplicateGroupBox(EMFactory::Instance() -> CreateGroupBox(EMRect(215 / 2 + 3,
																  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 73,
																  215 - EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
																  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 73 + 68),
														   "Replicate", 0)),
m_opReplicateView(EMFactory::Instance() ->CreateView(EMRect(215 / 2 + 3,
														  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 73,
														  215 - EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
														  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 73 + 68),
												   false)),
m_opRepXLabel(EMFactory::Instance() ->CreateLabel(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
														   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 2,
														   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 12,
														   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 2 + EM_METRICS_NATIVE_LABEL_HEIGHT),
													"X:", 0)),
m_opRepXTextControl(EMFactory::Instance() ->CreateTextControl(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 50,
																   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP,
																   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 50 + 30,
																   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + EM_METRICS_NATIVE_TEXT_CONTROL_HEIGHT),
															"0", 0, 0, false, EM_ALIGNMENT_RIGHT, true)),
m_opRepYLabel(EMFactory::Instance() ->CreateLabel(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE,
														   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 27,
														   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 12,
														   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 27 + EM_METRICS_NATIVE_LABEL_HEIGHT),
													"Y:", 0)),
m_opRepYTextControl(EMFactory::Instance() ->CreateTextControl(EMRect(EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 50,
																   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 25,
																   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE + 50 + 30,
																   EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP + 25 + EM_METRICS_NATIVE_TEXT_CONTROL_HEIGHT),
															"0", 0, 0, false, EM_ALIGNMENT_RIGHT, true)),

m_opSwapCheckBox(EMFactory::Instance() ->CreateCheckBox(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
																EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 73 + 68 + 6,
																EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 75,
																EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 73 + 68 + 6 + EM_METRICS_NATIVE_CHECKBOX_HEIGHT),
																"Swap Input", 0, 0, 0)),
m_opDefaultButton(EMFactory::Instance() -> CreateButton(EMRect(215 - (EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + EM_METRICS_NATIVE_BUTTON_WIDTH),
															  220 - (EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + EM_METRICS_NATIVE_BUTTON_HEIGHT * 2 + EM_METRICS_NATIVE_BUTTON_SPACE),
															  215 - EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
															  220 - (EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + EM_METRICS_NATIVE_BUTTON_HEIGHT + EM_METRICS_NATIVE_BUTTON_SPACE)),
													"Default", BUTTON_DEFAULT, 0, 0)),
m_opOK_Button(EMFactory::Instance() -> CreateButton(EMRect(215 - (EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + EM_METRICS_NATIVE_BUTTON_WIDTH * 2 + EM_METRICS_NATIVE_BUTTON_SPACE),
														   220 - (EM_METRICS_NATIVE_DIALOG_MARGIN_BOTTOM + EM_METRICS_NATIVE_BUTTON_HEIGHT),
														   215 - (EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + EM_METRICS_NATIVE_BUTTON_WIDTH + EM_METRICS_NATIVE_BUTTON_SPACE),
														   220 - EM_METRICS_NATIVE_DIALOG_MARGIN_BOTTOM),
													"OK", BUTTON_OK, 0, 0)),
m_opCancelButton(EMFactory::Instance() -> CreateButton(EMRect(215 - (EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + EM_METRICS_NATIVE_BUTTON_WIDTH),
															  220 - (EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + EM_METRICS_NATIVE_BUTTON_HEIGHT),
															  215 - EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
															  220 - EM_METRICS_NATIVE_DIALOG_MARGIN_TOP),
													   "Cancel", BUTTON_CANCEL, 0, 0))
{
	m_opDialogWindow -> AddChild(m_opMainView);
	m_opMainView -> AddChild(m_opBorderGroupBox);
	m_opMainView -> AddChild(m_opBorderView);
	m_opMainView -> AddChild(m_opOffsetGroupBox);
	m_opMainView -> AddChild(m_opOffsetView);
	m_opMainView -> AddChild(m_opReplicateGroupBox);
	m_opMainView -> AddChild(m_opReplicateView);
	m_opMainView -> AddChild(m_opSwapCheckBox);
	m_opMainView -> AddChild(m_opDefaultButton);
	m_opMainView -> AddChild(m_opCancelButton);
	m_opMainView -> AddChild(m_opOK_Button);

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
}

WipeTransitionPropertiesDialog::~WipeTransitionPropertiesDialog()
{
	delete m_opDialogWindow;
	delete m_opBorderGroupBox;
	delete m_opBorderView;
	delete m_opMainView;
	delete m_opOffsetGroupBox;
	delete m_opOffsetView;
	delete m_opReplicateGroupBox;
	delete m_opReplicateView;
	delete m_opSwapCheckBox;
	delete m_opDefaultButton;
	delete m_opOK_Button;
	delete m_opCancelButton;

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
}

void WipeTransitionPropertiesDialog::DisplayDialog()
{
	char vTemp[6];
	m_vpTransitionValues = static_cast<int32*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_PERSIST_TRANSITION_SETTINGS, &m_vTransitionID, NULL, NULL));
	m_opRTextControl		-> SetText(itoa(m_vpTransitionValues[0], vTemp, 10));
	m_opGTextControl		-> SetText(itoa(m_vpTransitionValues[1], vTemp, 10));
	m_opBTextControl		-> SetText(itoa(m_vpTransitionValues[2], vTemp, 10));
	m_opSoftnessTextControl -> SetText(itoa(m_vpTransitionValues[3], vTemp, 10));
	m_opWidthTextControl	-> SetText(itoa(m_vpTransitionValues[4], vTemp, 10));
	m_opOffXTextControl		-> SetText(itoa(m_vpTransitionValues[5], vTemp, 10));
	m_opOffYTextControl		-> SetText(itoa(m_vpTransitionValues[6], vTemp, 10));
	m_opRepXTextControl		-> SetText(itoa(m_vpTransitionValues[7], vTemp, 10));
	m_opRepYTextControl		-> SetText(itoa(m_vpTransitionValues[8], vTemp, 10));
	if(m_vpTransitionValues[9] > 0)
		m_opSwapCheckBox -> Check(true);
	else
		m_opSwapCheckBox -> Check(false);

	if(m_opDialogWindow -> Lock())
	{
		m_opDialogWindow -> SetTitle(m_oDialogTitle.c_str());
		m_opDialogWindow -> Show();
#ifdef PLATFORM_WINDOWS
		EnableWindow(static_cast<HWND>(m_opParentWindow -> GetNativeView()), FALSE);
		SetFocus(static_cast<HWND>(m_opOK_Button -> GetNativeView()));
#endif
		m_opDialogWindow -> Unlock();
	}
}

void WipeTransitionPropertiesDialog::HideDialog()
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

void WipeTransitionPropertiesDialog::Init()
{
	m_opDialogWindow -> AddListener(this);
	m_opMainView -> AddListener(this);
	m_opBorderView -> AddListener(this);
	m_opOffsetView -> AddListener(this);
	m_opReplicateView -> AddListener(this);
	if(m_opDialogWindow -> Lock())
	{
		//m_opMainView -> SetViewColor(EMColor(192, 192, 192, 255));
		m_opMainView -> SetDefaultColor();
		//m_opBorderView -> SetViewColor(EMColor(192, 192, 192, 255));
		m_opBorderView -> SetDefaultColor();
		//m_opOffsetView -> SetViewColor(EMColor(192, 192, 192, 255));
		m_opOffsetView -> SetDefaultColor();
		//m_opReplicateView -> SetViewColor(EMColor(192, 192, 192, 255));
		m_opReplicateView -> SetDefaultColor();
		m_opDialogWindow -> Unlock();
	}
	m_opCancelButton -> AddListener(this);
	m_opOK_Button -> AddListener(this);
	m_opDefaultButton -> AddListener(this);
}

bool WipeTransitionPropertiesDialog::MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage)
{
	switch(p_vMessage)
	{
	case BUTTON_CANCEL:
		eo << "Close button pressed" << ef;
	//	eo << "Getting TextControl value: " << m_opTextControl -> Text() << ef;
		HideDialog();
		return true;
	case BUTTON_OK:
		m_vpTransitionValues[0] = atoi(m_opRTextControl -> Text());
		m_vpTransitionValues[1] = atoi(m_opGTextControl -> Text());	
		m_vpTransitionValues[2] = atoi(m_opBTextControl -> Text());
		m_vpTransitionValues[3] = atoi(m_opSoftnessTextControl -> Text());
		m_vpTransitionValues[4] = atoi(m_opWidthTextControl -> Text());
		m_vpTransitionValues[5] = atoi(m_opOffXTextControl -> Text());
		m_vpTransitionValues[6] = atoi(m_opOffYTextControl -> Text());
		m_vpTransitionValues[7] = atoi(m_opRepXTextControl -> Text());
		m_vpTransitionValues[8] = atoi(m_opRepYTextControl -> Text());
		if(m_opSwapCheckBox -> IsChecked())
			m_vpTransitionValues[9] = 1;
		else
			m_vpTransitionValues[9] = 0;
		EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_SET_PERSIST_TRANSITION_SETTINGS, &m_vTransitionID, m_vpTransitionValues, NULL);
		HideDialog();
		return true;
	case BUTTON_DEFAULT:
		{
		eo << "BUTTON_DEFAULT" << ef;
		char vTemp[6];
		int32* vTransitionValues = static_cast<int32*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_DEFAULT_TRANSITION_SETTINGS, &m_vTransitionID, NULL, NULL));
		m_opRTextControl		-> SetText(itoa(vTransitionValues[0], vTemp, 10));
		m_opGTextControl		-> SetText(itoa(vTransitionValues[1], vTemp, 10));
		m_opBTextControl		-> SetText(itoa(vTransitionValues[2], vTemp, 10));
		m_opSoftnessTextControl -> SetText(itoa(vTransitionValues[3], vTemp, 10));
		m_opWidthTextControl	-> SetText(itoa(vTransitionValues[4], vTemp, 10));
		m_opOffXTextControl		-> SetText(itoa(vTransitionValues[5], vTemp, 10));
		m_opOffYTextControl		-> SetText(itoa(vTransitionValues[6], vTemp, 10));
		m_opRepXTextControl		-> SetText(itoa(vTransitionValues[7], vTemp, 10));
		m_opRepYTextControl		-> SetText(itoa(vTransitionValues[8], vTemp, 10));
		if(m_vpTransitionValues[9] > 0)
			m_opSwapCheckBox -> Check(true);
		else
			m_opSwapCheckBox -> Check(false);
		return true;
		}
	case EM_QUIT_PRE_REQUESTED:
		if(p_opSender == m_opDialogWindow)
		{
			eo << "EM_QUIT_PRE_REQUESTED" << ef;
			HideDialog();
			return true;
		}
		break;
	}
	return false;
}

void WipeTransitionPropertiesDialog::MoveDialog()
{
	EMRect oWindowRect(m_opMainAppWindow -> Frame());
	EMRect oDialogRect(m_opDialogWindow -> Frame());
	float vX(oWindowRect.m_vRight - oWindowRect.GetWidth() / 2 - oDialogRect.GetWidth() / 2);
	float vY(oWindowRect.m_vBottom - oWindowRect.GetHeight() / 2 - oDialogRect.GetHeight() / 2);

	MoveDialog(vX, vY);
}

void WipeTransitionPropertiesDialog::MoveDialog(float p_vX, float p_vY)
{
	m_opDialogWindow -> MoveTo(p_vX, p_vY);
}

void WipeTransitionPropertiesDialog::SetMainWindow(EMWindow* p_opWindow)
{
	m_opMainAppWindow = p_opWindow;
}

void WipeTransitionPropertiesDialog::SetParent(EMWindow* p_opWindow)
{
	m_opParentWindow = p_opWindow;
}

void WipeTransitionPropertiesDialog::SetTransition(int32 p_vTransitionID, char* p_vTransitionName)
{
	m_vTransitionID = p_vTransitionID;
	m_oDialogTitle = p_vTransitionName;
}