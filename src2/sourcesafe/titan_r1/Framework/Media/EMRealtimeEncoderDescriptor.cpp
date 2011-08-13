#include "EMRealtimeEncoderDescriptor.h"

EMRealtimeEncoderDescriptor::EMRealtimeEncoderDescriptor(EMMediaType p_eType) 
	:	EMEncoderDescriptor(p_eType),
		m_vIsInitialized(false)
{
}

EMRealtimeEncoderDescriptor::~EMRealtimeEncoderDescriptor()
{
}

