#ifndef __EM_WIN_MEDIA_INPUT_FILTER_EXPORT
#define __EM_WIN_MEDIA_INPUT_FILTER_EXPORT

#include "EMWinMediaInputFilter.h"

static const WCHAR g_wszName[] = L"EMWinMediaInputFilter";

const AMOVIESETUP_MEDIATYPE sudEMWinMediaInputFilterPinTypes =
{
    &MEDIATYPE_Audio, 
	&MEDIASUBTYPE_PCM
};

const AMOVIESETUP_PIN sudEMWinMediaInputFilterPinInfo =
{
    L"Input",							// Pin string name
    FALSE,								// Is it rendered
    FALSE,								// Is it an output
    FALSE,								// Allowed none
    FALSE,								// Likewise many
    &CLSID_NULL,						// Connects to filter
    L"Output",							// Connects to pin
    1,									// Number of types
    &sudEMWinMediaInputFilterPinTypes	// Pin information
};

const AMOVIESETUP_FILTER sudEMWinMediaInputFilterRegs =
{
    &CLSID_EMWinMediaInputFilter,		// Filter CLSID
    g_wszName,							// String name
    MERIT_NORMAL,						// Filter merit
    1,									// Number pins
    &sudEMWinMediaInputFilterPinInfo	// Pin details
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