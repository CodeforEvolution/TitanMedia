#include "EMMediaInputAudioSignalMeter.h"

#include "EMMediaTimer.h"
#include "EMSemaphore.h"
#include "EMSettingIDs.h"
#include "EMSettingsRepository.h"
#include "EMMediaEngine.h"
#include "EMMediaUtility.h"
#include "EMMediaFormat.h"

EMMediaInputAudioSignalMeter::EMMediaInputAudioSignalMeter()
	:	EMMediaSignalMeter(EM_TYPE_RAW_AUDIO),
		m_opSignalValues(NULL)
{
	EMMediaFormat* opFormat = EMMediaUtility::Instance() -> GetSystemAudioFormat();
	int64 vBufferSize = opFormat -> m_vBufferSizeBytes;

	m_opSignalValues = EM_new EMMediaSignal;
	m_opSignalValues -> m_vpValues[0] = m_opSignalValues -> m_vpValues[1] = 0.0;

	m_opSemaphore = EMSemaphore::CreateEMSemaphore(); //create_sem(1, "Signal meter protection sem");
}

EMMediaInputAudioSignalMeter::~EMMediaInputAudioSignalMeter()
{
	delete m_opSignalValues;
	delete m_opSemaphore;
}


void EMMediaInputAudioSignalMeter::Get(float* p_vpSamplesOut)
{
	m_opSemaphore -> Acquire();
	memcpy(p_vpSamplesOut, m_opSignalValues -> m_vpValues, 5 * sizeof(float));
	m_opSemaphore -> Release();
	return;
}

void EMMediaInputAudioSignalMeter::Put(const float* p_vpSamples, int32 p_vNumChannels)
{
	int64 vTime = EMMediaTimer::Instance() -> AudioThenFrame();
	m_opSemaphore -> Acquire();

	memcpy(m_opSignalValues -> m_vpValues, p_vpSamples, p_vNumChannels * sizeof(float));
	m_opSignalValues -> m_vTime = vTime;
	m_opSignalValues -> m_vFree = false;
	m_opSemaphore -> Release();
}

void EMMediaInputAudioSignalMeter::Clear()
{
	m_opSignalValues -> Clear();
	m_opSignalValues -> m_vpValues[0] = m_opSignalValues -> m_vpValues[1] = 0.0;
}

bool EMMediaInputAudioSignalMeter::PrepareToPlayE()
{
	return true;
}

EMMediaDataBuffer* EMMediaInputAudioSignalMeter::ProcessBufferE(list<EMMediaDataBuffer*>* p_opBufferList)
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

void EMMediaInputAudioSignalMeter::OnSeek()
{
	m_opSemaphore -> Acquire();
	Clear();
	m_opSemaphore -> Release();
}
void EMMediaInputAudioSignalMeter::OnStart()
{
	m_opSemaphore -> Acquire();
	Clear();
	m_opSemaphore -> Release();
}

void EMMediaInputAudioSignalMeter::OnStop()
{
	m_opSemaphore -> Acquire();
	Clear();
	m_opSemaphore -> Release();
}
