/*******************************************************
* Creator: Martin Rudefelt
* Portability: native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#ifndef __EM_WIN_BITMAP
#define __EM_WIN_BITMAP

#include "EMBitmap.h"
#include "EMGUIGlobals.h"
#include "EMRect.h"

#include <windows.h>

class __declspec(dllexport) EMWinBitmap : public EMBitmap
{
public:
	EMWinBitmap(EMBitmap* p_opBitmap);
	EMWinBitmap(void* p_opNativeBitmap);
	virtual ~EMWinBitmap();
	EMRect Bounds() const;
	static EMWinBitmap* CreateBitmap(void* p_opNativeBitmap);
	void* GetBits();
	int GetBitsPerPixel() const;
	EMColorSpace GetColorSpace() const;
	void* GetNativeBitmap() const;
	int32 GetSize() const;
//	void SetBits(void* p_upBitmapBits);
//	void SetDDBHandle

private:
	EMRect m_oBounds;
	HBITMAP m_opNativeBitmap;
//	BITMAPINFO m_sNativeBitmap;
//	HBITMAP m_opDDBHandle;
	void* m_upBitmapBits;
};

#endif

#endif

