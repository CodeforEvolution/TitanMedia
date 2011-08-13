#include "MediaCommandDestructiveSilence.h"

#include <windows.h>

#include "EMMediaEngine.h"
#include "EMMediaFileRepository.h"
#include "EMMediaProject.h"
#include "EMMediaClipRepository.h"
#include "EMMediaClip.h"
#include "EMDestructiveSilence.h"
#include "EMFileInputDescriptor.h"
#include "EMWinMediaUtility.h"

MediaCommandDestructiveSilence::MediaCommandDestructiveSilence()
{
}

MediaCommandDestructiveSilence::~MediaCommandDestructiveSilence()
{
}

EMDestructivePlugin* MediaCommandDestructiveSilence::GetPlugin(int32 p_vId)
{
	return &m_oPlugin;
}

EMCommand* MediaCommandDestructiveSilence::CloneAndClear()
{
	MediaCommandDestructiveSilence* opCmd = EM_new MediaCommandDestructiveSilence;

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
MediaCommandDestructiveSilence::MediaCommandDestructiveSilence()
	: m_oNewName("")
{

}

MediaCommandDestructiveSilence::~MediaCommandDestructiveSilence()
{
}

void* MediaCommandDestructiveSilence::ExecuteE(void* p_upClipID, void* p_upFxID, void* p_upTimes)
{
	int32 vClipID = *(static_cast<int32*>(p_upClipID));
	int64* vTimes = static_cast<int64*>(p_upTimes);

	int64 vStartRegion = EMMediaUtility::Instance() -> TimeToFrames(vTimes[0], EMMediaUtility::Instance() -> GetSystemAudioFormat());
	int64 vEndRegion = EMMediaUtility::Instance() -> TimeToFrames(vTimes[1], EMMediaUtility::Instance() -> GetSystemAudioFormat()) ;

	EMDestructiveSilence oPlugin;
	int64 vStartMedia;

	EMMediaClip* opClip;

	string oNewName;

	try
	{
		opClip = EMMediaEngine::Instance() -> GetMediaProject() -> GetAudioClipRepository() -> Find(vClipID);
	}
	catch(...)
	{
		EMDebugger("MediaCommandDestructiveSilence::GetClip");
	}

	if(opClip == NULL)
	{
		EMDebugger("MediaCommandDestructiveSilence - Couldn't find clip!!");
		return NULL;
	}

	vStartMedia = opClip -> GetMediaStart();

	m_oNewName = oPlugin.Process(opClip -> GetDescriptor() -> GetFileName().c_str(), vStartRegion - vStartMedia, vEndRegion - vStartMedia);
	m_vDummy = m_oNewName.c_str();

	return (void*)(m_vDummy);
}


void MediaCommandDestructiveSilence::UndoE()
{
	DeleteFile(m_vDummy);
}

EMCommand* MediaCommandDestructiveSilence::CloneAndClear()
{
	MediaCommandDestructiveSilence* opCmd = EM_new MediaCommandDestructiveSilence;

	opCmd -> m_oNewName = m_oNewName;
	opCmd -> m_vDummy = opCmd -> m_oNewName.c_str();

	m_oNewName = "";
	m_vDummy = NULL;
	
	return opCmd;
}



bool MediaCommandDestructiveSilence::RequiresParameters()
{
	return true;
}

bool MediaCommandDestructiveSilence::IsUndoable()
{
	return false;
}*/