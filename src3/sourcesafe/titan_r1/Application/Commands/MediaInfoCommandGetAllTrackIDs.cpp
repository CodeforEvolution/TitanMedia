#include "MediaInfoCommandGetAllTrackIDs.h"

#include "EMMediaProject.h"
#include "EMMediaTrack.h"
#include "EMMediaEngine.h"
#include "EMMediaTrackRepository.h"
#include "EMMediaTransitionTrackRepository.h"

MediaInfoCommandGetAllTrackIDs::MediaInfoCommandGetAllTrackIDs()
{
}

void* MediaInfoCommandGetAllTrackIDs::ExecuteE(void*, void*, void*)
{
//	eo << "MediaInfoCommandGetAllTrackIDs" << ef;
	m_oReturnValue.clear();
	
	EMMediaEngine::Instance() -> GetMediaProject() -> GetUsedTracks() -> LockContainer();
	try
	{
		EMMediaEngine::Instance() -> GetMediaProject() -> GetUsedTracks() -> Rewind();
		while(EMMediaEngine::Instance() -> GetMediaProject() -> GetUsedTracks() -> Current() != NULL)
		{
			if(! (EMMediaEngine::Instance() -> GetMediaProject() -> GetUsedTracks() -> Current() -> IsObjectDeleted()))
				m_oReturnValue.push_back(EMMediaEngine::Instance() -> GetMediaProject() -> GetUsedTracks() -> Current() -> GetID());
			EMMediaEngine::Instance() -> GetMediaProject() -> GetUsedTracks() -> Next();
		}
	}
	catch(...)
	{
		EMDebugger("ERROR! Exception in MediaInfoCommandGetAllTrackIDs::ExecuteE");
	}
	EMMediaEngine::Instance() -> GetMediaProject() -> GetUsedTracks() -> UnlockContainer();


	EMMediaEngine::Instance() -> GetMediaProject() -> GetUnusedTracks() -> LockContainer();
	try
	{
		EMMediaEngine::Instance() -> GetMediaProject() -> GetUnusedTracks() -> Rewind();
		while(EMMediaEngine::Instance() -> GetMediaProject() -> GetUnusedTracks() -> Current() != NULL)
		{
			if(! (EMMediaEngine::Instance() -> GetMediaProject() -> GetUnusedTracks() -> Current() -> IsObjectDeleted()))
				m_oReturnValue.push_back(EMMediaEngine::Instance() -> GetMediaProject() -> GetUnusedTracks() -> Current() -> GetID());
			EMMediaEngine::Instance() -> GetMediaProject() -> GetUnusedTracks() -> Next();
		}
	}
	catch(...)
	{
		EMDebugger("ERROR! Exception in MediaInfoCommandGetAllTrackIDs::ExecuteE");
	}
	EMMediaEngine::Instance() -> GetMediaProject() -> GetUnusedTracks() -> UnlockContainer();

	EMMediaEngine::Instance() -> GetMediaProject() -> GetVideoTransitionTrackRepository() -> LockContainer();
	try
	{
		EMMediaEngine::Instance() -> GetMediaProject() -> GetVideoTransitionTrackRepository() -> Rewind();
		while(EMMediaEngine::Instance() -> GetMediaProject() -> GetVideoTransitionTrackRepository() -> Current() != NULL)
		{
			if(! (EMMediaEngine::Instance() -> GetMediaProject() -> GetVideoTransitionTrackRepository() -> Current() -> IsObjectDeleted()))
				m_oReturnValue.push_back(EMMediaEngine::Instance() -> GetMediaProject() -> GetVideoTransitionTrackRepository() -> Current() -> GetID());
			EMMediaEngine::Instance() -> GetMediaProject() -> GetVideoTransitionTrackRepository() -> Next();
		}
	}
	catch(...)
	{
		EMDebugger("ERROR! Exception in MediaInfoCommandGetAllTrackIDs::ExecuteE");
	}
	EMMediaEngine::Instance() -> GetMediaProject() -> GetVideoTransitionTrackRepository() -> UnlockContainer();
	

	return static_cast<void*>(&m_oReturnValue);
}

bool MediaInfoCommandGetAllTrackIDs::RequiresParameters()
{
	return true;
}

bool MediaInfoCommandGetAllTrackIDs::IsUndoable()
{
	return false;
}