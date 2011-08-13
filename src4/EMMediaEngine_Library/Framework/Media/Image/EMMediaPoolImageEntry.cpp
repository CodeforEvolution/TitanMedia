#include "EMMediaPoolImageEntry.h"

#include "EMWinMediaUtility.h"
#include "EMImageFileInputDescriptor.h"
#include "EMDXImageFileInputDescriptor.h"
#include "EMTGAImageFileInputDescriptor.h"
#include "EMWinBitmap.h"

#include "EMProjectDataLoader.h"
#include "EMProjectDataSaver.h"


EMMediaPoolImageEntry::EMMediaPoolImageEntry(string p_oFileName, EMMediaType p_eType)
	:	EMMediaPoolEntry(p_oFileName, p_eType),
		m_oFormat(p_eType),
		m_vOne(1),
		m_opBuffer(NULL)
{
	m_opFormat = &m_oFormat;

	if(p_eType == EM_TYPE_TGA)
		m_opDesc = EM_new EMTGAImageFileInputDescriptor(p_oFileName);
	else
		m_opDesc = EM_new EMDXImageFileInputDescriptor(p_oFileName);
}

EMMediaPoolImageEntry::EMMediaPoolImageEntry(EMProjectDataLoader* p_opLoader, uint32 vUndecidedType)
  : EMMediaPoolEntry(p_opLoader, vUndecidedType),
	m_oFormat(EM_TYPE_ANY_IMAGE),
	m_vOne(1),
	m_opBuffer(NULL)
{
	m_opFormat = &m_oFormat;

	int32 vDummy = p_opLoader ->LoadInt32();

	m_oFormat.m_eType = EM_TYPE_ANY_IMAGE;

	switch(vDummy)
	{
	case 0:
		m_eType = EM_TYPE_BMP;
		m_opDesc = EM_new EMDXImageFileInputDescriptor(m_oFileName);
		break;
	case 1:
		m_eType = EM_TYPE_GIF;
		m_opDesc = EM_new EMDXImageFileInputDescriptor(m_oFileName);
		break;
	case 2:
		m_eType = EM_TYPE_JPG;
		m_opDesc = EM_new EMDXImageFileInputDescriptor(m_oFileName);
		break;
	case 3:
		m_eType = EM_TYPE_TGA;
		m_opDesc = EM_new EMTGAImageFileInputDescriptor(m_oFileName);
		break;
	default:
		EMDebugger("EMMediaPoolImageEntry - unknown image format");
	}
}

bool EMMediaPoolImageEntry::SaveData(EMProjectDataSaver* p_opSaver)
{
	bool vRet = EMMediaPoolEntry::SaveData(p_opSaver);

	switch(m_eType)
	{
	case EM_TYPE_BMP:
		p_opSaver -> SaveInt32(0);
		break;
	case EM_TYPE_GIF:
		p_opSaver -> SaveInt32(1);
		break;
	case EM_TYPE_JPG:
		p_opSaver -> SaveInt32(2);
		break;
	case EM_TYPE_TGA:
		p_opSaver -> SaveInt32(3);
		break;
	default:
		EMDebugger("What the fuck!?!?");
		break;
	}

	return vRet;
}


EMMediaPoolImageEntry::~EMMediaPoolImageEntry()
{
	if(m_opBuffer != NULL)
		delete m_opBuffer;
}

float* EMMediaPoolImageEntry::GetAspectRatio()
{
	return &m_vAspect;
}


EMMediaFormat* EMMediaPoolImageEntry::GetFormat() const
{
	return m_opFormat;
}

int64 EMMediaPoolImageEntry::GetLength() const
{
	// FIXME: Should not return a constant. 
	//        Should return a value that is the same for
	//        both MediaEngine and GUI. A value that is
	//        defined in one place only!
	return 20000000;
}

int64 EMMediaPoolImageEntry::GetSize() const
{
	return m_oFormat.m_vBufferSizeBytes;
}

int64 EMMediaPoolImageEntry::GetFrameRate() const
{
	return 0;
}	

string EMMediaPoolImageEntry::GetFormatString()
{
	SHFILEINFO sFileInfo;
	const char* vpPath = GetFileName().c_str();
	SHGetFileInfo(vpPath, 0, &sFileInfo, sizeof(sFileInfo), SHGFI_TYPENAME);
	return string(sFileInfo.szTypeName);
}

int64 EMMediaPoolImageEntry::BitDepth() const
{
	return m_oFormat.m_vDepth;
}

bool EMMediaPoolImageEntry::InitCheckE()
{
	bool vRet = m_opDesc -> InitCheckE();

	if(vRet)
	{
		m_oFormat = *(m_opDesc -> GetFormat());
		m_vAspect = static_cast<float>(m_oFormat.m_vWidth) / static_cast<float>(m_oFormat.m_vHeight);
	}

	return vRet;
}

int64* EMMediaPoolImageEntry::NumberOfFramesInWFPS(float p_vFramerateCurrentFPS)
{
	return &m_vOne;
}


EMBitmap* EMMediaPoolImageEntry::GetImage(int16 p_vHeight)
{
	BITMAPINFO oBmpInfo;
	DIBSECTION oDibInfo;
	void* pBmpData;

	try
	{
		EMMediaFormat oFormat(m_oFormat);

		oFormat.m_vWidth = (float) oFormat.m_vWidth * (float)p_vHeight / (float)oFormat.m_vHeight;
		oFormat.m_vHeight = p_vHeight;
	
		oFormat.m_vBytesPerRow = oFormat.m_vWidth * oFormat.m_vDepth;
		oFormat.m_vBufferSizeBytes = oFormat.m_vHeight * oFormat.m_vBytesPerRow;


		oBmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		oBmpInfo.bmiHeader.biHeight = oFormat.m_vHeight;
		oBmpInfo.bmiHeader.biWidth = oFormat.m_vWidth;
		oBmpInfo.bmiHeader.biBitCount = 8 * oFormat.m_vDepth;
		oBmpInfo.bmiHeader.biSizeImage = oFormat.m_vBufferSizeBytes;
		oBmpInfo.bmiHeader.biCompression = 0;
		oBmpInfo.bmiHeader.biPlanes = 1;

		HANDLE oBmp = CreateDIBSection(NULL, &oBmpInfo, DIB_RGB_COLORS, &pBmpData, NULL, NULL);

		GetObject(oBmp, sizeof(DIBSECTION), &oDibInfo);

		if (oDibInfo.dsBmih.biSizeImage != 0)
		{
			oFormat.m_vBufferSizeBytes = oDibInfo.dsBmih.biSizeImage;
			oFormat.m_vBytesPerRow = oDibInfo.dsBmih.biSizeImage / oDibInfo.dsBm.bmHeight;
		}
		else
		{
			oFormat.m_vBufferSizeBytes = oDibInfo.dsBm.bmWidthBytes * oDibInfo.dsBm.bmHeight;
			oFormat.m_vBytesPerRow = oDibInfo.dsBm.bmWidthBytes;
		}

		if(m_opBuffer != NULL)
			delete m_opBuffer;

		m_opBuffer = new EMMediaDataBuffer(EM_TYPE_ANY_IMAGE, oFormat.m_vBufferSizeBytes, pBmpData);
		m_opBuffer -> m_oFormat = oFormat;

		m_opDesc -> ReadFrames(m_opBuffer);

		return EM_new EMWinBitmap(oBmp);
	}
	catch(...)
	{
	}
	return NULL;

}


EMImageFileInputDescriptor* EMMediaPoolImageEntry::CloneDescriptor()
{
	return m_opDesc -> Clone();

}

