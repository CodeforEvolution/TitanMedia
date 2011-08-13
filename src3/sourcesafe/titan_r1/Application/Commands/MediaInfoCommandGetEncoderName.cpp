#include "MediaInfoCommandGetEncoderName.h"

#include "EMMediaEngine.h"
#include "EMEncoderRepository.h"
#include "EMRealtimeEncoderDescriptor.h"
#include "EMWinQuickTimeWriterSuper.h"
#include "EMWinMediaSingletonMuxer.h"
#include <list>

MediaInfoCommandGetEncoderName::MediaInfoCommandGetEncoderName()
{
}

void* MediaInfoCommandGetEncoderName::ExecuteE(void* p_upIDs, void*, void*)
{
	uint32 vpIDs = *static_cast<uint32*>(p_upIDs);

	if(EMWinMediaSingletonMuxer::Instance() ->m_vCurrentlyUsedFamily == EM_FAMILY_QUICKTIME)
	{
		m_vpName = EMWinQuickTimeWriterSuper::Instance() -> GetEncoderName(vpIDs);
		return static_cast<void*>(&m_vpName);
	}

	EMEncoderRepository* opOutputs = EMEncoderRepository::Instance();

	try
	{
		opOutputs -> Rewind();
		while(opOutputs -> Current() != NULL)
		{
			int32 vOutputID = opOutputs -> Current() -> GetID();
			if(vOutputID == vpIDs) //DIVX FAST MOTION , just temporarily set to 48
			{
				EMRealtimeEncoderDescriptor* opRealtimeEncoderDescriptor = opOutputs -> Find(vOutputID);
				m_vpName = opRealtimeEncoderDescriptor -> GetName();
				return static_cast<void*>(&m_vpName);
			}
			opOutputs -> Next();
		}
	}
	catch(...)
	{
		EMDebugger("ERROR! Exception in MediaInfoCommandGetAudioRenderOutput::ExecuteE");
	}

	return NULL;
}

bool MediaInfoCommandGetEncoderName::RequiresParameters()
{
	return true;
}

bool MediaInfoCommandGetEncoderName::IsUndoable()
{
	return false;
}