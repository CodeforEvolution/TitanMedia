#include "MediaCommandDestructiveBase.h"

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
#include "EMMediaEffectTrackRepository.h"
#include "EMMediaEffectTrack.h"
#include "EMMediaFileManager.h"
#include "EMAudioFileInputDescriptor.h"
#include "EMMediaPoolAudioEntry.h"

MediaCommandDestructiveBase::MediaCommandDestructiveBase()
	: m_oNewName(""),
	m_oPeaksFile(""),
	m_vMediaPoolValue(-1),
	m_vReturnValue(-1)
{

}

MediaCommandDestructiveBase::~MediaCommandDestructiveBase()
{
}

void* MediaCommandDestructiveBase::ExecuteE(void* p_upClipID, void* p_upFxID, void* p_upTimes)
{
//	int32 vpArray[2];
	int32 vClipID = *(static_cast<int32*>(p_upClipID));
	int32 vFxID = p_upFxID == NULL ? -1 : *(static_cast<int32*>(p_upFxID));
	int64* vTimes = static_cast<int64*>(p_upTimes);

	int64 vStartRegion = EMMediaUtility::Instance() -> TimeToFrames(vTimes[0], EMMediaUtility::Instance() -> GetSystemAudioFormat());
	int64 vEndRegion = EMMediaUtility::Instance() -> TimeToFrames(vTimes[1], EMMediaUtility::Instance() -> GetSystemAudioFormat());
	int64 *vpExtraParam = NULL;

	EMDestructivePlugin* opPlugin = GetPlugin(vFxID);

	int vNumParams = opPlugin -> NeedExtraParams();
	if(vNumParams != 0)
	{
		vpExtraParam = EM_new int64[vNumParams];

		for(int k = 0; k < vNumParams; ++k)
			vpExtraParam[k] = vTimes[2 + k];
	}


	int64 vStartMedia;

	EMMediaClip* opClip;
	EMMediaClip* opNewClip;

	try
	{
		opClip = EMMediaEngine::Instance() -> GetMediaProject() -> GetAudioClipRepository() -> Find(vClipID);

		if(opClip == NULL)
		{
			EMDebugger("MediaCommandDestructiveBase - Couldn't find clip!!");
			return NULL;
		}

		vStartMedia = opClip -> GetMediaStart();

		m_oNewName = opPlugin -> Process(opClip -> GetDescriptor() -> GetFileName().c_str(), vStartRegion - vStartMedia, vEndRegion - vStartMedia, vpExtraParam, opClip -> GetMarkInLength() , opClip -> GetMarkOutLength());

		opPlugin -> Done();

//		EMMediaEngine::Instance() -> GetMediaProject() -> GetMediaPool() -> AddMediaFileE(m_oNewName, vpArray);
//		EMFileInputDescriptor* opDescriptor = EMMediaEngine::Instance() -> GetMediaProject() -> GetMediaPool() -> InstantiateEntryE(vpArray[0]);
//		m_vMediaPoolValue = vpArray[0];


		//Add File to media pool... without copy/extraction etc...

		EMMediaFileRepository::Instance() -> AddFile(&m_oNewName);

		m_oPeaksFile = EMMediaFileManager::Instance() -> CreatePeaksFileE(&m_oNewName);

		EMMediaFileRepository::Instance() -> AddFile(&m_oPeaksFile);	

		EMMediaPoolEntry* opEntry = EM_new EMMediaPoolAudioEntry(m_oNewName);

		if(! opEntry -> InitCheckE())
			EMDebugger("ERROR! Could not initialize audio entry!");

		m_vMediaPoolValue = opEntry -> GetID();

		EMMediaEngine::Instance() -> GetMediaProject() -> GetMediaPool() -> Add(opEntry);

		EMFileInputDescriptor* opDescriptor = EM_new EMAudioFileInputDescriptor(opEntry -> GetFileName());

		if(! opDescriptor -> Open())
		{
			delete opDescriptor;
			return NULL; //TODO: Throw exception?
		}
	
		opDescriptor -> SetName(opEntry -> GetFileName());
		opDescriptor -> SetMediaPoolEntryID(m_vMediaPoolValue);
		opEntry -> IncreaseUsed();



		//Create the clip
		m_vReturnValue = opClip -> GetTrack() -> CreateClip(vStartRegion, opDescriptor -> GetName(), opDescriptor);

		if(opClip -> HasEffectTrack())
		{
			opNewClip = EMMediaEngine::Instance() -> GetMediaProject() -> GetAudioClipRepository() -> Find(m_vReturnValue);

			EMMediaEffectTrack* opFXTrack = EMMediaEffectTrackRepository::Instance() -> Find(opClip -> GetEffectTrackID());
			EMMediaEffectTrack* opFXClone = EM_new EMMediaEffectTrack(*opFXTrack);

			EMMediaEffectTrackRepository::Instance() -> Add(opFXClone);

			opFXClone -> SetOutput(opNewClip -> GetDestination());
			opNewClip -> SetDestination(opFXClone);
			opNewClip -> SetEffectTrackID(opFXClone -> GetID());
		}

		EMMediaEngine::Instance() -> GetMediaProject() -> SetDirty(true);
	}
	catch(...)
	{
		EMDebugger("MediaCommandDestructiveBase::GetClip");
	}

	if(vpExtraParam != NULL)
		delete vpExtraParam;

	return (void*)(&m_vReturnValue);
}


void MediaCommandDestructiveBase::UndoE()
{
	EMMediaFile* opFile = NULL;

	try
	{
		EMMediaClip* opClip = EMMediaEngine::Instance() -> GetMediaProject() -> GetAudioClipRepository() -> Find(m_vReturnValue);

		if(opClip != NULL)
		{
			EMMediaEngine::Instance() -> GetMediaProject() -> GetAudioClipRepository() -> Remove(opClip);
			delete opClip;
		}

		EMMediaPoolEntry* opEntry = EMMediaEngine::Instance() -> GetMediaProject() -> GetMediaPool() -> Find(m_vMediaPoolValue);

		if(opEntry != NULL)
		{
			EMMediaEngine::Instance() -> GetMediaProject() -> GetMediaPool() -> Remove(opEntry);
			delete opEntry;
		}

		opFile =  EMMediaFileRepository::Instance() -> FindFileName(&m_oPeaksFile);
		if(opFile != NULL)
		{
			EMMediaFileRepository::Instance() -> RemoveFile(opFile);
			opFile -> Close();
			delete opFile;
		}

		opFile =  EMMediaFileRepository::Instance() -> FindFileName(&m_oNewName);
		if(opFile != NULL)
		{
			EMMediaFileRepository::Instance() -> RemoveFile(opFile);
			opFile -> Close();
			delete opFile;
		}

	}
	catch(...)
	{

	}


	bool vOk;
	vOk = DeleteFile(m_oNewName.c_str())  != 0;
	vOk = DeleteFile(m_oPeaksFile.c_str()) != 0;
}

bool MediaCommandDestructiveBase::RequiresParameters()
{
	return true;
}

bool MediaCommandDestructiveBase::IsUndoable()
{
	return false;
}
