#include "MediaCommandSetTrackMIDIChannel.h"

#include "EMMediaEngine.h"
#include "EMMediaProject.h"
#include "EMMediaTrack.h"
#include "EMMediaTrackRepository.h"
#include "EMMediaMIDITrack.h"

#include "EMWinDXMIDIConsumer.h"
#include "EMRealtimeMIDIInputDescriptor.h"
#include "EMRealtimeMIDIOutputDescriptor.h"

MediaCommandSetTrackMIDIChannel::MediaCommandSetTrackMIDIChannel(int32 p_vRecentTrackID, int32 p_vRecentMIDIChannel)
	:	m_vRecentTrackID(p_vRecentTrackID),
		m_vRecentMIDIChannel(p_vRecentMIDIChannel)
{
}

void* MediaCommandSetTrackMIDIChannel::ExecuteE(void* p_upTrackID, void* p_upMIDIChannel, void*)
{
	//eo << "MediaCommandSetTrackMIDIChannel" << ef;
	EMMediaEngine* opEngine = EMMediaEngine::Instance();
	EMMediaProject* opProject = opEngine -> GetMediaProject(); 
	EMInputRepository* opInputs = EMInputRepository::Instance();

	int32 vTrackID = *(static_cast<int32*>(p_upTrackID));
	int32 vMIDIChannel = *(static_cast<int32*>(p_upMIDIChannel));
	vMIDIChannel--; //MIDI channels are zero-based inside media engine!

	EMMediaTrack* opTrack = NULL;
	opProject -> GetUsedTracks() -> LockContainer();
	try
	{
		opTrack = opProject -> GetUsedTracks() -> Find(vTrackID);
	}
	catch(...)
	{
		EMDebugger("ERROR! Exception in MediaCommandSetTrackMIDIChannel::ExecuteE");
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
			EMDebugger("ERROR! Excecption in MediaCommandSetTrackMIDIChannel::ExecuteE");
		}
		opProject -> GetUnusedTracks() -> UnlockContainer();
	}
	
	
	if(opTrack != NULL)
	{
		if((opTrack -> GetType() & EM_TYPE_MIDI) > 0)
		{
			EMMediaMIDITrack* opMIDITrack = static_cast<EMMediaMIDITrack*>(opTrack);
			m_vRecentMIDIChannel = opMIDITrack -> GetMIDIChannel();
			opMIDITrack -> SetMIDIChannel(vMIDIChannel);
			EMMediaEngine::Instance() -> GetMediaProject() -> SetDirty(true);
			if(opMIDITrack -> IsActive())
			{
				EMWinDXMIDIConsumer::Instance() -> SetTrackAsListener(opMIDITrack, static_cast<EMRealtimeMIDIInputDescriptor*>(opMIDITrack -> GetInput()), static_cast<EMRealtimeMIDIOutputDescriptor*>(opMIDITrack -> GetTrackDestination()));
				//EMWinDXMIDIProducer::Instance() -> MapInputToOutput(static_cast<EMRealtimeMIDIInputDescriptor*>(opMIDITrack -> GetInput()), static_cast<EMRealtimeMIDIOutputDescriptor*>(opMIDITrack -> GetTrackDestination()), vMIDIChannel);
				//EMWinDXMIDIProducer::Delete();
			}
		}
	}
	else
		EMDebugger("ERROR! Could not find media track id, in MediaCommandSetTrackMIDIChannel");
	return NULL;
}

bool MediaCommandSetTrackMIDIChannel::RequiresParameters()
{
	return true;
}

void MediaCommandSetTrackMIDIChannel::UndoE()
{
	if(m_vRecentTrackID != -1 && m_vRecentMIDIChannel != -1)
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
			EMDebugger("ERROR! Exception in MediaCommandSetTrackMIDIChannel::UndoE");
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
				EMDebugger("ERROR! Exception in MediaCommandSetTrackMIDIChannel::UndoE");
			}
			opProject -> GetUnusedTracks() -> UnlockContainer();
		}
		
		if(opTrack != NULL)
		{
			if((opTrack -> GetType() & EM_TYPE_MIDI) > 0)
			{
				EMMediaMIDITrack* opMIDITrack = static_cast<EMMediaMIDITrack*>(opTrack);
				opMIDITrack -> SetMIDIChannel(m_vRecentMIDIChannel);
			}
		}
	}
	else
		EMDebugger("Bad stored track or bad stored input ID in SetTrackMIDIChannel Command");
}

bool MediaCommandSetTrackMIDIChannel::IsUndoable()
{
	return false;
}

EMCommand* MediaCommandSetTrackMIDIChannel::CloneAndClear()
{
	return EM_new MediaCommandSetTrackMIDIChannel(m_vRecentTrackID, m_vRecentMIDIChannel);
}

