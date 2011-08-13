#include "EMMediaMIDIEventPosition.h"

#include "EMMediaMIDIEvent.h"
#include "EMMIDIClip.h"
#include "EMMediaMIDITrack.h"
#include "EMProjectDataLoader.h"
#include "EMProjectDataSaver.h"
#include "EMMediaUtility.h"
#include "EMMediaEngine.h"
#include "EMMediaProject.h"
#include "EMMediaClipRepository.h"
#include "EMRealtimeMIDIOutputDescriptor.h"

EMMediaMIDIEventPosition::EMMediaMIDIEventPosition(EMMediaMIDIEvent* p_opMIDIEvent)
	:	m_opEvent(p_opMIDIEvent),
		m_vStart(0),
		m_vMIDIChannel(0),
		m_opDestination(NULL),
		m_vIsMuted(false),
		m_opParentClip(NULL)
{
}

EMMediaMIDIEventPosition::EMMediaMIDIEventPosition(const EMMediaMIDIEventPosition& p_oPosition)
	:	m_opEvent(p_oPosition.m_opEvent),
		m_vStart(p_oPosition.m_vStart),
		m_vMIDIChannel(p_oPosition.m_vMIDIChannel),
		m_opDestination(p_oPosition.m_opDestination),
		m_vIsMuted(p_oPosition.m_vIsMuted),
		m_opParentClip(p_oPosition.m_opParentClip)
{
}

EMMediaMIDIEventPosition::~EMMediaMIDIEventPosition()
{
} 
 
int64 EMMediaMIDIEventPosition::GetClipStart() const
{
	return EMMediaUtility::Instance() -> FramesToTime(m_opParentClip -> GetStart(), EMMediaUtility::Instance() -> GetSystemAudioFormat());
}

void EMMediaMIDIEventPosition::SetClipStart(int64 p_vStart)
{
	m_opParentClip -> SetStart(EMMediaUtility::Instance() -> TimeToFrames(p_vStart, EMMediaUtility::Instance() -> GetSystemAudioFormat()));
}

int32 EMMediaMIDIEventPosition::GetClipID() const
{
	return m_opParentClip -> GetID();
}

void EMMediaMIDIEventPosition::SetClip(void* p_opClip)
{
	m_opParentClip = static_cast<EMMIDIClip*>(p_opClip);
}

void* EMMediaMIDIEventPosition::GetClip() const
{
	return static_cast<void*>(m_opParentClip);
}

void EMMediaMIDIEventPosition::SetStart(int64 p_vNativeReferenceTime)
{
	m_vStart = p_vNativeReferenceTime;
}

int64 EMMediaMIDIEventPosition::GetStart() const
{
	return m_vStart;
}

void EMMediaMIDIEventPosition::SetChannel(uint32 p_vMIDIChannel)
{
	m_vMIDIChannel = p_vMIDIChannel;
}

uint32 EMMediaMIDIEventPosition::GetChannel() const
{
	if(m_opParentClip == NULL)
		return m_vMIDIChannel;
	return static_cast<EMMediaMIDITrack*>(m_opParentClip -> GetTrack()) -> GetMIDIChannel(); //m_vMIDIChannel;
}

void EMMediaMIDIEventPosition::SetDestination(EMRealtimeMIDIOutputDescriptor* p_opDestination)
{
	m_opDestination = p_opDestination;
}

EMRealtimeMIDIOutputDescriptor* EMMediaMIDIEventPosition::GetDestination() const
{
//	if(m_opParentClip != NULL)
//		return static_cast<EMRealtimeMIDIOutputDescriptor*>(m_opParentClip -> GetDestination()); //m_opDestination;
	return m_opDestination;
}

void EMMediaMIDIEventPosition::SetMuted(bool p_vIsMuted)
{
	m_vIsMuted = p_vIsMuted;
}

bool EMMediaMIDIEventPosition::IsMuted() const
{
	return m_vIsMuted;
}

bool EMMediaMIDIEventPosition::SaveData(EMProjectDataSaver* p_opSaver)
{
	p_opSaver -> SaveString("EMT");
	p_opSaver -> SaveInt64(m_vStart);
	p_opSaver -> SaveUInt32(m_vMIDIChannel);
	p_opSaver -> SaveBool(m_vIsMuted);
	p_opSaver -> SaveInt32(m_opParentClip -> GetID());
	return true;
}

bool EMMediaMIDIEventPosition::LoadData(EMProjectDataLoader* p_opLoader)
{
	if(string(p_opLoader -> LoadString()) != string("EMT"))
		return false;
	m_vStart = p_opLoader -> LoadInt64();
	m_vMIDIChannel = p_opLoader -> LoadUInt32();
	m_vIsMuted = p_opLoader -> LoadBool();
	int32 vClipID = p_opLoader -> LoadInt32();
	EMMediaClipRepository* opClips = EMMediaEngine::Instance() -> GetMediaProject() -> GetMIDIClipRepository();
	EMMIDIClip* opClip = static_cast<EMMIDIClip*>(opClips -> Find(vClipID));
	if(opClip == NULL)
	{
		//Rescue!
		opClip = static_cast<EMMIDIClip*>(opClips -> First(EM_TYPE_MIDI));
		if(opClip == NULL)
		{
			EMDebugger("ERROR! Failed to find clip with that ID! You shouldn't (can't) load MIDI event position data before the MIDI clips have been loaded!");
			return false;
		}
	}
	m_opParentClip = opClip;
	m_opDestination = static_cast<EMRealtimeMIDIOutputDescriptor*>(m_opParentClip -> GetTrack() -> GetDestination());
	return true;
}
