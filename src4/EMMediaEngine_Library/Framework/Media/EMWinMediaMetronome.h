#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#ifndef __EM_WIN_MEDIA_METRONOME

#include "EMMediaMetronome.h"

class EMRealtimeMIDIOutputDescriptor;

class EMWinMediaMetronome : public EMMediaMetronome
{
public:
	~EMWinMediaMetronome();
	void ProduceSystemBeep(int32 p_vToneFrequency);
	void ProduceMIDIBeep(EMRealtimeMIDIOutputDescriptor* p_opOutput, uint8 p_vKey, uint8 p_vVelocity, uint32 p_vMIDIChannel, int64 p_vDuration);
private:
	EMWinMediaMetronome();
	friend class EMMediaMetronome;
	bool m_vHardwareProtected;
};

#endif

#endif