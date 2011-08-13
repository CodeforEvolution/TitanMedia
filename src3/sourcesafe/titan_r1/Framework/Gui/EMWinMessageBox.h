/*******************************************************
* This code is the copyrighted property of Elegant Media
*-------------------------------------------------------
* Creator: Johan Dunberg
* Component: Framework
* Portability: native
*-------------------------------------------------------
* 
*******************************************************/
#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#ifndef __EM_WIN_MESSAGE_BOX
#define __EM_WIN_MESSAGE_BOX

#include "EMMessageBox.h"

class EMWinWindow;

class EMWinMessageBox : public EMMessageBox
{
public:
	EMWinMessageBox();
	uint32 DisplayDialog(void* p_opNativeView, const char* p_vMessage, const char* p_vCaption, uint32 p_vFlags);
};

#endif

#endif