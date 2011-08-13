#include "EMMediaFormat.h"
#include "EMMediaGlobals.h"
#include "EMGlobals.h"
#include "EMGUIGlobals.h"
#include "EMBeMediaUtility.h"
#include "EMSettingsRepository.h"
#include "EMSettingIDs.h"
#include "EMMediaEngine.h"

EMMediaFormat::EMMediaFormat(EMMediaType p_eType)
	:	m_eType(p_eType)
{
	if((p_eType & EM_TYPE_ANY_AUDIO) > 0)
	{
		if(EMMediaEngine::Instance() -> GetSettingsRepository() != NULL)
		{
			m_vFrameRate = (float) *(static_cast<int32*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_AUDIO_FRAMERATE)));
			m_vBufferSizeBytes = static_cast<int64>(*static_cast<int32*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_AUDIO_BUFFER_SIZE)));
			m_vNumChannels = EM_AUDIO_NUM_CHANNELS;
			m_vBytesPerSample = EM_AUDIO_SAMPLESIZE;
		}
		else
		{
			m_vFrameRate = 0;
			m_vBufferSizeBytes = 0;
			m_vNumChannels = 0;
			m_vBytesPerSample = 0;
		}
	}
	else if((p_eType & EM_TYPE_ANY_VIDEO) > 0)
	{
		if(EMMediaEngine::Instance() -> GetSettingsRepository() != NULL)
		{
			m_vFrameRate = *(static_cast<float*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_VIDEO_FRAMERATE)));
			m_vWidth = *(static_cast<int*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_VIDEO_FORMAT_WIDTH)));
			m_vHeight = *(static_cast<int*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_VIDEO_FORMAT_HEIGHT)));
			m_vDepth = 3;
			m_vBytesPerRow = m_vWidth * 3;
			m_vBufferSizeBytes = m_vWidth * m_vHeight *3;
		}
		else
		{
			m_vFrameRate = 0;
			m_vWidth = 0;
			m_vHeight = 0;
			m_vDepth = 0;
			m_vBytesPerRow = 0;
			m_vBufferSizeBytes = 0;
		}
	}
	else if((p_eType & EM_TYPE_MIDI) > 0)
	{
	} 
	else if((p_eType & EM_TYPE_ANY_IMAGE) > 0)
	{
			m_vFrameRate = 0;
			m_vWidth = 0;
			m_vHeight = 0;
			m_vDepth = 0;
			m_vBytesPerRow = 0;
			m_vBufferSizeBytes = 0;		
	}
	else
		EMDebugger("ERROR! EMMediaFormat can only handle audio and video formats so far!");

}

EMMediaFormat::EMMediaFormat(const EMMediaFormat& p_oFormat)
	:	m_eType(p_oFormat.m_eType),
		m_vFrameRate(p_oFormat.m_vFrameRate),
		m_vWidth(p_oFormat.m_vWidth),
		m_vHeight(p_oFormat.m_vHeight),
		m_vDepth(p_oFormat.m_vDepth),
		m_vBufferSizeBytes(p_oFormat.m_vBufferSizeBytes),
		m_vNumChannels(p_oFormat.m_vNumChannels),
		m_vBytesPerSample(p_oFormat.m_vBytesPerSample),
		m_vBytesPerRow(p_oFormat.m_vBytesPerRow)
{
}

EMMediaFormat::~EMMediaFormat()
{
}

void EMMediaFormat::Copy(const EMMediaFormat* p_opFormat)
{
	m_eType = p_opFormat -> m_eType;
	m_vFrameRate = p_opFormat -> m_vFrameRate;
	m_vBufferSizeBytes = p_opFormat -> m_vBufferSizeBytes;
	m_vNumChannels = p_opFormat -> m_vNumChannels;
	m_vBytesPerSample = p_opFormat -> m_vBytesPerSample;
	m_vWidth = p_opFormat -> m_vWidth;
	m_vHeight = p_opFormat -> m_vHeight;
	m_vDepth = p_opFormat -> m_vDepth;
	m_vBytesPerRow = p_opFormat -> m_vBytesPerRow;
}

void EMMediaFormat::Display() const
{
	bool vIsAudio = ((m_eType & EM_TYPE_ANY_AUDIO) > 0);
	bool vIsVideo = ((m_eType & EM_TYPE_ANY_VIDEO) > 0);
	
	eo << ">> FORMAT INFO: Type=" << (vIsAudio ? "Audio" : (vIsVideo ? "Video" : "Unknown")) << ef;
	if((m_eType & EM_TYPE_ANY_AUDIO) > 0)
	{
		eo << ">> FORMAT INFO: Fps=" << m_vFrameRate << ef;
		eo << ">> FORMAT INFO: Buffsize=" << (int) m_vBufferSizeBytes << ef;
		eo << ">> FORMAT INFO: Channels=" << m_vNumChannels << ef;
		eo << ">> FORMAT INFO: bps=" << (int) m_vBytesPerSample * 8 << ef;
	}
	else if((m_eType & EM_TYPE_ANY_VIDEO) > 0)
	{
		;//cout_commented_out_4_release << ", Fps=" << m_vFrameRate << flush;
		;//cout_commented_out_4_release << ", Width=" << m_vWidth << flush;
		;//cout_commented_out_4_release << ", Height=" << m_vHeight << flush;
		;//cout_commented_out_4_release << ", Depth=" << m_vDepth << flush;
		;//cout_commented_out_4_release << ", BperRow=" << m_vBytesPerRow << endl;
	}
}

bool EMMediaFormat::operator==(const EMMediaFormat& p_oOther)
{
	bool vSumFormat = false;
	if(m_eType != p_oOther.m_eType)
		return false;
	
	if((m_eType & EM_TYPE_ANY_AUDIO) > 0) 
	{
		if(m_vFrameRate != p_oOther.m_vFrameRate)
			return false;
		if(m_vBufferSizeBytes != p_oOther.m_vBufferSizeBytes)
			return false;
		if(m_vNumChannels != p_oOther.m_vNumChannels)
			return false;
		if(m_vBytesPerSample != p_oOther.m_vBytesPerSample)
			return false;
		
		vSumFormat = true;
	}

	if((m_eType & EM_TYPE_ANY_VIDEO) > 0)
	{
		if(m_vFrameRate != p_oOther.m_vFrameRate)
			return false;
		if(m_vWidth != p_oOther.m_vWidth)
			return false;
		if(m_vHeight != p_oOther.m_vHeight)
			return false;
		if(m_vDepth != p_oOther.m_vDepth)
			return false;
		if(m_vBytesPerRow != p_oOther.m_vBytesPerRow)
			return false;
		
		vSumFormat = true;
	}

	if((m_eType & EM_TYPE_ANY_IMAGE) > 0)
	{
		if(m_vWidth != p_oOther.m_vWidth)
			return false;
		if(m_vHeight != p_oOther.m_vHeight)
			return false;
		if(m_vDepth != p_oOther.m_vDepth)
			return false;
		if(m_vBytesPerRow != p_oOther.m_vBytesPerRow)
			return false;
		
		vSumFormat = true;
	}

	if(! vSumFormat)
	{
		EMDebugger("ERROR! What the fuck?! Can't seem to decide which format 'I' am!");
		return false;
	}
	
	return true;
}


#ifdef PLATFORM_BEOS

void EMMediaFormat::CreateFormat(const media_format* p_sFormat)
{
	if(p_sFormat -> type == B_MEDIA_RAW_AUDIO)
	{
		m_eType = EM_TYPE_RAW_AUDIO;
		m_vFrameRate = p_sFormat -> u.raw_audio.frame_rate;
		m_vBufferSizeBytes = p_sFormat -> u.raw_audio.buffer_size;
		m_vNumChannels = p_sFormat -> u.raw_audio.channel_count;
		m_vBytesPerSample = (p_sFormat -> u.raw_audio.format & 0xf);
	}
	else if(p_sFormat -> type == B_MEDIA_RAW_VIDEO)
	{
		m_eType = EM_TYPE_RAW_VIDEO;
		m_vFrameRate = p_sFormat -> u.raw_video.field_rate;
		m_vWidth = p_sFormat -> u.raw_video.display.line_width;
		m_vHeight = p_sFormat -> u.raw_video.display.line_count;
		m_vDepth = p_sFormat -> ColorSpace(); //TODO: Fix!?
		m_vBytesPerRow = p_sFormat -> u.raw_video.display.bytes_per_row;
		m_vBufferSizeBytes = p_sFormat -> u.raw_video.display.bytes_per_row * p_sFormat -> u.raw_video.display.line_count;
	}
}

#endif

#ifdef PLATFORM_WINDOWS

void EMMediaFormat::CreateAudioFormat(const AM_MEDIA_TYPE* p_sFormat)
{
	WAVEFORMATEX* pAudioHeader = (WAVEFORMATEX*)p_sFormat;
	m_eType = EM_TYPE_RAW_AUDIO;
	m_vFrameRate = pAudioHeader -> nSamplesPerSec;
	m_vBufferSizeBytes = static_cast<int64>(*static_cast<int32*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_AUDIO_BUFFER_SIZE)));
	m_vNumChannels = pAudioHeader -> nChannels; //p_sFormat -> u.raw_audio.channel_count;
	m_vBytesPerSample = pAudioHeader -> wBitsPerSample / 8; //(p_sFormat -> u.raw_audio.format & 0xf);
}

void EMMediaFormat::CreateVideoFormat(const AM_MEDIA_TYPE* p_sFormat)
{
	// Get a pointer to the video header.
	VIDEOINFOHEADER* pVideoHeader = (VIDEOINFOHEADER*)p_sFormat->pbFormat;

	m_eType = EM_TYPE_RAW_VIDEO;
	m_vFrameRate = pVideoHeader -> AvgTimePerFrame == 0 ? 24 : (10000000 / pVideoHeader->AvgTimePerFrame); //p_sFormat -> u.raw_video.field_rate;
	m_vWidth = pVideoHeader -> bmiHeader.biWidth;//p_sFormat -> u.raw_video.display.line_width;
	m_vHeight = pVideoHeader -> bmiHeader.biHeight;//p_sFormat -> u.raw_video.display.line_count;
	m_vDepth = (pVideoHeader -> bmiHeader.biBitCount/8);//p_sFormat -> ColorSpace(); //TODO: Fix!?
	m_vBufferSizeBytes = m_vWidth*m_vHeight*m_vDepth;//p_sFormat -> u.raw_video.display.bytes_per_row * p_sFormat -> u.raw_video.display.line_count;
	m_vBytesPerRow = m_vBufferSizeBytes/m_vHeight;
}

#endif
