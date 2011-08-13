#include "EMWinTrackbarControl.h"

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

EMWinTrackbarControl::EMWinTrackbarControl(const EMRect p_oFrame, const char* p_vpName, uint32 p_vMessage, uint32 p_vResizingMode, uint32 p_vCommand, EMTrackbarTickStyle p_eTickStyle, bool p_vVertical) :
m_oFrame(p_oFrame),
m_vCommand(p_vCommand),
m_vMessage(p_vMessage),
m_vResizingMode(p_vResizingMode),
m_vStyle(WS_CHILD | WS_VISIBLE | TBS_FIXEDLENGTH/* | TBS_ENABLESELRANGE*/)
{
	INITCOMMONCONTROLSEX oInitCommonControlEX;
	oInitCommonControlEX.dwSize = sizeof(oInitCommonControlEX);
	oInitCommonControlEX.dwICC = ICC_TAB_CLASSES;

	if(p_vVertical)
		m_vStyle |= TBS_VERT;
	else
		m_vStyle |= TBS_HORZ;

	switch(p_eTickStyle)
	{
	case EM_NO_TICKS:
		m_vStyle |= TBS_NOTICKS;
		break;
	case EM_AUTO_TICKS:		// Display one tick for each incerement in its range of values
		m_vStyle |= TBS_AUTOTICKS;
		break;
	case EM_BOTTOM_TICKS:	// Only horizontal style
		if(p_vVertical)
			EMDebugger("Can't use \"EM_BOTTOM_TICKS\" when Vertical style is specified");
		else
			m_vStyle |= TBS_BOTTOM;
		break;
	case EM_TOP_TICKS:		// Only horizontal style
		if(p_vVertical)
			EMDebugger("Can't use \"EM_TOP_TICKS\" when Vertical style is specified");
		else
			m_vStyle |= TBS_TOP;
		break;
	case EM_LEFT_TICKS:		// Only vertical style
		if(p_vVertical)
			EMDebugger("Can't use \"EM_LEFT_TICKS\" when Horisontal style is specified");
		else
			m_vStyle |= TBS_LEFT;
		break;
	case EM_RIGHT_TICKS:		// Only vertical style
		if(p_vVertical)
			EMDebugger("Can't use \"EM_RIGHT_TICKS\" when Horisontal style is specified");
		else
			m_vStyle |= TBS_RIGHT;
		break;
	case EM_BOTH_TICKS:
		m_vStyle |= TBS_BOTH;
		break;
	default:
		EMDebugger("This tickstyle does not exist.");

	}

	if(InitCommonControlsEx(&oInitCommonControlEX))
	{
		m_opWindowHandle = CreateWindowEx( 0, TRACKBAR_CLASS, p_vpName, m_vStyle, m_oFrame.m_vLeft,  
								m_oFrame.m_vTop, m_oFrame.m_vRight - m_oFrame.m_vLeft + 1, m_oFrame.m_vBottom - m_oFrame.m_vTop + 1,
								EMWinWindow::GetDummyWindowHandle(), NULL, EMApplication::GetApplicationInstance(), NULL);
		SendMessage(m_opWindowHandle, WM_SETFONT, reinterpret_cast<WPARAM>(GetStockObject(DEFAULT_GUI_FONT)), true);
		SetWindowLong(m_opWindowHandle, GWL_USERDATA, reinterpret_cast<int32>(this));

		m_opDefaultWindowProcedure = reinterpret_cast<WNDPROC>(GetWindowLong(m_opWindowHandle, GWL_WNDPROC));
		SetWindowLong(m_opWindowHandle, GWL_WNDPROC, reinterpret_cast<LONG>(SubclassedWindowProcedure));
		if(p_vVertical)
			SendMessage(m_opWindowHandle, TBM_SETTHUMBLENGTH, (WPARAM) (UINT) 14, 0); 
		else
			SendMessage(m_opWindowHandle, TBM_SETTHUMBLENGTH, (WPARAM) (UINT) 14, 0); 
	}
	else
		eo << "Could not load CommonControl dll" << ef;
}

EMWinTrackbarControl::~EMWinTrackbarControl()
{
//	SetWindowLong(m_opWindowHandle, GWL_WNDPROC, reinterpret_cast<LONG>(m_opDefaultWindowProcedure));
	SetWindowLong(m_opWindowHandle, GWL_USERDATA, 0);
	DestroyWindow(m_opWindowHandle);
}

void EMWinTrackbarControl::AddChild(EMGUIComponent* p_opView)
{
//	AssertWindowLocked();
	if(dynamic_cast<EMWinView*>(p_opView))
		(dynamic_cast<EMWinView*>(p_opView)) -> SetWindowWindow(m_opWindowWindow);
	else if(dynamic_cast<EMWinGroupBox*>(p_opView))
		(dynamic_cast<EMWinGroupBox*>(p_opView)) -> SetWindowWindow(m_opWindowWindow);
	p_opView -> AddToComponent(this);
	m_oChildComponents.push_back(p_opView);
}

bool EMWinTrackbarControl::AddToComponent(EMGUIComponent* p_opComponent)
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

void EMWinTrackbarControl::ExecuteCommand()
{
	if(m_vCommand != 0)
		EMCommandRepository::Instance() -> ExecuteCommand(m_vCommand);
}

void EMWinTrackbarControl::Enable(bool p_vEnable)
{
	EnableWindow(m_opWindowHandle, p_vEnable);
}

EMRect EMWinTrackbarControl::Frame() const
{
	return m_oFrame;
}

void* EMWinTrackbarControl::GetNativeView() const
{
	return m_opWindowHandle;
}

int32 EMWinTrackbarControl::GetPosition()
{
	return SendMessage(m_opWindowHandle, TBM_GETPOS, 0, 0);
}

uint32 EMWinTrackbarControl::GetResizingMode()
{
	return m_vResizingMode;
}

// Generic window procedure passed to WIN32 API
LRESULT CALLBACK EMWinTrackbarControl::GlobalWindowProcedure(HWND p_vWindowHandle, UINT p_vMessage, WPARAM p_vParameterOne, LPARAM p_vParameterTwo)
{
/*eo << "EMWinTrackbarControl::GlobalWindowProcedure" << ef;
	EMWinTrackbarControl* opTrackbarControl;
	opTrackbarControl = reinterpret_cast<EMWinTrackbarControl*>(GetWindowLong(p_vWindowHandle, GWL_USERDATA));

	if(opTrackbarControl)
	{
		eo << "In If" << ef;
		uint32 vMessage;
		switch(HIWORD(p_vParameterOne))
		{
		case BN_CLICKED:
			eo << "BN_CLICKED " << BN_CLICKED << ef;
			break;
		default:
			eo << "Default" << ef;
			vMessage = 0;
		}
		switch(p_vMessage)

		switch(reinterpret_cast<LPNMHDR>(p_vParameterTwo) -> code)
		{
		case TB_THUMBTRACK:
			eo << "TB_THUMBTRACK" << ef;
			break;
		default:
			eo << "Default" << ef;
			vMessage = 0;
		}
		if(vMessage != 0)
			if(opTrackbarControl -> Notify(vMessage))
				return 0;
	}
*/
	return DefWindowProc(p_vWindowHandle, p_vMessage, p_vParameterOne, p_vParameterTwo);
}

void EMWinTrackbarControl::Hide()
{
	ShowWindow(m_opWindowHandle, SW_HIDE);
}

bool EMWinTrackbarControl::IsEnabled()
{
	return IsWindowEnabled(m_opWindowHandle) != 0;
}

void EMWinTrackbarControl::SetCommand(uint32 p_vCommand)
{
	m_vCommand = p_vCommand;
}

void EMWinTrackbarControl::SetFocus()
{
	::SetFocus(m_opWindowHandle);
}

void EMWinTrackbarControl::SetFrame(EMRect p_oFrame)
{
	m_oFrame = p_oFrame;
	MoveWindow(m_opWindowHandle, p_oFrame.m_vLeft, p_oFrame.m_vTop, p_oFrame.GetWidth() + 1, p_oFrame.GetHeight() + 1, true); //Changed to true to test if working ok
}

void EMWinTrackbarControl::SetLabel(const char* p_vpLabel)
{
	SetWindowText(m_opWindowHandle, p_vpLabel);
}

void EMWinTrackbarControl::SetPosition(int32 p_vPosition)
{
	SendMessage(m_opWindowHandle, TBM_SETPOS, static_cast<WPARAM>(true), static_cast<LPARAM>(p_vPosition));
}

void EMWinTrackbarControl::SetRange(int32 p_vStart, int32 p_vEnd)
{
	SendMessage(m_opWindowHandle, TBM_SETRANGE, static_cast<WPARAM>(true), static_cast<LPARAM>(MAKELONG(p_vStart, p_vEnd)));
}

void EMWinTrackbarControl::SetTick(int32 p_vPosition)
{
	SendMessage(m_opWindowHandle, TBM_SETTIC, 0, static_cast<LPARAM>(p_vPosition));
}

void EMWinTrackbarControl::SetWindowWindow(EMWinWindow* p_opWindow)
{
	m_opWindowWindow = p_opWindow;
}

void EMWinTrackbarControl::Show()
{
	ShowWindow(m_opWindowHandle, SW_SHOW);
}

LRESULT CALLBACK EMWinTrackbarControl::SubclassedWindowProcedure(HWND p_vWindowHandle, UINT p_vMessage, WPARAM p_vParameterOne, LPARAM p_vParameterTwo)
{
	EMWinTrackbarControl* opTrackbarControl;
	opTrackbarControl = reinterpret_cast<EMWinTrackbarControl*>(GetWindowLong(p_vWindowHandle, GWL_USERDATA));

	switch(p_vMessage)
	{
	case WM_CHAR:		
eo << "Window: WM_KEYDOWN: " << ef;
		switch(static_cast<int>(p_vParameterOne))
		{
		case EM_RETURN:
			opTrackbarControl -> Notify(EM_GUI_COMPONENT_ENTER_PRESSED);
			return 0;
		case EM_ESCAPE:
			opTrackbarControl -> Notify(EM_GUI_COMPONENT_ESC_PRESSED);
			return 0;
		case EM_TAB:
			if(EM_SHIFT_KEY & EMKeyboard::Instance() -> GetModifiers())
				opTrackbarControl -> Notify(EM_GUI_COMPONENT_SHIFT_TAB_PRESSED);	
			else
				opTrackbarControl -> Notify(EM_GUI_COMPONENT_TAB_PRESSED);
			return 0;
		}
		break;
	case WM_SETFOCUS:
		opTrackbarControl -> Notify(EM_GUI_COMPONENT_GOT_FOCUS);	
		break;
	case WM_PAINT:
		eo << "WM_PAINT" << ef;
		if(opTrackbarControl -> m_vMessage)
			opTrackbarControl -> Notify(opTrackbarControl -> m_vMessage);
		break;
	}

	return CallWindowProc(opTrackbarControl -> m_opDefaultWindowProcedure, p_vWindowHandle, p_vMessage, p_vParameterOne, p_vParameterTwo);
}


#endif
