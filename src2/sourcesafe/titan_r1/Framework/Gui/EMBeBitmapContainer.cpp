#include "EMBeBitmapContainer.h"

#ifdef PLATFORM_BEOS

#include <Bitmap.h>
#include <Rect.h>

EMBeBitmapContainer::EMBeBitmapContainer(const EMBitmap* p_opBitmap) :
m_opNativeBitmap(static_cast<BBitmap*>(p_opBitmap -> GetNativeBitmap()))
{
}

EMBeBitmapContainer::EMBeBitmapContainer(const void* p_opNativeBitmap) :
m_opNativeBitmap(static_cast<BBitmap*>(p_opNativeBitmap))
{
}

EMBeBitmapContainer::~EMBeBitmapContainer()
{
	delete m_opNativeBitmap;
}

EMRect EMBeBitmapContainer::Bounds() const
{
	BRect oRect = m_opNativeBitmap -> Bounds();
	return EMRect(oRect.left, oRect.top, oRect.right, oRect.bottom);
}

void* EMBeBitmapContainer::GetBits() const
{
	return m_opNativeBitmap -> Bits();
}

// This method has no support for big-endian colorspaces (yet)
EMColorSpace EMBeBitmapContainer::GetColorSpace() const
{
	color_space eSpace = m_opNativeBitmap -> ColorSpace();
	switch(eSpace)
	{
	case B_GRAY1:
		return EM_LE_GRAY1;
	case B_GRAY8:
		return EM_LE_GRAY8;
	case B_CMAP8:
		return EM_LE_CMAP8;
	case B_RGB15:
		return EM_LE_RGB15;
	case B_RGBA15:
		return EM_LE_RGBA15;
	case B_RGB16:
		return EM_LE_16;
	case B_RGB32:
		return EM_LE_32;
	default:
		return EM_COLORSPACE_UNKNOWN;
	}
}

void* EMBeBitmapContainer::GetNativeBitmap() const
{
	return m_opNativeBitmap;
}

int32 EMBeBitmapContainer::GetSize() const
{
	return m_opNativeBitmap -> BitsLength();
}

#endif
