/*******************************************************
* This code is the copyrighted property of Elegant Media
*-------------------------------------------------------
* Creator: Johan Dunberg
* Component: Framework, GUI
* Portability: native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#ifndef __EM_WIN_TRACKBAR_CONTROL
#define __EM_WIN_TRACKBAR_CONTROL

#include "EMGUIGlobals.h"
#include "EMRect.h"
#include "EMTrackbarControl.h"

#include <list>
#include <vector>
#include <windows.h>
#include <commctrl.h>

class EMWinWindow;

class EMWinTrackbarControl : public EMTrackbarControl
{
public:
	EMWinTrackbarControl(const EMRect p_oFrame, const char* p_vpName, uint32 p_vMessage, uint32 p_vResizingMode, uint32 p_vCommand, EMTrackbarTickStyle p_eTickStyle, bool p_vVertical);
	~EMWinTrackbarControl();
	void AddChild(EMGUIComponent* p_opView);
	bool AddToComponent(EMGUIComponent* p_opComponent);
	void Enable(bool p_vEnable);
	void ExecuteCommand();
	EMRect Frame() const;
	void* GetNativeView() const;
	int32 GetPosition();
	static LRESULT CALLBACK GlobalWindowProcedure(HWND p_vWindowHandle, UINT p_vMessage, WPARAM p_vParameterOne, LPARAM p_vParameterTwo);
	uint32 GetResizingMode();
	void Hide();
	bool IsEnabled();
	void SetCommand(uint32 p_vCommand);
	void SetFocus();
	void SetFrame(EMRect p_oFrame);
	void SetLabel(const char* p_vpLabel);
	void SetRange(int32 p_vStart, int32 p_vEnd);
	void SetPosition(int32 p_vPosition);
	void SetTick(int32 p_vPosition);
	void SetWindowWindow(EMWinWindow* p_opWindow);
	void Show();

private:
	static LRESULT CALLBACK SubclassedWindowProcedure(HWND p_vWindowHandle, UINT p_vMessage, WPARAM p_vParameterOne, LPARAM p_vParameterTwo);

private:
	list<EMGUIComponent*> m_oChildComponents;
	WNDPROC m_opDefaultWindowProcedure;
	EMRect m_oFrame;
	HWND  m_opWindowHandle;
	EMWinWindow* m_opWindowWindow; // The actual window that lies on the desktop
	uint32 m_vCommand;
	uint32 m_vMessage;
	uint32 m_vResizingMode;
	DWORD m_vStyle;
};

#endif

#endif