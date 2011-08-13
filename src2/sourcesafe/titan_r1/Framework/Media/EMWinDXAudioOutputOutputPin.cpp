#include "EMWinDXAudioOutputOutputPin.h"

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

#include "EMMediaIDManager.h"

EMWinDXAudioOutputOutputPin* EMWinDXAudioOutputOutputPin::Create(EMWinDXAudioOutputFilter* p_opParentFilter)
{
	HRESULT vResult = S_OK;
	return EM_new EMWinDXAudioOutputOutputPin(&vResult, p_opParentFilter, L"Output");
}

EMWinDXAudioOutputOutputPin::EMWinDXAudioOutputOutputPin(HRESULT *phr, EMWinDXAudioOutputFilter* p_opParentFilter, LPCWSTR pName)
	:	CBaseOutputPin(NAME(""), p_opParentFilter, p_opParentFilter -> GetStateLock(), phr, pName),
		m_opParentFilter(p_opParentFilter),
		m_oFilterName(p_opParentFilter -> GetName() + string("OutPinSem") + EMMediaIDManager::MakeUniqueString()),
		m_opThread(NULL),
		m_opCacheProtectionLock(NULL),
		m_opCache(NULL),
		m_vSongTime((REFERENCE_TIME) 0),
		m_fFirstSampleDelivered(false),
		m_vBuffersAhead(0),
		m_vShouldFlush(false),
		m_vShouldShutdown(false),
		m_vReferenceTimeValue(0),
		m_uCountingSemaphore(NULL),
		m_uShutdownSemaphore(NULL),
		m_vOutputGotInitBuffer(false)
{
	try
	{
		m_opCacheProtectionLock = EMSemaphore::CreateEMSemaphore(string(m_oFilterName + EMMediaIDManager::MakeUniqueString() + "Prot").c_str(), 1, 1);
		m_opCache = EM_new list<IMediaSample*>();
		m_opTimer = EMMediaTimer::Instance();
		m_opUtility = EMMediaUtility::Instance();
		if(EMMediaEngine::Instance() -> GetSettingsRepository() != NULL && m_opUtility != NULL)
			m_opSystemAudioFormat = m_opUtility -> GetSystemAudioFormat();
		m_uCountingSemaphore = CreateSemaphore(NULL, 0, 50, string(string("DXWinAudioOutput counting semaphore") + EMMediaIDManager::MakeUniqueString()).c_str());
		m_uShutdownSemaphore = CreateSemaphore(NULL, 0, 1, string(string("DXWinAudioOutput shutdown semaphore") + EMMediaIDManager::MakeUniqueString()).c_str());

		string oString = string("EMWinDXAudioOutputPin") + EMMediaIDManager::MakeUniqueString();
		m_opThread = EMThread::CreateEMThread(oString.c_str(), EM_THREAD_HIGH_REALTIME_PRIORITY, 0);
		m_opThread -> AddListener(this);
	}
	catch(...)
	{
		EMDebugger("ERROR! Exception in DXAudioOutput constructor!");
	}
}

EMWinDXAudioOutputOutputPin::~EMWinDXAudioOutputOutputPin()
{
	m_opThread -> Kill();
	m_opThread -> RemoveListener(this);

	delete m_opCacheProtectionLock;
	delete m_opCache;
	delete m_opThread;

	CloseHandle(m_uCountingSemaphore);
	CloseHandle(m_uShutdownSemaphore);
}

void EMWinDXAudioOutputOutputPin::SetName(const char* p_vpName)
{
	m_oFilterName = string(p_vpName);
}

HRESULT EMWinDXAudioOutputOutputPin::Active()
{
	CAutoLock lock(m_opParentFilter -> GetStateLock());
	if(m_opParentFilter -> IsActive())
		return S_FALSE;

	HRESULT hr = CBaseOutputPin::Active();
	if(hr != S_OK)
		return hr;

//		m_vBuffersAhead = 8; //Default is 200 ms processing-ahead.
	return NOERROR;
}

HRESULT EMWinDXAudioOutputOutputPin::BreakConnect()
{	
	CAutoLock lock(m_opParentFilter -> GetStateLock());
	HRESULT vResult = CBaseOutputPin::BreakConnect();
	if(FAILED(vResult))
		return vResult;
	return NOERROR;
}

HRESULT EMWinDXAudioOutputOutputPin::CompleteConnect(IPin *pReceivePin)
{
	CAutoLock lock(m_opParentFilter -> GetStateLock());
	HRESULT h = CBaseOutputPin::CompleteConnect(pReceivePin);
	if(FAILED(h))
		return h;

	return h;
}

HRESULT EMWinDXAudioOutputOutputPin::DecideBufferSize(IMemAllocator *pAlloc, ALLOCATOR_PROPERTIES *pProperties)
{
	CritCheckIn(m_opParentFilter -> GetStateLock());

	if(m_opSystemAudioFormat != NULL && m_opUtility != NULL)
	{
		EMSettingsRepository* opSettingsRepository = EMMediaEngine::Instance() -> GetSettingsRepository();
		int32 vTimeAhead = 1000 * (*static_cast<int32*>(opSettingsRepository -> GetSetting(SETTING_AUDIO_PROCESSING_AHEAD)));
		int64 vBufferDuration = m_opUtility -> FramesToTime(m_opUtility -> BytesToFrames(m_opSystemAudioFormat -> m_vBufferSizeBytes, m_opSystemAudioFormat), m_opSystemAudioFormat);
		m_vBuffersAhead = ceil(vTimeAhead / vBufferDuration);
	}
	else
		m_vBuffersAhead = 8; //Default is 200 mS of processing-ahead.

	pProperties -> cBuffers = m_vBuffersAhead;
	pProperties -> cbBuffer = EM_AUDIO_BUFFER_SIZE;

    ALLOCATOR_PROPERTIES Actual;
    HRESULT hr = pAlloc -> SetProperties(pProperties, &Actual);
    if(FAILED(hr)) 
	{
        return hr;
	}

	if(Actual.cbBuffer < pProperties -> cbBuffer) 
	{
        return E_FAIL;
	}
	return pAlloc -> Commit();
}

HRESULT EMWinDXAudioOutputOutputPin::GetMediaType(int p_vPosition, CMediaType *p_opMediaTypeObject)
{
	CAutoLock lock(m_opParentFilter -> GetStateLock());

	if(p_vPosition == 0)
	{
		//First, allocate a new WAVEFORMATEX structure and get a pointer to it...
		WAVEFORMATEX* sWaveFormatStructure = reinterpret_cast<WAVEFORMATEX*>(p_opMediaTypeObject -> AllocFormatBuffer(sizeof(WAVEFORMATEX)));

		//Then fill it out with the PCM format information
		sWaveFormatStructure -> wFormatTag = WAVE_FORMAT_PCM;
		sWaveFormatStructure -> nChannels = EM_AUDIO_NUM_CHANNELS;
		sWaveFormatStructure -> nSamplesPerSec = (uint32) 44100; //*(static_cast<int32*>(m_opFilter -> GetSettingsRepository() -> GetSetting(SETTING_AUDIO_FRAMERATE)));
		sWaveFormatStructure -> wBitsPerSample = EM_AUDIO_SAMPLESIZE * 8;        
		sWaveFormatStructure -> nBlockAlign = static_cast<WORD>((sWaveFormatStructure -> wBitsPerSample * sWaveFormatStructure -> nChannels) / 8);
		sWaveFormatStructure -> nAvgBytesPerSec = sWaveFormatStructure -> nBlockAlign * sWaveFormatStructure -> nSamplesPerSec;
		sWaveFormatStructure -> cbSize = 0;

		//And then fill out the CMediaType object with information based on that WAVEFORMATEX structure
		if(FAILED(CreateAudioMediaType(sWaveFormatStructure, p_opMediaTypeObject, true)))
			return E_UNEXPECTED;
	}
	else
		return VFW_S_NO_MORE_ITEMS;

	p_opMediaTypeObject -> SetTemporalCompression(TRUE);

	return S_OK;
}

HRESULT EMWinDXAudioOutputOutputPin::CheckMediaType(const CMediaType *pMediaType)
{
	CAutoLock lock(m_opParentFilter -> GetStateLock());
	return S_OK;
}

STDMETHODIMP EMWinDXAudioOutputOutputPin::NonDelegatingQueryInterface(REFIID riid, void **ppv)
{
	HRESULT h = CBaseOutputPin::NonDelegatingQueryInterface(riid, ppv);
	return h;
}

bool EMWinDXAudioOutputOutputPin::ProcessBufferE(EMMediaDataBuffer* p_opBuffer)
{
//	eo << "STORING BUFFER (" << m_opCache -> size() << " buffer(s))" << ef;

	IMediaSample *pSample = NULL;
	HRESULT hr = GetDeliveryBuffer(&pSample, NULL, NULL, AM_GBF_NOWAIT);

	bool vIsPlaying = (m_opTimer != NULL ? m_opTimer -> IsPlaying() : false);
	while(hr == VFW_E_TIMEOUT && vIsPlaying)
	{
		::Sleep(2);
		hr = GetDeliveryBuffer(&pSample, NULL, NULL, AM_GBF_NOWAIT);
		vIsPlaying = m_opTimer -> IsPlaying();
	}

	if(pSample != NULL)
	{
		BYTE* data;
		pSample -> GetPointer(&data);
		memcpy(data, p_opBuffer -> Data(), p_opBuffer -> m_vSizeUsed);
		pSample -> SetActualDataLength(p_opBuffer -> m_vSizeUsed);
		
		if(m_opCacheProtectionLock -> Acquire())
		{
			m_opCache -> push_back(pSample);
			m_opCacheProtectionLock -> Release();
			hr = ReleaseSemaphore(m_uCountingSemaphore, 1, NULL);
		}
		return true;
	}
	return false;
}

void EMWinDXAudioOutputOutputPin::ThreadRun(EMThread* p_opThread)
{
	if(! m_vOutputGotInitBuffer)
	{
		m_opCacheProtectionLock -> Acquire();
		IMediaSample* pSample = NULL; 
		HRESULT vResult = GetDeliveryBuffer(&pSample, NULL, NULL, AM_GBF_NOWAIT);
		if(pSample != NULL)
		{
			BYTE* data;
			pSample -> GetPointer(&data);
			memset(data, 0, EM_AUDIO_BUFFER_SIZE);
			pSample -> SetActualDataLength(0);
//			eo << "Initializing \"" << m_oFilterName.c_str() << "\" output by sending a dummy audio buffer..." << ef;
			m_opCache -> push_back(pSample);
			m_opCacheProtectionLock -> Release();
			vResult = ReleaseSemaphore(m_uCountingSemaphore, 1, NULL);
			m_vOutputGotInitBuffer = true;
		}
	}
	IMediaSample* opSample = NULL;
	bool vAcquireResult = false;

	if(! m_vShouldShutdown)
	{
		HANDLE uHandles[2];
		uHandles[0] = m_uCountingSemaphore;
		uHandles[1] = m_uShutdownSemaphore;
		HRESULT vWaitingResult = WaitForMultipleObjects(2, uHandles, false, INFINITE);

//		try
//		{
		if(vWaitingResult == WAIT_OBJECT_0)
		{
			if(m_opCacheProtectionLock -> Acquire())
			{
				if(m_opCache -> size() > 0)
				{
					opSample = m_opCache -> front();
					m_opCache -> pop_front();
				}
				else 
					opSample = NULL;
			
				if(opSample != NULL) // && ((uint32) opSample) != 0xcdcdcdcd)
				{
					if(m_opSystemAudioFormat != NULL)
						m_opSystemAudioFormat -> CreateAudioFormat((AM_MEDIA_TYPE*) ((CMediaType*) (m_mt.Format())));

					//Fetch the number of bytes in the buffer
					uint64 vNumberOfBytesInBuffer = opSample -> GetActualDataLength();
				
					//calculate the number of 100-nanoseconds in the buffer (ie the duration)
					int64 vBufferDurationInt = 0;
					if(m_opUtility != NULL && m_opSystemAudioFormat != NULL)
						vBufferDurationInt = (UNITS * m_opUtility -> FramesToTime(m_opUtility -> BytesToFrames(vNumberOfBytesInBuffer, m_opSystemAudioFormat), m_opSystemAudioFormat)) / 1000000;
					CRefTime vBufferDuration = (REFERENCE_TIME) vBufferDurationInt;

					REFERENCE_TIME vStart = m_opParentFilter -> m_vStartingTime; //; //m_opParentFilter -> m_tStart; //(REFERENCE_TIME) m_tStart;
					REFERENCE_TIME vNow = (m_opTimer != NULL ? m_opTimer -> RealTime() * 10.0 : 0);
					REFERENCE_TIME vRealSongTime = vNow - vStart;
					REFERENCE_TIME vBuff = m_vSongTime;
					REFERENCE_TIME vDifference = vBuff - vRealSongTime;

					if(vDifference > 0)
						m_opThread -> Sleep(vDifference / 10.0);
//					eo << "Sleeping " << vDifference / 10000 << ef;

					//eo << "Start = " << vStart / 10000.0 << " mS, vRealSongTime = " << vRealSongTime / 10000.0 << " mS, vNow = " << vNow / 10000.0 << " mS, vBuff = " << vBuff << ", vDifference = " << vDifference << ef;

					//set the buffer's end time to be the current song time + the duration
					CRefTime vBufferEndTime = m_vSongTime + vBufferDuration;
					HRESULT hr = opSample -> SetTime((REFERENCE_TIME*) &m_vSongTime , (REFERENCE_TIME*) &vBufferEndTime);
/*					if(FAILED(hr))	
					{
						EMDebugger("ERROR! Couldnt set time for buffer!");
						opSample -> Release();
						return;
					} */

					hr = opSample -> SetMediaTime((REFERENCE_TIME*) NULL, (REFERENCE_TIME*) NULL);
/*					if(FAILED(hr))	
					{
						EMDebugger("ERROR! Couldnt set media time for buffer!");
						opSample -> Release();
						return;
					} */

					hr = opSample -> SetPreroll(TRUE);
/*					if(FAILED(hr)) 
					{
						EMDebugger("ERROR! Couldnt preroll!");
						opSample -> Release();
						return;
					} */
					hr = opSample -> SetMediaType(NULL);
/*					if(FAILED(hr)) 
					{
						EMDebugger("ERROR! Couldnt set media type");
						opSample -> Release();
						return;
					} */
					hr = opSample -> SetDiscontinuity(TRUE); //! m_fFirstSampleDelivered);
/*					if(FAILED(hr)) 
					{
						EMDebugger("ERROR! Couldnt set discontinuity");
						opSample -> Release();
						return;
					} */
					hr = opSample -> SetSyncPoint(TRUE); //! m_fFirstSampleDelivered);
/*					if(FAILED(hr)) 
					{
						EMDebugger("ERROR! Couldnt set sync point!");
						opSample -> Release(); 
						return;
					} */

					m_vSongTime = m_vSongTime + vBufferDuration + 1;

//					eo << "d" << ef;
					hr = Deliver(opSample);
					uint32 vRefCount = opSample -> Release();
					int64 vTime = m_vReferenceTimeValue + ((int64) m_vSongTime) / 10;
					int64 vOfficialTime = (m_opTimer != NULL ? m_opTimer -> NowTime() : 0);
					int64 vDiff = vTime - vOfficialTime;

					if(m_opParentFilter != NULL && m_opParentFilter -> IsClockMaster() && m_opSystemAudioFormat != NULL && m_opTimer != NULL)
						m_opTimer -> IncreaseNowFrame(m_opUtility -> TimeToFrames(vDiff, m_opSystemAudioFormat), m_opParentFilter -> GetClockMasterID());
					m_fFirstSampleDelivered = TRUE;		
					m_opCacheProtectionLock -> Release();
				}
				else
				{
					m_opCacheProtectionLock -> Release();
					::Sleep(2);
				}
				
			}
		}
//			else
//				; //eo << "WARNING! DXAudioOutput wait for buffers failed!" << ef;
//		}
//		catch(...)
//		{
//			eo << "ERROR! DxAudioOutput caught an exception while sending buffer to \"" << m_oFilterName.c_str() << "\" physical output!" << ef;
//		}
	}
	::Sleep(10);
}

void EMWinDXAudioOutputOutputPin::StartProcessing()
{
	m_opThread -> Start();
}

bool EMWinDXAudioOutputOutputPin::StopProcessing()
{
	m_opThread -> Kill();
	return true;
}

//TODO: Make sure that no buffer is waiting at the ProcessBuffer semaphore after the flush!
void EMWinDXAudioOutputOutputPin::Flush()
{
	IMediaSample* opSample = NULL;

	if(! m_opCacheProtectionLock -> Acquire())
	{
		MessageBox(NULL, "Failed to acquire cache protection lock while flushing!", "Warning!", MB_OK);
		return;
	}

//	eo << "DXAudioOutput " << m_oFilterName.c_str() << " clearing " << m_opCache -> size() << " IMediaSample(s)" << ef;

	while(m_opCache -> size() > 0)
	{
		opSample = m_opCache -> front();
		m_opCache -> pop_front();
		opSample -> Release();
	}
	m_vShouldFlush = true;

	m_fFirstSampleDelivered = FALSE;
	m_vSongTime = (REFERENCE_TIME) 0;
	m_vReferenceTimeValue = (m_opTimer != NULL ? m_opTimer -> NowTime() : 0);

	m_opCacheProtectionLock -> Release();
}

bool EMWinDXAudioOutputOutputPin::Shutdown()
{
	m_opCacheProtectionLock -> Acquire();
	m_vShouldShutdown = true;
	ReleaseSemaphore(m_uShutdownSemaphore, 1, NULL);
	m_opCacheProtectionLock -> Release();
	m_opThread -> Kill();
	return true;
}