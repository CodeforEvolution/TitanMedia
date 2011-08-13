#include "EMGlobals.h"

#ifndef __EM_MEDIA_MIDI_SIGNAL_METER
#define __EM_MEDIA_MIDI_SIGNAL_METER

#include "EMMediaGlobals.h"
#include "EMMediaSignalMeterRepository.h"
#include "EMMediaSignalMeter.h"

class EMSemaphore;

class EMMediaMIDISignalMeter : public EMMediaSignalMeter
{
public:
	EMMediaMIDISignalMeter();
	~EMMediaMIDISignalMeter();
	void RegisterSignal(uint8 p_vLengthBytes, const uint8* p_vpEventData);
	float GetLatestSignal();

private:
	int64 m_vLastReceiveTime;
	float m_vLastValue;
	EMSemaphore* m_opSemaphore;
};

#endif
