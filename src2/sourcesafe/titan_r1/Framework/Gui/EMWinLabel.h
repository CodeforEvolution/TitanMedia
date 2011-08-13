/*******************************************************
* Creator: Martin Rudefelt
* Portability: native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#ifndef __EM_WIN_LABEL
#define __EM_WIN_LABEL

#include "EMGUIGlobals.h"
#include "EMRect.h"
#include "EMLabel.h"

#include <windows.h>

class EMWinLabel : public EMLabel
{
public:
	EMWinLabel(const EMRect p_oFrame, const char* p_vpName, uint32 p_vResizingMode, const EMAlignment p_eAlignment);
	~EMWinLabel();
	bool AddToComponent(EMGUIComponent* p_opComponent);
	void Enable(bool p_vEnable);
	EMRect Frame() const;
	void* GetNativeView() const;
	float GetStringWidth();
	uint32 GetResizingMode();
	void Hide();
	bool IsEnabled();
	void SetFrame(EMRect p_oFrame);
	void SetLabel(const char* p_vpLabel);
	void Show();

private:
	EMRect m_oFrame;
	HWND  m_opWindowHandle;
	string m_vName;
	uint32 m_vResizingMode;
};

#endif

#endif
