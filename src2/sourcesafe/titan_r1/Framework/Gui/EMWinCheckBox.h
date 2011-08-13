/*******************************************************
* Creator: Martin Rudefelt
* Portability: native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#ifndef __EM_WIN_CHECK_BOX
#define __EM_WIN_CHECK_BOX

#include "EMGUIGlobals.h"
#include "EMRect.h"
#include "EMCheckBox.h"

#include <windows.h>

class EMWinCheckBox : public EMCheckBox
{
public:
	EMWinCheckBox(const EMRect p_oFrame, const char* p_vpName, uint32 p_vMessage, uint32 p_vResizingMode, uint32 p_vCommand);
	~EMWinCheckBox();
	bool AddToComponent(EMGUIComponent* p_opComponent);
	void Check(bool p_vCheck);
	void ExecuteCommand();
	void Enable(bool p_vEnable);
	EMRect Frame() const;
	void* GetNativeView() const;
	static LRESULT CALLBACK GlobalWindowProcedure(HWND p_vWindowHandle, UINT p_vMessage, WPARAM p_vParameterOne, LPARAM p_vParameterTwo);
	uint32 GetResizingMode();
	void Hide();
	bool IsChecked();
	bool IsEnabled();
	bool IsVisible();
	void SetCommand(uint32 p_vCommand);
	void SetFocus();
	void SetFrame(EMRect p_oFrame);
	void SetLabel(const char* p_vpLabel);
	void Show();

private:
	static LRESULT CALLBACK SubclassedWindowProcedure(HWND p_vWindowHandle, UINT p_vMessage, WPARAM p_vParameterOne, LPARAM p_vParameterTwo);

private:
	EMRect m_oFrame;
	WNDPROC m_opDefaultWindowProcedure;
	HWND  m_opWindowHandle;
	uint32 m_vCommand;
	uint32 m_vMessage;
	uint32 m_vResizingMode;
};

#endif

#endif
