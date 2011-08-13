#include "EMWinBitmap.h"

#ifdef PLATFORM_WINDOWS

/*
EMWinBitmap::EMWinBitmap(EMBitmap* p_opBitmap) :
//m_oBounds(p_opBitmap -> Bounds()),
m_uppBitmapBits(NULL)
{
	m_sNativeBitmap.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	m_sNativeBitmap.bmiHeader.biWidth = (p_opBitmap -> Bounds()).GetWidth();
	m_sNativeBitmap.bmiHeader.biHeight = (p_opBitmap -> Bounds()).GetHeight();
	m_sNativeBitmap.bmiHeader.biPlanes = 1;
	m_sNativeBitmap.bmiHeader.biBitCount = (dynamic_cast<EMWinBitmap*>(p_opBitmap)) -> GetBitsPerPixel();
	m_sNativeBitmap.bmiHeader.biCompression = BI_RGB;
	CreateDIBSection(NULL, &m_sBitmapInfo, DIB_RGB_COLORS, m_uppBitmapBits, NULL, NULL);

//	m_opNativeBitmap = CreateBitmap(m_oBounds.GetWidth(), m_oBounds.GetHeight(), 1, (dynamic_cast<EMWinBitmap*>(p_opBitmap)) -> GetBitsPerPixel(), NULL);
//	SetBitmapBits(reinterpret_cast<HBITMAP>(m_opNativeBitmap), p_opBitmap -> GetSize(), p_opBitmap -> GetBits());

}
*/
EMWinBitmap::EMWinBitmap(void* p_opNativeBitmap) :
m_opNativeBitmap(static_cast<HBITMAP>(p_opNativeBitmap)),
//m_sNativeBitmap(static_cast<BITMAPINFO>(p_opNativeBitmap)),
m_upBitmapBits(NULL)
{
//	BITMAPINFO* spInfo = static_cast<BITMAPINFO*>(p_opNativeBitmap);
//	m_sNativeBitmap = *spInfo;
//eo << "native bitmap stuff: height: " << m_sNativeBitmap.bmiHeader.biHeight << ", width: " << m_sNativeBitmap.bmiHeader.biWidth << ", size: " << m_sNativeBitmap.bmiHeader.biSize << ef;

	DIBSECTION sDibsection;
	if(GetObject((HBITMAP) m_opNativeBitmap, sizeof(DIBSECTION), &sDibsection) == 0)
		eo << "GetObject returned 0" << ef;
	m_oBounds = EMRect(0, 0, sDibsection.dsBm.bmWidth - 1, sDibsection.dsBm.bmHeight - 1);
//m_oBounds.Print();
}

EMWinBitmap::~EMWinBitmap()
{
	DeleteObject(m_opNativeBitmap);
}

EMRect EMWinBitmap::Bounds() const
{
	return m_oBounds;
//	return EMRect(0, 0, m_sNativeBitmap.bmiHeader.biWidth - 1, m_sNativeBitmap.bmiHeader.biHeight - 1);
}

EMWinBitmap* EMWinBitmap::CreateBitmap(void* p_opNativeBitmap)
{
	return EM_new EMWinBitmap(p_opNativeBitmap);
}

void* EMWinBitmap::GetBits()
{
//	BITMAP sBitmapInfo;
//	GetObject(m_opNativeBitmap, sizeof(BITMAP), &sBitmapInfo);
	if(m_upBitmapBits == NULL) // If the pointer to the bit hasn't been set
//		EMDebugger("SetBits() hasn't been called, so no pointer to the bits are available");
	{
		DIBSECTION sDibsection;
		GetObject((HBITMAP) m_opNativeBitmap, sizeof(DIBSECTION), &sDibsection);
		m_upBitmapBits = sDibsection.dsBm.bmBits;
	}
//	return sBitmapInfo.bmBits;
	return m_upBitmapBits;
}

int EMWinBitmap::GetBitsPerPixel() const
{
	BITMAP sBitmapInfo;
	GetObject(m_opNativeBitmap, sizeof(BITMAP), &sBitmapInfo);
eo << "Bitmap bits:";
for(int vIndex = 0; vIndex < 20; vIndex++)
eo << static_cast<int>(static_cast<byte*>(sBitmapInfo.bmBits)[vIndex]) << ", ";
eo << ef;
	return sBitmapInfo.bmBitsPixel;
//	return m_sNativeBitmap.bmiHeader.biBitCount;
}

EMColorSpace EMWinBitmap::GetColorSpace() const
{
//	BITMAP sBitmapInfo;
//	GetObject(m_opNativeBitmap, sizeof(BITMAP), &sBitmapInfo);
//	return static_cast<int>(sBitmapInfo.bmBitsPixel) / 8;
	return EM_COLORSPACE_UNKNOWN;
}

void* EMWinBitmap::GetNativeBitmap() const
{
	return m_opNativeBitmap;
//	return (void*) &m_sNativeBitmap;
//	EMDebugger("Not supported yet");
//	return NULL;
}

int32 EMWinBitmap::GetSize() const
{
	BITMAP sBitmapInfo;
	GetObject(m_opNativeBitmap, sizeof(BITMAP), &sBitmapInfo);
	return sBitmapInfo.bmHeight * sBitmapInfo.bmWidth * sBitmapInfo.bmBitsPixel / 8;
//	return m_sNativeBitmap.bmiHeader.biBitCount / 8 * m_sNativeBitmap.bmiHeader.biHeight * m_sNativeBitmap.bmiHeader.biWidth + m_sNativeBitmap.bmiHeader.biSize;
}
/*
void EMWinBitmap::SetBits(void* p_upBitmapBits)
{
	m_upBitmapBits = p_upBitmapBits;
}
*/
#endif

