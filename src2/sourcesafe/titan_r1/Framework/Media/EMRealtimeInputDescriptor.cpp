#include "EMRealtimeInputDescriptor.h"
#include "EMRecording.h"

EMRealtimeInputDescriptor::EMRealtimeInputDescriptor(EMMediaType p_eType) 
	:	EMInputDescriptor(p_eType),
		m_vIsInitialized(false),
		m_opRecording(0)
{
}

EMRealtimeInputDescriptor::~EMRealtimeInputDescriptor()
{
}

void EMRealtimeInputDescriptor::AddRecorder(EMMediaTrack* p_opTrack)
{
	oRegisteredRecorders.push_back(p_opTrack);
}

void EMRealtimeInputDescriptor::RemoveRecorder(EMMediaTrack* p_opTrack)
{
	oRegisteredRecorders.remove(p_opTrack);
}

EMRecording* EMRealtimeInputDescriptor::GetLastRecording() const
{
	return m_opRecording;
}

void EMRealtimeInputDescriptor::SetArmed(bool p_vShouldArm)
{
}
