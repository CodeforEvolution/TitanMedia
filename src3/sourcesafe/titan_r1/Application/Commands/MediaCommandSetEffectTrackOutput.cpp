#include "MediaCommandSetEffectTrackOutput.h"

#include "EMMediaEngine.h"
#include "EMMediaProject.h"
#include "EMOutputRepository.h"
#include "EMMediaEffectTrack.h"
#include "EMRealtimeOutputDescriptor.h"
#include "EMMediaTrackRepository.h"
#include "EMMediaEffectTrack.h"
#include "EMMediaEffectTrackRepository.h"
#include "EMMediaTimer.h"
#include "EMMediaUtility.h"
#include "EMWinMediaUtility.h"

MediaCommandSetEffectTrackOutput::MediaCommandSetEffectTrackOutput(int32 p_vRecentTrackID, int32 p_vRecentOutputID)
	:	m_vRecentTrackID(p_vRecentTrackID),
		m_vRecentOutputID(p_vRecentOutputID)
{
}

void* MediaCommandSetEffectTrackOutput::ExecuteE(void* p_upFXTrackID, void* p_upOutputID, void* p_upOutputIsEffectTrack)
{
	//;//cout_commented_out_4_release << "MediaCommandSetEffectTrackOutput" << endl;
	EMMediaEngine* opEngine = EMMediaEngine::Instance();
	EMMediaProject* opProject = opEngine -> GetMediaProject();
	EMMediaTimer* opTimer = EMMediaTimer::Instance();
	EMOutputRepository* opOutputs = EMOutputRepository::Instance();
	EMMediaEffectTrackRepository* opFXTracks = EMMediaEffectTrackRepository::Instance();

	int32 vEffectTrackID = *(static_cast<int32*>(p_upFXTrackID));
	int32 vOutputID = *(static_cast<int32*>(p_upOutputID));
	bool vOutputIsEffectTrack = (p_upOutputIsEffectTrack == NULL ? false : (*(static_cast<bool*>(p_upOutputIsEffectTrack))));

	//eo << "Attempting to locate effect track ID " << vEffectTrackID << ef;
	//eo << "As well as output ID " << vOutputID << ef;
	
	EMMediaEffectTrack* opFXTrack = NULL;
	opFXTracks -> LockContainer();
	try
	{
		opFXTrack = opFXTracks -> Find(vEffectTrackID);
	}
	catch(...)
	{
		EMDebugger("ERROR! Exception in MediaCommandSetEffectTrackOutput::ExecuteE");
	}
	opFXTracks -> UnlockContainer();
	if(opFXTrack == NULL)
	{
		EMDebugger("ERROR! Could not find the specified effect track ID!");
		return NULL;
	}

	EMBufferDestination* opOutput = NULL;

	if(! vOutputIsEffectTrack)
	{
		opOutputs -> LockContainer();
		try
		{
			opOutput = opOutputs -> Find(vOutputID);
		}
		catch(...)
		{
			EMDebugger("ERROR! Exception in MediaCommandSetEffectTrackOutput::ExecuteE");
		}
		opOutputs -> UnlockContainer();
	}
	else
	{
		opFXTracks -> LockContainer();
		try
		{
			opOutput = opFXTracks -> Find(vOutputID);
		}
		catch(...)
		{
			EMDebugger("ERROR! Exception in MediaCommandSetEffectTrackOutput::ExecuteE");
		}
		opFXTracks -> UnlockContainer();
	}

	if(opFXTrack != NULL && opOutput != NULL)
	{
		if(opFXTrack -> GetOutput() != NULL)
		{
			m_vRecentTrackID = vEffectTrackID;
			m_vRecentOutputID = opFXTrack -> GetOutput() -> GetID();
		}
		else
		{
			m_vRecentTrackID = vEffectTrackID;
			m_vRecentOutputID = -1;
		}
			
/*		if(opOutput -> IsRenderOutput())
		{
		}
		else
		{
		} */

//		if(opTrack -> GetTrackDestination() != NULL)
//		{
			//If the track's destination is an effect track, then we should set that effect track's output to send to that instead!
//			if(opTrack -> GetTrackDestination() -> GetPosition() == EM_EFFECT)
//				((EMMediaEffectTrack*) opTrack -> GetTrackDestination()) -> SetEffectTrackOutput(opOutput);
//		}
//		else
		opFXTrack -> SetOutput(opOutput);
		EMMediaEngine::Instance() -> GetMediaProject() -> SetDirty(true);

		EMRealtimeOutputDescriptor* opOut = EMOutputRepository::Instance() -> FindClockMaster();
		if(opOut != NULL)
		{
			opOut -> RegisterAsClockMaster();
			EMMediaTimer::Instance() -> RegisterClockMaster(opOut);
		}
//		EMDebugger("STOP");
	}
	else
	{
//		eo << "ERROR! Couldnt find output ID " << vOutputID << " or couldnt find" << ef;
//		eo << "       effect track ID " << vEffectTrackID << ef;
//		EMDebugger("ERROR! Unknown effect track or bad output descriptor id!");
	}

	if(opProject -> IsPlaying())
	{
		EMMediaTimerState eMode = opTimer -> GetState();
		if(eMode == EM_PLAYING || eMode == EM_RECORDING)
			opTimer -> SetState(EM_STOPPED);
		opTimer -> SeekToFrame(opTimer -> NowFrame());
		if(eMode == EM_PLAYING || eMode == EM_RECORDING)
			opTimer -> SetState((eMode == EM_RECORDING ? EM_COUNTING_IN_FOR_RECORDING : EM_COUNTING_IN_FOR_PLAYBACK));
	}

	return NULL;
}

bool MediaCommandSetEffectTrackOutput::RequiresParameters()
{
	return true;
}

void MediaCommandSetEffectTrackOutput::UndoE()
{
/*	if(m_vRecentTrackID != -1 && m_vRecentOutputID != -1)
	{
		EMMediaEngine* opEngine = EMMediaEngine::Instance();
		EMMediaProject* opProject = opEngine -> GetMediaProject();
		EMOutputRepository* opOutputs = EMOutputRepository::Instance();
		//opProject -> GetUsedTracks() -> LockContainer();
		EMMediaTrack* opTrack = opProject -> GetUsedTracks() -> Find(m_vRecentTrackID);
		//opProject -> GetUsedTracks() -> UnlockContainer();
		if(opTrack == NULL)
		{
			//opProject -> GetUnusedTracks() -> LockContainer();
			opProject -> GetUnusedTracks() -> Find(m_vRecentTrackID);
			//opProject -> GetUnusedTracks() -> UnlockContainer();
		}

		//opOutputs -> LockContainer();		
		EMRealtimeOutputDescriptor* opOutput = opOutputs -> Find(m_vRecentOutputID);
		//opOutputs -> UnlockContainer();
		
		if(opTrack != NULL && opOutput != NULL)
			opTrack -> SetTrackDestination(opOutput);
	} */
}

bool MediaCommandSetEffectTrackOutput::IsUndoable()
{
	return false;
}

EMCommand* MediaCommandSetEffectTrackOutput::CloneAndClear()
{
	return EM_new MediaCommandSetEffectTrackOutput(m_vRecentTrackID, m_vRecentOutputID);
}

