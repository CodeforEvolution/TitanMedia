/*******************************************************
* Creator: Martin Rudefelt
* Portability: native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifdef PLATFORM_BEOS

#ifndef __EM_BE_MESSAGE_TRANSLATOR
#define __EM_BE_MESSAGE_TRANSLATOR

class EMBeMessageTranslator
{
public:
	static uint32 TranslateMessage(uint32 p_vMessage);
};

#endif

#endif
