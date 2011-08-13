#include "EMMediaAudioSignalMeter.h"

#include "EMMediaTimer.h"
#include "EMSemaphore.h"
#include "EMSettingIDs.h"
#include "EMSettingsRepository.h"
#include "EMMediaEngine.h"
#include "EMMediaUtility.h"
#include "EMMediaFormat.h"

EMMediaAudioSignalMeter::EMMediaAudioSignalMeter()
	:	EMMediaSignalMeter(EM_TYPE_RAW_AUDIO),
		m_opSignalArray(NULL),
		m_opZeroSignal(NULL),
		m_vMax(0),
		m_vMostRecentPut(0),
		m_vMostRecentGet(0)
//		m_vIsPaused(true)
{
	EMMediaFormat* opFormat = EMMediaUtility::Instance() -> GetSystemAudioFormat();
	int64 vBufferSize = opFormat -> m_vBufferSizeBytes;

	float vAudioFrameRate = (float) *(static_cast<int32*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_AUDIO_FRAMERATE)));
	int32 vNumSignals = 2 * static_cast<int32>(25 + EM_AUDIO_READAHEAD * (vAudioFrameRate) / (vBufferSize / (EM_AUDIO_SAMPLESIZE * EM_AUDIO_NUM_CHANNELS)));
	m_opSignalArray = EM_new EMMediaSignal*[vNumSignals];
	for(int32 vIndex = 0; vIndex < vNumSignals; vIndex++)
		m_opSignalArray[vIndex] = EM_new EMMediaSignal();
	m_opZeroSignal = EM_new EMMediaSignal();
	m_opZeroSignal -> m_vpValues[0] = m_opZeroSignal -> m_vpValues[1] = 0.0;
	
	m_vMax = vNumSignals;
	m_opSemaphore = EMSemaphore::CreateEMSemaphore(); //create_sem(1, "Signal meter protection sem");
}

EMMediaAudioSignalMeter::~EMMediaAudioSignalMeter()
{
	for(int32 vIndex = 0; vIndex < m_vMax; vIndex++)
		delete m_opSignalArray[vIndex];
	delete [] m_opSignalArray;
	delete m_opZeroSignal;
	delete m_opSemaphore;
}

void EMMediaAudioSignalMeter::Get(float* p_vpSamplesOut)
{
	if(EMMediaTimer::Instance() -> GetState() != EM_PLAYING && EMMediaTimer::Instance() -> GetState() != EM_RECORDING)
	{
		memcpy(p_vpSamplesOut, m_opZeroSignal -> m_vpValues, 5 * sizeof(float));
		return;
	}

	m_opSemaphore -> Acquire();
	if(m_vMostRecentGet % m_vMax == m_vMostRecentPut % m_vMax)
	{
		memcpy(p_vpSamplesOut, m_opZeroSignal -> m_vpValues, 5 * sizeof(float));
		m_opSemaphore -> Release();
		return;
	}

	int64 vTime = EMMediaTimer::Instance() -> NowFrame() - EMMediaUtility::Instance() -> FramesToTime(64, EMMediaUtility::Instance() -> GetSystemAudioFormat());
	for(int64 vIndex = m_vMostRecentGet; vIndex < m_vMostRecentPut && (EMMediaTimer::Instance() -> GetState() == EM_PLAYING || EMMediaTimer::Instance() -> GetState() == EM_RECORDING); vIndex++)
	{
		if(m_opSignalArray[vIndex % m_vMax] -> m_vTime < vTime && (EMMediaTimer::Instance() -> GetState() == EM_PLAYING || EMMediaTimer::Instance() -> GetState() == EM_RECORDING))
		{
			m_opSignalArray[vIndex % m_vMax] -> Clear();
			m_vMostRecentGet++;
		}
		else if(m_opSignalArray[vIndex % m_vMax] -> m_vTime >= vTime && (EMMediaTimer::Instance() -> GetState() == EM_PLAYING || EMMediaTimer::Instance() -> GetState() == EM_RECORDING))
		{
			memcpy(p_vpSamplesOut, m_opSignalArray[vIndex % m_vMax] -> m_vpValues, 5 * sizeof(float));
			m_opSemaphore -> Release();
			return;
		}
	}
	memset(p_vpSamplesOut, 0, 5 * sizeof(float));
	m_opSemaphore -> Release();
}

void EMMediaAudioSignalMeter::Put(const float* p_vpSamples, int32 p_vNumChannels)
{
	int64 vTime = EMMediaTimer::Instance() -> AudioThenFrame();
	m_opSemaphore -> Acquire();
	if(EMMediaTimer::Instance() -> GetState() != EM_PLAYING && EMMediaTimer::Instance() -> GetState() != EM_RECORDING)
	{
		m_opSemaphore -> Release();
		return; 
	}

	if(m_opSignalArray[m_vMostRecentPut % m_vMax] -> m_vFree)
	{
		memcpy(m_opSignalArray[m_vMostRecentPut % m_vMax] -> m_vpValues, p_vpSamples, p_vNumChannels * sizeof(float));
		m_opSignalArray[m_vMostRecentPut % m_vMax] -> m_vTime = vTime;
		m_opSignalArray[m_vMostRecentPut % m_vMax] -> m_vFree = false;
		m_vMostRecentPut++;
	}
	else
	{
		for(int32 vIndex = 0; vIndex < m_vMax; vIndex++)
		{ 
			if(m_opSignalArray[vIndex] -> m_vTime < EMMediaTimer::Instance() -> NowFrame())
				m_opSignalArray[vIndex] -> Clear();
			m_vMostRecentGet = vIndex;
		}
	}

	m_opSemaphore -> Release();
}

void EMMediaAudioSignalMeter::Clear()
{
	for(int32 vIndex = 0; vIndex < m_vMax; vIndex++)
		m_opSignalArray[vIndex] -> Clear();
	m_vMostRecentGet = 0;
	m_vMostRecentPut = 0;
}

bool EMMediaAudioSignalMeter::PrepareToPlayE()
{
	return true;
}

EMMediaDataBuffer* EMMediaAudioSignalMeter::ProcessBufferE(list<EMMediaDataBuffer*>* p_opBufferList)
{
	if(p_opBufferList -> size() != 1)
		EMDebugger("SignalMeter received an unsupported number of buffers (not 1)");
	EMMediaDataBuffer* opBuffer = p_opBufferList -> front();
	
	//We start by finding the peaks in the buffer. One peak for each channel!
	signed short* vpAllSamples = static_cast<signed short*>(opBuffer -> Data());
	float vpMaxSamples[5];
	for(int32 vIndex = 0; vIndex < 5; vIndex++)
		vpMaxSamples[vIndex] = 0;
	int32 vNumChannels = 2;
	int32 vSampleSize = 2; //bytes per sample
	for(vIndex = 0; vIndex < opBuffer -> m_vSizeUsed / vSampleSize; vIndex += vNumChannels)
	{
		for(int vChannel = 0; vChannel < vNumChannels; vChannel++)
		{
			float vCurrentFloatValue = static_cast<float>(abs(vpAllSamples[vIndex + vChannel])) / static_cast<float>(32768.0);
			if(vpMaxSamples[vChannel] < vCurrentFloatValue)
				vpMaxSamples[vChannel] = vCurrentFloatValue;
		}
	}
	Put(vpMaxSamples, vNumChannels);
	
	if(GetDestination() == NULL)
	{
		opBuffer -> Recycle();
		return NULL;
	}

	opBuffer -> m_opDestination = GetDestination();
	return opBuffer;
}

EMMediaDataBuffer* EMMediaAudioSignalMeter::ProcessBufferE(EMMediaDataBuffer** p_opBufferList)
{
	return NULL;
}

void EMMediaAudioSignalMeter::OnSeek()
{
	m_opSemaphore -> Acquire();
	Clear();
	m_opSemaphore -> Release();
}
void EMMediaAudioSignalMeter::OnStart()
{
//	m_opSemaphore -> Acquire();
//	m_opSemaphore -> Release();
}

void EMMediaAudioSignalMeter::OnStop()
{
	m_opSemaphore -> Acquire();
	Clear();
	m_opSemaphore -> Release();
}
