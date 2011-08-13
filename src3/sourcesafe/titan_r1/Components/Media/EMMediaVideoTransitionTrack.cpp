#include "EMMediaVideoTransitionTrack.h"
#include "EMWinVideoTransitionClipRepository.h"
#include "EMMediaVideoTransitionClip.h"
#include "EMMediaTrackRepository.h"
#include "EMMediaEngine.h"
#include "EMMediaProject.h"
#include "EMMediaTimer.h"
#include "EMMediaUtility.h"
#include "EMMediaTrack.h"
#include "EMMediaEffectTrack.h"

EMMediaVideoTransitionTrack::EMMediaVideoTransitionTrack(string p_oName)
	:	EMMediaTrack(EM_TYPE_TRANSITION, p_oName.c_str(), 0),
	EMBufferDestination(EM_TYPE_ANY_VIDEO, EM_TRANSITION_TRACK, p_oName.c_str()), //     "EM_TRANSITION_TRACK"), //p_oName),
	m_opParentTrack(NULL),
	m_vParentID(-1)
{
	EMMediaTrack::m_vID = EMBufferDestination::GetID();
}

EMMediaVideoTransitionTrack::~EMMediaVideoTransitionTrack()
{
}



bool EMMediaVideoTransitionTrack::PrepareToPlayE()
{
	return true; 
}

EMMediaDataBuffer* EMMediaVideoTransitionTrack::ProcessBufferE(list<EMMediaDataBuffer*>* p_opBufferList)
{
	int64 vTime;

	EMMediaDataBuffer* opBuffer = p_opBufferList -> front();
	p_opBufferList -> pop_front();

	while(p_opBufferList -> size() != 0)
	{
		EMMediaDataBuffer* opBuffer2 = p_opBufferList -> front();
		p_opBufferList -> pop_front();
		opBuffer2 -> Recycle();
	}

	vTime = EMMediaUtility::Instance() -> TimeToFrames(opBuffer->GetRelativeTimeToStart(), EMMediaUtility::Instance() -> GetSystemAudioFormat());   //EMMediaTimer::Instance() -> VideoThenFrame();
	EMMediaClip* opClip = static_cast<EMWinVideoTransitionClipRepository*>(m_opVideoTransitionClipRepository) -> GetTransitionsInTimeslot(vTime, this);

	if(opClip != NULL)
	{
		opBuffer -> m_opDestination = static_cast<EMVideoTransitionClip*>(opClip);
	} 
	else
	{
		opBuffer -> m_opDestination = EMMediaTrack::GetDestination();
	}

	return opBuffer;
}

int32 EMMediaVideoTransitionTrack::CreateClip(int64 p_vAtFrame, string p_oName, EMFileInputDescriptor* p_opSource)
{
	EMVideoTransitionClip* opNewClip = EM_new EMVideoTransitionClip(this, NULL);

	if(!opNewClip -> InitCheckE())
	{
		delete opNewClip;
		return -1;
	}

	opNewClip -> SetStart(p_vAtFrame);
	AddListener(opNewClip);
	opNewClip -> SetDestination(EMMediaTrack::GetTrackDestination());

	m_opVideoTransitionClipRepository -> LockContainer();

	try
	{
		m_opVideoTransitionClipRepository -> Add(static_cast<EMMediaClip*>(opNewClip));
	}
	catch(...)
	{
		EMDebugger("ERROR! Caught an exception in EMMediaVideoTransitionTrack::CreateClip");
	}
	m_opVideoTransitionClipRepository -> UnlockContainer();	

	return ((EMBufferDestination*)opNewClip) -> GetID();
}


void EMMediaVideoTransitionTrack::SetDestination(EMBufferDestination* p_opDestination)
{
	EMBufferDestination::SetDestination(p_opDestination);
	EMMediaTrack::SetTrackDestination(p_opDestination);


	EMMediaClipRepository* opRep = EMMediaEngine::Instance() -> GetMediaProject() -> GetVideoTransitionClipRepository();
	opRep -> LockContainer();
	opRep -> Rewind();

	while(opRep -> Current() != NULL)
	{
		if(opRep -> Current() -> GetTrack() == this)
			static_cast<EMVideoTransitionClip*>(opRep -> Current()) -> SetDestination(p_opDestination);
		opRep -> Next();
	}

	opRep -> UnlockContainer();
}

EMBufferDestination* EMMediaVideoTransitionTrack::GetDestination() const
{
	return EMMediaTrack::GetTrackDestination(); //EMBufferDestination::GetDestination();
}

void EMMediaVideoTransitionTrack::SetTrackDestination(EMBufferDestination* p_opTrackDataReceiver)
{
	SetDestination(p_opTrackDataReceiver);
}




void EMMediaVideoTransitionTrack::SetParentID(uint32 p_vParentID)
{
	EMMediaTrackRepository* opUsed = EMMediaEngine::Instance() -> GetMediaProject() -> GetUsedTracks();
	EMMediaTrackRepository* opFree = EMMediaEngine::Instance() -> GetMediaProject() -> GetUnusedTracks();

	m_vParentID = p_vParentID;

	opUsed -> LockContainer();
	m_opParentTrack = opUsed -> Find(p_vParentID);
	opUsed -> UnlockContainer();
	if(m_opParentTrack == NULL)
	{
		opFree -> LockContainer();
		m_opParentTrack = opFree -> Find(p_vParentID);
		opFree -> UnlockContainer();
	}
}

uint32 EMMediaVideoTransitionTrack::GetParentID()
{
	return m_vParentID;
}

void EMMediaVideoTransitionTrack::Connect()
{
	if(m_opParentTrack != NULL)
	{	
		SetTrackDestination(static_cast<EMMediaEffectTrack*>(m_opParentTrack -> GetTrackDestination()) -> GetOutput());
		static_cast<EMMediaEffectTrack*>(m_opParentTrack -> GetTrackDestination()) -> SetOutput(this);
//		SetTrackDestination(m_opParentTrack -> GetTrackDestination());
//		m_opParentTrack -> SetTrackDestination(this);
	}
}

void EMMediaVideoTransitionTrack::Disconnect()
{
	if(m_opParentTrack != NULL)
	{
		static_cast<EMMediaEffectTrack*>(m_opParentTrack -> GetTrackDestination()) -> SetOutput(GetTrackDestination());

//		m_opParentTrack -> SetTrackDestination(GetTrackDestination());
	}
}

EMMediaBufferSource* EMMediaVideoTransitionTrack::GetSilenceGenerator() const
{
	if(m_opParentTrack != NULL)
		return m_opParentTrack -> GetSilenceGenerator();
	return NULL;
}

uint32 EMMediaVideoTransitionTrack::GetPriority() const
{
	if(m_opParentTrack != NULL)
		return m_opParentTrack -> GetPriority(); 

	return 1<<31;
}

void EMMediaVideoTransitionTrack::SetPriority(uint32 p_vPrio)
{
	if(m_opParentTrack != NULL)
		m_opParentTrack -> SetPriority(p_vPrio); 

}


EMMediaType EMMediaVideoTransitionTrack::GetType() const
{
	return EMMediaTrack::GetType();
}

int32 EMMediaVideoTransitionTrack::GetID() const
{
	return EMMediaTrack::GetID();
}

bool EMMediaVideoTransitionTrack::LoadData(EMProjectDataLoader* p_opLoader)
{
	if(p_opLoader -> LoadString() != string("EMMediaVideoTransitionTrack - Begin"))
	{
		EMDebugger("EMMediaVideoTransitionTrack::LoadData Error! Corrupt file?");
		return false;
	}

	EMMediaTrack::LoadData(p_opLoader);
	EMBufferDestination::LoadData(p_opLoader);

	m_vParentID = p_opLoader -> LoadInt32();
	m_opParentTrack = NULL;

	LoadClips(p_opLoader);

	if(p_opLoader -> LoadString() != string("EMMediaVideoTransitionTrack - End"))
	{
		EMDebugger("EMMediaVideoTransitionTrack::LoadData Error! Corrupt file?");
		return false;
	}

	return true;
}

bool EMMediaVideoTransitionTrack::SaveData(EMProjectDataSaver* p_opSaver)
{
	p_opSaver -> SaveString("EMMediaVideoTransitionTrack - Begin");

	EMMediaTrack::SaveData(p_opSaver);
	EMBufferDestination::SaveData(p_opSaver);

	p_opSaver -> SaveInt32(m_vParentID);

	SaveClips(p_opSaver);

	p_opSaver -> SaveString("EMMediaVideoTransitionTrack - End");

	Disconnect();

	return true;
}

bool EMMediaVideoTransitionTrack::LoadClips(EMProjectDataLoader* p_opLoader)
{
	int i;

	EMMediaClipRepository* opClips = EMMediaEngine::Instance() -> GetMediaProject() -> GetVideoTransitionClipRepository();

	if(p_opLoader -> LoadString() != string("SaveClips - Begin"))
	{
		EMDebugger("EMMediaVideoTransitionTrack::LoadClips Error! Corrupt file?");
		return false;
	}

	int32 vNumClips = p_opLoader -> LoadInt32();

	for(i = 0; i < vNumClips; ++i)
	{
		EMVideoTransitionClip* opClip = new EMVideoTransitionClip(this);
		opClip -> LoadData(p_opLoader);
		opClip -> InitCheckE();

		opClips -> Add(opClip);
	}

	if(p_opLoader -> LoadString() != string("SaveClips - End"))
	{
		EMDebugger("EMMediaVideoTransitionTrack::LoadClips Error(2)! Corrupt file?");
		return false;
	}

	return true;
}

bool EMMediaVideoTransitionTrack::SaveClips(EMProjectDataSaver* p_opSaver)
{
	EMMediaClipRepository* opClips = EMMediaEngine::Instance() -> GetMediaProject() -> GetVideoTransitionClipRepository();

	p_opSaver -> SaveString("SaveClips - Begin");
	p_opSaver -> SaveInt32(opClips -> CountClipsForTrack(this));

	opClips -> LockContainer();
	opClips -> Rewind();

	while(opClips -> Current() != NULL)
	{
		//Make sure we save only those clips that belong to this transition track, and that are not deleted!
		if(opClips -> Current() -> GetTrack() -> GetID() == GetID() && ! opClips -> Current() -> IsObjectDeleted())
			opClips -> Current() -> SaveData(p_opSaver);
		opClips -> Next();
	}

	opClips -> UnlockContainer();
	p_opSaver -> SaveString("SaveClips - End");

	return true;
}

bool EMMediaVideoTransitionTrack::RestoreAfterLoad()
{	
	SetParentID(m_vParentID);
	Connect();

	return true;
}
