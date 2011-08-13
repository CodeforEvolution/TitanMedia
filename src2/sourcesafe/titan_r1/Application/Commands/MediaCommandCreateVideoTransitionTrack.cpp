#include "MediaCommandCreateVideoTransitionTrack.h"

#include "EMMediaEngine.h"
#include "EMMediaTrack.h"
#include "EMMediaProject.h"
#include "EMMediaTrackRepository.h"
#include "EMMediaVideoTransitionTrack.h"
#include "EMMediaTransitionTrackRepository.h"
#include "EMMediaEffectTrack.h"

MediaCommandCreateVideoTransitionTrack::MediaCommandCreateVideoTransitionTrack()
{
}

MediaCommandCreateVideoTransitionTrack::MediaCommandCreateVideoTransitionTrack(int32 p_vID)
  : m_vID(p_vID)
{
}

MediaCommandCreateVideoTransitionTrack::~MediaCommandCreateVideoTransitionTrack()
{
}


void MediaCommandCreateVideoTransitionTrack::ExecuteE()
{
	EMDebugger("You should not create an orphaned transition track!!");
}

EMMediaTrack* MediaCommandCreateVideoTransitionTrack::FindParentTrackE(uint32 p_vParentID)
{ 
	EMMediaTrack* opTrack = NULL;
	EMMediaTrackRepository* opUsed = EMMediaEngine::Instance() -> GetMediaProject() -> GetUsedTracks();
	EMMediaTrackRepository* opFree = EMMediaEngine::Instance() -> GetMediaProject() -> GetUnusedTracks();
	
	opUsed -> LockContainer();
	opTrack = opUsed -> Find(p_vParentID);
	opUsed -> UnlockContainer();

	if(opTrack == NULL)
	{
		opFree -> LockContainer();
		opTrack = opFree -> Find(p_vParentID);
		opFree -> UnlockContainer();
	}

	return opTrack;
}

void* MediaCommandCreateVideoTransitionTrack::ExecuteE(void* p_opParameterOne, void* p_opParameterTwo, void* p_opParameterThree)
{
	try
	{
		char* vpName = static_cast<char*>(p_opParameterOne);
		uint32 vParentID = *(static_cast<uint32*>(p_opParameterTwo));

		EMMediaTrack* opTrack = FindParentTrackE(vParentID);

		if(opTrack == NULL)
		{
			EMDebugger("MediaCommandCreateVideoTransitionTrack - Couldn't find parent track!!");
		}

		uint32 vPrio = opTrack -> GetPriority();

		EMMediaVideoTransitionTrack* opTrans = static_cast<EMMediaVideoTransitionTrack*>(EMMediaTrack::CreateEMTrack(EM_TYPE_TRANSITION, string(vpName), vPrio));
		opTrans -> InitCheckE();

		m_vID = static_cast<EMMediaTrack*>(opTrans) -> GetID();

//		opTimer -> SeekToFrame(opTimer -> NowFrame());

		EMMediaEngine::Instance() -> GetMediaProject() -> GetVideoTransitionTrackRepository() -> Add(opTrans);

		opTrans -> SetParentID(vParentID);

		EMBufferDestination* opDest = opTrack -> GetTrackDestination();
		EMBufferDestination* opNextDest;

		if(opDest != NULL && opDest -> GetPosition() == EM_EFFECT)
		{
			opNextDest = static_cast<EMMediaEffectTrack*>(opDest) -> GetOutput();
			while(opNextDest != NULL && opNextDest -> GetPosition() == EM_EFFECT)
			{
				opDest = opNextDest;
				opNextDest = static_cast<EMMediaEffectTrack*>(opDest) -> GetOutput();
			}

	
			if(opNextDest != NULL)
			{
				if(opDest -> GetPosition() == EM_EFFECT)	//Pure paranoia
				{
					opTrans -> SetTrackDestination(static_cast<EMMediaEffectTrack*>(opDest) -> GetOutput());
					EMBufferDestination* opTransBufferDestination = opTrans;
					static_cast<EMMediaEffectTrack*>(opDest) -> SetOutput(opTransBufferDestination);
				}
				else
					EMDebugger("MediaCommandCreateVideoTransitionTrack::ExecuteE - Have you added a new track-type?");
			}
			else
				MessageBox(NULL, "Video processessing is corrupted!\n\nReason: This video track is not connected to a video-renderer.\n\nThe reason may be one of (but not limited to) the following:\n\n1\tTitan was not installed properly.\n\nIf none of the above is true please let us know at bugs@elegant-media.com.\nDescribe the problem, your system and what you did to encounter this \nbehaviour (or lack thereof). Don't forget to include your name and some \nmeans for us to contact you.", "Error!", MB_OK | MB_ICONEXCLAMATION);

		}
		else
		{
			opTrans -> SetTrackDestination(opTrack -> GetTrackDestination());
			opTrack -> SetTrackDestination(opTrans);
		}


		EMMediaEngine::Instance() -> GetMediaProject() -> SetDirty(true);
	}
	catch(...)
	{
		return NULL;
	}

	return &m_vID;
}

bool MediaCommandCreateVideoTransitionTrack::RequiresParameters()
{
	return true;
}

bool MediaCommandCreateVideoTransitionTrack::IsUndoable()
{
	return false;
}

void MediaCommandCreateVideoTransitionTrack::UndoE()
{
	EMMediaVideoTransitionTrack* opTrans = EMMediaEngine::Instance() -> GetMediaProject() -> GetVideoTransitionTrackRepository() -> Find(m_vID);

	if(opTrans != NULL)
	{
		opTrans -> SetDeleted(true);
	}
}
	
EMCommand* MediaCommandCreateVideoTransitionTrack::CloneAndClear()
{
	return EM_new MediaCommandCreateVideoTransitionTrack(m_vID);
}
