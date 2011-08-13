#include "MediaCommandDestructiveFadeIn.h"

#include <windows.h>

#include "EMMediaEngine.h"
#include "EMMediaFileRepository.h"
#include "EMMediaProject.h"
#include "EMMediaClipRepository.h"
#include "EMMediaClip.h"
#include "EMDestructiveFadeIn.h"
#include "EMFileInputDescriptor.h"
#include "EMWinMediaUtility.h"



MediaCommandDestructiveFadeIn::MediaCommandDestructiveFadeIn()
{
}

MediaCommandDestructiveFadeIn::~MediaCommandDestructiveFadeIn()
{
}

EMDestructivePlugin* MediaCommandDestructiveFadeIn::GetPlugin(int32 p_vId)
{
	return &m_oPlugin;
}

EMCommand* MediaCommandDestructiveFadeIn::CloneAndClear()
{
	MediaCommandDestructiveFadeIn* opCmd = EM_new MediaCommandDestructiveFadeIn;

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
MediaCommandDestructiveFadeIn::MediaCommandDestructiveFadeIn()
	: m_oNewName("")
{

}

MediaCommandDestructiveFadeIn::~MediaCommandDestructiveFadeIn()
{
}

void* MediaCommandDestructiveFadeIn::ExecuteE(void* p_upClipID, void* p_upFxID, void* p_upTimes)
{
	int32 vClipID = *(static_cast<int32*>(p_upClipID));
	int64* vTimes = static_cast<int64*>(p_upTimes);

	int64 vStartRegion = EMMediaUtility::Instance() -> TimeToFrames(vTimes[0], EMMediaUtility::Instance() -> GetSystemAudioFormat());
	int64 vEndRegion = EMMediaUtility::Instance() -> TimeToFrames(vTimes[1], EMMediaUtility::Instance() -> GetSystemAudioFormat()) ;

	EMDestructiveFadeIn oPlugin;
	int64 vStartMedia;

	EMMediaClip* opClip;

	string oNewName;

	try
	{
		opClip = EMMediaEngine::Instance() -> GetMediaProject() -> GetAudioClipRepository() -> Find(vClipID);
	}
	catch(...)
	{
		EMDebugger("MediaCommandDestructiveFadeIn::GetClip");
	}

	if(opClip == NULL)
	{
		EMDebugger("MediaCommandDestructiveFadeIn - Couldn't find clip!!");
		return NULL;
	}


	vStartMedia = opClip -> GetMediaStart();

	m_oNewName = oPlugin.Process(opClip -> GetDescriptor() -> GetFileName().c_str(), vStartRegion - vStartMedia, vEndRegion - vStartMedia);
	m_vDummy = m_oNewName.c_str();

	return (void*)(m_vDummy);
}


void MediaCommandDestructiveFadeIn::UndoE()
{
	DeleteFile(m_vDummy);
}

EMCommand* MediaCommandDestructiveFadeIn::CloneAndClear()
{
	MediaCommandDestructiveFadeIn* opCmd = EM_new MediaCommandDestructiveFadeIn;

	opCmd -> m_oNewName = m_oNewName;
	opCmd -> m_vDummy = opCmd -> m_oNewName.c_str();

	m_oNewName = "";
	m_vDummy = NULL;
	
	return opCmd;
}



bool MediaCommandDestructiveFadeIn::RequiresParameters()
{
	return true;
}

bool MediaCommandDestructiveFadeIn::IsUndoable()
{
	return false;
}*/