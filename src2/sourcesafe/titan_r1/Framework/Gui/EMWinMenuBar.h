/*******************************************************
* Creator: Martin Rudefelt
* Portability: native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#ifndef __EM_WIN_MENU_BAR
#define __EM_WIN_MENU_BAR

#include "EMMenuBar.h"
#include "EMBeMenuBar.h"

#include <list>
#include <string>
#include <windows.h>

class EMCommand;
class EMListener;
class EMMenuItem;
class EMWinWindow;

class EMWinMenuBar : public EMMenuBar
{
public:
	EMWinMenuBar();
	~EMWinMenuBar();
	bool AddItem(EMMenu* p_opMenu);
	bool AddItem(EMMenu* p_opMenu, int32 p_vIndex);
	bool AddToComponent(EMGUIComponent* p_opComponent);
	EMRect Frame() const;
	EMMenuItem* GetLastTriggeredMenuItem();
	EMMenu* GetMenu(string* p_opName);
	void* GetNativeView() const;
	void Hide();
//	void InitComponent();
//	bool Notify(uint32 p_vMessage);
//	bool Notify(list<EMListener*>* p_oListeners, uint32 p_vMessage); // Needed to avoid a compiler warning
	bool RemoveItem(EMMenu* p_opMenu);
	static void SetLastTriggeredMenuItemID(uint32 p_vLastTriggeredMenuItemID);
	bool SetMarked(uint32 p_vMenuNameID, uint32 p_vMenuItemNameID, bool p_vMarked);
	bool SetEnabled(uint32 p_vMenuNameID, bool p_vEnabled);
	bool SetEnabled(uint32 p_vMenuNameID, uint32 p_vMenuItemNameID, bool p_vEnabled);
	void SetFrame(EMRect p_oFrame);
	void Show();

private:
	list<EMMenu*> m_oMenus;
	HMENU m_opMenu;
	EMWinWindow* m_opWindow;
	static uint32 m_vLastTriggeredMenuItemID;
};

#endif

#endif

