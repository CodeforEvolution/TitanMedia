#include "EMMediaEngine.h"
#include "EMAudioClip.h"
#include "EMMediaProject.h"
#include "EMMediaPool.h"
#include "EMFileInputDescriptor.h"
#include "EMMediaEffectTrack.h"
#include "EMMediaEffectTrackRepository.h"

EMAudioClip::EMAudioClip(EMMediaType p_eType, const char* p_vpFileName, int64 p_vStartFrame, EMMediaTrack* p_opBelongsToTrack)
	:	EMMediaClip(p_eType, p_vpFileName, p_vStartFrame, p_opBelongsToTrack)
{
} 

EMAudioClip::EMAudioClip(EMMediaTrack* p_opOwner) 
	:	EMMediaClip(p_opOwner)
{
}

EMAudioClip::~EMAudioClip()
{  
//	EMMediaEffectTrackRepository::Instance() -> LockContainer();
//	EMMediaEffectTrackRepository::Instance() -> Remove(m_opEffectTrack);
//	EMMediaEffectTrackRepository::Instance() -> UnlockContainer();
//	delete m_opEffectTrack;
}

EMMediaClip* EMAudioClip::CloneClip(int64 p_vStartFrame, EMMediaTrack* p_opBelongsToTrack)
{
	if(p_opBelongsToTrack == NULL)
		EMDebugger("ERROR! Belongs to track is NULL");

	EMAudioClip* opClipClone = EM_new EMAudioClip(GetType(), m_oFileName.c_str(), p_vStartFrame, p_opBelongsToTrack);
	
	//TODO: Make non-native
	EMFileInputDescriptor* opDesc = EMMediaEngine::Instance() -> GetMediaProject() -> GetMediaPool() -> InstantiateEntryE(EMMediaEngine::Instance() -> GetMediaProject() -> GetMediaPool() -> SearchFor(m_oFileName));

	opClipClone -> SetMarkInLength(GetMarkInLength());
	opClipClone -> SetMarkOutLength(GetMarkOutLength());
	opClipClone -> SetStart(p_vStartFrame);

	opClipClone -> SetDescriptor(opDesc);
	opClipClone -> SetDestination(GetDestination());
	opClipClone -> SetTrack(p_opBelongsToTrack); //Put this here to make sure that the audio signal meter's OnStart/OnStop is called (so the signal meter's isPaused flag is updated).
	return opClipClone;
}

bool EMAudioClip::InitCheckE()
{
	if(! EMMediaBufferSource::InitCheckE(m_opDescriptor -> GetFormat()))
		return false;
	EMMediaBufferSource::SetBufferFormat(m_opDescriptor -> GetFormat());
	return true;
}

int64 EMAudioClip::GetActiveLength()
{
	return EMMediaClip::GetActiveLength();
}

void EMAudioClip::SetDeleted(bool p_vFlag)
{
	EMMediaClip::SetDeleted(p_vFlag);
}

void EMAudioClip::SetTrack(EMMediaTrack* p_opBelongsToTrack)
{
	EMMediaClip::SetTrack(p_opBelongsToTrack);

//	if(HasEffectTrack())
//	{
//		SetDestination(m_opEffectTrack);
//		m_opEffectTrack -> SetOutput(GetDestination());
//	}
}


void EMAudioClip::CloseFile()
{
	EMFileInputDescriptor* opDesc = GetDescriptor();

	opDesc -> Close();
}

void EMAudioClip::OpenFile()
{
	EMFileInputDescriptor* opDesc = GetDescriptor();

	opDesc -> Open();
}
