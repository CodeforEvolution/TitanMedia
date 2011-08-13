#include "MediaCommandRefreshPreview.h"

#include "EMMediaEngine.h"
#include "EMMediaProject.h"
#include "EMMediaTrack.h"
#include "EMMediaClipRepository.h"
#include "EMMediaClip.h"
#include "EMMediaTimer.h"
#include "EMMediaFormat.h"
#include "EMMediaTrackRepository.h"
#include "EMMediaUtility.h"
#include "EMMediaVideoTransitionClip.h"

MediaCommandRefreshPreview::MediaCommandRefreshPreview()
{
}

void MediaCommandRefreshPreview::ExecuteE()
{
	EMMediaProject* opProject = EMMediaEngine::Instance() -> GetMediaProject();
	if(!opProject->IsPlaying())
	{
//		EMMediaTimer::Instance() -> SeekToFrame(EMMediaTimer::Instance() -> VideoThenFrame());
		EMMediaEngine::Instance() -> GetMediaProject() -> PlaySingleBuffer();
	}
}

bool MediaCommandRefreshPreview::RequiresParameters()
{
	return false;
}

bool MediaCommandRefreshPreview::IsUndoable()
{
	return false;
}

EMCommand* MediaCommandRefreshPreview::CloneAndClear()
{
	return EM_new MediaCommandRefreshPreview();
}

/*void MediaCommandRefreshPreview::UndoE()
{
}*/
