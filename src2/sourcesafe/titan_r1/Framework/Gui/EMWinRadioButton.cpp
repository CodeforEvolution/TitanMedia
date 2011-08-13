#include "EMWinRadioButton.h"

#ifdef PLATFORM_WINDOWS

#include "EMApplication.h"
#include "EMCommandRepository.h"
#include "EMKeyboard.h"
#include "EMKeyboardGlobals.h"
#include "EMMessages.h"
#include "EMWinGroupBox.h"
#include "EMWinView.h"
#include "EMWinWindow.h"
#include "EMWinTabControl.h"

EMWinRadioButton::EMWinRadioButton(const EMRect p_oFrame, const char* p_vpName, uint32 p_vMessage, uint32 p_vResizingMode, uint32 p_vCommand, bool p_vEndGroup) :
m_oFrame(p_oFrame),
m_vCommand(p_vCommand),
m_vMessage(p_vMessage),
m_vResizingMode(p_vResizingMode)
{
	int32 vStyle(WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON);
	eo << (char*) p_vpName <<  ": Creating RadioBox" << ef;
	if(p_vEndGroup)
	{
		eo << "    Ending group" << ef;
		vStyle |= WS_GROUP;
	}

	m_opWindowHandle = CreateWindowEx(0, "button", p_vpName, vStyle, m_oFrame.m_vLeft,  
							m_oFrame.m_vTop, m_oFrame.m_vRight - m_oFrame.m_vLeft + 1, m_oFrame.m_vBottom - m_oFrame.m_vTop + 1,
							EMWinWindow::GetDummyWindowHandle(), NULL, EMApplication::GetApplicationInstance(), NULL);
	SendMessage(m_opWindowHandle, WM_SETFONT, reinterpret_cast<WPARAM>(GetStockObject(DEFAULT_GUI_FONT)), true);
	SetWindowLong(m_opWindowHandle, GWL_USERDATA, reinterpret_cast<int32>(this));

	m_opDefaultWindowProcedure = reinterpret_cast<WNDPROC>(GetWindowLong(m_opWindowHandle, GWL_WNDPROC));
	SetWindowLong(m_opWindowHandle, GWL_WNDPROC, reinterpret_cast<LONG>(SubclassedWindowProcedure));
}

EMWinRadioButton::~EMWinRadioButton()
{
	SetWindowLong(m_opWindowHandle, GWL_WNDPROC, reinterpret_cast<LONG>(m_opDefaultWindowProcedure));
	SetWindowLong(m_opWindowHandle, GWL_USERDATA, 0);
	DestroyWindow(m_opWindowHandle);
}

bool EMWinRadioButton::AddToComponent(EMGUIComponent* p_opComponent)
{
	HWND opParent;

	if(dynamic_cast<EMWinView*>(p_opComponent))
	{
		EMWinView* opView = dynamic_cast<EMWinView*>(p_opComponent);
		opParent = opView -> GetWindowHandle();
	}
	else if(dynamic_cast<EMWinGroupBox*>(p_opComponent))
	{
		EMWinGroupBox* opView = dynamic_cast<EMWinGroupBox*>(p_opComponent);
		opParent = static_cast<HWND>(opView -> GetNativeView());
	}
	else if(dynamic_cast<EMWinTabControl*>(p_opComponent))
	{
		EMWinTabControl* opView = dynamic_cast<EMWinTabControl*>(p_opComponent);
		opParent = static_cast<HWND>(opView -> GetNativeView());
	}
	else
	{
		EMDebugger("The parent must be a view or a group box");
		return false;
	}

	SetParent(m_opWindowHandle, opParent);
	SetWindowPos(m_opWindowHandle, NULL, m_oFrame.m_vLeft, m_oFrame.m_vTop, m_oFrame.GetWidth() + 1, m_oFrame.GetHeight() + 1, SWP_NOZORDER);	

	return true;
}

void EMWinRadioButton::Check(bool p_vCheck)
{
	SendMessage(m_opWindowHandle, BM_SETCHECK, p_vCheck, 0);
}

void EMWinRadioButton::ExecuteCommand()
{
	if(m_vCommand != 0)
		EMCommandRepository::Instance() -> ExecuteCommand(m_vCommand);
}

void EMWinRadioButton::Enable(bool p_vEnable)
{
	EnableWindow(m_opWindowHandle, p_vEnable);
}

EMRect EMWinRadioButton::Frame() const
{
	return m_oFrame;
}

void* EMWinRadioButton::GetNativeView() const
{
	return m_opWindowHandle;
}

uint32 EMWinRadioButton::GetResizingMode()
{
	return m_vResizingMode;
}

// Generic window procedure passed to WIN32 API
LRESULT CALLBACK EMWinRadioButton::GlobalWindowProcedure(HWND p_vWindowHandle, UINT p_vMessage, WPARAM p_vParameterOne, LPARAM p_vParameterTwo)
{
eo << "EMWinRadioButton::GlobalWindowProcedure" << ef;
	EMWinRadioButton* opButton;
	opButton = reinterpret_cast<EMWinRadioButton*>(GetWindowLong(p_vWindowHandle, GWL_USERDATA));

	if(opButton)
	{
		uint32 vMessage;
		switch(HIWORD(p_vParameterOne))
		{
		case BN_CLICKED:
			vMessage = 0;
			opButton -> ExecuteCommand();
			if(opButton -> m_vMessage != 0)
				opButton -> Notify(opButton -> m_vMessage);
			break;
		default:
			vMessage = 0;
		}
		if(vMessage != 0)
			if(opButton -> Notify(vMessage))
				return 0;
	}
	return DefWindowProc(p_vWindowHandle, p_vMessage, p_vParameterOne, p_vParameterTwo);
}

void EMWinRadioButton::Hide()
{
	ShowWindow(m_opWindowHandle, SW_HIDE);
}

bool EMWinRadioButton::IsChecked()
{
	if(SendMessage(m_opWindowHandle, BM_GETCHECK, 0, 0) == BST_CHECKED)
		return true;
	return false;
}

bool EMWinRadioButton::IsEnabled()
{
	return IsWindowEnabled(m_opWindowHandle) != 0;
}

bool EMWinRadioButton::IsVisible()
{
	return IsWindowVisible(m_opWindowHandle) != 0;
}

void EMWinRadioButton::SetCommand(uint32 p_vCommand)
{
	m_vCommand = p_vCommand;
}

void EMWinRadioButton::SetFocus()
{
	::SetFocus(m_opWindowHandle);
}

void EMWinRadioButton::SetFrame(EMRect p_oFrame)
{
	m_oFrame = p_oFrame;
	MoveWindow(m_opWindowHandle, p_oFrame.m_vLeft, p_oFrame.m_vTop, p_oFrame.GetWidth() + 1, p_oFrame.GetHeight() + 1, true); //Changed to true to test if working ok
}

void EMWinRadioButton::SetLabel(const char* p_vpLabel)
{
	SetWindowText(m_opWindowHandle, p_vpLabel);
}

void EMWinRadioButton::Show()
{
	ShowWindow(m_opWindowHandle, SW_SHOW);
}

LRESULT CALLBACK EMWinRadioButton::SubclassedWindowProcedure(HWND p_vWindowHandle, UINT p_vMessage, WPARAM p_vParameterOne, LPARAM p_vParameterTwo)
{
	EMWinRadioButton* opRadioButton;
	opRadioButton = reinterpret_cast<EMWinRadioButton*>(GetWindowLong(p_vWindowHandle, GWL_USERDATA));

	if(p_vMessage == WM_CHAR)
	{					
eo << "Window: WM_KEYDOWN: " << ef;
		switch(static_cast<int>(p_vParameterOne))
		{
		case EM_RETURN:
			opRadioButton -> Notify(EM_GUI_COMPONENT_ENTER_PRESSED);
			return 0;
		case EM_ESCAPE:
			opRadioButton -> Notify(EM_GUI_COMPONENT_ESC_PRESSED);
			return 0;
		case EM_TAB:
			if(EM_SHIFT_KEY & EMKeyboard::Instance() -> GetModifiers())
				opRadioButton -> Notify(EM_GUI_COMPONENT_SHIFT_TAB_PRESSED);	
			else
				opRadioButton -> Notify(EM_GUI_COMPONENT_TAB_PRESSED);
			return 0;
		}
	}
	if(p_vMessage == WM_SETFOCUS)
		opRadioButton -> Notify(EM_GUI_COMPONENT_GOT_FOCUS);	

	return CallWindowProc(opRadioButton -> m_opDefaultWindowProcedure, p_vWindowHandle, p_vMessage, p_vParameterOne, p_vParameterTwo);
}

#endif
