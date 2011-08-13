#include "EMMediaIDManager.h"
#include "EMEncoderDescriptor.h"

EMEncoderDescriptor::EMEncoderDescriptor(EMMediaType p_eType) 
	: EMDescriptor(), 
	m_eType(p_eType)
{
	m_vID = EMMediaIDManager::MakeID();
}

EMEncoderDescriptor::~EMEncoderDescriptor()
{
}

int32 EMEncoderDescriptor::GetID() const
{
	return m_vID;
}

EMMediaType EMEncoderDescriptor::GetType() const
{
	return m_eType;
}
