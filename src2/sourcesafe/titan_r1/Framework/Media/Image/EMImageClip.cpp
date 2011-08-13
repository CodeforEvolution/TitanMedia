#include "EMImageClip.h"
#include "EMMediaEngine.h"
#include "EMMediaProject.h"
#include "EMMediaPool.h"
#include "EMFileInputDescriptor.h"
#include "EMImageFileInputDescriptor.h"
#include "EMMediaUtility.h"

#include "EMProjectDataLoader.h"
#include "EMProjectDataSaver.h"
 
EMImageClip::EMImageClip(EMMediaType p_eType, const char* p_vpFileName, int64 p_vStartFrame, EMMediaTrack* p_opBelongsToTrack)
	:	EMMediaClip(p_eType, p_vpFileName, p_vStartFrame, p_opBelongsToTrack)
{
	m_vStart = p_vStartFrame;
	m_vStop = p_vStartFrame + EMMediaUtility::Instance() -> TimeToFrames(20000000, EMMediaUtility::Instance() -> GetSystemAudioFormat());
}

EMImageClip::EMImageClip(EMMediaTrack* p_opOwner)
	:	EMMediaClip(p_opOwner)
{
}

EMImageClip::~EMImageClip()
{
} 

EMMediaClip* EMImageClip::CloneClip(int64 p_vStartFrame, EMMediaTrack* p_opBelongsToTrack)
{
	if(p_opBelongsToTrack == NULL) 
		EMDebugger("ERROR! Belongs to track is NULL");

	EMImageClip* opClipClone = EM_new EMImageClip(GetType(), m_oFileName.c_str(), p_vStartFrame, p_opBelongsToTrack);
	
	EMFileInputDescriptor* opDesc = EMMediaEngine::Instance() -> GetMediaProject() -> GetMediaPool() -> InstantiateEntryE(EMMediaEngine::Instance() -> GetMediaProject() -> GetMediaPool() -> SearchFor(m_oFileName));

	opClipClone -> SetMarkInLength(GetMarkInLength());
	opClipClone -> SetMarkOutLength(GetMarkOutLength());
	opClipClone -> SetStart(p_vStartFrame);

	opClipClone -> SetDescriptor(opDesc);
	opClipClone -> SetDestination(GetDestination());
	return opClipClone;
}

bool EMImageClip::InitCheckE()
{
	GetDescriptor() -> InitCheckE();
	return true;
}

int64 EMImageClip::GetActiveLength()
{
	return m_vStop - m_vStart; 
}

void EMImageClip::SetDeleted(bool p_vFlag)
{
	EMMediaClip::SetDeleted(p_vFlag);
}

bool EMImageClip::IsActiveNow(int64 p_vNow)
{
	return (p_vNow >= m_vStart) && (p_vNow < m_vStop) && (!EMMediaClip::IsMuted()) && !m_vIsDeleted;
}

EMMediaDataBuffer* EMImageClip::GetBuffer() const
{
	EMMediaDataBuffer* opMediaDataBuffer = m_opBelongsToTrack -> GetBuffer();

	if(opMediaDataBuffer != NULL)
	{
		opMediaDataBuffer -> m_opDestination = this -> GetDestination();
		opMediaDataBuffer ->m_opStartDestination = this -> GetDestination();
		return opMediaDataBuffer;
	}
	else 
		return NULL;
}

EMMediaDataBuffer* EMImageClip::GetBufferAt(int64 p_vTime, bool p_vSeeking)
{
	int64 vDummy = 0;
	int64 vTime = EMMediaUtility::Instance() -> FramesToTime(p_vTime, EMMediaUtility::Instance() -> GetSystemAudioFormat());

	EMMediaDataBuffer* opBuffer = GetBuffer();

	static_cast<EMImageFileInputDescriptor*>(GetDescriptor()) -> ReadFrames(opBuffer);
	opBuffer -> m_vSizeUsed = opBuffer -> m_vSizeAvailable;
	opBuffer -> m_vBelongsToUniqueID = this -> GetID();

	opBuffer -> SetRelativeTimeToStart(vTime);

	opBuffer -> m_vIsSilence = false;

	if(p_vSeeking)
		opBuffer -> m_vDiscon = true;

	return opBuffer;
}

int64 EMImageClip::GetStart()
{
	return m_vStart;
}

void EMImageClip::SetStart(int64 p_vTime)
{
	m_vStop = m_vStop - m_vStart + p_vTime;
	m_vStart = p_vTime;

}

int64 EMImageClip::GetMediaStart() const
{
	return 0;
}

int64 EMImageClip::GetStop() const
{
	return m_vStop;
}

void EMImageClip::SetStop(int64 p_vTime)
{
	m_vStop = p_vTime;
}

int64 EMImageClip::GetMediaStop() const
{
	return GetMediaLength();
}

int64 EMImageClip::GetMediaLength() const
{
	return 100000000000000000;
}


int64 EMImageClip::GetMarkInLength() const
{
	return m_vStart;
}

void EMImageClip::SetMarkInLength(int64 p_vLength)
{
	m_vStart = p_vLength;
}
	
int64 EMImageClip::GetMarkOutLength() const
{
	return GetMediaLength() - m_vStop; 
}

void EMImageClip::SetMarkOutLength(int64 p_vLength)
{
	SetStop(GetMediaLength() - p_vLength);
}

bool EMImageClip::SaveData(EMProjectDataSaver* p_opSaver)
{
	bool vRet = EMMediaClip::SaveData(p_opSaver);
	p_opSaver -> SaveInt64(m_vStart);
	p_opSaver -> SaveInt64(m_vStop);

	return vRet;
}

bool EMImageClip::LoadData(EMProjectDataLoader* p_opLoader)
{
	bool vRet = EMMediaClip::LoadData(p_opLoader);
	m_vStart = p_opLoader -> LoadInt64();
	m_vStop = p_opLoader -> LoadInt64();

	return vRet;
}

