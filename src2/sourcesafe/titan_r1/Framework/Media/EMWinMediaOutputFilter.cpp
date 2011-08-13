#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#include "EMWinMediaOutputFilterExport.h"
#include "EMWinMediaOutputFilterInterface.h"
#include "EMWinMediaOutputFilterVersion.h"

#include "EMWinMediaOutputFilter.h"
#include "EMWinVideoOutputPin.h"

#include "EMMediaDataBuffer.h"
#include "EMSemaphore.h"
#include "EMMediaTimer.h"

#include <list>

static const GUID CLSID_EMWinMediaOutputFilter = {0x952c35a5, 0x4dcb, 0x11d5, {0xbc, 0x74, 0x0, 0x2, 0xb3, 0x17, 0xf3, 0xf2}};
static const GUID CLSID_EMWinMediaVideoOutputFilter = {0xbedfce61, 0x535d, 0x11d5, {0x82, 0x59, 0x0, 0x2, 0xb3, 0x18, 0x0, 0x46}};
static const GUID IID_IEMWinMediaOutputFilter =  {0x952c35a8, 0x4dcb, 0x11d5, {0xbc, 0x74, 0x0, 0x2, 0xb3, 0x17, 0xf3, 0xf2}};
static const GUID IID_IEMWinMediaVideoOutputFilter = { 0xbedfce63, 0x535d, 0x11d5, { 0x82, 0x59, 0x0, 0x2, 0xb3, 0x18, 0x0, 0x46 } };

CFactoryTemplate g_Templates[] = 
{
	{L"EMWinMediaVideoOutputFilter", &CLSID_EMWinMediaVideoOutputFilter, EMWinMediaOutputFilter::CreateInstance, NULL, &sudEMWinMediaVideoOutputFilterRegs}
};
int g_cTemplates = 1;

CUnknown* WINAPI EMWinMediaOutputFilter::CreateInstance(LPUNKNOWN lpunk, HRESULT *phr)
{	
	CUnknown *punk = new EMWinMediaOutputFilter(lpunk, phr);
	if(punk == NULL) 
	{
		*phr = E_OUTOFMEMORY;
	}
    return punk;
}

EMWinMediaOutputFilter::EMWinMediaOutputFilter(LPUNKNOWN lpunk, HRESULT *phr) 
	:	CDynamicSource(NAME("EMWinMediaVideoOutputFilter"), lpunk, CLSID_EMWinMediaVideoOutputFilter, phr),
		m_opSemaphoreOne(NULL),
		m_opSemaphoreTwo(NULL),
		m_opProtection(NULL),
		m_opTimer(NULL),
		m_opSettingsRepository(NULL),
		m_vRecentlyFlushed(false),
		m_opFormat(NULL),
		m_vIsRunning(true),
		m_vIsSilenceBuffer(false),
		m_vBelongedToUniqueID(-1)
{
    m_paStreams = (CDynamicSourceStream **) new EMWinMediaOutputPin*[1];
    if(m_paStreams == NULL) 
	{
        *phr = E_OUTOFMEMORY;
        return;
    }

	//m_eMediaType = *(EMMediaType*) lpunk;


	m_paStreams[0] = EMWinMediaOutputPin::Create(EM_TYPE_RAW_VIDEO, this);
    if(m_paStreams[0] == NULL) 
	{
		*phr = E_OUTOFMEMORY;
	}

//	m_vSemCounter = 0;
	m_opSemaphoreOne = EMSemaphore::CreateEMSemaphore("DirectX counting semaphore I", 0, 2);
	m_opSemaphoreTwo = EMSemaphore::CreateEMSemaphore("DirectX counting semaphore II", 0, 2);
	m_opProtection = EMSemaphore::CreateEMSemaphore("DirectX protection semaphore", 1, 1);
	m_oCache.clear();
}

EMWinMediaOutputFilter::~EMWinMediaOutputFilter()
{
	m_paStreams[0]->Exit();

	delete m_paStreams[0];
	delete m_paStreams;


	delete m_opSemaphoreOne;
	delete m_opSemaphoreTwo;
	delete m_opProtection;

	delete m_opFormat;
}

STDMETHODIMP EMWinMediaOutputFilter::NonDelegatingQueryInterface(REFIID riid, void **ppv)
{
/*	int a;
	a = 5;*/

    if(riid == IID_IEMWinMediaOutputFilter)
	{
		eo << "MediaOutputFilter Library v0.1Win, Build " << __EMWinMediaOutputFilterVersion << " by " << __EMWinMediaOutputFilterVersion_Owner << " (C) Elegant Media AB 2000-2001" << ef;
        return GetInterface((EMWinMediaOutputFilterInterface*) this, ppv);
    } 
	else 
	{
        return CBaseFilter::NonDelegatingQueryInterface(riid, ppv);
    }
}

AM_MEDIA_TYPE *EMWinMediaOutputFilter::GetMediaFormatInternal(CMediaType *p_opFormat)
{
	AM_MEDIA_TYPE* opType = static_cast<AM_MEDIA_TYPE*>(p_opFormat);
	
	m_sVideoFormatStructure = reinterpret_cast<VIDEOINFOHEADER *>( p_opFormat -> AllocFormatBuffer(sizeof(VIDEOINFOHEADER)));

	m_sVideoFormatStructure -> bmiHeader.biBitCount = 8 * m_opFormat -> m_vDepth;
	m_sVideoFormatStructure -> bmiHeader.biCompression = 0;
	m_sVideoFormatStructure -> bmiHeader.biHeight = m_opFormat -> m_vHeight;
	m_sVideoFormatStructure -> bmiHeader.biPlanes = 1;
	m_sVideoFormatStructure -> bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	m_sVideoFormatStructure -> bmiHeader.biSizeImage = m_opFormat -> m_vBufferSizeBytes;
	m_sVideoFormatStructure -> bmiHeader.biWidth = m_opFormat -> m_vWidth;
	m_sVideoFormatStructure -> AvgTimePerFrame = 10000000 / m_opFormat -> m_vFrameRate;
	m_sVideoFormatStructure -> dwBitRate = 15552016; //m_opFormat -> m_vFrameRate*m_opFormat -> m_vBufferSizeBytes*8;

	opType->majortype = MEDIATYPE_Video;
	opType->subtype = MEDIASUBTYPE_RGB24;
	opType->formattype = FORMAT_VideoInfo;

	return opType;
}


bool EMWinMediaOutputFilter::FormatHasChanged(EMMediaFormat *p_opFormat)
{
	if (p_opFormat->m_vBufferSizeBytes != m_opFormat -> m_vBufferSizeBytes)
		return true;

	if (p_opFormat->m_vFrameRate != m_opFormat -> m_vFrameRate)
		return true;

	if (p_opFormat->m_vHeight != m_opFormat -> m_vHeight)
		return true;

	if (p_opFormat->m_vWidth != m_opFormat -> m_vWidth) 
		return true;
	
	
	return false;
}

void EMWinMediaOutputFilter::SaveFormat(EMMediaFormat *p_opFormat)
{
	memcpy(m_opFormat, p_opFormat, sizeof(EMMediaFormat));
}


STDMETHODIMP EMWinMediaOutputFilter::RegisterAsClockMaster(int32 p_vRealtimeOutputDescriptorID)
{
	((EMWinVideoOutputPin*) m_paStreams[0]) -> RegisterAsClockMaster(p_vRealtimeOutputDescriptorID);
	return S_OK;
}

STDMETHODIMP EMWinMediaOutputFilter::UnregisterAsClockMaster()
{
	((EMWinVideoOutputPin*) m_paStreams[0]) -> UnregisterAsClockMaster();
	return S_OK;
}



STDMETHODIMP EMWinMediaOutputFilter::AddBufferToCache(EMMediaDataBuffer* p_opBuffer)
{
//	CAutoLock lShared(&static_cast<EMWinMediaOutputPin*>(m_paStreams[0])->m_cSharedState);

	CMediaType oMediaType;

	if(p_opBuffer == NULL)
	{
		eo << "   DirectX -> ERROR! EMWinMediaOutputFilter::AddToCache cannot accept a NULL parameter!" << ef;
		return E_FAIL;
	}

	if(!m_vIsRunning)
	{
		eo << "EMWinMediaOutputFilter::AddBufferToCache called when stopped"<<ef;
		return E_FAIL;
	}

	if (FormatHasChanged(&p_opBuffer->m_oFormat))
	{
		m_paStreams[0]->StartUsingOutputPin();

		SaveFormat(&p_opBuffer->m_oFormat);
		
		FlushCache();

		AM_MEDIA_TYPE* myFormat = (AM_MEDIA_TYPE*) &oMediaType;

		VIDEOINFOHEADER *sVideoFormatStructure = reinterpret_cast<VIDEOINFOHEADER *>( oMediaType.AllocFormatBuffer(sizeof(VIDEOINFOHEADER)));

		sVideoFormatStructure -> bmiHeader.biBitCount = 8*m_opFormat -> m_vDepth;
		sVideoFormatStructure -> bmiHeader.biCompression = 0;
		sVideoFormatStructure -> bmiHeader.biHeight = m_opFormat -> m_vHeight;
		sVideoFormatStructure -> bmiHeader.biPlanes = 1;
		sVideoFormatStructure -> bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		sVideoFormatStructure -> bmiHeader.biSizeImage =m_opFormat -> m_vBufferSizeBytes;
		sVideoFormatStructure -> bmiHeader.biWidth = m_opFormat -> m_vWidth;
		sVideoFormatStructure -> AvgTimePerFrame = 10000000 / m_opFormat -> m_vFrameRate;
		sVideoFormatStructure -> dwBitRate = 15552016; //m_opFormat -> m_vFrameRate*m_opFormat -> m_vBufferSizeBytes*8;


		oMediaType.majortype = MEDIATYPE_Video;
		oMediaType.subtype = MEDIASUBTYPE_RGB24;
		oMediaType.formattype = FORMAT_VideoInfo;

		HRESULT hr = m_paStreams[0]->ChangeOutputFormat(myFormat, 0, 1, 1.0);
		
//		TCHAR buff[1024];
//		AMGetErrorText( hr, buff, 1024);

		m_paStreams[0]->StopUsingOutputPin();
	}

	//Copy the buffer into a IMediaSample and store the IMediaSample!
	
	IMediaSample* opIMediaSample = NULL;

	CMediaType arne;

	opIMediaSample = ((EMWinVideoOutputPin*) m_paStreams[0]) -> ConvertFromBufferToSample(p_opBuffer);

	if(opIMediaSample == NULL)
	{
		eo << "111   DirectX -> ERROR! Could not convert from MediaBuffer to MediaSample! Project might be stopped?" << ef;
		p_opBuffer -> Recycle();
		return S_OK;
	}
	p_opBuffer -> Recycle();


	REFERENCE_TIME vTimeStart = 10*p_opBuffer->GetFrame();
	REFERENCE_TIME vTimeEnd   = vTimeStart + 10000000.0f / p_opBuffer -> m_oFormat.m_vFrameRate;

	opIMediaSample -> SetTime(&vTimeStart,&vTimeEnd);

/*	if(m_vBelongedToUniqueID == -1)
		m_vBelongedToUniqueID = p_opBuffer -> m_vBelongsToUniqueID;

	if(p_opBuffer -> m_vBelongsToUniqueID != m_vBelongedToUniqueID)
	{
		opIMediaSample -> SetDiscontinuity(TRUE);
		p_opBuffer -> m_vDiscon = TRUE;
		m_vBelongedToUniqueID = p_opBuffer -> m_vBelongsToUniqueID;
	} 
*/	
	if(p_opBuffer -> m_vIsSilence && !m_vIsSilenceBuffer)
	{
		p_opBuffer->m_vDiscon = TRUE;
		m_vIsSilenceBuffer = TRUE;
	} 
	else if((!p_opBuffer -> m_vIsSilence) && m_vIsSilenceBuffer)
	{
		p_opBuffer -> m_vDiscon = TRUE;
		m_vIsSilenceBuffer = FALSE;
	}

	opIMediaSample -> SetDiscontinuity(p_opBuffer->m_vDiscon);

	if(p_opBuffer -> m_vDiscon)
	{
		FlushCache();
	}
	
	p_opBuffer -> m_vDiscon = FALSE;
	m_opProtection -> Acquire();

	m_oCache.push_back(opIMediaSample);
	m_opSemaphoreOne -> Release();

	m_opProtection -> Release();

//	m_vSemCounter ++;
	return S_OK;
}

STDMETHODIMP EMWinMediaOutputFilter::FlushCache()
{
	if(m_oCache.size() <= 0)
		return E_FAIL;
	
	m_opProtection -> Acquire();

	while(m_vIsRunning && (m_oCache.size() > 0))
	{
		m_opSemaphoreOne -> Acquire();// NonBlockingAcquire();

		IMediaSample* opSample = m_oCache.front();
		m_oCache.pop_front();

		opSample -> Release();
	}
	m_opProtection -> Release();

	return S_OK;
}

STDMETHODIMP EMWinMediaOutputFilter::GetBufferFromCache(IMediaSample** p_opIMediaSample)
{

	if(p_opIMediaSample == NULL)
	{
		eo << "   DirectX -> ERROR! EMWinMediaOutputFilter::GetFromCache cannot accept a NULL parameter!" << ef;
		*p_opIMediaSample = NULL;
		return E_FAIL;
	}

	while((! m_opSemaphoreOne -> NonBlockingAcquire()) && m_vIsRunning)
		::Sleep(5);

	m_opProtection -> Acquire();
	if(m_oCache.size() <= 0 || m_vIsRunning == false)
	{
		*p_opIMediaSample = NULL;
		m_opProtection -> Release();
		m_opSemaphoreOne -> Release();
		return E_FAIL;
	}
	
	(*p_opIMediaSample) = m_oCache.front();
	m_oCache.pop_front();

/*	if(pSample -> GetTime(&pTimeStart, &pTimeStop) != S_OK)
		MessageBox(NULL, "Couldn't get Time Stamp", "ERROR of Some kind", MB_OK);

	if (m_pClock == NULL)
	{
		eo << "EMWinVideoVirtualRenderer::Receive(): No reference clock is available!!" << ef;
	}

	m_pClock -> GetTime(&vTime);

	if (pTimeStop < vTime)
	{
		return;
	}

	Sleep(max(pTimeStart - vTime, 0));*/



	m_opProtection -> Release();

	return S_OK;
}


IReferenceClock *EMWinMediaOutputFilter::GetClock()
{
	return m_pClock;
}


IMediaEvent *EMWinMediaOutputFilter::GetEventSource()
{
	IMediaEvent *opIEvent = NULL;

	m_pGraph -> QueryInterface(IID_IMediaEvent, (void **) &opIEvent);

	return opIEvent;
}

int EMWinMediaOutputFilter::GetNumBuffers()
{
	return m_oCache.size();
}

STDMETHODIMP EMWinMediaOutputFilter::InitializeFilter(EMMediaTimer* p_opTimer, EMSettingsRepository* p_opSettingsRepository)
{
	m_opFormat = EM_new EMMediaFormat(EM_TYPE_RAW_VIDEO);
	m_opTimer = p_opTimer;
	m_opSettingsRepository = p_opSettingsRepository;
	m_vIsRunning = true;
	return S_OK;
}


EMMediaTimer* EMWinMediaOutputFilter::GetTimer()
{
	return m_opTimer;
}

EMSettingsRepository* EMWinMediaOutputFilter::GetSettingsRepository()
{
	return m_opSettingsRepository;
}

STDMETHODIMP EMWinMediaOutputFilter::Stop()
{
	return Shutdown();
}

STDMETHODIMP EMWinMediaOutputFilter::Shutdown()
{
	m_vIsRunning = false;

	m_opProtection -> Release();
	m_opSemaphoreOne -> Release();

	return S_OK;
}

#endif
