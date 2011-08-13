/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifdef PLATFORM_BEOS

#ifndef __EM_BE_FACTORY
#define __EM_BE_FACTORY

#include "EMFactory.h"
#include "EMGUIGlobals.h"
#include "EMRect.h"

#include <string>

class EMBitmap;
class EMBorder;
class EMButton;
class EMFileWindow;
class EMMenu;
class EMMenuBar;
class EMMenuItem;
class EMPopUpMenu;
class EMScrollbar;
class EMTextControl;
class EMTextView;
class EMView;
class EMWindow;

class EMBeFactory : public EMFactory
{
public:
	EMBitmap* CreateBitmap(const void* p_opNativeBitmap);
	EMBorder* CreateBorder(EMRect p_oFrame, uint32 p_vResizingMode, EMBorderStyle p_vStyle, const char* p_vLabel);
	EMButton* CreateButton(const EMRect p_oFrame, const char* p_vpLabel, uint32 p_vMessage, uint32 p_vResizingMode, uint32 p_vCommandID);
	EMFileWindow* CreateFileWindow(EMFilePanelMode p_eFilePanelMode/*bool p_vIsLoadWindow*/, 
									const char* p_vpWindowTitle,
									bool p_vMultipleSelection, 
									const char* p_vpSaveDefaultText,
									vector<string>* p_vpVisibleExtensions,
									const char* p_vpStartDirectory, uint32 p_vMessage);
	EMMenu* CreateMenu(string* p_opName);
	EMMenuBar* CreateMenuBar();
	EMMenuItem* CreateMenuItem(string* p_opName, uint32 p_vCommand);
	EMPopUpMenu* CreatePopUpMenu();
	EMScrollbar* CreateScrollbar(EMRect p_oFrame, float p_vMinValue, float p_vMaxValue, EMOrientation p_vOrientation);
	EMTextControl* CreateTextControl(EMRect p_oFrame, const char* p_vpText, uint32 p_vResizingMode);
	EMTextView* CreateTextView(EMRect p_oFrame, const char* p_vpName, uint32 p_vResizingMode);
	EMView* CreateView(const EMRect p_oFrame, uint32 p_vResizingMode, bool p_vDoubleBuffered);
	EMWindow* CreateWindow(const EMRect p_oFrame, const char* p_vpName, const EMWindowType p_oType);
};

#endif

#endif
