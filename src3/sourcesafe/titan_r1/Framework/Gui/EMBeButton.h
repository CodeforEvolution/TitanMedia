/*******************************************************
* Creator: Martin Rudefelt
* Portability: native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifdef PLATFORM_BEOS

#ifndef __EM_BE_BUTTON
#define __EM_BE_BUTTON

#include <Button.h>

class EMBeButtonContainer;
class EMListener;

class BHandler;
class BLooper;
class BMessage;

class EMBeButton : public BButton
{
public:
	EMBeButton(EMBeButtonContainer* p_opContainer, const BRect p_oFrame, const char* p_vpLabel, BMessage* p_opMessage, uint32 p_vResizingMode);
	void MessageReceived(BMessage* p_opMessage); // Needed to catch native messages
//	void SetListeners(EMListener* p_opListener);

private:
	EMBeButtonContainer* m_opContainer;
};

#endif

#endif
