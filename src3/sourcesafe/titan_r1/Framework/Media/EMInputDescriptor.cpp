#include "EMMediaIDManager.h"
#include "EMInputDescriptor.h"

EMInputDescriptor::EMInputDescriptor(EMMediaType p_eType) 
	: EMDescriptor(), 
	m_eType(p_eType)
{
	m_vID = EMMediaIDManager::MakeID();
}

EMInputDescriptor::~EMInputDescriptor()
{
}

int32 EMInputDescriptor::GetID() const
{
	return m_vID;
}

EMMediaType EMInputDescriptor::GetType() const
{
	return m_eType;
}
