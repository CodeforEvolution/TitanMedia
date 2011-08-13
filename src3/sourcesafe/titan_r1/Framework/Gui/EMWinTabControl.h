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

#ifndef __EM_WIN_TAB_CONTROL
#define __EM_WIN_TAB_CONTROL

#include "EMGUIGlobals.h"
#include "EMRect.h"
#include "EMTabControl.h"

#include <list>
#include <vector>
#include <windows.h>
#include <commctrl.h>

class EMWinWindow;

class EMWinTabControl : public EMTabControl
{
public:
	EMWinTabControl(const EMRect p_oFrame, const char* p_vpName, bool p_vMultiLined, uint32 p_vMessage, uint32 p_vResizingMode, uint32 p_vCommand);
	~EMWinTabControl();
	void AddChild(EMGUIComponent* p_opView);
	bool AddTab(char* p_vTabName, EMView* p_opTabView);
	bool AddToComponent(EMGUIComponent* p_opComponent);
	void Enable(bool p_vEnable);
	void ExecuteCommand();
	EMRect Frame() const;
	int GetTab();
	void* GetNativeView() const;
	static LRESULT CALLBACK GlobalWindowProcedure(HWND p_vWindowHandle, UINT p_vMessage, WPARAM p_vParameterOne, LPARAM p_vParameterTwo);
	uint32 GetResizingMode();
	void Hide();
	void HideTab();
	bool IsEnabled();
	void SetCommand(uint32 p_vCommand);
	void SetFocus();
	void SetFrame(EMRect p_oFrame);
	void SetLabel(const char* p_vpLabel);
	bool SetTab(int p_vTabIndex);
	void SetWindowWindow(EMWinWindow* p_opWindow);
	void Show();
	void ShowTab();

protected:
	void AdjustViewFrames(int p_vId);

private:
	static LRESULT CALLBACK SubclassedWindowProcedure(HWND p_vWindowHandle, UINT p_vMessage, WPARAM p_vParameterOne, LPARAM p_vParameterTwo);

private:
	list<EMGUIComponent*> m_oChildComponents;
	WNDPROC m_opDefaultWindowProcedure;
	vector<EMView*> m_oViewVector;
	EMRect m_oFrame;
	HWND  m_opWindowHandle;
	EMWinWindow* m_opWindowWindow; // The actual window that lies on the desktop
	uint32 m_vCommand;
	uint32 m_vMessage;
	uint32 m_vResizingMode;
	DWORD m_vStyle;
	int m_vId;
};

#endif

#endif
