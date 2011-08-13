#include "EMWinLabel.h"

#ifdef PLATFORM_WINDOWS

#include "EMApplication.h"
#include "EMWinGroupBox.h"
#include "EMWinView.h"
#include "EMWinWindow.h"

EMWinLabel::EMWinLabel(const EMRect p_oFrame, const char* p_vpName, uint32 p_vResizingMode, const EMAlignment p_eAlignment) :
m_oFrame(p_oFrame),
m_vName(p_vpName),
m_vResizingMode(p_vResizingMode)
{
	int32 vStyle(WS_CHILD | WS_VISIBLE);
	if(p_eAlignment == EM_ALIGNMENT_LEFT)
		vStyle |= SS_LEFTNOWORDWRAP;
	else if(p_eAlignment == EM_ALIGNMENT_CENTER)
		vStyle |= SS_CENTER;
	else if(p_eAlignment == EM_ALIGNMENT_RIGHT)
		vStyle |= SS_RIGHT;
	m_opWindowHandle = CreateWindowEx(0, "static", p_vpName, vStyle, m_oFrame.m_vLeft,  
							m_oFrame.m_vTop, m_oFrame.m_vRight - m_oFrame.m_vLeft + 1, m_oFrame.m_vBottom - m_oFrame.m_vTop + 1,
							EMWinWindow::GetDummyWindowHandle(), NULL, EMApplication::GetApplicationInstance(), NULL);

	SendMessage(m_opWindowHandle, WM_SETFONT, reinterpret_cast<WPARAM>(GetStockObject(DEFAULT_GUI_FONT)), true);
	SetWindowLong(m_opWindowHandle, GWL_USERDATA, reinterpret_cast<int32>(this));
}

EMWinLabel::~EMWinLabel()
{
}

bool EMWinLabel::AddToComponent(EMGUIComponent* p_opComponent)
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

void EMWinLabel::Enable(bool p_vEnable)
{
	EnableWindow(m_opWindowHandle, p_vEnable);
}

EMRect EMWinLabel::Frame() const
{
	return m_oFrame;
}

void* EMWinLabel::GetNativeView() const
{
	return m_opWindowHandle;
}

float EMWinLabel::GetStringWidth()
{
	SIZE sSize;
	HDC opPaintHandle = CreateCompatibleDC(NULL);
	SelectObject(opPaintHandle, GetStockObject(ANSI_VAR_FONT));
	GetTextExtentPoint32(opPaintHandle, m_vName.c_str(), m_vName.length(), &sSize);
	DeleteDC(opPaintHandle);
	eo << m_vName.c_str() << "     sSize.cx = " << sSize.cx << ef;
	return sSize.cx;
}

uint32 EMWinLabel::GetResizingMode()
{
	return m_vResizingMode;
}

void EMWinLabel::Hide()
{
	ShowWindow(m_opWindowHandle, SW_HIDE);
}

bool EMWinLabel::IsEnabled()
{
	return IsWindowEnabled(m_opWindowHandle) != 0;
}

void EMWinLabel::SetFrame(EMRect p_oFrame)
{
	m_oFrame = p_oFrame;
	MoveWindow(m_opWindowHandle, p_oFrame.m_vLeft, p_oFrame.m_vTop, p_oFrame.GetWidth() + 1, p_oFrame.GetHeight() + 1, true); //Changed to true to test if working ok
}

void EMWinLabel::SetLabel(const char* p_vpLabel)
{
	SetWindowText(m_opWindowHandle, p_vpLabel);
	m_vName = p_vpLabel;
}

void EMWinLabel::Show()
{
	ShowWindow(m_opWindowHandle, SW_SHOW);
}

#endif