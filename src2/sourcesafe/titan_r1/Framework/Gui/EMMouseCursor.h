/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_MOUSE_CURSOR
#define __EM_MOUSE_CURSOR

enum EMMouseCursorType
{
	EM_MOUSE_CURSOR_UNKNOWN,
	EM_MOUSE_CURSOR_ARROW,
	EM_MOUSE_CURSOR_CUT_PASTE,
	EM_MOUSE_CURSOR_DELETE,
	EM_MOUSE_CURSOR_DRAW,
	EM_MOUSE_CURSOR_MUTE,
	EM_MOUSE_CURSOR_RENAME,
	EM_MOUSE_CURSOR_RESIZE_V_BAR,
	EM_MOUSE_CURSOR_RESIZE_H_BAR,
	EM_MOUSE_CURSOR_RESIZE_CLIP,
	EM_MOUSE_CURSOR_RESIZE_CLIP_LEFT,
	EM_MOUSE_CURSOR_RESIZE_CLIP_RIGHT,
	EM_MOUSE_CURSOR_DROP_TARGET,
	EM_MOUSE_CURSOR_DROP_NOT_TARGET,
	EM_MOUSE_CURSOR_H_ARROWS,
	EM_MOUSE_CURSOR_V_ARROWS
};

class EMMouseCursor
{
public:
	EMMouseCursor(EMMouseCursorType p_eType);
	virtual ~EMMouseCursor();
	EMMouseCursorType GetType();

private:
	EMMouseCursorType m_eType;
};

#endif

