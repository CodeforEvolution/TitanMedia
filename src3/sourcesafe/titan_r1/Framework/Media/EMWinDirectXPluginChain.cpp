#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#include "EMMediaGlobals.h"
#include "EMWinDirectXPlugin.h"
#include "EMWinDirectXPluginChain.h"
#include "EMWinPluginEntry.h"

EMWinDirectXPluginChain::EMWinDirectXPluginChain()
	:	EMPluginChain(EM_TYPE_ANY, EM_WINDOWS_DIRECTX_AUDIO),
		m_opDirectXPluginInterface(NULL)
{
}

EMWinDirectXPluginChain::~EMWinDirectXPluginChain()
{
	m_opDirectXPluginInterface -> Release();
}

bool EMWinDirectXPluginChain::InitCheckE()
{
	if(GetPluginType() == EM_WINDOWS_DIRECTX_AUDIO)
	{
		//Type is Windows DirectX Audio plugin, so we need to get a DirectX wrapper etc.
		if(! IsInitialized())
		{
			HRESULT vResultCode = CoCreateInstance(CLSID_EMWinDirectXPluginWrapper, NULL, CLSCTX_INPROC, IID_IEMWinPluginWrapperInterface, (void**) &m_opDirectXPluginInterface);
			if(! FAILED(vResultCode))
			{
				
				//Need to modify filter's interface to handle multiple plugins, and to handle inserting them here and there!

				if(FAILED(m_opDirectXPluginInterface -> InitializeFilter()))
				{
					EMDebugger("ERROR! Failed to initialize the plugin interface!");
					return false;
				}
				SetInitialized();
			}
			else
			{
				EMDebugger("ERROR! Failed to create a COM instance of the EMWinDirectXPluginWrapper module!");
				return false;
			}
		}
		else
			eo << "WARNING! EMWinPluginChain initialized more than once!" << ef;
	}
	else
	{
		EMDebugger("ERROR! Plugin chain cannot handle the particular type...");
		return false;
	}
	return true;
}

bool EMWinDirectXPluginChain::PrepareToAddLast(EMPlugin* p_opPlugin)
{
	eo << "Adding \"" << p_opPlugin ->GetName() << "\" to DirectX plugin chain..." << ef;
	EMWinDirectXPlugin* opDXPlugin = (EMWinDirectXPlugin*) p_opPlugin;
	IBaseFilter* opIBaseFilter = opDXPlugin -> GetIBaseFilter();
	if(FAILED(m_opDirectXPluginInterface -> AddFilter(opIBaseFilter)))
		return false;
	eo << "OK! Effect added!" << ef;
	return true;
}

bool EMWinDirectXPluginChain::PrepareToInsertBefore(EMPlugin* p_opPlugin, EMPlugin* p_opReferencePlugin)
{
	EMWinDirectXPlugin* opDXPlugin = (EMWinDirectXPlugin*) p_opPlugin;
	EMWinDirectXPlugin* opReferenceDXPlugin = (EMWinDirectXPlugin*) p_opReferencePlugin;
	IBaseFilter* opIBaseFilter = opDXPlugin -> GetIBaseFilter();
	IBaseFilter* opReferenceIBaseFilter = opReferenceDXPlugin -> GetIBaseFilter();
	if(FAILED(m_opDirectXPluginInterface -> InsertFilterBefore(opIBaseFilter, opReferenceIBaseFilter)))
		return false;
	return true;
}

#endif