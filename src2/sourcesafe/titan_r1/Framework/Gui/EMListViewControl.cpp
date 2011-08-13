#include "EMListViewControl.h"

EMListViewControl::EMListViewControl() :
__m_vNoUpdate(false)
{
}

EMListViewControl::~EMListViewControl()
{
}

void EMListViewControl::StopUpdate()
{
	__m_vNoUpdate = true;
}