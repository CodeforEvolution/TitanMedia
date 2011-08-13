#ifndef __EM_I_FRAME_READ
#define __EM_I_FRAME_READ
//#include "windows.h"

#include "EMMediaDataBuffer.h"

#ifdef __cplusplus
extern "C" {
#endif



DECLARE_INTERFACE_(EMReadFrameInterface, IUnknown) 
{
    STDMETHOD(SampleCB) (THIS_
                IMediaSample* p_opSample
             ) PURE;

    STDMETHOD (GrabFrame) (THIS_
                EMMediaDataBuffer* p_opBuffer, REFERENCE_TIME& p_vSampleTime
             ) PURE;

    STDMETHOD(SeekToFrame) (THIS_
                REFERENCE_TIME p_vFrameNumber
             ) PURE;
    STDMETHOD(SeekToTime) (THIS_
                REFERENCE_TIME p_vSeekTime
             ) PURE;

	STDMETHOD(SetOutputFormat) (THIS_
				long p_vDestSizeX, long p_vDestSizeY, long p_vSkipSize
			) PURE;

	STDMETHOD(UseScaling) (THIS_
				bool p_vScale
			) PURE;

    STDMETHOD(StopGraph) (THIS_
                
             ) PURE;

    STDMETHOD(InitializeRenderer) (THIS_
				AM_MEDIA_TYPE* p_opMediaType, IMediaSeeking* p_opSeek, IMediaControl*	p_opMediaControl                
             ) PURE;

};

#ifdef __cplusplus
}
#endif

#endif


