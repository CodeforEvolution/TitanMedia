#ifndef __EM_I_EM_WIN_MEDIA_OUTPUT_FILTER
#define __EM_I_EM_WIN_MEDIA_OUTPUT_FILTER

#include <streams.h>

class EMMediaDataBuffer;
class EMMediaTimer;
class EMSettingsRepository;

struct EMWinMediaOutputFilterInterface;

DECLARE_INTERFACE_(EMWinMediaOutputFilterInterface, IUnknown) 
{
	STDMETHOD(InitializeFilter) (THIS_ EMMediaTimer* p_opTimer, EMSettingsRepository* p_opSettingsRepository) PURE;
    STDMETHOD(AddBufferToCache) (THIS_ EMMediaDataBuffer* p_opBuffer) PURE;
    STDMETHOD(FlushCache) (THIS_ void) PURE;
	STDMETHOD(Shutdown) (THIS_ void) PURE;
	STDMETHOD(RegisterAsClockMaster) (THIS_ int32 p_vRealtimeOutputDescriptorID) PURE;
	STDMETHOD(UnregisterAsClockMaster) (THIS_ void) PURE;
};

#endif


