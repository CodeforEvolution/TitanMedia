/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_BITMAP
#define __EM_BITMAP

#include "EMGUIGlobals.h"
#include "EMRect.h"

class __declspec(dllexport) EMBitmap
{
public:
	virtual ~EMBitmap();
	virtual EMRect Bounds() const = 0;
	virtual void* GetBits() = 0;
	virtual EMColorSpace GetColorSpace() const = 0;
	virtual	void* GetNativeBitmap() const = 0;
	virtual int32 GetSize() const = 0;

protected:
	EMBitmap();
};

#endif
