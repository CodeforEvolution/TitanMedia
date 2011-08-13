#include "EMGlobals.h"

#include "EMOutputDescriptor.h"

#include <list>

EMOutputDescriptor::EMOutputDescriptor(EMMediaType p_eType) 
	: 	EMDescriptor(),
		EMBufferDestination(p_eType, EM_OUTPUT, string("EMOutputDescriptor")),
		m_vIsInitialized(false), 
		m_eType(p_eType),
		m_vIsClockMaster(false),
		m_vIsRenderOutput(false)
{
}

EMOutputDescriptor::~EMOutputDescriptor()
{
}

EMMediaType EMOutputDescriptor::GetType() const
{
	return m_eType;
}

bool EMOutputDescriptor::IsInitialized() const
{
	return m_vIsInitialized;
}

void EMOutputDescriptor::SetClockMaster(bool p_vFlag)
{
	m_vIsClockMaster = p_vFlag;
	if(m_vIsClockMaster)
		RegisterAsClockMaster();
	else
		UnregisterAsClockMaster();
}

bool EMOutputDescriptor::IsClockMaster() const
{
	return m_vIsClockMaster;
}

void EMOutputDescriptor::SetRenderOutput(bool p_vFlag)
{
	m_vIsRenderOutput = p_vFlag;
}

bool EMOutputDescriptor::IsRenderOutput() const
{
	return m_vIsRenderOutput;
}
