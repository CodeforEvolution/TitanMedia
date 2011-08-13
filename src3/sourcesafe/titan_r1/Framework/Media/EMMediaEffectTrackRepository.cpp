#include "EMMediaEffectTrackRepository.h"

#include "EMGlobals.h"
#include "EMProjectDataSaver.h"
#include "EMProjectDataLoader.h"
#include "EMMediaEngine.h"
#include "EMMediaProject.h"
#include "EMRealtimeOutputDescriptor.h"
#include "EMMediaTimer.h"

EMMediaEffectTrackRepository* EMMediaEffectTrackRepository::m_opInstance = NULL;

EMMediaEffectTrackRepository* EMMediaEffectTrackRepository::Instance()
{
	if(m_opInstance == NULL)
		m_opInstance = EM_new EMMediaEffectTrackRepository();
	return m_opInstance;
}

void EMMediaEffectTrackRepository::Delete()
{
	delete m_opInstance;
	m_opInstance = NULL;
} 

EMMediaEffectTrackRepository::EMMediaEffectTrackRepository()
	:	EMMediaItemContainer<EMMediaEffectTrack>(EM_TYPE_ANY)
{
}

EMMediaEffectTrackRepository::~EMMediaEffectTrackRepository()
{
}

void EMMediaEffectTrackRepository::OnItemAdd()
{
	EMMediaEngine::Instance() -> GetMediaProject() -> SetDirty(true);
}

void EMMediaEffectTrackRepository::OnItemRemove()
{
	EMMediaEngine::Instance() -> GetMediaProject() -> SetDirty(true);
}

bool EMMediaEffectTrackRepository::SaveData(EMProjectDataSaver* p_opSaver)
{
	int vCnt = 0;
	LockContainer();
	p_opSaver -> SaveString("EMT");

	Rewind();
	while(Current() != NULL)
	{
		if(! Current() -> IsObjectDeleted())
			++ vCnt;
		Next();
	}

	p_opSaver -> SaveUInt32(vCnt); //Size());

	Rewind();
	while(Current() != NULL)
	{
		if(!(Current() -> IsObjectDeleted()))
		{
			p_opSaver -> SaveInt32(Current() -> GetType());
			Current() -> SaveData(p_opSaver);
		}
		Next();
	}
	UnlockContainer();
	return true;
}

bool EMMediaEffectTrackRepository::LoadData(EMProjectDataLoader* p_opLoader)
{
	if(string(p_opLoader -> LoadString()) != string("EMT"))
	{
		EMDebugger("ERROR! Error in project file!");
		return false;
	}
	uint32 vSize = p_opLoader -> LoadUInt32();
	for(uint32 vIndex = 0; vIndex < vSize; vIndex++)
	{
		EMMediaType eType = static_cast<EMMediaType>(p_opLoader -> LoadInt32());
		EMMediaEffectTrack* opFXTrack = EM_new EMMediaEffectTrack(eType);
		opFXTrack -> InitCheckE();
		if(! opFXTrack -> LoadData(p_opLoader))
		{
			delete opFXTrack;
			return false;
		}
		Add(opFXTrack);
	}
	
	EMRealtimeOutputDescriptor* opOut = EMOutputRepository::Instance() -> FindClockMaster();
	if(opOut != NULL)
	{
		opOut -> RegisterAsClockMaster();
		EMMediaTimer::Instance() -> RegisterClockMaster(opOut);
	}

	return true; //RestoreAfterLoad();
}

bool EMMediaEffectTrackRepository::RestoreAfterLoad()
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

