#include "EMGlobals.h"
#ifdef PLATFORM_WINDOWS

//------------------------------------------------------------------------------
// File: Dump.h
//
// Desc: DirectShow sample code - definitions for dump renderer.
//
// Copyright (c) 1992 - 2000, Microsoft Corporation.  All rights reserved.
//------------------------------------------------------------------------------


#include "EMWinVideoVirtualRendererFilter.h"
#include "EMVideoPreview.h"
class CDumpInputPin;
class CDump;
class CDumpFilter;

#define BYTES_PER_LINE 20
#define FIRST_HALF_LINE "   %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x"
#define SECOND_HALF_LINE " %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x"


// Main filter object

class CDumpFilter : public CBaseFilter
{
    CDump * const m_pDump;

public:

    // Constructor
    CDumpFilter(CDump *pDump,
                LPUNKNOWN pUnk,
                CCritSec *pLock,
                HRESULT *phr);

    // Pin enumeration
    CBasePin * GetPin(int n);
    int GetPinCount();

    // Open and close the file as necessary
    STDMETHODIMP Run(REFERENCE_TIME tStart);
    STDMETHODIMP Pause();
    STDMETHODIMP Stop();
};


//  Pin object

class CDumpInputPin : public CRenderedInputPin, public IPinConnection
{

public:

    CDumpInputPin(CDump *pDump,
                  LPUNKNOWN pUnk,
                  CBaseFilter *pFilter,
                  CCritSec *pLock,
                  CCritSec *pReceiveLock,
                  HRESULT *phr);

	STDMETHODIMP QueryInterface(REFIID riid, void ** ppv);


    // Do something with this media sample
    STDMETHODIMP Receive(IMediaSample *pSample);
    // Check if the pin can support this specific proposed type and format
    HRESULT CheckMediaType(const CMediaType *pmt);
	BOOL CanPerformEZrgb24(const CMediaType *pMediaType) const;
/*  STDMETHODIMP EndOfStream(void);
    STDMETHODIMP ReceiveCanBlock();
    HRESULT WriteStringInfo(IMediaSample *pSample);


    // Break connection
    HRESULT BreakConnect();
*/
    // Track NewSegment
    STDMETHODIMP NewSegment(REFERENCE_TIME tStart,
                            REFERENCE_TIME tStop,
                            double dRate);


	ULONG __stdcall AddRef();
	ULONG __stdcall Release();
	HRESULT __stdcall DynamicQueryAccept(const AM_MEDIA_TYPE *pmt);
	HRESULT __stdcall NotifyEndOfStream(HANDLE hNotifyEvent);
	HRESULT __stdcall IsEndPin();
	HRESULT __stdcall DynamicDisconnect(void);

    STDMETHODIMP ReceiveConnection(IPin * pConnector,const AM_MEDIA_TYPE *pmt);

private:
    CDump*	const m_pDump;           // Main renderer object
    CCritSec * const m_pReceiveLock;    // Sample critical section
    REFERENCE_TIME m_tLast;             // Last sample receive time
	VIDEOINFOHEADER *BrainVideoHeader;
	bool m_vBePrepared;

	bool m_vIsConnected;

};


//  CDump object which has filter and pin members

class CDump : public EMWinVideoVirtualRendererFilter, public CBaseRenderer//, public IFileSinkFilter
{
    friend class CDumpFilter;
    friend class CDumpInputPin;
    CDumpFilter *m_pFilter;         // Methods for filter interfaces
    CDumpInputPin *m_pPin;          // A simple rendered input pin
    CCritSec m_Lock;                // Main renderer critical section
    CCritSec m_ReceiveLock;         // Sublock for received samples
    CRendererPosPassThru *m_pPosition;  // Renderer position controls
    HANDLE m_hFile;                 // Handle to file for dumping
    LPOLESTR m_pFileName;           // The filename where we dump to
    HRESULT CheckMediaType(const CMediaType *pmtIn);
    HRESULT DoRenderSample(IMediaSample *pMediaSample);
//	HRESULT Receive(IMediaSample *pSample);
	VIDEOINFOHEADER* m_opMediaVideoInfoHeader;


public:

	CImageDisplay m_Display;

    DECLARE_IUNKNOWN
    CDump(LPUNKNOWN pUnk, HRESULT *phr);
    ~CDump();

	HRESULT SetMediaType(const CMediaType *pmt);
    static CUnknown * WINAPI CreateInstance(LPUNKNOWN punk, HRESULT *phr);
	void ProcessSample(IMediaSample *pSample);

	STDMETHODIMP InitializeRenderer(EMVideoPreview* p_opVideoPreview);
private:

    // Overriden to say what interfaces we support where
    STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void ** ppv);


private:
	const HANDLE	m_hSem;
	const HANDLE	m_hSemWaitForFrameToArrive;
	const HANDLE	m_hSemWaitForSampleCBToFinish;
	AM_MEDIA_TYPE* m_opMediaType;
	bool makabert;
	void* m_opBuffer;
	HBITMAP m_oBitmap;
	REFERENCE_TIME m_vSampleTime;
	IMediaSeeking* m_opSeek;
	IMediaControl*	m_opMediaControl;
	bool timeWasSeeked;
	LONGLONG m_vDuration;
	REFERENCE_TIME m_vSegmentStart;
	EMVideoPreview* m_opVideoPreview;
	EMBitmap* m_opOldEMBeBitmapContainer;
	EMBitmap* m_opNewEMBeBitmapContainer;
};


#endif