#include "CommandGetGTrackIndex.h"

#include "EMGTrackRepository.h"

CommandGetGTrackIndex::CommandGetGTrackIndex()
{
}

void* CommandGetGTrackIndex::ExecuteE(void* p_upParameterOne, void* p_upParameterTwo, void* p_upParameterThree)
{
	m_vTrackIndex = EMGTrackRepository::Instance() -> GetTrackIndex(*static_cast<int32*>(p_upParameterOne));
	return &m_vTrackIndex;
}

bool CommandGetGTrackIndex::IsUndoable()
{
	return false;
}

bool CommandGetGTrackIndex::RequiresParameters()
{
	return true;
}

