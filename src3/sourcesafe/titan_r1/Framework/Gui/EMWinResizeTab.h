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

#ifndef __EM_WIN_RESIZE_TAB
#define __EM_WIN_RESIZE_TAB

#include "EMRect.h"
#include "EMResizeTab.h"

#include <windows.h>

class EMView;

class EMWinResizeTab : public EMResizeTab
{
public:
	EMWinResizeTab(void* v_opParentNativeView, EMRect p_oParentRect);
	~EMWinResizeTab();
	EMView* GetView();

private:
	EMView* m_opView;
	HWND m_opParentHandle;
};

#endif

#endif