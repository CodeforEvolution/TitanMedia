#include "MediaCommandSeek.h"

//#include "EMMediaEngine.h"
//#include "EMMediaProject.h"
#include "EMMediaTimer.h"
#include "EMBeMediaTimer.h"
#include "EMMediaEngine.h"
#include "EMMediaProject.h"
#include "EMSettingIDs.h"
#include "EMSettingsRepository.h"
#include "EMMediaClipRepository.h"
#include "EMMediaFormat.h"

MediaCommandSeek::MediaCommandSeek()
{
}

void* MediaCommandSeek::ExecuteE(void* p_opParameterOne, void* p_opParameterTwo, void* p_opParameterThree)
{
	int64* vpNewTime = static_cast<int64*>(p_opParameterOne);
	float vAudioFrameRate = (float) *(static_cast<int32*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_AUDIO_FRAMERATE)));
	int64 vNewFrame = static_cast<int64>((*vpNewTime) * (vAudioFrameRate) / 1000000.0);
	EMMediaTimer::Instance() -> SeekToFrame(vNewFrame);
	return NULL;
}

bool MediaCommandSeek::RequiresParameters()
{
	return true;
}

bool MediaCommandSeek::IsUndoable()
{
	return false;
}