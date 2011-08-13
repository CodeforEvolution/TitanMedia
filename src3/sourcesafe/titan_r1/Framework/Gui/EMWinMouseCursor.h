/*******************************************************
* Creator: Martin Rudefelt
* Portability: native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#ifndef __EM_WIN_MOUSE_CURSOR
#define __EM_WIN_MOUSE_CURSOR

#include "EMMouseCursor.h"

#include <windows.h>

const int EM_MOUSE_CURSOR_SIZE = 32;
const int BITS_PER_BYTE = 8;

class EMWinMouseCursor : public EMMouseCursor
{
public:
	EMWinMouseCursor(EMMouseCursorType p_eType);
	~EMWinMouseCursor();
	HCURSOR GetNativeCursor();

private:
	HCURSOR m_opCursor;
	unsigned char m_vANDArray[EM_MOUSE_CURSOR_SIZE * EM_MOUSE_CURSOR_SIZE / BITS_PER_BYTE];
	bool m_vDoNotDestroy;
	unsigned char m_vXORArray[EM_MOUSE_CURSOR_SIZE * EM_MOUSE_CURSOR_SIZE / BITS_PER_BYTE];
};

#endif

#endif

