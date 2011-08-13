//------------------------------------------------------------------------------
// File: Dump.h
//
// Desc: DirectShow sample code - definitions for dump renderer.
//
// Copyright (c) 1992 - 2000, Microsoft Corporation.  All rights reserved.
//------------------------------------------------------------------------------

//#include "EMReadFrameuids.h"

#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#ifndef __EM_READ_FRAME
#define __EM_READ_FRAME

#include "EMReadFrameInterface.h"
#include "EMScalingUtils.h"
#include "EMMediaDataBuffer.h"

class EMReadFrameInputPin;
class EMReadFrame;
class EMReadFrameFilter;
class EMMediaFormat;

#define BYTES_PER_LINE 20
#define FIRST_HALF_LINE "   %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x"
#define SECOND_HALF_LINE " %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x"

// Main filter object
DEFINE_GUID(CLSID_EMReadFrame, 0x36a5f770, 0xfe4c, 0x11ce, 0xa8, 0xed, 0x00, 0xaa, 0x00, 0x2f, 0xea, 0xb5);
DEFINE_GUID(IID_EMReadFrameInterface, 0xfe75b1e1, 0x3821, 0x11d5, 0x82, 0x59, 0x0, 0x2, 0xb3, 0x18, 0x0, 0x46);

class EMReadFrameFilter : public CBaseFilter
{
    EMReadFrame * const m_pDump;

public:

    // Constructor
    EMReadFrameFilter(EMReadFrame *pDump,
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

class EMReadFrameInputPin : public CRenderedInputPin
{
    EMReadFrame    * const m_pDump;           // Main renderer object
    CCritSec * const m_pReceiveLock;    // Sample critical section
    REFERENCE_TIME m_tLast;             // Last sample receive time

public:

    EMReadFrameInputPin(EMReadFrame *pDump,
                  LPUNKNOWN pUnk,
                  CBaseFilter *pFilter,
                  CCritSec *pLock,
                  CCritSec *pReceiveLock,
                  HRESULT *phr);

    // Do something with this media sample
    STDMETHODIMP Receive(IMediaSample *pSample);
    // Check if the pin can support this specific proposed type and format
    HRESULT CheckMediaType(const CMediaType *pmt);
	HRESULT SetMediaType(const CMediaType *pmt);
	BOOL CanPerformEZrgb24(const CMediaType *pMediaType) const;

	STDMETHODIMP EndOfStream(void);


    // Track NewSegment
    STDMETHODIMP NewSegment(REFERENCE_TIME tStart,
                            REFERENCE_TIME tStop,
                            double dRate);

};


//  EMReadFrame object which has filter and pin members

class EMReadFrame : public EMReadFrameInterface, public CBaseRenderer//, public IFileSinkFilter
{
    friend class EMReadFrameFilter;
    friend class EMReadFrameInputPin;

    EMReadFrameFilter *m_pFilter;         // Methods for filter interfaces
    EMReadFrameInputPin *m_pPin;          // A simple rendered input pin
    CCritSec m_Lock;                // Main renderer critical section
    CCritSec m_ReceiveLock;         // Sublock for received samples

    HRESULT CheckMediaType(const CMediaType *pmtIn);
    HRESULT DoRenderSample(IMediaSample *pMediaSample);
	HRESULT Receive(IMediaSample *pSample);

public:
	CImageDisplay m_Display;
    DECLARE_IUNKNOWN
    EMReadFrame(LPUNKNOWN pUnk, HRESULT *phr);
    ~EMReadFrame();

	HRESULT SetMediaType(const CMediaType *pmt);
    static CUnknown * WINAPI CreateInstance(LPUNKNOWN punk, HRESULT *phr);

	//STDMETHODIMP GetBuffer(IMediaSample** p_opSample);
	STDMETHODIMP SampleCB(IMediaSample *p_opSample);
	//STDMETHODIMP ReleaseBuffer();

	//STDMETHODIMP GrabFrame(void* p_opBuffer, REFERENCE_TIME& p_vSampleTime);//, HBITMAP& p_opBitmap);

	STDMETHODIMP GrabFrame(EMMediaDataBuffer *p_opData, REFERENCE_TIME& p_vSampleTime);

	STDMETHODIMP SeekToFrame(REFERENCE_TIME p_vFrameNumber);
	STDMETHODIMP SeekToTime(REFERENCE_TIME p_vSeekTime);
	STDMETHODIMP SetOutputFormat(long p_vDestSizeX, long p_vDestSizeY, long p_vSkipSize);
	STDMETHODIMP StopGraph();
	STDMETHODIMP InitializeRenderer(AM_MEDIA_TYPE* p_opMediaType, IMediaSeeking* p_opSeek, IMediaControl*	p_opMediaControl);
	STDMETHODIMP UseScaling(bool p_vScale);
private:

    // Overriden to say what interfaces we support where
	STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void ** ppv);
	STDMETHODIMP SetOutputFormat(EMMediaFormat* m_opFormat);


private:
	EMScalingUtils	m_oScale;

	EMMediaFormat* m_opSource;

	long	m_vSourceSizeX;
	long	m_vSourceSizeY;
	long	m_vDestinationSizeX;
	long	m_vDestinationSizeY;
	long	m_vBytesPerSourcePixel;
	long	m_vBytesPerDestinationPixel;
	long	m_vSourceSkipSize;
	long	m_vBytesPerFrame;

	HANDLE	m_hSem;
	HANDLE	m_hSemWaitForFrameToArrive;
	HANDLE	m_hSemWaitForSampleCBToFinish;
	HANDLE	m_hSemWaitBeforeSeekOrGrab;
	//EVENT HANDLES
	/*HANDLE m_oEventWaitingForSampleCB;
	HANDLE m_oEventWaitingForGrab;
	HANDLE m_oEventSeekWaitingForGrab;
	HANDLE m_oEventGrabWaitingForSeek;*/

	HANDLE m_oSemWaitingForSampleCB;
	HANDLE m_oSemWaitingForGrab;
	HANDLE m_oSemSeekWaitingForGrab;
	HANDLE m_oSemGrabWaitingForSeek;

	//END EVENT HANDLES
	AM_MEDIA_TYPE* m_opMediaType;

	//bool makabert;
	bool m_vSeeking;
	bool m_vFrameValid;
	bool m_vFrameValid2;
	void* m_opBuffer;
	bool m_vIsRunning;
	bool m_vScale;
	bool m_vEndOfTheLine;
	bool m_vCacheClean;

	REFERENCE_TIME m_vSampleTime;
	REFERENCE_TIME m_vSegmentStart;

	IMediaSeeking* m_opSeek;
	IMediaControl* m_opMediaControl;

	LONGLONG m_vDuration;
	int32 m_vNumberOfTimesReleasedSemaphores;
	int32 m_vNumberOfTimesUsedSemaphores;
	bool SizeHasChanged(EMMediaFormat *p_opData);
	int32 m_vWaitForSampleCBToFinishCount;
	int32 m_vSemCount;
	int32 m_vWaitForFrameToArriveCount;
	int32 m_vWaitBeforeSeekOrGrab;

};

#endif

#endif