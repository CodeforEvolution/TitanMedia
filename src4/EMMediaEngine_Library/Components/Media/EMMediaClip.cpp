#include "EMGlobals.h"

#include "EMMediaClip.h"

#include "EMFileInputDescriptor.h"
#include "EMMediaTrack.h"
#include "EMMediaEffectTrack.h"
#include "EMMediaDataBuffer.h"
#include "EMMediaEngine.h"
#include "EMMediaProject.h"
#include "EMMediaTrackRepository.h"
#include "EMMediaPool.h"
#include "EMMediaUtility.h"
#include "EMProjectDataLoader.h"
#include "EMProjectDataSaver.h"
#include "EMMediaEffectTrack.h"
#include "EMMediaEffectTrackRepository.h"
#include "DirectoryIDs.h" 
#include "EMMediaSignalMeter.h"
 
EMMediaClip::EMMediaClip(EMMediaType p_eType, const char* p_vpFileName, int64 p_vStartFrame, EMMediaTrack* p_opBelongsToTrack)
	:	EMMediaBufferSource(p_eType),
		m_vActiveStart(0),
		m_vMarkInLength(0),
		m_vMarkOutLength(0),
		m_oFileName(p_vpFileName),
		m_oName(p_vpFileName),
		m_opDescriptor(NULL),
		m_opBelongsToTrack(p_opBelongsToTrack),
		m_vIsDeleted(false),
		/*m_opEffectTrack(NULL),*/
		m_vFxID(-1)
{
}

EMMediaClip::EMMediaClip(EMMediaTrack* p_opOwner)
	:	EMMediaBufferSource(),
		m_vActiveStart(0),
		m_vMarkInLength(0),
		m_vMarkOutLength(0),
		m_oFileName(""),
		m_oName(""),
		m_opDescriptor(NULL),
		m_opBelongsToTrack(p_opOwner),
		m_vIsDeleted(false),
		/*m_opEffectTrack(NULL),*/
		m_vFxID(-1)
{
}

EMMediaClip::~EMMediaClip()
{
	if(m_opBelongsToTrack != NULL)
		m_opBelongsToTrack -> RemoveListener(this);
	if(m_opDescriptor != NULL)
		delete m_opDescriptor;
}

void EMMediaClip::OnItemAdd()
{
	EMMediaEngine::Instance() -> GetMediaProject() -> SetDirty(true);
}

void EMMediaClip::OnItemRemove()
{
	EMMediaEngine::Instance() -> GetMediaProject() -> SetDirty(true);
}

EMFileInputDescriptor* EMMediaClip::GetDescriptor() const
{
	return m_opDescriptor;
}

string EMMediaClip::GetFileName() const
{
	return m_oFileName;
}

EMMediaTrack* EMMediaClip::GetTrack() const
{
	return m_opBelongsToTrack;
}

/*bool EMMediaClip::InitCheckE()
{
	if(! EMMediaBufferSource::InitCheckE(m_opDescriptor -> GetFormat()))
		return false;
	SetBufferFormat(m_opDescriptor -> GetFormat());
	return true;
}
*/

EMMediaDataBuffer* EMMediaClip::GetBuffer() const
{
	if((GetType() & EM_TYPE_ANY_VIDEO) > 0)
	{
		return m_opBelongsToTrack->GetBuffer();
	}

	return EMMediaBufferSource::GetBuffer();
}

void EMMediaClip::RecycleAllBuffers() const
{
	if((GetType() & EM_TYPE_ANY_VIDEO) > 0)
	{
		m_opBelongsToTrack->RecycleAllBuffers();
	}
}


void EMMediaClip::SetDescriptor(EMFileInputDescriptor* p_opDescriptor) 
{
	m_opDescriptor = p_opDescriptor;
}

void EMMediaClip::SetTrack(EMMediaTrack* p_opBelongsToTrack)
{
	if(m_opBelongsToTrack != NULL)
		m_opBelongsToTrack -> RemoveListener(this);

	if(EMMediaEngine::Instance() -> GetMediaProject() -> IsPlaying() && ((GetType() & EM_TYPE_ANY_AUDIO) > 0))
	{
		//Semi-hack, to update the signal meters paused/notpaused flag.. 
		m_opBelongsToTrack -> GetSignalMeter() -> OnStop();
		m_opBelongsToTrack -> GetSignalMeter() -> OnStart();

//		if(p_opBelongsToTrack != m_opBelongsToTrack)
//		{
			p_opBelongsToTrack -> GetSignalMeter() -> OnStop();
			p_opBelongsToTrack -> GetSignalMeter() -> OnStart();
//		}
	}

	m_opBelongsToTrack = p_opBelongsToTrack;
	m_opBelongsToTrack -> AddListener(this);

	//Make sure that the (especially) mute- and delete status follow the clip
	//as we move it to another track.
	SetDeleted(m_opBelongsToTrack -> IsObjectDeleted());
	
	if((GetType() & EM_TYPE_ANY_GFX) > 0)
	{
		p_opBelongsToTrack -> InitializeTrackBuffers();
		p_opBelongsToTrack -> SetBufferPriority(p_opBelongsToTrack -> GetPriority());
	}

	;//cout_commented_out_4_release << "Inside clip's SetTrack: Clip " << GetID() << " now have " << m_opBelongsToTrack -> GetClipDataDestination() -> GetID() << " as destination!" << endl;
	if(HasEffectTrack())
		reinterpret_cast<EMMediaEffectTrack*>(GetDestination()) -> SetOutput(m_opBelongsToTrack -> GetClipDataDestination());
	else
		SetDestination(m_opBelongsToTrack -> GetClipDataDestination());
}

bool EMMediaClip::IsMuted() const
{
	bool vClipIsMuted = (m_opBelongsToTrack -> GetMuteState() == EM_MUTE_ON || m_opBelongsToTrack -> GetMuteState() == EM_MUTE_ON_BY_SOLO);
	return vClipIsMuted;
}

string EMMediaClip::GetName() const
{
	return m_oName;
}

void EMMediaClip::SetName(string p_oName)
{
	m_oName = p_oName;
}

void EMMediaClip::SetDeleted(bool p_vFlag)
{
	m_vIsDeleted = p_vFlag;
}

bool EMMediaClip::IsObjectDeleted() const
{
	return m_vIsDeleted;
}

int64 EMMediaClip::GetMediaStart() const
{
	return m_vActiveStart - m_vMarkInLength;
}

int64 EMMediaClip::GetStart()
{
	return m_vActiveStart; 
}

void EMMediaClip::SetStart(int64 p_vTime)
{
	m_vActiveStart = p_vTime;
}

int64 EMMediaClip::GetMarkInLength() const
{
	return m_vMarkInLength;
}

void EMMediaClip::SetMarkInLength(int64 p_vLength)
{
	m_vMarkInLength = p_vLength;
}

int64 EMMediaClip::GetMarkOutLength() const
{
	return m_vMarkOutLength;
}

void EMMediaClip::SetMarkOutLength(int64 p_vLength)
{
	m_vMarkOutLength = p_vLength;
}

int64 EMMediaClip::GetStop()
{
	return GetStart() + GetActiveLength();
}

int64 EMMediaClip::GetActiveLength()
{
	if(m_opDescriptor != NULL)
		return m_opDescriptor -> GetNumFrames() - (m_vMarkInLength + m_vMarkOutLength);
	return 0;
} 

int64 EMMediaClip::GetMediaLength() const
{
	if(m_opDescriptor != NULL)
		return m_opDescriptor -> GetNumFrames();
	return 0;
}

bool EMMediaClip::IsActiveNow(int64 p_vNow)
{
	return (p_vNow>=EMMediaClip::GetStart()) && (p_vNow<EMMediaClip::GetStop()) && (!EMMediaClip::IsMuted());
}

bool EMMediaClip::MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage)
{
	;//cout_commented_out_4_release << "Clip " << GetID() << " found out (by its track (" << m_opBelongsToTrack -> GetID() << ") that the destination has changed to " << m_opBelongsToTrack -> GetClipDataDestination() -> GetID() << endl;
	
	if(p_vMessage == EM_MESSAGE_TRACK_DEST_CHANGE)
		if(HasEffectTrack())
			dynamic_cast<EMMediaEffectTrack*>(GetDestination()) -> SetOutput(m_opBelongsToTrack -> GetClipDataDestination());
		else
			SetDestination(m_opBelongsToTrack -> GetClipDataDestination());
/*	else if(p_vMessage == EM_MESSAGE_CLOSE_FILE)
		CloseFile();
	else if(p_vMessage == EM_MESSAGE_OPEN_FILE)
		OpenFile();*/

	return true;
}

bool EMMediaClip::SaveData(EMProjectDataSaver* p_opSaver)
{
	EMMediaBufferSource::SaveData(p_opSaver);
	p_opSaver -> SaveUInt64(static_cast<uint64>(m_vActiveStart));
	p_opSaver -> SaveUInt64(static_cast<uint64>(m_vMarkInLength));
	p_opSaver -> SaveUInt64(static_cast<uint64>(m_vMarkOutLength));
	p_opSaver -> SaveString(m_oName.c_str());

	if((GetType() & EM_TYPE_ANY_AUDIO) > 0) //If it's an audio file, we can use a project-relative path when loading the project back in
		p_opSaver -> SaveString(EMMediaUtility::Instance() -> GetFileName(m_oFileName.c_str()));
	else if((GetType() & EM_TYPE_ANY_VIDEO) > 0) //But if it's a video file, we can't (since those files aren't copied in to the project!)
		p_opSaver -> SaveString(m_oFileName.c_str());
	else if((GetType() & EM_TYPE_ANY_IMAGE) > 0)
		p_opSaver -> SaveString(m_oFileName.c_str());
	else
		; //Do nothing if it's other than audio or video! (eg, for MIDI we don't have a filename to save!)

	p_opSaver -> SaveUInt32(m_vFxID);

	if(m_opDescriptor != NULL)
	{
		p_opSaver -> SaveBool(true);
		p_opSaver -> SaveUInt32(static_cast<uint32>(m_opDescriptor -> GetMediaPoolEntryID()));
	}
	else
	{
		p_opSaver -> SaveBool(false);
	}
	
	//TODO: Don't forget to save descriptor information... Or at least don't forget to create one when loading!!
	return true;
}

bool EMMediaClip::LoadData(EMProjectDataLoader* p_opLoader)
{
	EMMediaBufferSource::LoadData(p_opLoader);

	m_vActiveStart = static_cast<int64>(p_opLoader -> LoadUInt64());
	m_vMarkInLength = static_cast<int64>(p_opLoader -> LoadUInt64());
	m_vMarkOutLength = static_cast<int64>(p_opLoader -> LoadUInt64());
	m_oName = p_opLoader -> LoadString();

	if((GetType() & EM_TYPE_ANY_AUDIO) > 0)
		m_oFileName = EMMediaUtility::Instance() -> ParseFullPath(p_opLoader -> LoadString(), DIRECTORY_ID_AUDIO_DATA_USED);
	else if((GetType() & EM_TYPE_ANY_VIDEO) > 0)
		m_oFileName = p_opLoader -> LoadString();
	else if((GetType() & EM_TYPE_ANY_IMAGE) > 0)
		m_oFileName = p_opLoader -> LoadString();
	else
		; //Do nothing, coz for MIDI we didn't save the filename, so we have nothing to load!

	m_vFxID = static_cast<int32>(p_opLoader -> LoadUInt32());;


	bool vHasEntryID = p_opLoader -> LoadBool();

	if(vHasEntryID)
	{
		int32 vEntryID = static_cast<int32>(p_opLoader -> LoadUInt32());
		EMFileInputDescriptor* opDescriptor = EMMediaEngine::Instance() -> GetMediaProject() -> GetMediaPool() -> InstantiateEntryE(vEntryID);
		if(opDescriptor == NULL)
			EMDebugger("ERROR! Clip could not instantiate the pool-entry described in the project file!");
		m_opDescriptor = opDescriptor;
	}
	else
	{
		m_opDescriptor = NULL;
	}

	if(HasEffectTrack())
	{
		EMMediaEffectTrack* opFXTrack = EMMediaEffectTrackRepository::Instance() -> Find(m_vFxID);
		if(opFXTrack != NULL)
		{
			opFXTrack -> SetOutput(GetDestination());
			SetDestination(opFXTrack);
		}
	}
	
//	int32 vTrackID = static_cast<int32>(p_opLoader -> LoadUInt32());
	
//	EMMediaTrack* opTrack = EMMediaEngine::Instance() -> GetMediaProject() -> GetUsedTracks() -> Find(vTrackID);
//	if(opTrack == NULL)
//	{
//		opTrack = EMMediaEngine::Instance() -> GetMediaProject() -> GetUnusedTracks() -> Find(vTrackID);
//	}
//	if(opTrack == NULL)
//	{
//		EMDebugger("ERROR! The track described in the project file could not be found!");
//	}
//	m_opBelongsToTrack = opTrack;
	return true;
}

/*
EMMediaDataBuffer* EMMediaClip::GetBuffer() const
{
	EMMediaDataBuffer* opBuffer = EMMediaBufferSource::GetBuffer();
	if(opBuffer != NULL)
		opBuffer -> m_opDestination = m_opBelongsToTrack -> GetClipDataDestination();
	return opBuffer;
}
*/


bool EMMediaClip::HasEffectTrack() const
{
	return m_vFxID != -1;
}

int32 EMMediaClip::GetEffectTrackID() const
{
	return m_vFxID;
}

void EMMediaClip::SetEffectTrackID(int32 p_vFxID)
{
	m_vFxID = p_vFxID;
}

void EMMediaClip::CloseFile()
{
	return;
}

void EMMediaClip::OpenFile()
{
	return;
}

