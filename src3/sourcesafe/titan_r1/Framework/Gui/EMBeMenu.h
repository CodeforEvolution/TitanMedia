/*******************************************************
* Creator: Martin Rudefelt
* Portability: native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifdef PLATFORM_BEOS

#ifndef __EM_BE_MENU
#define __EM_BE_MENU

class EMBeMenuContainer;
class EMListener;

#include <Menu.h>

class EMBeMenu : public BMenu
{
public:
	EMBeMenu(EMBeMenuContainer* p_opContainer, const char* p_vpLabel);
	~EMBeMenu();
	void MessageReceived(BMessage* p_opMessage); // Needed to catch native messages

private:
	EMBeMenuContainer* m_opContainer;
};

#endif

#endif

