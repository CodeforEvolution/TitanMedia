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

#ifndef __EM_WIN_RADIO_BUTTON
#define __EM_WIN_RADIO_BUTTON

#include "EMGUIGlobals.h"
#include "EMRect.h"
#include "EMRadioButton.h"

#include <windows.h>

class EMWinRadioButton : public EMRadioButton
{
public:
	EMWinRadioButton(const EMRect p_oFrame, const char* p_vpName, uint32 p_vMessage, uint32 p_vResizingMode, uint32 p_vCommand, bool p_vEndGroup);
	~EMWinRadioButton();
	bool AddToComponent(EMGUIComponent* p_opComponent);
	void Check(bool p_vCheck);
	void Enable(bool p_vEnable);
	void ExecuteCommand();
	EMRect Frame() const;
	void* GetNativeView() const;
	static LRESULT CALLBACK GlobalWindowProcedure(HWND p_vWindowHandle, UINT p_vMessage, WPARAM p_vParameterOne, LPARAM p_vParameterTwo);
	uint32 GetResizingMode();
	void Hide();
	bool IsChecked();
	bool IsEnabled();
	bool IsVisible();
	void SetCommand(uint32 p_vCommand);
	void SetFrame(EMRect p_oFrame);
	void SetFocus();
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
