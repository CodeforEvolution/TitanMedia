/*******************************************************
* Creator: Martin Rudefelt
* Portability: native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#ifndef __EM_WIN_TOOL_TIP
#define __EM_WIN_TOOL_TIP

#include "EMGUIGlobals.h"
#include "EMRect.h"
#include "EMToolTip.h"

#include <windows.h>

class EMWinToolTip : public EMToolTip
{
public:
	EMWinToolTip(const EMRect p_oFrame, const char* p_vpLabel, void* p_vpParentNativeView, bool p_vAutomatic);
	~EMWinToolTip();
	bool AddToComponent(EMGUIComponent* p_opComponent);
	void Enable(bool p_vEnable);
	EMRect Frame() const;
	void* GetNativeView() const;
	static LRESULT CALLBACK GlobalWindowProcedure(HWND p_vWindowHandle, UINT p_vMessage, WPARAM p_vParameterOne, LPARAM p_vParameterTwo);
	void Hide();
	bool IsEnabled();
	//bool IsVisible();
	//void SetFocus();
	void SetFrame(EMRect p_oFrame);
	void SetLabel(const char* p_vpLabel);
	void SetPosition(int32 p_vX, int32 p_vY);
	void Show();

private:
	EMRect m_oFrame;
	HWND m_opParentWindowHandle;
	HWND m_opWindowHandle;
	bool m_vAutomatic;
	//static int32 m_vGlobalID;
	//int32 m_vID;
};

#endif

#endif
