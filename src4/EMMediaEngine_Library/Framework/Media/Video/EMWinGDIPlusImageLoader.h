/*******************************************************
* Creator: Jesper Svensson
* Portability: Non-Native
*-------------------------------------------------------
* 
*******************************************************/


#ifndef __EM_WIN_GDI_PLUS_IMAGE_LOADER
#define __EM_WIN_GDI_PLUS_IMAGE_LOADER

#include <windows.h>
#include <gdiplus.h>
#include "stdio.h"
using namespace Gdiplus;

class EMWinGDIPlusImageLoader
{
public:
	EMWinGDIPlusImageLoader();
	~EMWinGDIPlusImageLoader();
	static EMWinGDIPlusImageLoader* Instance();
	void PrepareImageFile(unsigned short* p_pFileName);
	int GetWidth();
	int GetHeight();
	int GetBytesPerRow();
	void CopyImageIntoBuffer(unsigned char* p_opBuffer);
	void CloseImageFile();

private:
	static EMWinGDIPlusImageLoader* m_opGDIPlusImageLoader;
	BitmapData* m_bitmapData;
	Bitmap*	m_Bitmap;

};

#endif