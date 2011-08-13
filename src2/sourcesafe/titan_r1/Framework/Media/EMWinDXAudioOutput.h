#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#ifndef __EM_WIN_DX_AUDIO_OUTPUT
#define __EM_WIN_DX_AUDIO_OUTPUT

#include <streams.h>

class EMMediaDataBuffer;
struct EMWinDXAudioOutputInterface;

class __declspec(dllexport) EMWinDXAudioOutput
{
public:
	EMWinDXAudioOutput(const char* p_vpName);
	~EMWinDXAudioOutput();
	
	//Initialize this filter with the actual output that the data should be sent to!
	bool InitCheckE(IBaseFilter* p_opFilter); 

	//Process a buffer (i e play it!)
	bool ProcessBuffer(EMMediaDataBuffer* p_opBuffer);
	void ProjectStarted(int64 p_vSystemTimeWhenStarted);
	void ProjectStopped();
	void Flush();
	bool Setup();
	bool Shutdown();

	bool RegisterAsClockMaster(int32 p_vWithRealtimeOutputID);
	bool UnregisterAsClockMaster();

private:
	EMWinDXAudioOutputInterface* m_opSourceFilterInterface;

	IFilterGraph2* m_opFilterGraph;
	IBaseFilter* m_opSourceFilter;
	IBaseFilter* m_opPhysicalOutputFilter;
	IPin* m_opSourceFilterOutputPin;
	IPin* m_opPhysicalOutputInputPin;
	IMediaControl* m_opGraphControl;
	bool m_vIsHookedUp;
	string m_oName;
	IReferenceClock* m_opRefClock;
	bool m_vIsClockMaster;
};

#endif

#endif