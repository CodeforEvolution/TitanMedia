#include "MediaCommandDestructiveEdit.h"

#include <windows.h>

#include "EMMediaEngine.h"
#include "EMMediaFileRepository.h"
#include "EMMediaPool.h"
#include "EMMediaProject.h"
#include "EMMediaClipRepository.h"
#include "EMMediaClip.h"
#include "EMFileInputDescriptor.h"
#include "EMPluginRepository.h"
#include "EMCommandRepository.h"
#include "CommandIDs.h"
#include "EMDestructivePlugin.h"
#include "EMDestructiveDXWrapper.h"
#include "EMWinMediaUtility.h"

MediaCommandDestructiveEdit::MediaCommandDestructiveEdit()
	:	m_opPlug(NULL),
		m_vWrapped(false)
{
}

MediaCommandDestructiveEdit::~MediaCommandDestructiveEdit()
{
	if(m_opPlug != NULL && !m_vWrapped)
		delete m_opPlug;
}

EMDestructivePlugin* MediaCommandDestructiveEdit::GetPlugin(int32 p_vId)
{
	try
	{
		EMPluginEntry* opEntry = EMPluginRepository::Instance() -> Find(p_vId);

		if(opEntry != NULL)
		{
			if(opEntry -> GetPluginType() != EM_DESTRUCTIVE && opEntry -> GetPluginType() != EM_DESTRUCTIVE_NATIVE)
			{
				m_opPlug = EM_new EMDestructiveDXWrapper(opEntry);
				m_vWrapped = true;
			}
			else
				m_opPlug = dynamic_cast<EMDestructivePlugin*>(opEntry -> InstantiateNativePlugin());
		}
	}
	catch(...)
	{
		EMDebugger("MediaCommandDestructiveEdit::GetFx");
	}

	return m_opPlug;
}

EMCommand* MediaCommandDestructiveEdit::CloneAndClear()
{
	MediaCommandDestructiveEdit* opCmd = EM_new MediaCommandDestructiveEdit;


	opCmd -> m_opPlug = m_opPlug;
	opCmd -> m_vWrapped = m_vWrapped;

	m_opPlug = NULL;
	m_vWrapped = false;

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
MediaCommandDestructiveEdit::MediaCommandDestructiveEdit()
	: m_oNewName("")
{

}

MediaCommandDestructiveEdit::~MediaCommandDestructiveEdit()
{
}


list<EMMediaClip*> MediaCommandDestructiveEdit::GetClips(int64 p_vStart, int64 p_vStop)
{
	int64 vStartClip;
	int64 vEndClip;

	EMMediaClip* opClip = NULL;
	EMMediaPoolEntry* opEntry = NULL;
	EMMediaClipRepository* opClips = EMMediaEngine::Instance() -> GetMediaProject() -> GetAudioClipRepository();

	opClips -> LockContainer();
	opClips -> Rewind();

	list<EMMediaClip*> oList;

	while(opClips -> Current() != NULL)
	{
		opClip = opClips -> Current();

		vStartClip = opClip -> GetStart();
		vEndClip = opClip -> GetStop();

		if(vStartClip < p_vStop && vEndClip > p_vStart)
		{
			oList.push_back(opClip);

			opClip -> SetDeleted(true);
			opEntry =  EMMediaEngine::Instance() -> GetMediaProject() -> GetMediaPool() -> Find(opClip -> GetDescriptor() -> GetMediaPoolEntryID());
			opEntry -> DecreaseUsed();
		}
		opClips -> Next();
	}
	opClips -> UnlockContainer();

	return oList;
}

EMMediaClip* MediaCommandDestructiveEdit::GetClip(int32 p_vID)
{
	try
	{
		return EMMediaEngine::Instance() -> GetMediaProject() -> GetAudioClipRepository() -> Find(p_vID);
	}
	catch(...)
	{
	}

	EMDebugger("MediaCommandDestructiveEdit::GetClip()");

	return NULL;
}


void* MediaCommandDestructiveEdit::ExecuteE(void* p_upClipID, void* p_upFxID, void* p_upTimes)
{
	int32 vClipID = *(static_cast<int32*>(p_upClipID));
	int32 vEffectID = *(static_cast<int32*>(p_upFxID));
	int64* vTimes = static_cast<int64*>(p_upTimes);

	int64 vStartRegion = EMMediaUtility::Instance() -> TimeToFrames(vTimes[0], EMMediaUtility::Instance() -> GetSystemAudioFormat());
	int64 vEndRegion = EMMediaUtility::Instance() -> TimeToFrames(vTimes[1], EMMediaUtility::Instance() -> GetSystemAudioFormat()) ;

	int64 vStartMedia;

	EMMediaClip* opClip;

	EMDestructivePlugin* opPlug = NULL;
	EMPluginEntry* opEntry = NULL;

	bool vWrapped = false;

	string oNewName;

	try
	{
		opClip = EMMediaEngine::Instance() -> GetMediaProject() -> GetAudioClipRepository() -> Find(vClipID);
	}
	catch(...)
	{
		EMDebugger("MediaCommandDestructiveEdit::GetClip");
	}

	if(opClip == NULL)
	{
		EMDebugger("MediaCommandDestructiveEdit - Couldn't find clip!!");
		return NULL;
	}

	try
	{
		opEntry = EMPluginRepository::Instance() -> Find(vEffectID);

		if(opEntry != NULL)
		{
			if(opEntry -> GetPluginType() != EM_DESTRUCTIVE && opEntry -> GetPluginType() != EM_DESTRUCTIVE_NATIVE)
			{
				opPlug = EM_new EMDestructiveDXWrapper(opEntry);
				vWrapped = true;
			}
			else
				opPlug = dynamic_cast<EMDestructivePlugin*>(opEntry -> InstantiateNativePlugin());
		}
	}
	catch(...)
	{
		EMDebugger("MediaCommandDestructiveEdit::GetFx");
	}

	if(opPlug == NULL)
	{
		EMDebugger("MediaCommandDestructiveEdit - Couldn't find plugin!!");
		return NULL;
	}

	vStartMedia = opClip -> GetMediaStart();

	m_oNewName = opPlug -> Process(opClip -> GetDescriptor() -> GetFileName().c_str(), vStartRegion - vStartMedia, vEndRegion - vStartMedia);
	m_vDummy = m_oNewName.c_str();

	if(vWrapped)
		delete opPlug;

	return (void*)(m_vDummy);

}


void MediaCommandDestructiveEdit::UndoE()
{
	DeleteFile(m_vDummy);
}

EMCommand* MediaCommandDestructiveEdit::CloneAndClear()
{
	MediaCommandDestructiveEdit* opCmd = EM_new MediaCommandDestructiveEdit;

	opCmd -> m_oNewName = m_oNewName;
	opCmd -> m_vDummy = opCmd -> m_oNewName.c_str();

	m_oNewName = "";
	m_vDummy = NULL;
	
	return opCmd;
}



bool MediaCommandDestructiveEdit::RequiresParameters()
{
	return true;
}

bool MediaCommandDestructiveEdit::IsUndoable()
{
	return false;
}*/