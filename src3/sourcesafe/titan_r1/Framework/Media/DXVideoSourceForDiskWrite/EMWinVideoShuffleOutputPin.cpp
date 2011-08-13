#include "EMWinVideoShuffleOutputPin.h"

#include "EMGlobals.h"
#include "EMMediaGlobals.h"
#include "EMMediaDataBuffer.h"
#include "EMThread.h"
#include "EMSemaphore.h"
#include "EMMediaUtility.h"
#include "EMMediaTimer.h"
#include "EMMediaEngine.h"
#include "EMSettingsRepository.h"
#include "EMSettingIDs.h"

#include <streams.h>
#include <math.h>
#include <DVDMedia.h>
#include <process.h>

static const GUID CLSID_DUMMY = {0x00000001, 0x0000, 0x0010, {0x80, 0x00,0x00, 0xAA, 0x00, 0x38, 0x9B ,0x71}};

EMWinVideoShuffleOutputPin* EMWinVideoShuffleOutputPin::Create(EMWinVideoShuffleOutputFilter* p_opParentFilter)
{
	HRESULT vResult = S_OK;
	
	return EM_new EMWinVideoShuffleOutputPin(&vResult, p_opParentFilter, L"Output");
}

EMWinVideoShuffleOutputPin::EMWinVideoShuffleOutputPin(HRESULT *phr, EMWinVideoShuffleOutputFilter* p_opParentFilter, LPCWSTR pName)
	:	CBaseOutputPin(NAME(""), p_opParentFilter, p_opParentFilter -> GetStateLock(), phr, pName),
		m_opParentFilter(p_opParentFilter),
		m_oFilterName(p_opParentFilter -> GetName()),
		m_vSongTime((REFERENCE_TIME) 0),//?
		m_fFirstSampleDelivered(true),
		m_vBuffersAhead(0),
		m_vIsClockMaster(false),
		m_vShouldFlush(false),
		m_opCopiedMediaFormat(EM_TYPE_RAW_VIDEO)
{
	try
	{
		EMMediaFormat* opSystemAudioFormat = EMMediaUtility::Instance() -> GetSystemAudioFormat();
		int64 vFramesPerBuffer = EMMediaUtility::Instance() -> FramesPerBuffer(opSystemAudioFormat);
		int64 vFramesAhead = EMMediaUtility::Instance() -> TimeToFrames(EM_AUDIO_READAHEAD * 1000000, opSystemAudioFormat);
		int64 vBuffersAhead = 8; //static_cast<int64>(ceil(static_cast<float>(vFramesAhead) / static_cast<float>(vFramesPerBuffer)));
		m_vBuffersAhead = vBuffersAhead;
	}
	catch(...)
	{
		EMDebugger("ERROR! Exception in EMWinVideoShuffleOutputPin constructor!");
	}


	m_vIsRunning = false;
	m_opIMediaSampleOut = NULL;
//	m_hFullCache = CreateEvent(NULL, false, false, NULL);
//	m_hEmptyCache = CreateEvent(NULL, false, true, NULL);

//	m_hThread = (HANDLE) _beginthreadex(NULL, 0, ThreadProc, (void *)this, 0, &m_vThreadID);
//	SetPriorityClass(m_hThread, HIGH_PRIORITY_CLASS);
//	SetThreadPriority(m_hThread, THREAD_PRIORITY_ABOVE_NORMAL);
}

EMWinVideoShuffleOutputPin::~EMWinVideoShuffleOutputPin()
{
	eo << "EMWinVideoShuffleOutputPin::~EMWinVideoShuffleOutputPin()" << ef;

	EndThread();
}


STDMETHODIMP EMWinVideoShuffleOutputPin::StartThread()
{
	if(!m_vIsRunning)
	{
		m_opIMediaSampleOut = NULL;
		m_hFullCache = CreateEvent(NULL, false, false, NULL);
		m_hEmptyCache = CreateEvent(NULL, false, true, NULL);
		m_hThreadQuit = CreateEvent(NULL,false, false, NULL);

		m_hThread = (HANDLE) _beginthreadex(NULL, 0, ThreadProc, (void *)this, 0, &m_vThreadID);
		SetPriorityClass(m_hThread, HIGH_PRIORITY_CLASS);
		SetThreadPriority(m_hThread, THREAD_PRIORITY_ABOVE_NORMAL);

		m_vIsRunning = true;
	}

	return S_OK;
}

STDMETHODIMP EMWinVideoShuffleOutputPin::EndThread()
{
	if(m_vIsRunning)
	{
		m_vIsRunning = false;
		SetEvent(m_hFullCache);

		while( WaitForSingleObject(m_hThreadQuit, 5000) == WAIT_TIMEOUT)
		{
			m_vIsRunning = false;
			SetEvent(m_hFullCache);		
		}
		
		
		CloseHandle(m_hThreadQuit);
		CloseHandle(m_hFullCache);
		CloseHandle(m_hEmptyCache);
	}
	return S_OK;
}


unsigned EMWinVideoShuffleOutputPin::ThreadProc(void *p_vpThis)
{
	return ((EMWinVideoShuffleOutputPin*)p_vpThis)->ShuffleData();
}


unsigned EMWinVideoShuffleOutputPin::ShuffleData()
{
	while(m_vIsRunning)
	{
		WaitForSingleObject(m_hFullCache, INFINITE);

		if (m_vIsRunning)
		{
			if ( m_opIMediaSampleOut != NULL)
			{
//				AM_MEDIA_TYPE* opAMMediaType = ((AM_MEDIA_TYPE*) ((CMediaType*) (m_mt.Format())));
//				VIDEOINFOHEADER* opInfo = (VIDEOINFOHEADER*)(opAMMediaType->pbFormat);
				VIDEOINFOHEADER* opInfo = (VIDEOINFOHEADER*)(m_mt.Format());
				
				CRefTime vBufferDuration = opInfo->AvgTimePerFrame;

				Deliver(m_opIMediaSampleOut);
				if(m_vIsClockMaster)
					EMMediaTimer::Instance() -> IncreaseNowFrame(EMMediaUtility::Instance() -> TimeToFrames(vBufferDuration.m_time/10, EMMediaUtility::Instance() -> GetSystemAudioFormat()), m_vMasterID);
				m_opIMediaSampleOut->Release();
				m_opIMediaSampleOut = NULL;
			}
			SetEvent(m_hEmptyCache);
		}
	}

	SetEvent(m_hThreadQuit);

	return 0;
}

void EMWinVideoShuffleOutputPin::DeliverCached(IMediaSample *p_opIMediaSampleOut)
{
	while(WaitForSingleObject(m_hEmptyCache, 5) == WAIT_TIMEOUT && m_vIsRunning);

	if (m_vIsRunning)
	{
		m_opIMediaSampleOut = p_opIMediaSampleOut;
		SetEvent(m_hFullCache);
	}

}



void EMWinVideoShuffleOutputPin::SetName(string p_oName)
{
	m_oFilterName = p_oName;
}

HRESULT EMWinVideoShuffleOutputPin::Active()
{
	CAutoLock lock(m_opParentFilter -> GetStateLock());
	if(m_opParentFilter -> IsActive())
		return S_FALSE;

	HRESULT hr = CBaseOutputPin::Active();
	if(hr != S_OK)
		return hr;

	return NOERROR;
}

HRESULT EMWinVideoShuffleOutputPin::BreakConnect()
{	
	CAutoLock lock(m_opParentFilter -> GetStateLock());
	HRESULT vResult = CBaseOutputPin::BreakConnect();
	if(FAILED(vResult))
		return vResult;
	return NOERROR;
}

HRESULT EMWinVideoShuffleOutputPin::CompleteConnect(IPin *pReceivePin)
{

	CAutoLock lock(m_opParentFilter -> GetStateLock());
	HRESULT h = CBaseOutputPin::CompleteConnect(pReceivePin);
	if(FAILED(h))
		return h;


	return h;
}

HRESULT EMWinVideoShuffleOutputPin::DecideBufferSize(IMemAllocator *pAlloc, ALLOCATOR_PROPERTIES *pProperties)
{
//TODO: FIND OUT WHAT THE HELL IS ALREADY DEFINED PRIOR TO EVERYTHING ELSE, ETC
	ASSERT(CritCheckIn(m_opParentFilter -> GetStateLock()));

	if((m_opMediaFormat->m_eType & EM_TYPE_RAW_VIDEO) > 0)
	{

		pProperties -> cBuffers = 1;
		pProperties -> cbBuffer = m_opMediaFormat->m_vBufferSizeBytes;

	    ALLOCATOR_PROPERTIES Actual;
		HRESULT hr = pAlloc -> SetProperties(pProperties, &Actual);
		if(FAILED(hr)) 
			return hr;

		if(Actual.cbBuffer < pProperties -> cbBuffer) 
			return E_FAIL;
		hr = pAlloc -> Commit();
		return hr;

		return NOERROR;
	}
	else if((m_opMediaFormat->m_eType & EM_TYPE_RAW_AUDIO) > 0)
	{
		pProperties -> cBuffers = 16; //m_vBuffersAhead;
		pProperties -> cbBuffer = *static_cast<int32*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_AUDIO_BUFFER_SIZE));

	    ALLOCATOR_PROPERTIES Actual;
		HRESULT hr = pAlloc -> SetProperties(pProperties, &Actual);
		if(FAILED(hr)) 
			return hr;

		if(Actual.cbBuffer < pProperties -> cbBuffer) 
			return E_FAIL;
		return pAlloc -> Commit();
	}
	return E_FAIL;
}

HRESULT EMWinVideoShuffleOutputPin::GetMediaType(int p_vPosition, CMediaType* p_opMediaTypeObject)
{
	CAutoLock lock(m_opParentFilter -> GetStateLock());
	if(p_vPosition == 0)
	{
		//Gets the format set by SetShuffleFormat (This is an interface class and has to be set externally)
		(*p_opMediaTypeObject) = oMediaType;
//		memcpy(p_opMediaTypeObject, m_opConnectedMediaType, sizeof(CMediaType));
	}
	else
		return VFW_S_NO_MORE_ITEMS;

	return S_OK;
}

void EMWinVideoShuffleOutputPin::SetShuffleFormat(EMMediaFormat* p_opMediaFormat)
{
	m_vSongTime = 0;
	memcpy(&m_opCopiedMediaFormat, p_opMediaFormat, sizeof(EMMediaFormat));
	m_opMediaFormat = &m_opCopiedMediaFormat;

	AM_MEDIA_TYPE* myFormat = (AM_MEDIA_TYPE*) &oMediaType;

	if((p_opMediaFormat->m_eType & EM_TYPE_RAW_VIDEO) > 0)
	{
		myFormat->bFixedSizeSamples = true;
		myFormat->lSampleSize = p_opMediaFormat -> m_vHeight * p_opMediaFormat -> m_vWidth * p_opMediaFormat -> m_vDepth;

		VIDEOINFOHEADER *sVideoFormatStructure = reinterpret_cast<VIDEOINFOHEADER *>( oMediaType.AllocFormatBuffer(sizeof(VIDEOINFOHEADER)));
		ZeroMemory(sVideoFormatStructure, sizeof(VIDEOINFOHEADER));


		sVideoFormatStructure->rcSource.top = 0;
		sVideoFormatStructure->rcSource.bottom = p_opMediaFormat -> m_vHeight;
		sVideoFormatStructure->rcSource.left = 0;
		sVideoFormatStructure->rcSource.right = p_opMediaFormat -> m_vWidth;
		sVideoFormatStructure -> AvgTimePerFrame = 10000000 / p_opMediaFormat -> m_vFrameRate;
		sVideoFormatStructure -> dwBitRate = 1851392;//8 * p_opMediaFormat -> m_vBufferSizeBytes * p_opMediaFormat -> m_vFrameRate; //How should this be calculated?

		sVideoFormatStructure -> bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		sVideoFormatStructure -> bmiHeader.biWidth = p_opMediaFormat -> m_vWidth;
		sVideoFormatStructure -> bmiHeader.biHeight = p_opMediaFormat -> m_vHeight;
		sVideoFormatStructure -> bmiHeader.biPlanes = 1;
		sVideoFormatStructure -> bmiHeader.biBitCount = 8*p_opMediaFormat -> m_vDepth;
		sVideoFormatStructure -> bmiHeader.biCompression = 0;
		sVideoFormatStructure -> bmiHeader.biSizeImage =p_opMediaFormat -> m_vBufferSizeBytes;
		sVideoFormatStructure -> bmiHeader.biXPelsPerMeter = 2250;
		sVideoFormatStructure -> bmiHeader.biYPelsPerMeter = 2000;

		oMediaType.majortype = MEDIATYPE_Video;
		oMediaType.subtype = MEDIASUBTYPE_RGB24; //Temporily set to RGB24(will depend on render options)
		oMediaType.formattype = FORMAT_VideoInfo;
		m_opConnectedMediaType = &oMediaType;
	}
	else if((p_opMediaFormat->m_eType & EM_TYPE_RAW_AUDIO) > 0)
	{
		//First, allocate a new WAVEFORMATEX structure and get a pointer to it...
		WAVEFORMATEX* sWaveFormatStructure = reinterpret_cast<WAVEFORMATEX*>(oMediaType.AllocFormatBuffer(sizeof(WAVEFORMATEX)));

		//Then fill it out with the PCM format information
		sWaveFormatStructure -> wFormatTag = WAVE_FORMAT_PCM;
		sWaveFormatStructure -> nChannels = EM_AUDIO_NUM_CHANNELS;
		sWaveFormatStructure -> nSamplesPerSec = (uint32) 44100; //*(static_cast<int32*>(m_opFilter -> GetSettingsRepository() -> GetSetting(SETTING_AUDIO_FRAMERATE)));
		sWaveFormatStructure -> wBitsPerSample = EM_AUDIO_SAMPLESIZE * 8;        
		sWaveFormatStructure -> nBlockAlign = static_cast<WORD>((sWaveFormatStructure -> wBitsPerSample * sWaveFormatStructure -> nChannels) / 8);
		sWaveFormatStructure -> nAvgBytesPerSec = sWaveFormatStructure -> nBlockAlign * sWaveFormatStructure -> nSamplesPerSec;
		sWaveFormatStructure -> cbSize = 0;

		oMediaType.majortype = MEDIATYPE_Audio;
		oMediaType.subtype = CLSID_DUMMY; //PCM - format.... MEDIASUBTYPE_WAVE
		oMediaType.formattype = FORMAT_WaveFormatEx;
		m_opConnectedMediaType = &oMediaType;
	}
}

HRESULT EMWinVideoShuffleOutputPin::CheckMediaType(const CMediaType *pMediaType)
{
	CAutoLock lock(m_opParentFilter -> GetStateLock());

	AM_MEDIA_TYPE* myFormat = (AM_MEDIA_TYPE*) pMediaType;
	VIDEOINFOHEADER *sVideoFormatStructure = reinterpret_cast<VIDEOINFOHEADER *>( myFormat->pbFormat);

	if (*pMediaType == oMediaType)
		return S_OK;

	return S_FALSE;
}

STDMETHODIMP EMWinVideoShuffleOutputPin::NonDelegatingQueryInterface(REFIID riid, void **ppv)
{
	HRESULT h = CBaseOutputPin::NonDelegatingQueryInterface(riid, ppv);
	return h;
}

bool EMWinVideoShuffleOutputPin::ProcessBufferE(EMMediaDataBuffer* p_opBuffer)
{
	try
	{
		IMediaSample *pSample = NULL;
		HRESULT hr = GetDeliveryBuffer(&pSample, NULL, NULL, 0);
		if(FAILED(hr))
			return false;
		else
		{
			BYTE* data;
			pSample -> GetPointer(&data);
			memcpy(data, p_opBuffer -> Data(), p_opBuffer -> m_vSizeUsed);
			pSample -> SetActualDataLength(p_opBuffer -> m_vSizeUsed);

			//NO Caching NEEDED?
			//Instead of cahcing the sample send it of right away!
			m_opBuffer = p_opBuffer;
			TimeStampAndDeliverBuffer(pSample);
		}
	}
	catch(...)
	{
		eo << "ERROR! Exception caught in ProcessBuffer in AudioOutputPin" << ef;
		throw;
	}
	return false;
}


void EMWinVideoShuffleOutputPin::SetTimeStamp(REFERENCE_TIME p_vRefTime)
{
	m_vSongTime = p_vRefTime;
}


HRESULT EMWinVideoShuffleOutputPin::TimeStampAndDeliverBuffer(IMediaSample* p_opIMediaSampleOut)
{
	//Try reconectiom
	HRESULT hr;

	EMMediaDataBuffer* opDataBuffer = NULL;
	if(p_opIMediaSampleOut == NULL)
	{
//		eo << "   DirectX -> Internal error! Failed to get IMediaSample from cache!" << ef;
		DeliverEndOfStream();
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

	if((m_opMediaFormat->m_eType & EM_TYPE_RAW_AUDIO) > 0)
	{
			EMMediaFormat opFormat(EM_TYPE_RAW_AUDIO);
			opFormat.CreateAudioFormat((AM_MEDIA_TYPE*) ((CMediaType*) (m_mt.Format())));

			//Fetch the number of bytes in the buffer
			
			uint64 vNumberOfBytesInBuffer = p_opIMediaSampleOut -> GetActualDataLength();
		
			//calculate the number of 100-nanoseconds in the buffer (ie the duration)
			int64 vBufferDurationInt = (UNITS * EMMediaUtility::Instance() -> FramesToTime(EMMediaUtility::Instance() -> BytesToFrames(vNumberOfBytesInBuffer, &opFormat), &opFormat)) / 1000000;
			CRefTime vBufferDuration = (REFERENCE_TIME) vBufferDurationInt;

			//set the buffer's end time to be the current song time + the duration
			CRefTime vBufferEndTime = m_vSongTime + vBufferDuration;
			HRESULT hr = p_opIMediaSampleOut -> SetTime((REFERENCE_TIME*) &m_vSongTime , (REFERENCE_TIME*) &vBufferEndTime);
			if(FAILED(hr))	
			{
				EMDebugger("ERROR! Couldnt set time for buffer!");
				p_opIMediaSampleOut -> Release();
				return hr;
			}
			hr = p_opIMediaSampleOut -> SetPreroll(FALSE);
			if(FAILED(hr)) 
			{
				EMDebugger("ERROR! Couldnt preroll!");
				p_opIMediaSampleOut -> Release();
				return hr;
			}
			hr = p_opIMediaSampleOut -> SetMediaType(NULL);
			if(FAILED(hr)) 
			{
				EMDebugger("ERROR! Couldnt set media type");
				p_opIMediaSampleOut -> Release();
				return hr;
			}
			hr = p_opIMediaSampleOut -> SetDiscontinuity(! m_fFirstSampleDelivered);
			if(FAILED(hr)) 
			{
				EMDebugger("ERROR! Couldnt set discontinuity");
				p_opIMediaSampleOut -> Release();
				return hr;
			}
			hr = p_opIMediaSampleOut -> SetSyncPoint(! m_fFirstSampleDelivered);
			if(FAILED(hr)) 
			{
				EMDebugger("ERROR! Couldnt set sync point!");
				p_opIMediaSampleOut -> Release(); 
				return hr;
			}

			//TODO: Make format-independent!
			//m_opFilter -> GetTimer() -> IncreaseNowFrame(EMMediaUtility::Instance() -> BytesToFrames(vNumberOfBytesInBuffer, EMMediaUtility::Instance() -> GetSystemAudioFormat()));
		
			//The next song- and media-times will be the 100-nanosecond after the previous + duration
			m_vSongTime = m_vSongTime + vBufferDuration + 1;
			//m_vMediaTimeStart = llMediaTimeStop + 1;

			m_fFirstSampleDelivered = TRUE;		

			Deliver(p_opIMediaSampleOut);
			p_opIMediaSampleOut -> Release();

			EMMediaTimer::Instance() -> IncreaseNowFrame(1024, m_vMasterID);

	} else if((m_opMediaFormat->m_eType & EM_TYPE_RAW_VIDEO) > 0)
	{
//		AM_MEDIA_TYPE* opAMMediaType = ((AM_MEDIA_TYPE*) ((CMediaType*) (m_mt.Format())));

//		CRefTime vBufferDuration = (REFERENCE_TIME) opAMMediaType -> lSampleSize;

//		VIDEOINFOHEADER* opInfo = (VIDEOINFOHEADER*)(opAMMediaType->pbFormat);				
//		CRefTime vBufferDuration = opInfo->AvgTimePerFrame;

		AM_MEDIA_TYPE* opAMMediaType = (AM_MEDIA_TYPE*) (&m_mt);
		VIDEOINFOHEADER* opInfo = (VIDEOINFOHEADER*)(m_mt.Format());				
		CRefTime vBufferDuration = opInfo->AvgTimePerFrame;

		CRefTime vBufferEndTime = m_vSongTime + vBufferDuration;//*/m_opBuffer -> GetRelativeTimeToStart();// 1000000;//(REFERENCE_TIME) opAMMediaType -> lSampleSize;
		//REFERENCE_TIME agust = 1000000;
		hr = p_opIMediaSampleOut -> SetTime((REFERENCE_TIME*) &m_vSongTime , (REFERENCE_TIME*) &vBufferEndTime);

		if(FAILED(hr))
		{
			eo << "ERROR! Couldnt set time for buffer!" << ef;
	     return hr;
		}
		hr = p_opIMediaSampleOut -> SetPreroll(FALSE);
		if(FAILED(hr)) 
		{
			eo << "ERROR! Couldnt preroll!" << ef;
			return hr;
		}
		hr = p_opIMediaSampleOut -> SetMediaType(NULL);
		if(FAILED(hr)) 
		{
	        return hr;
		}
		hr = p_opIMediaSampleOut -> SetDiscontinuity(!m_fFirstSampleDelivered);
		if(FAILED(hr)) 
		{
	       return hr;
	    }
	    hr = p_opIMediaSampleOut -> SetSyncPoint(!m_fFirstSampleDelivered);
	    if(FAILED(hr)) 
		{
			return hr;
		}
	
		//The next song- and media-times will be the 100-nanosecond after the previous + duration
		m_vSongTime = m_vSongTime + vBufferDuration + 1;
		//	m_vMediaTimeStart = llMediaTimeStop + 1;

		m_fFirstSampleDelivered = TRUE;

		if (m_vIsRunning)
		{
			DeliverCached(p_opIMediaSampleOut);
		}
		else
		{
			Deliver(p_opIMediaSampleOut);	

			if(m_vIsClockMaster)
				EMMediaTimer::Instance() -> IncreaseNowFrame(EMMediaUtility::Instance() -> TimeToFrames(vBufferDuration.m_time/10, EMMediaUtility::Instance() -> GetSystemAudioFormat()), m_vMasterID);
			p_opIMediaSampleOut->Release();
		}

//
	}
	return S_OK;
}

/*void EMWinVideoShuffleOutputPin::StartProcessing()
{
	m_opThread -> Start();
}

bool EMWinVideoShuffleOutputPin::StopProcessing()
{
	m_opThread -> Kill();
	return true;
}
*/
//TODO: Make sure that no buffer is waiting at the ProcessBuffer semaphore after the flush!
void EMWinVideoShuffleOutputPin::Flush()
{
/*	IMediaSample* opSample = NULL;

	if(! m_opCacheProtectionLock -> Acquire())
	{
		MessageBox(NULL, "Failed to acquire cache protection lock while flushing!", "Warning!", MB_OK);
		return;
	}

	while(m_opCache -> size() > 0)
	{
		opSample = m_opCache -> front();
		m_opCache -> pop_front();
		opSample -> Release();
		//m_opCountingSemaphore -> Release();
	}
	m_vShouldFlush = true;

	m_fFirstSampleDelivered = FALSE;
	m_vSongTime = (REFERENCE_TIME) 0;

	m_opCacheProtectionLock -> Release();
*/
}

/*bool EMWinVideoShuffleOutputPin::Lock(uint32 p_vTimeoutMilliSeconds)
{
	try
	{
		if(p_vTimeoutMilliSeconds == 0)
		{
			if(! m_opCountingSemaphore -> Acquire())
			{
				MessageBox(NULL, "Failed to acquire the cache's counting semaphore!", "Warning!", MB_OK);
				return false;
			}
		}
		else
			EMDebugger("ERROR! Not implemented!");
		return true;
	}
	catch(...)
	{
		EMDebugger("ERROR! Exception while acquiring semaphore!");
	}
	return false;
}

void EMWinVideoShuffleOutputPin::Unlock()
{
	try
	{
		m_opCountingSemaphore -> Release();
	}
	catch(...)
	{
		EMDebugger("ERROR! Exception while releasing semaphore!");
	}
}
*/

/*bool EMWinVideoShuffleOutputPin::Shutdown()
{
	m_opThread -> Kill();
	return true;
}
*/

void EMWinVideoShuffleOutputPin::RegisterAsClockMaster(int32 p_vRealtimeOutputDescriptorID)
{
	m_vMasterID = p_vRealtimeOutputDescriptorID;
	m_vIsClockMaster = true;
}

void EMWinVideoShuffleOutputPin::UnregisterAsClockMaster()
{
	m_vIsClockMaster = false;
}
