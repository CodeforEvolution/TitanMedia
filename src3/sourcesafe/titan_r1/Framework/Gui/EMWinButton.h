/*******************************************************
* Creator: Martin Rudefelt
* Portability: native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#ifndef __EM_WIN_BUTTON
#define __EM_WIN_BUTTON

#include "EMGUIGlobals.h"
#include "EMRect.h"
#include "EMButton.h"

#include <windows.h>
/*
enum EMWinButtonStyle
{
	EM_PUSHBUTTON,
	EM_RADIOBUTTON,
	EM_CHECKBOX
};
*/
class EMWinButton : public EMButton
{
public:
	EMWinButton(const EMRect p_oFrame, const char* p_vpName, uint32 p_vMessage, uint32 p_vResizingMode, uint32 p_vCommand, void* p_vpParentNativeView, bool p_vDefault);//, EMWinButtonStyle p_eStyle);
	~EMWinButton();
	bool AddToComponent(EMGUIComponent* p_opComponent);
	void Enable(bool p_vEnable);
	void ExecuteCommand();
	EMRect Frame() const;
	void* GetNativeView() const;
	static LRESULT CALLBACK GlobalWindowProcedure(HWND p_vWindowHandle, UINT p_vMessage, WPARAM p_vParameterOne, LPARAM p_vParameterTwo);
	uint32 GetResizingMode();
	void Hide();
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
	WNDPROC m_opDefaultWindowProcedure;
	EMRect m_oFrame;
	HWND  m_opWindowHandle;
	uint32 m_vCommand;
	uint32 m_vMessage;
	uint32 m_vResizingMode;
};

#endif

#endif
