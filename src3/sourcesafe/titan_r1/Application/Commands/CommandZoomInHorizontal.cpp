#include "CommandZoomInHorizontal.h"

#include "EMSettingIDs.h"
#include "EMSettingsRepository.h"
#include "GUIGlobals.h"

CommandZoomInHorizontal::CommandZoomInHorizontal()
{
}

void CommandZoomInHorizontal::ExecuteE()
{
	int64 vTimeScale = *(static_cast<int64*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_TIME_ZOOM_SCALE)));
	float vFramerate = *(static_cast<float*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_VIDEO_FRAMERATE)));
	int32 vSamplerate = *(static_cast<int32*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_AUDIO_FRAMERATE)));

	switch(vTimeScale)
	{
	case ZOOM_SAMPLES_96000:
	case ZOOM_SAMPLES_48000:
	case ZOOM_SAMPLES_44100:
	case ZOOM_SAMPLES_32000:
	case ZOOM_SAMPLES_22050:
		// Already reached max zoom out
		break;
	case ZOOM_FRAMES_24:
	case ZOOM_FRAMES_25:
	case ZOOM_FRAMES_30DROP:
	case ZOOM_FRAMES_30:
;//cout_commented_out_4_release << "Zooming in to sample resolution" << endl;
		if(vSamplerate == 96000)
			vTimeScale = ZOOM_SAMPLES_96000;
		else if(vSamplerate == 48000)
			vTimeScale = ZOOM_SAMPLES_48000;
		else if(vSamplerate == 44100)
			vTimeScale = ZOOM_SAMPLES_44100;
		else if(vSamplerate == 32000)
			vTimeScale = ZOOM_SAMPLES_32000;
		else if(vSamplerate == 22050)
			vTimeScale = ZOOM_SAMPLES_22050;
		else
			EMDebugger("Unknown sample rate");
		break;
	case ZOOM_1MS:
;//cout_commented_out_4_release << "Zooming in to frame resolution" << endl;
		if(vFramerate == 24)
			vTimeScale = ZOOM_FRAMES_24;
		else if(vFramerate == 25)
			vTimeScale = ZOOM_FRAMES_25;
		else if(vFramerate == 29.97)
			vTimeScale = ZOOM_FRAMES_30DROP;
		else if(vFramerate == 30)
			vTimeScale = ZOOM_FRAMES_30;
		else
		{
;//cout_commented_out_4_release << "Unknown framerate: " << vFramerate << endl;
//			EMDebugger("Unknown framerate");
			// FIXME: Fixed framerates should be changed to unfixed some way
			vTimeScale = ZOOM_FRAMES_24;
		}
		break;
	case ZOOM_10MS:
;//cout_commented_out_4_release << "Zooming in to 1ms resolution" << endl;
		vTimeScale = ZOOM_1MS;
		break;
	case ZOOM_100MS:
;//cout_commented_out_4_release << "Zooming in to 10ms resolution" << endl;
		vTimeScale = ZOOM_10MS;
		break;
	case ZOOM_1S:
;//cout_commented_out_4_release << "Zooming in to 100ms resolution" << endl;
		vTimeScale = ZOOM_100MS;
		break;
	case ZOOM_10S:
;//cout_commented_out_4_release << "Zooming in to 1s resolution" << endl;
		vTimeScale = ZOOM_1S;
		break;
	default:
		EMDebugger("Unknown zoom scale");
	}
	EMSettingsRepository::Instance() -> SetSetting(SETTING_TIME_ZOOM_SCALE, EM_SETTING_INT64, &vTimeScale);
}

bool CommandZoomInHorizontal::IsUndoable()
{
	return false;
}

bool CommandZoomInHorizontal::RequiresParameters()
{
	return false;
}


