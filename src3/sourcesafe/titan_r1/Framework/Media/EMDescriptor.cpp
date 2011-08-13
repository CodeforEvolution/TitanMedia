#include "EMDescriptor.h"

#include "EMMediaIDManager.h"

EMDescriptor::EMDescriptor()
	:	m_vIsRenderer(false),
		m_oName("Untitled")
{
}

EMDescriptor::~EMDescriptor()
{
}

string EMDescriptor::GetName() const
{
	return m_oName;
}

void EMDescriptor::SetName(const string p_oName)
{
	m_oName = p_oName;
}
