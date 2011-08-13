/*******************************************************
* Creator: Martin Johansson
* Portability: Non-Native
*-------------------------------------------------------
*
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_MEDIA_INPUT_AUDIO_SIGNAL_METER
#define __EM_MEDIA_INPUT_AUDIO_SIGNAL_METER

#include "EMMediaGlobals.h"

#include "EMMediaSignalMeter.h"
#include "EMMediaAudioSignalMeter.h"
#include "EMMediaSignalMeterRepository.h"

class EMMediaInputAudioSignalMeter : public EMMediaSignalMeter //, public EMListener
{
public:
	EMMediaInputAudioSignalMeter();
	virtual ~EMMediaInputAudioSignalMeter();
	void Get(float* p_vpSamplesOut);
	void Put(const float* p_vpSamples, int32 p_vNumChannels);
	void Clear();

	bool PrepareToPlayE();
	EMMediaDataBuffer* ProcessBufferE(list<EMMediaDataBuffer*>* p_opBufferList);

	void OnStop();
	void OnStart();
	void OnSeek();

private:
	EMMediaSignal* m_opSignalValues;
	EMMediaSignal* m_opZeroSignal;

	EMSemaphore* m_opSemaphore;

	friend class EMMediaSignalMeterRepository;
};

#endif
