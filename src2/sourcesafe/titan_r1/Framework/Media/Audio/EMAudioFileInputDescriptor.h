/*******************************************************
* Creator: Richard Kronfalt
* Portability: Non-Native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#ifndef __EM_AUDIO_FILE_INPUT_DESCRIPTOR
#define __EM_AUDIO_FILE_INPUT_DESCRIPTOR

#include "EMFileInputDescriptor.h"
#include "EMWaveFileReader.h"

class EMMediaFormat;

class EMAudioFileInputDescriptor : public EMFileInputDescriptor
{
public:
	EMAudioFileInputDescriptor(string p_oFileName);
	~EMAudioFileInputDescriptor();
	EMMediaFormat* GetFormat();
	bool InitCheckE();
	bool ReadFrames(EMMediaDataBuffer** p_opPrimaryBuffer, EMMediaDataBuffer** p_opSecondaryBuffer, int64 p_vMediaFramePosition, int64 p_vOffsetFrames, int64 & p_vOutNumRead, bool p_vSeeking = false);
	bool SeekTo(int64 p_vNewMediaFrame);
	bool SeekAndDisplay(int64 p_vNewMediaFrame);

	bool Open();
	bool Close();

private:
	EMWaveFileReader* m_opWaveFile;
	EMMediaFormat* m_opSystemAudioFormat;

private:
	bool m_vIsClosed;
};

#endif

#endif
