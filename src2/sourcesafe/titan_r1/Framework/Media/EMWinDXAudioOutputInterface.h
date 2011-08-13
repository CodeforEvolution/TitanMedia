#ifndef __EM_WIN_DX_AUDIO_OUTPUT_INTERFACE
#define __EM_WIN_DX_AUDIO_OUTPUT_INTERFACE

#include "EMGlobals.h"
#include "EMWinDXAudioOutput.h"

class EMMediaDataBuffer;
struct IReferenceClock;

DECLARE_INTERFACE_(EMWinDXAudioOutputInterface, IUnknown) 
{ 
	STDMETHOD(ProcessBuffer) (THIS_ EMMediaDataBuffer* p_opBuffer) PURE;
	STDMETHOD(ProjectStarted) (THIS_ int64 p_vSystemTimeWhenStarted) PURE;
	STDMETHOD(ProjectStopped) (THIS_) PURE;
	STDMETHOD(Flush) (THIS_) PURE;
	STDMETHOD(SetName) (THIS_ const char* p_vpName) PURE;
	STDMETHOD(Shutdown) (THIS_) PURE;
	STDMETHOD(SetRefClock) (THIS_ IReferenceClock*) PURE;
	STDMETHOD(RegisterAsClockMaster) (THIS_ int32 p_vRealtimeOutputDescriptorID) PURE;
	STDMETHOD(UnregisterAsClockMaster) (THIS_) PURE;
};

#endif


