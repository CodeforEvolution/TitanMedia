#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#include "EMWinVideoOutputPin.h"
#include "EMWinMediaOutputPin.h"

#include "EMMediaDataBuffer.h"

#include "EMWinMediaOutputFilterInterface.h"
#include "EMWinMediaOutputFilter.h"

#include "EMMediaTimer.h"
#include "EMMediaUtility.h"
#include "EMSettingsRepository.h"
#include "EMSettingIDs.h"

EMWinVideoOutputPin::EMWinVideoOutputPin(HRESULT *phr, CDynamicSource* p_opParentFilter)
	:	EMWinMediaOutputPin(phr, p_opParentFilter, L"This is the video output's name")
{
}

EMWinVideoOutputPin::~EMWinVideoOutputPin()
{
}

uint32 EMWinVideoOutputPin::ReportNumBuffers()
{
	return 2;
}

uint32 EMWinVideoOutputPin::ReportBufferSize()
{
	return m_opFilter -> m_opFormat -> m_vBufferSizeBytes;
}

EMMediaType EMWinVideoOutputPin::ReportFormatType()
{
	return EM_TYPE_ANY_VIDEO;
}

bool EMWinVideoOutputPin::OnActive()
{
	return true;
}

bool EMWinVideoOutputPin::OnDisconnect() 
{
	return true;
}

bool EMWinVideoOutputPin::ReportFormat(CMediaType* p_opFormat)
{

//	m_opFilter->GetMediaFormatInternal(p_opFormat);

	AM_MEDIA_TYPE* myFormat = (AM_MEDIA_TYPE*)p_opFormat;

	m_sVideoFormatStructure = reinterpret_cast<VIDEOINFOHEADER *>( p_opFormat -> AllocFormatBuffer(sizeof(VIDEOINFOHEADER)));

	m_sVideoFormatStructure -> bmiHeader.biBitCount = 8 * m_opFilter -> m_opFormat -> m_vDepth;
	m_sVideoFormatStructure -> bmiHeader.biCompression = 0;
	m_sVideoFormatStructure -> bmiHeader.biHeight = m_opFilter -> m_opFormat -> m_vHeight;
	m_sVideoFormatStructure -> bmiHeader.biPlanes = 1;
	m_sVideoFormatStructure -> bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	m_sVideoFormatStructure -> bmiHeader.biSizeImage = m_opFilter -> m_opFormat -> m_vBufferSizeBytes;
	m_sVideoFormatStructure -> bmiHeader.biWidth = m_opFilter -> m_opFormat -> m_vWidth;
	m_sVideoFormatStructure -> AvgTimePerFrame = 10000000 / m_opFilter -> m_opFormat -> m_vFrameRate;
	m_sVideoFormatStructure -> dwBitRate = 15552016; //m_opFormat -> m_vFrameRate*m_oFormat.m_vBufferSizeBytes*8;


	myFormat->majortype = MEDIATYPE_Video;
	myFormat->subtype = MEDIASUBTYPE_RGB24;
	myFormat->formattype = FORMAT_VideoInfo;

	return true;
}

#endif
