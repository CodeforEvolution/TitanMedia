#include  "EMWinScrollbar.h"

#ifdef PLATFORM_WINDOWS

#include "EMApplication.h"
#include "EMMessages.h"
#include "EMSemaphore.h"
#include "EMWinView.h"
#include "EMWinGroupBox.h"
#include "EMWinMessages.h"
#include "EMWinWindow.h"

const char* EM_SCROLLBAR_SEM_NAME = "ScrollbarSemaphore";

//EMWinScrollbar* g_opScrollbar;
WNDPROC EMWinScrollbar::m_opOriginalWindowProcedure = NULL;

EMWinScrollbar::EMWinScrollbar(EMRect p_oFrame, int32 p_vMinValue, int32 p_vMaxValue, EMOrientation p_vOrientation) :
m_oFrame(p_oFrame),
m_opSemaphore(EMSemaphore::CreateEMSemaphore(EM_SCROLLBAR_SEM_NAME, 1, 1)),
m_vMaxValue(p_vMaxValue),
m_vMinValue(p_vMinValue),
m_vOrientation(p_vOrientation),
m_vPage(1)
{
//	g_opScrollbar = this;

	m_sScrollInfo.cbSize = sizeof(SCROLLINFO);
	m_sScrollInfo.cbSize = 0;
	m_sScrollInfo.nMax = 0;
	m_sScrollInfo.nMin = 0;
	m_sScrollInfo.nPage = 0;
	m_sScrollInfo.nPos = 0;
	m_sScrollInfo.nTrackPos = 0;

	if(m_vOrientation == EM_HORIZONTAL)
		m_opWindowHandle = CreateWindowEx(0, "scrollbar", NULL, WS_CHILD | WS_VISIBLE/* | WS_TABSTOP^*/ | SBS_HORZ, m_oFrame.m_vLeft,  
								m_oFrame.m_vTop, m_oFrame.m_vRight - m_oFrame.m_vLeft + 1, m_oFrame.m_vBottom - m_oFrame.m_vTop + 1,
								EMWinWindow::GetDummyWindowHandle(), NULL, EMApplication::GetApplicationInstance(), NULL);
	else
		m_opWindowHandle = CreateWindowEx(0, "scrollbar", NULL, WS_CHILD | WS_VISIBLE/* | WS_TABSTOP*/ | SBS_VERT, m_oFrame.m_vLeft,  
								m_oFrame.m_vTop, m_oFrame.m_vRight - m_oFrame.m_vLeft + 1, m_oFrame.m_vBottom - m_oFrame.m_vTop + 1,
								EMWinWindow::GetDummyWindowHandle(), NULL, EMApplication::GetApplicationInstance(), NULL);

	SetWindowLong(m_opWindowHandle, GWL_USERDATA, reinterpret_cast<int32>(this));

	// Subclassing
	m_opOriginalWindowProcedure = reinterpret_cast<WNDPROC>(SetWindowLong(m_opWindowHandle, GWL_WNDPROC, reinterpret_cast<long>(SubclassProcedure)));

	// Why is this needed? Only needed for BeOS?
//	SetRange(p_vMinValue, p_vMaxValue);
}

EMWinScrollbar::~EMWinScrollbar()
{
	delete m_opSemaphore;
}

bool EMWinScrollbar::AddToComponent(EMGUIComponent* p_opComponent)
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
	if(m_vOrientation == EM_HORIZONTAL)
		m_opWindowHandle = CreateWindowEx(0, "scrollbar", NULL, WS_CHILD | WS_VISIBLE | SBS_HORZ, m_oFrame.m_vLeft,  
								m_oFrame.m_vTop, m_oFrame.m_vRight - m_oFrame.m_vLeft, m_oFrame.m_vBottom - m_oFrame.m_vTop,
								opParent, NULL, EMApplication::GetApplicationInstance(), NULL);
	else
		m_opWindowHandle = CreateWindowEx(0, "scrollbar", NULL, WS_CHILD | WS_VISIBLE | SBS_VERT, m_oFrame.m_vLeft,  
								m_oFrame.m_vTop, m_oFrame.m_vRight - m_oFrame.m_vLeft, m_oFrame.m_vBottom - m_oFrame.m_vTop,
								opParent, NULL, EMApplication::GetApplicationInstance(), NULL);

	SetWindowLong(m_opWindowHandle, GWL_USERDATA, reinterpret_cast<int32>(this));
	// Why is this needed?
	SetRange(0, 100);
*/
	SetParent(m_opWindowHandle, opParent);
	SetWindowPos(m_opWindowHandle, NULL, m_oFrame.m_vLeft, m_oFrame.m_vTop, m_oFrame.GetWidth() + 1, m_oFrame.GetHeight() + 1, SWP_NOZORDER);	

	return true;
}

void EMWinScrollbar::Enable(bool p_vEnable)
{
	EnableWindow(m_opWindowHandle, p_vEnable);
}

EMRect EMWinScrollbar::Frame() const
{
	return m_oFrame;
}

void* EMWinScrollbar::GetNativeView() const
{
	return m_opWindowHandle;
}

void EMWinScrollbar::GetRange(int32& p_vMin, int32& p_vMax)
{
	p_vMin = m_vMinValue;
	p_vMax = m_vMaxValue;
/*
	SCROLLINFO sScrollInfo;
	sScrollInfo.cbSize = sizeof(SCROLLINFO);
	sScrollInfo.fMask = SIF_RANGE;
	GetScrollInfo(m_opWindowHandle, SB_CTL, &sScrollInfo);
	p_vMin = sScrollInfo.nMin;
	p_vMax = sScrollInfo.nMax;
	*/
}

uint32 EMWinScrollbar::GetResizingMode()
{
	if(m_vOrientation == EM_VERTICAL)
		return EM_FOLLOW_TOP_BOTTOM | EM_FOLLOW_RIGHT;
	else
		return EM_FOLLOW_BOTTOM | EM_FOLLOW_LEFT_RIGHT;
}

void EMWinScrollbar::GetSteps(int32& p_vSmallStep, int32& p_vBigStep)
{
	p_vSmallStep = m_vStepSmall;
	p_vBigStep = m_vStepBig;
}

int32 EMWinScrollbar::GetValue()
{
//	return GetScrollPos(m_opWindowHandle, SB_CTL);
	SCROLLINFO sScrollInfo;
	sScrollInfo.cbSize = sizeof(SCROLLINFO);
	sScrollInfo.fMask = SIF_POS;
	GetScrollInfo(m_opWindowHandle, SB_CTL, &sScrollInfo);
	return sScrollInfo.nPos;
}

// Generic window procedure passed to WIN32 API
LRESULT CALLBACK EMWinScrollbar::GlobalWindowProcedure(HWND p_vWindowHandle, UINT p_vMessage, WPARAM p_vParameterOne, LPARAM p_vParameterTwo)
{
	EMWinScrollbar * opScrollbar;
	opScrollbar = (EMWinScrollbar *)GetWindowLong(p_vWindowHandle, GWL_USERDATA);

	if(opScrollbar)
	{
		SCROLLINFO sScrollInfo;
		sScrollInfo.cbSize = sizeof(SCROLLINFO);
		sScrollInfo.fMask = SIF_TRACKPOS;
		if(GetScrollInfo(opScrollbar -> m_opWindowHandle, SB_CTL, &sScrollInfo) == 0)
			eo << "EMWinScrollbar::GlobalWindowProcedure: Failed to call GetScrollinfo()" << ef;
//		else
//			eo << "EMWinScrollbar::GlobalWindowProcedure: Got scrollbar pos: " << sScrollInfo.nTrackPos << ef;
		uint32 vMessage;
		switch(p_vMessage)
		{
		case WM_HSCROLL:
			{
				switch(LOWORD(p_vParameterOne))
				{
				case SB_THUMBTRACK:
					SetScrollPos(p_vWindowHandle, SB_CTL, sScrollInfo.nTrackPos/*(int) HIWORD(p_vParameterOne)*/, true);
					break;
				case SB_LEFT:
					SetScrollPos(p_vWindowHandle, SB_CTL, opScrollbar -> m_vMinValue, true);
					break;
				case SB_RIGHT:
					SetScrollPos(p_vWindowHandle, SB_CTL, opScrollbar -> m_vMaxValue, true);
					break;
				case SB_LINELEFT:
					SetScrollPos(p_vWindowHandle, SB_CTL, opScrollbar -> GetValue() - opScrollbar -> m_vStepSmall, true);
					break;
				case SB_LINERIGHT:
					SetScrollPos(p_vWindowHandle, SB_CTL, opScrollbar -> GetValue() + opScrollbar -> m_vStepSmall, true);
					break;
				case SB_PAGELEFT:
					SetScrollPos(p_vWindowHandle, SB_CTL, opScrollbar -> GetValue() - opScrollbar -> m_vStepBig, true);
					break;
				case SB_PAGERIGHT:
					SetScrollPos(p_vWindowHandle, SB_CTL, opScrollbar -> GetValue() + opScrollbar -> m_vStepBig, true);
					break;
				default:
					break;
				}
				vMessage = EM_SCROLLBAR_CHANGED;
				break;
			}
		case WM_VSCROLL:
			{
				switch(LOWORD(p_vParameterOne))
				{
				case SB_THUMBTRACK:
					SetScrollPos(p_vWindowHandle, SB_CTL, sScrollInfo.nTrackPos/*(int) HIWORD(p_vParameterOne)*/, true);
					break;
				case SB_LEFT:
					SetScrollPos(p_vWindowHandle, SB_CTL, opScrollbar -> m_vMinValue, true);
					break;
				case SB_RIGHT:
					SetScrollPos(p_vWindowHandle, SB_CTL, opScrollbar -> m_vMaxValue, true);
					break;
				case SB_LINEUP:
					SetScrollPos(p_vWindowHandle, SB_CTL, opScrollbar -> GetValue() - opScrollbar -> m_vStepSmall, true);
					break;
				case SB_LINEDOWN:
					SetScrollPos(p_vWindowHandle, SB_CTL, opScrollbar -> GetValue() + opScrollbar -> m_vStepSmall, true);
					break;
				case SB_PAGEUP:
					SetScrollPos(p_vWindowHandle, SB_CTL, opScrollbar -> GetValue() - opScrollbar -> m_vStepBig, true);
					break;
				case SB_PAGEDOWN:
					SetScrollPos(p_vWindowHandle, SB_CTL, opScrollbar -> GetValue() + opScrollbar -> m_vStepBig, true);
					break;
				default:
					break;
				}
				vMessage = EM_SCROLLBAR_CHANGED;
				break;
			}
		default:
			vMessage = 0;
		}
		if(vMessage != 0)
		{
			opScrollbar -> m_opWindowWindow -> Lock();
			if(opScrollbar -> Notify(vMessage))
			{
				opScrollbar -> m_opWindowWindow -> Unlock();
				return 0;
			}
			opScrollbar -> m_opWindowWindow -> Unlock();
		}
	}
	return DefWindowProc(p_vWindowHandle, p_vMessage, p_vParameterOne, p_vParameterTwo);
}

void EMWinScrollbar::Hide()
{
	ShowWindow(m_opWindowHandle, SW_HIDE);
}

bool EMWinScrollbar::IsEnabled()
{
	return IsWindowEnabled(m_opWindowHandle) != 0;
}

EMOrientation EMWinScrollbar::Orientation()
{
	return m_vOrientation;
}

void EMWinScrollbar::SetFrame(EMRect p_oFrame)
{
	m_oFrame = p_oFrame;
	MoveWindow(m_opWindowHandle, p_oFrame.m_vLeft, p_oFrame.m_vTop, p_oFrame.GetWidth() + 1, p_oFrame.GetHeight() + 1, true);
}

void EMWinScrollbar::SetPageAndRange(uint32 p_vPage, uint32 p_vMin, uint32 p_vMax)
{
	m_vMinValue = p_vMin;
	m_vMaxValue = p_vMax;
	m_sScrollInfo.fMask = SIF_PAGE | SIF_RANGE | SIF_DISABLENOSCROLL;
	m_sScrollInfo.nPage = p_vPage + 1;
	m_sScrollInfo.nMin = m_vMinValue;
	m_sScrollInfo.nMax = m_vMaxValue;
	m_opSemaphore -> Acquire();
	m_oPostedScrollInfos.push_back(m_sScrollInfo);
	m_opSemaphore -> Release();
	PostMessage(m_opWindowHandle, NATIVE_MESSAGE_SET_SCROLL_INFO, 0, 0);
}
/*
void EMWinScrollbar::SetProportion(int32 p_vProportion)
{
//eo << "EMWinScrollbar::SetProportion: " << p_vProportion << ef;
	if(p_vProportion > 1)
		p_vProportion = 1;

	int32 vMin;
	int32 vMax;
	GetRange(vMin, vMax);

	int32 vPage = (vMax - vMin) * p_vProportion;
	if(vPage - static_cast<int>(vPage) < 0.5)
		vPage = static_cast<int>(vPage);
	else
		vPage = static_cast<int>(vPage) + 1;
vPage = vMax - 3;
	if(vPage < 1)
		vPage = 1;
	m_vPage = vPage;

//eo << "EMWinScrollbar::SetProportion2: " << p_vProportion << ", " << vPage << ef;
//	SCROLLINFO sScrollInfo;
//	sScrollInfo.cbSize = sizeof(SCROLLINFO);
	m_sScrollInfo.fMask = SIF_PAGE | SIF_RANGE | SIF_DISABLENOSCROLL;
	m_sScrollInfo.nPage = 90;//vPage;
	m_sScrollInfo.nMin = 0;//m_vMinValue;
	m_sScrollInfo.nMax = 100;//m_vMaxValue + m_vPage;
	m_opSemaphore -> Acquire();
	m_oPostedScrollInfos.push_back(m_sScrollInfo);
	m_opSemaphore -> Release();
	PostMessage(m_opWindowHandle, NATIVE_MESSAGE_SET_SCROLL_INFO, 0, 0);
//	SetScrollInfo(m_opWindowHandle, SB_CTL, &sScrollInfo, false);//true);
}

// SetProportion must be called after SetRange for the scrollbar to change
void EMWinScrollbar::SetRange(int32 p_vMin, int32 p_vMax)
{
//eo << "EMWinScrollbar::SetRange: " << p_vMin << ", " << p_vMax << ef;
//	if(p_vMax + m_vPage < p_vMin)
//		p_vMax = p_vMin + m_vPage;
	m_vMinValue = p_vMin;
	m_vMaxValue = p_vMax;

//	m_sScrollInfo.fMask = SIF_RANGE | SIF_DISABLENOSCROLL;
//	m_sScrollInfo.nMin = p_vMin;
//	m_sScrollInfo.nMax = p_vMax + m_vPage;
//	m_opSemaphore -> Acquire();
//	m_oPostedScrollInfos.push_back(m_sScrollInfo);
//	m_opSemaphore -> Release();
//	int vResult = PostMessage(m_opWindowHandle, NATIVE_MESSAGE_SET_SCROLL_INFO, 0, 0);

}
*/
void EMWinScrollbar::SetSteps(int32 p_vSmallStep, int32 p_vBigStep)
{
//eo << "EMWinScrollbar::SetSteps: " << p_vSmallStep << ", " << p_vBigStep << ef;
	m_vStepSmall = p_vSmallStep;
	m_vStepBig = p_vBigStep;
}

void EMWinScrollbar::SetValue(int32 p_vValue)
{
//eo << "EMWinScrollbar::SetValue: " << p_vValue << ef;
//	SetScrollPos(m_opWindowHandle, SB_CTL, p_vValue, true);
//	SCROLLINFO sScrollInfo;
//	sScrollInfo.cbSize = sizeof(SCROLLINFO);
	m_sScrollInfo.fMask = SIF_POS | SIF_DISABLENOSCROLL;
	m_sScrollInfo.nPos = p_vValue;
	m_opSemaphore -> Acquire();
	m_oPostedScrollInfos.push_back(m_sScrollInfo);
	m_opSemaphore -> Release();
	PostMessage(m_opWindowHandle, NATIVE_MESSAGE_SET_SCROLL_INFO, 1, 0);
//	SetScrollInfo(m_opWindowHandle, SB_CTL, &sScrollInfo, false);//true);
}

void EMWinScrollbar::SetWindowWindow(EMWinWindow* p_opWindow)
{
	m_opWindowWindow = p_opWindow;
}

void EMWinScrollbar::Show()
{
	ShowWindow(m_opWindowHandle, SW_SHOW);
}

LRESULT CALLBACK EMWinScrollbar::SubclassProcedure(HWND p_vWindowHandle, UINT p_vMessage, WPARAM p_vParameterOne, LPARAM p_vParameterTwo)
{
	if(p_vMessage == NATIVE_MESSAGE_SET_SCROLL_INFO)
	{
		EMWinScrollbar * opScrollbar = reinterpret_cast<EMWinScrollbar*>(GetWindowLong(p_vWindowHandle, GWL_USERDATA));
		opScrollbar -> m_opSemaphore -> Acquire();
		SCROLLINFO sScrollInfo = opScrollbar -> m_oPostedScrollInfos.front();
		opScrollbar -> m_oPostedScrollInfos.pop_front();
		opScrollbar -> m_opSemaphore -> Release();
// FIXME: Why is the size not already set?
sScrollInfo.cbSize = sizeof(SCROLLINFO);
//eo << "NATIVE_MESSAGE_SET_SCROLL_INFO 1: " << sScrollInfo.cbSize << ", " << sScrollInfo.fMask << ", " << sScrollInfo.nMax << ", " << sScrollInfo.nMin << ", " << sScrollInfo.nPage << ", " << sScrollInfo.nPos << ef;
		int vResult = SetScrollInfo(p_vWindowHandle, SB_CTL, &sScrollInfo, true);//true);
//eo << "NATIVE_MESSAGE_SET_SCROLL_INFO 2: vResult: " << vResult << ", " << sScrollInfo.cbSize << ", " << sScrollInfo.fMask << ", " << sScrollInfo.nMax << ", " << sScrollInfo.nMin << ", " << sScrollInfo.nPage << ", " << sScrollInfo.nPos << ef;
		if(p_vParameterOne != 0)
		{
			opScrollbar -> m_opWindowWindow -> Lock();
			opScrollbar -> Notify(EM_SCROLLBAR_CHANGED);
			opScrollbar -> m_opWindowWindow -> Unlock();
		}

		return 0;
	}
	return CallWindowProc(m_opOriginalWindowProcedure, p_vWindowHandle, p_vMessage, p_vParameterOne, p_vParameterTwo);
}

#endif

