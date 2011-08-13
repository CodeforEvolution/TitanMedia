/*******************************************************
* This code is the copyrighted property of Elegant Media
*-------------------------------------------------------
* Creator: Johan Dunberg
* Component: Framework, GUI
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_WIN_POP_UP_MENU
#define __EM_WIN_POP_UP_MENU

#include "EMPopUpMenu.h"

//#include <winuser.h>
#include "EMGUIComponent.h"
#include <windows.h>

class EMWinPopUpMenu : public EMPopUpMenu
{
public:
	EMWinPopUpMenu();
	~EMWinPopUpMenu();
	bool AddItem(EMMenu* p_opMenu);
	bool AddItem(EMMenuItem* p_opMenuItem);
	bool AddSeparatorItem();
	void* GetNativeView() const;
//	uint32 GetResult();
	EMRect Frame() const {return EMRect();};
	void PopUp(int32 p_vX, int32 p_vY, EMView* p_opView);
	void SetFrame(EMRect p_oFrame) {};
//	void SetResult(uint32 p_vResult); // The user selection
//	void SetWindowHandle(HWND p_vWindowHandle);
	void Show() {};
	void Hide() {};

protected:

private:
	list<EMMenu*> m_oMenus;
	list<EMMenuItem*> m_oMenuItems;
	HMENU m_opPopUpMenu;
	HWND m_opWindowHandle;
	uint32 m_vResult;
};

#endif
