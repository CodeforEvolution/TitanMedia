#include "ColorBalanceDialog.h"

#include "CommandIDs.h"
#include "EMButton.h"
#include "EMCommandRepository.h"
#include "EMFactory.h"
#include "EMLabel.h"
#include "EMMessages.h"
#include "EMRGBFilter.h"
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

ColorBalanceDialog::ColorBalanceDialog() :
m_opDialogWindow(EMFactory::Instance() -> CreateEMWindow(EMRect(0, 0, 280, 91), "Color Balance", EM_WINDOW_DIALOG)), 
m_opMainView(EMFactory::Instance() -> CreateView(EMRect(0, 0, 280, 91), false)),
m_opRedLabel(EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
														 EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 3,
														 EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 35,
														 EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 3 + EM_METRICS_NATIVE_LABEL_HEIGHT),
												  "Red:", 0)),
m_opRedTrackbarControl(EMFactory::Instance() -> CreateTrackbarControl(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 39,
																			EM_METRICS_NATIVE_DIALOG_MARGIN_TOP,
																			EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 39 + 100,
																			EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + EM_METRICS_NATIVE_TRACKBAR_CONTROL_HEIGHT),
																	 "RedTrackbar", TRACKBAR_CHANGED, 0, 0)),
m_opRedTextControl(EMFactory::Instance() -> CreateTextControl(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 149,
																	 EM_METRICS_NATIVE_DIALOG_MARGIN_TOP,
																	 EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 149 + 27,
																	 EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + EM_METRICS_NATIVE_TEXT_CONTROL_HEIGHT),
															  "0", TEXT_CONTROL_CHANGED, 0, false, EM_ALIGNMENT_RIGHT)),
m_opGreenLabel(EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
														   EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 28,
														   EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 35,
														   EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 28 + EM_METRICS_NATIVE_LABEL_HEIGHT),
													"Green:", 0)),
m_opGreenTrackbarControl(EMFactory::Instance() -> CreateTrackbarControl(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 39,
																			EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 25,
																			EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 39 + 100,
																			EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 25 + EM_METRICS_NATIVE_TRACKBAR_CONTROL_HEIGHT),
																	 "GreenTrackbar", TRACKBAR_CHANGED, 0, 0)),
m_opGreenTextControl(EMFactory::Instance() -> CreateTextControl(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 149,
																	   EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 25,
																	   EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 149 + 27,
																	   EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 25 + EM_METRICS_NATIVE_TEXT_CONTROL_HEIGHT),
																"0", TEXT_CONTROL_CHANGED, 0, false, EM_ALIGNMENT_RIGHT)),
m_opBlueLabel(EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
														  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 53,
														  EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 35,
														  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 53 + EM_METRICS_NATIVE_LABEL_HEIGHT),
												   "Blue:", 0)),
m_opBlueTrackbarControl(EMFactory::Instance() -> CreateTrackbarControl(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 39,
																			EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 50,
																			EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 39 + 100,
																			EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 50 + EM_METRICS_NATIVE_TRACKBAR_CONTROL_HEIGHT),
																	 "BlueTrackbar", TRACKBAR_CHANGED, 0, 0)),
m_opBlueTextControl(EMFactory::Instance() -> CreateTextControl(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 149,
																	  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 50,
																	  EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 149 + 27,
																	  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 50 + EM_METRICS_NATIVE_TEXT_CONTROL_HEIGHT),
															   "0", TEXT_CONTROL_CHANGED, 0, false, EM_ALIGNMENT_RIGHT)),
m_opOK_Button(EMFactory::Instance() -> CreateButton(EMRect(280 - (EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + EM_METRICS_NATIVE_BUTTON_WIDTH),
														   91 - (EM_METRICS_NATIVE_DIALOG_MARGIN_BOTTOM + EM_METRICS_NATIVE_BUTTON_HEIGHT * 2 + EM_METRICS_NATIVE_BUTTON_SPACE),
														   280 - (EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE),
														   91 - (EM_METRICS_NATIVE_DIALOG_MARGIN_BOTTOM + EM_METRICS_NATIVE_BUTTON_HEIGHT + EM_METRICS_NATIVE_BUTTON_SPACE)),
													"OK", OK_BUTTON, 0, 0, NULL, true)),
m_opCancelButton(EMFactory::Instance() -> CreateButton(EMRect(280 - (EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + EM_METRICS_NATIVE_BUTTON_WIDTH),
															  91 - (EM_METRICS_NATIVE_DIALOG_MARGIN_BOTTOM + EM_METRICS_NATIVE_BUTTON_HEIGHT),
															  280 - (EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE),
															  91 - (EM_METRICS_NATIVE_DIALOG_MARGIN_BOTTOM)),
													   "Cancel", CANCEL_BUTTON, 0, 0)),
m_vAlreadyChanged(false)
{
	m_opDialogWindow -> AddChild(m_opMainView);
	m_opMainView -> AddChild(m_opRedLabel);
	m_opMainView -> AddChild(m_opRedTextControl);
	m_opMainView -> AddChild(m_opRedTrackbarControl);
	m_opMainView -> AddChild(m_opGreenLabel);
	m_opMainView -> AddChild(m_opGreenTextControl);
	m_opMainView -> AddChild(m_opGreenTrackbarControl);
	m_opMainView -> AddChild(m_opBlueLabel);
	m_opMainView -> AddChild(m_opBlueTextControl);
	m_opMainView -> AddChild(m_opBlueTrackbarControl);
	m_opMainView -> AddChild(m_opOK_Button);
	m_opMainView -> AddChild(m_opCancelButton);
}
ColorBalanceDialog::~ColorBalanceDialog()
{
	delete m_opRedLabel;
	delete m_opRedTextControl;
	delete m_opRedTrackbarControl;
	delete m_opGreenLabel;
	delete m_opGreenTextControl;
	delete m_opGreenTrackbarControl;
	delete m_opBlueLabel;
	delete m_opBlueTextControl;
	delete m_opBlueTrackbarControl;
	delete m_opOK_Button;
	delete m_opCancelButton;
	delete m_opDialogWindow;
	delete m_opMainView;
}

void ColorBalanceDialog::DisplayDialog(EMRGBFilter* p_opRGBFilter)
{	
	float vRed;
	float vGreen;
	float vBlue;
//	char vTemp[5];
	m_opRGBFilter = p_opRGBFilter;
	m_opRGBFilter -> GetValues(vRed, vGreen, vBlue);
	m_vRed = vRed;
	m_vGreen = vGreen;
	m_vBlue = vBlue;
	m_opRedTrackbarControl -> SetPosition(vRed);
	m_opGreenTrackbarControl -> SetPosition(vGreen);
	m_opBlueTrackbarControl -> SetPosition(vBlue);
	//m_opRedTextControl -> SetText(itoa(static_cast<int>(vRed), vTemp, 10));
	//m_opGreenTextControl -> SetText(itoa(static_cast<int>(vGreen), vTemp, 10));
	//m_opBlueTextControl -> SetText(itoa(static_cast<int>(vBlue), vTemp, 10));

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

void ColorBalanceDialog::HideDialog()
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

void ColorBalanceDialog::Init()
{
	m_opDialogWindow -> AddListener(this);
	m_opMainView -> AddListener(this);
	if(m_opDialogWindow -> Lock())
	{
		//m_opMainView -> SetViewColor(EMColor(0, 192, 0, 255));
		m_opMainView -> SetDefaultColor();
		m_opDialogWindow -> Unlock();
	}

	m_opRedTrackbarControl -> AddListener(this);
	m_opGreenTrackbarControl -> AddListener(this);
	m_opBlueTrackbarControl -> AddListener(this);
	m_opRedTextControl -> AddListener(this);
	m_opGreenTextControl -> AddListener(this);
	m_opBlueTextControl -> AddListener(this);
	m_opOK_Button -> AddListener(this);
	m_opCancelButton -> AddListener(this);

	m_opRedTrackbarControl -> SetRange(-255, 255);
	m_opGreenTrackbarControl -> SetRange(-255, 255);
	m_opBlueTrackbarControl -> SetRange(-255, 255);

	// TabOrder
	m_opDialogWindow -> AddTabComponent(m_opRedTrackbarControl);
	m_opDialogWindow -> AddTabComponent(m_opGreenTrackbarControl);
	m_opDialogWindow -> AddTabComponent(m_opBlueTrackbarControl);
	m_opDialogWindow -> AddTabComponent(m_opRedTextControl);
	m_opDialogWindow -> AddTabComponent(m_opGreenTextControl);
	m_opDialogWindow -> AddTabComponent(m_opBlueTextControl);
	m_opDialogWindow -> AddTabComponent(m_opOK_Button);
	m_opDialogWindow -> AddTabComponent(m_opCancelButton);
}

bool ColorBalanceDialog::MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage)
{
	switch(p_vMessage)
	{
	case EM_GUI_COMPONENT_ESC_PRESSED:
	case CANCEL_BUTTON:
		m_opRGBFilter -> SetValues(m_vRed, m_vGreen, m_vBlue);
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
			if(p_opSender == m_opRedTextControl)
			{
				vpTemp = const_cast<char*>(m_opRedTextControl -> Text());
				vpTemp = SetValidValue(vpTemp);
				m_opRedTextControl -> GetSelection(vStart, vEnd);
				m_opRedTextControl -> SetText(vpTemp);
				m_opRedTrackbarControl -> SetPosition(atoi(vpTemp));
				m_opRedTextControl -> SetSelection(vStart, vEnd);
			}
			if(p_opSender == m_opGreenTextControl)
			{
				vpTemp = const_cast<char*>(m_opGreenTextControl -> Text());
				vpTemp = SetValidValue(vpTemp);
				m_opGreenTextControl -> GetSelection(vStart, vEnd);
				m_opGreenTextControl -> SetText(vpTemp);
				m_opGreenTrackbarControl -> SetPosition(atoi(vpTemp));
				m_opGreenTextControl -> SetSelection(vStart, vEnd);
			}
			if(p_opSender == m_opBlueTextControl)
			{
				vpTemp = const_cast<char*>(m_opBlueTextControl -> Text());
				vpTemp = SetValidValue(vpTemp);
				m_opBlueTextControl -> GetSelection(vStart, vEnd);
				m_opBlueTextControl -> SetText(vpTemp);
				m_opBlueTrackbarControl -> SetPosition(atoi(vpTemp));
				m_opBlueTextControl -> SetSelection(vStart, vEnd);
			}
			m_opRGBFilter -> SetValues(atoi(m_opRedTextControl -> Text()), atoi(m_opGreenTextControl -> Text()), atoi(m_opBlueTextControl -> Text()));
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
			if(p_opSender == m_opRedTrackbarControl)
				m_opRedTextControl -> SetText(itoa(m_opRedTrackbarControl -> GetPosition(), vTemp, 10));
			else if(p_opSender == m_opGreenTrackbarControl)
				m_opGreenTextControl -> SetText(itoa(m_opGreenTrackbarControl -> GetPosition(), vTemp, 10));
			else if(p_opSender == m_opBlueTrackbarControl)
				m_opBlueTextControl -> SetText(itoa(m_opBlueTrackbarControl -> GetPosition(), vTemp, 10));
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
			m_opRedTextControl -> SetFocus();
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

void ColorBalanceDialog::MoveDialog()
{
	EMRect oWindowRect(m_opParentWindow -> Frame());
	EMRect oDialogRect(m_opDialogWindow -> Frame());
	float vX(oWindowRect.m_vRight - oWindowRect.GetWidth() / 2 - oDialogRect.GetWidth() / 2);
	float vY(oWindowRect.m_vBottom - oWindowRect.GetHeight() / 2 - oDialogRect.GetHeight() / 2);

	MoveDialog(vX, vY);
}

void ColorBalanceDialog::MoveDialog(float p_vX, float p_vY)
{
	m_opDialogWindow -> MoveTo(p_vX, p_vY);
}

void ColorBalanceDialog::SetParent(EMWindow* p_opWindow)
{
	m_opParentWindow = p_opWindow;
}

char* ColorBalanceDialog::SetValidValue(char* p_vpValue)
{
	eo << p_vpValue << ef;
	m_vAlreadyChanged = true;

	int vValue(atoi(p_vpValue));
	if(vValue > 255)
		vValue = 255;
	if(vValue < -255)
		vValue = -255;
	return itoa(vValue, m_vTemp, 10);

}