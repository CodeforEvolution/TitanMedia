#include "CommandZoomOutHorizontal.h"

#include "EMSettingIDs.h"
#include "EMSettingsRepository.h"
#include "GUIGlobals.h"

CommandZoomOutHorizontal::CommandZoomOutHorizontal()
{
}

void CommandZoomOutHorizontal::ExecuteE()
{
	int64 vTimeScale = *(static_cast<int64*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_TIME_ZOOM_SCALE)));
	float vFramerate = *(static_cast<float*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_VIDEO_FRAMERATE)));

	switch(vTimeScale)
	{
	case ZOOM_SAMPLES_96000:
	case ZOOM_SAMPLES_48000:
	case ZOOM_SAMPLES_44100:
	case ZOOM_SAMPLES_32000:
	case ZOOM_SAMPLES_22050:
;//cout_commented_out_4_release << "Zooming out to frames resolution" << endl;
		if(vFramerate == 24)
			vTimeScale = ZOOM_FRAMES_24;
		else if(vFramerate == 25)
			vTimeScale = ZOOM_FRAMES_25;
		else if(vFramerate == 29.97)
			vTimeScale = ZOOM_FRAMES_30DROP;
		else if(vFramerate == 30)
			vTimeScale = ZOOM_FRAMES_30;
		else
//			EMDebugger("Unknown framerate");
			// FIXME: Fixed framerates should be changed to unfixed some way
			vTimeScale = ZOOM_FRAMES_24;
		break;
	case ZOOM_FRAMES_24:
	case ZOOM_FRAMES_25:
	case ZOOM_FRAMES_30DROP:
	case ZOOM_FRAMES_30:
;//cout_commented_out_4_release << "Zooming out to 1ms resolution" << endl;
		vTimeScale = ZOOM_1MS;
		break;
	case ZOOM_1MS:
;//cout_commented_out_4_release << "Zooming out to 10ms resolution" << endl;
		vTimeScale = ZOOM_10MS;
		break;
	case ZOOM_10MS:
;//cout_commented_out_4_release << "Zooming out to 100ms resolution" << endl;
		vTimeScale = ZOOM_100MS;
		break;
	case ZOOM_100MS:
;//cout_commented_out_4_release << "Zooming out to 1s resolution" << endl;
		vTimeScale = ZOOM_1S;
		break;
	case ZOOM_1S:
;//cout_commented_out_4_release << "Zooming out to 10s resolution" << endl;
		vTimeScale = ZOOM_10S;
		break;
	case ZOOM_10S:
		// Already reached max zoom out
		return;
	default:
		EMDebugger("Unknown zoom scale");
	}
	EMSettingsRepository::Instance() -> SetSetting(SETTING_TIME_ZOOM_SCALE, EM_SETTING_INT64, &vTimeScale);
}

bool CommandZoomOutHorizontal::IsUndoable()
{
	return false;
}

bool CommandZoomOutHorizontal::RequiresParameters()
{
	return false;
}


