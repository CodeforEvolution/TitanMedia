#include "EMWinGroupBox.h"

#ifdef PLATFORM_WINDOWS

#include "EMApplication.h"
#include "EMCommandRepository.h"
#include "EMWinView.h"
#include "EMWinWindow.h"

EMWinGroupBox::EMWinGroupBox(const EMRect p_oFrame, const char* p_vpName, uint32 p_vResizingMode) :
m_oFrame(p_oFrame),
m_vpName(p_vpName),
m_vResizingMode(p_vResizingMode)
{
/*	m_opWindowHandle = CreateWindowEx(WS_EX_TRANSPARENT, "button", p_vpName, WS_CHILD | WS_VISIBLE | /*WS_GROUP | /BS_GROUPBOX, m_oFrame.m_vLeft,  
							m_oFrame.m_vTop, m_oFrame.m_vRight - m_oFrame.m_vLeft + 1, m_oFrame.m_vBottom - m_oFrame.m_vTop + 1,
							EMWinWindow::GetDummyWindowHandle(), NULL, EMApplication::GetApplicationInstance(), NULL);
	SendMessage(m_opWindowHandle, WM_SETFONT, reinterpret_cast<WPARAM>(GetStockObject(DEFAULT_GUI_FONT)), true);
	SetWindowLong(m_opWindowHandle, GWL_USERDATA, reinterpret_cast<int32>(this));
*/
}

EMWinGroupBox::~EMWinGroupBox()
{
}

void EMWinGroupBox::AddChild(EMGUIComponent* p_opView)
{
//	AssertWindowLocked();
	if(dynamic_cast<EMWinView*>(p_opView))
		(dynamic_cast<EMWinView*>(p_opView)) -> SetWindowWindow(m_opWindowWindow);
	else if(dynamic_cast<EMWinGroupBox*>(p_opView))
		(dynamic_cast<EMWinGroupBox*>(p_opView)) -> SetWindowWindow(m_opWindowWindow);
	p_opView -> AddToComponent(this);
	m_oChildComponents.push_back(p_opView);
}

bool EMWinGroupBox::AddToComponent(EMGUIComponent* p_opComponent)
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

	m_opWindowHandle = CreateWindowEx(WS_EX_TRANSPARENT, "button", m_vpName, WS_CHILD | WS_VISIBLE | /*WS_GROUP | */BS_GROUPBOX, m_oFrame.m_vLeft,  
							m_oFrame.m_vTop, m_oFrame.m_vRight - m_oFrame.m_vLeft + 1, m_oFrame.m_vBottom - m_oFrame.m_vTop + 1,
							opParent, NULL, EMApplication::GetApplicationInstance(), NULL);
	SendMessage(m_opWindowHandle, WM_SETFONT, reinterpret_cast<WPARAM>(GetStockObject(DEFAULT_GUI_FONT)), true);
	SetWindowLong(m_opWindowHandle, GWL_USERDATA, reinterpret_cast<int32>(this));


//	SetParent(m_opWindowHandle, opParent);
//	SetWindowPos(m_opWindowHandle, NULL, m_oFrame.m_vLeft, m_oFrame.m_vTop, m_oFrame.GetWidth() + 1, m_oFrame.GetHeight() + 1, SWP_NOZORDER);	

	return true;
}

void EMWinGroupBox::Enable(bool p_vEnable)
{
	EnableWindow(m_opWindowHandle, p_vEnable);
}

EMRect EMWinGroupBox::Frame() const
{
	return m_oFrame;
}

void* EMWinGroupBox::GetNativeView() const
{
	return m_opWindowHandle;
}

uint32 EMWinGroupBox::GetResizingMode()
{
	return m_vResizingMode;
}

void EMWinGroupBox::Hide()
{
	ShowWindow(m_opWindowHandle, SW_HIDE);
}

bool EMWinGroupBox::IsEnabled()
{
	return IsWindowEnabled(m_opWindowHandle) != 0;
}

void EMWinGroupBox::SetFrame(EMRect p_oFrame)
{
	m_oFrame = p_oFrame;
	MoveWindow(m_opWindowHandle, p_oFrame.m_vLeft, p_oFrame.m_vTop, p_oFrame.GetWidth() + 1, p_oFrame.GetHeight() + 1, true); //Changed to true to test if working ok
}

void EMWinGroupBox::SetLabel(const char* p_vpLabel)
{
	SetWindowText(m_opWindowHandle, p_vpLabel);
}

void EMWinGroupBox::SetWindowWindow(EMWinWindow* p_opWindow)
{
	m_opWindowWindow = p_opWindow;
}

void EMWinGroupBox::Show()
{
	ShowWindow(m_opWindowHandle, SW_SHOW);
}

#endif
