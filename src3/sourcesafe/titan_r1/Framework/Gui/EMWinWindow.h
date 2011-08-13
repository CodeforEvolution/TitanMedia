/*******************************************************
* Creator: Martin Rudefelt
* Portability: native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#ifndef __EM_WIN_WINDOW
#define __EM_WIN_WINDOW

#include "EMGUIGlobals.h"
#include "EMRect.h"
#include "EMWindow.h"

#include <windows.h>

//#include "EMDebugOutput.h"
#include "EMRect.h"

const int EM_SMALL_ICON_SIZE = 16;

class EMListener;
class EMWinPopUpMenu;
class EMSemaphore;
class EMView;

class EMWinWindow : public EMWindow
{
public:
	EMWinWindow(const EMRect p_oRect, const char* p_vpName, const EMWindowType p_oType);
	virtual ~EMWinWindow();
	void AddChild(EMGUIComponent* p_opView);
	EMRect Bounds() const;
	static void CleanUp();
	void ConvertFromWorkAreaToWindow(EMRect &p_oRect);
	EMRect Frame() const;
	static HWND GetDummyWindowHandle();
	static HWND GetMainWindowHandle();
	void* GetNativeView() const;
	EMWindowType GetType();
	HWND GetWindowHandle();
	void Hide();
	bool IsEnabled();
	bool IsHidden();
	bool IsLocked();
	bool IsLockedByThread();
	bool IsMaximized();
	bool Lock();
	void MoveTo(int32 p_vX, int32 p_vY);
	void SetBounds(EMRect p_oBounds);
	void SetCurrentFocus();
	void SetFrame(EMRect p_oFrame);
	void SetFrame(EMRect p_oFrame, bool p_vAdjustRect);
	void SetMaximized(bool p_vMaximized);
//	static void SetPopUpMenu(EMWinPopUpMenu* p_opMenu);
	void SetSizeLimits(int32 p_vMinimumWidth, int32 p_vMaximumWidth, int32 p_vMinimumHeight, int32 p_vMaximumHeight);
	void SetTitle(const char* p_vpTitle);
	void Show();
	static WPARAM StartMessageLoop();
	void Unlock();

private:
	void AssertLocked() const;
//void BeginPaint();
//	void EndPaint();
	static LRESULT CALLBACK GlobalWindowProcedure(HWND p_vWindowHandle, UINT p_vMessage, WPARAM p_vParameterOne, LPARAM p_vParameterTwo);
	WPARAM MessageLoop(void);
	static uint32 TranslateKey(uint32 p_vKeyCode);

private:
	EMWindowType m_eWindowType;
	EMRect m_oBounds;
	list<EMGUIComponent*> m_oChildComponents;
	EMRect m_oFrame;
	static HWND m_opDummyWindowHandle;
	static unsigned char m_opIconMaskBits[EM_SMALL_ICON_SIZE * EM_SMALL_ICON_SIZE / 8];
	static EMWinWindow* m_opMainWindow;
	string m_oName;
//	static EMWinPopUpMenu* m_opPopUpMenu;
	EMSemaphore* m_opSemaphore;
	HWND  m_opWindowHandle;
	static HICON m_opWindowIcon;
	PAINTSTRUCT m_sPaintStruct;
	//static bool m_vIconMaskIsSet;
	int16 m_vLockCount;
	int32 m_vLockingThreadID;
	int m_vMaximumHeight;
	int m_vMaximumWidth;
	int m_vMinimumHeight;
	int m_vMinimumWidth;
	HDC m_vPaintHandle;
	bool m_vReSize;
	uint32 m_vStyle;
	int32 m_vStyleEX;
	static bool m_vWindowIsDeleted;

	bool m_vHasFocus;
	bool m_vIsEnabled;
};

#endif

#endif
