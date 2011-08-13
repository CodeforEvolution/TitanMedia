/*******************************************************
* Creator: Richard Kronfalt
* Portability: Native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifdef PLATFORM_BEOS

#ifndef __EM_BE_AUDIO_FILE_INPUT_DESCRIPTOR
#define __EM_BE_AUDIO_FILE_INPUT_DESCRIPTOR

#include "EMBeFileInputDescriptor.h"
#include "EMWaveFileReader.h"

class EMMediaFormat;

class EMBeAudioFileInputDescriptor : public EMBeFileInputDescriptor
{
public:
	EMBeAudioFileInputDescriptor(string p_oFileName);
	~EMBeAudioFileInputDescriptor();
	EMMediaFormat* GetFormat();
	bool InitCheckE();
	bool ReadFrames(EMMediaDataBuffer** p_opPrimaryBuffer, EMMediaDataBuffer** p_opSecondaryBuffer, int64 p_vMediaFramePosition, int64 p_vOffsetFrames, int64 & p_vOutNumRead, bool p_vSeeking = false);
	bool SeekTo(int64 p_vNewMediaFrame);
	bool SeekAndDisplay(int64 p_vNewMediaFrame);

private:
	EMWaveFileReader* m_opWaveFile;
	EMMediaFormat* m_opSystemAudioFormat;
};

#endif

#endif
