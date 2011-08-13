#include "EMMediaTransitionTrackRepository.h"

#include "EMGlobals.h"
#include "EMProjectDataSaver.h"
#include "EMProjectDataLoader.h"
#include "EMMediaEngine.h"
#include "EMMediaProject.h"

//EMMediaTransitionTrackRepository* EMMediaTransitionTrackRepository::m_opInstance = NULL;
/*
EMMediaTransitionTrackRepository* EMMediaTransitionTrackRepository::Instance()
{
	if(m_opInstance == NULL)
	{
		m_opInstance = EM_new EMMediaTransitionTrackRepository();
	}
	return m_opInstance;
}

void EMMediaTransitionTrackRepository::Delete()
{
	delete m_opInstance;
}
*/

EMMediaTransitionTrackRepository::EMMediaTransitionTrackRepository()
	:	EMMediaItemContainer<EMMediaVideoTransitionTrack>(EM_TYPE_TRANSITION)
{
//	m_opInstance = NULL; 
}

EMMediaTransitionTrackRepository::~EMMediaTransitionTrackRepository()
{
}

void EMMediaTransitionTrackRepository::OnItemAdd()
{
	EMMediaEngine::Instance() -> GetMediaProject() -> SetDirty(true); //Project needs saving, when transition tracks are added! Added by Richard, 010821
}

void EMMediaTransitionTrackRepository::OnItemRemove()
{
	EMMediaEngine::Instance() -> GetMediaProject() -> SetDirty(true); //Project needs saving, when transition tracks are added! Added by Richard, 010821
}

bool EMMediaTransitionTrackRepository::SaveData(EMProjectDataSaver* p_opSaver)
{
	p_opSaver -> SaveString("EMMediaTransitionTrackRepository - Begin");
	
	LockContainer();
	uint32 vCount = 0;
	Rewind();
	while(Current() != NULL)
	{
		if(! Current() -> IsObjectDeleted())
			vCount++;
		Next();
	}
	p_opSaver -> SaveUInt32(vCount);

	Rewind();
	while(Current() != NULL)
	{
		if(! Current() -> IsObjectDeleted())
			Current() -> SaveData(p_opSaver);
		Next();
	}
	UnlockContainer();

	p_opSaver -> SaveString("EMMediaTransitionTrackRepository - End");

	return true;
}

bool EMMediaTransitionTrackRepository::LoadData(EMProjectDataLoader* p_opLoader)
{
	if(p_opLoader -> LoadString() != string("EMMediaTransitionTrackRepository - Begin"))
	{
		EMDebugger("ERROR! Error in project file!");
		return false;
	}

	uint32 vSize = p_opLoader -> LoadUInt32();
	for(uint32 vIndex = 0; vIndex < vSize; vIndex++)
	{
		EMMediaVideoTransitionTrack* opTransitionTrack = EM_new EMMediaVideoTransitionTrack("Dummy name - change loader if present!");
		opTransitionTrack -> InitCheckE();
		if(! opTransitionTrack -> LoadData(p_opLoader))
		{
			delete opTransitionTrack;
			return false;
		}
		Add(opTransitionTrack);
	}

	if(p_opLoader -> LoadString() != string("EMMediaTransitionTrackRepository - End"))
	{
		EMDebugger("ERROR! Error in project file!");
		return false;
	}
	
	return true;
}

bool EMMediaTransitionTrackRepository::RestoreAfterLoad()
{
	LockContainer();
	Rewind();
	while(Current() != NULL)
	{
		Current() -> RestoreAfterLoad();
		Next();
	}
	UnlockContainer();	

	return true;
}

