#ifndef __EM_VIRTUAL_RENDERER_FILTER
#define __EM_VIRTUAL_RENDERER_FILTER
//#include "windows.h"
//#ifdef __cplusplus
//extern "C" {
//#endif

#include <streams.h>
#include "EMVideoPreview.h"
#include "EMMediaEngineUIDs.h"

DECLARE_INTERFACE_(EMWinVideoVirtualRendererFilter, IUnknown) 
{
    STDMETHOD(InitializeRenderer) (THIS_
				EMVideoPreview* p_opVideoPreview                
             ) PURE;

};

//#ifdef __cplusplus
//}
//#endif

#endif


