
#include "EMWinGrab.h"
#include <stdio.h>

EMWinGrab::EMWinGrab(AM_MEDIA_TYPE* p_opMediaType, IMediaSeeking* p_opSeek, IMediaControl*	p_opMediaControl) : CUnknown("SGCB", NULL), m_hSem(CreateSemaphore(NULL, 0, 1, NULL)),
 m_hSemWaitForFrameToArrive(CreateSemaphore(NULL, 0, 1, NULL)), m_hSemWaitForSampleCBToFinish(CreateSemaphore(NULL, 0, 1, NULL))
{
	IHaveProcessed = 0;
	HasCollected = FALSE;
	m_opMediaType = p_opMediaType;
	m_opSeek = p_opSeek;
	m_opMediaControl = p_opMediaControl;
	makabert = false;
	MessageBox(NULL, "EMWinGrab Object Created", "Message for Martin", MB_OK);

}

EMWinGrab::~EMWinGrab()
{
	ReleaseSemaphore(m_hSemWaitForFrameToArrive, 1, NULL);

}

// ISampleGrabberCB methods
STDMETHODIMP EMWinGrab::SampleCB(double p_vSampleTime, IMediaSample *p_opSample)
{
	REFERENCE_TIME pTimeStart;
	REFERENCE_TIME pTimeStop;
	HRESULT hr;
	MessageBox(NULL, "Went into SampleCB", "Message for Martin", MB_OK);

	if(!makabert)
	{
		ReleaseSemaphore(m_hSem, 1, NULL);

		if(p_opSample -> GetTime(&pTimeStart, &pTimeStop) != S_OK)
			MessageBox(NULL, "Couldn't get Time Stamp", "ERROR of Some kind", MB_OK);


		PBYTE  opDataPointer;
		hr = p_opSample->GetPointer(&opDataPointer);
		if (FAILED(hr))
		{
			MessageBox(NULL, strerror(hr), "ERROR GetConnectedMediaType", MB_OK);
		}

		// Get a pointer to the video header.
		VIDEOINFOHEADER *pVideoHeader = (VIDEOINFOHEADER*)m_opMediaType->pbFormat;


		// The video header contains the bitmap information. 
		// Copy it into a BITMAPINFO structure.
		BITMAPINFO BitmapInfo;
		ZeroMemory(&BitmapInfo, sizeof(BitmapInfo));
		CopyMemory(&BitmapInfo.bmiHeader, &(pVideoHeader->bmiHeader), sizeof(BITMAPINFOHEADER));
		BitmapInfo.bmiHeader.biCompression = BI_RGB;

		//Wait for GrabFrame to execute and release this semaphore
		if(!makabert)
		{
			WaitForSingleObject(m_hSemWaitForFrameToArrive, INFINITE);

			// Create a DIB from the bitmap header, and get a pointer to the buffer. (IS this line neccessary?? Maybe NOT??!)
			//m_oBitmap = CreateDIBSection(NULL, &BitmapInfo, DIB_RGB_COLORS, &m_opBuffer, NULL, NULL);	

			//Copy the data into our buffer
			memcpy(m_opBuffer, opDataPointer,p_opSample->GetSize());

			/*	char myNumber[20];
			_itoa(p_vSampleTime, myNumber, 10);
			MessageBox(NULL, "Received One Buffer", myNumber, MB_OK);
			*/
			m_vSampleTime = p_vSampleTime;
			ReleaseSemaphore(m_hSemWaitForSampleCBToFinish, 1, NULL);
		}
		else
		{
			//MessageBox(NULL, "MAKABERT", "MAKABAERT", MB_OK);
		}
	}
	else
	{
	}
		//MessageBox(NULL, "MAKABERT2", "MAKABAERT2", MB_OK);
	return S_OK;

}

STDMETHODIMP EMWinGrab::BufferCB(double p_vSampleTime, BYTE *p_pBuffer, long p_vBufferLen)
{
	return E_NOTIMPL;
}

HBITMAP EMWinGrab::GrabFrame(void* p_opBuffer, __int64& p_vSampleTime)
{
	//Wait for SampleCB to receive a frame
	WaitForSingleObject(m_hSem, INFINITE);

	m_opBuffer = p_opBuffer;
	//Release semaphore so that CB can fill our buffer	
	ReleaseSemaphore(m_hSemWaitForFrameToArrive, 1, NULL);
	WaitForSingleObject(m_hSemWaitForSampleCBToFinish, INFINITE);
	//We wannt to have it in microseconds(GrabFrame return it in seconds)
	p_vSampleTime = static_cast<__int64>(m_vSampleTime * 1000000);
	//sDibsection.dsBm.bmHeight

	//m_oBounds = EMRect(0, 0, sDibsection.dsBm.bmWidth - 1, sDibsection.dsBm.bmHeight - 1);

	return m_oBitmap;
}

void EMWinGrab::SeekToFrame(REFERENCE_TIME p_vFrameNumber)
{

	ReleaseSemaphore(m_hSemWaitForFrameToArrive, 1, NULL);
	WaitForSingleObject(m_hSemWaitForSampleCBToFinish, INFINITE);
	m_opSeek->SetPositions(&p_vFrameNumber, AM_SEEKING_AbsolutePositioning, NULL, AM_SEEKING_NoPositioning);
}

void EMWinGrab::SeekToTime(REFERENCE_TIME p_vSeekTime)
{

	WaitForSingleObject(m_hSem, INFINITE);
	ReleaseSemaphore(m_hSemWaitForFrameToArrive, 1, NULL);
	makabert = true;
	WaitForSingleObject(m_hSemWaitForSampleCBToFinish, INFINITE);
	m_opSeek->SetPositions(&p_vSeekTime, AM_SEEKING_AbsolutePositioning, NULL, AM_SEEKING_NoPositioning);
	makabert = false;

}

void EMWinGrab::WaitForHandling()
{
	while(!HasCollected)
	{
		Sleep(10);
	}
	HasCollected = false;
}

void EMWinGrab::StopGraph()
{
	WaitForSingleObject(m_hSem, INFINITE);
	ReleaseSemaphore(m_hSemWaitForFrameToArrive, 1, NULL);
	makabert = true;
	WaitForSingleObject(m_hSemWaitForSampleCBToFinish, INFINITE);
	m_opMediaControl -> Stop();
	makabert = false;
}

void EMWinGrab::WriteIt()
{
	MessageBox(NULL, "Swing it, swing it", "Message for Martin", MB_OK);

}
