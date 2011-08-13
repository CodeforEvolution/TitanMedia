/*******************************************************
* Creator: Martin Johansson
* Portability: Native
*-------------------------------------------------------
*
* Represents the existance of a native transition, in the
* the system. It holds native information such as the
* ability to actually instantiate the plugin into a 
* system dependent object. In windows (this file) this
* means having a CLSID that describes the filter's registry
* information, and code for fetching it as a COM object,
* etc, etc.
*
*******************************************************/

#ifndef __EM_NATIVE_PLUGIN_ENTRY
#define __EM_NATIVE_PLUGIN_ENTRY

#include "EMGlobals.h"

#include "EMMediaGlobals.h"
#include "EMPluginEntry.h"

#include <streams.h>

class EMNativePlugin;
class EMPlugin;

class EMNativePluginEntry : public EMPluginEntry
{
public:
	EMNativePluginEntry(string p_oPluginName, EMNativePlugin* p_opPlugin);
	~EMNativePluginEntry();
	EMPlugin* InstantiateNativePlugin();

	virtual void* GetDefaultProperties();

	virtual string GetUniqueID();
	
private:
	EMNativePlugin* m_opPlugin;
};

#endif
