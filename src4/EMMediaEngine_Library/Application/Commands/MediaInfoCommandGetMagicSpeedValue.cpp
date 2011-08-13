#include "MediaInfoCommandGetMagicSpeedValue.h"

#include "EMMediaEngine.h"
#include "EMCommandRepository.h"
#include "CommandIDs.h"
#include "EMMediaProject.h"
#include "EMMediaTrack.h"
#include "EMMediaClipRepository.h"
#include "EMMediaClip.h"
#include "EMMediaTrackRepository.h"
#include "EMMediaTransitionTrackRepository.h"
#include "EMVideoFileInputDescriptor.h"

MediaInfoCommandGetMagicSpeedValue::MediaInfoCommandGetMagicSpeedValue()
{

}

void* MediaInfoCommandGetMagicSpeedValue::ExecuteE(void* p_upClipID, void*, void*)
{
	EMMediaClip* opClip = NULL;
	EMMediaTrack* opRecentTrack = NULL;

	int32 vClipID = *(static_cast<int32*>(p_upClipID));
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
	
	m_vMagicSpeedValue = opVideoFileInputDescriptor -> m_opVideoFile -> GetMagicSpeedValue();

	return static_cast<void*>(&m_vMagicSpeedValue);
}

bool MediaInfoCommandGetMagicSpeedValue::RequiresParameters()
{
	return true;
}

bool MediaInfoCommandGetMagicSpeedValue::IsUndoable()
{
	return false;
}
