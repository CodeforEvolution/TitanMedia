/*******************************************************
* Creator: Martin Rudefelt
* Portability: native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#ifndef __EM_WIN_SCROLLBAR
#define __EM_WIN_SCROLLBAR

#include "EMGUIGlobals.h"
#include "EMRect.h"
#include "EMScrollbar.h"

#include <list>
#include <windows.h>

class EMSemaphore;
class EMWinWindow;

class EMWinScrollbar : public EMScrollbar
{
public:
	EMWinScrollbar(EMRect p_oFrame, int32 p_vMinValue, int32 p_vMaxValue, EMOrientation p_vOrientation);
	~EMWinScrollbar();
	bool AddToComponent(EMGUIComponent* p_opComponent);
	void Enable(bool p_vEnable);
	virtual EMRect Frame() const;
	virtual void* GetNativeView() const;
	void GetRange(int32& p_vMin, int32& p_vMax);
	uint32 GetResizingMode();
	void GetSteps(int32& p_vSmallStep, int32& p_vBigStep);
	int32 GetValue();
	static LRESULT CALLBACK GlobalWindowProcedure(HWND p_vWindowHandle, UINT p_vMessage, WPARAM p_vParameterOne, LPARAM p_vParameterTwo);
	void Hide();
	bool IsEnabled();
	EMOrientation Orientation();
	void SetFrame(EMRect p_oFrame);
	void SetPageAndRange(uint32 p_vPage, uint32 p_vMin, uint32 p_vMax);
//	void SetProportion(int32 p_vProportion);
//	void SetRange(int32 p_vMin, int32 p_vMax);
	void SetSteps(int32 p_vSmallStep, int32 p_vBigStep);
	void SetValue(int32 p_vValue);
	void SetWindowWindow(EMWinWindow* p_opWindow);
	void Show();
	static LRESULT CALLBACK SubclassProcedure(HWND p_vWindowHandle, UINT p_vMessage, WPARAM p_vParameterOne, LPARAM p_vParameterTwo);

private:

private:
	EMRect m_oFrame;
	static WNDPROC m_opOriginalWindowProcedure;
	EMSemaphore* m_opSemaphore;
	HWND  m_opWindowHandle;
	EMWinWindow* m_opWindowWindow; // The actual window that lies on the desktop
	SCROLLINFO m_sScrollInfo;
	int32 m_vMaxValue;
	int32 m_vMinValue;
	EMOrientation m_vOrientation;
	int32 m_vPage;
	uint16 m_vStepSmall; // The amount moved when clicking on an arrow
	uint16 m_vStepBig; // The amount moved when clicking in the scrollbar outside the scrollbox
	list<SCROLLINFO> m_oPostedScrollInfos;
};

#endif

#endif
