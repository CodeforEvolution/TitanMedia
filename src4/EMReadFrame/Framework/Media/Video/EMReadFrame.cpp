#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#include <windows.h>
//#include <commdlg.h>
#include <streams.h>
//#include <initguid.h>

#include "EMReadFrame.h"
#include "EMScalingUtils.h"
#include "EMMediaFormat.h"

// Setup data
static const GUID CLSID_EMReadFrame = {0x36a5f770, 0xfe4c, 0x11ce, {0xa8, 0xed, 0x00, 0xaa, 0x00, 0x2f, 0xea, 0xb5}};
static const GUID IID_EMReadFrameInterface = {0xfe75b1e1, 0x3821, 0x11d5, {0x82, 0x00, 0x2, 0xb3, 0x18, 0x0, 0x46}};

const AMOVIESETUP_MEDIATYPE sudPinTypes =
{
    &MEDIATYPE_NULL,            // Major type
    &MEDIASUBTYPE_NULL          // Minor type
};

const AMOVIESETUP_PIN sudPins =
{
    L"Input",                   // Pin string name
    FALSE,                      // Is it rendered
    FALSE,                      // Is it an output
    FALSE,                      // Allowed none
    FALSE,                      // Likewise many
    &CLSID_NULL,                // Connects to filter
    L"Output",                  // Connects to pin
    1,                          // Number of types
    &sudPinTypes                // Pin information
};

const AMOVIESETUP_FILTER sudDump =
{
    &CLSID_EMReadFrame,                // Filter CLSID
    L"Elegant Media FrameGrabber",                    // String name
    MERIT_DO_NOT_USE,           // Filter merit
    1,                          // Number pins
    &sudPins                    // Pin details
};


//
//  Object creation stuff
//
CFactoryTemplate g_Templates[]= {
    L"EMReadFrame", &CLSID_EMReadFrame, EMReadFrame::CreateInstance, NULL, &sudDump
};
int g_cTemplates = 1;


// Constructor

EMReadFrameFilter::EMReadFrameFilter(EMReadFrame *pDump,
                         LPUNKNOWN pUnk,
                         CCritSec *pLock,
                         HRESULT *phr) :
    CBaseFilter(NAME("EMReadFrameFilter"), pUnk, pLock, CLSID_EMReadFrame),
    m_pDump(pDump)
{
}


//
// GetPin
//
CBasePin * EMReadFrameFilter::GetPin(int n)
{
    if (n == 0) {
        return m_pDump->m_pPin;
    } else {
        return NULL;
    }
}


//
// GetPinCount
//
int EMReadFrameFilter::GetPinCount()
{
    return 1;
}


//
// Stop
//
// Overriden to close the dump file
//
STDMETHODIMP EMReadFrameFilter::Stop()
{
    CAutoLock cObjectLock(m_pLock);
   // m_pDump->CloseFile();
    return CBaseFilter::Stop();
}


//
// Pause
//
// Overriden to open the dump file
//
STDMETHODIMP EMReadFrameFilter::Pause()
{
    CAutoLock cObjectLock(m_pLock);
    //m_pDump->OpenFile();
    return CBaseFilter::Pause();
}


//
// Run
//
// Overriden to open the dump file
//
STDMETHODIMP EMReadFrameFilter::Run(REFERENCE_TIME tStart)
{
	IReferenceClock *pClock = m_pClock;
    CAutoLock cObjectLock(m_pLock);
	CRefTime agnusStreamTimeObject;
	StreamTime(agnusStreamTimeObject);
	long millisekunder = agnusStreamTimeObject.Millisecs();
    return CBaseFilter::Run(tStart);
}


//
//  Definition of EMReadFrameInputPin
//
EMReadFrameInputPin::EMReadFrameInputPin(EMReadFrame *pDump,
                             LPUNKNOWN pUnk,
                             CBaseFilter *pFilter,
                             CCritSec *pLock,
                             CCritSec *pReceiveLock,
                             HRESULT *phr) :

    CRenderedInputPin(NAME("EMReadFrameInputPin"),
                  pFilter,                   // Filter
                  pLock,                     // Locking
                  phr,                       // Return code
                  L"Input"),                 // Pin name
    m_pReceiveLock(pReceiveLock),
    m_pDump(pDump),
    m_tLast(0)
{
}


//
// CheckMediaType
//
// Check if the pin can support this specific proposed type and format
//
HRESULT EMReadFrameInputPin::CheckMediaType(const CMediaType *pmt)
{
    if (CanPerformEZrgb24(pmt)) {
		//m_pDump->m_opMediaVideoInfoHeader = (VIDEOINFOHEADER *) pmt->Format();
		return S_OK;
    }
    return E_FAIL;

}

HRESULT EMReadFrameInputPin::SetMediaType(const CMediaType *pmt) 
{	
	CAutoLock cLock(m_pReceiveLock);

	m_pDump->SetMediaType(pmt);
	return CRenderedInputPin::SetMediaType(pmt);
}

BOOL EMReadFrameInputPin::CanPerformEZrgb24(const CMediaType *pMediaType) const
{
    if (IsEqualGUID(*pMediaType->Type(), MEDIATYPE_Video)) {
        if (IsEqualGUID(*pMediaType->Subtype(), MEDIASUBTYPE_RGB24)) {
            VIDEOINFOHEADER *pvi = (VIDEOINFOHEADER *) pMediaType->Format();
            return (pvi->bmiHeader.biBitCount == 24);
        }
    }
    return FALSE;

} // CanPerformEZrgb24


STDMETHODIMP EMReadFrameInputPin::EndOfStream()
{
	m_pDump -> m_vIsRunning = false;
	m_pDump -> m_vEndOfTheLine = true;
	m_pDump -> m_vFrameValid = true;
	//ReleaseSemaphore(m_pDump -> m_hSem, 1, &m_pDump -> m_vSemCount);
	//SetEvent(m_pDump -> m_oEventSeekWaitingForGrab);
	//SetEvent(m_pDump -> m_oEventGrabWaitingForSeek);
	ReleaseSemaphore(m_pDump -> m_oSemWaitingForSampleCB, 1, &m_pDump -> m_vWaitForSampleCBToFinishCount);
	ReleaseSemaphore(m_pDump -> m_oSemSeekWaitingForGrab, 1, &m_pDump -> m_vWaitForFrameToArriveCount);

	//ReleaseSemaphore(m_pDump -> m_oSemGrabWaitingForSeek, 1,);
	return S_OK;
}



//
// Receive
//
// Do something with this media sample
//
STDMETHODIMP EMReadFrameInputPin::Receive(IMediaSample *pSample)
{
//    CAutoLock lock(m_pReceiveLock);		//Kaneda test - sätt tillbaka!!
	m_pDump->SampleCB(pSample);

	return S_OK;//m_pDump->Write(pbData,pSample->GetActualDataLength());

}

//
// NewSegment
//
// Called when we are seeked
//
STDMETHODIMP EMReadFrameInputPin::NewSegment(REFERENCE_TIME tStart,
                                       REFERENCE_TIME tStop,
                                       double dRate)
{

    m_tLast = 0;
    return S_OK;

} // NewSegment


//
//  EMReadFrame class
//

EMReadFrame::EMReadFrame(LPUNKNOWN pUnk, HRESULT *phr) :
     CBaseRenderer (CLSID_EMReadFrame, "DUMP", pUnk, phr),
    m_pFilter(NULL),
    m_pPin(NULL),
    //m_pPosition(NULL),
    //m_hFile(INVALID_HANDLE_VALUE),
    //m_pFileName(0),
	m_hSem(CreateSemaphore(NULL, 0, 1, NULL)),
	m_hSemWaitForFrameToArrive(CreateSemaphore(NULL, 0, 1, NULL)),
	m_hSemWaitForSampleCBToFinish(CreateSemaphore(NULL, 0, 1, NULL)),
	m_hSemWaitBeforeSeekOrGrab(CreateSemaphore(NULL, 0, 1, NULL)),
	m_vSegmentStart(0),
	m_vSeeking(false),
	m_vIsRunning(true),
	m_vScale(true),
	m_vEndOfTheLine(false),
	m_opSeek(NULL),
	m_opMediaControl(NULL),
	m_opSource(EM_new EMMediaFormat(EM_TYPE_RAW_VIDEO))

{

	m_vNumberOfTimesReleasedSemaphores = 0;
	m_vNumberOfTimesUsedSemaphores = 0;
	m_vWaitForSampleCBToFinishCount = 0;
	m_vSemCount = 0;
	m_vWaitForFrameToArriveCount = 0;
	m_vWaitBeforeSeekOrGrab = 0;
	m_pPosition = 0;

    m_pFilter = new EMReadFrameFilter(this, GetOwner(), &m_Lock, phr);
    if (m_pFilter == NULL) {
        *phr = E_OUTOFMEMORY;
        return;
    }


	m_opBuffer = 0;

    m_pPin = new EMReadFrameInputPin(this,GetOwner(),
                               m_pFilter,
                               &m_Lock,
                               &m_ReceiveLock,
                               phr);
    if (m_pPin == NULL) {
        *phr = E_OUTOFMEMORY;
        return;
    }

	m_vFrameValid = false;

	//Create some events
	/*m_oEventWaitingForSampleCB = CreateEvent(NULL, true, false, NULL);
	m_oEventWaitingForGrab = CreateEvent(NULL, true, false, NULL);
	m_oEventSeekWaitingForGrab = CreateEvent(NULL, true, true, NULL);
	m_oEventGrabWaitingForSeek = CreateEvent(NULL, true, true, NULL);*/


	m_oSemWaitingForSampleCB = CreateSemaphore(NULL, 0, 1, NULL);
	m_oSemWaitingForGrab = CreateSemaphore(NULL, 0, 1, NULL);
	m_oSemSeekWaitingForGrab = CreateSemaphore(NULL, 1, 1, NULL);
	m_oSemGrabWaitingForSeek = CreateSemaphore(NULL, 0, 1, NULL);

	//End creating events

}

HRESULT EMReadFrame::DoRenderSample(IMediaSample *pMediaSample)
{
	SampleCB(pMediaSample);
    return S_OK;
} // DoRenderSample

HRESULT EMReadFrame::CheckMediaType(const CMediaType *pmtIn)
{
    return m_Display.CheckMediaType(pmtIn);
} // CheckMediaType

// Destructor
EMReadFrame::~EMReadFrame()
{
	CloseHandle(m_hSemWaitForFrameToArrive);
	m_hSemWaitForFrameToArrive = 0;

	CloseHandle(m_hSemWaitForSampleCBToFinish);
	m_hSemWaitForSampleCBToFinish = 0;

	CloseHandle(m_hSem);
	m_hSem = 0;

	CloseHandle(m_hSemWaitBeforeSeekOrGrab);
	m_hSemWaitBeforeSeekOrGrab = 0;

	if (m_pPin != NULL) 
	{
		delete m_pPin;
		m_pPin = NULL;
	}

	if (m_pFilter != NULL)
	{
		delete m_pFilter;
		m_pFilter = NULL;
	}

	if (m_pPosition != NULL)
	{
		delete m_pPosition;
		m_pPosition = NULL;
	}

	if(m_opSeek != NULL)
	{
		m_opSeek -> Release();
		m_opSeek = NULL;
	}

	if(m_opMediaControl != NULL)
	{
		m_opMediaControl -> Release();
		m_opMediaControl = NULL;
	}

//	delete m_opSource;
}


//
// CreateInstance
//
// Provide the way for COM to create a dump filter
//
CUnknown * WINAPI EMReadFrame::CreateInstance(LPUNKNOWN punk, HRESULT *phr)
{
    EMReadFrame *pNewObject = new EMReadFrame(punk, phr);
    if (pNewObject == NULL) {
        *phr = E_OUTOFMEMORY;
    }
    return pNewObject;

} // CreateInstance


//
// NonDelegatingQueryInterface
//
// Override this to say what interfaces we support where
//
STDMETHODIMP EMReadFrame::NonDelegatingQueryInterface(REFIID riid, void ** ppv)
{
    CheckPointer(ppv,E_POINTER);
    CAutoLock lock(&m_Lock);

    if (riid == IID_EMReadFrameInterface)
		return GetInterface((EMReadFrameInterface*) this, ppv);

	if (riid == IID_IBaseFilter || riid == IID_IMediaFilter || riid == IID_IPersist)
	{
		return m_pFilter -> NonDelegatingQueryInterface(riid, ppv);
		//return CBaseFilter::/*m_pFilter->*/NonDelegatingQueryInterface(riid, ppv);
    }
	else 

	if (riid == IID_IMediaPosition || riid == IID_IMediaSeeking)
	{
		if (m_pPosition == NULL)
		{

            HRESULT hr = S_OK;
            m_pPosition = new CRendererPosPassThru(NAME("Dump Pass Through"), GetOwner(), (HRESULT *) &hr, m_pPin);

			//hr = CreatePosPassThru(GetOwner(), TRUE, GetPin(0), (IUnknown**)&m_pPosition);
            if (m_pPosition == NULL)
			{
                return E_OUTOFMEMORY;
            }

            if (FAILED(hr))
			{
                delete m_pPosition;
                m_pPosition = NULL;
                return hr;
            }
        }
        return m_pPosition->NonDelegatingQueryInterface(riid, ppv);
    }

  else
	{
		return CUnknown::NonDelegatingQueryInterface(riid, ppv);
	}

} // NonDelegatingQueryInterface

//
// DllRegisterSever
//
// Handle the registration of this filter
//
STDAPI DllRegisterServer()
{
    return AMovieDllRegisterServer2( TRUE );

} // DllRegisterServer


//
// DllUnregisterServer
//
STDAPI DllUnregisterServer()
{
    return AMovieDllRegisterServer2( FALSE );

} // DllUnregisterServer


HRESULT EMReadFrame::Receive(IMediaSample *pSample)
{
	return m_pPin->Receive(pSample);
}

HRESULT EMReadFrame::SetMediaType(const CMediaType *pmt)
{
	m_opMediaType = (AM_MEDIA_TYPE*)pmt;

	VIDEOINFOHEADER* oVidInfo = (VIDEOINFOHEADER*)m_opMediaType->pbFormat;
	
	m_vSourceSizeX = oVidInfo -> bmiHeader.biWidth;
	m_vSourceSizeY = oVidInfo -> bmiHeader.biHeight;
	m_vBytesPerSourcePixel = oVidInfo -> bmiHeader.biBitCount / 8;
	m_vSourceSkipSize = oVidInfo -> bmiHeader.biSizeImage / abs(oVidInfo->bmiHeader.biHeight);

	m_opSource -> m_vWidth = oVidInfo -> bmiHeader.biWidth;
	m_opSource -> m_vHeight = oVidInfo -> bmiHeader.biHeight;
	m_opSource -> m_vDepth = oVidInfo -> bmiHeader.biBitCount / 8;
	m_opSource -> m_vBytesPerRow = oVidInfo -> bmiHeader.biSizeImage / abs(oVidInfo->bmiHeader.biHeight);
	m_opSource -> m_vBufferSizeBytes = m_opSource -> m_vBytesPerRow * m_opSource -> m_vHeight;

	return NOERROR;
}

STDMETHODIMP EMReadFrame::SampleCB(IMediaSample *p_opSample)
{
    CAutoLock cRendererLock(&m_InterfaceLock);
	CAutoLock cSampleLock(&m_RendererLock);
	REFERENCE_TIME pTimeStart;
	REFERENCE_TIME pTimeStop;


	if(p_opSample -> GetTime(&pTimeStart, &pTimeStop) != S_OK)
		MessageBox(NULL, "Couldn't get Time Stamp", "ERROR of Some kind", MB_OK);

	PBYTE  opDataPointer;
	if (p_opSample->GetPointer(&opDataPointer) != S_OK)
		MessageBox(NULL, "ERROR", "ERROR GetConnectedMediaType", MB_OK);

	if(!m_vFrameValid && !m_vFrameValid2)
	{
		//ReleaseSemaphore(m_hSem, 1, &m_vSemCount);

		//WaitForSingleObject(m_hSemWaitBeforeSeekOrGrab, INFINITE);
		//WaitForSingleObject(m_oEventWaitingForGrab, INFINITE);
		WaitForSingleObject(m_oSemWaitingForGrab, INFINITE);

		if(!m_vFrameValid && !m_vFrameValid2)
		{
			//WaitForSingleObject(m_hSemWaitForFrameToArrive, INFINITE);

			//Copy the data into our buffer
			if(m_opBuffer!=0)
			{
				
				if(m_vScale)
					m_oScale.Scale(opDataPointer, m_opBuffer);
				else
					memcpy(m_opBuffer, opDataPointer, m_vBytesPerFrame);
			}

			m_vSampleTime = pTimeStart;
	
			m_vNumberOfTimesReleasedSemaphores++;
			//SetEvent(m_oEventWaitingForSampleCB);
			ReleaseSemaphore(m_oSemWaitingForSampleCB, 1, &m_vWaitForSampleCBToFinishCount);

			//ReleaseSemaphore(m_hSemWaitForSampleCBToFinish, 1, &m_vWaitForSampleCBToFinishCount);
		}
//		eo << "I'm waiting no more for you" << ef;
	}
	else
	{
	//	ReleaseSemaphore(m_hSemWaitForSampleCBToFinish, 1, NULL);
	}

//	eo << "Leaving EMReadFrame::SampleCB" << ef;
	return S_OK;
}

STDMETHODIMP EMReadFrame::GrabFrame(EMMediaDataBuffer *p_opData, REFERENCE_TIME& p_vSampleTime)
{
	//WaitForSingleObject(m_oEventGrabWaitingForSeek,1000);
	WaitForSingleObject(m_oSemSeekWaitingForGrab,1000);
	//ResetEvent(m_oEventSeekWaitingForGrab);
	m_vFrameValid2 = false;
	if(m_vFrameValid)
	{
		ReleaseSemaphore(m_oSemSeekWaitingForGrab, 1, &m_vWaitForFrameToArriveCount);
		return E_FAIL;
	}

	if(m_vNumberOfTimesUsedSemaphores > m_vNumberOfTimesReleasedSemaphores)
	{
			//eo << "JUMP IN HERE" << ef;				
	}

//	WaitForSingleObject(m_hSem, INFINITE);

	if (m_vIsRunning || !m_vEndOfTheLine)
	{
		//WaitForSingleObject(m_hSem, INFINITE);
		if (!m_vIsRunning || m_vEndOfTheLine)
		{
			//SetEvent(m_oEventSeekWaitingForGrab);
			ReleaseSemaphore(m_oSemSeekWaitingForGrab, 1, &m_vWaitForFrameToArriveCount);
			return E_FAIL;
		}
		if (SizeHasChanged(&p_opData->m_oFormat))
		{
			SetOutputFormat(&(p_opData -> m_oFormat));
		}

		m_opBuffer = p_opData->Data();

		//ReleaseSemaphore(m_hSemWaitForFrameToArrive, 1, &m_vWaitForFrameToArriveCount);
		//ReleaseSemaphore(m_hSemWaitBeforeSeekOrGrab, 1, &m_vWaitBeforeSeekOrGrab);

		m_vNumberOfTimesUsedSemaphores++;

		//SetEvent(m_oEventWaitingForGrab);
		ReleaseSemaphore(m_oSemWaitingForGrab, 1, &m_vWaitBeforeSeekOrGrab);
		//WaitForSingleObject(m_oEventWaitingForSampleCB, INFINITE);
		WaitForSingleObject(m_oSemWaitingForSampleCB, 1000);//INFINITE);
		//ResetEvent(m_oEventWaitingForGrab);
		//ResetEvent(m_oEventWaitingForSampleCB);

		//WaitForSingleObject(m_hSemWaitForSampleCBToFinish, 100);//INFINITE);

		p_vSampleTime = (m_vSampleTime + m_vSegmentStart)/10;
		if(p_opData->Data() == NULL)
		{
			//eo << "Break Here" << ef;
		}

		m_vEndOfTheLine = false;
		//SetEvent(m_oEventSeekWaitingForGrab);
		ReleaseSemaphore(m_oSemSeekWaitingForGrab, 1, &m_vWaitForFrameToArriveCount);

		return S_OK;
	}
	
	//SetEvent(m_oEventSeekWaitingForGrab);
	ReleaseSemaphore(m_oSemSeekWaitingForGrab, 1, &m_vWaitForFrameToArriveCount);
	return E_FAIL;
}

STDMETHODIMP EMReadFrame::SeekToFrame(REFERENCE_TIME p_vFrameNumber)
{
/*	ReleaseSemaphore(m_hSemWaitForFrameToArrive, 1, NULL);
	WaitForSingleObject(m_hSemWaitForSampleCBToFinish, INFINITE);
	m_opSeek->SetPositions(&p_vFrameNumber, AM_SEEKING_AbsolutePositioning, NULL, AM_SEEKING_NoPositioning);
*/	

	MessageBox(NULL, "Obsolete function!!", "EMReadFrame::SeekToFrame - Error!!", MB_OK);

	return S_FALSE;

}

STDMETHODIMP EMReadFrame::SeekToTime(REFERENCE_TIME p_vSeekTime)
{
	//ResetEvent(m_oEventGrabWaitingForSeek);
	//WaitForSingleObject(m_oEventSeekWaitingForGrab, INFINITE);
	WaitForSingleObject(m_oSemSeekWaitingForGrab, INFINITE);
	p_vSeekTime = p_vSeekTime*10;

	m_opBuffer = 0;
	m_vFrameValid = true;
	m_vFrameValid2 = false;
	//ReleaseSemaphore(m_hSemWaitBeforeSeekOrGrab, 1, &m_vWaitForFrameToArriveCount);
	//SetEvent(m_oEventWaitingForGrab);
	ReleaseSemaphore(m_oSemWaitingForGrab, 1, &m_vWaitBeforeSeekOrGrab);

//	eo << "Doing SetPosition" << ef;
	if(m_opSeek->SetPositions(&p_vSeekTime, AM_SEEKING_AbsolutePositioning | AM_SEEKING_SeekToKeyFrame, NULL, AM_SEEKING_NoPositioning) != S_OK)
		MessageBox(NULL, "Seeking not OK", "Warning", MB_OK);
//	eo << "SetPosition has been done" << ef;
	//ResetEvent(m_oEventWaitingForGrab);
	m_vSegmentStart = p_vSeekTime;
	m_vFrameValid = false;
	m_vEndOfTheLine = false;
	m_vIsRunning = true;
	//SetEvent(m_oEventGrabWaitingForSeek);
	ReleaseSemaphore(m_oSemSeekWaitingForGrab, 1, &m_vWaitForFrameToArriveCount);
	return S_OK;

}

STDMETHODIMP EMReadFrame::SetOutputFormat(long p_vDestSizeX, long p_vDestSizeY, long p_vSkipSize)
{
	EMMediaFormat oFormat(EM_TYPE_RAW_VIDEO);
	
	oFormat.m_vWidth = m_vDestinationSizeX = p_vDestSizeX;
	oFormat.m_vHeight = m_vDestinationSizeY = p_vDestSizeY;
	oFormat.m_vBytesPerRow = m_vBytesPerDestinationPixel = p_vSkipSize;
	oFormat.m_vBufferSizeBytes = m_vBytesPerFrame =  p_vDestSizeY * p_vSkipSize;

	SetOutputFormat(&oFormat);

	return S_OK;
}



STDMETHODIMP EMReadFrame::SetOutputFormat(EMMediaFormat* p_opFormat) 
{
//	WaitForSingleObject(m_hSemChangeInAction, INFINITE);
	
	m_vDestinationSizeX = p_opFormat -> m_vWidth;
	m_vDestinationSizeY = p_opFormat -> m_vHeight;
	m_vBytesPerDestinationPixel = p_opFormat -> m_vBytesPerRow;
	m_vBytesPerFrame = p_opFormat -> m_vBufferSizeBytes;

	if(m_vScale)
		m_oScale.InitScaleFunctions(m_opSource, p_opFormat);

//	ReleaseSemaphore(m_hSemChangeInAction, 1, NULL);

	return S_OK;
}

bool EMReadFrame::SizeHasChanged(EMMediaFormat *p_opFormat)
{
	if (p_opFormat -> m_vHeight != m_vDestinationSizeY || p_opFormat -> m_vWidth != m_vDestinationSizeX || p_opFormat -> m_vBytesPerRow != m_vBytesPerDestinationPixel)
		return true;

	return false;
}

STDMETHODIMP EMReadFrame::UseScaling(bool p_vScale)
{
	m_vScale = p_vScale;
	return true;
}

STDMETHODIMP EMReadFrame::StopGraph()
{
	m_vFrameValid = true;
	ReleaseSemaphore(m_oSemWaitingForGrab, 1, &m_vWaitBeforeSeekOrGrab);
//	ReleaseSemaphore(m_oSemWaitingForSampleCB, 1, &m_vWaitForSampleCBToFinishCount);

/*	m_vFrameValid = false;
	m_opBuffer = NULL;
	ReleaseSemaphore(m_hSemWaitBeforeSeekOrGrab, 1, &m_vWaitBeforeSeekOrGrab);	
	ReleaseSemaphore(m_hSemWaitForFrameToArrive, 1, &m_vWaitForFrameToArriveCount);
*/

	m_opMediaControl -> Stop();

	return S_OK;
}

STDMETHODIMP EMReadFrame::InitializeRenderer(AM_MEDIA_TYPE* p_opMediaType, IMediaSeeking* p_opSeek, IMediaControl* p_opMediaControl)
{
	m_opSeek = p_opSeek;
	m_opMediaControl = p_opMediaControl;
	m_opSeek->GetDuration(&m_vDuration);

	m_opSeek -> AddRef();
	m_opMediaControl -> AddRef();

	return S_OK;
}

//Alla metoderna i interfacet avklarade .. börja med resterande implementationer

#endif