/*******************************************************
* Creator: Richard Kronfalt
* Portability: Non-Native
*-------------------------------------------------------
*
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_MEDIA_TIMER
#define __EM_MEDIA_TIMER

#include "EMMediaGlobals.h"
#include "EMListenerRepository.h"
#include "EMListener.h"

class EMSemaphore;
class EMProjectDataSaver;
class EMProjectDataLoader;
class EMRealtimeOutputDescriptor;

class __declspec(dllexport) EMMediaTimer : public EMListenerRepository, public EMListener
{
public:
	static EMMediaTimer* Instance();
	static void Delete();

	int64 AudioThenFrame();
	int64 AudioThenTime();
	int32 GetID() const;
	int64 GetLoopEnd() const;
	int64 GetLoopStart() const;
	EMMediaTimerState GetState() const;
	int64 GetSystemTimeWhenStarted() const;
	bool IncreaseAudioThenFrame(int64 p_vWithFrames); //Will return false if time shouldnt be increased anymore!
	bool IncreaseMIDIThenTime(int64 p_vWithMicroseconds); //Will return false if time shouldnt be increased anymore!
	bool IncreaseMetronomeNowTime(int64 p_vWithMicroseconds); //Will return false if time shouldn't be increased anymore!
	bool IncreaseNowFrame(int64 p_vWithFrames, int32 p_vPotentialClockMasterID); //Will return false if time shouldnt be increased anymore!
	bool IncreaseVideoThenFrame(int64 p_vWithFrames); //Will return false if time shouldnt be increased anymore!
	bool IsLooped() const;
	void Lock();
	int64 MIDIThenTime();
	bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage);
	int64 MetronomeNowTime();
	int64 NowFrame();
	int64 NowTime();
	void Unlock();
	bool RegisterClockMaster(EMRealtimeOutputDescriptor* p_opOutputThatControlsNowTime);
	void SeekToFrame(int64 p_vNewFrame);
	bool SetState(EMMediaTimerState p_eNewState);
	int64 VideoThenFrame();
	int64 VideoThenTime();

	virtual bool ClearData();
	virtual bool LoadData(EMProjectDataLoader* p_opLoader);
	virtual void OnStartPlayback() = 0;
	virtual void OnStopPlayback() = 0;
	virtual void OnStartRecording() = 0;
	virtual void OnStopRecording() = 0;
	virtual void OnCountInPlayback() = 0;
	virtual void OnCountInRecording() = 0;
	virtual int64 RealTime() const = 0;
	virtual bool SaveData(EMProjectDataSaver* p_opSaver);

protected:
	EMMediaTimer();
	virtual ~EMMediaTimer();
	int32 m_vID;
	int64 m_vNow;
	int64 m_vAudioThen;
	int64 m_vVideoThen;
	int64 m_vMIDIThen;
	int64 m_vMetronomeThen;
	EMMediaTimerState m_eState;
	int64 m_vLoopStart;
	int64 m_vLoopEnd;
	bool m_vIsLooped;
	uint64 m_vSystemTimeWhenStarted;
	uint64 m_vSystemTimeWhenCountInStarted;
	int32 m_vClockMasterID;

private:
	static EMMediaTimer* m_opInstance;
	EMSemaphore* m_opTimeProtectionSemaphore;
};

#endif

