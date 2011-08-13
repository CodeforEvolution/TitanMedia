#include "EMGlobals.h"

#include "EMMediaClipRepository.h"
#include "EMMediaIDManager.h"
#include "EMMediaItemContainer.h"
#include "EMOutputRepository.h"
#include "EMMediaTrack.h"

#include "EMMediaEngine.h"
#include "EMProjectDataSaver.h"
#include "EMProjectDataLoader.h"
#include "CommandIDs.h"
#include "EMCommandRepository.h"
#include "EMMediaEngine.h"
#include "EMMediaProject.h"

#ifdef PLATFORM_WINDOWS
#include "EMWinAudioClipRepository.h"
#include "EMWinVideoClipRepository.h"
#include "EMWinMIDIClipRepository.h"
#include "EMWinVideotransitionClipRepository.h"
#endif

EMMediaClipRepository* EMMediaClipRepository::New(EMMediaType p_eType)
{
#ifdef PLATFORM_BEOS
	if((p_eType & EM_TYPE_ANY_AUDIO) > 0)
		return EM_new EMBeAudioClipRepository();
	else if((p_eType & EM_TYPE_ANY_VIDEO) > 0)
		return EM_new EMBeVideoClipRepository();
#endif
#ifdef PLATFORM_WINDOWS
	if((p_eType & EM_TYPE_ANY_AUDIO) > 0)
		return EM_new EMWinAudioClipRepository();
	else if((p_eType & EM_TYPE_ANY_VIDEO) > 0)
		return EM_new EMWinVideoClipRepository();
	else if((p_eType & EM_TYPE_MIDI) > 0)
		return EM_new EMWinMIDIClipRepository();
	else if((p_eType & EM_TYPE_TRANSITION) > 0)
		return EM_new EMWinVideoTransitionClipRepository();
#endif
	EMDebugger("ERROR! Unknown type! Can't create new EMMediaClipRepository!");
	return NULL;
}

EMMediaClipRepository::EMMediaClipRepository(EMMediaType p_eType)
	:	EMMediaItemContainer<EMMediaClip>(p_eType)
{
	m_vID = EMMediaIDManager::MakeID();
}

EMMediaClipRepository::~EMMediaClipRepository()
{
}

void EMMediaClipRepository::OnItemAdd()
{
	//If clips are added, the project is considered "dirty" and needs saving!
	EMMediaEngine::Instance() -> GetMediaProject() -> SetDirty(true);
}

void EMMediaClipRepository::OnItemRemove()
{
	//If clips are removed, the project is considered "dirty" and needs saving!
	EMMediaEngine::Instance() -> GetMediaProject() -> SetDirty(true);
}

int32 EMMediaClipRepository::GetID() const
{
	return m_vID;
}

int64 EMMediaClipRepository::CountClipsForTrack(const EMMediaTrack* p_opTrack, EMMediaType p_eSpecType)
{
	int64 vCount = 0;
	LockContainer();
	try
	{
		Rewind();
		while(Current() != NULL)
		{
			if(Current() -> GetTrack() -> GetID() == p_opTrack -> GetID() &&
				! Current() -> IsObjectDeleted() && (p_eSpecType & Current() -> GetType()) != 0)
				vCount++;
			
			if(Current() -> GetTrack() == NULL)
				EMDebugger("ERROR! NULL as track owner!");
			Next();
		}
	}
	catch(...)
	{
		EMDebugger("ERROR! Exception in EMMediaClipRepository::CountClipsForTrack");
	}
	UnlockContainer();
	return vCount;
}

int64 EMMediaClipRepository::CountActiveClips()
{
	int64 vCount = 0;
	LockContainer();
	try
	{
		Rewind();
		while(Current() != NULL)
		{
			if(! Current() -> IsObjectDeleted())
				vCount++;
			Next();
		}
	}
	catch(...)
	{
		EMDebugger("ERROR! Exception in EMMediaClipRepository::CountActiveClips");
	}
	UnlockContainer();
	return vCount;
}

bool EMMediaClipRepository::SaveData(EMProjectDataSaver* p_opSaver)
{
/*	;//cout_commented_out_4_release << "EMMediaClipRepository::SaveData" << endl;
	Lock();
	
	uint32 vNumClips = static_cast<uint32>(Size());
	p_opSaver -> SaveUInt32(vNumClips);

	Rewind();
	while(Current() != NULL)
	{
		uint32 vType = static_cast<uint32>(Current() -> GetType());
		p_opSaver -> SaveUInt32(vType);
		Current() -> SaveData(p_opSaver);
		Next();
	}
	
	Unlock();
	;//cout_commented_out_4_release << "EMMediaClipRepository::SaveData complete" << endl; */
	return true;
}

bool EMMediaClipRepository::LoadData(EMProjectDataLoader* p_opLoader)
{
/*	;//cout_commented_out_4_release << "EMMediaClipRepository::LoadData" << endl;
	Lock();
	uint32 vNumClips = p_opLoader -> LoadUInt32();
	
	for(uint32 vIndex = 0; vIndex < vNumClips; vIndex++)
	{
		uint32 vType = p_opLoader -> LoadUInt32();
		EMMediaType eRealType = EM_TYPE_ANY;
		if((vType & EM_TYPE_RAW_AUDIO) == EM_TYPE_RAW_AUDIO) eRealType = EM_TYPE_RAW_AUDIO;
		else if((vType & EM_TYPE_ENCODED_AUDIO) == EM_TYPE_RAW_AUDIO) eRealType = EM_TYPE_ENCODED_AUDIO;
		else if((vType & EM_TYPE_ANY_AUDIO) == EM_TYPE_RAW_AUDIO) eRealType = EM_TYPE_ANY_AUDIO;
		else if((vType & EM_TYPE_RAW_VIDEO) == EM_TYPE_RAW_VIDEO) eRealType = EM_TYPE_RAW_VIDEO;
		else if((vType & EM_TYPE_ENCODED_VIDEO) == EM_TYPE_ENCODED_VIDEO) eRealType = EM_TYPE_ENCODED_VIDEO;
		else if((vType & EM_TYPE_ANY_VIDEO) == EM_TYPE_ANY_VIDEO) eRealType = EM_TYPE_ANY_VIDEO;
		else 
			EMDebugger("ERROR! Unknown type saved in project file!");

		EMMediaClip* opClip = EM_new EMMediaClip(p_opLoader, eRealType);
		opClip;
	}
	
	Unlock();
	;//cout_commented_out_4_release << "EMMediaClipRepository::LoadData complete" << endl; */
	return true; 
}

bool EMMediaClipRepository::ClearData()
{
	LockContainer();
	try
	{
		while(Size() > 0)
		{
			Rewind();
			EMMediaClip* opClip = Remove(Current());
			delete opClip;
		}
	}
	catch(...)
	{
		EMDebugger("ERROR! Exception in EMMediaClipRepository::ClearData()");
	}
	UnlockContainer();
	return true;
}
