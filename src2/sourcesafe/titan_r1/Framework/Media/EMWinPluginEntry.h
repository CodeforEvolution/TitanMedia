/*******************************************************
* Creator: Richard Kronfalt
* Portability: Non-Native
*-------------------------------------------------------
*
* Represents the existance of a native plugin, in the
* the system. It holds native information such as the
* ability to actually instantiate the plugin into a 
* system dependent object. In windows (this file) this
* means having a CLSID that describes the filter's registry
* information, and code for fetching it as a COM object,
* etc, etc.
*
*******************************************************/

#ifndef __EM_WIN_PLUGIN_ENTRY
#define __EM_WIN_PLUGIN_ENTRY

#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#include "EMMediaGlobals.h"
#include "EMPluginEntry.h" 
#include "EMMediaEngineUIDs.h"
#include <streams.h>

class EMPlugin;

/*static const GUID CLSID_EMWinDirectXPluginWrapper =	{0x760956e1, 0x50f9, 0x11d5, {0xbc, 0x74, 0x0, 0x2, 0xb3, 0x17, 0xf3, 0xf2}};
static const GUID IID_IEMWinPluginWrapperInterface = {0x773a681, 0x50ff, 0x11d5, {0xbc, 0x74, 0x0, 0x2, 0xb3, 0x17, 0xf3, 0xf2}}; */

class EMWinPluginEntry : public EMPluginEntry
{
public:
	EMWinPluginEntry(string p_oPluginName, CLSID p_vCLSID, EMMediaType p_vType, EMPluginType p_ePluginType);
	~EMWinPluginEntry();
	EMPlugin* InstantiateNativePlugin();

private:
	CLSID m_vCLSID;
};

#endif

#endif