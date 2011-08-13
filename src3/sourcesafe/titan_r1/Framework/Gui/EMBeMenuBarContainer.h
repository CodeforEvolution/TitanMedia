/*******************************************************
* Creator: Martin Rudefelt
* Portability: native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifdef PLATFORM_BEOS

#ifndef __EM_BE_MENU_BAR_CONTAINER
#define __EM_BE_MENU_BAR_CONTAINER

#include "EMMenuBar.h"
#include "EMBeMenuBar.h"

#include <list>
#include <string>

class BMenu;
class EMBeMenuBar;
class EMCommand;
class EMListener;

class EMBeMenuBarContainer : public EMMenuBar
{
public:
	EMBeMenuBarContainer();
	~EMBeMenuBarContainer();
	bool AddItem(EMMenu* p_opMenu);
	bool AddItem(EMMenu* p_opMenu, int32 p_vIndex);
	EMRect Frame() const;
	EMMenu* GetMenu(string* p_opName);
	void* GetNativeView() const;
	void Hide();
	void InitComponent();
//	bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage);
	bool Notify(uint32 p_vMessage);
	bool Notify(list<EMListener*>* p_oListeners, uint32 p_vMessage); // Needed to avoid a compiler warning
	bool RemoveItem(EMMenu* p_opMenu);
//	void SetApplicationMessageTarget(EMListener* p_opMessageTarget);
	void Show();
	
private:
	list<EMMenu*> m_oMenus;
//	EMListener* m_opApplicationMessageTarget;
	EMBeMenuBar* m_opNativeMenuBar;
};

#endif

#endif

