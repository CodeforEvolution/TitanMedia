#include "EMNetworkFileId.h"

EMNetworkFileID::EMNetworkFileID(int64 p_vUID, int32 p_vID, bool p_vToMediaPool) 
: m_vUID(p_vUID), 
  m_vID(p_vID),
  m_vpName(NULL),
  m_vIsMediaPoolMaterial(p_vToMediaPool)
{
}

EMNetworkFileID::~EMNetworkFileID()
{
	if(m_vpName != NULL)
		delete m_vpName;
}

void EMNetworkFileID::Clone(EMNetworkFileID* p_opId)
{
	p_opId -> m_vID = m_vID;
	p_opId -> m_vUID = m_vUID;
	p_opId -> SetName(m_vpName);
	p_opId -> m_vIsMediaPoolMaterial = m_vIsMediaPoolMaterial;
}

bool EMNetworkFileID::ToMediaPool()
{
	return m_vIsMediaPoolMaterial;
}

uint32 EMNetworkFileID::GetId() 
{ 
	return m_vID; 
}

uint64 EMNetworkFileID::GetUId() 
{ 
	return m_vUID; 
}

const char* EMNetworkFileID::GetName() 
{
	return m_vpName;
}

void EMNetworkFileID::SetName(const char* p_opName)
{
	int len = strlen(p_opName);

	if(m_vpName != NULL)
		delete m_vpName;

	m_vpName = new char[len + 1];

	memcpy(m_vpName, p_opName, len);
	m_vpName[len] = 0;
}