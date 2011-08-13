/*******************************************************
* Creator: Martin Rudefelt
* Portability: native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifdef PLATFORM_BEOS

#ifndef __EM_BE_POP_UP_MENU
#define __EM_BE_POP_UP_MENU

#include "PopUpMenu.h"

class EMBePopUpMenuContainer;

class EMBePopUpMenu : public BPopUpMenu
{
public:
	EMBePopUpMenu(EMBePopUpMenuContainer* p_opContainer);
//	~EMBePopUpMenu();
	void AttachedToWindow();
	void MessageReceived(BMessage* p_opMessage); // Needed to catch native messages

private:
	EMBePopUpMenuContainer* m_opContainer;
};

#endif

#endif

