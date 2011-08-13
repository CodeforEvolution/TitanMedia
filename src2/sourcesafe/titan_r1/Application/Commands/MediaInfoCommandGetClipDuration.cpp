#include "MediaInfoCommandGetClipDuration.h"

#include "EMMediaEngine.h"
#include "EMMediaPool.h"
#include "EMMediaPoolEntry.h"
#include "EMMediaProject.h"
#include "EMMediaClipRepository.h"
#include "EMMediaClip.h"
#include "EMMediaUtility.h"

MediaInfoCommandGetClipDuration::MediaInfoCommandGetClipDuration()
{
}
 
void* MediaInfoCommandGetClipDuration::ExecuteE(void* p_upMediaClipID, void*, void*)
{
	//eo << "MediaInfoCommandGetClipDuration" << ef;
	int32 vMediaClipID = *(static_cast<int32*>(p_upMediaClipID));

	EMMediaClipRepository* opAudioClipRepository = EMMediaEngine::Instance() -> GetMediaProject() -> GetAudioClipRepository();
	EMMediaClipRepository* opVideoClipRepository = EMMediaEngine::Instance() -> GetMediaProject() -> GetVideoClipRepository();
	EMMediaClipRepository* opVideoTransitionClipRepository = EMMediaEngine::Instance() -> GetMediaProject() -> GetVideoTransitionClipRepository();
	EMMediaClipRepository* opMIDIClipRepository = EMMediaEngine::Instance() -> GetMediaProject() -> GetMIDIClipRepository();

//	if(EMMediaEngine::Instance() -> GetMediaProject() -> LockAudio())
//	{
//		if(EMMediaEngine::Instance() -> GetMediaProject() -> LockVideo())
//		{
			EMMediaClip* opClip = NULL;
			opAudioClipRepository -> LockContainer();
			try
			{
				opClip = opAudioClipRepository -> Find(vMediaClipID);
			}
			catch(...)
			{
				EMDebugger("ERROR! Exception in MediaInfoCommandGetClipDuration::ExecuteE");
			}
			opAudioClipRepository -> UnlockContainer();
			if(opClip == NULL)
			{
				opVideoClipRepository -> LockContainer();
				try
				{
					opClip = opVideoClipRepository -> Find(vMediaClipID);
				}
				catch(...)
				{
					EMDebugger("ERROR! Exception in MediaInfoCommandGetClipDuration::ExecuteE");
				}
				opVideoClipRepository -> UnlockContainer();
			}
			if(opClip == NULL)
			{
				opMIDIClipRepository -> LockContainer();
				try
				{
					opClip = opMIDIClipRepository -> Find(vMediaClipID); 
				}
				catch(...)
				{
					EMDebugger("ERROR! Exception in MediaInfoCommandGetClipDuration::ExecuteE");
				}
				opMIDIClipRepository -> UnlockContainer();
			}
			if(opClip == NULL)
			{
				opVideoTransitionClipRepository -> LockContainer();
				try
				{
					opClip = opVideoTransitionClipRepository -> Find(vMediaClipID); 
				}
				catch(...)
				{
					EMDebugger("ERROR! Exception in MediaInfoCommandGetClipDuration::ExecuteE");
				}
				opVideoTransitionClipRepository -> UnlockContainer();
			}
		
			if(opClip == NULL)
				EMDebugger("ERROR! MediaInfoCommandGetClipDuration could not find media clip ID");
 
			m_vReturnValue = EMMediaUtility::Instance() -> FramesToTime(opClip -> GetActiveLength(), EMMediaUtility::Instance() -> GetSystemAudioFormat());
//			EMMediaEngine::Instance() -> GetMediaProject() -> UnlockVideo();
//		}
//		EMMediaEngine::Instance() -> GetMediaProject() -> UnlockAudio();
//	}

	//;//cout_commented_out_4_release << "MediaInfoCommandGetClipDuration returning!" << endl;
	return static_cast<void*>(&m_vReturnValue);
}

bool MediaInfoCommandGetClipDuration::RequiresParameters()
{
	return true;
}

bool MediaInfoCommandGetClipDuration::IsUndoable()
{
	return false;
}