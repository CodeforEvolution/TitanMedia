/*******************************************************
* Creator: Martin Rudefelt
* Portability: native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#ifndef __EM_WIN_TEXT_CONTROL
#define __EM_WIN_TEXT_CONTROL

#include "EMGUIGlobals.h"
#include "EMRect.h"
#include "EMTextControl.h"

#include <string>
#include <windows.h>

class EMWinView;

class EMWinTextControl : public EMTextControl
{
public:
	EMWinTextControl(EMRect p_oFrame, const char* p_vpText, uint32 p_vMessage, uint32 p_vResizingMode, bool p_vAutoClose, const EMAlignment p_eAlignment, bool p_vOnlyNumbers, bool p_vPasswordChar);
	~EMWinTextControl();
	bool AddToComponent(EMGUIComponent* p_opComponent);
	void Enable(bool p_vEnable);
	EMRect Frame() const;
	void* GetNativeView() const;
	void GetSelection(int32 &p_vStart, int32 &p_vEnd);
	static LRESULT CALLBACK GlobalWindowProcedure(HWND p_vWindowHandle, UINT p_vMessage, WPARAM p_vParameterOne, LPARAM p_vParameterTwo);
	uint32 GetResizingMode();
	void Hide();
	void InitComponent();
	bool IsEnabled();
	bool IsVisible();
//	void Invalidate();
	void SelectAll();
	void SetFocus();
	void SetFrame(EMRect p_oFrame);
	void SetSelection(int32 p_vStart, int32 p_vEnd); // If p_vStart == 0 and p_vEnd == -1 all text will be selected
	void SetText(const char* p_vpText);
	void Show();
	void SetMaxBytes(int32 p_vMaxBytes);
	const char* Text();

private:
	static LRESULT CALLBACK SubclassedWindowProcedure(HWND p_vWindowHandle, UINT p_vMessage, WPARAM p_vParameterOne, LPARAM p_vParameterTwo);

private:
	EMRect m_oFrame;
	WNDPROC m_opDefaultWindowProcedure;
	HWND m_opWindowHandle;
	string m_oText;
	bool m_vAutoClose;
	uint32 m_vMessage;
	uint32 m_vResizingMode;
	EMWinView* m_opParent;
};

#endif

#endif
