#include "EMWinMouseCursor.h"

#ifdef PLATFORM_WINDOWS

#include "EMApplication.h"
#include "EMBitmap.h"
#include "EMResourceRepository.h"
#include "EMWinWindow.h"
#include "ResourceIDs.h" // FIXME: Application header in the framework

const int COLOR_WHITE = 0xFF;
const int COLOR_BLACK = 0;

const int MOUSE_HOT_SPOT_X_CUT_PASTE = 0;
const int MOUSE_HOT_SPOT_Y_CUT_PASTE = 0;
const int MOUSE_HOT_SPOT_X_DELETE = 16;
const int MOUSE_HOT_SPOT_Y_DELETE = 16;
const int MOUSE_HOT_SPOT_X_DRAW = 0;
const int MOUSE_HOT_SPOT_Y_DRAW = 0;
const int MOUSE_HOT_SPOT_X_MUTE = 0;
const int MOUSE_HOT_SPOT_Y_MUTE = 0;
const int MOUSE_HOT_SPOT_X_RENAME = 0;
const int MOUSE_HOT_SPOT_Y_RENAME = 0;
const int MOUSE_HOT_SPOT_X_RESIZE_V_BAR = 16;
const int MOUSE_HOT_SPOT_Y_RESIZE_V_BAR = 16;
const int MOUSE_HOT_SPOT_X_RESIZE_H_BAR = 16;
const int MOUSE_HOT_SPOT_Y_RESIZE_H_BAR = 16;
const int MOUSE_HOT_SPOT_X_RESIZE_CLIP = 16;
const int MOUSE_HOT_SPOT_Y_RESIZE_CLIP = 11;
const int MOUSE_HOT_SPOT_X_RESIZE_CLIP_LEFT = 16;
const int MOUSE_HOT_SPOT_Y_RESIZE_CLIP_LEFT = 11;
const int MOUSE_HOT_SPOT_X_RESIZE_CLIP_RIGHT = 16;
const int MOUSE_HOT_SPOT_Y_RESIZE_CLIP_RIGHT = 11;
const int MOUSE_HOT_SPOT_X_DROP_TARGET = 0;
const int MOUSE_HOT_SPOT_Y_DROP_TARGET = 0;
const int MOUSE_HOT_SPOT_X_H_ARROWS = 16;
const int MOUSE_HOT_SPOT_Y_H_ARROWS = 16;
const int MOUSE_HOT_SPOT_X_V_ARROWS = 16;
const int MOUSE_HOT_SPOT_Y_V_ARROWS = 16;

EMWinMouseCursor::EMWinMouseCursor(EMMouseCursorType p_eType) :
EMMouseCursor(p_eType)
{
	m_opCursor = NULL;
	EMBitmap* opBitmap;
	int vHotSpotX(0);
	int vHotSpotY(0);

	switch(p_eType)
	{
	case EM_MOUSE_CURSOR_UNKNOWN:
		EMDebugger("Cannot set a mouse cursor to EM_MOUSE_CURSOR_UNKNOWN");
		break;
	case EM_MOUSE_CURSOR_ARROW:
		m_opCursor = LoadCursor(NULL, IDC_ARROW);
		break;
	case EM_MOUSE_CURSOR_CUT_PASTE:
		opBitmap = EMResourceRepository::GetBitmap(RES_MOUSE_CUT_PASTE);
		vHotSpotX = MOUSE_HOT_SPOT_X_CUT_PASTE;
		vHotSpotY = MOUSE_HOT_SPOT_Y_CUT_PASTE;
		break;
	case EM_MOUSE_CURSOR_DELETE:
		opBitmap = EMResourceRepository::GetBitmap(RES_MOUSE_DELETE);
		vHotSpotX = MOUSE_HOT_SPOT_X_DELETE;
		vHotSpotY = MOUSE_HOT_SPOT_Y_DELETE;
		break;
	case EM_MOUSE_CURSOR_DRAW:
		opBitmap = EMResourceRepository::GetBitmap(RES_MOUSE_DRAW);
		vHotSpotX = MOUSE_HOT_SPOT_X_DRAW;
		vHotSpotY = MOUSE_HOT_SPOT_Y_DRAW;
		break;
	case EM_MOUSE_CURSOR_MUTE:
		opBitmap = EMResourceRepository::GetBitmap(RES_MOUSE_MUTE);
		vHotSpotX = MOUSE_HOT_SPOT_X_MUTE;
		vHotSpotY = MOUSE_HOT_SPOT_Y_MUTE;
		break;
	case EM_MOUSE_CURSOR_RENAME:
		opBitmap = EMResourceRepository::GetBitmap(RES_MOUSE_RENAME);
		vHotSpotX = MOUSE_HOT_SPOT_X_RENAME;
		vHotSpotY = MOUSE_HOT_SPOT_Y_RENAME;
		break;
	case EM_MOUSE_CURSOR_RESIZE_V_BAR:
		opBitmap = EMResourceRepository::GetBitmap(RES_MOUSE_RESIZE_V_BAR);
		vHotSpotX = MOUSE_HOT_SPOT_X_RESIZE_V_BAR;
		vHotSpotY = MOUSE_HOT_SPOT_Y_RESIZE_V_BAR;
		break;
	case EM_MOUSE_CURSOR_RESIZE_H_BAR:
		opBitmap = EMResourceRepository::GetBitmap(RES_MOUSE_RESIZE_H_BAR);
		vHotSpotX = MOUSE_HOT_SPOT_X_RESIZE_H_BAR;
		vHotSpotY = MOUSE_HOT_SPOT_Y_RESIZE_H_BAR;
		break;
	case EM_MOUSE_CURSOR_RESIZE_CLIP:
		opBitmap = EMResourceRepository::GetBitmap(RES_MOUSE_RESIZE_CLIP);
		vHotSpotX = MOUSE_HOT_SPOT_X_RESIZE_CLIP;
		vHotSpotY = MOUSE_HOT_SPOT_Y_RESIZE_CLIP;
		break;
	case EM_MOUSE_CURSOR_RESIZE_CLIP_LEFT:
		opBitmap = EMResourceRepository::GetBitmap(RES_MOUSE_RESIZE_CLIP_LEFT);
		vHotSpotX = MOUSE_HOT_SPOT_X_RESIZE_CLIP_LEFT;
		vHotSpotY = MOUSE_HOT_SPOT_Y_RESIZE_CLIP_LEFT;
		break;
	case EM_MOUSE_CURSOR_RESIZE_CLIP_RIGHT:
		opBitmap = EMResourceRepository::GetBitmap(RES_MOUSE_RESIZE_CLIP_RIGHT);
		vHotSpotX = MOUSE_HOT_SPOT_X_RESIZE_CLIP_RIGHT;
		vHotSpotY = MOUSE_HOT_SPOT_Y_RESIZE_CLIP_RIGHT;
		break;
	case EM_MOUSE_CURSOR_DROP_TARGET:
		opBitmap = EMResourceRepository::GetBitmap(RES_MOUSE_DROP_TARGET);
		vHotSpotX = MOUSE_HOT_SPOT_X_DROP_TARGET;
		vHotSpotY = MOUSE_HOT_SPOT_Y_DROP_TARGET;
		break;
	case EM_MOUSE_CURSOR_DROP_NOT_TARGET:
		m_opCursor = LoadCursor(NULL, IDC_NO);
		break;
	case EM_MOUSE_CURSOR_H_ARROWS:
		m_opCursor = LoadCursor(NULL, IDC_SIZEWE);
		break;
	case EM_MOUSE_CURSOR_V_ARROWS:
		m_opCursor = LoadCursor(NULL, IDC_SIZENS);
		break;
	}

	if(m_opCursor != NULL)
		m_vDoNotDestroy = true;
	else
	{
		m_vDoNotDestroy = false;

		memset(m_vANDArray, 0, EM_MOUSE_CURSOR_SIZE * EM_MOUSE_CURSOR_SIZE / BITS_PER_BYTE);
		memset(m_vXORArray, 0, EM_MOUSE_CURSOR_SIZE * EM_MOUSE_CURSOR_SIZE / BITS_PER_BYTE);

		unsigned char* vpBits = new unsigned char[opBitmap -> GetSize()];
		GetBitmapBits(static_cast<HBITMAP>(opBitmap -> GetNativeBitmap()), opBitmap -> GetSize(), vpBits);

		int vCounterIncrease = 3; // FIXME: find out if 24 or 32 bit bitmap
		for(int vIndex = 0; vIndex < EM_MOUSE_CURSOR_SIZE * EM_MOUSE_CURSOR_SIZE * vCounterIncrease; vIndex += vCounterIncrease)
		{
			if(!(vpBits[vIndex] == COLOR_WHITE && vpBits[vIndex + 1] == COLOR_WHITE && vpBits[vIndex + 2] == COLOR_WHITE) && !(vpBits[vIndex] == COLOR_BLACK && vpBits[vIndex + 1] == COLOR_BLACK && vpBits[vIndex + 2] == COLOR_BLACK))
				m_vANDArray[static_cast<int>(vIndex / (BITS_PER_BYTE * vCounterIncrease))] |= (0x80 >> ((vIndex / vCounterIncrease) % 8));
			else if(vpBits[vIndex] == COLOR_WHITE && vpBits[vIndex + 1] == COLOR_WHITE && vpBits[vIndex + 2] == COLOR_WHITE)
				m_vXORArray[static_cast<int>(vIndex / (BITS_PER_BYTE * vCounterIncrease))] |= (0x80 >> ((vIndex / vCounterIncrease) % 8));
		}
		delete [] vpBits;
		m_opCursor = CreateCursor(EMApplication::GetApplicationInstance(), vHotSpotX, vHotSpotY, EM_MOUSE_CURSOR_SIZE, EM_MOUSE_CURSOR_SIZE, m_vANDArray, m_vXORArray);
	}
}

EMWinMouseCursor::~EMWinMouseCursor()
{
	if(!m_vDoNotDestroy)
		DestroyCursor(m_opCursor);
}

HCURSOR EMWinMouseCursor::GetNativeCursor()
{
	return m_opCursor;
}

#endif