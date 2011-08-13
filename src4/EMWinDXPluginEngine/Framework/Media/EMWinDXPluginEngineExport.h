#include "EMWinDXPluginEngineFilter.h"

static const WCHAR g_wszName[] = L"EMWinDXPluginEngineFilter";

const AMOVIESETUP_MEDIATYPE sSupportedMediaTypes = 
{
    &MEDIATYPE_Audio, 
	&MEDIASUBTYPE_PCM
};

const AMOVIESETUP_PIN sPinInformation = 
{
	L"",								// Obsolete, not used.
    FALSE,								// Is this pin rendered?
    TRUE,								// Is it an output pin?
    FALSE,								// Can the filter create zero instances?
    FALSE,								// Does the filter create multiple instances?
    &CLSID_NULL,						// Obsolete.
    NULL,								// Obsolete.
    1,									// Number of media types.
    &sSupportedMediaTypes				// Pointer to media types.
};

const AMOVIESETUP_FILTER sFilterInformation = 
{
    &CLSID_EMWinDXPluginEngineFilter,	// Filter CLSID.
    g_wszName,							// Filter name.
    MERIT_NORMAL,						// Merit.
    1,									// Number of pin types.
    &sPinInformation					// Pointer to pin information.
};