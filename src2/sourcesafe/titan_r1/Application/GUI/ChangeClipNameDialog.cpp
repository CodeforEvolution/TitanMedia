#include "ChangeClipNameDialog.h"

#include "EMFactory.h"

#include "EMGClip.h"
#include "EMWindow.h"
#include "EMView.h"
#include "EMButton.h"
#include "EMTextControl.h"

#include "EMMessages.h"

#ifdef PLATFORM_WINDOWS
#include <windows.h> // To get modal dialog to work.
#include "EMWinWindow.h" // To get modal dialog to work.
#endif

const uint32 EM_BUTTON_OK = 1;
const uint32 EM_BUTTON_CANCEL = 2;

ChangeClipNameDialog::ChangeClipNameDialog() :
m_opDialogWindow(EMFactory::Instance() -> CreateEMWindow(EMRect(600, 200, 600 + 188, 200 + 73), "Change Clip-Name", EM_WINDOW_DIALOG)), 
m_opMainView(EMFactory::Instance() -> CreateView(EMRect(0, 0, 188, 73), false)),
m_opOK_Button(EMFactory::Instance() -> CreateButton(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 7,
														   EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + EM_METRICS_NATIVE_BUTTON_HEIGHT + EM_METRICS_NATIVE_BUTTON_SPACE,
														   EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 7 + EM_METRICS_NATIVE_BUTTON_WIDTH,
														   EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + EM_METRICS_NATIVE_BUTTON_HEIGHT + EM_METRICS_NATIVE_BUTTON_HEIGHT + EM_METRICS_NATIVE_BUTTON_SPACE),
													"OK", EM_BUTTON_OK, 0, 0, NULL, true)),
m_opCancelButton(EMFactory::Instance() -> CreateButton(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 7 +EM_METRICS_NATIVE_BUTTON_WIDTH + EM_METRICS_NATIVE_BUTTON_SPACE,
															  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + EM_METRICS_NATIVE_BUTTON_HEIGHT + EM_METRICS_NATIVE_BUTTON_SPACE,
															  EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 7 + EM_METRICS_NATIVE_BUTTON_WIDTH + EM_METRICS_NATIVE_BUTTON_SPACE + EM_METRICS_NATIVE_BUTTON_WIDTH,
															  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + EM_METRICS_NATIVE_BUTTON_HEIGHT + EM_METRICS_NATIVE_BUTTON_SPACE + EM_METRICS_NATIVE_BUTTON_HEIGHT),
													   "Cancel", EM_BUTTON_CANCEL, 0, 0)),
m_opNameTextControl(EMFactory::Instance() ->CreateTextControl(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
																	  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP,
																	  EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 168,
																	  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + EM_METRICS_NATIVE_TEXT_CONTROL_HEIGHT),
															   "Edit", 0, 0, false)),
m_vFinished(false),
m_opClip(NULL),
m_opClipView(NULL)
{
	m_opDialogWindow -> AddChild(m_opMainView);
	m_opMainView -> AddChild(m_opOK_Button);
	m_opMainView -> AddChild(m_opCancelButton);
	m_opMainView -> AddChild(m_opNameTextControl);
}
ChangeClipNameDialog::~ChangeClipNameDialog()
{
	delete m_opDialogWindow;
	delete m_opMainView;
	delete m_opOK_Button;
	delete m_opCancelButton;
	delete m_opNameTextControl;
}

void ChangeClipNameDialog::DisplayDialog(EMGClip* p_opClip, EMView* p_opClipView)
{
	m_opClip = p_opClip;
	m_opClipView = p_opClipView;
	m_vEditText = true;
	if(m_opDialogWindow -> Lock())
	{
		m_opNameTextControl -> Show();
		m_opDialogWindow -> Show();
#ifdef PLATFORM_WINDOWS
		EnableWindow(dynamic_cast<EMWinWindow*>(m_opDialogWindow) -> GetMainWindowHandle(), FALSE);
		SetFocus(static_cast<HWND>(m_opNameTextControl -> GetNativeView()));
#endif
		m_opDialogWindow -> Unlock();
	}
}

string ChangeClipNameDialog::GetText()
{
	return m_opNameTextControl -> Text();
}

void ChangeClipNameDialog::HideDialog()
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

void ChangeClipNameDialog::Init()
{
	m_opMainView -> AddListener(this);
	if(m_opDialogWindow -> Lock())
	{
		//m_opMainView -> SetViewColor(EMColor(192, 192, 192, 255));
		m_opMainView -> SetDefaultColor();
		m_opDialogWindow -> Unlock();
	}
	m_opNameTextControl -> AddListener(this);
	m_opOK_Button -> AddListener(this);
	m_opCancelButton -> AddListener(this);

	//Setting TabOrder
	m_opDialogWindow -> AddTabComponent(m_opNameTextControl);
	m_opDialogWindow -> AddTabComponent(m_opOK_Button);
	m_opDialogWindow -> AddTabComponent(m_opCancelButton);
}

bool ChangeClipNameDialog::MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage)
{
	switch(p_vMessage)
	{
	case EM_GUI_COMPONENT_ENTER_PRESSED:
	case EM_BUTTON_OK:
		if(m_vEditText == false)
			break;
		HideDialog();
		if(m_opClipView -> LockWindow())
		{
			m_opClip -> SetLabel(const_cast<char*>(GetText().c_str()), m_opClipView);
			m_opClipView -> Invalidate(m_opClip -> Frame());
			m_opClipView -> UnlockWindow();
		}
		return true;
	case EM_GUI_COMPONENT_ESC_PRESSED:
	case EM_BUTTON_CANCEL:
		m_vEditText = false;
		HideDialog();
		return true;
	case EM_GUI_COMPONENT_TAB_PRESSED:
		m_opDialogWindow -> NextTab();
		return true;
	case EM_GUI_COMPONENT_SHIFT_TAB_PRESSED:
		m_opDialogWindow -> PreviousTab();
		return true;
	case EM_GUI_COMPONENT_GOT_FOCUS:
		m_opDialogWindow -> SetCurrentFocus();
		return true;
	case EM_CLOSE_WINDOW:
		if(p_opSender == m_opDialogWindow)
		{
			eo << "EM_CLOSE_WINDOW" << ef;
			m_vEditText = false;
			HideDialog();
			return true;
		}
		break;
	}
	return false;
}

void ChangeClipNameDialog::MoveDialog(float p_vX, float p_vY)
{
	m_opDialogWindow -> MoveTo(p_vX, p_vY);
}

void ChangeClipNameDialog::SetText(string p_oText)
{
	m_opNameTextControl -> SetText(p_oText.c_str());
	m_opNameTextControl -> SetSelection(0, -1);
}
