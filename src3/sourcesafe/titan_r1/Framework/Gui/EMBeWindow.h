/*******************************************************
* Creator: Martin Rudefelt
* Portability: native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifdef PLATFORM_BEOS

#ifndef __EM_BE_WINDOW
#define __EM_BE_WINDOW

#include <Window.h>

class EMBeWindowContainer;
class EMListener;

class BMessage;

class EMBeWindow : public BWindow
{
public:
	EMBeWindow(EMBeWindowContainer* p_opContainer, const BRect p_oRect, const char* p_vpName, const window_type p_vType, uint32 p_vFlags);
	EMBeWindow(EMBeWindowContainer* p_opContainer, const BRect p_oRect, const char* p_vpName, const window_look p_vLook, const window_feel p_vFeel, uint32 p_vFlags);
	~EMBeWindow();
	void MessageReceived(BMessage* p_opMessage); // Needed to catch native messages
	bool QuitRequested();

private:
	EMBeWindowContainer* m_opContainer;
};

#endif

#endif
