#include "EMWinTextControl.h"

#ifdef PLATFORM_WINDOWS

#include "EMApplication.h"
#include "EMCommandRepository.h"
#include "EMGUIGlobals.h"
#include "EMKeyboard.h"
#include "EMKeyboardGlobals.h"
#include "EMMessages.h"
#include "EMWinGroupBox.h"
#include "EMWinView.h"
#include "EMWinWindow.h"

EMWinTextControl::EMWinTextControl(EMRect p_oFrame, const char* p_vpText, uint32 p_vMessage, uint32 p_vResizingMode, bool p_vAutoClose, const EMAlignment p_eAlignment, bool p_vOnlyNumbers, bool p_vPasswordChar) :
EMTextControl(p_vAutoClose),
m_oFrame(p_oFrame),
m_oText(p_vpText),
m_vAutoClose(p_vAutoClose),
m_vMessage(p_vMessage),
m_vResizingMode(p_vResizingMode)
{
	int32 vStyle(WS_CHILD | WS_VISIBLE/* | WS_TABSTOP*/);
	if(p_eAlignment == EM_ALIGNMENT_LEFT)
		vStyle |= ES_LEFT | ES_AUTOHSCROLL;
	else if(p_eAlignment == EM_ALIGNMENT_CENTER)
		vStyle |= ES_CENTER;
	else if(p_eAlignment == EM_ALIGNMENT_RIGHT)
		vStyle |= ES_RIGHT;

	if(p_vOnlyNumbers)
		vStyle |= ES_NUMBER;
	if(p_vPasswordChar)
		vStyle |= ES_PASSWORD;

	m_opWindowHandle = CreateWindowEx(WS_EX_CLIENTEDGE, "edit", p_vpText, vStyle, m_oFrame.m_vLeft,  
							m_oFrame.m_vTop, m_oFrame.m_vRight - m_oFrame.m_vLeft + 1, m_oFrame.m_vBottom - m_oFrame.m_vTop + 1,
							EMWinWindow::GetDummyWindowHandle(), NULL, EMApplication::GetApplicationInstance(), NULL);

	SendMessage(m_opWindowHandle, WM_SETFONT, reinterpret_cast<WPARAM>(GetStockObject(DEFAULT_GUI_FONT)), true);
	SetWindowLong(m_opWindowHandle, GWL_USERDATA, reinterpret_cast<int32>(this));

	m_opDefaultWindowProcedure = reinterpret_cast<WNDPROC>(GetWindowLong(m_opWindowHandle, GWL_WNDPROC));
	SetWindowLong(m_opWindowHandle, GWL_WNDPROC, reinterpret_cast<LONG>(SubclassedWindowProcedure));
}

EMWinTextControl::~EMWinTextControl()
{
	SetWindowLong(m_opWindowHandle, GWL_WNDPROC, reinterpret_cast<LONG>(m_opDefaultWindowProcedure));
	SetWindowLong(m_opWindowHandle, GWL_USERDATA, 0);
	DestroyWindow(m_opWindowHandle);
}

bool EMWinTextControl::AddToComponent(EMGUIComponent* p_opComponent)
{
	HWND opParent;

	if(dynamic_cast<EMWinView*>(p_opComponent))
	{
		EMWinView* opView = dynamic_cast<EMWinView*>(p_opComponent);
		opParent = opView -> GetWindowHandle();
		m_opParent = opView;
	}
	else if(dynamic_cast<EMWinGroupBox*>(p_opComponent))
	{
		EMWinGroupBox* opView = dynamic_cast<EMWinGroupBox*>(p_opComponent);
		opParent = static_cast<HWND>(opView -> GetNativeView());
//		m_opParent = opView;
	}
	else
	{
		EMDebugger("The parent must be a view or a group box");
		return false;
	}
/*
	m_opWindowHandle = CreateWindowEx(0, "edit", NULL, WS_CHILD | WS_VISIBLE, m_oFrame.m_vLeft,  
							m_oFrame.m_vTop, m_oFrame.m_vRight - m_oFrame.m_vLeft, m_oFrame.m_vBottom - m_oFrame.m_vTop,
							opParent, NULL, EMApplication::GetApplicationInstance(), NULL);
	SetWindowLong(m_opWindowHandle, GWL_USERDATA, reinterpret_cast<int32>(this));
*/
	SetParent(m_opWindowHandle, opParent);
	SetWindowPos(m_opWindowHandle, NULL, Frame().m_vLeft, Frame().m_vTop, Frame().GetWidth() + 1, Frame().GetHeight() + 1, SWP_NOZORDER);	

	return true;
}

void EMWinTextControl::Enable(bool p_vEnable)
{
	EnableWindow(m_opWindowHandle, p_vEnable);
}

EMRect EMWinTextControl::Frame() const
{
	return m_oFrame;
}

void* EMWinTextControl::GetNativeView() const
{
	return m_opWindowHandle;
}

uint32 EMWinTextControl::GetResizingMode()
{
	return m_vResizingMode;
}

void EMWinTextControl::GetSelection(int32 &p_vStart, int32 &p_vEnd)
{
	DWORD vStart(0);
	DWORD vEnd(0);
	SendMessage(m_opWindowHandle, EM_GETSEL, reinterpret_cast<WPARAM>(&vStart), reinterpret_cast<LPARAM>(&vEnd));
	p_vStart = vStart;
	p_vEnd = vEnd;
}

// Generic window procedure passed to WIN32 API
LRESULT CALLBACK EMWinTextControl::GlobalWindowProcedure(HWND p_vWindowHandle, UINT p_vMessage, WPARAM p_vParameterOne, LPARAM p_vParameterTwo)
{
	EMWinTextControl* opTextControl;
	opTextControl = reinterpret_cast<EMWinTextControl*>(GetWindowLong(p_vWindowHandle, GWL_USERDATA));

	if(opTextControl)
	{
		uint32 vMessage;
		switch(HIWORD(p_vParameterOne))
		{
		case EN_UPDATE:
			vMessage = opTextControl -> m_vMessage;//EM_TEXT_CHANGED;
			//eo << "EN_UPDATE" << ef;
			break;
		default:
			vMessage = 0;
		}
		if(vMessage != 0)
			if(opTextControl -> Notify(vMessage))
				return 0;
	}
	return DefWindowProc(p_vWindowHandle, p_vMessage, p_vParameterOne, p_vParameterTwo);
}

void EMWinTextControl::Hide()
{
	ShowWindow(m_opWindowHandle, SW_HIDE);
}

void EMWinTextControl::InitComponent()
{
}

bool EMWinTextControl::IsEnabled()
{
	return IsWindowEnabled(m_opWindowHandle) != 0;
}

bool EMWinTextControl::IsVisible()
{
	return IsWindowVisible(m_opWindowHandle) != 0;
}
/*
void EMWinTextControl::Invalidate()
{
	m_opTextControl -> Invalidate();
}
*/
void EMWinTextControl::SelectAll()
{
	SendMessage(m_opWindowHandle, EM_SETSEL, 0, -1);
}

void EMWinTextControl::SetFocus()
{
	::SetFocus(m_opWindowHandle);
}

void EMWinTextControl::SetFrame(EMRect p_oFrame)
{
	m_oFrame = p_oFrame;
	MoveWindow(m_opWindowHandle, p_oFrame.m_vLeft, p_oFrame.m_vTop, p_oFrame.GetWidth() + 1, p_oFrame.GetHeight() + 1, true);
}

void EMWinTextControl::SetSelection(int32 p_vStart, int32 p_vEnd) // If p_vStart == 0 and p_vEnd == -1 all text will be selected
{
	SendMessage(m_opWindowHandle, EM_SETSEL, p_vStart, static_cast<LPARAM>(p_vEnd));
	SendMessage(m_opWindowHandle, EM_SCROLLCARET, 0, 0);
}

void EMWinTextControl::SetText(const char* p_vpText)
{
	SendMessage(m_opWindowHandle, WM_SETTEXT, EM_MAX_STRING_BUFFER_SIZE, reinterpret_cast<LPARAM>(p_vpText));
	m_oText = p_vpText;
}

void EMWinTextControl::Show()
{
	ShowWindow(m_opWindowHandle, SW_SHOW);
}

void EMWinTextControl::SetMaxBytes(int32 p_vMaxBytes)
{
}
/*
void EMWinTextControl::SetWordWrap(bool p_vWrap)
{
	m_opTextControl -> SetWordWrap(p_vWrap);
}
*/

LRESULT CALLBACK EMWinTextControl::SubclassedWindowProcedure(HWND p_vWindowHandle, UINT p_vMessage, WPARAM p_vParameterOne, LPARAM p_vParameterTwo)
{
	EMWinTextControl* opTextControl;
	opTextControl = reinterpret_cast<EMWinTextControl*>(GetWindowLong(p_vWindowHandle, GWL_USERDATA));

	if(opTextControl -> m_vAutoClose)
	{
		if(p_vMessage == WM_KILLFOCUS)
		{
//eo << "EMWinTextControl: WM_KILLFOCUS" << ef;

			opTextControl -> Notify(EM_TEXT_CHANGED);
			opTextControl -> Hide();
			return 0;
		}

		else if(p_vMessage == WM_KEYDOWN || p_vMessage == WM_CHAR)
		{					
//eo << "Window: WM_KEYDOWN: " << ef;
			if(static_cast<int>(p_vParameterOne) == EM_RETURN)
			{
				opTextControl -> Notify(EM_TEXT_CHANGED);
				opTextControl -> Hide();
				::SetFocus(opTextControl -> m_opParent -> GetWindowWindow() -> GetMainWindowHandle());
				return 0;
			}

			else if(static_cast<int>(p_vParameterOne) == EM_ESCAPE)
			{
				opTextControl -> Notify(EM_TEXT_CHANGE_UNDOED);
				opTextControl -> Hide();
				::SetFocus(opTextControl -> m_opParent -> GetWindowWindow() -> GetMainWindowHandle());
				return 0;
			}		
		}

	}
	else
	{
		if(p_vMessage == WM_CHAR)
		{					
//eo << "Window: WM_KEYDOWN: " << ef;
			switch(static_cast<int>(p_vParameterOne))
			{
			case EM_RETURN:
				opTextControl -> Notify(EM_GUI_COMPONENT_ENTER_PRESSED);
				return 0;
			case EM_ESCAPE:
				opTextControl -> Notify(EM_GUI_COMPONENT_ESC_PRESSED);
				return 0;
			case EM_TAB:
				if(EM_SHIFT_KEY & EMKeyboard::Instance() -> GetModifiers())
					opTextControl -> Notify(EM_GUI_COMPONENT_SHIFT_TAB_PRESSED);	
				else
					opTextControl -> Notify(EM_GUI_COMPONENT_TAB_PRESSED);
				return 0;
			}
		}
		if(p_vMessage == WM_KEYDOWN)
		{
//			eo << "WM_KEYDOWN in TextControl" << ef;
			switch(static_cast<int>(p_vParameterOne))
			{
			case EM_END:
				if(opTextControl -> Notify(EM_TEXT_CONTROL_END_PRESSED))
					return 0;
				break;
			case EM_DELETE:
				if(opTextControl -> Notify(EM_TEXT_CONTROL_DELETE_PRESSED))
					return 0;
				break;
			case EM_UP_ARROW:
				if(opTextControl -> Notify(EM_TEXT_CONTROL_UP_ARROW_PRESSED))
					return 0;
				break;
			case EM_DOWN_ARROW:
				if(opTextControl -> Notify(EM_TEXT_CONTROL_DOWN_ARROW_PRESSED))
					return 0;
				break;
			case EM_LEFT_ARROW:
				if(opTextControl -> Notify(EM_TEXT_CONTROL_LEFT_ARROW_PRESSED))
					return 0;
				break;
			case EM_RIGHT_ARROW:
				if(opTextControl -> Notify(EM_TEXT_CONTROL_RIGHT_ARROW_PRESSED))
					return 0;
				break;
			}
		}
		if(p_vMessage == WM_SETFOCUS)
		{
			opTextControl -> Notify(EM_GUI_COMPONENT_GOT_FOCUS);
			opTextControl -> SelectAll();
		}
		if(p_vMessage == WM_LBUTTONUP)
			if(opTextControl -> Notify(EM_TEXT_CONTROL_LEFT_MOUSE_BUTTON_DOWN))
				return 0;
		if(p_vMessage == WM_CONTEXTMENU)
			opTextControl -> Notify(EM_TEXT_CONTROL_MENU_SELECTED);
		if(p_vMessage == EM_SETSEL)
			if(opTextControl -> Notify(EM_TEXT_CONTROL_SET_SELECTION))
				return 0;
	}

	return CallWindowProc(opTextControl -> m_opDefaultWindowProcedure, p_vWindowHandle, p_vMessage, p_vParameterOne, p_vParameterTwo);

}

const char* EMWinTextControl::Text()
{
	char vString[EM_MAX_STRING_BUFFER_SIZE];
	SendMessage(m_opWindowHandle, WM_GETTEXT, EM_MAX_STRING_BUFFER_SIZE, reinterpret_cast<LPARAM>(vString));
	m_oText = vString;
	return m_oText.c_str();
}

#endif


