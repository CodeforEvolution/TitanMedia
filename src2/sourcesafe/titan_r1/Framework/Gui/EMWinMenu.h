/*******************************************************
* Creator: Martin Rudefelt
* Portability: native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#ifndef __EM_WIN_MENU
#define __EM_WIN_MENU

#include "EMMenu.h"

#include <list>
#include <string>
#include <windows.h>

class EMMenuItem;

class EMWinMenu : public EMMenu
{
public:
	EMWinMenu(string* p_opName);
	~EMWinMenu();
	bool AddItem(EMMenu* p_opMenu);
	bool AddItem(EMMenuItem* p_opMenuItem);
	bool AddItem(EMMenu* p_opMenu, int32 p_vIndex);
	bool AddItem(EMMenuItem* p_opMenuItem, int32 p_vIndex);
	bool AddSeparatorItem();
	bool AddToComponent(EMGUIComponent* p_opComponent);
	EMRect Frame() const;
	bool GetEnabled();
	EMMenu* GetMenu(string* p_opName);
	EMMenuItem* GetMenuItem(string* p_opName);
	EMMenuItem* GetMenuItem(uint32 p_vCommand);
	const char* GetName();
	void* GetNativeView() const;
	void Hide();
	bool RemoveItem(EMMenu* p_opMenu);
	bool RemoveItem(EMMenuItem* p_opMenuItem);
	void SetEnabled(bool p_vIsEnabled);
	void SetFrame(EMRect p_oFrame);
	void SetMessageTarget(void* p_upTarget);
	void SetWindowHandle(HWND p_opWindow);
	void Show();

private:
	list<EMMenu*> m_oMenus;
	list<EMMenuItem*> m_oMenuItems;
	string m_oName;
	HMENU m_opMenu;
	HMENU m_opParentMenu;
	HWND m_opWindowHandle;
//	EMWinWindow* m_opWindow;
};

#endif

#endif


