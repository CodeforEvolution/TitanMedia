#include "MediaInfoCommandGetAllEntryIDs.h"

#include "EMMediaEngine.h"
#include "EMMediaPool.h"
#include "EMMediaPoolEntry.h"
//#include "EMBeFileInputDescriptor.h"
#include "EMMediaProject.h"
#include "EMMediaClipRepository.h"
#include "EMMediaClip.h"

#include <list>

MediaInfoCommandGetAllEntryIDs::MediaInfoCommandGetAllEntryIDs()
{
}

void* MediaInfoCommandGetAllEntryIDs::ExecuteE(void*, void*, void*)
{
//	eo << "MediaInfoCommandGetAllEntryIDs" << ef;

	m_oReturnValue.clear();
	EMMediaPool* opPool = EMMediaEngine::Instance() -> GetMediaProject() -> GetMediaPool();
	
	opPool -> LockContainer();

	opPool -> Rewind();

	while(opPool -> Current() != NULL)
	{
		int32 vID = opPool -> Current() -> GetID();
		m_oReturnValue.push_back(vID);
		
		opPool -> Next();
	}

	opPool -> UnlockContainer();
	
	return static_cast<void*>(&m_oReturnValue);
}

bool MediaInfoCommandGetAllEntryIDs::RequiresParameters()
{
	return true;
}

bool MediaInfoCommandGetAllEntryIDs::IsUndoable()
{
	return false;
}