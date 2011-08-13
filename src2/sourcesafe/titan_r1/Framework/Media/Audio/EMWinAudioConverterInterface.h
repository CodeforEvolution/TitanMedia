#ifndef __EM_WIN_AUDIO_CONVERTER_INTERFACE
#define __EM_WIN_AUDIO_CONVERTER_INTERFACE

#include "EMMediaEngineUIDs.h"

DECLARE_INTERFACE_(EMWinAudioConverterInterface, IUnknown) 
{ 

	STDMETHOD(SetSampleParams) (THIS_ int p_vSampleRate, int p_vBitDepth, bool p_vIsMono) PURE;
};

#endif
