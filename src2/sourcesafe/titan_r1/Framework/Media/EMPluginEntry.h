/*******************************************************
* Creator: Richard Kronfalt
* Portability: Non-Native
*-------------------------------------------------------
*
* Represents the existance of a certain plugin in the
* system. It's abstract so it doesnt hold any information
* about how to use/instantiate the respective plugins.
* However, it holds information such as the plugin's
* name, and so on.
*
*******************************************************/

#ifndef __EM_PLUGIN_ENTRY
#define __EM_PLUGIN_ENTRY

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

class EMPlugin;

class EMPluginEntry
{
public:
	virtual ~EMPluginEntry();
	string GetName() const;
	int32 GetID() const;
	virtual EMPlugin* InstantiateNativePlugin() = 0;
	EMMediaType GetType() const;
	EMPluginType GetPluginType() const;
	uint32 GetCount() const;
	void Reset();

	virtual void SetPersistProperties(void* p_upProperties) {};
	virtual void* GetPersistProperties() {return NULL;};
	virtual void* GetDefaultProperties() {return NULL;};

	virtual string GetUniqueID();

protected:
	EMPluginEntry(string p_oPluginName, EMMediaType p_vMediaType, EMPluginType p_ePluginType);
	void IncreaseInstanceCount(); //Only avaliable for this baseclass' children

private:
	string m_oName;
	int32 m_vID;
	EMMediaType m_vType;
	EMPluginType m_ePluginType;
	uint32 m_vInstanceCount;
};

#endif