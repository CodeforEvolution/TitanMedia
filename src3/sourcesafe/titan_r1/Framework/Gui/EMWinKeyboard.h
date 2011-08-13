/*******************************************************
* Creator: Martin Rudefelt
* Portability: native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#ifndef __EM_WIN_KEYBOARD
#define __EM_WIN_KEYBOARD

#include "EMKeyboard.h"

class EMWinKeyboard : public EMKeyboard
{
public:
//	bool BindKey(char p_vKey, uint32 p_vModifiers, uint32 p_vCommandID);
	uint32 GetModifiers();
};

#endif

#endif


