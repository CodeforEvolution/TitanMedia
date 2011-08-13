#include "MediaInfoCommandGetEntryInfo.h"

#include "EMMediaEngine.h"
#include "EMMediaProject.h"
#include "EMMediaPool.h"
#include "EMMediaPoolEntry.h"
#include "EMMediaClipRepository.h"
#include "EMMediaClip.h"
#include "EMFileInputDescriptor.h"

#include <list>

MediaInfoCommandGetEntryInfo::MediaInfoCommandGetEntryInfo()
{
}

void* MediaInfoCommandGetEntryInfo::ExecuteE(void* p_upEntryID, void*, void*)
{
	//eo << "MediaInfoCommandGetEntryInfo" << ef;
	int32 vEntryID = *static_cast<int32*>(p_upEntryID);
	
	EMMediaProject* opProject = EMMediaEngine::Instance() -> GetMediaProject();
	EMMediaPool* opPool = opProject -> GetMediaPool(); 
	EMMediaClipRepository* opAudioClips = opProject -> GetAudioClipRepository();
	EMMediaClipRepository* opVideoClips = opProject -> GetVideoClipRepository();

	opPool -> LockContainer();
	EMMediaPoolEntry* opEntry = opPool -> Find(vEntryID);
	opPool -> UnlockContainer();

	if(opEntry == NULL)
		return static_cast<void*>(NULL);
	
	m_oReturnValue.m_vID = opEntry -> GetID();
	m_oReturnValue.m_eType = opEntry -> GetType();
	m_oReturnValue.m_oFileName = opEntry -> GetFileName();
	m_oReturnValue.m_oOriginalFileName = string("ERROR! Unknown! Not implemented yet!");
	m_oReturnValue.m_oPeaksFileName = string("ERROR! Unknown! Not implemented yet!");
	m_oReturnValue.m_vBitDepth = opEntry -> BitDepth();
	m_oReturnValue.m_vFrameRate = opEntry -> GetFrameRate();
	m_oReturnValue.m_vLength = opEntry -> GetLength();
	m_oReturnValue.m_vSize = opEntry -> GetSize();
	m_oReturnValue.m_vUsageCount = opEntry -> GetUsage();

	m_oReturnValue.m_oClipIDs.clear();

	opAudioClips -> LockContainer();
	opAudioClips -> Rewind();
	while(opAudioClips -> Current() != NULL)
	{
		if(opAudioClips -> Current() -> GetDescriptor() -> GetMediaPoolEntryID() == vEntryID && ! opAudioClips -> Current() -> IsObjectDeleted())
			m_oReturnValue.m_oClipIDs.push_back(opAudioClips -> Current() -> GetID());
		opAudioClips -> Next();
	}
	opAudioClips -> UnlockContainer();

	opVideoClips -> LockContainer();
	opVideoClips -> Rewind();
	while(opVideoClips -> Current() != NULL)
	{
		if(opVideoClips -> Current() -> GetDescriptor() -> GetMediaPoolEntryID() == vEntryID && ! opVideoClips -> Current() -> IsObjectDeleted())
			m_oReturnValue.m_oClipIDs.push_back(opVideoClips -> Current() -> GetID());
		opVideoClips -> Next();
	}
	opVideoClips -> UnlockContainer();

	return static_cast<void*>(&m_oReturnValue);
}

bool MediaInfoCommandGetEntryInfo::RequiresParameters()
{
	return true;
}

bool MediaInfoCommandGetEntryInfo::IsUndoable()
{
	return false;
}