/*******************************************************
* Creator: Martin Rudefelt
* Portability: native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifdef PLATFORM_BEOS

#ifndef __EM_BE_MENU_CONTAINER
#define __EM_BE_MENU_CONTAINER

#include "EMBeMenu.h"
#include "EMMenu.h"

#include <list>
#include <string>

class EMMenuItem;

class EMBeMenuContainer : public EMMenu
{
public:
	EMBeMenuContainer(string* p_opName);
	~EMBeMenuContainer();
	bool AddItem(EMMenu* p_opMenu);
	bool AddItem(EMMenuItem* p_opMenuItem);
	bool AddItem(EMMenu* p_opMenu, int32 p_vIndex);
	bool AddItem(EMMenuItem* p_opMenuItem, int32 p_vIndex);
	bool AddSeparatorItem();
	EMRect Frame() const;
	EMMenu* GetMenu(string* p_opName);
	EMMenuItem* GetMenuItem(string* p_opName);
	const char* GetName();
	void* GetNativeView() const;
	void Hide();
//	void InitComponent();
//	bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage);
	bool RemoveItem(EMMenu* p_opMenu);
	bool RemoveItem(EMMenuItem* p_opMenuItem);
//	void SetApplicationMessageTarget(EMListener* p_opMessageTarget);
	void Show();

private:
	void SetMessageTarget(void* p_opMessageTarget);	

private:
//	EMListener* m_opApplicationMessageTarget;
	EMBeMenu* m_opNativeMenu;
	list<EMMenu*> m_oMenus;
	list<EMMenuItem*> m_oMenuItems;
};

#endif

#endif


