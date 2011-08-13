#ifndef __EM_MEDIA_METRONOME
#define __EM_MEDIA_METRONOME

#include "EMThreadListener.h"
#include "EMMIDIGlobals.h"
#include "EMListener.h"

class EMSemaphore;
class EMThread;
class EMListenerRepository;
class EMRealtimeMIDIOutputDescriptor;

#include <windows.h>

class EMMediaMetronome : public EMThreadListener, public EMListener
{
public:
	static EMMediaMetronome* Instance();
	static void Delete();
	virtual ~EMMediaMetronome();
	
	void ThreadRun(EMThread* p_opThread);
	bool MessageReceived(EMListenerRepository* p_opWho, uint32 p_vMessage);

	void Start();
	void Stop();

	virtual void ProduceSystemBeep(int32 p_vToneFrequency) = 0;
	virtual void ProduceMIDIBeep(EMRealtimeMIDIOutputDescriptor* p_opOutput, uint8 p_vKey, uint8 p_vVelocity, uint32 p_vMIDIChannel, int64 p_vDuration) = 0;

protected:
	EMMediaMetronome();
	EMThread* m_opThread;

private:
	static EMMediaMetronome* m_opInstance;
	int64 m_vClickNumber;
	int64 m_vSystemTimeWhenStarted;
	int64 m_vSongTimeWhenStarted;
	float m_vBPM;
	int32 m_vSignatureMajor;
	int32 m_vSignatureMinor;
	HANDLE m_uSleepSemaphore;
	bool m_vIsEnabled;
	EMSemaphore* m_opProtectionSemaphore;
	bool m_vUsePCSpeaker;
	bool m_vUseMIDINote;
	bool m_vDuringPlayback;
	bool m_vDuringRecording;
	string m_oDevice;
	uint32 m_vMIDIChannel;
	uint8 m_vHighKey;
	uint8 m_vHighVelocity;
	int64 m_vHighDuration;
	uint8 m_vLowKey;
	uint8 m_vLowVelocity;
	int64 m_vLowDuration;
	int32 m_vCountIn;
	EMRealtimeMIDIOutputDescriptor* m_opMetronomeMIDIDevice;
	int64 m_vLastTickTime;

};

#endif
