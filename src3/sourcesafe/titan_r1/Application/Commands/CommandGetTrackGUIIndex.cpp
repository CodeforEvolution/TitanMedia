#include "CommandGetTrackGUIIndex.h"

#include "EMGTrackRepository.h"

CommandGetTrackGUIIndex::CommandGetTrackGUIIndex()
{
}

CommandGetTrackGUIIndex::~CommandGetTrackGUIIndex()
{
}

void* CommandGetTrackGUIIndex::ExecuteE(void* p_upParameterOne, void* p_upParameterTwo, void* p_upParameterThree)
{
	m_vReturnValue = EMGTrackRepository::Instance() -> GetTrackIndex(*(static_cast<int32*>(p_upParameterOne)));
	return &m_vReturnValue;
}

bool CommandGetTrackGUIIndex::IsUndoable()
{
	return false;
}

bool CommandGetTrackGUIIndex::RequiresParameters()
{
	return true;
}

