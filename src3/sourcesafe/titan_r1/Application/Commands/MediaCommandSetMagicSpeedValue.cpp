#include "MediaCommandSetMagicSpeedValue.h"

#include "EMMediaEngine.h"
#include "EMCommandRepository.h"
#include "CommandIDs.h"
#include "EMMediaProject.h"
#include "EMMediaTrack.h"
#include "EMMediaClipRepository.h"
#include "EMMediaClip.h"
#include "EMMediaTimer.h"
#include "EMMediaTrackRepository.h"
#include "EMMediaTransitionTrackRepository.h"
#include "EMMediaUtility.h"
#include "EMVideoFileInputDescriptor.h"

MediaCommandSetMagicSpeedValue::MediaCommandSetMagicSpeedValue()
{

}

void* MediaCommandSetMagicSpeedValue::ExecuteE(void* p_upClipID, void* p_upMagicSpeedValue, void*)
{
	EMMediaClip* opClip = NULL;
	EMMediaTrack* opRecentTrack = NULL;

	int32 vClipID = *(static_cast<int32*>(p_upClipID));
	float vMagicSpeedValue = *(static_cast<float*>(p_upMagicSpeedValue));
	int32 vTrackID = -1;
	bool vTrackWasEmpty = false;

	EMMediaEngine* opEngine = EMMediaEngine::Instance();
	EMMediaProject* opProject = opEngine -> GetMediaProject();

	EMMediaClipRepository* opVideoClipRepository = opProject -> GetVideoClipRepository();

	opVideoClipRepository -> LockContainer();
	try
	{
		opClip = opVideoClipRepository -> Find(vClipID);
		//if(opClip != NULL)
		//	opRepository = opVideoClipRepository;
	}
	catch(...)
	{
		EMDebugger("ERROR! Caught an exception in MediaCommandCloneClip::ExecuteE (4)");
	}
	opVideoClipRepository -> UnlockContainer();

	EMVideoFileInputDescriptor* opVideoFileInputDescriptor = static_cast<EMVideoFileInputDescriptor*>( opClip->GetDescriptor() );
	
	opVideoFileInputDescriptor -> m_opVideoFile -> SetMagicSpeedValue(vMagicSpeedValue);

	return NULL;
}

bool MediaCommandSetMagicSpeedValue::RequiresParameters()
{
	return true;
}

bool MediaCommandSetMagicSpeedValue::IsUndoable()
{
	return false;
}
