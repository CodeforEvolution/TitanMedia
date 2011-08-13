#include "EMWinToolTip.h"

#ifdef PLATFORM_WINDOWS

#include "EMApplication.h"
#include "EMWinWindow.h"

#include <commctrl.h>

// If p_vAutomatic is false, the tooltip must be shown and hidden manually (it becomes a so called "tracking" tooltip)
EMWinToolTip::EMWinToolTip(const EMRect p_oFrame, const char* p_vpLabel, void* p_vpParentNativeView, bool p_vAutomatic) :
m_oFrame(p_oFrame),
m_vAutomatic(p_vAutomatic)
{
//eo << "New tooltip - Label: " << p_vpLabel << ", frame:" << p_oFrame.m_vLeft << ", " << p_oFrame.m_vTop << ", " << p_oFrame.m_vRight << ", " << p_oFrame.m_vBottom << ef;

	if(p_vpParentNativeView != NULL)
		m_opParentWindowHandle = reinterpret_cast<HWND>(p_vpParentNativeView);//dynamic_cast<EMWinWindow*>(static_cast<EMWindow*>(p_vpParentNativeView)) -> GetWindowHandle();
	else
		EMDebugger("p_vpParentNativeView is not specified");
		//vpParentNativeWindowHandle = EMWinWindow::GetDummyWindowHandle();


	m_opWindowHandle = CreateWindowEx(WS_EX_TOPMOST, TOOLTIPS_CLASS, NULL, WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,
							CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, m_opParentWindowHandle,
							NULL, EMApplication::GetApplicationInstance(), NULL);

    SetWindowPos(m_opWindowHandle, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE); // Needed?

	TOOLINFO sInfo;
    sInfo.cbSize = sizeof(TOOLINFO);
	if(!p_vAutomatic)
	    sInfo.uFlags = /*TTF_IDISHWND | */TTF_TRACK | TTF_ABSOLUTE;
	else
		sInfo.uFlags = TTF_SUBCLASS;
    sInfo.hwnd = m_opParentWindowHandle;
    sInfo.hinst = EMApplication::GetApplicationInstance();
    sInfo.uId = reinterpret_cast<uint32>(m_opParentWindowHandle);
    sInfo.lpszText = const_cast<char*>(p_vpLabel);

	sInfo.rect.left = p_oFrame.m_vLeft;    
	sInfo.rect.top = p_oFrame.m_vTop;
	sInfo.rect.right = p_oFrame.m_vRight;
	sInfo.rect.bottom = p_oFrame.m_vBottom;

	SetWindowLong(m_opWindowHandle, GWL_USERDATA, reinterpret_cast<int32>(this));

	SendMessage(m_opWindowHandle, TTM_ADDTOOL, 0, reinterpret_cast<LPARAM>(static_cast<LPTOOLINFO>(&sInfo)));
}

EMWinToolTip::~EMWinToolTip()
{
	SetWindowLong(m_opWindowHandle, GWL_USERDATA, 0);
	DestroyWindow(m_opWindowHandle);
}

bool EMWinToolTip::AddToComponent(EMGUIComponent* p_opComponent)
{
	return true;
}

void EMWinToolTip::Enable(bool p_vEnable)
{
	EMDebugger("Method not supported");
}

EMRect EMWinToolTip::Frame() const
{
	return m_oFrame;
}

void* EMWinToolTip::GetNativeView() const
{
	return m_opWindowHandle;
}
/*
// Generic window procedure passed to WIN32 API
LRESULT CALLBACK EMWinToolTip::GlobalWindowProcedure(HWND p_vWindowHandle, UINT p_vMessage, WPARAM p_vParameterOne, LPARAM p_vParameterTwo)
{
//eo << "EMWinToolTip::GlobalWindowProcedure" << ef;
	EMWinToolTip* opToolTip;

	opToolTip = reinterpret_cast<EMWinToolTip*>(GetWindowLong(p_vWindowHandle, GWL_USERDATA));

	if(!opToolTip)
		return DefWindowProc(p_vWindowHandle, p_vMessage, p_vParameterOne, p_vParameterTwo);

	if(opToolTip -> m_vAutomatic)
		return DefWindowProc(p_vWindowHandle, p_vMessage, p_vParameterOne, p_vParameterTwo);

	uint32 vMessage;
	switch(HIWORD(p_vParameterOne))
	{
		case 
	default:
		vMessage = 0;
	}
	if(vMessage != 0)
		if(opButton -> Notify(vMessage))
			return 0;

	return DefWindowProc(p_vWindowHandle, p_vMessage, p_vParameterOne, p_vParameterTwo);
}
*/
void EMWinToolTip::Hide()
{
	TOOLINFO sInfo;
    sInfo.cbSize = sizeof(TOOLINFO);
    sInfo.uFlags = 0;
    sInfo.hwnd = m_opParentWindowHandle;
    sInfo.hinst = 0;
    sInfo.uId = reinterpret_cast<uint32>(m_opParentWindowHandle);
    sInfo.lpszText = NULL;
	/*
    sInfo.rect.left = p_oFrame.m_vLeft;    
    sInfo.rect.top = p_oFrame.m_vTop;
    sInfo.rect.right = p_oFrame.m_vRight;
    sInfo.rect.bottom = p_oFrame.m_vBottom;
    */
	SendMessage(m_opWindowHandle, TTM_TRACKACTIVATE, static_cast<WPARAM>(false), reinterpret_cast<LPARAM>(&sInfo));
}

bool EMWinToolTip::IsEnabled()
{
	EMDebugger("Method not supported");
	return true;
}

void EMWinToolTip::SetFrame(EMRect p_oFrame)
{
/*
eo << "EMWinToolTip::SetFrame - Old frame: ";
m_oFrame.Print();
eo << "New frame:";
p_oFrame.Print();
*/
	m_oFrame = p_oFrame;

	TOOLINFO sInfo;
    sInfo.cbSize = sizeof(TOOLINFO);
    sInfo.uFlags = 0;
    sInfo.hwnd = m_opParentWindowHandle;
    sInfo.hinst = 0;
    sInfo.uId = reinterpret_cast<uint32>(m_opParentWindowHandle);
    sInfo.lpszText = NULL;
    sInfo.rect.left = p_oFrame.m_vLeft;    
    sInfo.rect.top = p_oFrame.m_vTop;
    sInfo.rect.right = p_oFrame.m_vRight;
    sInfo.rect.bottom = p_oFrame.m_vBottom;
    
	SendMessage(m_opWindowHandle, TTM_NEWTOOLRECT, 0, reinterpret_cast<LPARAM>(&sInfo));
}

void EMWinToolTip::SetLabel(const char* p_vpLabel)
{
	TOOLINFO sInfo;
    sInfo.cbSize = sizeof(TOOLINFO);
    sInfo.uFlags = 0;
    sInfo.hwnd = m_opParentWindowHandle;
    sInfo.hinst = 0;
    sInfo.uId = reinterpret_cast<uint32>(m_opParentWindowHandle);
    sInfo.lpszText = const_cast<char*>(p_vpLabel);
	/*
    sInfo.rect.left = p_oFrame.m_vLeft;    
    sInfo.rect.top = p_oFrame.m_vTop;
    sInfo.rect.right = p_oFrame.m_vRight;
    sInfo.rect.bottom = p_oFrame.m_vBottom;
    */
	SendMessage(m_opWindowHandle, TTM_UPDATETIPTEXT, static_cast<WPARAM>(true), reinterpret_cast<LPARAM>(&sInfo));
}

void EMWinToolTip::SetPosition(int32 p_vX, int32 p_vY)
{
	SendMessage(m_opWindowHandle, TTM_TRACKPOSITION, 0, static_cast<LPARAM>(MAKELPARAM(p_vX, p_vY)));
}

void EMWinToolTip::Show()
{
	TOOLINFO sInfo;
    sInfo.cbSize = sizeof(TOOLINFO);
    sInfo.uFlags = 0;
    sInfo.hwnd = m_opParentWindowHandle;
    sInfo.hinst = 0;
    sInfo.uId = reinterpret_cast<uint32>(m_opParentWindowHandle);
    sInfo.lpszText = NULL;
	/*
    sInfo.rect.left = p_oFrame.m_vLeft;    
    sInfo.rect.top = p_oFrame.m_vTop;
    sInfo.rect.right = p_oFrame.m_vRight;
    sInfo.rect.bottom = p_oFrame.m_vBottom;
    */
	SendMessage(m_opWindowHandle, TTM_TRACKACTIVATE, static_cast<WPARAM>(true), reinterpret_cast<LPARAM>(&sInfo));
}

#endif
