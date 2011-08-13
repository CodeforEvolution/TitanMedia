#include "MediaCommandSetActiveMIDITrack.h"

#include "EMMediaEngine.h"
#include "EMMediaProject.h"
#include "EMInputRepository.h"
#include "EMMediaTrack.h"
#include "EMMediaMIDITrack.h"
#include "EMRealtimeInputDescriptor.h"
#include "EMMediaTrackRepository.h"
#include "EMRealtimeMIDIInputDescriptor.h"
#include "EMRealtimeMIDIOutputDescriptor.h"

#include "EMWinRealtimeMIDIOutputDescriptor.h" //TODO: Make non-native!
#include "EMWinDXMIDIProducer.h" //TODO: Make non-native!
#include "EMWinDXMIDIConsumer.h" //TODO: Make non-native!
MediaCommandSetActiveMIDITrack::MediaCommandSetActiveMIDITrack(int32 p_vRecentTrackID)
	:	m_vRecentTrackID(p_vRecentTrackID)
{
}

void* MediaCommandSetActiveMIDITrack::ExecuteE(void* p_upTrackID, void*, void*)
{
	//eo << "MediaCommandSetActiveMIDITrack" << ef; 
	EMMediaEngine* opEngine = EMMediaEngine::Instance();
	EMMediaProject* opProject = opEngine -> GetMediaProject();
	EMInputRepository* opInputs = EMInputRepository::Instance();

	int32 vTrackID = *(static_cast<int32*>(p_upTrackID)); 

	EMMediaTrack* opTrack = NULL;
	opProject -> GetUsedTracks() -> LockContainer();
	try
	{
		opTrack = opProject -> GetUsedTracks() -> Find(vTrackID);
	}
	catch(...)
	{
		EMDebugger("ERROR! Exception in MediaCommandSetActiveMIDITrack::ExecuteE");
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
			EMDebugger("ERROR! Excecption in MediaCommandSetActiveMIDITrack::ExecuteE");
		}
		opProject -> GetUnusedTracks() -> UnlockContainer();
	}
	
	if(opTrack != NULL && (opTrack -> GetType() & EM_TYPE_MIDI) > 0)
	{
		EMMediaMIDITrack* opMIDITrack = static_cast<EMMediaMIDITrack*>(opTrack);
//		EMRealtimeMIDIOutputDescriptor* opTrackDestination = static_cast<EMRealtimeMIDIOutputDescriptor*>(opMIDITrack -> GetTrackDestination());

		//opProducer ->SetActiveMIDIRoutingOutputChannel(opMIDITrack
		
		EMMediaEngine::Instance() -> GetMediaProject() -> GetUsedTracks() -> SetAsActiveTrack(opMIDITrack);
		EMMediaEngine::Instance() -> GetMediaProject() -> GetUnusedTracks() -> SetAsActiveTrack(opMIDITrack);
		EMWinDXMIDIConsumer::Instance() -> SetTrackAsListener(opMIDITrack, static_cast<EMRealtimeMIDIInputDescriptor*>(opMIDITrack -> GetInput()), static_cast<EMRealtimeMIDIOutputDescriptor*>(opMIDITrack -> GetTrackDestination()));
		EMWinDXMIDIProducer::Instance() -> SetActiveMIDIRoutingOutputChannel(opMIDITrack -> GetMIDIChannel());
		EMMediaEngine::Instance() -> GetMediaProject() -> SetDirty(true);
//		EMInputRepository::Instance() -> SetAsActiveMIDIInput(vInputID, vOutputID);
//		((EMWinRealtimeMIDIOutputDescriptor*) opMIDIOutputDevice) -> GetProducer() -> SetActiveMIDIRoutingOutputChannel(opMIDITrack -> GetMIDIChannel());
	} 
	return NULL;
}

bool MediaCommandSetActiveMIDITrack::RequiresParameters()
{
	return true;
}

void MediaCommandSetActiveMIDITrack::UndoE()
{
	if(m_vRecentTrackID)
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
			EMDebugger("ERROR! Exception in MediaCommandSetActiveMIDITrack::UndoE");
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
				EMDebugger("ERROR! Exception in MediaCommandSetActiveMIDITrack::UndoE");
			}
			opProject -> GetUnusedTracks() -> UnlockContainer();
		}
		
		if(opTrack != NULL)
		{
			//opTrack -> SetDestination(opOutput);
//			opTrack -> SetInput(opInput);
			//Do somehting else!
		}
	}
	else
		EMDebugger("Bad stored track or bad stored input ID in SetTrackInput Command");
}

bool MediaCommandSetActiveMIDITrack::IsUndoable()
{
	return false;
}

EMCommand* MediaCommandSetActiveMIDITrack::CloneAndClear()
{
	return EM_new MediaCommandSetActiveMIDITrack(m_vRecentTrackID);
}

