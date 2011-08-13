#include "MediaCommandSetTrackInput.h"

#include "EMMediaEngine.h"
#include "EMMediaProject.h"
#include "EMInputRepository.h"
#include "EMMediaTrack.h"
#include "EMRealtimeInputDescriptor.h"
#include "EMMediaTrackRepository.h"

#include "EMMediaMIDITrack.h"
#include "EMRealtimeMIDIInputDescriptor.h"
#include "EMRealtimeMIDIOutputDescriptor.h"
#include "EMWinDXMIDIConsumer.h"

MediaCommandSetTrackInput::MediaCommandSetTrackInput(int32 p_vRecentTrackID, int32 p_vRecentInputID)
	:	m_vRecentTrackID(p_vRecentTrackID),
		m_vRecentInputID(p_vRecentInputID)
{ 
}

void* MediaCommandSetTrackInput::ExecuteE(void* p_upTrackID, void* p_upInputID, void*)
{
	//eo << "MediaCommandSetTrackInput" << ef;
	EMMediaEngine* opEngine = EMMediaEngine::Instance();
	EMMediaProject* opProject = opEngine -> GetMediaProject();
	EMInputRepository* opInputs = EMInputRepository::Instance();

	int32 vTrackID = *(static_cast<int32*>(p_upTrackID));
	int32 vInputID = *(static_cast<int32*>(p_upInputID));

	if(vInputID == -1)
		return NULL;

	EMMediaTrack* opTrack = NULL;
	opProject -> GetUsedTracks() -> LockContainer();
	try
	{
		opTrack = opProject -> GetUsedTracks() -> Find(vTrackID);
	}
	catch(...)
	{
		EMDebugger("ERROR! Exception in MediaCommandSetTrackInput::ExecuteE");
	}
	opProject -> GetUsedTracks() -> UnlockContainer();
	if(opTrack == NULL)
	{
		opProject -> GetUnusedTracks() -> LockContainer();
		try
		{
			opTrack = opProject -> GetUnusedTracks() -> Find(vTrackID);
		}
		catch(...)
		{
			EMDebugger("ERROR! Excecption in MediaCommandSetTrackInput::ExecuteE");
		}
		opProject -> GetUnusedTracks() -> UnlockContainer();
	}
	
	EMRealtimeInputDescriptor* opInput = NULL;
	opInputs -> LockContainer();
	try
	{
		opInput = opInputs -> Find(vInputID);
	}
	catch(...)
	{
		EMDebugger("ERROR! Exception in MediaCommandSetTrackInput::ExecuteE");
	}
	opInputs -> UnlockContainer();
	
	if(opTrack != NULL && opInput != NULL)
	{
		m_vRecentTrackID = vTrackID;
		m_vRecentInputID = vInputID;

		EMRealtimeInputDescriptor* opOldInput = opTrack -> GetInput();;

		opTrack -> SetInput(opInput);
		EMMediaEngine::Instance() -> GetMediaProject() -> SetDirty(true);
	
		if((opTrack -> GetType() & EM_TYPE_MIDI) > 0)
		{
//			if(static_cast<EMMediaMIDITrack*>(opTrack) -> IsActive())
//			{
				uint32 vMIDIChannel = static_cast<EMMediaMIDITrack*>(opTrack) -> GetMIDIChannel();
				if(opOldInput != NULL)
				{
					EMWinDXMIDIConsumer::Instance() -> SetTrackAsListener(static_cast<EMMediaMIDITrack*>(opTrack), NULL, NULL);
//					EMWinDXMIDIProducer::Instance() -> UnmapInput(static_cast<EMRealtimeMIDIInputDescriptor*>(opInput));
//					EMWinDXMIDIProducer::Delete();
				}
				
				EMWinDXMIDIConsumer::Instance() -> SetTrackAsListener(static_cast<EMMediaMIDITrack*>(opTrack), static_cast<EMRealtimeMIDIInputDescriptor*>(opInput), static_cast<EMRealtimeMIDIOutputDescriptor*>(opTrack -> GetTrackDestination()));
//				EMWinDXMIDIProducer::Instance() -> MapInputToOutput(static_cast<EMRealtimeMIDIInputDescriptor*>(opInput), static_cast<EMRealtimeMIDIOutputDescriptor*>(opTrack -> GetTrackDestination()), vMIDIChannel);
//				EMWinDXMIDIProducer::Delete();
//			}
		}
	}
	return NULL;
}

bool MediaCommandSetTrackInput::RequiresParameters()
{
	return true;
}

void MediaCommandSetTrackInput::UndoE()
{
	if(m_vRecentTrackID != -1 && m_vRecentInputID != -1)
	{
		EMMediaEngine* opEngine = EMMediaEngine::Instance();
		EMMediaProject* opProject = opEngine -> GetMediaProject();
		EMInputRepository* opInputs = EMInputRepository::Instance();
		
		EMMediaTrack* opTrack = NULL;
		opProject -> GetUsedTracks() -> LockContainer();
		try
		{
			opTrack = opProject -> GetUsedTracks() -> Find(m_vRecentTrackID);
		}
		catch(...)
		{
			EMDebugger("ERROR! Exception in MediaCommandSetTrackInput::UndoE");
		}
		opProject -> GetUsedTracks() -> UnlockContainer();
		if(opTrack == NULL)
		{
			opProject -> GetUnusedTracks() -> LockContainer();
			try
			{
				opProject -> GetUnusedTracks() -> Find(m_vRecentTrackID);
			}
			catch(...)
			{
				EMDebugger("ERROR! Exception in MediaCommandSetTrackInput::UndoE");
			}
			opProject -> GetUnusedTracks() -> UnlockContainer();
		}
		
		EMRealtimeInputDescriptor* opInput = NULL;
		opInputs -> LockContainer();
		try
		{
			opInput = opInputs -> Find(m_vRecentInputID);
		}
		catch(...)
		{
			EMDebugger("ERROR! Exception in MediaCommandSetTrackInput::UndoE");
		}
		opInputs -> UnlockContainer();
		if(opTrack != NULL && opInput != NULL)
		{
			//opTrack -> SetDestination(opOutput);
			opTrack -> SetInput(opInput);
			//Do somehting else!
		}
	}
	else
		EMDebugger("Bad stored track or bad stored input ID in SetTrackInput Command");
}

bool MediaCommandSetTrackInput::IsUndoable()
{
	return false;
}

EMCommand* MediaCommandSetTrackInput::CloneAndClear()
{
	return EM_new MediaCommandSetTrackInput(m_vRecentTrackID, m_vRecentInputID);
}

