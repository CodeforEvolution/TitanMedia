#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#include <dmusicc.h>

#include "EMWinMediaOutputPin.h"
#include "EMMediaDataBuffer.h"
#include "EMWinMediaOutputFilterInterface.h"
#include "EMWinMediaOutputFilter.h"
#include "EMMediaTimer.h"
#include "EMMediaUtility.h"
#include "EMSettingsRepository.h"
#include "EMSettingIDs.h"
#include "EMMediaFormat.h"
//#include "EMWinAudioOutputPin.h"
#include "EMWinVideoOutputPin.h"

//#include <uuids.h>
#include <math.h>

EMWinMediaOutputPin* EMWinMediaOutputPin::Create(EMMediaType p_eType, CDynamicSource* p_opParentFilter)
{
	HRESULT vResult = S_OK;
	if((p_eType & EM_TYPE_RAW_AUDIO) > 0)
		return NULL; //EM_new EMWinAudioOutputPin(&vResult, p_opParentFilter);
	else if((p_eType & EM_TYPE_RAW_VIDEO) > 0)
		return EM_new EMWinVideoOutputPin(&vResult, p_opParentFilter);
	else
		return NULL;
}

EMWinMediaOutputPin::EMWinMediaOutputPin(HRESULT *phr, CDynamicSource* p_opParentFilter, LPCWSTR pName)
	:	CDynamicSourceStream(NAME("Debug name"), phr, p_opParentFilter, pName),
		m_opFilter(((EMWinMediaOutputFilter*) p_opParentFilter)),
		m_opConnectedMediaType(NULL),
		m_vMediaTimeStart(0),
		m_vTimeToShutdown(false),
		m_oTimerEvent(CreateEvent(NULL, false, false, NULL)),
		m_vDiscont(FALSE),
		m_vIsClockMaster(false)
{
}

EMWinMediaOutputPin::~EMWinMediaOutputPin()
{
	m_vTimeToShutdown = true;
	CloseHandle(m_oTimerEvent);
}

HRESULT EMWinMediaOutputPin::GetMediaType(CMediaType* p_opMediaTypeObject)
{
	if(! ReportFormat(p_opMediaTypeObject))
		return E_FAIL;
	return S_OK;
}

HRESULT EMWinMediaOutputPin::DecideBufferSize(IMemAllocator *pAlloc, ALLOCATOR_PROPERTIES *pProperties)
{
//	ASSERT(CritCheckIn(&m_cSharedState));

	pProperties -> cBuffers = ReportNumBuffers();
	pProperties -> cbBuffer = ReportBufferSize();

    ALLOCATOR_PROPERTIES Actual;
    HRESULT hr = pAlloc -> SetProperties(pProperties, &Actual);
    if(FAILED(hr)) 
	{
		eo << "   DirectX -> ERROR! Couldn't set properties for allocator!" << ef;
        return hr;
    }

	if(Actual.cbBuffer < pProperties -> cbBuffer) 
	{
		eo << "   DirectX -> ERROR! Properties set doesn't match what I want!" << ef;
        return E_FAIL;
    }

	return NOERROR;
}

HRESULT EMWinMediaOutputPin::CompleteConnect(IPin *pReceivePin)
{
	CAutoLock lShared(&m_cSharedState);
	m_fFirstSampleDelivered = FALSE;
	HRESULT h = CDynamicSourceStream::CompleteConnect(pReceivePin);
	return h;
}

HRESULT EMWinMediaOutputPin::Active(void)
{
	CAutoLock lShared(&m_cSharedState);

	if(m_opFilter -> IsActive())
		return S_FALSE;

	HRESULT hr = CDynamicSourceStream::Active();
	
	if(hr != S_OK)
	{
		eo << "   DirectX -> ERROR! Filter parent call failed!" << ef;
		return hr;
	}

	m_vSongTime = 0;
	m_fFirstSampleDelivered = false;

	if(! OnActive())
		return E_FAIL;
	return NOERROR;
}

HRESULT EMWinMediaOutputPin::Inactive(void)
{
	CAutoLock lShared(&m_cSharedState);

	m_vTimeToShutdown = true;

	HRESULT vResult = Exit();
	if(vResult != S_OK)
	{
		EMDebugger("ERROR! Couldn't exit the Dynamic Source Stream thread!");
		return vResult;
	}

	vResult = CDynamicSourceStream::Inactive();
	if(vResult != S_OK)
	{
		EMDebugger("ERROR! Couldn't call Inactive on Dynamic Source Stream!");
		return vResult;
	}

	return NOERROR;
}

HRESULT EMWinMediaOutputPin::BreakConnect(void)
{
	CAutoLock lShared(&m_cSharedState);
	m_vTimeToShutdown = true;
	HRESULT vResult = CDynamicSourceStream::BreakConnect();
	if(FAILED(vResult))
		return vResult;
	if(! OnDisconnect())
		return E_FAIL;
	return NOERROR;
}

STDMETHODIMP EMWinMediaOutputPin::NonDelegatingQueryInterface(REFIID riid, void **ppv)
{
	return CDynamicSourceStream::NonDelegatingQueryInterface(riid, ppv);
}

IMediaSample* EMWinMediaOutputPin::ConvertFromBufferToSample(EMMediaDataBuffer* p_opFromEMMediaDataBuffer)
{
	IMediaSample* p_opToIMediaSample = RequestBuffer();
	if(p_opToIMediaSample != NULL)
	{
		BYTE* vpData = NULL;
		HRESULT vResult = p_opToIMediaSample -> GetPointer(&vpData);
		if(vResult != S_OK)
			return NULL;
		int64 vSizeOfBuffer = p_opToIMediaSample -> GetSize();
		if((p_opToIMediaSample -> GetSize()) < (p_opFromEMMediaDataBuffer -> m_vSizeAvailable))
		{
			eo << "ERROR! MediaSample too small. Can't hold the data from the MediaDataBuffer!" << ef;
			return NULL;
		}
		else
		{
			p_opToIMediaSample -> SetActualDataLength(p_opFromEMMediaDataBuffer -> m_vSizeUsed);
			memcpy(vpData, p_opFromEMMediaDataBuffer -> Data(), p_opFromEMMediaDataBuffer -> m_vSizeUsed);
		}
		return p_opToIMediaSample;
	}
	return NULL;
}

IMediaSample* EMWinMediaOutputPin::RequestBuffer()
{
	IMediaSample* pSample = NULL;

	HRESULT hr = S_OK;
	while((hr = GetDeliveryBuffer(&pSample, NULL, NULL, AM_GBF_NOWAIT)) == VFW_E_TIMEOUT && (! m_vTimeToShutdown))
	{
		if(! EMMediaTimer::Instance() -> IsPlaying())
			break;
		::Sleep(5);
	}
    if(FAILED(hr) || m_vTimeToShutdown) 
	{
		if(! EMMediaTimer::Instance() -> IsPlaying())
			eo << "Timer is not playing!!!!!" << ef;
		else
			eo << "Timer is playing!!!!!" << ef;
		eo << "Error code from GetDeliveryBuffer was " << (int) hr << ef;
		eo << "   DirectX -> 5ERROR! Could not fetch media sample!" << ef;
		return NULL;
	}

	return pSample;
}

HRESULT EMWinMediaOutputPin::FillBuffer(IMediaSample** p_opIMediaSampleOut)
{
	DWORD hEventId;

	CAutoLock lShared(&m_cSharedState);

	
	//Try reconectiom

	EMMediaDataBuffer* opDataBuffer = NULL;
	IMediaSample* pSamp = NULL;
	m_opFilter -> GetBufferFromCache(&pSamp);
	if(pSamp == NULL)
	{
//		eo << "   DirectX -> Internal error! Failed to get IMediaSample from cache!" << ef;
//		DeliverEndOfStream();
		return E_FAIL;
	}
	
	/*		Time stamps used:

			Song time (0 equals actual start of playback)
				Start (first song time unit included in the playback-duration of the buffer)
				Stop (last song time unit included in the playback-duration of the buffer)

				Next equals Stop +1

	
			Media time (0 equals the actual position (in time units) inside the media data)
				Start (first media time unit included in the playback-duration of the buffer)
				Stop (last song time unit included in the playback-duration of the buffer)

				Next equals Stop +1
	*/

	//TODO: Make format-independent!! !PS NOW IT IS FORMAT INDEPENDET !!! :))) ??? says Jesper 01-05-23
	if((ReportFormatType() & EM_TYPE_RAW_AUDIO) > 0)
	{
		EMMediaFormat opFormat(EM_TYPE_RAW_AUDIO);
		opFormat.CreateAudioFormat((AM_MEDIA_TYPE*) ((CMediaType*) (m_mt.Format())));

		//Fetch the number of bytes in the buffer
		uint64 vNumberOfBytesInBuffer = pSamp -> GetActualDataLength();

		//calculate the number of 100-nanoseconds in the buffer (ie the duration)
		int64 vBufferDurationInt = (UNITS * EMMediaUtility::Instance() -> FramesToTime(EMMediaUtility::Instance() -> BytesToFrames(vNumberOfBytesInBuffer, &opFormat), &opFormat)) / 1000000;
		CRefTime vBufferDuration = (REFERENCE_TIME) vBufferDurationInt;

		//set the buffer's end time to be the current song time + the duration
		CRefTime vBufferEndTime = m_vSongTime + vBufferDuration;

		//just copy the old media start time
		//	int64 llMediaTimeStart = m_vMediaTimeStart;

		//the media stop time is the media time start plus the duration.. of course!
		//  int64 llMediaTimeStop = m_vMediaTimeStart + vBufferDurationInt;
	
		/*	HRESULT hr = pSamp -> SetMediaTime(&llMediaTimeStart, &llMediaTimeStop);
	    if(FAILED(hr))
		{
			eo << "ERROR! Couldnt preroll!" << ef;
			return hr;
		}*/
		HRESULT hr = pSamp -> SetTime((REFERENCE_TIME*) &m_vSongTime , (REFERENCE_TIME*) &vBufferEndTime);
		if(FAILED(hr))
		{
			eo << "ERROR! Couldnt set time for buffer!" << ef;
	     return hr;
		}
		hr = pSamp -> SetPreroll(FALSE);
		if(FAILED(hr)) 
		{
			eo << "ERROR! Couldnt preroll!" << ef;
			return hr;
		}
		hr = pSamp -> SetMediaType(NULL);
		if(FAILED(hr)) 
		{
	        return hr;
		}
/*		hr = pSamp -> SetDiscontinuity(! m_fFirstSampleDelivered);
		if(FAILED(hr)) 
		{
	        return hr;
	    }*/
	    hr = pSamp -> SetSyncPoint(! m_fFirstSampleDelivered);
	    if(FAILED(hr)) 
		{
			return hr;
		}
	
		//TODO: Make format-independent!
		m_opFilter -> GetTimer() -> IncreaseNowFrame(EMMediaUtility::Instance() -> BytesToFrames(vNumberOfBytesInBuffer, EMMediaUtility::Instance() -> GetSystemAudioFormat()), -1);
	
		//The next song- and media-times will be the 100-nanosecond after the previous + duration
		m_vSongTime = m_vSongTime + vBufferDuration + 1;
		//	m_vMediaTimeStart = llMediaTimeStop + 1;

		m_fFirstSampleDelivered = TRUE;
		*p_opIMediaSampleOut = pSamp;
	} else 	if((ReportFormatType() & EM_TYPE_RAW_VIDEO) > 0)
	{
	//	EMMediaFormat opFormat(EM_TYPE_RAW_VIDEO);
	//	opFormat.CreateVideoFormat((AM_MEDIA_TYPE*) ((CMediaType*) (m_mt.Format())));
		AM_MEDIA_TYPE* opAMMediaType = ((AM_MEDIA_TYPE*) ((CMediaType*) (m_mt.Format())));
		//AM_MEDIA_TYPE* myFormat = (AM_MEDIA_TYPE*)p_opFormat;
		//m_sVideoFormatStructure = reinterpret_cast<VIDEOINFOHEADER *>( p_opFormat -> AllocFormatBuffer(sizeof(VIDEOINFOHEADER)));

		
		//int64 vBufferDurationInt = static_cast<int64>(opAMMediaType -> lSampleSize);// 6666666;

		CRefTime vBufferDuration = (REFERENCE_TIME) opAMMediaType -> lSampleSize;
//		CRefTime vBufferEndTime = m_vSongTime + (REFERENCE_TIME) opAMMediaType -> lSampleSize;
//		HRESULT hr = pSamp -> SetTime((REFERENCE_TIME*) &m_vSongTime , (REFERENCE_TIME*) &vBufferEndTime);


//		REFERENCE_TIME vTime;
//		REFERENCE_TIME vCurTime;
		REFERENCE_TIME vDummy;
		HRESULT hr;

		if ((!m_fFirstSampleDelivered))
		{
			pSamp -> SetDiscontinuity(true);
		}


		if (pSamp->IsDiscontinuity() == S_OK)
		{
			m_opFilter -> GetClock() -> GetTime(&m_vStartTime);//10000
			pSamp->GetTime((REFERENCE_TIME*)&m_vSongOffset,&vDummy);//40
			m_vDiscont = TRUE;
		}
		else if(m_vDiscont)
		{
			m_opFilter -> GetClock() -> GetTime(&m_vStartTime);//10000
			pSamp->GetTime((REFERENCE_TIME*)&m_vSongOffset,&vDummy);//40
			m_vDiscont = FALSE;
		}
		

		pSamp->GetTime((REFERENCE_TIME*)&m_vSongTime,&vDummy);//40 //20

		if(	(m_vSongTime - m_vSongOffset) < 0)
		{
			m_vSongOffset = m_vSongTime;
		}

		vDummy -= m_vSongTime;//

		m_vSongTime -= m_vSongOffset;
		if (m_vSongTime <0)
			return S_FALSE;


/*		if(FAILED(hr))
		{
			eo << "ERROR! Couldnt set time for buffer!" << ef;
	     return hr;
		}*/
		hr = pSamp -> SetPreroll(FALSE);
		if(FAILED(hr)) 
		{
			eo << "ERROR! Couldnt preroll!" << ef;
			return hr;
		}
		hr = pSamp -> SetMediaType(NULL);
		if(FAILED(hr)) 
		{
	        return hr;
		}
/*		hr = pSamp -> SetDiscontinuity(! m_fFirstSampleDelivered);
		if(FAILED(hr)) 
		{
	        return hr;
	    }*/
	    hr = pSamp -> SetSyncPoint(! m_fFirstSampleDelivered);
	    if(FAILED(hr)) 
		{
			return hr;
		}
	
		//TODO: Make format-independent!
		if(m_vIsClockMaster)
			EMMediaTimer::Instance() -> IncreaseNowFrame(EMMediaUtility::Instance() -> TimeToFrames((vDummy)/10, EMMediaUtility::Instance() -> GetSystemAudioFormat()), m_vMasterID);
		
		if (pSamp->IsDiscontinuity() != S_OK)
		{
			hr = m_opFilter -> GetClock() -> AdviseTime(m_vStartTime, m_vSongTime, m_oTimerEvent, &hEventId);

			if (hr != S_OK)
				return hr;

			WaitForSingleObject(m_oTimerEvent, INFINITE);
		}


		//The next song- and media-times will be the 100-nanosecond after the previous + duration
//		m_vSongTime = m_vSongTime + vBufferDuration + 1;
		//	m_vMediaTimeStart = llMediaTimeStop + 1;
	
		pSamp -> SetDiscontinuity(false);
	
		m_fFirstSampleDelivered = TRUE;
		*p_opIMediaSampleOut = pSamp;

	}
	return S_OK;
}

void EMWinMediaOutputPin::RegisterAsClockMaster(int32 p_vRealtimeOutputDescriptorID)
{
	m_vMasterID = p_vRealtimeOutputDescriptorID;
	m_vIsClockMaster = true;
}

void EMWinMediaOutputPin::UnregisterAsClockMaster()
{
	m_vIsClockMaster = false;
}

#endif