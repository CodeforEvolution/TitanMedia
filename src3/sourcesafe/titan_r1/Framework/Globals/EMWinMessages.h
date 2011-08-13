/*******************************************************
* Creator: Martin Rudefelt
* Component: none
* Portability: native
*-------------------------------------------------------
*
*******************************************************/

#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#include <windows.h>

#ifndef __EM_WIN_MESSAGES
#define __EM_WIN_MESSAGES

const UINT NATIVE_MESSAGE_SET_SCROLL_INFO = WM_APP + 1;
const UINT NATIVE_MESSAGE_MAIL_SENT = WM_APP + 2;
const UINT NATIVE_MESSAGE_QUIT_APPLICATION = WM_APP + 3;

#endif

#endif