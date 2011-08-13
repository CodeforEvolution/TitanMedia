#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#ifndef __EM_WIN_DIRECT_SOUND_PRODUCER

#include "EMThreadListener.h"

#include <windows.h>

class EMThread;
class EMWinDirectSoundPlayback;
class EMDSCache;
class EMMediaDataBuffer;

class __declspec(dllexport) EMWinDirectSoundProducer : public EMThreadListener
{
public:
	EMWinDirectSoundProducer(GUID* p_upGuid);
	~EMWinDirectSoundProducer();
	bool InitCheckE();
	bool Flush();
	bool Start();
	bool Stop();
	bool ProcessBufferE(EMMediaDataBuffer* p_opBuffer);
	void OnThreadCreated(EMThread* p_opThread);
	void OnThreadKilled(EMThread* p_opThread);
	void ThreadRun(EMThread* p_opThread);

private:
	EMThread* m_opThread;
	GUID* m_upDSDeviceGUID;

	EMDSCache* m_opDSCache;
	EMWinDirectSoundPlayback* m_opDSOutput;
	bool m_vIsInitialized;
};

#endif

#endif