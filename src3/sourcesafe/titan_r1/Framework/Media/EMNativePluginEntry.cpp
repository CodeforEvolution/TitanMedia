#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#include "EMNativePluginEntry.h"

#include <streams.h>
#include <qedit.h>

#include "EMNativePlugin.h"

EMNativePluginEntry::EMNativePluginEntry(string p_oPluginName, EMNativePlugin* p_opPlugin)
	:	EMPluginEntry(p_oPluginName, (EMMediaType)p_opPlugin -> GetType(), p_opPlugin -> GetPluginType()),
	m_opPlugin(p_opPlugin)
{
}

EMNativePluginEntry::~EMNativePluginEntry()
{
	if(m_opPlugin != NULL)
		delete m_opPlugin;
}

EMPlugin* EMNativePluginEntry::InstantiateNativePlugin()
{
	EMPlugin* opRet = m_opPlugin -> Clone();
	opRet -> SetEntryID(GetID());
	return opRet;
}


string EMNativePluginEntry::GetUniqueID()
{
	return m_opPlugin -> GetName();	
}

void* EMNativePluginEntry::GetDefaultProperties()
{
	return m_opPlugin -> GetDefaultProperties();
}

#endif