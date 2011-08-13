#ifndef __EM_WIN_MEDIA_OUTPUT_FILTER_EXPORT
#define __EM_WIN_MEDIA_OUTPUT_FILTER_EXPORT

//#include <streams.h>
//#include "EMMediaEngineUIDs.h"

#include "EMWinMediaOutputFilter.h"

#ifdef PLATFORM_WINDOWS

static const WCHAR g_wszName[] = L"EMWinMediaOutputFilter";
static const WCHAR g_wszVideoName[] = L"EMWinMediaVideoOutputFilter";

#endif

const AMOVIESETUP_MEDIATYPE sudEMWinMediaOutputFilterPinTypes = 
{
    &MEDIATYPE_Audio, 
	&MEDIASUBTYPE_PCM
};

const AMOVIESETUP_MEDIATYPE sudEMWinMediaVideoOutputFilterPinTypes = 
{
    &MEDIATYPE_Video, 
	&MEDIASUBTYPE_RGB24
}; 

const AMOVIESETUP_PIN sudEMWinMediaOutputFilterPinInfo = 
{
    L"Output",							// Obsolete, not used.
    FALSE,								// Is this pin rendered?
    TRUE,								// Is it an output pin?
    FALSE,								// Can the filter create zero instances?
    FALSE,								// Does the filter create multiple instances?
    &CLSID_NULL,						// Obsolete.
    L"Input",							// Obsolete.
    1,									// Number of media types.
    &sudEMWinMediaOutputFilterPinTypes	// Pointer to media types.
};

const AMOVIESETUP_FILTER sudEMWinMediaOutputFilterRegs = 
{
    &CLSID_EMWinMediaOutputFilter,		// Filter CLSID.
    g_wszName,							// Filter name.
    MERIT_NORMAL,						// Merit.
    1,									// Number of pin types.
    &sudEMWinMediaOutputFilterPinInfo	// Pointer to pin information.
};

const AMOVIESETUP_PIN sudEMWinMediaVideoOutputFilterPinInfo = 
{
    L"Output",							// Obsolete, not used.
    FALSE,								// Is this pin rendered?
    TRUE,								// Is it an output pin?
    FALSE,								// Can the filter create zero instances?
    FALSE,								// Does the filter create multiple instances?
    &CLSID_NULL,						// Obsolete.
    L"Input",							// Obsolete.
    1,									// Number of media types.
    &sudEMWinMediaVideoOutputFilterPinTypes	// Pointer to media types.
};

const AMOVIESETUP_FILTER sudEMWinMediaVideoOutputFilterRegs = 
{
    &CLSID_EMWinMediaVideoOutputFilter,		// Filter CLSID.
    g_wszVideoName,							// Filter name.
    MERIT_NORMAL,						// Merit.
    1,									// Number of pin types.
    &sudEMWinMediaVideoOutputFilterPinInfo	// Pointer to pin information.
};



STDAPI DllRegisterServer()
{
	return AMovieDllRegisterServer2(TRUE);
}

STDAPI DllUnregisterServer()
{
	return AMovieDllRegisterServer2(FALSE);
}

#endif