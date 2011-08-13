#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#include "EMWinMediaMetronome.h"
#include "EMThread.h"
#include "EMRealtimeMIDIOutputDescriptor.h"
#include "EMWinDXMIDIProducer.h"
#include "EMMediaTimer.h"

#include <conio.h>
#include <process.h>

#define METRONOME_TIMER_FREQ 1193180L   /* Timer freq = 1.99 MHz */
#define METRONOME_TIMER_COUNT 0x42      /* 8253 timer count */
#define METRONOME_TIMER_MODE 0x43       /* 8253 timer control port */
#define METRONOME_TIMER_OSC 0xb6        /* To use timer as oscilator */
#define OUT_8255 0x61					/* 8255 PPI output port adrs */
#define SPKRON 3						/* Bit 0 = control spkr by timer */
#define DURATION 5000					/* Microseconds duration of beep */

EMWinMediaMetronome::EMWinMediaMetronome()
	:	m_vHardwareProtected(true)
{
	OSVERSIONINFO uOSVersionInfo;
	memset(&uOSVersionInfo, 0, sizeof(uOSVersionInfo));
	uOSVersionInfo.dwOSVersionInfoSize = sizeof(uOSVersionInfo);
	GetVersionEx(&uOSVersionInfo);
	if(uOSVersionInfo.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
	{
		//If we're running on a Windows 9x-based platform we can
		//directly access the hardware through port calls.
		uOSVersionInfo.dwMajorVersion;
		uOSVersionInfo.dwMinorVersion;
		m_vHardwareProtected = false;
	}
	else if(uOSVersionInfo.dwPlatformId == VER_PLATFORM_WIN32_NT)
	{
		//However, if we're running on a NT-based platform, we cannot
		//do that since it will cause the system to scream out, we need
		//to make sure that we use a non-direct-access method against
		//the hardware. See ProduceSystemBeep!
		uOSVersionInfo.dwMajorVersion;
		uOSVersionInfo.dwMinorVersion;
		m_vHardwareProtected = true;
	}
	else
		MessageBox(NULL, "Failed to determine which platform you're running!", "Error!", MB_OK);
}

EMWinMediaMetronome::~EMWinMediaMetronome()
{
}

void EMWinMediaMetronome::ProduceMIDIBeep(EMRealtimeMIDIOutputDescriptor* p_opOutput, uint8 p_vKey, uint8 p_vVelocity, uint32 p_vMIDIChannel, int64 p_vDuration)
{
	uint8 vpData[3];
	vpData[0] = static_cast<uint8>(EM_MIDI_EVENT_TYPE_NOTE_ON);
	vpData[1] = p_vKey;
	vpData[2] = p_vVelocity;
	int64 vNow = EMMediaTimer::Instance() -> MetronomeNowTime();
	EMWinDXMIDIProducer::Instance() -> SendEvent(p_opOutput, /*vNow*/0, 3, vpData, p_vMIDIChannel);
	vpData[0] = static_cast<uint8>(EM_MIDI_EVENT_TYPE_NOTE_OFF);
	vpData[1] = p_vKey;
	vpData[2] = 0;
	EMWinDXMIDIProducer::Instance() -> SendEvent(p_opOutput, vNow + p_vDuration, 3, vpData, p_vMIDIChannel);
}

void EMWinMediaMetronome::ProduceSystemBeep(int32 p_vToneFrequency)
{
//	MessageBeep(0xffffffff);
	if(m_vHardwareProtected)
	{
		Beep(p_vToneFrequency, DURATION / 1000);
	}
	else
	{
		unsigned status = _inp (OUT_8255);
		unsigned ratio = (unsigned) (METRONOME_TIMER_FREQ / p_vToneFrequency);
		unsigned part_ratio;
		_outp (METRONOME_TIMER_MODE, METRONOME_TIMER_OSC);
		part_ratio = ratio & 0xff;
		_outp (METRONOME_TIMER_COUNT, part_ratio);
		part_ratio = (ratio >> 8) & 0xff;
		_outp (METRONOME_TIMER_COUNT, part_ratio);
		_outp (OUT_8255, (status | SPKRON));
		m_opThread -> Sleep(DURATION);
		status = _inp (OUT_8255);
		_outp (OUT_8255, (status & ~SPKRON));
	}
}

#endif