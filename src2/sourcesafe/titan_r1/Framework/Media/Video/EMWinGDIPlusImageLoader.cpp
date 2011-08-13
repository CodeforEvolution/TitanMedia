
#include "EMWinGDIPlusImageLoader.h"


EMWinGDIPlusImageLoader* EMWinGDIPlusImageLoader::m_opGDIPlusImageLoader = NULL;


ULONG_PTR m_gdiplusToken;

EMWinGDIPlusImageLoader::EMWinGDIPlusImageLoader()
{
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);
	m_Bitmap = NULL;
	m_bitmapData = NULL;
}

EMWinGDIPlusImageLoader::~EMWinGDIPlusImageLoader()
{
	if(m_bitmapData != NULL)
		delete m_bitmapData;

	if(m_bitmapData != NULL)
		delete m_Bitmap;
	GdiplusShutdown(m_gdiplusToken);
}

EMWinGDIPlusImageLoader* EMWinGDIPlusImageLoader::Instance()
{
	if(m_opGDIPlusImageLoader != NULL)
		return m_opGDIPlusImageLoader;

	if(m_opGDIPlusImageLoader == NULL)
		m_opGDIPlusImageLoader = new EMWinGDIPlusImageLoader();

	return m_opGDIPlusImageLoader;
}

void EMWinGDIPlusImageLoader::PrepareImageFile(unsigned short* p_pFileName)
{
	m_Bitmap = Bitmap::FromFile(p_pFileName);

	m_bitmapData = new BitmapData;

	Rect rect(0, 0, m_Bitmap -> GetWidth(), m_Bitmap -> GetHeight());

	// Lock a 5×3 rectangular portion of the bitmap for reading.
	m_Bitmap -> LockBits( &rect, ImageLockModeRead, PixelFormat24bppRGB, m_bitmapData);

}

int EMWinGDIPlusImageLoader::GetWidth()
{
	return m_bitmapData -> Width;
}

int EMWinGDIPlusImageLoader::GetHeight()
{
	return m_bitmapData -> Height;
}

int EMWinGDIPlusImageLoader::GetBytesPerRow()
{
	return m_bitmapData -> Stride;
}

void EMWinGDIPlusImageLoader::CopyImageIntoBuffer(unsigned char* p_opBuffer)
{
	UINT* pPixels = (UINT*)m_bitmapData->Scan0;
	memcpy(p_opBuffer, pPixels, m_bitmapData -> Stride * m_bitmapData -> Height);
}

void EMWinGDIPlusImageLoader::CloseImageFile()
{
	if(m_bitmapData != NULL && m_bitmapData != NULL)
		m_Bitmap -> UnlockBits(m_bitmapData);

	if(m_bitmapData != NULL)
		delete m_bitmapData;
	m_bitmapData = NULL;

	if(m_Bitmap != NULL)
		delete m_Bitmap;
	m_Bitmap = NULL;
}





