/*******************************************************
* Creator: Martin Johansson
* Portability: Non-Native
*-------------------------------------------------------
* 
* Class used to hold bitmaps before drawing in GUI.
*
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_BITMAP_BUFFER
#define __EM_BITMAP_BUFFER

#include "EMMediaGlobals.h"
#include "EMMediaDataBuffer.h"
#include "EMBitmap.h"

class EMBitmap;

class EMBitmapBuffer
{
public:
	~EMBitmapBuffer();

	static EMBitmapBuffer* Instance();
	static void Delete();

	EMBitmap* GetBitmap();

	bool IsBitmapNew();

	void PutMediaBuffer(EMMediaDataBuffer* p_opBuffer);
	void PutBitmap(EMBitmap* p_opBitmap);
	void Swap();

protected:
	EMBitmap* CreateBitmap(int32 p_vHeight, int32 p_vWidth);

protected:
	int32 m_vHeight[2];
	int32 m_vWidth[2];

	EMBitmap* m_opBitmaps[2];

	int m_vFrontBuffer;

	bool m_vIsNew;

private:
	EMBitmapBuffer();

private:
	static EMBitmapBuffer* m_opInstance;
};

#endif
