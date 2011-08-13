#ifndef __EM_I_TEST_FILTER
#define __EM_I_TEST_FILTER
//#include "windows.h"
#ifdef __cplusplus
extern "C" {
#endif

DEFINE_GUID(IID_ITestFilter, 
0xfe75b1e1, 0x3821, 0x11d5, 0x82, 0x59, 0x0, 0x2, 0xb3, 0x18, 0x0, 0x46);


DECLARE_INTERFACE_(ITestFilter, IUnknown) 
{
    STDMETHOD(SampleCB) (THIS_
                IMediaSample* p_opSample
             ) PURE;

    STDMETHOD (GrabFrame) (THIS_
                void* p_opBuffer, REFERENCE_TIME& p_vSampleTime//, HBITMAP& p_opBitmap
             ) PURE;

    STDMETHOD(SeekToFrame) (THIS_
                REFERENCE_TIME p_vFrameNumber
             ) PURE;
    STDMETHOD(SeekToTime) (THIS_
                REFERENCE_TIME p_vSeekTime
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


