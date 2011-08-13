#include "EMWinProgressBar.h"

#ifdef PLATFORM_WINDOWS

#include "EMApplication.h"
#include "EMRect.h"
#include "EMWinGroupBox.h"
#include "EMWinView.h"
#include "EMWinWindow.h"

EMWinProgressBar::EMWinProgressBar(EMRect p_oFrame, uint32 p_vMessage) :
m_oFrame(p_oFrame),
m_vMessage(p_vMessage)
{
	INITCOMMONCONTROLSEX oInitCommonControlEX;
	oInitCommonControlEX.dwSize = sizeof(oInitCommonControlEX);
	oInitCommonControlEX.dwICC = ICC_TAB_CLASSES;

	if(InitCommonControlsEx(&oInitCommonControlEX))
	{
		m_opWindowHandle = CreateWindowEx(0, PROGRESS_CLASS, NULL, WS_CHILD | WS_VISIBLE | PBS_SMOOTH, 
										  m_oFrame.m_vLeft, m_oFrame.m_vTop,
										  m_oFrame.GetWidth(), m_oFrame.GetHeight(),
										  EMWinWindow::GetDummyWindowHandle(), NULL, 
										  EMApplication::GetApplicationInstance(), NULL);
		SetWindowLong(m_opWindowHandle, GWL_USERDATA, reinterpret_cast<int32>(this));
	}
	else
		eo << "Could not load CommonControl dll" << ef;
}

EMWinProgressBar::~EMWinProgressBar()
{
}

bool EMWinProgressBar::AddToComponent(EMGUIComponent* p_opComponent)
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
	SetWindowPos(m_opWindowHandle, NULL, Frame().m_vLeft, Frame().m_vTop, Frame().GetWidth(), Frame().GetHeight(), SWP_NOZORDER);	

	return true;
}

bool EMWinProgressBar::CanGetTab()
{
	return false;
}

EMRect EMWinProgressBar::Frame() const
{
	return m_oFrame;
}

void* EMWinProgressBar::GetNativeView() const
{
	return static_cast<void*>(m_opWindowHandle);
}

uint16 EMWinProgressBar::GetPosition()
{
	return static_cast<uint16>(SendMessage(m_opWindowHandle, PBM_GETPOS, 0, 0));
}

void EMWinProgressBar::Hide()
{
	ShowWindow(m_opWindowHandle, SW_HIDE);
}

bool EMWinProgressBar::IsFinnished()
{
	if(m_vCurrentValue < m_vEndValue)
		return false;
	return true;
}

void EMWinProgressBar::SetEndValue(uint16 p_vEnd)
{
	m_vEndValue = p_vEnd;
	SendMessage(m_opWindowHandle, PBM_SETRANGE, 0, MAKELPARAM(0, p_vEnd));
}

void EMWinProgressBar::SetFrame(EMRect p_oFrame)
{
	m_oFrame = p_oFrame;
	//MoveWindow(m_opWindowHandle, m_oFrame.m_vLeft, m_oFrame.m_vTop, m_oFrame.GetWidth(), m_oFrame.GetHeight(), true);
}

void EMWinProgressBar::SetPosition(uint16 p_vPosition)
{
	m_vCurrentValue = p_vPosition;
	SendMessage(m_opWindowHandle, PBM_SETPOS, static_cast<WPARAM>(p_vPosition), 0);
}

void EMWinProgressBar::Show()
{
	ShowWindow(m_opWindowHandle, SW_SHOW);
}

#endif