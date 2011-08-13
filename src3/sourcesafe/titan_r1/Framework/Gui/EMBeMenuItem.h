/*******************************************************
* Creator: Martin Rudefelt
* Portability: native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifdef PLATFORM_BEOS

#ifndef __EM_BE_MENU_ITEM
#define __EM_BE_MENU_ITEM

#include <MenuItem.h>

class EMBeMenuItem : public BMenuItem
{
public:
	EMBeMenuItem(const char* p_vpLabel);
	~EMBeMenuItem();
//	status_t Invoke(BMessage* p_opMessage = NULL);
	void SetLabel(char const* p_vpLabel);
//	void ExecuteCommand();
//	void MessageReceived(BMessage* p_opMessage); // Needed to catch native messages
/*
private:
	uint32 m_vCommand;
*/
};

#endif

#endif
