#include "MediaInfoCommandGetTrackInfo.h"

#include "EMGlobals.h"
#include "EMMediaEngine.h"
#include "EMMediaProject.h"
#include "EMMediaTrackRepository.h"
#include "EMMediaClipRepository.h"
#include "EMMediaTransitionTrackRepository.h"
#include "EMMediaTrack.h"
#include "EMMediaMIDITrack.h"
#include <iostream>
#include <list>

MediaInfoCommandGetTrackInfo::MediaInfoCommandGetTrackInfo()
{
}

void* MediaInfoCommandGetTrackInfo::ExecuteE(void* p_upMediaTrackID, void*, void*)
{
	//eo << "MediaInfoCommandGetTrackInfo" << ef;
	//TODO: Throw exception if fails:
	int32 vMediaTrackID = *static_cast<int32*>(p_upMediaTrackID);
	
	EMMediaProject* opProject = EMMediaEngine::Instance() -> GetMediaProject();
	EMMediaTrackRepository* opUsedTracks = opProject -> GetUsedTracks();
	EMMediaTrackRepository* opUnusedTracks = opProject -> GetUnusedTracks();
	EMMediaTransitionTrackRepository* opTransitionTracks = opProject -> GetVideoTransitionTrackRepository();

	EMMediaTrack* opTrack = NULL;
	opUsedTracks -> LockContainer();
	try
	{
		opTrack = opUsedTracks -> Find(vMediaTrackID);
	}
	catch(...)
	{
		EMDebugger("ERROR! Exception in MediaInfoCommandGetTrackInfo::ExecuteE");
	}
	opUsedTracks -> UnlockContainer();
	if(opTrack == NULL)
	{
		opUnusedTracks -> LockContainer();
		try
		{
			opTrack = opUnusedTracks -> Find(vMediaTrackID);
		}
		catch(...)
		{
			EMDebugger("ERROR! Exception in MediaInfoCommandGetTrackInfo::ExecuteE");
		}
		opUnusedTracks -> UnlockContainer();
	}
	if(opTrack == NULL)
	{
		opTransitionTracks -> LockContainer();
		try
		{
			opTrack = dynamic_cast<EMMediaTrack*>(opTransitionTracks -> Find(vMediaTrackID));
		}
		catch(...)
		{
			EMDebugger("ERROR! Exception in MediaInfoCommandGetTrackInfo::ExecuteE");
		}
		opTransitionTracks -> UnlockContainer();
	}


	if(opTrack == NULL)
		return static_cast<void*>(NULL); 
	
	m_oReturnValue.m_vID = opTrack -> GetID();
	m_oReturnValue.m_eType = opTrack -> GetType();
	m_oReturnValue.m_vFaderLevel = opTrack -> GetFader();
	m_oReturnValue.m_vPanLevel = opTrack -> GetPan();
	m_oReturnValue.m_vStartOffset = 0; //TODO: Put in get/set methods in EMMediaTrack
	m_oReturnValue.m_vPriority = opTrack -> GetPriority();
	m_oReturnValue.m_eMuteState = opTrack -> GetMuteState();
	m_oReturnValue.m_vIsSoloActive = opTrack -> IsSoloed();
	m_oReturnValue.m_vIsArmActive = opTrack -> IsArmed();
	m_oReturnValue.m_vIsRenderActive = opTrack -> IsRender(); //TODO: Make MediaEngine store the Render-status for each track!
	m_oReturnValue.m_oName = opTrack -> GetName();
	m_oReturnValue.m_vInputID = (opTrack -> GetInput() != NULL ? opTrack -> GetInput() -> GetID() : -1);
	m_oReturnValue.m_vOutputID = (opTrack -> GetTrackDestination() != NULL ? opTrack -> GetTrackDestination() -> GetID() : -1);
	m_oReturnValue.m_vOutputIsEffectTrack = (opTrack -> GetTrackDestination() != NULL ? (opTrack -> GetTrackDestination() -> GetPosition() == EM_EFFECT) : false);

	if(opTrack -> GetType() == EM_TYPE_MIDI)
	{
		m_oReturnValue.m_vMIDIChannel = 1 + static_cast<EMMediaMIDITrack*>(opTrack) -> GetMIDIChannel(); //MIDI channels are zero-based in media engine.
		m_oReturnValue.m_vVelocityOffset = static_cast<EMMediaMIDITrack*>(opTrack) -> GetVelocityOffset();
		m_oReturnValue.m_vKeyOffset = static_cast<EMMediaMIDITrack*>(opTrack) -> GetKeyOffset();
	}
	else
	{
		m_oReturnValue.m_vMIDIChannel = 0;
		m_oReturnValue.m_vVelocityOffset = 0;
		m_oReturnValue.m_vKeyOffset = 0;
	}

	m_oReturnValue.m_oMediaClipIDs.clear();

	EMMediaClipRepository* opClips = NULL;
	if(m_oReturnValue.IsAudio()) 
		opClips = opProject -> GetAudioClipRepository();
	else if(m_oReturnValue.IsVideo())
		opClips = opProject -> GetVideoClipRepository();
	else if(m_oReturnValue.IsMIDI())
		opClips = opProject -> GetMIDIClipRepository();
	else if(m_oReturnValue.IsTransition())
		opClips = opProject -> GetVideoTransitionClipRepository();
	else
	{
		cout << "ERROR! Unknown track type in MediaInfoCommandGetTrackInfo!" << endl;
		return static_cast<void*>(NULL);
	}

	opClips -> LockContainer();
	try
	{
		opClips -> Rewind();
		while(opClips -> Current() != NULL)
		{
			if(opClips -> Current() -> GetTrack() -> GetID() == opTrack -> GetID() && 
				! opClips -> Current() -> IsObjectDeleted()) //Make sure we don't show the track's deleted-but-unpurged clips!
				m_oReturnValue.m_oMediaClipIDs.push_back(opClips -> Current() -> GetID());
			opClips -> Next();
		}
	}
	catch(...)
	{
		EMDebugger("ERROR! Exception in MediaInfoCommandGetTrackInfo::ExecuteE");
	}
	opClips -> UnlockContainer();
//	m_oReturnValue.Display();

	return static_cast<void*>(&m_oReturnValue);
}

bool MediaInfoCommandGetTrackInfo::RequiresParameters()
{
	return true;
}

bool MediaInfoCommandGetTrackInfo::IsUndoable()
{
	return false;
}