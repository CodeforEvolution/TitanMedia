#ifndef __EM_I_EM_WIN_MEDIA_INPUT_FILTER
#define __EM_I_EM_WIN_MEDIA_INPUT_FILTER

#include <string>

class EMMediaSignalMeter;

DECLARE_INTERFACE_(EMWinMediaInputFilterInterface, IUnknown) 
{
	STDMETHOD(SetTargetFile) (THIS_ string p_oFileName) PURE;
	STDMETHOD(CloseTargetFile) (THIS_ void) PURE;

	STDMETHOD(Suspend) (THIS_ bool p_vSuspended) PURE;
	STDMETHOD(SetSignalMeter) (THIS_ EMMediaSignalMeter* p_opMeter) PURE;
	STDMETHOD(StartOnTime) (THIS_ int64 p_vStartTime) PURE;
};

#endif


