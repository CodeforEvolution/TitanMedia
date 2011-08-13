#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#include "EMWinMediaInputFilterExport.h"
#include "EMWinMediaInputFilterInterface.h"
#include "EMWinMediaInputFilterVersion.h"
#include "EMWinMediaInputFilter.h"
#include "EMWinMediaInputPin.h"
#include "EMWinAudioInputPin.h"
#include "EMWaveFileWriter.h"
#include "EMMediaUtility.h"
#include "EMMediaFormat.h"
#include "EMMediaDataBuffer.h"
#include "EMMediaSignalMeter.h"
#include "EMMediaAudioSignalMeter.h"

#include <math.h>

static const GUID CLSID_EMWinMediaInputFilter = {0x952c35ab, 0x4dcb, 0x11d5, {0xbc, 0x74, 0x0, 0x2, 0xb3, 0x17, 0xf3, 0xf2}};
static const GUID IID_IEMWinMediaInputFilter =  {0x952c35ac, 0x4dcb, 0x11d5, {0xbc, 0x74, 0x0, 0x2, 0xb3, 0x17, 0xf3, 0xf2}};

CFactoryTemplate g_Templates[] = 
{
  	{L"EMWinMediaInputFilter", &CLSID_EMWinMediaInputFilter, EMWinMediaInputFilter::CreateInstance, NULL, &sudEMWinMediaInputFilterRegs}
};
int g_cTemplates = 1;

CUnknown* WINAPI EMWinMediaInputFilter::CreateInstance(LPUNKNOWN pUnk, HRESULT *phr)
{
	CUnknown *punk = EM_new EMWinMediaInputFilter(pUnk, phr);
	if(punk == NULL) 
	{
		*phr = E_OUTOFMEMORY;
	}
    return punk;
}

EMWinMediaInputFilter::EMWinMediaInputFilter(LPUNKNOWN pUnk, HRESULT *phr)
	:	EMDirectXFilter(NAME("EMWinMediaInputFilter"), pUnk, CLSID_EMWinMediaInputFilter),
		m_opWaveFile(NULL),
		m_opMeter(NULL),
		m_vIsSuspended(true),
		m_opBuffer(NULL),
		m_vStartTime(-1)
{
	m_opPin = EMWinMediaInputPin::Create(EM_TYPE_RAW_AUDIO, this);

	m_opBuffer = EM_new EMMediaDataBuffer(EM_TYPE_RAW_AUDIO, 0, NULL);
}

EMWinMediaInputFilter::~EMWinMediaInputFilter()
{
	if(m_opBuffer != NULL)
		delete m_opBuffer;

	delete m_opPin;
}

STDMETHODIMP EMWinMediaInputFilter::NonDelegatingQueryInterface(REFIID riid, void **ppv)
{
	if(riid == IID_IEMWinMediaInputFilter)
	{
		eo << "MediaInputFilter Library v0.1Win, Build " << __EMWinMediaInputFilterVersion << " by " << __EMWinMediaInputFilterVersion_Owner << " (C) Elegant Media AB 2000-2001" << ef;
        return GetInterface((EMWinMediaInputFilterInterface*) this, ppv);
    } 
	else 
	{
        return CBaseFilter::NonDelegatingQueryInterface(riid, ppv);
    }
}

CBasePin * EMWinMediaInputFilter::GetPin(int n)
{
	return m_opPin;
}

int EMWinMediaInputFilter::GetPinCount()
{
	return 1;
}

STDMETHODIMP EMWinMediaInputFilter::Run(REFERENCE_TIME tStart)
{
	return CBaseFilter::Run(tStart);
}

STDMETHODIMP EMWinMediaInputFilter::Pause()
{
	HRESULT hr = CBaseFilter::Pause();
	if(FAILED(hr))
		return hr;
//	if(m_opPin -> OnEndOfStream())
//		return S_OK;
	return S_OK;
}

STDMETHODIMP EMWinMediaInputFilter::Stop()
{
	HRESULT hr = CBaseFilter::Stop();
	if(FAILED(hr))
		return hr;
	return S_OK;
}

STDMETHODIMP EMWinMediaInputFilter::SetTargetFile(string p_oFileName)
{
	if(m_opWaveFile != NULL)
		delete m_opWaveFile;
	m_opWaveFile = EM_new EMWaveFileWriter(p_oFileName.c_str());
	if(! m_opWaveFile -> InitCheckE())
		return S_OK;
	return S_OK;
}

STDMETHODIMP EMWinMediaInputFilter::CloseTargetFile()
{
	if(m_opWaveFile != NULL)
	{
		m_opWaveFile -> WriteHeaderE(EMMediaUtility::Instance() -> GetSystemAudioFormat());
		delete m_opWaveFile;
		m_opWaveFile = NULL;
	}
	return S_OK;
}

STDMETHODIMP EMWinMediaInputFilter::StartOnTime(int64 p_vStartTime)
{
	m_vStartTime = p_vStartTime;
	return true;
}

STDMETHODIMP EMWinMediaInputFilter::Suspend(bool p_vSuspended)
{
	m_vIsSuspended = p_vSuspended;
	return m_vIsSuspended;
}

STDMETHODIMP EMWinMediaInputFilter::SetSignalMeter(EMMediaSignalMeter* p_opMeter)
{
	m_opMeter = p_opMeter;
	return true;
}

bool EMWinMediaInputFilter::BufferReceived(IMediaSample* p_opMediaSample)
{
	BYTE* vpData = NULL;
	HRESULT hr = p_opMediaSample -> GetPointer(&vpData);
	if(FAILED(hr))
		;//eo << "ERROR! Couldnt get data pointer from IMediaSample!" << ef;
	else
	{
		int64 vStart;
		int64 vStop;

		int64 vStreamStart;
		int64 vStreamStop;

		p_opMediaSample -> GetMediaTime(&vStart, &vStop);
		p_opMediaSample -> GetTime(&vStreamStart, &vStreamStop);

		int64 vStartTime = 1000000.0 * ((double)vStart) / 44100.0;
		int64 vStopTime = 1000000.0 * ((double)vStop) / 44100.0;

		//eo << "EMWinMediaInputFilter::BufferReceived - " << vStart << " - " << vStop << ef;
		//eo << "EMWinMediaInputFilter::BufferReceived - " << vStreamStart/10 << " - " << vStreamStop/10 << ef;


		//eo << "EMWinMediaInputFilter::BufferReceived - has: " << vStartTime << " - wants: " << m_vStartTime << ef;

		if(m_opWaveFile != NULL && !m_vIsSuspended /*&& vStartTime <= m_vStartTime && m_vStartTime != -1*/)
		{
			/*if(m_vStartTime < vStop)
				vpData += 4 * (m_vStartTime - vStart) * 44100 / 10000000;*/

			m_opWaveFile -> WriteData((void*) vpData, p_opMediaSample -> GetActualDataLength());
		}

		if(m_opMeter != NULL)
		{
			list<EMMediaDataBuffer*> oList;
			oList.push_front(m_opBuffer);

			m_opBuffer -> m_opDestination = NULL;
			m_opBuffer -> SetDataPointer((void*)vpData);
			m_opBuffer -> m_vSizeAvailable = p_opMediaSample -> GetActualDataLength();
			m_opBuffer -> m_vSizeUsed = m_opBuffer -> m_vSizeAvailable;

			m_opMeter -> ProcessBufferE(&oList);

			long vLen =  p_opMediaSample -> GetActualDataLength();

			float vTmp = ((float)(vStreamStop - vStreamStart)) / 10000000.0f;

			int16* vpSrc = (int16*)vpData;
			int16 vDummyL = 0;
			int16 vDummyR = 0;
			for(int i = 0; i < vLen / 2; i += 2)
			{
				 vDummyL = max(abs(vpSrc[i]), vDummyL);
				 vDummyR = max(abs(vpSrc[i + 1]), vDummyR);
			}

			//eo << "Noise lvl2: " << vDummyL << " | " << vDummyR << " Bytes used:" << vLen << " dT: " << vTmp << ef;


		}

//		else
//			eo << "Cannot write data to disk, since the file writer is NULL!" << ef;

//		int64 start, end;
//		p_opMediaSample -> GetMediaTime(&start, &end);
//		EMMediaTimer::Instance() -> IncreaseNowFrame(EMBeMediaUtility::BytesToFrames(p_opBuffer -> SizeUsed(), EMBeMediaUtility::GetSystemAudioFormat()));
//		p_opMediaSample -> Release();
	}
	return true;
}

#endif