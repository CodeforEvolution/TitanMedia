#include "EMWinButton.h"

#ifdef PLATFORM_WINDOWS

#include "EMApplication.h"
#include "EMCommandRepository.h"
#include "EMKeyboard.h"
#include "EMKeyboardGlobals.h"
#include "EMMessages.h"
#include "EMWinGroupBox.h"
#include "EMWinView.h"
#include "EMWindow.h"
#include "EMWinWindow.h"

EMWinButton::EMWinButton(const EMRect p_oFrame, const char* p_vpName, uint32 p_vMessage, uint32 p_vResizingMode, uint32 p_vCommand, void* p_vpParentNativeView, bool p_vDefault) :
m_oFrame(p_oFrame),
m_vCommand(p_vCommand),
m_vMessage(p_vMessage),
m_vResizingMode(p_vResizingMode)
{
	uint32 vStyle(WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON/* | WS_TABSTOP <- Not working*//* | BS_NOTIFY*/);
	if(p_vDefault)
		vStyle |= BS_DEFPUSHBUTTON;
	HWND temp;
	if(p_vpParentNativeView)
		temp = dynamic_cast<EMWinWindow*>(static_cast<EMWindow*>(p_vpParentNativeView)) -> GetWindowHandle();
	else
		temp = EMWinWindow::GetDummyWindowHandle();
	m_opWindowHandle = CreateWindowEx(0, "button", p_vpName, vStyle, m_oFrame.m_vLeft,  
							m_oFrame.m_vTop, m_oFrame.m_vRight - m_oFrame.m_vLeft + 1, m_oFrame.m_vBottom - m_oFrame.m_vTop + 1,
							temp, NULL, EMApplication::GetApplicationInstance(), NULL);

	SendMessage(m_opWindowHandle, WM_SETFONT, reinterpret_cast<WPARAM>(GetStockObject(DEFAULT_GUI_FONT)), true);
	SetWindowLong(m_opWindowHandle, GWL_USERDATA, reinterpret_cast<int32>(this));

	m_opDefaultWindowProcedure = reinterpret_cast<WNDPROC>(GetWindowLong(m_opWindowHandle, GWL_WNDPROC));
	SetWindowLong(m_opWindowHandle, GWL_WNDPROC, reinterpret_cast<LONG>(SubclassedWindowProcedure));
}

EMWinButton::~EMWinButton()
{
	SetWindowLong(m_opWindowHandle, GWL_WNDPROC, reinterpret_cast<LONG>(m_opDefaultWindowProcedure));
	SetWindowLong(m_opWindowHandle, GWL_USERDATA, 0);
	DestroyWindow(m_opWindowHandle);
}

bool EMWinButton::AddToComponent(EMGUIComponent* p_opComponent)
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
	else
	{
		EMDebugger("The parent must be a view or a group box");
		return false;
	}
/*
	m_opWindowHandle = CreateWindowEx(0, "button", "button", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, m_oFrame.m_vLeft,  
							m_oFrame.m_vTop, m_oFrame.m_vRight - m_oFrame.m_vLeft, m_oFrame.m_vBottom - m_oFrame.m_vTop,
							opParent, NULL, EMApplication::GetApplicationInstance(), NULL);

	SetWindowLong(m_opWindowHandle, GWL_USERDATA, reinterpret_cast<int32>(this));

	m_opDefaultWindowProcedure = reinterpret_cast<WNDPROC>(GetWindowLong(m_opWindowHandle, GWL_WNDPROC));
	SetWindowLong(m_opWindowHandle, GWL_WNDPROC, reinterpret_cast<LONG>(SubclassedWindowProcedure));
*/
	SetParent(m_opWindowHandle, opParent);
	SetWindowPos(m_opWindowHandle, NULL, m_oFrame.m_vLeft, m_oFrame.m_vTop, m_oFrame.GetWidth() + 1, m_oFrame.GetHeight() + 1, SWP_NOZORDER);	

	return true;
}

void EMWinButton::Enable(bool p_vEnable)
{
	EnableWindow(m_opWindowHandle, p_vEnable);
}

void EMWinButton::ExecuteCommand()
{
	if(m_vCommand != 0)
		EMCommandRepository::Instance() -> ExecuteCommand(m_vCommand);
}

EMRect EMWinButton::Frame() const
{
	return m_oFrame;
}

void* EMWinButton::GetNativeView() const
{
	return m_opWindowHandle;
}

uint32 EMWinButton::GetResizingMode()
{
	return m_vResizingMode;
}

// Generic window procedure passed to WIN32 API
LRESULT CALLBACK EMWinButton::GlobalWindowProcedure(HWND p_vWindowHandle, UINT p_vMessage, WPARAM p_vParameterOne, LPARAM p_vParameterTwo)
{
eo << "EMWinButton::GlobalWindowProcedure" << ef;
	EMWinButton* opButton;
	opButton = reinterpret_cast<EMWinButton*>(GetWindowLong(p_vWindowHandle, GWL_USERDATA));

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
/*
		case BN_SETFOCUS:
eo << "button: BN_SETFOCUS" << ef;
			vMessage = 0;
			SetFocus(reinterpret_cast<HWND>(p_vParameterOne));
			return 0;
*/
		case WM_KEYDOWN:
			eo << "WM_KEYDOWN in EMWinButton" << ef;
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

void EMWinButton::Hide()
{
	ShowWindow(m_opWindowHandle, SW_HIDE);
}

bool EMWinButton::IsEnabled()
{
	return IsWindowEnabled(m_opWindowHandle) != 0;
}

bool EMWinButton::IsVisible()
{
	return IsWindowVisible(m_opWindowHandle) != 0;
}

void EMWinButton::SetCommand(uint32 p_vCommand)
{
	m_vCommand = p_vCommand;
}

void EMWinButton::SetFocus()
{
	::SetFocus(m_opWindowHandle);
}

void EMWinButton::SetFrame(EMRect p_oFrame)
{
	m_oFrame = p_oFrame;
	MoveWindow(m_opWindowHandle, p_oFrame.m_vLeft, p_oFrame.m_vTop, p_oFrame.GetWidth() + 1, p_oFrame.GetHeight() + 1, true); //Changed to true to test if working ok
}

void EMWinButton::SetLabel(const char* p_vpLabel)
{
	SetWindowText(m_opWindowHandle, p_vpLabel);
}

void EMWinButton::Show()
{
	ShowWindow(m_opWindowHandle, SW_SHOW);
}

LRESULT CALLBACK EMWinButton::SubclassedWindowProcedure(HWND p_vWindowHandle, UINT p_vMessage, WPARAM p_vParameterOne, LPARAM p_vParameterTwo)
{
//eo << "Button subclassing" << ef;
	EMWinButton* opButton;
	opButton = reinterpret_cast<EMWinButton*>(GetWindowLong(p_vWindowHandle, GWL_USERDATA));

//	switch(p_vMessage)
//	{
/*
	case WM_COMMAND:
		eo << "button: WM_COMMAND" << ef;
		break;
*/
/*
	case WM_LBUTTONUP:
		eo << "button: WM_LBUTTONUP" << ef;
		SetFocus(reinterpret_cast<HWND>(GetForegroundWindow()));
		break;
*/
//	}
/*
	switch(HIWORD(p_vParameterOne))
	{

	case WM_SETFOCUS:
eo << "button: WM_SETFOCUS" << ef;
		SetFocus(reinterpret_cast<HWND>(p_vParameterOne));
		return 0;
	}
*/
/*
	if(opButton)
	{
		uint32 vMessage;
		switch(HIWORD(p_vParameterOne))
		{
		case BN_CLICKED:
			vMessage = 0;
			opButton -> ExecuteCommand();
			break;
		default:
			vMessage = 0;
		}
		if(vMessage != 0)
			if(opButton -> Notify(vMessage))
				return 0;
	}
*/
//	int vResult = CallWindowProc(opButton -> m_opDefaultWindowProcedure, p_vWindowHandle, p_vMessage, p_vParameterOne, p_vParameterTwo);
//	if(p_vMessage == WM_LBUTTONUP)
//		SetFocus(reinterpret_cast<HWND>(GetForegroundWindow()));
//	return vResult;

	if(p_vMessage == WM_CHAR)
	{					
eo << "Window: WM_KEYDOWN: " << ef;
		switch(static_cast<int>(p_vParameterOne))
		{
		case EM_RETURN:
			opButton -> Notify(EM_GUI_COMPONENT_ENTER_PRESSED);
			return 0;
		case EM_ESCAPE:
			opButton -> Notify(EM_GUI_COMPONENT_ESC_PRESSED);
			return 0;
		case EM_TAB:
			if(EM_SHIFT_KEY & EMKeyboard::Instance() -> GetModifiers())
				opButton -> Notify(EM_GUI_COMPONENT_SHIFT_TAB_PRESSED);	
			else
				opButton -> Notify(EM_GUI_COMPONENT_TAB_PRESSED);
			return 0;
		}
	}
	if(p_vMessage == WM_SETFOCUS)
		opButton -> Notify(EM_GUI_COMPONENT_GOT_FOCUS);	

	return CallWindowProc(opButton -> m_opDefaultWindowProcedure, p_vWindowHandle, p_vMessage, p_vParameterOne, p_vParameterTwo);
//	return DefWindowProc(p_vWindowHandle, p_vMessage, p_vParameterOne, p_vParameterTwo);
}

#endif