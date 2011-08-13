/*******************************************************
* Creator: Martin Rudefelt
* Portability: native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifdef PLATFORM_BEOS

#ifndef __EM_BE_BITMAP_CONTAINER
#define __EM_BE_BITMAP_CONTAINER

#include "EMBitmap.h"
#include "EMGUIGlobals.h"
#include "EMRect.h"

class BBitmap;

class EMBeBitmapContainer : public EMBitmap
{
public:
	EMBeBitmapContainer(const EMBitmap* p_opBitmap);
	EMBeBitmapContainer(const void* p_opNativeBitmap);
	virtual ~EMBeBitmapContainer();
	EMRect Bounds() const;
	void* GetBits() const;
	EMColorSpace GetColorSpace() const;
	void* GetNativeBitmap() const;
	int32 GetSize() const;

private:
	BBitmap* m_opNativeBitmap;
};

#endif

#endif

