#include "MediaInfoCommandGetClipInfo.h"

#include "EMMediaEngine.h"
#include "EMMediaProject.h"
#include "EMMediaClipRepository.h"
#include "EMMediaClip.h"
#include "EMMediaFormat.h"
#include "EMMediaUtility.h"
#include "EMFileInputDescriptor.h"
#include "EMMediaTrack.h"

#include <list>

MediaInfoCommandGetClipInfo::MediaInfoCommandGetClipInfo()
{
}

void* MediaInfoCommandGetClipInfo::ExecuteE(void* p_upMediaClipID, void*, void*)
{
	//eo << "MediaInfoCommandGetClipInfo" << ef;
	int32 vMediaClipID = *static_cast<int32*>(p_upMediaClipID);
	
	EMMediaProject* opProject = EMMediaEngine::Instance() -> GetMediaProject(); 
	EMMediaClipRepository* opAudioClipRepository = opProject -> GetAudioClipRepository();
	EMMediaClipRepository* opVideoClipRepository = opProject -> GetVideoClipRepository();
	EMMediaClipRepository* opVideoTransitionClipRepository = opProject -> GetVideoTransitionClipRepository();
	EMMediaClipRepository* opMIDIClipRepository = opProject -> GetMIDIClipRepository();
	
	EMMediaClip* opClip = NULL;
	opAudioClipRepository -> LockContainer();
	try
	{
		opClip = opAudioClipRepository -> Find(vMediaClipID);
	}
	catch(...)
	{
		EMDebugger("ERROR! Exception in MediaInfoCommandGetClipInfo::ExecuteE");
	}
	opAudioClipRepository -> UnlockContainer();
	if(opClip == NULL)
	{
		opVideoClipRepository -> LockContainer();
		try
		{
			opClip = opVideoClipRepository -> Find(vMediaClipID);
		}
		catch(...)
		{
			EMDebugger("ERROR! Exception in MediaInfoCommandGetClipInfo::ExecuteE");
		}
		opVideoClipRepository -> UnlockContainer();
	}
	if(opClip == NULL)
	{
		opMIDIClipRepository -> LockContainer();
		try
		{
			opClip = opMIDIClipRepository -> Find(vMediaClipID);
		}
		catch(...)
		{
			EMDebugger("ERROR! Exception in MediaInfoCommandGetClipInfo::ExecuteE");
		}
		opMIDIClipRepository -> UnlockContainer();
	}
	if(opClip == NULL)
	{
		opVideoTransitionClipRepository -> LockContainer();
		try
		{
			opClip = opVideoTransitionClipRepository -> Find(vMediaClipID);
		}
		catch(...)
		{
			EMDebugger("ERROR! Exception in MediaInfoCommandGetClipInfo::ExecuteE");
		}
		opVideoTransitionClipRepository -> UnlockContainer();
	}
	if(opClip == NULL)
		return static_cast<void*>(NULL);
	
	EMMediaUtility* opUtility = EMMediaUtility::Instance();
	EMMediaFormat* opAudioFormat = opUtility -> GetSystemAudioFormat();
	
	int64 vStart = opClip -> GetStart();
	int64 vMarkInLength = opClip -> GetMarkInLength();
	int64 vMarkOutLength = opClip -> GetMarkOutLength();
	int64 vMediaLength = opClip -> GetMediaLength();
	int64 vActiveLength = opClip -> GetActiveLength();

	m_oReturnValue.m_vID = opClip -> GetID();
	m_oReturnValue.m_eType = ((opClip -> GetType() & EM_TYPE_ANY_IMAGE) != 0) ? EM_TYPE_RAW_VIDEO : opClip -> GetType();
	m_oReturnValue.m_vActiveStart = opUtility -> FramesToTime(vStart, opAudioFormat);
	m_oReturnValue.m_vMarkInDuration = opUtility -> FramesToTime(vMarkInLength, opAudioFormat);
	m_oReturnValue.m_vMarkOutDuration = opUtility -> FramesToTime(vMarkOutLength, opAudioFormat);
	m_oReturnValue.m_vPhysicalDuration = opUtility -> FramesToTime(vMediaLength, opAudioFormat);
	m_oReturnValue.m_vActiveDuration = opUtility -> FramesToTime(vActiveLength, opAudioFormat);
	m_oReturnValue.m_oName = opClip -> GetName();
	m_oReturnValue.m_oFileName = opClip -> GetFileName();
	m_oReturnValue.m_vMediaEntryID = (((opClip -> GetType() & EM_TYPE_MIDI) > 0 || ((opClip -> GetType() & EM_TYPE_TRANSITION) > 0)) ? 0 : opClip -> GetDescriptor() -> GetMediaPoolEntryID());
	m_oReturnValue.m_vMediaTrackID = opClip -> GetTrack() -> GetID();

/*	if(opClip ->GetType() & EM_TYPE_ANY_AUDIO)
		m_oReturnValue.m_vMediaEffectTrackID = (opClip -> GetDestination() != NULL ? (opClip -> GetDestination() -> GetPosition() == EM_EFFECT ? opClip -> GetDestination() -> GetID() : -1) : -1);
	else if(opClip ->GetType() & EM_TYPE_ANY_VIDEO)
	{
		EMBufferDestination* opDest = opClip -> GetDestination();
		if(opDest != opClip -> GetTrack() -> GetDestination() && opDest -> GetPosition() == EM_EFFECT)
			m_oReturnValue.m_vMediaEffectTrackID = opDest -> GetID();
		else
			m_oReturnValue.m_vMediaEffectTrackID = -1;
	}
	else*/
	m_oReturnValue.m_vMediaEffectTrackID = opClip -> GetEffectTrackID();

	return static_cast<void*>(&m_oReturnValue);
}

bool MediaInfoCommandGetClipInfo::RequiresParameters()
{
	return true;
}

bool MediaInfoCommandGetClipInfo::IsUndoable()
{
	return false;
}