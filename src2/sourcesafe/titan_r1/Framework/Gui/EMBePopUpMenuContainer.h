/*******************************************************
* Creator: Martin Rudefelt
* Portability: native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifdef PLATFORM_BEOS

#ifndef __EM_BE_POP_UP_MENU_CONTAINER
#define __EM_BE_POP_UP_MENU_CONTAINER

#include "EMPopUpMenu.h"

#include "EMBePopUpMenu.h"
#include "EMRect.h"

#include <list>
#include <string>

class EMBePopUpMenuContainer : public EMPopUpMenu
{
public:
	EMBePopUpMenuContainer();
	virtual ~EMBePopUpMenuContainer();
	bool AddItem(EMMenu* p_opMenu);
	bool AddItem(EMMenuItem* p_opMenuItem);
	bool AddSeparatorItem();
	EMRect Frame() const { return EMRect(); };
	void* GetNativeView() const;
	void Hide() {};
	void PopUp(float p_vX, float p_vY);
	void Show() {};

private:
	EMBePopUpMenu* m_opNativeMenu;
	list<EMMenu*> m_oMenus;
	list<EMMenuItem*> m_oMenuItems;
};

#endif

#endif