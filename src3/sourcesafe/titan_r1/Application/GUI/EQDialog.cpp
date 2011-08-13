#include "EQDialog.h"

#include "CommandIDs.h"
#include "EMButton.h"
#include "EMCommandRepository.h"
#include "EMEqualizerInterface.h"
#include "EMFactory.h"
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

EQDialog::EQDialog() :
m_opDialogWindow(EMFactory::Instance() -> CreateEMWindow(EMRect(0, 0, 174, 198), "EQ", EM_WINDOW_DIALOG)), 
m_opMainView(EMFactory::Instance() -> CreateView(EMRect(0, 0, 174, 198), false)),
m_opBassLabel(EMFactory::Instance() ->CreateLabel(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
														   EM_METRICS_NATIVE_DIALOG_MARGIN_TOP,
														   EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 45,
														   EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + EM_METRICS_NATIVE_LABEL_HEIGHT),
													"Bass", 0, EM_ALIGNMENT_CENTER)),
m_opMiddleLabel(EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 54,
														  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP,
														  EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 54 + 45,
														  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + EM_METRICS_NATIVE_LABEL_HEIGHT),
												   "Middle", 0, EM_ALIGNMENT_CENTER)),
m_opTrebleLabel(EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 108,
														 EM_METRICS_NATIVE_DIALOG_MARGIN_TOP,
														 EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 108 + 45,
														 EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + EM_METRICS_NATIVE_LABEL_HEIGHT),
												  "Treble", 0, EM_ALIGNMENT_CENTER)),
m_opBassTextControl(EMFactory::Instance() -> CreateTextControl(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 9,
																	   EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 20,
																	   EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 9 + 27,
																	   EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 20 + EM_METRICS_NATIVE_TEXT_CONTROL_HEIGHT),
																"0", TEXT_CONTROL_CHANGED, 0, false, EM_ALIGNMENT_CENTER)),
m_opMiddleTextControl(EMFactory::Instance() -> CreateTextControl(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 63,
																	  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 20,
																	  EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 63 + 27,
																	  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 20 + EM_METRICS_NATIVE_TEXT_CONTROL_HEIGHT),
															   "0", TEXT_CONTROL_CHANGED, 0, false, EM_ALIGNMENT_CENTER)),
m_opTrebleTextControl(EMFactory::Instance() -> CreateTextControl(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 117,
																	 EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 20,
																	 EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 117 + 27,
																	 EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 20 + EM_METRICS_NATIVE_TEXT_CONTROL_HEIGHT),
															  "0", TEXT_CONTROL_CHANGED, 0, false, EM_ALIGNMENT_CENTER)),
m_opBassTrackbarControl(EMFactory::Instance() ->CreateTrackbarControl(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 5,
																			  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 45,
																			  EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 5 + 34, //EM_METRICS_NATIVE_TRACKBAR_CONTROL_HEIGHT,
																			  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 45 + 100),
																	   "BassTrackbar", TRACKBAR_CHANGED, 0, 0, EM_BOTH_TICKS, true)),
m_opMiddleTrackbarControl(EMFactory::Instance() ->CreateTrackbarControl(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 59,
																			  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 45,
																			  EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 59 + 34, //EM_METRICS_NATIVE_TRACKBAR_CONTROL_HEIGHT,
																			  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 45 + 100),
																	   "BassTrackbar", TRACKBAR_CHANGED, 0, 0, EM_BOTH_TICKS, true)),
m_opTrebleTrackbarControl(EMFactory::Instance() ->CreateTrackbarControl(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 113,
																			  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 45,
																			  EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 113 + 34, //EM_METRICS_NATIVE_TRACKBAR_CONTROL_HEIGHT,
																			  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 45 + 100),
																	   "BassTrackbar", TRACKBAR_CHANGED, 0, 0, EM_BOTH_TICKS, true)),
m_opOK_Button(EMFactory::Instance() -> CreateButton(EMRect(174 - (EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + EM_METRICS_NATIVE_BUTTON_WIDTH * 2 + EM_METRICS_NATIVE_BUTTON_SPACE),
														   198 - (EM_METRICS_NATIVE_DIALOG_MARGIN_BOTTOM + EM_METRICS_NATIVE_BUTTON_HEIGHT),
														   174 - (EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + EM_METRICS_NATIVE_BUTTON_WIDTH + EM_METRICS_NATIVE_BUTTON_SPACE),
														   198 - (EM_METRICS_NATIVE_DIALOG_MARGIN_BOTTOM)),
													"OK", OK_BUTTON, 0, 0, NULL, true)),
m_opCancelButton(EMFactory::Instance() -> CreateButton(EMRect(174 - (EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + EM_METRICS_NATIVE_BUTTON_WIDTH),
															  198 - (EM_METRICS_NATIVE_DIALOG_MARGIN_BOTTOM + EM_METRICS_NATIVE_BUTTON_HEIGHT),
															  174 - (EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE),
															  198 - (EM_METRICS_NATIVE_DIALOG_MARGIN_BOTTOM)),
													   "Cancel", CANCEL_BUTTON, 0, 0)),
m_vAlreadyChanged(false)
{
	m_opDialogWindow -> AddChild(m_opMainView);
	m_opMainView -> AddChild(m_opBassLabel);
	m_opMainView -> AddChild(m_opMiddleLabel);
	m_opMainView -> AddChild(m_opTrebleLabel);
	m_opMainView -> AddChild(m_opBassTextControl);
	m_opMainView -> AddChild(m_opMiddleTextControl);
	m_opMainView -> AddChild(m_opTrebleTextControl);
	m_opMainView -> AddChild(m_opBassTrackbarControl);
	m_opMainView -> AddChild(m_opMiddleTrackbarControl);
	m_opMainView -> AddChild(m_opTrebleTrackbarControl);
	m_opMainView -> AddChild(m_opOK_Button);
	m_opMainView -> AddChild(m_opCancelButton);
}
EQDialog::~EQDialog()
{
	delete m_opBassLabel;
	delete m_opTrebleLabel;
	delete m_opMiddleLabel;
	delete m_opBassTextControl;
	delete m_opTrebleTextControl;
	delete m_opMiddleTextControl;
	delete m_opBassTrackbarControl;
	delete m_opMiddleTrackbarControl;
	delete m_opTrebleTrackbarControl;
	delete m_opOK_Button;
	delete m_opCancelButton;

	delete m_opDialogWindow;
	delete m_opMainView;
}

void EQDialog::DisplayDialog()
{	

	m_opEQ -> GetSettings(m_vBass, m_vMiddle, m_vTreble);
	m_opBassTrackbarControl -> SetPosition(m_vBass);
	m_opMiddleTrackbarControl -> SetPosition(m_vMiddle);
	m_opTrebleTrackbarControl -> SetPosition(m_vTreble);

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

void EQDialog::HideDialog()
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

void EQDialog::Init()
{
	m_opDialogWindow -> AddListener(this);
	m_opMainView -> AddListener(this);
	if(m_opDialogWindow -> Lock())
	{
		//m_opMainView -> SetViewColor(EMColor(0, 192, 0, 255));
		m_opMainView -> SetDefaultColor();
		m_opDialogWindow -> Unlock();
	}

	m_opBassTextControl -> AddListener(this);
	m_opBassTrackbarControl -> AddListener(this);
	m_opMiddleTextControl -> AddListener(this);
	m_opMiddleTrackbarControl -> AddListener(this);
	m_opTrebleTextControl -> AddListener(this);
	m_opTrebleTrackbarControl -> AddListener(this);
	m_opOK_Button -> AddListener(this);
	m_opCancelButton -> AddListener(this);

	m_opBassTrackbarControl -> SetRange(-63, 63);
	m_opBassTrackbarControl -> SetTick(0);
	m_opMiddleTrackbarControl -> SetRange(-63, 63);
	m_opMiddleTrackbarControl -> SetTick(0);
	m_opTrebleTrackbarControl -> SetRange(-63, 63);
	m_opTrebleTrackbarControl -> SetTick(0);

	// TabOrder
	m_opDialogWindow -> AddTabComponent(m_opBassTextControl);
	m_opDialogWindow -> AddTabComponent(m_opBassTrackbarControl);
	m_opDialogWindow -> AddTabComponent(m_opMiddleTextControl);
	m_opDialogWindow -> AddTabComponent(m_opMiddleTrackbarControl);
	m_opDialogWindow -> AddTabComponent(m_opTrebleTextControl);
	m_opDialogWindow -> AddTabComponent(m_opTrebleTrackbarControl);
	m_opDialogWindow -> AddTabComponent(m_opOK_Button);
	m_opDialogWindow -> AddTabComponent(m_opCancelButton);
}

bool EQDialog::MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage)
{
	switch(p_vMessage)
	{
	case EM_GUI_COMPONENT_ENTER_PRESSED:
	case OK_BUTTON:
		m_vBass = m_opBassTrackbarControl -> GetPosition();
		m_vMiddle = m_opMiddleTrackbarControl -> GetPosition();
		m_vTreble = m_opTrebleTrackbarControl -> GetPosition();
	case EM_GUI_COMPONENT_ESC_PRESSED:
	case CANCEL_BUTTON:
		m_opEQ -> SetSettings(m_vBass, m_vMiddle, m_vTreble);
		HideDialog();
		return true;
	case TEXT_CONTROL_CHANGED:
		if(m_vAlreadyChanged == false)
		{
			char* vpTemp;
			int32 vStart(0);
			int32 vEnd(0);
			if(p_opSender == m_opBassTextControl)
			{
				vpTemp = const_cast<char*>(m_opBassTextControl -> Text());
				vpTemp = SetValidValue(vpTemp);
				m_opBassTextControl -> GetSelection(vStart, vEnd);
				m_opBassTextControl -> SetText(vpTemp);
				m_opBassTrackbarControl -> SetPosition(atoi(vpTemp) * (-1));
				m_opBassTextControl -> SetSelection(vStart, vEnd);
			}
			if(p_opSender == m_opMiddleTextControl)
			{
				vpTemp = const_cast<char*>(m_opMiddleTextControl -> Text());
				vpTemp = SetValidValue(vpTemp);
				m_opMiddleTextControl -> GetSelection(vStart, vEnd);
				m_opMiddleTextControl -> SetText(vpTemp);
				m_opMiddleTrackbarControl -> SetPosition(atoi(vpTemp) * (-1));
				m_opMiddleTextControl -> SetSelection(vStart, vEnd);
			}
			if(p_opSender == m_opTrebleTextControl)
			{
				vpTemp = const_cast<char*>(m_opTrebleTextControl -> Text());
				vpTemp = SetValidValue(vpTemp);
				m_opTrebleTextControl -> GetSelection(vStart, vEnd);
				m_opTrebleTextControl -> SetText(vpTemp);
				m_opTrebleTrackbarControl -> SetPosition(atoi(vpTemp) * (-1));
				m_opTrebleTextControl -> SetSelection(vStart, vEnd);
			}
			// FIXME: If the next row is "on" the TextControls doesn't get updated as they do if the next row is a "comment". 
			m_opEQ -> SetSettings(m_opBassTrackbarControl -> GetPosition(), m_opMiddleTrackbarControl -> GetPosition(), m_opTrebleTrackbarControl -> GetPosition());
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
			if(p_opSender == m_opBassTrackbarControl)
				m_opBassTextControl -> SetText(itoa(m_opBassTrackbarControl -> GetPosition() * (-1), vTemp, 10));
			else if(p_opSender == m_opMiddleTrackbarControl)
				m_opMiddleTextControl -> SetText(itoa(m_opMiddleTrackbarControl -> GetPosition() * (-1), vTemp, 10));
			else if(p_opSender == m_opTrebleTrackbarControl)
				m_opTrebleTextControl -> SetText(itoa(m_opTrebleTrackbarControl -> GetPosition() * (-1), vTemp, 10));
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
			m_opOK_Button -> SetFocus();
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

void EQDialog::MoveDialog()
{
	EMRect oWindowRect(m_opParentWindow -> Frame());
	EMRect oDialogRect(m_opDialogWindow -> Frame());
	float vX(oWindowRect.m_vRight - oWindowRect.GetWidth() / 2 - oDialogRect.GetWidth() / 2);
	float vY(oWindowRect.m_vBottom - oWindowRect.GetHeight() / 2 - oDialogRect.GetHeight() / 2);

	MoveDialog(vX, vY);
}

void EQDialog::MoveDialog(float p_vX, float p_vY)
{
	m_opDialogWindow -> MoveTo(p_vX, p_vY);
}

void EQDialog::SetEqualizer(EMEqualizerInterface* p_opEQ)
{
	m_opEQ = p_opEQ;
}

void EQDialog::SetParent(EMWindow* p_opWindow)
{
	m_opParentWindow = p_opWindow;
}

char* EQDialog::SetValidValue(char* p_vpValue)
{
	eo << p_vpValue << ef;
	m_vAlreadyChanged = true;

	int vValue(atoi(p_vpValue));
	if(vValue > 63)
		vValue = 63;
	if(vValue < -63)
		vValue = -63;
	return itoa(vValue, m_vTemp, 10);

}