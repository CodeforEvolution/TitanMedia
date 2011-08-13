#include "MediaCommandSetTrackOutput.h"

#include "EMMediaEngine.h"
#include "EMMediaProject.h"
#include "EMOutputRepository.h"
#include "EMMediaTrack.h"
#include "EMRealtimeOutputDescriptor.h"
#include "EMMediaTrackRepository.h"
#include "EMMediaEffectTrack.h"
#include "EMMediaEffectTrackRepository.h"

#include "EMMediaMIDITrack.h"
#include "EMWinDXMIDIConsumer.h"
#include "EMWinDXMIDIProducer.h"
#include "EMRealtimeMIDIInputDescriptor.h"
#include "EMRealtimeMIDIOutputDescriptor.h"
#include "EMMediaTimer.h"

MediaCommandSetTrackOutput::MediaCommandSetTrackOutput(int32 p_vRecentTrackID, int32 p_vRecentOutputID)
	:	m_vRecentTrackID(p_vRecentTrackID),
		m_vRecentOutputID(p_vRecentOutputID)
{
} 

void* MediaCommandSetTrackOutput::ExecuteE(void* p_upTrackID, void* p_upOutputID, void* p_upIsEffectTrack)
{
	//eo << "MediaCommandSetTrackOutput" << ef;
 
	EMMediaEngine* opEngine = EMMediaEngine::Instance();
	EMMediaProject* opProject = opEngine -> GetMediaProject();
	EMOutputRepository* opOutputs = EMOutputRepository::Instance();

	int64 vSi = EMOutputRepository::Instance() ->Size();

	int32 vTrackID = *(static_cast<int32*>(p_upTrackID));
	int32 vOutputID = *(static_cast<int32*>(p_upOutputID));
	bool vOutputIsEffectTrack = (p_upIsEffectTrack == NULL ? false : *static_cast<bool*>(p_upIsEffectTrack));

	EMMediaTrack* opTrack = NULL;
	opProject -> GetUsedTracks() -> LockContainer();
	try
	{
		opTrack = opProject -> GetUsedTracks() -> Find(vTrackID);
	}
	catch(...)
	{
		EMDebugger("ERROR! Exception in MediaCommandSetTrackOutput::ExecuteE");
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
			EMDebugger("ERROR! Exception in MediaCommandSetTrackOutput::ExecuteE");
		}
		opProject -> GetUnusedTracks() -> UnlockContainer();
	}

/*	EMRealtimeOutputDescriptor* opPhysicalOutput = NULL;
	opOutputs -> LockContainer();
	try
	{
		int64 v = opOutputs -> Size();
		opPhysicalOutput = opOutputs -> Find(vOutputID);
	}
	catch(...)
	{
		EMDebugger("ERROR! Exception in MediaCommandSetTrackOutput::ExecuteE");
	}
	opOutputs -> UnlockContainer();

	if(opTrack != NULL && opPhysicalOutput != NULL)
	{
		if(SetTrackPhysicalOutput(opTrack, opPhysicalOutput))
		{
			if((opTrack -> GetType() & EM_TYPE_ANY_AUDIO) > 0)
			{
				opPhysicalOutput -> SetClockMaster(true);

			}
		}
	}

	if((opTrack -> GetType() & EM_TYPE_ANY_AUDIO) > 0 && opTrack -> GetTrackDestination() != NULL)
	{	//Look's like we have an output already!
		EMBufferDestination* opDest = opTrack -> GetTrackDestination();
		if((opDest -> GetType() & EM_OUTPUT_TYPE_EFFECTSLOT) > 0)
		{
			static_cast<EMMediaEffectTrack*>(opDest) -> SetOutput(opPhysicalOutput);
		}
		else
			EMDebugger("ERROR! You can no longer set outputs directly on a MediaTrack. Set them for the effect track instead!");
	} */

	if(! vOutputIsEffectTrack)
	{
		EMRealtimeOutputDescriptor* opPhysicalOutput = NULL;
		opOutputs -> LockContainer();
		try
		{
			int64 v = opOutputs -> Size();
			opPhysicalOutput = opOutputs -> Find(vOutputID);
		}
		catch(...)
		{
			EMDebugger("ERROR! Exception in MediaCommandSetTrackOutput::ExecuteE");
		}
		opOutputs -> UnlockContainer();

		if(opTrack != NULL && opPhysicalOutput != NULL)
		{
			if(SetTrackPhysicalOutput(opTrack, opPhysicalOutput))
			{
				if((opTrack -> GetType() & EM_TYPE_ANY_AUDIO) > 0)
				{
					opPhysicalOutput -> SetClockMaster(true);

				}
				EMMediaEngine::Instance() -> GetMediaProject() -> SetDirty(true);
			}
		}

		if(opPhysicalOutput == NULL)
		{
			return NULL;
		}

		EMRealtimeOutputDescriptor* opOut = EMOutputRepository::Instance() -> FindClockMaster();
		if(opOut != NULL)
		{
			opOut -> RegisterAsClockMaster();
			EMMediaTimer::Instance() -> RegisterClockMaster(opOut);
		}
	}
	else
	{
		EMMediaEffectTrack* opMediaEffectTrack = NULL;
		EMMediaEffectTrackRepository::Instance() -> LockContainer();
		try
		{
			opMediaEffectTrack = EMMediaEffectTrackRepository::Instance() -> Find(vOutputID);
		}
		catch(...)
		{
			EMDebugger("ERROR! Exception in MediaCommandSetTrackOutput::ExecuteE");
		}
		EMMediaEffectTrackRepository::Instance() -> UnlockContainer();
		
		if(opMediaEffectTrack != NULL)
		{
			SetTrackEffectOutput(opTrack, opMediaEffectTrack);
			EMMediaEngine::Instance() -> GetMediaProject() -> SetDirty(true);
		}
		else
			EMDebugger("ERROR! Couldn't find the specified effect track ID!");
	}

	return NULL;
}

bool MediaCommandSetTrackOutput::RequiresParameters()
{
	return true;
}

void MediaCommandSetTrackOutput::UndoE()
{
	if(m_vRecentTrackID != -1 && m_vRecentOutputID != -1)
	{
		EMMediaEngine* opEngine = EMMediaEngine::Instance();
		EMMediaProject* opProject = opEngine -> GetMediaProject();
		EMOutputRepository* opOutputs = EMOutputRepository::Instance();
		
		EMMediaTrack* opTrack = NULL;
		opProject -> GetUsedTracks() -> LockContainer();
		try
		{
			opTrack = opProject -> GetUsedTracks() -> Find(m_vRecentTrackID);
		}
		catch(...)
		{
			EMDebugger("ERROR! Exception in MediaCommandSetTrackOutput::ExecuteE");
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
				EMDebugger("ERROR! Exception in MediaCommandSetTrackOutput::ExecuteE");
			}
			opProject -> GetUnusedTracks() -> UnlockContainer();
		}

		EMRealtimeOutputDescriptor* opOutput = NULL;
		opOutputs -> LockContainer();		
		try
		{
			opOutput = opOutputs -> Find(m_vRecentOutputID);
		}
		catch(...)
		{
			EMDebugger("ERROR! Exception in MediaCommandSetTrackOutput::ExecuteE");
		}
		opOutputs -> UnlockContainer();
		
		if(opTrack != NULL && opOutput != NULL)
			opTrack -> SetTrackDestination(opOutput);
	}
}

bool MediaCommandSetTrackOutput::IsUndoable()
{
	return false;
}

EMCommand* MediaCommandSetTrackOutput::CloneAndClear()
{
	return EM_new MediaCommandSetTrackOutput(m_vRecentTrackID, m_vRecentOutputID);
}

bool MediaCommandSetTrackOutput::SetTrackPhysicalOutput(EMMediaTrack* p_opTrack, EMRealtimeOutputDescriptor* p_opPhysicalOutput)
{
	eo << "Now setting a media track's (" << p_opTrack ->GetID() << ") output to be a physical output (" << p_opPhysicalOutput ->GetID() << ")" << ef;
	EMMediaEngine* opEngine = EMMediaEngine::Instance();
	EMMediaProject* opProject = opEngine -> GetMediaProject();

	//If the output is a regular "physical" output...
	if(p_opPhysicalOutput -> IsRenderOutput())
	{
		p_opTrack -> SetRender(true);
		if((p_opTrack -> GetType() & EM_TYPE_ANY_AUDIO) > 0)
			opProject -> SetRenderingAudio(true);
		else if((p_opTrack -> GetType() & EM_TYPE_ANY_VIDEO) > 0)
			opProject -> SetRenderingVideo(true);
	}
	else
	{
		p_opTrack -> SetRender(false); //Fix me!

		bool vProjectAudioRenderStatus = false;
		bool vProjectVideoRenderStatus = false;
	
		try
		{
			opProject -> GetUsedTracks() -> LockContainer();

			opProject -> GetUsedTracks() -> Rewind();
			while(opProject -> GetUsedTracks() -> Current() != NULL)
			{
				if(opProject -> GetUsedTracks() -> Current() -> IsRender())
				{
					if((opProject -> GetUsedTracks() -> Current() -> GetType() & EM_TYPE_ANY_AUDIO) > 0)
						vProjectAudioRenderStatus = true;
					else if((opProject -> GetUsedTracks() -> Current() -> GetType() & EM_TYPE_ANY_VIDEO) > 0)
						vProjectVideoRenderStatus = true;
				}
				opProject -> GetUsedTracks() -> Next();
			}
		}
		catch(...)
		{
			EMDebugger("ERROR! Exception in MediaCommandSetTrackOutput::ExecuteE");
		}
		opProject -> GetUsedTracks() -> UnlockContainer();
		
		opProject -> GetUnusedTracks() -> LockContainer();
		try
		{
			opProject -> GetUnusedTracks() -> Rewind();
			while(opProject -> GetUnusedTracks() -> Current() != NULL)
			{
				if(opProject -> GetUnusedTracks() -> Current() -> IsRender())
				{
					if((opProject -> GetUnusedTracks() -> Current() -> GetType() & EM_TYPE_ANY_AUDIO) > 0)
						vProjectAudioRenderStatus = true;
					else if((opProject -> GetUnusedTracks() -> Current() -> GetType() & EM_TYPE_ANY_VIDEO) > 0)
						vProjectVideoRenderStatus = true;
				}
				opProject -> GetUnusedTracks() -> Next();
			}
		}
		catch(...)
		{
			EMDebugger("ERROR! Exception in MediaCommandSetTrackOutput::ExecuteE");
		}
		opProject -> GetUnusedTracks() -> UnlockContainer();
		
		opProject -> SetRenderingAudio(vProjectAudioRenderStatus);
		opProject -> SetRenderingVideo(vProjectVideoRenderStatus);
	}

	EMBufferDestination* opOldOutput = NULL;

	if(p_opTrack -> GetTrackDestination() != NULL && (p_opTrack -> GetType() & EM_TYPE_ANY_AUDIO) > 0)
	{
		if(p_opTrack -> GetTrackDestination() -> GetPosition() == EM_EFFECT)
		{
			opOldOutput = static_cast<EMMediaEffectTrack*>(p_opTrack -> GetTrackDestination()) -> GetOutput();
			static_cast<EMMediaEffectTrack*>(p_opTrack -> GetTrackDestination()) -> SetOutput(p_opPhysicalOutput);
		}
		else
		{
			opOldOutput = p_opTrack -> GetDestination();
			p_opTrack -> SetTrackDestination(p_opPhysicalOutput);
		}
	}
	else
	{
		opOldOutput = p_opTrack -> GetTrackDestination();
		p_opTrack -> SetTrackDestination(p_opPhysicalOutput);
	}

	if((p_opTrack -> GetType() & EM_TYPE_MIDI) > 0)
	{
//		if(static_cast<EMMediaMIDITrack*>(p_opTrack) -> IsActive())
//		{
			uint32 vMIDIChannel = static_cast<EMMediaMIDITrack*>(p_opTrack) -> GetMIDIChannel();
			if(opOldOutput != NULL)
			{
				EMWinDXMIDIConsumer::Instance() -> SetTrackAsListener(static_cast<EMMediaMIDITrack*>(p_opTrack), NULL, NULL); //p_opTrack -> GetInput());
//				EMWinDXMIDIProducer::Instance() -> UnmapInput(static_cast<EMRealtimeMIDIInputDescriptor*>(p_opTrack -> GetInput()));
//				EMWinDXMIDIProducer::Delete();
			}
			EMWinDXMIDIConsumer::Instance() -> SetTrackAsListener(static_cast<EMMediaMIDITrack*>(p_opTrack), static_cast<EMRealtimeMIDIInputDescriptor*>(p_opTrack -> GetInput()), static_cast<EMRealtimeMIDIOutputDescriptor*>(p_opTrack -> GetTrackDestination()));
//			EMWinDXMIDIProducer::Instance() -> MapInputToOutput(static_cast<EMRealtimeMIDIInputDescriptor*>(p_opTrack -> GetInput()), static_cast<EMRealtimeMIDIOutputDescriptor*>(p_opPhysicalOutput), vMIDIChannel);
//			EMWinDXMIDIProducer::Delete();
//		}
	}

	return true;
}

bool MediaCommandSetTrackOutput::SetTrackEffectOutput(EMMediaTrack* p_opTrack, EMMediaEffectTrack* p_opEffectTrack)
{
	eo << "Now setting a media track's (" << p_opTrack ->GetID() << ") output to be an effect track (" << p_opEffectTrack ->GetID() << ")" << ef;
	
//	p_opEffectTrack ->SetDestination(p_opTrack -> GetTrackDestination());
	p_opTrack -> SetTrackDestination(p_opEffectTrack);
	return true;
}