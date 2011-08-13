#include "EMWinTabControl.h"

#ifdef PLATFORM_WINDOWS

#include "EMApplication.h"
#include "EMCommandRepository.h"
#include "EMKeyboard.h"
#include "EMKeyboardGlobals.h"
#include "EMMessages.h"
#include "EMWinGroupBox.h"
#include "EMWinView.h"
#include "EMWinWindow.h"

EMWinTabControl::EMWinTabControl(const EMRect p_oFrame, const char* p_vpName, bool p_vMultiLined, uint32 p_vMessage, uint32 p_vResizingMode, uint32 p_vCommand) :
m_oFrame(p_oFrame),
m_vCommand(p_vCommand),
m_vMessage(p_vMessage),
m_vResizingMode(p_vResizingMode),
m_vId(0),
m_vStyle(WS_CHILD | WS_VISIBLE)
{
	INITCOMMONCONTROLSEX oInitCommonControlEX;
	oInitCommonControlEX.dwSize = sizeof(oInitCommonControlEX);
	oInitCommonControlEX.dwICC = ICC_TAB_CLASSES;

	// To get resize, move to work, i.e. so it doesn't add a pixel on this control
	m_oFrame.m_vRight--;	
	m_oFrame.m_vBottom--; 

	if(p_vMultiLined)
		m_vStyle |= TCS_MULTILINE;

	if(InitCommonControlsEx(&oInitCommonControlEX))
	{
		m_opWindowHandle = CreateWindowEx( 0, WC_TABCONTROL, p_vpName, m_vStyle, m_oFrame.m_vLeft,  
								m_oFrame.m_vTop, m_oFrame.m_vRight - m_oFrame.m_vLeft + 1, m_oFrame.m_vBottom - m_oFrame.m_vTop + 1,
								EMWinWindow::GetDummyWindowHandle(), NULL, EMApplication::GetApplicationInstance(), NULL);
		SendMessage(m_opWindowHandle, WM_SETFONT, reinterpret_cast<WPARAM>(GetStockObject(DEFAULT_GUI_FONT)), true);
		SetWindowLong(m_opWindowHandle, GWL_USERDATA, reinterpret_cast<int32>(this));

		m_opDefaultWindowProcedure = reinterpret_cast<WNDPROC>(GetWindowLong(m_opWindowHandle, GWL_WNDPROC));
		SetWindowLong(m_opWindowHandle, GWL_WNDPROC, reinterpret_cast<LONG>(SubclassedWindowProcedure));
	}
	else
		eo << "Could not load CommonControl dll" << ef;
}

EMWinTabControl::~EMWinTabControl()
{
	SetWindowLong(m_opWindowHandle, GWL_WNDPROC, reinterpret_cast<LONG>(m_opDefaultWindowProcedure));
	SetWindowLong(m_opWindowHandle, GWL_USERDATA, 0);
	DestroyWindow(m_opWindowHandle);
}

void EMWinTabControl::AddChild(EMGUIComponent* p_opView)
{
//	AssertWindowLocked();
	if(dynamic_cast<EMWinView*>(p_opView))
		(dynamic_cast<EMWinView*>(p_opView)) -> SetWindowWindow(m_opWindowWindow);
	else if(dynamic_cast<EMWinGroupBox*>(p_opView))
		(dynamic_cast<EMWinGroupBox*>(p_opView)) -> SetWindowWindow(m_opWindowWindow);
	else if(dynamic_cast<EMWinTabControl*>(p_opView))
		(dynamic_cast<EMWinTabControl*>(p_opView)) -> SetWindowWindow(m_opWindowWindow);
	p_opView -> AddToComponent(this);
	m_oChildComponents.push_back(p_opView);
}

bool EMWinTabControl::AddTab(char* p_vTabName, EMView* p_opTabView)
{
	TCITEM oTcItem;
	oTcItem.mask = TCIF_TEXT;
	oTcItem.pszText = p_vTabName;
	
	//static int vId = 0;

//	RECT oRc;
//	EMRect oRect = Frame();
/*
	oRc.left = oRect.m_vLeft;
	oRc.top = oRect.m_vTop;
	oRc.right = oRect.m_vRight;
	oRc.bottom = oRect.m_vBottom;

	AdjustWindowRect(&oRc, m_vStyle, FALSE);

	oRect.m_vRight = -oRc.left + oRc.right;
	oRect.m_vBottom = -oRc.top + oRc.bottom;
*/

	if(m_vId > 0)
		p_opTabView -> Hide();
	m_oViewVector.push_back(p_opTabView);

	bool vResult(TabCtrl_InsertItem(m_opWindowHandle, m_vId, &oTcItem) != -1);

	AdjustViewFrames(m_vId++);

/*	oRc.left = 0;
	oRc.top = 0;
	oRc.right = oRect.m_vRight - oRect.m_vLeft;
	oRc.bottom = oRect.m_vBottom - oRect.m_vTop;

	TabCtrl_AdjustRect(m_opWindowHandle, FALSE, &oRc);
	
	p_opTabView -> LockWindow();
//eo << "TabCtrl_GetRowCount(m_opWindowHandle)" << TabCtrl_GetRowCount(m_opWindowHandle) << ef;
	p_opTabView -> SetFrame(EMRect(oRc.left, oRc.top, oRc.right, oRc.bottom));
	
	if(m_oViewVector[0] -> Frame().m_vTop < m_oViewVector[m_vId - 1] -> Frame().m_vTop)
		for(int vVectorCounter = 0; vVectorCounter < m_vId - 1; vVectorCounter++)
			m_oViewVector[vVectorCounter] -> SetFrame(m_oViewVector[m_vId - 1] -> Frame());
   
	p_opTabView -> UnlockWindow();
*/
	return vResult;
}

bool EMWinTabControl::AddToComponent(EMGUIComponent* p_opComponent)
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

void EMWinTabControl::AdjustViewFrames(int p_vId)
{
	RECT oRc;
	EMRect oRect = Frame();

	oRc.left = 0;
	oRc.top = 0;
	oRc.right = oRect.m_vRight - oRect.m_vLeft;
	oRc.bottom = oRect.m_vBottom - oRect.m_vTop;

	TabCtrl_AdjustRect(m_opWindowHandle, FALSE, &oRc);
	
	m_oViewVector[p_vId] -> LockWindow();
//eo << "TabCtrl_GetRowCount(m_opWindowHandle)" << TabCtrl_GetRowCount(m_opWindowHandle) << ef;
	m_oViewVector[p_vId] -> SetFrame(EMRect(oRc.left, oRc.top, oRc.right, oRc.bottom));
	
	if(m_oViewVector[0] -> Frame().m_vTop < m_oViewVector[m_vId - 1] -> Frame().m_vTop)
		for(int vVectorCounter = 0; vVectorCounter < m_vId - 1; vVectorCounter++)
			m_oViewVector[vVectorCounter] -> SetFrame(m_oViewVector[m_vId - 1] -> Frame());
   
	m_oViewVector[p_vId] -> UnlockWindow();
}

void EMWinTabControl::ExecuteCommand()
{
	if(m_vCommand != 0)
		EMCommandRepository::Instance() -> ExecuteCommand(m_vCommand);
}

void EMWinTabControl::Enable(bool p_vEnable)
{
	EnableWindow(m_opWindowHandle, p_vEnable);
}

EMRect EMWinTabControl::Frame() const
{
	return m_oFrame;
}

int EMWinTabControl::GetTab()
{
	return TabCtrl_GetCurSel(m_opWindowHandle);
}

void* EMWinTabControl::GetNativeView() const
{
	return m_opWindowHandle;
}

uint32 EMWinTabControl::GetResizingMode()
{
	return m_vResizingMode;
}

// Generic window procedure passed to WIN32 API
LRESULT CALLBACK EMWinTabControl::GlobalWindowProcedure(HWND p_vWindowHandle, UINT p_vMessage, WPARAM p_vParameterOne, LPARAM p_vParameterTwo)
{
eo << "EMWinTabControl::GlobalWindowProcedure" << ef;
	EMWinTabControl* opTabControl;
	opTabControl = reinterpret_cast<EMWinTabControl*>(GetWindowLong(p_vWindowHandle, GWL_USERDATA));

/*	eo << "BN_CLICKED     " << BN_CLICKED << ef;
	eo << "NM_CLICK       " << NM_CLICK << ef;
	eo << "NM_DBLCLK      " << NM_DBLCLK << ef;
	eo << "NM_RCLICK      " << NM_RCLICK << ef;
	eo << "NM_RDBLCLK     " << NM_RDBLCLK << ef;
	eo << "NM_RETURN      " << NM_RETURN << ef;
	eo << "NM_SETFOCUS    " << NM_SETFOCUS << ef;
	eo << "NM_KILLFOCUS   " << NM_KILLFOCUS << ef;
	eo << "NM_OUTOFMEMORY " << NM_OUTOFMEMORY << ef;
	eo << "NM_FIRST  " << NM_FIRST << " " << 4294966574 << ef;
	eo << "LVN_FIRST " << LVN_FIRST << ef;
	eo << "HDN_FIRST " << HDN_FIRST << ef;
	eo << "TVN_FIRST " << TVN_FIRST << ef;
	eo << "TTN_FIRST " << TTN_FIRST << ef;
	eo << "TCN_FIRST " << TCN_FIRST << ef;
	eo << "CDN_FIRST " << CDN_FIRST << ef;
	eo << "TBN_FIRST " << TBN_FIRST << ef;
	eo << "TBN_LAST " << TBN_LAST << ef;
	eo << "UDN_FIRST " << UDN_FIRST << ef;
*/
	if(opTabControl)
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

//		eo << "Code = " << reinterpret_cast<LPNMHDR>(p_vParameterTwo) -> code << ef;

		switch(reinterpret_cast<LPNMHDR>(p_vParameterTwo) -> code)
		{
		case NM_CLICK:
			eo << "NM_CLICK " << NM_CLICK << ef;
			vMessage = 0;
			opTabControl -> ExecuteCommand();
			if(opTabControl -> m_vMessage != 0)
				opTabControl -> Notify(opTabControl -> m_vMessage);
			break;
		case TCN_SELCHANGE:
			eo << "TCN_SELCHANGE" << ef;
			eo << "Tab Id = " << opTabControl -> GetTab() << ef;
			opTabControl -> ShowTab();
			opTabControl -> SetFocus();
			break;
		case TCN_SELCHANGING:
			eo << "TCN_SELCHANGING" << ef;
			eo << "Tab Id = " << opTabControl -> GetTab() << ef;
			opTabControl -> HideTab();
			break;
		default:
			eo << "Default" << ef;
			vMessage = 0;
		}
		if(vMessage != 0)
			if(opTabControl -> Notify(vMessage))
				return 0;
	}
	return DefWindowProc(p_vWindowHandle, p_vMessage, p_vParameterOne, p_vParameterTwo);
}

void EMWinTabControl::Hide()
{
	ShowWindow(m_opWindowHandle, SW_HIDE);
}

void EMWinTabControl::HideTab()
{
	m_oViewVector[GetTab()] -> Hide();
}

bool EMWinTabControl::IsEnabled()
{
	return IsWindowEnabled(m_opWindowHandle) != 0;
}

void EMWinTabControl::SetCommand(uint32 p_vCommand)
{
	m_vCommand = p_vCommand;
}

void EMWinTabControl::SetFocus()
{
	::SetFocus(m_opWindowHandle);
}

void EMWinTabControl::SetFrame(EMRect p_oFrame)
{
	m_oFrame = p_oFrame;
	MoveWindow(m_opWindowHandle, p_oFrame.m_vLeft, p_oFrame.m_vTop, p_oFrame.GetWidth() + 1, p_oFrame.GetHeight() + 1, true); //Changed to true to test if working ok
	for(int vVectorCounter = 0; vVectorCounter < m_vId; vVectorCounter++)
		AdjustViewFrames(vVectorCounter);
}

void EMWinTabControl::SetLabel(const char* p_vpLabel)
{
	SetWindowText(m_opWindowHandle, p_vpLabel);
}

bool EMWinTabControl::SetTab(int p_vTabIndex)
{
	return TabCtrl_SetCurSel(m_opWindowHandle, p_vTabIndex) > -1;
}

void EMWinTabControl::SetWindowWindow(EMWinWindow* p_opWindow)
{
	m_opWindowWindow = p_opWindow;
}

void EMWinTabControl::Show()
{
	ShowWindow(m_opWindowHandle, SW_SHOW);
}

void EMWinTabControl::ShowTab()
{
	m_oViewVector[GetTab()] -> Show();
}

LRESULT CALLBACK EMWinTabControl::SubclassedWindowProcedure(HWND p_vWindowHandle, UINT p_vMessage, WPARAM p_vParameterOne, LPARAM p_vParameterTwo)
{
	EMWinTabControl* opTabControl;
	opTabControl = reinterpret_cast<EMWinTabControl*>(GetWindowLong(p_vWindowHandle, GWL_USERDATA));

	if(p_vMessage == WM_CHAR)
	{					
eo << "Window: WM_KEYDOWN: " << ef;
		switch(static_cast<int>(p_vParameterOne))
		{
		case EM_RETURN:
			opTabControl -> Notify(EM_GUI_COMPONENT_ENTER_PRESSED);
			return 0;
		case EM_ESCAPE:
			opTabControl -> Notify(EM_GUI_COMPONENT_ESC_PRESSED);
			return 0;
		case EM_TAB:
			if(EM_SHIFT_KEY & EMKeyboard::Instance() -> GetModifiers())
				opTabControl -> Notify(EM_GUI_COMPONENT_SHIFT_TAB_PRESSED);	
			else
				opTabControl -> Notify(EM_GUI_COMPONENT_TAB_PRESSED);
			return 0;
		}
	}
	if(p_vMessage == WM_SETFOCUS)
		opTabControl -> Notify(EM_GUI_COMPONENT_GOT_FOCUS);	

	return CallWindowProc(opTabControl -> m_opDefaultWindowProcedure, p_vWindowHandle, p_vMessage, p_vParameterOne, p_vParameterTwo);
}


#endif
