#include "MediaCommandDestructiveCrop.h"

#include <windows.h>

#include "EMMediaEngine.h"
#include "EMMediaFileRepository.h"
#include "EMMediaProject.h"
#include "EMMediaClipRepository.h"
#include "EMMediaClip.h"
#include "EMDestructiveCrop.h"
#include "EMFileInputDescriptor.h"
#include "EMWinMediaUtility.h"
#include "EMMediaPool.h"

MediaCommandDestructiveCrop::MediaCommandDestructiveCrop()
{
}

MediaCommandDestructiveCrop::~MediaCommandDestructiveCrop()
{
}

EMDestructivePlugin* MediaCommandDestructiveCrop::GetPlugin(int32 p_vId)
{
	return &m_oPlugin;
}

EMCommand* MediaCommandDestructiveCrop::CloneAndClear()
{
	MediaCommandDestructiveCrop* opCmd = EM_new MediaCommandDestructiveCrop;

	opCmd -> m_oNewName = m_oNewName;
	opCmd -> m_vReturnValue = m_vReturnValue;
	opCmd -> m_vMediaPoolValue = m_vMediaPoolValue;
	opCmd -> m_oPeaksFile = m_oPeaksFile;

	m_oNewName = "";
	m_oPeaksFile = "";
	m_vReturnValue = -1;
	m_vMediaPoolValue = -1;


	return opCmd;
}

/*
void* MediaCommandDestructiveCrop::ExecuteE(void* p_upClipID, void* p_upFxID, void* p_upTimes)
{
	int32 vpArray[2];
	int32 vClipID = *(static_cast<int32*>(p_upClipID));
	int64* vTimes = static_cast<int64*>(p_upTimes);

	int64 vStartRegion = EMMediaUtility::Instance() -> TimeToFrames(vTimes[0], EMMediaUtility::Instance() -> GetSystemAudioFormat());
	int64 vEndRegion = EMMediaUtility::Instance() -> TimeToFrames(vTimes[1], EMMediaUtility::Instance() -> GetSystemAudioFormat()) ;

	EMDestructiveCrop oPlugin;
	int64 vStartMedia;

	EMMediaClip* opClip;

	try
	{
		opClip = EMMediaEngine::Instance() -> GetMediaProject() -> GetAudioClipRepository() -> Find(vClipID);

		if(opClip == NULL)
		{
			EMDebugger("MediaCommandDestructiveCrop - Couldn't find clip!!");
			return NULL;
		}

		vStartMedia = opClip -> GetMediaStart();

		m_oNewName = oPlugin.Process(opClip -> GetDescriptor() -> GetFileName().c_str(), vStartRegion - vStartMedia, vEndRegion - vStartMedia);

		EMMediaEngine::Instance() -> GetMediaProject() -> GetMediaPool() -> AddMediaFileE(m_oNewName, vpArray);
		EMFileInputDescriptor* opDescriptor = EMMediaEngine::Instance() -> GetMediaProject() -> GetMediaPool() -> InstantiateEntryE(vpArray[0]);
		
		m_vReturnValue = opClip -> GetTrack() -> CreateClip(vStartRegion, opClip -> GetName(), opDescriptor);


		EMMediaEngine::Instance() -> GetMediaProject() -> SetDirty(true);
	}
	catch(...)
	{
		EMDebugger("MediaCommandDestructiveCrop::GetClip");
	}

	return (void*)(&m_vReturnValue);
}


void MediaCommandDestructiveCrop::UndoE()
{
	DeleteFile(m_oNewName.c_str());
}

EMCommand* MediaCommandDestructiveCrop::CloneAndClear()
{
	MediaCommandDestructiveCrop* opCmd = EM_new MediaCommandDestructiveCrop;

	opCmd -> m_oNewName = m_oNewName;
	opCmd -> m_vDummy = opCmd -> m_oNewName.c_str();

	m_oNewName = "";
	m_vDummy = NULL;
	
	return opCmd;
}



bool MediaCommandDestructiveCrop::RequiresParameters()
{
	return true;
}

bool MediaCommandDestructiveCrop::IsUndoable()
{
	return false;
}
*/