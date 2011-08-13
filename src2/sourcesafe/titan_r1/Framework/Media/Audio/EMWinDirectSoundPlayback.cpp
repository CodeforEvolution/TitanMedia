#include "EMWinDirectSoundPlayback.h"

#include "CommandIDs.h"
#include "EMApplication.h"
#include "EMCommandRepository.h"
#include "EMDSCache.h"
#include "EMMediaDataBuffer.h"
#include "EMMediaDebugLog.h"
#include "EMMediaEngine.h"
#include "EMMediaIDManager.h"
#include "EMMediaTimer.h"
#include "EMMediaUtility.h"
#include "EMMessages.h"
#include "EMSemaphore.h"
#include "EMSettingIDs.h"
#include "EMSettingsRepository.h"
#include "EMThread.h"
#include "EMThreadListener.h"
#include "Messages.h"

#include <dsound.h>
#include <math.h>
#include <strmif.h> //AM_MEDIA_TYPE defined here

#define BUFFER_SIZE (4096 * 20)

//Operations on the buffer:
#define EM_DS_BUFFER_LOCK 1
#define EM_DS_BUFFER_UNLOCK 2
#define EM_DS_BUFFER_MUTE_AND_FLUSH 3
#define EM_DS_BUFFER_START 4
#define EM_DS_BUFFER_STOP 5

const int32 EM_DS_STATE_HANDLED = 0;
const int32 EM_DS_STATE_STOPPING = 1;
const int32 EM_DS_STATE_STARTING = 2;
const int32 EM_DS_STATE_SEEKING = 3;

EMWinDirectSoundPlayback::EMWinDirectSoundPlayback(const GUID* p_upGUID, int32 p_vOutputDescriptorID)
	:	m_opDSound8(NULL),
		m_opPrimaryDSoundBuffer(NULL),
		m_opDSoundBuffer(NULL),
//		m_opDSNotify(NULL),
		m_vWriteCursor(0),
		m_opDSCache(NULL),
		m_opThread(NULL),
		m_vOutputDescriptorID(p_vOutputDescriptorID),
		m_vReadAhead(0),
		m_opMediaTimer(NULL),
		m_opSettingsRepository(NULL),
		m_opMediaUtility(NULL),
		m_vLastReadCursor(0),
		m_uConsumerContinueSignal(0),
		m_uProducerContinueSignal(0),
		m_uAbortSignal(0),
		m_opProtectionSem(NULL),
		m_vIsInitialized(false),

		m_vStarted(false),
		m_vSeeked(false),
		m_vBuffering(false),
		m_vBytesToPlay(0),

		m_vFirstRun(true),
		m_vIsShuttingDown(false),
		m_vTotalRead(0),
		m_vTotalWritten(0),
		m_oOutputName("Unknown audio output device"),
		m_vLastTime(0),
		m_vThreadWaiting(false)
{
	EMApplication::Instance() -> AddListener(this); //For listening for quit-messages
	memcpy(&m_uDeviceGUID, p_upGUID, sizeof(GUID));
	HRESULT vResult = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if(FAILED(vResult))
		MessageBox(NULL, "ERROR! Failed to initialize COM system in ~EMWinMIDISystem!", "Error!", MB_OK);

	m_opMediaTimer = EMMediaTimer::Instance();
	m_opSettingsRepository = EMMediaEngine::Instance() -> GetSettingsRepository();
	m_opMediaUtility = EMMediaUtility::Instance();
	m_opSettingsRepository -> AddListener(this);
	m_opMediaTimer -> AddListener(this);

	m_uConsumerContinueSignal	=	CreateEvent(NULL, FALSE, FALSE, string(string("EMDirectSoundConsumerSignal") + EMMediaIDManager::MakeUniqueString()).c_str());
	m_uProducerContinueSignal	=	CreateEvent(NULL, FALSE, FALSE, string(string("EMDirectSoundProducerSignal") + EMMediaIDManager::MakeUniqueString()).c_str());
	m_uAbortSignal				=	CreateEvent(NULL, TRUE, FALSE, string(string("DSAbortSignal") + EMMediaIDManager::MakeUniqueString()).c_str());

	m_opProtectionSem = EMSemaphore::CreateEMSemaphore(string(string("EMDirectSoundPlaybackProtSem") + EMMediaIDManager::MakeUniqueString()).c_str(), 1, 1);
}

EMWinDirectSoundPlayback::~EMWinDirectSoundPlayback()
{
	m_oOutputName = "";
	EMApplication::Instance() -> RemoveListener(this); //Stop listening for quit-messages
	try
	{
		m_opSettingsRepository -> RemoveListener(this);
		m_opMediaTimer -> RemoveListener(this);
	}
	catch(...) 
	{
		MessageBox(NULL, "Error while cleaning up DirectSound objects.", "Error!", MB_OK);
	}
	CoUninitialize();
}

int32 EMWinDirectSoundPlayback::Capacity()
{
	int32 vSize = m_opDSCache -> SizeNTS();
	return vSize;
}

int32 EMWinDirectSoundPlayback::Used()
{
	int32 vUsed = m_opDSCache -> UsedNTS();
	return vUsed;
}

void EMWinDirectSoundPlayback::SetName(const char* p_vpOutputName)
{
	m_oOutputName = string(p_vpOutputName);
}

const char* EMWinDirectSoundPlayback::GetName() const
{
	return m_oOutputName.c_str();
}

void EMWinDirectSoundPlayback::CleanUpE()
{
	m_opProtectionSem -> Acquire();

	m_vIsShuttingDown = true;

	if(m_opDSCache != NULL)
		m_opDSCache -> FlushNTS();
	
//	if(m_opThread != NULL)
//	{
		m_vIsShuttingDown = true;
		m_opProtectionSem -> Release();
		CloseHandle(m_uConsumerContinueSignal);
		CloseHandle(m_uProducerContinueSignal);
		CloseHandle(m_uAbortSignal);
//	}
//	else
//		m_opProtectionSem -> Release();

	delete m_opDSCache;
	m_opDSCache = NULL;
}

//IMPORTANT: This has to be called before we create the secondary buffer!!!!
bool EMWinDirectSoundPlayback::SetPrimaryBufferFormat(float p_vFrameRate, uint32 p_vChannels, uint32 p_vBitDepth)
{
	DSBUFFERDESC uPrimaryBuffDesc;
	WAVEFORMATEX uPrimaryWaveFormatEx;
	//LPDIRECTSOUNDBUFFER upDSPrimaryBuffer = NULL;

	memset(&uPrimaryBuffDesc, 0, sizeof(DSBUFFERDESC));
	memset(&uPrimaryWaveFormatEx, 0, sizeof(WAVEFORMATEX));

	uPrimaryBuffDesc.dwSize = sizeof(DSBUFFERDESC);
	uPrimaryBuffDesc.dwFlags = DSBCAPS_PRIMARYBUFFER;
	uPrimaryBuffDesc.dwBufferBytes = 0;
	uPrimaryBuffDesc.lpwfxFormat = NULL;

	uPrimaryWaveFormatEx.wFormatTag = WAVE_FORMAT_PCM;
	uPrimaryWaveFormatEx.nChannels = static_cast<int32>(p_vChannels);
	uPrimaryWaveFormatEx.nSamplesPerSec = static_cast<int32>(p_vFrameRate);
	uPrimaryWaveFormatEx.wBitsPerSample = static_cast<int32>(p_vBitDepth);
	uPrimaryWaveFormatEx.nBlockAlign = uPrimaryWaveFormatEx.wBitsPerSample * uPrimaryWaveFormatEx.nChannels / 8;
	uPrimaryWaveFormatEx.nAvgBytesPerSec = uPrimaryWaveFormatEx.nBlockAlign * uPrimaryWaveFormatEx.nSamplesPerSec;

	HRESULT vResult = m_opDSound8 -> CreateSoundBuffer(&uPrimaryBuffDesc, &m_opPrimaryDSoundBuffer, NULL);
	if(vResult != DS_OK)
	{
		switch(vResult)
		{
		case DSERR_ALLOCATED:
			MessageBox(NULL, "Failed to change the format of the primary direct sound buffer!\n\nReason: The request failed because resources, such as a priority level, were already in use by another caller.\n\nSystem will use default audio format (22kHz 8bit stereo)", "Error!", MB_OK | MB_ICONSTOP);
			break;
		case DSERR_CONTROLUNAVAIL:
			MessageBox(NULL, "Failed to change the format of the primary direct sound buffer!\n\nReason: The buffer control (volume, pan, and so on) requested by the caller is not available.\n\nSystem will use default audio format (22kHz 8bit stereo)", "Error!", MB_OK | MB_ICONSTOP);
			break;
		case DSERR_BADFORMAT:
			MessageBox(NULL, "Failed to change the format of the primary direct sound buffer!\n\nReason: The specified wave format is not supported.\n\nSystem will use default audio format (22kHz 8bit stereo)", "Error!", MB_OK | MB_ICONSTOP);
			break;
		case DSERR_BUFFERTOOSMALL:
			MessageBox(NULL, "Failed to change the format of the primary direct sound buffer!\n\nReason: The buffer size is not great enough to enable effects processing.\n\nSystem will use default audio format (22kHz 8bit stereo)", "Error!", MB_OK | MB_ICONSTOP);
			break;
		case DSERR_DS8_REQUIRED:
			MessageBox(NULL, "Failed to change the format of the primary direct sound buffer!\n\nReason: A DirectSound object of class CLSID_DirectSound8 or later is required for the requested functionality.\n\nSystem will use default audio format (22kHz 8bit stereo)", "Error!", MB_OK | MB_ICONSTOP);
			break;
		case DSERR_INVALIDCALL:
			MessageBox(NULL, "Failed to change the format of the primary direct sound buffer!\n\nReason: This function is not valid for the current state of this object.\n\nSystem will use default audio format (22kHz 8bit stereo)", "Error!", MB_OK | MB_ICONSTOP);
			break;
		case DSERR_INVALIDPARAM:
			MessageBox(NULL, "Failed to change the format of the primary direct sound buffer!\n\nReason: An invalid parameter was passed to the returning function.\n\nSystem will use default audio format (22kHz 8bit stereo)", "Error!", MB_OK | MB_ICONSTOP);
			break;
		case DSERR_NOAGGREGATION:
			MessageBox(NULL, "Failed to change the format of the primary direct sound buffer!\n\nReason: The object does not support aggregation.\n\nSystem will use default audio format (22kHz 8bit stereo)", "Error!", MB_OK | MB_ICONSTOP);
			break;
		case DSERR_OUTOFMEMORY:
			MessageBox(NULL, "Failed to change the format of the primary direct sound buffer!\n\nReason: The DirectSound subsystem could not allocate sufficient memory to complete the caller's request.\n\nSystem will use default audio format (22kHz 8bit stereo)", "Error!", MB_OK | MB_ICONSTOP);
			break;
		case DSERR_UNINITIALIZED:
			MessageBox(NULL, "Failed to change the format of the primary direct sound buffer!\n\nReason: The IDirectSound8::Initialize method has not been called or has not been called successfully before other methods were called.\n\nSystem will use default audio format (22kHz 8bit stereo)", "Error!", MB_OK | MB_ICONSTOP);
			break;
		case DSERR_UNSUPPORTED:
			MessageBox(NULL, "Failed to change the format of the primary direct sound buffer!\n\nReason: The function called is not supported at this time.\n\nSystem will use default audio format (22kHz 8bit stereo)", "Error!", MB_OK | MB_ICONSTOP);
			break;
		default:
			MessageBox(NULL, "Failed to access the primary direct sound buffer!\n\nReason: Don't know why!\n\nSystem will use default audio format (22kHz 8bit stereo)", "Error!", MB_OK | MB_ICONSTOP);
		};
		return false;
	}
	else
	{
		vResult = m_opPrimaryDSoundBuffer -> SetFormat(&uPrimaryWaveFormatEx);
		if(vResult != DS_OK)
		{
			switch(vResult)
			{
			case DSERR_BADFORMAT:
				MessageBox(NULL, "Failed to change the format of the primary direct sound buffer!\n\nReason: Bad format specified!\n\nSystem will use default audio format (22kHz 8bit stereo)", "Error!", MB_OK | MB_ICONSTOP);
				break;
			case DSERR_INVALIDCALL:
				MessageBox(NULL, "Failed to change the format of the primary direct sound buffer!\n\nReason: Invalid call!\n\nSystem will use default audio format (22kHz 8bit stereo)", "Error!", MB_OK | MB_ICONSTOP);
				break;
			case DSERR_INVALIDPARAM:
				MessageBox(NULL, "Failed to change the format of the primary direct sound buffer!\n\nReason: Invalid parameter!\n\nSystem will use default audio format (22kHz 8bit stereo)", "Error!", MB_OK | MB_ICONSTOP);
				break;
			case DSERR_OUTOFMEMORY:
				MessageBox(NULL, "Failed to change the format of the primary direct sound buffer!\n\nReason: Amnesia (out of memory)!\n\nSystem will use default audio format (22kHz 8bit stereo)", "Error!", MB_OK | MB_ICONSTOP);
				break;
			case DSERR_PRIOLEVELNEEDED:
				MessageBox(NULL, "Failed to change the format of the primary direct sound buffer!\n\nReason: Application has too humble cooperation level!\n\nSystem will use default audio format (22kHz 8bit stereo)", "Error!", MB_OK | MB_ICONSTOP);
				break;
			case DSERR_UNSUPPORTED:
				MessageBox(NULL, "Failed to change the format of the primary direct sound buffer!\n\nReason: Unsupported!\n\nSystem will use default audio format (22kHz 8bit stereo)", "Error!", MB_OK | MB_ICONSTOP);
				break;
			default:
				MessageBox(NULL, "Failed to change the format of the primary direct sound buffer!\n\nReason: Sorry. Don't know why!\n\nSystem will use default audio format (22kHz 8bit stereo)", "Error!", MB_OK | MB_ICONSTOP);
				break;
			};
			return false;
		}
	}
	return true;
}

bool EMWinDirectSoundPlayback::SetOutputFormat(float p_vFrameRate, uint32 p_vChannels, uint32 p_vBitDepth)
{
	DSBUFFERDESC uBuffDesc;
	memset(&uBuffDesc, 0, sizeof(DSBUFFERDESC));
	try
	{
		WAVEFORMATEX* upWaveFormatEx = EM_new WAVEFORMATEX;
		memset(upWaveFormatEx, 0, sizeof(WAVEFORMATEX));
		upWaveFormatEx -> wFormatTag = WAVE_FORMAT_PCM;
		upWaveFormatEx -> nChannels = static_cast<int32>(p_vChannels);
		upWaveFormatEx -> nSamplesPerSec = static_cast<int32>(p_vFrameRate);
		upWaveFormatEx -> wBitsPerSample = static_cast<int32>(p_vBitDepth);
		upWaveFormatEx -> nBlockAlign = static_cast<WORD>((upWaveFormatEx -> wBitsPerSample * upWaveFormatEx -> nChannels) / 8);
		upWaveFormatEx -> nAvgBytesPerSec = upWaveFormatEx -> nBlockAlign * upWaveFormatEx -> nSamplesPerSec;
		upWaveFormatEx -> cbSize = 0;

		uBuffDesc.dwSize = sizeof(uBuffDesc);
		uBuffDesc.dwFlags = /*DSBCAPS_CTRLPOSITIONNOTIFY |*/ DSBCAPS_LOCDEFER | DSBCAPS_GLOBALFOCUS /*| DSBCAPS_GETCURRENTPOSITION2*/ /*| DSBCAPS_STICKYFOCUS | DSBCAPS_CTRLPOSITIONNOTIFY */;
		uBuffDesc.dwBufferBytes = (BUFFER_SIZE < DSBSIZE_MIN ? DSBSIZE_MIN : (BUFFER_SIZE > DSBSIZE_MAX ? DSBSIZE_MAX : BUFFER_SIZE));
		uBuffDesc.lpwfxFormat = upWaveFormatEx;
		uBuffDesc.dwReserved = 0;
	}
	catch(...)
	{
		return false;
	}

	if(m_opDSoundBuffer != NULL)
	{
		m_opDSoundBuffer -> Stop();
		m_opDSoundBuffer -> Release();
	}

	HRESULT vResult = m_opDSound8 -> CreateSoundBuffer(&uBuffDesc, &m_opDSoundBuffer, NULL);
	if(FAILED(vResult)) { EMMediaDebugLog::Instance() -> Log("ERROR! Failed to create buffer!"); return false; }

	m_vReadAhead = 750; //*static_cast<int32*>(m_opSettingsRepository -> GetSetting(SETTING_AUDIO_PROCESSING_AHEAD));
	EMMediaFormat* opSystemAudioFormat = m_opMediaUtility -> GetSystemAudioFormat();
	int64 vReadAheadFrames = m_opMediaUtility -> TimeToFrames(m_vReadAhead * 1000, opSystemAudioFormat);
	int32 vBytesReadAhead = m_opMediaUtility -> FramesToBytes(vReadAheadFrames, opSystemAudioFormat);
	int64 vAudioBufferSize = static_cast<int64>(*static_cast<int32*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_AUDIO_BUFFER_SIZE)));
	if(m_opDSCache != NULL)
	{
		delete m_opDSCache;
		m_opDSCache = NULL;
	}
	m_opDSCache = EM_new EMDSCache((vBytesReadAhead - (vBytesReadAhead % vAudioBufferSize)) + vAudioBufferSize * 40);
	return true;
}

bool EMWinDirectSoundPlayback::InitCheckE()
{
	m_opProtectionSem -> Acquire();
	if(m_vIsInitialized)
	{
		m_opProtectionSem -> Release();
		return true;
	}

	if(m_uDeviceGUID == GUID_NULL)
	{
		m_opProtectionSem -> Release();
		return false;
	}
	
	HRESULT vResult = CoCreateInstance(CLSID_DirectSound, NULL, CLSCTX_INPROC, IID_IDirectSound, reinterpret_cast<void**>(&m_opDSound8));
	if(FAILED(vResult)) { m_opProtectionSem -> Release(); EMMediaDebugLog::Instance() -> Log("ERROR! Could not create DirectSound object!"); return false; }

	try
	{
		vResult = m_opDSound8 -> Initialize(&m_uDeviceGUID);
		if(FAILED(vResult)) { m_opProtectionSem -> Release(); EMMediaDebugLog::Instance() -> Log("ERROR! Failed to initialize DirectSound output with GUID!"); return false; }
	}
	catch(...)
	{
		m_opProtectionSem -> Release();
		return false;
	}

	try
	{
		uint32 vInt = 1; //1 = main window.
		HWND uWindowHandle = static_cast<HWND>(EMMediaEngine::Instance() -> GetCommandRepository() -> ExecuteCommand(COMMAND_GET_APPLICATION_DATA, &vInt, NULL, NULL));
		HRESULT vResult = m_opDSound8 -> SetCooperativeLevel(uWindowHandle, DSSCL_PRIORITY);
		if(FAILED(vResult)) { m_opProtectionSem -> Release(); EMMediaDebugLog::Instance() -> Log("ERROR! Failed to set cooperative level!"); return false; }
	}
	catch(...)
	{
		m_opProtectionSem -> Release();
		return false;
	}

	m_opThread = EMThread::CreateEMThread(string(string("DSProdThread") + EMMediaIDManager::MakeUniqueString()).c_str(), EM_THREAD_MEDIUM_PRIORITY, 0); 
	m_opThread -> AddListener(this);
	m_opThread -> StartSuspended();

	if(! SetPrimaryBufferFormat(44100, 2, 16))
	{
		m_opProtectionSem -> Release();
		return false;
	}

	if(! SetOutputFormat(44100 /**static_cast<int32*>(m_opSettingsRepository -> GetSetting(SETTING_AUDIO_FRAMERATE))*/, 2/**static_cast<int32*>(m_opSettingsRepository -> GetSetting(SETTING_AUDIO_NUM_CHANNELS))*/, 16))
	{
		m_opProtectionSem -> Release();
		return false;
	}

/*	int32 vNumNotifies = BUFFER_SIZE / 512;
	m_upNotifyArray = EM_new DSBPOSITIONNOTIFY[vNumNotifies];
	m_uNotifyEventArray = EM_new HANDLE[vNumNotifies];
	
	for(int32 vIndex = 0; vIndex < vNumNotifies; vIndex++)
	{
		m_upNotifyArray[vIndex].dwOffset = vIndex * 512;
		HANDLE uEvent = CreateEvent(NULL, FALSE, FALSE, string(string("DSNotifyEvent") + EMMediaIDManager::MakeUniqueString()).c_str());
		m_uNotifyEventArray[vIndex] = uEvent;
		m_upNotifyArray[vIndex].hEventNotify = uEvent;
	}
	vResult = m_opDSNotify -> SetNotificationPositions(vNumNotifies, m_upNotifyArray);
	if(vResult != S_OK)
	{
	} */

	BufferOperation(EM_DS_BUFFER_MUTE_AND_FLUSH);
	m_vIsInitialized = true;
	m_opProtectionSem -> Release();
	m_opThread -> Resume();

	return true;
}

/*
int64 EMWinDirectSoundPlayback::Sine(float p_vFrequency, int64 p_vFrameNo, int16* p_vpBuffer, int64 p_vBufferLengthSamples, int8 p_vChannels)
{
	const float vcPI = 3.1415926535;
	for(int16 i = 0; i < p_vBufferLengthSamples / p_vChannels; i += p_vChannels)
	{
		int64 vRelFrame = i + p_vFrameNo;
		float vAmplitude = 32768.0 * sin((p_vFrequency * 2.0 * vcPI / 44100.0) * static_cast<float>(vRelFrame));
		int16 vSample = static_cast<int16>(vAmplitude);
		p_vpBuffer[i] = p_vpBuffer[i + 1] = vSample;
	}
	p_vFrameNo += (p_vBufferLengthSamples * p_vChannels);
	return p_vFrameNo;
}
*/

void EMWinDirectSoundPlayback::OnThreadCreated(EMThread* p_opThread)
{
	HRESULT vResult = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if(FAILED(vResult))
		MessageBox(NULL, "ERROR! Failed to initialize COM system in ~EMWinMIDISystem!", "Error!", MB_OK);
}

void EMWinDirectSoundPlayback::OnThreadKilled(EMThread* p_opThread)
{
	CoUninitialize();
}

uint32 EMWinDirectSoundPlayback::GetMaxWriteSize(bool p_vUseDSWritePointer)
{
	uint32 vWriteCursor = 0;
	uint32 vPlayCursor = 0;
	uint32 vMaxSize = 0;

	//Get current play position
	if(m_opDSoundBuffer -> GetCurrentPosition(&vPlayCursor, &vWriteCursor) == DS_OK)
	{
		uint32 vUsed = m_opDSCache -> UsedNTS();
		
		if((! p_vUseDSWritePointer && m_vWriteCursor <= vPlayCursor) || (p_vUseDSWritePointer && vWriteCursor <= vPlayCursor))
		{	//Our write position trails play cursor
			vMaxSize = vPlayCursor - (p_vUseDSWritePointer ? vWriteCursor : m_vWriteCursor);
			if(vMaxSize > vUsed)
				vMaxSize = vUsed;
		}
		else //(m_vWriteCursor > vPlayCursor)
		{	//Play cursor has wrapped
			vMaxSize = BUFFER_SIZE - (p_vUseDSWritePointer ? vWriteCursor : m_vWriteCursor) + vPlayCursor;
			if(vMaxSize > vUsed)
				vMaxSize = vUsed;
		}
	}
	else
		return 0;
	return vMaxSize;
}

void EMWinDirectSoundPlayback::ThreadRun(EMThread* p_opThread)
{
	m_opProtectionSem -> Acquire();
	if(m_vIsShuttingDown)
	{
		m_opProtectionSem -> Release();
		return;
	}

	if(m_vBuffering)
	{
		EMMediaFormat* opSystemAudioFormat = m_opMediaUtility -> GetSystemAudioFormat();
		int64 vReadAheadFrames = m_opMediaUtility -> TimeToFrames(m_vReadAhead * 1000, opSystemAudioFormat);
		int32 vBytesReadAhead = m_opMediaUtility -> FramesToBytes(vReadAheadFrames, opSystemAudioFormat);

		//The thread should go to sleep if the used cache smaller than the read-ahead limit...
		if(m_opDSCache -> SizeNTS() >= vBytesReadAhead && m_opDSCache -> UsedNTS() < vBytesReadAhead)
		{
			HANDLE uHandles[2];
			uHandles[0] = m_uConsumerContinueSignal;
			uHandles[1] = m_uAbortSignal;
			m_vThreadWaiting = true;
			m_opProtectionSem -> Release();

			PulseEvent(m_uProducerContinueSignal);

			HRESULT vResult = WaitForMultipleObjects(2, uHandles, FALSE, INFINITE);
			m_vThreadWaiting = false;
			return;
		}
		else
		{
			m_vBuffering = false;
			m_vStarted = true;
			m_vSeeked = false;
			m_vBytesToPlay = 0;
			m_vFirstRun = true;
			m_opProtectionSem -> Release();
		}
		PulseEvent(m_uProducerContinueSignal);
		return;
	}
	else if(m_vStarted)
	{
		char* vpSourceData = NULL;
		LPVOID upDataArray1 = NULL;
		uint32  vDataArray1Size = 0;
		LPVOID upDataArray2 = NULL;
		uint32 vDataArray2Size = 0;

		//The thread should also go to sleep if the used cache equals zero (ie there's no data whatsoever in there)...
		if(m_opDSCache -> UsedNTS() <= 0)
		{
			HANDLE uHandles[2];
			uHandles[0] = m_uConsumerContinueSignal;
			uHandles[1] = m_uAbortSignal;
			m_vThreadWaiting = true;
			m_opProtectionSem -> Release();

			HRESULT vResult = WaitForMultipleObjects(2, uHandles, FALSE, INFINITE);
			m_vThreadWaiting = false;
			return;
		}

		if(BufferOperation(EM_DS_BUFFER_LOCK, &upDataArray1, &vDataArray1Size, &upDataArray2, &vDataArray2Size))
		{
			int32 vNumActuallyRead = 0;
			m_opDSCache -> ReadNTS(static_cast<char*>(upDataArray1), vDataArray1Size, &vNumActuallyRead);
			m_vTotalWritten += vNumActuallyRead;
			if(vDataArray2Size > 0)
			{
				m_opDSCache -> ReadNTS(static_cast<char*>(upDataArray2), vDataArray2Size, &vNumActuallyRead);
				m_vTotalWritten += vNumActuallyRead;
			}

			PulseEvent(m_uProducerContinueSignal);

			uint32 vNumBytesPlayedSinceLastTime = 0;
			uint32 vPosition = 0;
			
			HRESULT vResult = m_opDSoundBuffer -> GetCurrentPosition(&vPosition, NULL);

			int32 vNowTime = timeGetTime();
			int32 vDeltaTime = vNowTime - m_vLastTime;
			m_vLastTime = vNowTime;

			if(m_vLastReadCursor == -1)
				m_vLastReadCursor = vPosition;

			if(vPosition >= m_vLastReadCursor)
			{
				vNumBytesPlayedSinceLastTime = vPosition - m_vLastReadCursor;
				m_vLastReadCursor = vPosition;
			}
			else
			{
				vNumBytesPlayedSinceLastTime = (BUFFER_SIZE - m_vLastReadCursor) + vPosition;
				m_vLastReadCursor = vPosition;
			}

			m_vTotalRead += vNumBytesPlayedSinceLastTime;

			if(m_vTotalRead > m_vTotalWritten && ! m_vSeeked)
			{
				BufferOperation(EM_DS_BUFFER_UNLOCK, upDataArray1, &vDataArray1Size, upDataArray2, &vDataArray2Size);
				m_opProtectionSem -> Release();
				//EMMediaEngine::Instance() -> GetCommandRepository() -> ExecuteCommand(COMMAND_STOP);
				int32 vCommandStop = COMMAND_STOP;
				EMMediaEngine::Instance() -> GetCommandRepository() -> ExecuteCommand(COMMAND_POST_COMMAND, &vCommandStop);

				m_vTotalRead = m_vTotalWritten = 0;	
//				MessageBox(NULL, "WARNING - Audio buffer underrun!\n\nYour system was overloaded and failed to produce audio buffers as fast,\nor faster than, the audio playback required! This failure is generally\ncaused by one or more of the following reasons:\n1.\tYou have too many applications running. Close as many as you\n\tcan to save performance.\n2.\tYou have too many effects, video transitions or other media\n\tobjects in your projects for your computer to handle in \n\trealtime. Try minimizing the video preview format (Project settings)\n\tto save performance.\n3.\tThe media files you are using in your project is of too\n\thigh quality (such as DivX or DV format) which requires too much\n\tCPU time to decode in realtime for your computer to handle.\n4.\tYour computer does not meet the minimum requirements.\n", string(string("Audio buffer underrun in ") + m_oOutputName + string("!")).c_str(), MB_OK | MB_ICONWARNING);
				EMMediaEngine::Instance() -> GetCommandRepository() -> ExecuteCommand(COMMAND_WRITE_STATUS_BAR_MESSAGE, (void*) string("Error: Audio buffer underrun!").c_str());
				return;
			}
			else
			{
				int64 vFrames = m_opMediaUtility -> BytesToFrames(vNumBytesPlayedSinceLastTime, m_opMediaUtility -> GetSystemAudioFormat());
 				m_opMediaTimer -> IncreaseNowFrame(vFrames, m_vOutputDescriptorID);
			}
			BufferOperation(EM_DS_BUFFER_UNLOCK, upDataArray1, &vDataArray1Size, upDataArray2, &vDataArray2Size);
		}
		else
		{
		}

		if(m_vSeeked)
		{
			m_vBytesToPlay -= (vDataArray1Size + vDataArray2Size);
			if(m_vBytesToPlay <= 0)
			{
				m_vBuffering = false;
				m_vSeeked = true;
				m_vStarted = false;
			}
		}

		m_opProtectionSem -> Release();
		PulseEvent(m_uProducerContinueSignal);

		::Sleep(21);
		return;
	}
	else if(m_vSeeked)
	{
		m_vBuffering = false;
		m_vStarted = false;
		m_vSeeked = false;

		BufferOperation(EM_DS_BUFFER_MUTE_AND_FLUSH);

		m_vLastReadCursor = -1;

		m_opProtectionSem -> Release();
		PulseEvent(m_uProducerContinueSignal);
		return;
	}
	else //Idle
	{
		BufferOperation(EM_DS_BUFFER_MUTE_AND_FLUSH);

		HANDLE uHandles[2];
		uHandles[1] = m_uConsumerContinueSignal;
		uHandles[0] = m_uAbortSignal;
		m_vLastReadCursor = -1; 
		m_vThreadWaiting = true;
		m_opProtectionSem -> Release();

		PulseEvent(m_uProducerContinueSignal);

		BufferOperation(EM_DS_BUFFER_MUTE_AND_FLUSH);

		HRESULT vResult = WaitForMultipleObjects(2, uHandles, FALSE, INFINITE);
		m_vThreadWaiting = false;
		return;
	}
	m_opProtectionSem -> Release();
	return;
}

bool EMWinDirectSoundPlayback::Start()
{
	bool vResult = BufferOperation(EM_DS_BUFFER_START);
	return vResult;
}

bool EMWinDirectSoundPlayback::Stop()
{
	bool vResult = BufferOperation(EM_DS_BUFFER_STOP);
	return vResult;
}

bool EMWinDirectSoundPlayback::ProcessBufferE(EMMediaDataBuffer* p_opBuffer)
{
	PulseEvent(m_uConsumerContinueSignal);

	if(! m_vIsShuttingDown)
	{
		m_opProtectionSem -> Acquire();
		while(m_opDSCache -> SizeNTS() - m_opDSCache -> UsedNTS() < p_opBuffer -> m_vSizeUsed && (m_vStarted || m_vSeeked || m_vBuffering))
		{
			HANDLE uHandles[2];
			uHandles[0] = m_uProducerContinueSignal;
			uHandles[1] = m_uAbortSignal;
			m_vThreadWaiting = true;
			m_opProtectionSem -> Release();

			PulseEvent(m_uConsumerContinueSignal);

			HRESULT vResult = WaitForMultipleObjects(2, uHandles, FALSE, INFINITE);
			if(vResult != WAIT_OBJECT_0)
			{
				m_vThreadWaiting = false;
				return false;
			}
			m_vThreadWaiting = false;

			if((! m_vSeeked && ! m_vStarted && ! m_vBuffering) || m_vIsShuttingDown)
				return false;

			m_opProtectionSem -> Acquire();
		}
		m_opDSCache -> WriteNTS(static_cast<char*>(p_opBuffer -> Data()), p_opBuffer -> m_vSizeUsed);
		m_opProtectionSem -> Release();
	}

	PulseEvent(m_uConsumerContinueSignal);
	return true;
}

bool EMWinDirectSoundPlayback::MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage)
{
	switch(p_vMessage)
	{
		case EM_MESSAGE_STOP_PLAYORRECORD:
		{
			uint32 vLastReadCursor = 0;
			m_opProtectionSem -> Acquire();
			m_vTotalRead = 0;
			m_vTotalWritten = 0;
			m_vBuffering = false;
			m_vStarted = false;
			m_vSeeked = false;
			m_vFirstRun = false;
			m_vLastReadCursor = -1;
			BufferOperation(EM_DS_BUFFER_MUTE_AND_FLUSH);
			m_opProtectionSem -> Release();

			PulseEvent(m_uAbortSignal);
			break;
		}
		case EM_MESSAGE_BEGIN_BUFFER_UP:
		{
			break;
		}
		case EM_MESSAGE_BEGIN_PLAYORRECORD:
		{
			uint32 vLastReadCursor = 0;
			m_opProtectionSem -> Acquire();
			m_vTotalRead = 0;
			m_vTotalWritten = 0;
			m_vBuffering = true;
			m_vStarted = false;
			m_vSeeked = false;
			m_vFirstRun = true;
			m_vLastReadCursor = -1;
			m_opProtectionSem -> Release();

			PulseEvent(m_uAbortSignal);	
			break;
		}
		case EM_MESSAGE_TIME_WAS_SEEKED:
		{
			uint32 vLastReadCursor = 0;
			m_opProtectionSem -> Acquire();
			m_vTotalRead = 0;
			m_vTotalWritten = 0;
			m_vBuffering = false;
			m_vStarted = true;
			m_vSeeked = true;
			m_vFirstRun = true;
			m_vLastReadCursor = -1;
			m_opProtectionSem -> Release();

			PulseEvent(m_uAbortSignal);
			break;
		}
		case EM_SETTING_UPDATED:
		{
			m_opProtectionSem -> Acquire();

			int32 vReadAheadSetting = 750; //*static_cast<int32*>(m_opSettingsRepository -> GetSetting(SETTING_AUDIO_PROCESSING_AHEAD));
			if(m_vReadAhead != vReadAheadSetting)
			{
				m_vReadAhead = vReadAheadSetting;
				m_opSettingsRepository = EMMediaEngine::Instance() -> GetSettingsRepository();
				m_opMediaUtility = EMMediaUtility::Instance();
			}
			m_opProtectionSem -> Release();

			PulseEvent(m_uAbortSignal);
			break;
		}
		case EM_QUIT_PHASE_ONE:
			return false;
		case EM_QUIT_PHASE_TWO:
			break;
		case EM_QUIT_PHASE_THREE:
		{
			m_vIsShuttingDown = true;

			::Sleep(25);
			PulseEvent(m_uAbortSignal);
			::Sleep(25);
			while(m_vThreadWaiting)
			{
				PulseEvent(m_uAbortSignal);
				::Sleep(75);
			}
			m_opThread -> Suspend();
			m_opThread -> RemoveListener(this);
			delete m_opThread;
			m_opThread = NULL;
			return true;
		}
		case EM_QUIT_PHASE_FOUR:
			break;
		case EM_QUIT_PHASE_FIVE:
			break;
		case EM_QUIT_PHASE_SIX:
			break;
		case EM_QUIT_ABORTED:
			break;
		default:
			break;
	};
	return true;
}

bool EMWinDirectSoundPlayback::BufferOperation(int32 p_vOperation, void* p_upParam1, void* p_upParam2, void* p_upParam3, void* p_upParam4, void* p_upParam5)
{
	switch(p_vOperation)
	{
		case EM_DS_BUFFER_LOCK:
		{
			LPVOID upDataArray1 = NULL;
			uint32  vDataArray1Size = 0;
			LPVOID upDataArray2 = NULL;
			uint32 vDataArray2Size = 0;

			LPVOID* upArray1 = reinterpret_cast<LPVOID*>(p_upParam1);
			LPVOID* upArray2 = reinterpret_cast<LPVOID*>(p_upParam3);

			HRESULT vResult = DS_OK;
			if(m_vFirstRun)
			{
				int32 vBytesToAlloc = GetMaxWriteSize(true);
				if(vBytesToAlloc == 0)
					return false;
				vResult = m_opDSoundBuffer -> Lock(0, vBytesToAlloc, &upDataArray1, &vDataArray1Size, &upDataArray2, &vDataArray2Size, DSBLOCK_FROMWRITECURSOR);
				if(vResult != DS_OK)
				{
					MessageBox(NULL, "ERROR! Failed to get Lock on first IDirectSoundBuffer8 since playback start!", "Error!", MB_OK | MB_ICONSTOP);
					return false;
				}
				DWORD vWriteCursor = 0;
				HRESULT vResult = m_opDSoundBuffer -> GetCurrentPosition(NULL, &vWriteCursor);
				if(FAILED(vResult))
					MessageBox(NULL, "ERROR! Failed to retrieve play- and write pointers for IDirectSoundBuffer!", "Error!", MB_OK | MB_ICONSTOP);
				else
				{
					m_vWriteCursor = vWriteCursor;
					m_vFirstRun = false;
				}
			}
			else
			{
				int32 vBytesToAlloc = GetMaxWriteSize(false);
				if(vBytesToAlloc == 0)
					return false;
				vResult = m_opDSoundBuffer -> Lock(m_vWriteCursor, vBytesToAlloc, &upDataArray1, &vDataArray1Size, &upDataArray2, &vDataArray2Size, 0);
				if(vResult != DS_OK)
				{
					MessageBox(NULL, "ERROR! Failed to get Lock on other IDirectSoundBuffer8(s) since playback start!", "Error!", MB_OK | MB_ICONSTOP);
					return false;
				}
			}

			if(vResult == DS_OK)
			{
				LPVOID ptr1 = upDataArray1;
				LPVOID ptr2 = upDataArray2;
				(*upArray1) = ptr1;
				(*upArray2) = ptr2;
				*static_cast<uint32*>(p_upParam2) = vDataArray1Size;
				*static_cast<uint32*>(p_upParam4) = vDataArray2Size;
				return true;
			}
			return false;
		}
		case EM_DS_BUFFER_UNLOCK:
		{
			HRESULT vResult = m_opDSoundBuffer -> Unlock((void**) p_upParam1, *static_cast<uint32*>(p_upParam2), (void**) p_upParam3, *static_cast<uint32*>(p_upParam4));
			if(SUCCEEDED(vResult))
			{
				int64 vOld = m_vWriteCursor;
				m_vWriteCursor = (m_vWriteCursor + *static_cast<uint32*>(p_upParam2) + *static_cast<uint32*>(p_upParam4)) % BUFFER_SIZE;
			}
			return (vResult == DS_OK);
		}
		case EM_DS_BUFFER_MUTE_AND_FLUSH:
		{
			void* upDataArray1 = NULL;
			uint32  vDataArray1Size = 0;
			void* upDataArray2 = NULL;
			uint32  vDataArray2Size = 0;
			HRESULT vResult = m_opDSoundBuffer -> Lock(0, 0, reinterpret_cast<void**>(&upDataArray1), &vDataArray1Size, reinterpret_cast<void**>(&upDataArray2), &vDataArray2Size, DSBLOCK_ENTIREBUFFER);
			if(vResult == DS_OK)
			{
				memset(upDataArray1, 0, vDataArray1Size);
				if(vDataArray2Size > 0)
					memset(upDataArray2, 0, vDataArray2Size);
				vResult = m_opDSoundBuffer -> Unlock(upDataArray1, vDataArray1Size, upDataArray2, vDataArray2Size);
				m_opDSCache -> FlushNTS();
				return (vResult == DS_OK);
			}
			return false;
		}
		case EM_DS_BUFFER_START:
		{
			if(m_opDSoundBuffer != NULL)
			{
				HRESULT vResult = m_opDSoundBuffer -> Play(0, 0xFFFFFFFF, DSBPLAY_LOOPING);
				return (SUCCEEDED(vResult) == true);
			}
			else
				return false;
		}
		case EM_DS_BUFFER_STOP:
		{
			if(m_opDSoundBuffer != NULL)
			{
				HRESULT vResult = m_opDSoundBuffer -> Stop();
				return (SUCCEEDED(vResult) == true);
			}
			else
				return false;
		}
		default:
			break;
	};
	return false;
}