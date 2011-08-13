#include "EMBitmapBuffer.h"
#include "EMBitmap.h"
#include "EMWinBitmap.h"

#include "EMMediaEngine.h"
#include "EMSettingsRepository.h"
#include "EMSettingIds.h"

#include "stdio.h"

EMBitmapBuffer* EMBitmapBuffer::m_opInstance = NULL;


EMBitmapBuffer::EMBitmapBuffer()
{
	m_vWidth[0] = *(static_cast<int*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_VIDEO_FORMAT_WIDTH)));
	m_vHeight[0] = *(static_cast<int*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_VIDEO_FORMAT_HEIGHT)));
	m_opBitmaps[0] = CreateBitmap(m_vHeight[0], m_vWidth[0]);

	m_vWidth[1] = *(static_cast<int*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_VIDEO_FORMAT_WIDTH)));
	m_vHeight[1] = *(static_cast<int*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_VIDEO_FORMAT_HEIGHT)));
	m_opBitmaps[1] = CreateBitmap(m_vHeight[1], m_vWidth[1]);

	m_vFrontBuffer = 0;
	m_vIsNew = false;
}
	
EMBitmapBuffer::~EMBitmapBuffer()
{
	if(m_opBitmaps[0] != NULL)
	{
		delete m_opBitmaps[0];
		m_opBitmaps[0] = NULL;
	}


	if(m_opBitmaps[1] != NULL)
	{
		delete m_opBitmaps[1];
		m_opBitmaps[1] = NULL;
	}
}

EMBitmapBuffer* EMBitmapBuffer::Instance()
{
	if(m_opInstance == NULL)
		m_opInstance = EM_new EMBitmapBuffer;

	return m_opInstance;
}

void EMBitmapBuffer::Delete()
{
	if(m_opInstance != NULL)
		delete m_opInstance;

	m_opInstance = NULL;
}


EMBitmap* EMBitmapBuffer::CreateBitmap(int32 p_vHeight, int32 p_vWidth)
{
	BITMAPINFO BitmapInfo;

	BitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	BitmapInfo.bmiHeader.biWidth  = p_vWidth; //
	BitmapInfo.bmiHeader.biHeight = p_vHeight; //
	BitmapInfo.bmiHeader.biPlanes = 1;
	BitmapInfo.bmiHeader.biBitCount = 24;
	BitmapInfo.bmiHeader.biCompression = 0;
	BitmapInfo.bmiHeader.biSizeImage = p_vWidth * p_vHeight * 3;
	BitmapInfo.bmiHeader.biXPelsPerMeter = 2250;
	BitmapInfo.bmiHeader.biYPelsPerMeter = 2000;

	HBITMAP bmp =  CreateDIBSection(0, &BitmapInfo, DIB_RGB_COLORS, NULL, NULL, 0);

	return EM_new EMWinBitmap(bmp);
}

bool EMBitmapBuffer::IsBitmapNew()
{
	return m_vIsNew;
}

EMBitmap* EMBitmapBuffer::GetBitmap()
{
	m_vIsNew = false;
	return m_opBitmaps[1 - m_vFrontBuffer];
}

void EMBitmapBuffer::PutMediaBuffer(EMMediaDataBuffer* p_opBuffer)
{
	if(p_opBuffer -> m_oFormat.m_vHeight != m_vHeight[m_vFrontBuffer] || p_opBuffer -> m_oFormat.m_vWidth != m_vWidth[m_vFrontBuffer])
	{
		delete m_opBitmaps[m_vFrontBuffer];

		m_vHeight[m_vFrontBuffer] = p_opBuffer -> m_oFormat.m_vHeight;
		m_vWidth[m_vFrontBuffer] = p_opBuffer -> m_oFormat.m_vWidth;
		
		m_opBitmaps[m_vFrontBuffer] = CreateBitmap(m_vHeight[m_vFrontBuffer], m_vWidth[m_vFrontBuffer]);
	}

	/*
	unsigned char Header[18];

	memset(&Header, 0, 18); 

	*(WORD*)(Header + 12) = p_opBuffer -> m_oFormat.m_vWidth;
	*(WORD*)(Header + 14) = p_opBuffer -> m_oFormat.m_vHeight;
	*(char*)(Header + 16) = p_opBuffer -> m_oFormat.m_vDepth * 8;
	*(char*)(Header +  2) = 2;

	FILE* fp = fopen("TillMartin.tga","wb");
	fwrite(&Header, 1, sizeof(Header), fp);
	fwrite(p_opBuffer -> Data(), 1, p_opBuffer -> m_vSizeUsed, fp);
	fclose(fp);
	*/


	HBITMAP opBits = static_cast<HBITMAP>(m_opBitmaps[m_vFrontBuffer] -> GetNativeBitmap());
	DIBSECTION oInfo;

	GetObject(opBits, sizeof(DIBSECTION), (void*)&oInfo);

	//TEMP!!!! BUGG!!!! USCH!!!
	if(oInfo.dsBmih.biSizeImage <  p_opBuffer -> m_vSizeUsed)
	{
		memcpy(oInfo.dsBm.bmBits, p_opBuffer -> Data(), oInfo.dsBmih.biSizeImage);
	}
	else
	{
		memcpy(oInfo.dsBm.bmBits, p_opBuffer -> Data(), p_opBuffer -> m_vSizeUsed);
	}
}

void EMBitmapBuffer::PutBitmap(EMBitmap* p_opBitmap)
{
	EMDebugger("EMBitmapBuffer::PutBitmap(EMBitmap*) not implemented yet.. You need? You write ;-)");
}

void EMBitmapBuffer::Swap()
{
	m_vIsNew = true;
	m_vFrontBuffer = 1 - m_vFrontBuffer;
}
