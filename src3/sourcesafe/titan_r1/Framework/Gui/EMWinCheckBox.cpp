#include "EMWinCheckBox.h"

#ifdef PLATFORM_WINDOWS

#include "EMApplication.h"
#include "EMCommandRepository.h"
#include "EMKeyboard.h"
#include "EMKeyboardGlobals.h"
#include "EMMessages.h"
#include "EMWinGroupBox.h"
#include "EMWinView.h"
#include "EMWinWindow.h"

EMWinCheckBox::EMWinCheckBox(const EMRect p_oFrame, const char* p_vpName, uint32 p_vMessage, uint32 p_vResizingMode, uint32 p_vCommand) :
m_oFrame(p_oFrame),
m_vCommand(p_vCommand),
m_vMessage(p_vMessage),
m_vResizingMode(p_vResizingMode)
{
	m_opWindowHandle = CreateWindowEx(0, "button", p_vpName, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, m_oFrame.m_vLeft,  
							m_oFrame.m_vTop, m_oFrame.m_vRight - m_oFrame.m_vLeft + 1, m_oFrame.m_vBottom - m_oFrame.m_vTop + 1,
							EMWinWindow::GetDummyWindowHandle(), NULL, EMApplication::GetApplicationInstance(), NULL);
	SendMessage(m_opWindowHandle, WM_SETFONT, reinterpret_cast<WPARAM>(GetStockObject(DEFAULT_GUI_FONT)), true);
	SetWindowLong(m_opWindowHandle, GWL_USERDATA, reinterpret_cast<int32>(this));

	m_opDefaultWindowProcedure = reinterpret_cast<WNDPROC>(GetWindowLong(m_opWindowHandle, GWL_WNDPROC));
	SetWindowLong(m_opWindowHandle, GWL_WNDPROC, reinterpret_cast<LONG>(SubclassedWindowProcedure));
}

EMWinCheckBox::~EMWinCheckBox()
{
	SetWindowLong(m_opWindowHandle, GWL_WNDPROC, reinterpret_cast<LONG>(m_opDefaultWindowProcedure));
	SetWindowLong(m_opWindowHandle, GWL_USERDATA, 0);
	DestroyWindow(m_opWindowHandle);
}

bool EMWinCheckBox::AddToComponent(EMGUIComponent* p_opComponent)
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

	SetParent(m_opWindowHandle, opParent);
	SetWindowPos(m_opWindowHandle, NULL, m_oFrame.m_vLeft, m_oFrame.m_vTop, m_oFrame.GetWidth() + 1, m_oFrame.GetHeight() + 1, SWP_NOZORDER);	

	return true;
}

void EMWinCheckBox::Check(bool p_vCheck)
{
	SendMessage(m_opWindowHandle, BM_SETCHECK, p_vCheck, 0);
}

void EMWinCheckBox::ExecuteCommand()
{
	if(m_vCommand != 0)
		EMCommandRepository::Instance() -> ExecuteCommand(m_vCommand);
}

void EMWinCheckBox::Enable(bool p_vEnable)
{
	EnableWindow(m_opWindowHandle, p_vEnable);
}

EMRect EMWinCheckBox::Frame() const
{
	return m_oFrame;
}

void* EMWinCheckBox::GetNativeView() const
{
	return m_opWindowHandle;
}

uint32 EMWinCheckBox::GetResizingMode()
{
	return m_vResizingMode;
}

// Generic window procedure passed to WIN32 API
LRESULT CALLBACK EMWinCheckBox::GlobalWindowProcedure(HWND p_vWindowHandle, UINT p_vMessage, WPARAM p_vParameterOne, LPARAM p_vParameterTwo)
{
eo << "EMWinCheckBox::GlobalWindowProcedure" << ef;
	EMWinCheckBox* opButton;
	opButton = reinterpret_cast<EMWinCheckBox*>(GetWindowLong(p_vWindowHandle, GWL_USERDATA));

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

void EMWinCheckBox::Hide()
{
	ShowWindow(m_opWindowHandle, SW_HIDE);
}

bool EMWinCheckBox::IsChecked()
{
	if(SendMessage(m_opWindowHandle, BM_GETCHECK, 0, 0) == BST_CHECKED)
		return true;
	return false;
}

bool EMWinCheckBox::IsEnabled()
{
	return IsWindowEnabled(m_opWindowHandle) != 0;
}

bool EMWinCheckBox::IsVisible()
{
	return IsWindowVisible(m_opWindowHandle) != 0;
}

void EMWinCheckBox::SetCommand(uint32 p_vCommand)
{
	m_vCommand = p_vCommand;
}

void EMWinCheckBox::SetFocus()
{
	::SetFocus(m_opWindowHandle);
}

void EMWinCheckBox::SetFrame(EMRect p_oFrame)
{
	m_oFrame = p_oFrame;
	MoveWindow(m_opWindowHandle, p_oFrame.m_vLeft, p_oFrame.m_vTop, p_oFrame.GetWidth() + 1, p_oFrame.GetHeight() + 1, true); //Changed to true to test if working ok
}

void EMWinCheckBox::SetLabel(const char* p_vpLabel)
{
	SetWindowText(m_opWindowHandle, p_vpLabel);
}

void EMWinCheckBox::Show()
{
	ShowWindow(m_opWindowHandle, SW_SHOW);
}

LRESULT CALLBACK EMWinCheckBox::SubclassedWindowProcedure(HWND p_vWindowHandle, UINT p_vMessage, WPARAM p_vParameterOne, LPARAM p_vParameterTwo)
{
	EMWinCheckBox* opCheckBox;
	opCheckBox = reinterpret_cast<EMWinCheckBox*>(GetWindowLong(p_vWindowHandle, GWL_USERDATA));

	if(p_vMessage == WM_CHAR)
	{					
eo << "Window: WM_KEYDOWN: " << ef;
		switch(static_cast<int>(p_vParameterOne))
		{
		case EM_RETURN:
			opCheckBox -> Notify(EM_GUI_COMPONENT_ENTER_PRESSED);
			return 0;
		case EM_ESCAPE:
			opCheckBox -> Notify(EM_GUI_COMPONENT_ESC_PRESSED);
			return 0;
		case EM_TAB:
			if(EM_SHIFT_KEY & EMKeyboard::Instance() -> GetModifiers())
				opCheckBox -> Notify(EM_GUI_COMPONENT_SHIFT_TAB_PRESSED);	
			else
				opCheckBox -> Notify(EM_GUI_COMPONENT_TAB_PRESSED);
			return 0;
		}
	}
	if(p_vMessage == WM_SETFOCUS)
		opCheckBox -> Notify(EM_GUI_COMPONENT_GOT_FOCUS);	

	return CallWindowProc(opCheckBox -> m_opDefaultWindowProcedure, p_vWindowHandle, p_vMessage, p_vParameterOne, p_vParameterTwo);
}

#endif
