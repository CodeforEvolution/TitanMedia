#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#ifndef __EM_WIN_DIRECT_SOUND_PLAYBACK
#define __EM_WIN_DIRECT_SOUND_PLAYBACK

#include "EMListener.h"
#include "EMThreadListener.h"

#include <dsound.h>

class EMSemaphore;
class EMThread;
class EMMediaDataBuffer;
class EMDSCache;
class EMListenerRepository;
class EMSemaphore;
class EMMediaTimer;
class EMSettingsRepository;
class EMMediaUtility;

class __declspec(dllexport) EMWinDirectSoundPlayback : public EMThreadListener, public EMListener
{
public:
	EMWinDirectSoundPlayback(const GUID* p_upGUID, int32 p_vOutputDescriptorID);
	~EMWinDirectSoundPlayback();
	bool InitCheckE();
	bool Start();
	bool Stop();
	bool ProcessBufferE(EMMediaDataBuffer* p_opBuffer);
	void OnThreadCreated(EMThread* p_opThread);
	void OnThreadKilled(EMThread* p_opThread);
	void ThreadRun(EMThread* p_opThread);
	bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage);
	bool BufferOperation(int32 p_vOperation, void* p_upParam1 = NULL, void* p_upParam2 = NULL, void* p_upParam3 = NULL, void* p_upParam4 = NULL, void* p_upParam5 = NULL);
	void SetName(const char* p_vpOutputName);
	const char* GetName() const;
	bool SetPrimaryBufferFormat(float p_vFrameRate, uint32 p_vChannels, uint32 p_vBitDepth);
	bool SetOutputFormat(float p_vFrameRate, uint32 p_vChannels, uint32 p_vBitDepth);

	int32 Capacity();
	int32 Used();
	void CleanUpE();

private:
	uint32 GetMaxWriteSize(bool p_vUseDSWritePointer);

	GUID m_uDeviceGUID;
	LPDIRECTSOUND m_opDSound8;
	LPDIRECTSOUNDBUFFER m_opPrimaryDSoundBuffer;
	LPDIRECTSOUNDBUFFER m_opDSoundBuffer;
//	LPDIRECTSOUNDNOTIFY m_opDSNotify;
//	DSBPOSITIONNOTIFY* m_upNotifyArray;
//	HANDLE* m_uNotifyEventArray;

	uint64 m_vWriteCursor;
	EMDSCache* m_opDSCache;
	EMThread* m_opThread;
	int32 m_vOutputDescriptorID;
	int32 m_vReadAhead; //milliseconds
	EMMediaTimer* m_opMediaTimer;
	EMSettingsRepository* m_opSettingsRepository;
	EMMediaUtility* m_opMediaUtility;
	uint32 m_vLastReadCursor;

	HANDLE m_uConsumerContinueSignal;
	HANDLE m_uProducerContinueSignal;
	HANDLE m_uAbortSignal;
	EMSemaphore* m_opProtectionSem;
	

	bool m_vIsInitialized;

	bool m_vStarted;
	bool m_vSeeked;
	bool m_vBuffering;
	int64 m_vBytesToPlay;

	bool m_vFirstRun;

	bool m_vIsShuttingDown;

	int64 m_vTotalRead;
	int64 m_vTotalWritten;

	string m_oOutputName;

	int32 m_vLastTime;

	bool m_vThreadWaiting;
};

#endif

#endif