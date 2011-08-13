/*******************************************************
* Creator: Martin Rudefelt
* Portability: native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#ifndef __EM_WIN_GROUP_BOX
#define __EM_WIN_GROUP_BOX

#include "EMGUIGlobals.h"
#include "EMRect.h"
#include "EMGroupBox.h"

#include <list>
#include <windows.h>

class EMWinWindow;

class EMWinGroupBox : public EMGroupBox
{
public:
	EMWinGroupBox(const EMRect p_oFrame, const char* p_vpName, uint32 p_vResizingMode);
	~EMWinGroupBox();
	void AddChild(EMGUIComponent* p_opView);
	bool AddToComponent(EMGUIComponent* p_opComponent);
	void Enable(bool p_vEnable);
	EMRect Frame() const;
	void* GetNativeView() const;
	uint32 GetResizingMode();
	void Hide();
	bool IsEnabled();
	void SetFrame(EMRect p_oFrame);
	void SetLabel(const char* p_vpLabel);
	void SetWindowWindow(EMWinWindow* p_opWindow);
	void Show();

private:
	list<EMGUIComponent*> m_oChildComponents;
	EMRect m_oFrame;
	const char* m_vpName;
	HWND  m_opWindowHandle;
	EMWinWindow* m_opWindowWindow; // The actual window that lies on the desktop
	uint32 m_vResizingMode;
};

#endif

#endif
