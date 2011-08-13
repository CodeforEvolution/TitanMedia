#include "EMTreeViewControl.h"

EMTreeViewControl::EMTreeViewControl() :
__m_vNoUpdate(false)
{
}

EMTreeViewControl::~EMTreeViewControl()
{
}

void EMTreeViewControl::StopUpdate()
{
	__m_vNoUpdate = true;
}