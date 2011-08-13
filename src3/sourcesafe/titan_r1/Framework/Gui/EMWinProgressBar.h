/*******************************************************
* This code is the copyrighted property of Elegant Media
*-------------------------------------------------------
* Creator: Johan Dunberg
* Component: Framework
* Portability: native
*-------------------------------------------------------
* 
*******************************************************/
#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#ifndef __EM_WIN_PROGRESS_BAR
#define __EM_WIN_PROGRESS_BAR

#include "EMProgressBar.h"

#include "EMRect.h"
#include <windows.h>
#include <commctrl.h>

class EMWinView;

class EMWinProgressBar : public EMProgressBar
{
public:
	EMWinProgressBar(EMRect p_oFrame, uint32 p_vMessage);
	~EMWinProgressBar();
	bool AddToComponent(EMGUIComponent* p_opComponent);
	bool CanGetTab();
	EMRect Frame() const;
	void* GetNativeView() const;
	uint16 GetPosition();
	void Hide();
	bool IsFinnished();
//	static LRESULT CALLBACK GlobalWindowProcedure(HWND p_vWindowHandle, UINT p_vMessage, WPARAM p_vParameterOne, LPARAM p_vParameterTwo);
	void SetEndValue(uint16 p_vEnd);
	void SetFrame(EMRect p_oFrame);
	void SetPosition(uint16 p_vPosition);
	void Show();

private:
	EMRect m_oFrame;
	HWND m_opWindowHandle;
	uint16 m_vCurrentValue;
	uint16 m_vEndValue;
	uint32 m_vMessage;
	EMWinView* m_opParent;
};

#endif

#endif