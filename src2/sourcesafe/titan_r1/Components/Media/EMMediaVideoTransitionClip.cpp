#include "EMMediaVideoTransitionClip.h"
#include "EMMediaTrack.h"
#include "EMMediaUtility.h"
#include "EMPluginRepository.h"

EMVideoTransitionClip::EMVideoTransitionClip()
	: EMMediaClip(NULL),
	EMBufferDestination(EM_TYPE_ANY_VIDEO, EM_TRANSITION,"EMVideoTransitionClip"),
	m_vSwap(false)
{
	SetType(EM_TYPE_TRANSITION);

	EMPluginRepository::Instance() -> LockContainer();
	EMPluginRepository::Instance() -> Rewind();

	while(EMPluginRepository::Instance() -> Current() != NULL && EMPluginRepository::Instance() -> Current() -> GetName() != "EM Fader")
		EMPluginRepository::Instance() -> Next();

	if(EMPluginRepository::Instance() -> Current() != NULL)
	{
		m_opTransEntry = EMPluginRepository::Instance() -> Current();
		m_opTrans = static_cast<EMVideoTransition*>(EMPluginRepository::Instance() -> Current() -> InstantiateNativePlugin());
	}
	else
	{	
		m_opTrans = NULL;
	}

	EMPluginRepository::Instance() -> UnlockContainer();
}

EMVideoTransitionClip::EMVideoTransitionClip(EMMediaTrack* p_opOwner)
	: EMMediaClip(p_opOwner),
	EMBufferDestination(EM_TYPE_ANY_VIDEO, EM_TRANSITION,"EMVideoTransitionClip"),
	m_opTransEntry(NULL),
	m_vSwap(false)
{
	SetType(EM_TYPE_TRANSITION);

	EMMediaClip::m_vID = EMBufferDestination::GetID();

	EMPluginRepository::Instance() -> LockContainer();
	EMPluginRepository::Instance() -> Rewind();

	while(EMPluginRepository::Instance() -> Current() != NULL && EMPluginRepository::Instance() -> Current() -> GetName() != "EM Fader")
		EMPluginRepository::Instance() -> Next();

	if(EMPluginRepository::Instance() -> Current() != NULL)
	{
		m_opTransEntry = EMPluginRepository::Instance() -> Current();
		m_opTrans = static_cast<EMVideoTransition*>(EMPluginRepository::Instance() -> Current() -> InstantiateNativePlugin());
	}
	else
	{	
		m_opTrans = NULL;
	}

	EMPluginRepository::Instance() -> UnlockContainer();
}


EMVideoTransitionClip::EMVideoTransitionClip(EMMediaTrack* p_opOwner, EMPluginEntry* p_opTransEntry)
	: EMMediaClip(p_opOwner),
	EMBufferDestination(EM_TYPE_ANY_VIDEO, EM_TRANSITION,"EMVideoTransitionClip"),
	m_opTransEntry(p_opTransEntry),
	m_vSwap(false)
{
	SetType(EM_TYPE_TRANSITION);

	EMMediaClip::m_vID = EMBufferDestination::GetID();

	m_opTrans = NULL;

	if (p_opTransEntry != NULL)
	{
		m_opTrans = static_cast<EMVideoTransition*>(p_opTransEntry -> InstantiateNativePlugin());
	} 
	else
	{
		EMPluginRepository::Instance() -> LockContainer();
		EMPluginRepository::Instance() -> Rewind();

		EMPluginEntry* opPlugEntry = EMPluginRepository::Instance() -> Current();
		while(opPlugEntry != NULL && opPlugEntry -> GetName() != "EM Fader")
		{
			EMPluginRepository::Instance() -> Next();
			opPlugEntry = EMPluginRepository::Instance() -> Current();
		}

		if(EMPluginRepository::Instance() -> Current() != NULL)
		{
			m_opTransEntry = EMPluginRepository::Instance() -> Current();
			m_opTrans = static_cast<EMVideoTransition*>(EMPluginRepository::Instance() -> Current() -> InstantiateNativePlugin());
		}
		EMPluginRepository::Instance() -> UnlockContainer();
	}
}

EMVideoTransitionClip::~EMVideoTransitionClip()
{
	if (m_opTrans != NULL)
		delete m_opTrans;
}

EMMediaClip* EMVideoTransitionClip::CloneClip(int64 p_vStartFrame, EMMediaTrack* p_opBelongsToTrack)
{
	EMVideoTransitionClip* opClip = new EMVideoTransitionClip(p_opBelongsToTrack, m_opTransEntry);

	opClip -> m_opTrans -> CopyTransParams(m_opTrans);

	opClip -> SetDestination(EMMediaClip::GetDestination());

	opClip -> SetStart(p_vStartFrame);
	opClip -> SetStop(p_vStartFrame + GetActiveLength());

	return opClip;
}
	
bool EMVideoTransitionClip::InitCheckE()
{

	if (m_opTrans != NULL)
		return m_opTrans->PrepareToPlayE();
	return true;
}

bool EMVideoTransitionClip::IsActiveNow(int64 p_vNow)
{
	return (p_vNow>=m_vTransStart) && (p_vNow<=m_vTransStop) && !m_vIsDeleted;
}

int64 EMVideoTransitionClip::GetActiveLength()
{
	return m_vTransStop - m_vTransStart; //GetStop() - GetStart();
}

void EMVideoTransitionClip::SetDeleted(bool p_vFlag)
{
	EMMediaClip::SetDeleted(p_vFlag); 	
}


EMMediaDataBuffer* EMVideoTransitionClip::GetBuffer() const
{
	EMDebugger("MVideoTransitionClip::GetBuffer(): Go Away!!");
	return NULL;
}


bool EMVideoTransitionClip::PrepareToPlayE()
{
	return true;
}

EMMediaDataBuffer* EMVideoTransitionClip::ProcessBufferE(list<EMMediaDataBuffer*>* p_opBufferList)
{
	EMMediaDataBuffer* opBuffer1;
	EMMediaDataBuffer* opBuffer2;


	if(m_opBelongsToTrack -> GetMuteState() == EM_MUTE_OFF)
	{
		m_opTrans -> SetDestination(m_opBelongsToTrack -> GetDestination());
		return m_opTrans -> ProcessBufferE(p_opBufferList);
	}

	opBuffer1 = p_opBufferList -> front();
	p_opBufferList -> pop_front();

	while(p_opBufferList -> size() != 0)
	{
		opBuffer2 = p_opBufferList -> front();
		p_opBufferList -> pop_front();

		if(opBuffer2 != NULL && opBuffer2 != opBuffer1)
			opBuffer2 -> Recycle();
	}

	opBuffer1 -> m_opDestination = m_opBelongsToTrack -> GetDestination();

	return opBuffer1;
}


void EMVideoTransitionClip::SetTransition(EMPluginEntry* p_opTransEntry)
{
	if(m_opTrans != NULL && m_opTrans ->GetEntryID() == p_opTransEntry -> GetID())
		return;

	if(m_opTrans != NULL)
	{
		delete m_opTrans;
	}

	m_opTransEntry = p_opTransEntry;
	m_opTrans = static_cast<EMWinDirectXVideoTransition*>(p_opTransEntry -> InstantiateNativePlugin());

	m_opTrans ->SetStartTime(EMMediaUtility::Instance() -> FramesToTime(m_vTransStart, EMMediaUtility::Instance() -> GetSystemAudioFormat()));
	m_opTrans ->SetStopTime(EMMediaUtility::Instance() -> FramesToTime(m_vTransStop, EMMediaUtility::Instance() -> GetSystemAudioFormat()));
//	m_opTrans -> SwapInputs(m_vSwap);
}

EMPlugin* EMVideoTransitionClip::GetTransition()
{
	return m_opTrans;
}

int64 EMVideoTransitionClip::GetStart()
{
	return m_vTransStart;
}
void EMVideoTransitionClip::SetStart(int64 p_vTime)
{
	int64 vLength = m_vTransStop - m_vTransStart; //m_opTrans -> GetStopTime() - m_opTrans -> GetStartTime();

	if (m_opTrans != NULL)
	{
		int64 vDUmmy = EMMediaUtility::Instance() -> FramesToTime(p_vTime, EMMediaUtility::Instance() -> GetSystemAudioFormat());
		m_opTrans -> SetStartTime(EMMediaUtility::Instance() -> FramesToTime(p_vTime, EMMediaUtility::Instance() -> GetSystemAudioFormat()));
		m_opTrans -> SetStopTime(EMMediaUtility::Instance() -> FramesToTime(p_vTime + vLength, EMMediaUtility::Instance() -> GetSystemAudioFormat()));
	}
	m_vTransStart = p_vTime;
	m_vTransStop = p_vTime + vLength;
}

int64 EMVideoTransitionClip::GetMediaStart() const
{
	return 0;
}

int64 EMVideoTransitionClip::GetStop() const
{
	return m_vTransStop;
}

void EMVideoTransitionClip::SetStop(int64 p_vTime)
{
	if (m_opTrans != NULL)
	{
		int64 vDummy = EMMediaUtility::Instance() -> FramesToTime(p_vTime, EMMediaUtility::Instance() -> GetSystemAudioFormat());
		m_opTrans -> SetStopTime(vDummy);
	}

	m_vTransStop = p_vTime;
}

int64 EMVideoTransitionClip::GetMediaStop() const
{
	return GetMediaLength();
}

int64 EMVideoTransitionClip::GetMediaLength() const
{
	return 100000000000000000;
}

void EMVideoTransitionClip::MoveClip(int64 p_vNewTime)
{
	SetStart(p_vNewTime);
}

void EMVideoTransitionClip::SetDestination(EMBufferDestination* p_opBuffer)
{
	EMMediaClip::SetDestination(p_opBuffer);
	EMBufferDestination::SetDestination(p_opBuffer);
}

int64 EMVideoTransitionClip::GetMarkInLength() const
{
//	int32 vDummy = EMMediaClip::GetMarkInLength();
//	eo << "EMVideoTransitionClip::GetMarkInLength(" << vDummy <<")"<<ef;

	return m_vTransStart; //EMMediaClip::GetMarkInLength();
}

void EMVideoTransitionClip::SetMarkInLength(int64 p_vLength)
{
	m_vTransStart = p_vLength;
	m_opTrans -> SetStartTime(EMMediaUtility::Instance() -> FramesToTime(GetStart(), EMMediaUtility::Instance() -> GetSystemAudioFormat()));
}
	
int64 EMVideoTransitionClip::GetMarkOutLength() const
{
	return GetMediaLength() - m_vTransStop; 
}

void EMVideoTransitionClip::SetMarkOutLength(int64 p_vLength)
{
	SetStop(GetMediaLength() - p_vLength);
}



EMBufferDestination* EMVideoTransitionClip::GetDestination()
{
	return m_opBelongsToTrack -> GetDestination();
}

void EMVideoTransitionClip::SwapInputs(bool p_vSwap)
{
	m_vSwap = p_vSwap;
	if(m_opTrans != NULL)
		m_opTrans -> SwapInputs(p_vSwap);
}	

bool EMVideoTransitionClip::SaveData(EMProjectDataSaver* p_opSaver)
{
	p_opSaver -> SaveString("EMVideoTransitionClip - begin");

	EMMediaClip::SaveData(p_opSaver);
	EMBufferDestination::SaveData(p_opSaver);

	p_opSaver -> SaveInt64(m_vTransStart);
	p_opSaver -> SaveInt64(m_vTransStop);
	p_opSaver -> SaveBool(m_vSwap);

	p_opSaver -> SaveString((m_opTransEntry -> GetUniqueID()).c_str());
//	m_opTrans -> SaveData(p_opSaver);
	m_opTrans -> SaveSetting(p_opSaver);

	p_opSaver -> SaveString("EMVideoTransitionClip - end");

	return true;
}

bool EMVideoTransitionClip::LoadData(EMProjectDataLoader* p_opLoader)
{
	string oTest = p_opLoader -> LoadString();

	if(oTest != "EMVideoTransitionClip - begin")
	{
		EMDebugger("EMVideoTransitionClip::LoadData - Error: Mismatched data structure!");
	}

	EMMediaClip::LoadData(p_opLoader);
	EMBufferDestination::LoadData(p_opLoader);

	m_vTransStart = p_opLoader -> LoadInt64();
	m_vTransStop = p_opLoader -> LoadInt64();
	m_vSwap = p_opLoader -> LoadBool();

	string oUniqueID = p_opLoader -> LoadString();


	EMPluginRepository* opClips = EMPluginRepository::Instance();

	opClips -> LockContainer();
	opClips -> Rewind();

	while((opClips -> Current() != NULL) && (opClips -> Current() -> GetUniqueID() != oUniqueID))
		opClips -> Next();

	m_opTransEntry = opClips -> Current();

	opClips -> UnlockContainer();


	if(m_opTransEntry == NULL)
		EMDebugger("EMVideoTransitionClip::LoadData - Error: Couldn't find transition!");

	if(m_opTrans != NULL)
		delete m_opTrans;

	m_opTrans = static_cast<EMWinDirectXVideoTransition*>(m_opTransEntry -> InstantiateNativePlugin());
//	m_opTrans -> LoadData(p_opLoader);
	m_opTrans -> LoadSetting(p_opLoader);

	oTest = p_opLoader -> LoadString();
	if(oTest != "EMVideoTransitionClip - end")
	{
		EMDebugger("EMVideoTransitionClip::LoadData - Error: Mismatched data structure!");
	}

	return true;
}