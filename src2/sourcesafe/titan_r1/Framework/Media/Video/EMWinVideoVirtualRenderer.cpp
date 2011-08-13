//------------------------------------------------------------------------------
// File: Dump.cpp
//
// Desc: DirectShow sample code - implementation of a renderer that dumps
//       the samples it receives into a text file.
//
// Copyright (c) 1992 - 2000, Microsoft Corporation.  All rights reserved.
//------------------------------------------------------------------------------


// Summary
//
// We are a generic renderer that can be attached to any data stream that
// uses IMemInputPin data transport. For each sample we receive we write
// its contents including its properties into a dump file. The file we
// will write into is specified when the dump filter is created. GraphEdit
// creates a file open dialog automatically when it sees a filter being
// created that supports the IFileSinkFilter interface.
//
//
// Implementation
//
// Pretty straightforward really, we have our own input pin class so that
// we can override Receive, all that does is to write the properties and
// data into a raw data file (using the Write function). We don't keep
// the file open when we are stopped so the flags to the open function
// ensure that we open a file if already there otherwise we create it.
// 
//
// Demonstration instructions
//
// Start GraphEdit, which is available in the SDK DXUtils folder. Drag and drop
// an MPEG, AVI or MOV file into the tool and it will be rendered. Then go to
// the filters in the graph and find the filter (box) titled "Video Renderer"
// This is the filter we will be replacing with the dump renderer. Then click
// on the box and hit DELETE. After that go to the Graph menu and select the
// "Insert Filters", from the dialog box find and select the "Dump Filter".
//
// You will be asked to supply a filename where you would like to have the
// data dumped, the data we receive in this filter is dumped in text form.
// Then dismiss the dialog. Back in the graph layout find the output pin of
// the filter that used to be connected to the input of the video renderer
// you just deleted, right click and do "Render". You should see it being
// connected to the input pin of the dump filter you just inserted.
//
// Click Pause and Run and then a little later stop on the GraphEdit frame and
// the data being passed to the renderer will be dumped into a file. Stop the
// graph and dump the filename that you entered when inserting the filter into
// the graph, the data supplied to the renderer will be displayed as raw data
//
//
// Files
//
// dump.cpp             Main implementation of the dump renderer
// dump.def             What APIs the DLL will import and export
// dump.h               Class definition of the derived renderer
// dump.rc              Version information for the sample DLL
// dumpuids.h           CLSID for the dump filter
// makefile             How to build it...
//
//
// Base classes used
//
// CBaseFilter          Base filter class supporting IMediaFilter
// CRenderedInputPin    An input pin attached to a renderer
// CUnknown             Handle IUnknown for our IFileSinkFilter
// CPosPassThru         Passes seeking interfaces upstream
// CCritSec             Helper class that wraps a critical section
//
//
#include "EMGlobals.h"
#include "EMMediaGlobals.h"
#ifdef PLATFORM_WINDOWS

#include "EMMediaProject.h"
#include "EMMediaEngine.h"

//#include "EMMediaGlobals.h"
#include "EMWinVideoVirtualRenderer.h"
#include "EMWinVideoVirtualRendererIDs.h"
#include "EMVideoPreview.h"
//#include "EMSettingIDs.h"
#include "EMBitmap.h"
#include "EMWinBitmap.h"
#include "EMMediaProject.h"
#include "EMMediaEngineUIDs.h"
//#include "EMMediaEngine.h"
//#include "EMSettingsRepository.h"

#include <windows.h>
//#include <commdlg.h>
#include <streams.h>
//#include <initguid.h>

//#include "EMVideoPreview.h"

//static const GUID IID_VideoVirtualRenderer = {0xc22ad161, 0x5523, 0x11d5, {0x82, 0x59, 0x0, 0x2, 0xb3, 0x18, 0x0, 0x46}};
//static const GUID CLSID_VideoVirtualRenderer = { 0xf0e70ea5, 0x51e6, 0x11d5, { 0x82, 0x59, 0x0, 0x2, 0xb3, 0x18, 0x0, 0x46 } };

// Setup data

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
    &CLSID_VideoVirtualRenderer, // Filter CLSID
    L"VideoVirtualRenderer",    // String name
    MERIT_DO_NOT_USE,           // Filter merit
    1,                          // Number pins
    &sudPins                    // Pin details
};


//
//  Object creation stuff
//
CFactoryTemplate g_Templates[]= {
    L"Dump", &CLSID_VideoVirtualRenderer, CDump::CreateInstance, NULL, &sudDump
};
int g_cTemplates = 1;


// Constructor

CDumpFilter::CDumpFilter(CDump *pDump,
                         LPUNKNOWN pUnk,
                         CCritSec *pLock,
                         HRESULT *phr) :
    CBaseFilter(NAME("CDumpFilter"), pUnk, pLock, CLSID_VideoVirtualRenderer),
    m_pDump(pDump)
{
}


//
// GetPin
//
CBasePin * CDumpFilter::GetPin(int n)
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
int CDumpFilter::GetPinCount()
{
    return 1;
}


//
// Stop
//
// Overriden to close the dump file
//
STDMETHODIMP CDumpFilter::Stop()
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
STDMETHODIMP CDumpFilter::Pause()
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
STDMETHODIMP CDumpFilter::Run(REFERENCE_TIME tStart)
{
	IReferenceClock *pClock = m_pClock;
    CAutoLock cObjectLock(m_pLock);
// 	MessageBox(NULL, "Get ready to ruuuuumble", "A MESSAGE FOR YOU :)", MB_OK);
   //m_pDump->OpenFile();
	CRefTime agnusStreamTimeObject;
	StreamTime(agnusStreamTimeObject);
	long millisekunder = agnusStreamTimeObject.Millisecs();
    return CBaseFilter::Run(tStart);
}


//
//  Definition of CDumpInputPin
//
CDumpInputPin::CDumpInputPin(CDump *pDump,
                             LPUNKNOWN pUnk,
                             CBaseFilter *pFilter,
                             CCritSec *pLock,
                             CCritSec *pReceiveLock,
                             HRESULT *phr) :

    CRenderedInputPin(NAME("CDumpInputPin"),
                  pFilter,                   // Filter
                  pLock,                     // Locking
                  phr,                       // Return code
                  L"Input"),                 // Pin name
    m_pReceiveLock(pReceiveLock),
    m_pDump(pDump),
    m_tLast(0),
	m_vBePrepared(false),
	m_vIsConnected(false)
{
}


//
// CheckMediaType
//
// Check if the pin can support this specific proposed type and format
//
HRESULT CDumpInputPin::CheckMediaType(const CMediaType *pmt)
{
    //VIDEOINFOHEADER *pVideoInfo = (VIDEOINFOHEADER *) pmt->Format();
    if (CanPerformEZrgb24(pmt)) {
		VIDEOINFOHEADER* arneweise = new VIDEOINFOHEADER;
		memcpy(arneweise, (VIDEOINFOHEADER *) pmt->Format(), sizeof(VIDEOINFOHEADER));
		
		if (m_pDump->m_opMediaVideoInfoHeader != NULL)
			delete m_pDump->m_opMediaVideoInfoHeader;

		m_pDump->m_opMediaVideoInfoHeader = arneweise;//(VIDEOINFOHEADER *) pmt->Format();

//		VIDEOINFOHEADER* BrainDamaged = (VIDEOINFOHEADER *) pmt->Format();
//		BrainVideoHeader = (VIDEOINFOHEADER *) pmt->Format();
		return S_OK;//m_pDump->CheckMediaType(pmt);
    }
    return E_FAIL;

}

BOOL CDumpInputPin::CanPerformEZrgb24(const CMediaType *pMediaType) const
{
    if (IsEqualGUID(*pMediaType->Type(), MEDIATYPE_Video)) {
        if (IsEqualGUID(*pMediaType->Subtype(), MEDIASUBTYPE_RGB24)) {
            VIDEOINFOHEADER *pvi = (VIDEOINFOHEADER *) pMediaType->Format();
            return (pvi->bmiHeader.biBitCount == 24);
        }
    }
    return FALSE;

} // CanPerformEZrgb24



STDMETHODIMP CDumpInputPin::QueryInterface(REFIID riid, void ** ppv)
{
	if (riid == IID_IPinConnection)
	{
//		MessageBox(NULL, "We've got an IPinConnection", "Tjoho", MB_OK);
		return GetInterface(static_cast<IPinConnection*>(this), ppv);
	}

	if (riid == IID_IPin)
	{
		return GetInterface(static_cast<IPin*>(this), ppv);		
	}

	return CRenderedInputPin::QueryInterface(riid, ppv);
}

//
// Receive
//
// Do something with this media sample
//
/*STDMETHODIMP CDumpInputPin::Receive(IMediaSample *pSample)
{
    CAutoLock lock(m_pReceiveLock);

	if(!m_vBePrepared)
	{
		m_vBePrepared = true;
	}
	m_pDump->Receive(pSample);
	//m_pDump->ProcessSample(pSample);

	return S_OK;//m_pDump->Write(pbData,pSample->GetActualDataLength());

}*/

STDMETHODIMP CDumpInputPin::Receive(IMediaSample *pSample)
{
//	REFERENCE_TIME vTime;
//	REFERENCE_TIME vStart;
//	REFERENCE_TIME vStop;
	
    CheckPointer(pSample,E_POINTER);
    ValidateReadPtr(pSample,sizeof(IMediaSample));
    ASSERT(pSample);

    HRESULT hr = CheckStreaming();
    if (S_OK != hr) 
	{
        return hr;
    }

    /* Check for IMediaSample2 */
    IMediaSample2 *pSample2;
    if (SUCCEEDED(pSample->QueryInterface(IID_IMediaSample2, (void **)&pSample2))) {
        hr = pSample2->GetProperties(sizeof(m_SampleProps), (PBYTE)&m_SampleProps);
        pSample2->Release();
        if (FAILED(hr)) {
            return hr;
        }
    } else {
        /*  Get the properties the hard way */
        m_SampleProps.cbData = sizeof(m_SampleProps);
        m_SampleProps.dwTypeSpecificFlags = 0;
        m_SampleProps.dwStreamId = AM_STREAM_MEDIA;
        m_SampleProps.dwSampleFlags = 0;
        if (S_OK == pSample->IsDiscontinuity()) {
            m_SampleProps.dwSampleFlags |= AM_SAMPLE_DATADISCONTINUITY;
        }
        if (S_OK == pSample->IsPreroll()) {
            m_SampleProps.dwSampleFlags |= AM_SAMPLE_PREROLL;
        }
        if (S_OK == pSample->IsSyncPoint()) {
            m_SampleProps.dwSampleFlags |= AM_SAMPLE_SPLICEPOINT;
        }
        if (SUCCEEDED(pSample->GetTime(&m_SampleProps.tStart,
                                       &m_SampleProps.tStop))) {
            m_SampleProps.dwSampleFlags |= AM_SAMPLE_TIMEVALID |
                                           AM_SAMPLE_STOPVALID;
        }
        if (S_OK == pSample->GetMediaType(&m_SampleProps.pMediaType)) {
            m_SampleProps.dwSampleFlags |= AM_SAMPLE_TYPECHANGED;
        }
        pSample->GetPointer(&m_SampleProps.pbBuffer);
        m_SampleProps.lActual = pSample->GetActualDataLength();
        m_SampleProps.cbBuffer = pSample->GetSize();
    }

    /* Has the format changed in this sample */
//	IReferenceClock *pClock = m_pClock
	//AM_MEDIA_TYPE* testing = (AM_MEDIA_TYPE*)(CMediaType *)pSample->pMediaType;
	double testing = CurrentRate();
    if (!(m_SampleProps.dwSampleFlags & AM_SAMPLE_TYPECHANGED)) {
		//m_pDump->Receive(pSample);

		m_pDump->ProcessSample(pSample);
		//Sleep(40);
        return NOERROR;
    }

    /* Check the derived class accepts this format */
    /* This shouldn't fail as the source must call QueryAccept first */

    hr = CheckMediaType((CMediaType *)m_SampleProps.pMediaType);

    if (hr == NOERROR) {
	//	m_pDump->ProcessSample(pSample);
        return NOERROR;
    }

    /* Raise a runtime error if we fail the media type */

    m_bRunTimeError = TRUE;
    EndOfStream();
    m_pFilter->NotifyEvent(EC_ERRORABORT,VFW_E_TYPE_NOT_ACCEPTED,0);
    return VFW_E_INVALIDMEDIATYPE;
}


//
// NewSegment
//
// Called when we are seeked
//
STDMETHODIMP CDumpInputPin::NewSegment(REFERENCE_TIME tStart,
                                       REFERENCE_TIME tStop,
                                       double dRate)
{

    m_tLast = 0;
    return S_OK;

} // NewSegment



ULONG CDumpInputPin::AddRef()
{
	return CRenderedInputPin::AddRef();
}

ULONG CDumpInputPin::Release()
{
	return CRenderedInputPin::Release();
}

HRESULT CDumpInputPin::DynamicQueryAccept(const AM_MEDIA_TYPE *pmt)
{
	return S_OK;
}


HRESULT CDumpInputPin::NotifyEndOfStream(HANDLE hNotifyEvent)
{
	return S_FALSE;
}

HRESULT CDumpInputPin::IsEndPin()
{
	return S_OK;
}


HRESULT CDumpInputPin::DynamicDisconnect(void)
{
	return S_OK;
}


STDMETHODIMP CDumpInputPin::ReceiveConnection(IPin * pConnector,const AM_MEDIA_TYPE *pmt)
{
	if (pConnector == m_Connected)
	{
		CheckMediaType((CMediaType*)(pmt));
		//SetMediaType((CMediaType*)(pmt));
		return S_OK;
	}

	return CRenderedInputPin::ReceiveConnection(pConnector, pmt);
}


//
//  CDump class
//

CDump::CDump(LPUNKNOWN pUnk, HRESULT *phr) :
     CBaseRenderer (CLSID_VideoVirtualRenderer, "DUMP", pUnk, phr),
    m_pFilter(NULL),
    m_pPin(NULL),
    m_pPosition(NULL),
    m_hFile(INVALID_HANDLE_VALUE),
    m_pFileName(0),
	m_hSem(CreateSemaphore(NULL, 0, 1, NULL)),
	m_hSemWaitForFrameToArrive(CreateSemaphore(NULL, 0, 1, NULL)),
	m_hSemWaitForSampleCBToFinish(CreateSemaphore(NULL, 0, 1, NULL)),
	timeWasSeeked(false),
	m_vSegmentStart(0),
	m_opOldEMBeBitmapContainer(NULL),
	m_opMediaVideoInfoHeader(NULL)
{
    m_pFilter = new CDumpFilter(this, GetOwner(), &m_Lock, phr);
    if (m_pFilter == NULL) {
        *phr = E_OUTOFMEMORY;
        return;
    }

    m_pPin = new CDumpInputPin(this,GetOwner(),
                               m_pFilter,
                               &m_Lock,
                               &m_ReceiveLock,
                               phr);
    if (m_pPin == NULL) {
        *phr = E_OUTOFMEMORY;
        return;
    }

	makabert = false;

}


HRESULT CDump::DoRenderSample(IMediaSample *pMediaSample)
{

//	SampleCB(pMediaSample);
	ProcessSample(pMediaSample);
    return S_OK;//m_DrawImage.DrawImage(pMediaSample);

} // DoRenderSample

HRESULT CDump::CheckMediaType(const CMediaType *pmtIn)
{


    return m_Display.CheckMediaType(pmtIn);

} // CheckMediaType



// Destructor

CDump::~CDump()
{
	if (m_opMediaVideoInfoHeader != NULL)
		delete m_opMediaVideoInfoHeader;

    delete m_pPin;
    delete m_pFilter;
    delete m_pPosition;
    delete m_pFileName;
}


//
// CreateInstance
//
// Provide the way for COM to create a dump filter
//
CUnknown * WINAPI CDump::CreateInstance(LPUNKNOWN punk, HRESULT *phr)
{
    CDump *pNewObject = new CDump(punk, phr);
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
STDMETHODIMP CDump::NonDelegatingQueryInterface(REFIID riid, void ** ppv)
{
    CheckPointer(ppv,E_POINTER);
    CAutoLock lock(&m_Lock);
	int kalle = 1;
    if (riid == IID_VideoVirtualRenderer)
		return GetInterface((EMWinVideoVirtualRendererFilter*) this, ppv);

	if (riid == IID_IBaseFilter || riid == IID_IMediaFilter || riid == IID_IPersist)
	{
		return m_pFilter->NonDelegatingQueryInterface(riid, ppv);
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

/*
HRESULT CDump::Receive(IMediaSample *pSample)
{
//	return CBaseRenderer::Receive(pSample);
	REFERENCE_TIME pTimeReceiveStart;
	REFERENCE_TIME pTimeReceiveStop;

	ASSERT(pSample);
	pSample -> GetTime(&pTimeReceiveStart, &pTimeReceiveStop);

    // It may return VFW_E_SAMPLE_REJECTED code to say don't bother

    HRESULT hr = PrepareReceive(pSample);
    ASSERT(m_bInReceive == SUCCEEDED(hr));
    if (FAILED(hr)) {
	if (hr == VFW_E_SAMPLE_REJECTED) {
	    return NOERROR;
	}
	return hr;
    }

    // We realize the palette in "PrepareRender()" so we have to give away the
    // filter lock here.
    if (m_State == State_Paused) {
	PrepareRender();
	// no need to use InterlockedExchange
	m_bInReceive = FALSE;
	{
	    // We must hold both these locks
	    CAutoLock cRendererLock(&m_InterfaceLock);
	    if (m_State == State_Stopped)
		return NOERROR;
	    m_bInReceive = TRUE;
	    CAutoLock cSampleLock(&m_RendererLock);
	    OnReceiveFirstSample(pSample);
	}
	Ready();
    }
    // Having set an advise link with the clock we sit and wait. We may be
    // awoken by the clock firing or by a state change. The rendering call
    // will lock the critical section and check we can still render the data

    hr = WaitForRenderTime();
    if (FAILED(hr)) {
	m_bInReceive = FALSE;
	return NOERROR;
    }

//	MessageBox(NULL, "Doing SampleCB in receive", "doing SampleCB", MB_OK);

    PrepareRender();

    //  Set this here and poll it until we work out the locking correctly
    //  It can't be right that the streaming stuff grabs the interface
    //  lock - after all we want to be able to wait for this stuff
    //  to complete
    m_bInReceive = FALSE;

    // We must hold both these locks
    CAutoLock cRendererLock(&m_InterfaceLock);

    // since we gave away the filter wide lock, the sate of the filter could
    // have chnaged to Stopped
    if (m_State == State_Stopped)
	return NOERROR;

    CAutoLock cSampleLock(&m_RendererLock);

    // Deal with this sample
//	m_pPosition -> RegisterMediaTime(pSample);
    //Render(pSample);//m_pMediaSample);
	ProcessSample(pSample);
    ClearPendingSample();
    SendEndOfStream();
    CancelNotification();
    return NOERROR;

}*/

HRESULT CDump::SetMediaType(const CMediaType *pmt)
{
	m_opMediaType = (AM_MEDIA_TYPE*)pmt;
    return NOERROR;
}

void CDump::ProcessSample(IMediaSample *pSample)
{
try
{
	
/*    HRESULT hr = WaitForRenderTime();
    if (FAILED(hr))
	{
		m_bInReceive = FALSE;
 		MessageBox(NULL, "ERROR WAITING FOR SAMPLE", "WAITING ERROR", MB_OK);
    }
*/
	if(m_opOldEMBeBitmapContainer != NULL)
		delete m_opOldEMBeBitmapContainer;

//	REFERENCE_TIME vTime;
	REFERENCE_TIME pTimeStart;
	REFERENCE_TIME pTimeStop;

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


	VIDEOINFOHEADER *pVideoHeader = m_opMediaVideoInfoHeader;//(VIDEOINFOHEADER*)m_opMediaType->pbFormat;

	// The video header contains the bitmap information. 
	// Copy it into a BITMAPINFO structure.
	BITMAPINFO BitmapInfo;
	ZeroMemory(&BitmapInfo, sizeof(BitmapInfo));
	CopyMemory(&BitmapInfo.bmiHeader, &(pVideoHeader->bmiHeader), sizeof(BITMAPINFOHEADER));
	BitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	// Create a DIB from the bitmap header, and get a pointer to the buffer.
	m_oBitmap = CreateDIBSection(0, &BitmapInfo, DIB_RGB_COLORS, &m_opBuffer, NULL, 0);	


	PBYTE  opDataPointer;
	pSample->GetPointer(&opDataPointer);
	memcpy(m_opBuffer, opDataPointer,pSample->GetSize());

	//EMBitmap* opEMBeBitmapContainer = new EMWinBitmap(m_oBitmap);
	m_opNewEMBeBitmapContainer = EM_new EMWinBitmap(m_oBitmap);
//	delete m_opNewEMBeBitmapContainer;

	//EMVideoPreview* m_opPreviewWindow = static_cast<EMVideoPreview*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_VIDEO_PREVIEW));	
//	MessageBox(NULL, "Just about to render the image to preview window", "MESSAGE", MB_OK);
	
	m_opVideoPreview -> Lock();

	m_opVideoPreview -> DrawBitmap(m_opNewEMBeBitmapContainer);//opEMBeBitmapContainer);
//	EMMediaProject* arne = EMMediaEngine::Instance() -> GetMediaProject();//->UnlockVideo();//EMMediaProject::UnlockVideo();
//	arne->LoadData(NULL);
	m_opOldEMBeBitmapContainer = m_opNewEMBeBitmapContainer;
	m_opVideoPreview -> Unlock();
}
catch(...)
{
	EMDebugger("ERROR! Exception while sending bitmap to GUI!");
}
}


STDMETHODIMP CDump::InitializeRenderer(EMVideoPreview* p_opVideoPreview)
{
	m_opVideoPreview = p_opVideoPreview;
//	MessageBox(NULL, "VideoPreview was sent to videovirtualrenderer", "Initialized", MB_OK);
	return S_OK;
}

//Alla metoderna i interfacet avklarade .. börja med resterande implementationer

#endif