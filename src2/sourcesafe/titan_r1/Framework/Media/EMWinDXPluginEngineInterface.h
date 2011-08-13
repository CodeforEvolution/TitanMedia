#ifndef __EM_WIN_DX_PLUGIN_ENGINE_INTERFACE
#define __EM_WIN_DX_PLUGIN_ENGINE_INTERFACE

#include <streams.h>
#include "EMGlobals.h"
#include "EMMediaEngineUIDs.h"

class EMMediaDataBuffer;

DECLARE_INTERFACE_(EMWinDXPluginEngineInterface, IUnknown) 
{ 
	STDMETHOD(SetTargetBuffer) (THIS_ EMMediaDataBuffer* p_opBuffer) PURE;
	STDMETHOD(ProcessBuffer) (THIS_ EMMediaDataBuffer* p_opBuffer) PURE;
	STDMETHOD(AssertBufferProcessed) (THIS_) PURE;
	STDMETHOD(SetName) (THIS_ string p_oName) PURE;
};

#endif


