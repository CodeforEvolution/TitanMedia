#ifndef __EM_WIN_VIDEO_SHUFFLE_OUTPUT_INTERFACE
#define __EM_WIN_VIDEO_SHUFFLE_OUTPUT_INTERFACE

#include <streams.h>
#include "EMGlobals.h"
#include "EMMediaEngineUIDs.h"

class EMMediaDataBuffer;
class EMMediaFormat;

DECLARE_INTERFACE_(EMWinVideoShuffleOutputInterface, IUnknown) 
{ 
	STDMETHOD(ProcessBuffer) (THIS_ EMMediaDataBuffer* p_opBuffer) PURE;
	STDMETHOD(Flush) (THIS_) PURE;
	STDMETHOD(SetName) (THIS_ string p_oName) PURE;
	STDMETHOD(SetShuffleFormat) (THIS_ EMMediaFormat* p_opMediaFormat) PURE;
	STDMETHOD(SetTimeStamp) (THIS_ REFERENCE_TIME p_vRefTime) PURE;
	STDMETHOD(StartThread) (THIS_) PURE;
	STDMETHOD(EndThread) (THIS_) PURE;
	STDMETHOD(RegisterAsClockMaster) (THIS_ int32 p_vRealtimeOutputDescriptorID) PURE;
	STDMETHOD(UnregisterAsClockMaster) (THIS_ void) PURE;
};

#endif
