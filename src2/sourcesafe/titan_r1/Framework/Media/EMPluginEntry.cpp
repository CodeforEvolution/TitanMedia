#include "EMPluginEntry.h"

#include "EMMediaIDManager.h"

EMPluginEntry::EMPluginEntry(string p_oPluginName, EMMediaType p_vMediaType, EMPluginType p_ePluginType)
	:	m_oName(p_oPluginName),
		m_vID(-1),
		m_vType(p_vMediaType),
		m_ePluginType(p_ePluginType),
		m_vInstanceCount(0)
{
	m_vID = EMMediaIDManager::MakeID();
}

EMPluginEntry::~EMPluginEntry()
{
}

string EMPluginEntry::GetName() const
{
	return m_oName;
}

int32 EMPluginEntry::GetID() const
{
	return m_vID;
}

EMMediaType EMPluginEntry::GetType() const
{
	return m_vType;
}

EMPluginType EMPluginEntry::GetPluginType() const
{
	return m_ePluginType;
}

void EMPluginEntry::IncreaseInstanceCount()
{
	m_vInstanceCount++;
}

uint32 EMPluginEntry::GetCount() const
{
	return m_vInstanceCount;
}

void EMPluginEntry::Reset()
{
	m_vInstanceCount = NULL;
}

string EMPluginEntry::GetUniqueID() 
{ 
	return "Dummy string";
}
