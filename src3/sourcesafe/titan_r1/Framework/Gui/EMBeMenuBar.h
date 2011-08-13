/*******************************************************
* Creator: Martin Rudefelt
* Portability: native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifdef PLATFORM_BEOS

#ifndef __EM_BE_MENU_BAR
#define __EM_BE_MENU_BAR

class EMBeMenuBarContainer;
class EMListener;

#include <MenuBar.h>

class EMBeMenuBar : public BMenuBar
{
public:
	EMBeMenuBar(EMBeMenuBarContainer* p_opContainer, char* p_vpLabel);
	~EMBeMenuBar();
	void MessageReceived(BMessage* p_opMessage); // Needed to catch native messages

private:
	EMBeMenuBarContainer* m_opContainer;
};

#endif

#endif

