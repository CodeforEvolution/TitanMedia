
#include "EMRubberBandNode.h"

EMRubberBandNode::EMRubberBandNode() :
	m_vPosition(0),
	m_vValue(0)
{
}

EMRubberBandNode::~EMRubberBandNode()
{
}

//Position is in micro seconds
EMRubberBandNode::SetPosition(int64 p_vPosition)
{
	m_vPosition = p_vPosition;
}

EMRubberBandNode::SetValue(int32 p_vValue)
{
	m_vValue = p_vValue;
}

int64 EMRubberBandNode::GetPosition()
{
	return m_vPosition;
}

int32 EMRubberBandNode::GetValue()
{
	return m_vValue;
}
