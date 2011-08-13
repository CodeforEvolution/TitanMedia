#include "NormalizeDialog.h"

#include "CommandAudioNormalize.h"
#include "EMButton.h"
#include "EMFactory.h"
#include "EMLabel.h"
#include "EMMessages.h"
#include "EMTextControl.h"
#include "EMView.h"
#include "EMWindow.h"

#ifdef PLATFORM_WINDOWS
#include <windows.h> // To get modal dialog to work.
#include "EMWinWindow.h" // To get modal dialog to work.
#endif

const uint32 EM_BUTTON_OK = 1;
const uint32 EM_BUTTON_CANCEL = 2;
const uint32 TEXT_CONTROL_CHANGED = 3;

NormalizeDialog::NormalizeDialog() :
m_opDialogWindow(EMFactory::Instance() -> CreateEMWindow(EMRect(0, 0, 219, 73), "Normalize", EM_WINDOW_DIALOG)), 
m_opMainView(EMFactory::Instance() -> CreateView(EMRect(0, 0, 219, 73), false)),
m_opOK_Button(EMFactory::Instance() ->CreateButton(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 124,
														   EM_METRICS_NATIVE_DIALOG_MARGIN_TOP,
														   EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 124 + EM_METRICS_NATIVE_BUTTON_WIDTH,
														   EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + EM_METRICS_NATIVE_BUTTON_HEIGHT),
													"OK", EM_BUTTON_OK, 0, 0, NULL, true)),
m_opCancelButton(EMFactory::Instance() -> CreateButton(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 124,
															  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + EM_METRICS_NATIVE_BUTTON_HEIGHT + EM_METRICS_NATIVE_BUTTON_SPACE,
															  EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 124 + EM_METRICS_NATIVE_BUTTON_WIDTH,
															  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + EM_METRICS_NATIVE_BUTTON_HEIGHT + EM_METRICS_NATIVE_BUTTON_SPACE + EM_METRICS_NATIVE_BUTTON_HEIGHT),
													   "Cancel", EM_BUTTON_CANCEL, 0, 0)),
m_opLabel(EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
													  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 2,
													  EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 82,
													  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 3 + EM_METRICS_NATIVE_LABEL_HEIGHT),
											   "Normalize to (dB):", 0)),
m_opTextControl(EMFactory::Instance() -> CreateTextControl(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 89,
																  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP,
																  EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 89 + 25,
																  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + EM_METRICS_NATIVE_TEXT_CONTROL_HEIGHT),
														   "-1", TEXT_CONTROL_CHANGED, 0, false, EM_ALIGNMENT_RIGHT)),
m_vAlreadyChanged(false)
{
	m_opDialogWindow -> AddChild(m_opMainView);
	m_opMainView -> AddChild(m_opOK_Button);
	m_opMainView -> AddChild(m_opCancelButton);
	m_opMainView -> AddChild(m_opLabel);
	m_opMainView -> AddChild(m_opTextControl);
}
NormalizeDialog::~NormalizeDialog()
{
	delete m_opDialogWindow;
	delete m_opMainView;
	delete m_opOK_Button;
	delete m_opCancelButton;
	delete m_opLabel;
	delete m_opTextControl;
}

void NormalizeDialog::DisplayDialog(void* p_opCommandAudioNormalize)
{
	m_opCommandAudioNormalize = static_cast<CommandAudioNormalize*>(p_opCommandAudioNormalize);
	if(m_opDialogWindow -> Lock())
	{
		m_opDialogWindow -> Show();
#ifdef PLATFORM_WINDOWS
		EnableWindow(dynamic_cast<EMWinWindow*>(m_opDialogWindow) -> GetMainWindowHandle(), FALSE);
		SetFocus(static_cast<HWND>(m_opTextControl -> GetNativeView()));
#endif
		m_opDialogWindow -> Unlock();
	}
}

void NormalizeDialog::HideDialog()
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

void NormalizeDialog::Init()
{
	m_opDialogWindow -> AddListener(this);
	m_opMainView -> AddListener(this);
	if(m_opDialogWindow -> Lock())
	{
		//m_opMainView -> SetViewColor(EMColor(192, 192, 192, 255));
		m_opMainView -> SetDefaultColor();
		m_opDialogWindow -> Unlock();
	}
	m_opOK_Button -> AddListener(this);
	m_opCancelButton -> AddListener(this);
	m_opTextControl -> AddListener(this);

	// Tab Order
	m_opDialogWindow -> AddTabComponent(m_opTextControl);
	m_opDialogWindow -> AddTabComponent(m_opOK_Button);
	m_opDialogWindow -> AddTabComponent(m_opCancelButton);
}

bool NormalizeDialog::MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage)
{
	switch(p_vMessage)
	{
	case EM_GUI_COMPONENT_ENTER_PRESSED:
	case EM_BUTTON_OK:
		eo << "OK button pressed" << ef;
		HideDialog();
		m_opCommandAudioNormalize -> DialogCallback(atoi(m_opTextControl -> Text()));
		return true;
	case EM_GUI_COMPONENT_ESC_PRESSED:
	case EM_BUTTON_CANCEL:
		eo << "Cancel button pressed" << ef;
		HideDialog();
		return true;
	case TEXT_CONTROL_CHANGED:
		if(m_vAlreadyChanged == false)
		{
			char* vpTemp;
			int32 vStart(0);
			int32 vEnd(0);
			if(p_opSender == m_opTextControl)
			{
				vpTemp = const_cast<char*>(m_opTextControl -> Text());
				vpTemp = SetValidValue(vpTemp);
				m_opTextControl -> GetSelection(vStart, vEnd);
				m_opTextControl -> SetText(vpTemp);
				m_opTextControl -> SetSelection(vStart, vEnd);
				return true;
			}
		}
		else
		{
			m_vAlreadyChanged = false;
			return true;
		}
		return false;
	// Tab-Key
	case EM_GUI_COMPONENT_TAB_PRESSED:
		m_opDialogWindow -> NextTab();
		return true;
	case EM_GUI_COMPONENT_SHIFT_TAB_PRESSED:
		m_opDialogWindow -> PreviousTab();
		return true;
	case EM_GUI_COMPONENT_GOT_FOCUS:
		if(p_opSender == m_opDialogWindow)
			m_opTextControl -> SetFocus();
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

void NormalizeDialog::MoveDialog()
{
	EMRect oWindowRect(m_opParentWindow -> Frame());
	EMRect oDialogRect(m_opDialogWindow -> Frame());
	float vX(oWindowRect.m_vRight - oWindowRect.GetWidth() / 2 - oDialogRect.GetWidth() / 2);
	float vY(oWindowRect.m_vBottom - oWindowRect.GetHeight() / 2 - oDialogRect.GetHeight() / 2);

	MoveDialog(vX, vY);
}

void NormalizeDialog::MoveDialog(float p_vX, float p_vY)
{
	m_opDialogWindow -> MoveTo(p_vX, p_vY);
}

void NormalizeDialog::SetParent(EMWindow* p_opWindow)
{
	m_opParentWindow = p_opWindow;
}

char* NormalizeDialog::SetValidValue(char* p_vpValue)
{
	eo << p_vpValue << ef;
	m_vAlreadyChanged = true;

	int vValue(atoi(p_vpValue));
	if(vValue > 0)
	{
		vValue = vValue * (-1);
	//	vValue = 0;
	}
	if(vValue < -99)
		vValue = -99;
	return itoa(vValue, m_vTemp, 10);

}