/*******************************************************
* Creator: Martin Rudefelt
* Component: none
* Portability: native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifdef PLATFORM_BEOS

#ifndef __EM_BE_KEYBOARD
#define __EM_BE_KEYBOARD

#include "EMKeyboard.h"

class EMBeKeyboard : public EMKeyboard
{
public:
	uint32 GetModifiers();
};

#endif

#endif


