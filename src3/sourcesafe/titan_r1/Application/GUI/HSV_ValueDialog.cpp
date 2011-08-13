#include "HSV_ValueDialog.h"

#include "CommandIDs.h"
#include "EMButton.h"
#include "EMCommandRepository.h"
#include "EMFactory.h"
#include "EMHSVFilter.h"
#include "EMLabel.h"
#include "EMMessages.h"
#include "EMTextControl.h"
#include "EMTrackbarControl.h"
#include "EMView.h"
#include "EMWindow.h"

#ifdef PLATFORM_WINDOWS
#include <windows.h> // To get modal dialog to work.
#include "EMWinWindow.h" // To get modal dialog to work.
#endif

const uint32 OK_BUTTON = 1;
const uint32 CANCEL_BUTTON = 2;
const uint32 TEXT_CONTROL_CHANGED = 3;
const uint32 TRACKBAR_CHANGED = 4;

HSV_ValueDialog::HSV_ValueDialog() :
m_opDialogWindow(EMFactory::Instance() -> CreateEMWindow(EMRect(0, 0, 305, 91), "HSV Value", EM_WINDOW_DIALOG)), 
m_opMainView(EMFactory::Instance() -> CreateView(EMRect(0, 0, 305, 91), false)),
m_opHueLabel(EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
														 EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 3,
														 EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 50,
														 EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 3 + EM_METRICS_NATIVE_LABEL_HEIGHT),
												  "Hue:", 0)),
m_opHueTrackbarControl(EMFactory::Instance() -> CreateTrackbarControl(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 54,
																			EM_METRICS_NATIVE_DIALOG_MARGIN_TOP,
																			EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 54 + 100,
																			EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + EM_METRICS_NATIVE_TRACKBAR_CONTROL_HEIGHT),
																	 "HueTrackbar", TRACKBAR_CHANGED, 0, 0)),
m_opHueTextControl(EMFactory::Instance() -> CreateTextControl(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 164,
																	 EM_METRICS_NATIVE_DIALOG_MARGIN_TOP,
																	 EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 164 + 27,
																	 EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + EM_METRICS_NATIVE_TEXT_CONTROL_HEIGHT),
															  "0", TEXT_CONTROL_CHANGED, 0, false, EM_ALIGNMENT_RIGHT)),
m_opHueProcentLabel(EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 195,
														 EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 4,
														 EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 195 + 10,
														 EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 4 + EM_METRICS_NATIVE_LABEL_HEIGHT),
												  "%", 0)),
m_opSaturationLabel(EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
																EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 28,
																EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 50,
															EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 28 + EM_METRICS_NATIVE_LABEL_HEIGHT),
														 "Saturation:", 0)),
m_opSaturationTrackbarControl(EMFactory::Instance() -> CreateTrackbarControl(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 54,
																			EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 25,
																			EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 54 + 100,
																			EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 25 + EM_METRICS_NATIVE_TRACKBAR_CONTROL_HEIGHT),
																	 "HueTrackbar", TRACKBAR_CHANGED, 0, 0)),
m_opSaturationTextControl(EMFactory::Instance() -> CreateTextControl(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 164,
																			EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 25,
																			EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 164 + 27,
																		EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 25 + EM_METRICS_NATIVE_TEXT_CONTROL_HEIGHT),
																	 "0", TEXT_CONTROL_CHANGED, 0, false, EM_ALIGNMENT_RIGHT)),
m_opSaturationProcentLabel(EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 195,
																	   EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 29,
																	   EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 195 + 10,
																	   EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 29 + EM_METRICS_NATIVE_LABEL_HEIGHT),
																"%", 0)),
m_opValueLabel(EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
														   EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 53,
														   EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 50,
														   EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 53 + EM_METRICS_NATIVE_LABEL_HEIGHT),
													"Value:", 0)),
m_opValueTrackbarControl(EMFactory::Instance() -> CreateTrackbarControl(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 54,
																			EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 50,
																			EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 54 + 100,
																			EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 50 + EM_METRICS_NATIVE_TRACKBAR_CONTROL_HEIGHT),
																	 "HueTrackbar", TRACKBAR_CHANGED, 0, 0)),
m_opValueTextControl(EMFactory::Instance() -> CreateTextControl(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 164,
																	   EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 50,
																	   EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 164 + 27,
																	   EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 50 + EM_METRICS_NATIVE_TEXT_CONTROL_HEIGHT),
																"0", TEXT_CONTROL_CHANGED, 0, false, EM_ALIGNMENT_RIGHT)),
m_opValueProcentLabel(EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 195,
																  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 54,
																  EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 195 + 10,
																  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 54 + EM_METRICS_NATIVE_LABEL_HEIGHT),
														   "%", 0)),
m_opOK_Button(EMFactory::Instance() -> CreateButton(EMRect(305 - (EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + EM_METRICS_NATIVE_BUTTON_WIDTH),
														   91 - (EM_METRICS_NATIVE_DIALOG_MARGIN_BOTTOM + EM_METRICS_NATIVE_BUTTON_HEIGHT * 2 + EM_METRICS_NATIVE_BUTTON_SPACE),
														   305 - (EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE),
														   91 - (EM_METRICS_NATIVE_DIALOG_MARGIN_BOTTOM + EM_METRICS_NATIVE_BUTTON_HEIGHT + EM_METRICS_NATIVE_BUTTON_SPACE)),
													"OK", OK_BUTTON, 0, 0, NULL, true)),
m_opCancelButton(EMFactory::Instance() -> CreateButton(EMRect(305 - (EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + EM_METRICS_NATIVE_BUTTON_WIDTH),
															  91 - (EM_METRICS_NATIVE_DIALOG_MARGIN_BOTTOM + EM_METRICS_NATIVE_BUTTON_HEIGHT),
															  305 - (EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE),
															  91 - (EM_METRICS_NATIVE_DIALOG_MARGIN_BOTTOM)),
													   "Cancel", CANCEL_BUTTON, 0, 0)),
m_vAlreadyChanged(false)
{
	m_opDialogWindow -> AddChild(m_opMainView);
	m_opMainView -> AddChild(m_opHueLabel);
	m_opMainView -> AddChild(m_opHueTrackbarControl);
	m_opMainView -> AddChild(m_opHueTextControl);
	m_opMainView -> AddChild(m_opHueProcentLabel);
	m_opMainView -> AddChild(m_opSaturationLabel);
	m_opMainView -> AddChild(m_opSaturationTrackbarControl);
	m_opMainView -> AddChild(m_opSaturationTextControl);
	m_opMainView -> AddChild(m_opSaturationProcentLabel);
	m_opMainView -> AddChild(m_opValueLabel);
	m_opMainView -> AddChild(m_opValueTrackbarControl);
	m_opMainView -> AddChild(m_opValueTextControl);
	m_opMainView -> AddChild(m_opValueProcentLabel);
	m_opMainView -> AddChild(m_opOK_Button);
	m_opMainView -> AddChild(m_opCancelButton);
}
HSV_ValueDialog::~HSV_ValueDialog()
{
	delete m_opHueLabel;
	delete m_opHueTrackbarControl;
	delete m_opHueTextControl;
	delete m_opHueProcentLabel;
	delete m_opSaturationLabel;
	delete m_opSaturationTrackbarControl;
	delete m_opSaturationTextControl;
	delete m_opSaturationProcentLabel;
	delete m_opValueLabel;
	delete m_opValueTrackbarControl;
	delete m_opValueTextControl;
	delete m_opValueProcentLabel;
	delete m_opOK_Button;
	delete m_opCancelButton;
	delete m_opDialogWindow;
	delete m_opMainView;
}

void HSV_ValueDialog::DisplayDialog(EMHSVFilter* p_opHSVFilter)
{
	float vHue(0);
	float vSaturation(0);
	float vValue(0);
//	char vTemp[5];
	m_opHSVFilter = p_opHSVFilter;
	m_opHSVFilter -> GetValues(vHue, vSaturation, vValue);
	m_vHue = vHue;
	m_vSaturation = vSaturation;
	m_vValue = vValue;
	m_opHueTrackbarControl -> SetPosition(vHue);
	m_opSaturationTrackbarControl -> SetPosition(vSaturation);
	m_opValueTrackbarControl -> SetPosition(vValue);
//	m_opHueTextControl -> SetText(itoa(static_cast<int>(vHue), vTemp, 10));
//	m_opSaturationTextControl -> SetText(itoa(static_cast<int>(vSaturation), vTemp, 10));
//	m_opValueTextControl -> SetText(itoa(static_cast<int>(vValue), vTemp, 10));

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

void HSV_ValueDialog::HideDialog()
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

void HSV_ValueDialog::Init()
{
	m_opDialogWindow -> AddListener(this);
	m_opMainView -> AddListener(this);
	if(m_opDialogWindow -> Lock())
	{
		//m_opMainView -> SetViewColor(EMColor(0, 192, 0, 255));
		m_opMainView -> SetDefaultColor();
		m_opDialogWindow -> Unlock();
	}

	m_opHueTrackbarControl -> AddListener(this);
	m_opSaturationTrackbarControl -> AddListener(this);
	m_opValueTrackbarControl -> AddListener(this);
	m_opHueTextControl -> AddListener(this);
	m_opSaturationTextControl -> AddListener(this);
	m_opValueTextControl -> AddListener(this);
	m_opOK_Button -> AddListener(this);
	m_opCancelButton -> AddListener(this);

	m_opHueTrackbarControl -> SetRange(-100, 100);
	m_opSaturationTrackbarControl -> SetRange(-100, 100);
	m_opValueTrackbarControl -> SetRange(-100, 100);

	// TabOrder
	m_opDialogWindow -> AddTabComponent(m_opHueTrackbarControl);
	m_opDialogWindow -> AddTabComponent(m_opSaturationTrackbarControl);
	m_opDialogWindow -> AddTabComponent(m_opValueTrackbarControl);
	m_opDialogWindow -> AddTabComponent(m_opHueTextControl);
	m_opDialogWindow -> AddTabComponent(m_opSaturationTextControl);
	m_opDialogWindow -> AddTabComponent(m_opValueTextControl);
	m_opDialogWindow -> AddTabComponent(m_opOK_Button);
	m_opDialogWindow -> AddTabComponent(m_opCancelButton);
}

bool HSV_ValueDialog::MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage)
{
	switch(p_vMessage)
	{
	case EM_GUI_COMPONENT_ESC_PRESSED:
	case CANCEL_BUTTON:
		m_opHSVFilter -> SetValues(m_vHue, m_vSaturation, m_vValue);
	case EM_GUI_COMPONENT_ENTER_PRESSED:
	case OK_BUTTON:
		HideDialog();
		return true;
	case TEXT_CONTROL_CHANGED:
		if(m_vAlreadyChanged == false)
		{
			char* vpTemp;
			int32 vStart(0);
			int32 vEnd(0);
			if(p_opSender == m_opHueTextControl)
			{
				vpTemp = const_cast<char*>(m_opHueTextControl -> Text());
				vpTemp = SetValidValue(vpTemp);
				m_opHueTextControl -> GetSelection(vStart, vEnd);
				m_opHueTextControl -> SetText(vpTemp);
				m_opHueTrackbarControl -> SetPosition(atoi(vpTemp));
				m_opHueTextControl -> SetSelection(vStart, vEnd);
			}
			if(p_opSender == m_opSaturationTextControl)
			{
				vpTemp = const_cast<char*>(m_opSaturationTextControl -> Text());
				vpTemp = SetValidValue(vpTemp);
				m_opSaturationTextControl -> GetSelection(vStart, vEnd);
				m_opSaturationTextControl -> SetText(vpTemp);
				m_opSaturationTrackbarControl -> SetPosition(atoi(vpTemp));	
				m_opSaturationTextControl -> SetSelection(vStart, vEnd);
			}
			if(p_opSender == m_opValueTextControl)
			{
				vpTemp = const_cast<char*>(m_opValueTextControl -> Text());
				vpTemp = SetValidValue(vpTemp);
				m_opValueTextControl -> GetSelection(vStart, vEnd);
				m_opValueTextControl -> SetText(vpTemp);
				m_opValueTrackbarControl -> SetPosition(atoi(vpTemp));
				m_opValueTextControl -> SetSelection(vStart, vEnd);
			}
			m_opHSVFilter ->SetValues(atoi(m_opHueTextControl -> Text()), atoi(m_opSaturationTextControl -> Text()), atoi(m_opValueTextControl -> Text()));
			EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_REFRESH_PREVIEW);
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
			if(p_opSender == m_opHueTrackbarControl)
				m_opHueTextControl -> SetText(itoa(m_opHueTrackbarControl -> GetPosition(), vTemp, 10));
			else if(p_opSender == m_opSaturationTrackbarControl)
				m_opSaturationTextControl -> SetText(itoa(m_opSaturationTrackbarControl -> GetPosition(), vTemp, 10));
			else if(p_opSender == m_opValueTrackbarControl)
				m_opValueTextControl -> SetText(itoa(m_opValueTrackbarControl -> GetPosition(), vTemp, 10));
			else
				return false;
			return true;
		}
	case EM_GUI_COMPONENT_TAB_PRESSED:
		m_opDialogWindow -> NextTab();
		return true;
	case EM_GUI_COMPONENT_SHIFT_TAB_PRESSED:
		m_opDialogWindow -> PreviousTab();
		return true;
	case EM_GUI_COMPONENT_GOT_FOCUS:
		if(p_opSender == m_opDialogWindow)
			m_opHueTextControl -> SetFocus();
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

void HSV_ValueDialog::MoveDialog()
{
	EMRect oWindowRect(m_opParentWindow -> Frame());
	EMRect oDialogRect(m_opDialogWindow -> Frame());
	float vX(oWindowRect.m_vRight - oWindowRect.GetWidth() / 2 - oDialogRect.GetWidth() / 2);
	float vY(oWindowRect.m_vBottom - oWindowRect.GetHeight() / 2 - oDialogRect.GetHeight() / 2);

	MoveDialog(vX, vY);
}

void HSV_ValueDialog::MoveDialog(float p_vX, float p_vY)
{
	m_opDialogWindow -> MoveTo(p_vX, p_vY);
}

void HSV_ValueDialog::SetParent(EMWindow* p_opWindow)
{
	m_opParentWindow = p_opWindow;
}

char* HSV_ValueDialog::SetValidValue(char* p_vpValue)
{
	eo << p_vpValue << ef;
	m_vAlreadyChanged = true;

	int vValue(atoi(p_vpValue));
	if(vValue > 100)
		vValue = 100;
	if(vValue < -100)
		vValue = -100;
	return itoa(vValue, m_vTemp, 10);

}