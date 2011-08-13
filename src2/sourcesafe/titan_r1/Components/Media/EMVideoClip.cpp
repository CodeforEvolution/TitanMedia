#include "EMVideoClip.h"
#include "EMMediaEngine.h"
#include "EMMediaProject.h"
#include "EMMediaPool.h"
#include "EMFileInputDescriptor.h"
//#include "EMVideoFile.h"
//#include "EMVideoFileInputDescriptor.h"
#include "EMMediaUtility.h"
#include "EMVideoSynchronizer.h"
 
EMVideoClip::EMVideoClip(EMMediaType p_eType, const char* p_vpFileName, int64 p_vStartFrame, EMMediaTrack* p_opBelongsToTrack)
	:	EMMediaClip(p_eType, p_vpFileName, p_vStartFrame, p_opBelongsToTrack),
	m_opBackBuffer(NULL)
{
}

EMVideoClip::EMVideoClip(EMMediaTrack* p_opOwner)
	:	EMMediaClip(p_opOwner),
		m_opBackBuffer(NULL)
{
}

EMVideoClip::~EMVideoClip()
{
} 

EMMediaClip* EMVideoClip::CloneClip(int64 p_vStartFrame, EMMediaTrack* p_opBelongsToTrack)
{
	if(p_opBelongsToTrack == NULL) 
		EMDebugger("ERROR! Belongs to track is NULL");

	EMVideoClip* opClipClone = EM_new EMVideoClip(GetType(), m_oFileName.c_str(), p_vStartFrame, p_opBelongsToTrack);
	
	//TODO: Make non-native
	EMFileInputDescriptor* opDesc = EMMediaEngine::Instance() -> GetMediaProject() -> GetMediaPool() -> InstantiateEntryE(EMMediaEngine::Instance() -> GetMediaProject() -> GetMediaPool() -> SearchFor(m_oFileName));

	opClipClone -> SetMarkInLength(GetMarkInLength());
	opClipClone -> SetMarkOutLength(GetMarkOutLength());
	opClipClone -> SetStart(p_vStartFrame);

	opClipClone -> SetDescriptor(opDesc);
	opClipClone -> SetDestination(GetDestination());
	return opClipClone;
}

bool EMVideoClip::InitCheckE()
{

//	EMVideoFile * oVideo = static_cast<EMVideoFileInputDescriptor>(m_opDescriptor)->m_opVideoFile;
	EMMediaBufferSource::m_vHasBeenInitialized = true;
	return true;//EMMediaBufferSource::InitCheckDestinationsE(m_opDescriptor -> GetFormat());
}

int64 EMVideoClip::GetActiveLength()
{
	return EMMediaClip::GetActiveLength(); 
}

void EMVideoClip::SetDeleted(bool p_vFlag)
{
	EMMediaClip::SetDeleted(p_vFlag);
}

bool EMVideoClip::IsActiveNow(int64 p_vNow)
{
	EMMediaFormat *opFormat = m_opDescriptor->GetFormat();
	int64 vCensure = 0; //5*EMMediaUtility::Instance() -> TimeToFrames(1000000 / opFormat->m_vFrameRate, EMMediaUtility::Instance() -> GetSystemAudioFormat());

	return (p_vNow >= EMMediaClip::GetStart()) && (p_vNow < EMMediaClip::GetStop()) && (!EMMediaClip::IsMuted()) && !m_vIsDeleted;
//	return (p_vNow >= (EMMediaClip::GetStart() + EMMediaClip::GetMarkInLength())) && (p_vNow < (EMMediaClip::GetStop() - vCensure - EMMediaClip::GetMarkOutLength())) && (!EMMediaClip::IsMuted()) && !m_vIsDeleted;
}

EMMediaDataBuffer* EMVideoClip::GetBuffer() const
{

	if(! EMMediaBufferSource::m_vHasBeenInitialized)
		MessageBox(NULL, "ERROR! Tried to get video buffer from buffer source before it was initialized!", "MediaEngine error!", MB_OK);
	
	EMMediaDataBuffer* opMediaDataBuffer = m_opBelongsToTrack -> GetBuffer();
	if(opMediaDataBuffer != NULL)
	{
		opMediaDataBuffer -> m_opDestination = this -> GetDestination();
		opMediaDataBuffer ->m_opStartDestination = this -> GetDestination();
		return opMediaDataBuffer;
	}
	else return NULL;
}

EMMediaDataBuffer* EMVideoClip::GetBufferAt(int64 p_vTime, bool p_vSeeking)
{
	int64 vFramePosition = 0;
	int64 vDummy = EMMediaClip::GetStart();
	int64 vMediaStart = GetMediaStart();

	EMMediaDataBuffer* opBuffer = NULL;
	EMMediaDataBuffer* opBuffer2 = NULL;

	opBuffer = GetBuffer();

	if (m_opBackBuffer == NULL)
	{
		m_opBackBuffer = GetBuffer();
		opBuffer2 = m_opBackBuffer;
	} 
	else
	{
		opBuffer2 = NULL;
	}

	if(opBuffer == NULL || m_opBackBuffer == NULL)
	{
		m_opBelongsToTrack -> RecycleAllBuffers();

		opBuffer = GetBuffer();
		m_opBackBuffer = GetBuffer();
		opBuffer2 = m_opBackBuffer;
	}


	//This marked out code is for handling drop frames
/*	int64 vRealtimeRelativeClock = EMMediaUtility::Instance() -> TimeToFrames(EMVideoSynchronizer::Instance() -> GetSynchClockRelativeRealTime(), EMMediaUtility::Instance() -> GetSystemAudioFormat());
	int64 vRelativeOffsetTime = EMMediaUtility::Instance() -> TimeToFrames(EMVideoSynchronizer::Instance() -> GetRelativeOffsetTime(), EMMediaUtility::Instance() -> GetSystemAudioFormat());	

	if((vRealtimeRelativeClock + vRelativeOffsetTime) > vMediaStart && !p_vSeeking)
	{
		//vFramePosition = p_vTime - vMediaStart;
		vFramePosition =  vRealtimeRelativeClock - vMediaStart + vRelativeOffsetTime;
		p_vTime = vRealtimeRelativeClock + vRelativeOffsetTime;
	}
	else*/
	//if(p_vSeeking) //Seeking
	//{
		vFramePosition = p_vTime - vMediaStart;
	//}

	opBuffer -> RestoreBuffer();

	if(opBuffer2 != NULL)
		opBuffer2 -> RestoreBuffer();

	GetDescriptor() -> ReadFrames(&opBuffer, &opBuffer2, vFramePosition, vDummy, vDummy,p_vSeeking);
	opBuffer -> m_vSizeUsed = opBuffer -> m_vSizeAvailable;
	opBuffer -> m_vBelongsToUniqueID = this -> GetID();

/*	if(EMMediaUtility::Instance() -> FramesToTime(p_vTime, EMMediaUtility::Instance() -> GetSystemAudioFormat()) > 16000000)
	{
		int dummy = 0;
	}*/

	opBuffer -> SetRelativeTimeToStart(EMMediaUtility::Instance() -> FramesToTime(p_vTime, EMMediaUtility::Instance() -> GetSystemAudioFormat()));

	opBuffer -> m_vIsSilence = false;

	if(p_vSeeking)
		opBuffer -> m_vDiscon = true;

	return opBuffer;
}
