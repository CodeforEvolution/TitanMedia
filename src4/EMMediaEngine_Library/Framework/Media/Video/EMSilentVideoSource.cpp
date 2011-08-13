#include "EMSilentVideoSource.h"

#include "EMMediaDataBuffer.h"
#include "EMMediaFormat.h"
#include "EMMediaUtility.h"
#include "EMMediaTimer.h"
#include "EMVideoSynchronizer.h"
#include "EMMediaEngine.h"
#include "EMSettingIDs.h"
#include "EMSettingsRepository.h"
#include "EMMessages.h"
#include "EMMediaProject.h"

EMSilentVideoSource::EMSilentVideoSource()
	:	EMMediaBufferSource(EM_TYPE_RAW_VIDEO),
	m_oFormat(EM_TYPE_RAW_VIDEO),
	m_opTrack(NULL),
	m_vNeedToSyncWithTime(false)
{
	EMMediaTimer::Instance() -> AddListener(this);
	EMSettingsRepository::Instance() -> AddListener(this);
}

EMSilentVideoSource::~EMSilentVideoSource()
{
	EMMediaTimer::Instance() -> RemoveListener(this);
	EMSettingsRepository::Instance() -> RemoveListener(this);
}

bool EMSilentVideoSource::InitCheckE(EMMediaFormat* p_opFormat)
{
	//Fix so that EMMediaBufferSource::InitCheckE implements EM_TYPE_RAW_VIDEO also!

	if(!(m_oFormat == *p_opFormat))
	{
		m_oFormat = *p_opFormat;
//		EMMediaFormat* opFormat = EM_new EMMediaFormat(EM_TYPE_ANY_VIDEO);

		if(! EMMediaBufferSource::InitCheckE(&m_oFormat))
		{
//			delete opFormat;
			return false;
		}
//		delete opFormat;
	}

	if(EMSettingsRepository::Instance() -> SettingExists(SETTING_VIDEO_BACKGROUND_COLOR))
	{
		m_vColor = *(static_cast<int32*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_VIDEO_BACKGROUND_COLOR)));

		m_vR = (m_vColor & 0xff0000) >> 16;
		m_vG = (m_vColor & 0xff00) >> 8;
		m_vB = (m_vColor & 0xff);
	}
	else
	{
		m_vColor = 0xffffff;

		m_vR = 0xff;
		m_vG = 0xff;
		m_vB = 0xff;
	}

	;//emout_commented_out_4_release << "OK, Initialized!" << endl;

	SetBufferSilence(true);

//	m_opBuffer -> m_vIsSilence = true; //Set the parent's flag so our buffer is stamped "silent"
//	m_opBuffer -> m_vSizeUsed = m_opBuffer -> m_vSizeAvailable;
//	m_opBuffer -> m_sFormat.type = B_MEDIA_RAW_VIDEO;
//	m_opBuffer -> m_sFormat.u.raw_video.field_rate = EM_VIDEO_FRAME_RATE; //Should not be static but read from settings
//	m_opBuffer -> m_sFormat.u.raw_video.display.line_width = 720;//Non static get from setings
//	m_opBuffer -> m_sFormat.u.raw_video.display.line_width = 576;//Non static get from setings
//	m_opBuffer -> m_sFormat.u.raw_video.display.format = B_RGB32;//Non static get from settings (color_space)
//	m_opBuffer -> m_sFormat.u.raw_video.display.bytes_per_row = 720*8;//Non static get from settings???

//	m_opBuffer -> m_sFormat.u.raw_video.orientation = B_VIDEO_TOP_LEFT_RIGHT;
//	m_opBuffer -> m_sFormat.u.raw_video.pixel_width_aspect=1; //PAL , 4=NTSC,, should be set through settings
//	m_opBuffer -> m_sFormat.u.raw_video.pixel_height_aspect=1; //PAL , 3=NTSC, should be set through settings
//	m_opBuffer -> m_sFormat.u.raw_video.interlace=1; //1 = NON-Interlace 2 = Interlace (NTSC)!!
//	memset(m_opBuffer -> Data(), 255, m_opBuffer -> m_vSizeAvailable); //Currently white, should be chnaged through settings

	return true;
}
/*
EMMediaDataBuffer* EMSilentVideoSource::GetBuffer() const
{
//	if(! m_opBuffer -> m_vAvailable)
//		EMDebugger("Tried to allocate silent EMMediaDataBuffer that was in use!");
	m_opBuffer -> m_vAvailable = false;
	m_opBuffer -> m_vSizeUsed = m_opBuffer -> m_vSizeAvailable;
	m_opBuffer -> m_vIsSilence = true;
	memset(m_opBuffer -> Data(), 255, m_opBuffer -> m_vSizeAvailable); //Currently white, should be chnaged through settings
	return m_opBuffer;
}
*/

EMMediaTrack* EMSilentVideoSource::GetTrack() const
{
	return m_opTrack;
}

EMMediaDataBuffer* EMSilentVideoSource::GetBufferAt(int64 p_vTime, bool p_vSeeking)
{
	int64 vLen;

	if(p_vTime >= 220500)
	{
		int unkle = 0;
	}
	float vAudioFrameRate = (float) *(static_cast<int32*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_AUDIO_FRAMERATE)));
	float vWorkFPS = *(static_cast<float*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_VIDEO_FRAMERATE)));
	int64 vAudioFrames = static_cast<int64>(vAudioFrameRate / vWorkFPS);

	EMMediaDataBuffer* opBuffer = GetBuffer();

	if(opBuffer == NULL)
	{
		RecycleAllBuffers();
		opBuffer = GetBuffer();
	}

	opBuffer -> SetFrame(EMMediaUtility::Instance() -> FramesToTime(p_vTime + 0*vAudioFrames, EMMediaUtility::Instance() -> GetSystemAudioFormat()));
	opBuffer -> SetRelativeTimeToStart(EMMediaUtility::Instance() -> FramesToTime(p_vTime + 0*vAudioFrames, EMMediaUtility::Instance() -> GetSystemAudioFormat()));
	
	//memset(opBuffer -> Data(), 255, opBuffer -> m_vSizeAvailable); //Currently white, should be changed 

	vLen = opBuffer -> m_vSizeAvailable;
	unsigned char* opData = static_cast<unsigned char*>(opBuffer -> Data());
	for(int64 i = 0; i < vLen; i += 3)
	{
		*(opData++) = m_vB;
		*(opData++) = m_vG;
		*(opData++) = m_vR;
	}

	opBuffer -> m_vSizeUsed = 	opBuffer -> m_vSizeAvailable;
/*	if(m_vNeedToSyncWithTime)
	{
		opBuffer -> m_vDiscon = true;
		m_vNeedToSyncWithTime = false;
	}
	else
		opBuffer -> m_vDiscon = p_vSeeking;
*/
	opBuffer -> m_vIsSilence = true;

	opBuffer -> m_vBelongsToUniqueID = this -> GetID();

//	EMVideoSynchronizer::Instance() -> SetRelativeOffsetTime(EMMediaUtility::Instance() -> FramesToTime(p_vTime, EMMediaUtility::Instance() -> GetSystemAudioFormat()));
	//EMVideoSynchronizer::Instance() -> ZeroRealTimeClock();
/*	if(p_vSeeking || EMVideoSynchronizer::Instance() -> NeedsToSynch())
	{
		EMVideoSynchronizer::Instance() -> ZeroRealTimeClock();
		//EMVideoSynchronizer::Instance() -> SetRelativeOffsetTime(opBuffer -> GetRelativeTimeToStartForSynchClock());
		//EMVideoSynchronizer::Instance() -> SetRelativeOffsetTime(opBuffer -> GetFrame() + vTime);
		EMVideoSynchronizer::Instance() -> SetNeedsToSynch(false);
	}*/

	return opBuffer;
}

void EMSilentVideoSource::SetTrack(EMMediaTrack* p_opTrack)
{
	m_opTrack = p_opTrack;
}

bool EMSilentVideoSource::MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage)
{
	switch(p_vMessage)
	{
		case EM_MESSAGE_TIME_WAS_SEEKED:
			break;
		case EM_MESSAGE_STOP_PLAYORRECORD:
			break;
		case EM_MESSAGE_BEGIN_PLAYORRECORD:
			break;
		case EM_SETTING_UPDATED:
			if(EMMediaEngine::Instance() -> GetSettingsRepository() -> SettingExists(SETTING_VIDEO_BACKGROUND_COLOR))
			{
				m_vColor = *(static_cast<int32*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_VIDEO_BACKGROUND_COLOR)));

				m_vR = (m_vColor & 0xff0000) >> 16;
				m_vG = (m_vColor & 0xff00) >> 8;
				m_vB = (m_vColor & 0xff);

				EMMediaFormat oMediaFormat(EM_TYPE_RAW_VIDEO);
				oMediaFormat.m_vWidth = *(static_cast<int*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_VIDEO_FORMAT_WIDTH)));//(m_opVideoPreview -> Bounds()).GetHeight();
				oMediaFormat.m_vHeight  = *(static_cast<int*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_VIDEO_FORMAT_HEIGHT)));//(m_opVideoPreview -> Bounds()).GetWidth();
				oMediaFormat.m_vBytesPerSample = 3;

				if(m_vOldWidth != oMediaFormat.m_vWidth || m_vOldheight != oMediaFormat.m_vHeight)
				{
					oMediaFormat.m_vBytesPerRow  = oMediaFormat.m_vWidth*3;
					oMediaFormat.m_vFrameRate = *(static_cast<float*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_VIDEO_FRAMERATE)));
					EMMediaBufferSource::InitCheckBufferE(&oMediaFormat);
				}
				m_vOldWidth = oMediaFormat.m_vWidth;
				m_vOldheight = oMediaFormat.m_vHeight;
			}
			break;
		case EM_MEDIA_ENGINE_VIDEO_WORKSIZE_CHANGED:
			{
				//POSSIBLE MEMORY LEAK
				EMMediaFormat oMediaFormat(EM_TYPE_RAW_VIDEO);
				if(EMMediaEngine::Instance() -> GetMediaProject() -> IsRenderingVideo())
				{
					oMediaFormat.m_vWidth = *(static_cast<int*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_VIDEO_RENDER_FORMAT_WIDTH)));//(m_opVideoPreview -> Bounds()).GetHeight();
					oMediaFormat.m_vHeight  = *(static_cast<int*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_VIDEO_RENDER_FORMAT_HEIGHT)));//(m_opVideoPreview -> Bounds()).GetWidth();
					oMediaFormat.m_vBytesPerSample = 3;
					
					while(((oMediaFormat.m_vHeight * oMediaFormat.m_vBytesPerSample) % 4) != 0)
						oMediaFormat.m_vHeight--;
					
					if(m_vOldWidth != oMediaFormat.m_vWidth || m_vOldheight != oMediaFormat.m_vHeight)
					{
						oMediaFormat.m_vBytesPerRow  = oMediaFormat.m_vWidth*3;
						oMediaFormat.m_vFrameRate = *(static_cast<float*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_VIDEO_RENDER_FRAMERATE)));
						EMMediaBufferSource::InitCheckBufferE(&oMediaFormat);
					}
				}
				else
				{
					oMediaFormat.m_vWidth = *(static_cast<int*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_VIDEO_FORMAT_WIDTH)));//(m_opVideoPreview -> Bounds()).GetHeight();
					oMediaFormat.m_vHeight  = *(static_cast<int*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_VIDEO_FORMAT_HEIGHT)));//(m_opVideoPreview -> Bounds()).GetWidth();
					oMediaFormat.m_vBytesPerSample = 3;

					while(((oMediaFormat.m_vHeight * oMediaFormat.m_vBytesPerSample) % 4) != 0)
						oMediaFormat.m_vHeight--;


					if(m_vOldWidth != oMediaFormat.m_vWidth || m_vOldheight != oMediaFormat.m_vHeight)
					{
						oMediaFormat.m_vBytesPerRow  = oMediaFormat.m_vWidth*3;
						oMediaFormat.m_vFrameRate = *(static_cast<float*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_VIDEO_FRAMERATE)));
						EMMediaBufferSource::InitCheckBufferE(&oMediaFormat);
					}
				}
				m_vOldWidth = oMediaFormat.m_vWidth;
				m_vOldheight = oMediaFormat.m_vHeight;
			}			 
			break;
		default:
			break;
	};
	
	return true;
}
