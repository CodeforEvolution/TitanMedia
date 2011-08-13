#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#include "EMWinPluginEntry.h"

#include "EMWinDirectXPlugin.h"
#include "EMPlugin.h"

#include <streams.h>


EMWinPluginEntry::EMWinPluginEntry(string p_oPluginName, CLSID p_vCLSID, EMMediaType p_vType, EMPluginType p_ePluginType)
	:	EMPluginEntry(p_oPluginName, p_vType, p_ePluginType),
		m_vCLSID(p_vCLSID)
{
}

EMWinPluginEntry::~EMWinPluginEntry()
{
}

EMPlugin* EMWinPluginEntry::InstantiateNativePlugin()
{
	IBaseFilter* opBaseFilter = NULL;
	EMPlugin* opTheResultingPlugin = NULL;

	HRESULT vResultCode = CoCreateInstance(m_vCLSID, NULL, CLSCTX_INPROC, IID_IBaseFilter, (void **) &opBaseFilter);
	if(! FAILED(vResultCode))
		opTheResultingPlugin = EM_new EMWinDirectXPlugin(GetName(), opBaseFilter);
	else
		EMDebugger("ERROR! Could not CoCreateInstance of COM object, using the CLSID reported by the system inspector!");

	IncreaseInstanceCount(); //Make sure we know how many instances there's been of this effect!
	opTheResultingPlugin -> SetEntryID(GetID()); //Also, make sure we store the ID number of this entry in the plugin so we can find the entry later on...
	opTheResultingPlugin -> PrepareToPlayE();

	return opTheResultingPlugin;
}

#endif