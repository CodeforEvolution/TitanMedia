/*******************************************************
* Creator: Jesper Svensson
* Portability: Non-Native
*-------------------------------------------------------
* 
*******************************************************/

#ifndef __EM_WIN_QUICKTIME_WRITER
#define __EM_WIN_QUICKTIME_WRITER

//#include "QTML.h"
//#include "Movies.h"
//#include "TextUtils.h"
//#include "Quickdraw.h"
#include "QuickTimeComponents.h"
#include <string>
#include "EMScalingUtils.h"

//#include <FixMath.h>

class EMMediaFormat;
class EMMediaDataBuffer;
enum EMMediaType;

class EMWinQuickTimeWriter
{
public:
	EMWinQuickTimeWriter();
	~EMWinQuickTimeWriter();
	static EMWinQuickTimeWriter* Instance();
	void SetRenderName(char* p_vpName);
	bool InitCheckE(EMMediaType p_vMediaType);
	void GetAudioEncoderIDs(unsigned long* p_vpIDs);
	void GetVideoEncoderIDs(unsigned long* p_vpIDs);
	int GetNumberOfVideoEncoders();
	int GetNumberOfAudioEncoders();
	void SetShuffleFormatForVideo(EMMediaFormat* p_opMediaFormat);
	void SetShuffleFormatForAudio(EMMediaFormat* p_opMediaFormat);
	void SetAudioEncoderToUse(long p_vEncoderID);
	void SetVideoEncoderToUse(long p_vEncoderID);
	void DisConnectWithEncoders();
	void StopWritingToDisk();
	void ProcessVideoBuffer(EMMediaDataBuffer* p_opDataBuffer);
	void ProcessAudioBuffer(EMMediaDataBuffer* p_opDataBuffer);
	void SetCompressionCapabilityInfo(long p_vEncoderID, long  p_vPropertyID);
	char* GetCompressionCapabilities(long p_vEncoderID);
	std::string GetEncoderName(long p_vEncoderID);
	bool GetEncoderPropertyPages(long p_vEncoderID, bool p_vShouldShow);
	bool ExtractAudioFromQuickTimeFile(const char* p_opSource, const char* p_opDestination);
private:
	void AddVideoSamplesToMedia (Media theMedia, GWorldPtr p_opGWorld, void* p_vBitmapDataPtr, OSType p_cType);

private:
	EMScalingUtils	m_oScale;
	static EMWinQuickTimeWriter* m_opInstance;
	char* m_oOutputFileName;
	EMMediaFormat* m_opMediaFormatForVideo;
	EMMediaFormat* m_opMediaFormatForAudio;
	long m_vEncoderID;

	Track m_oMovieExportTrack;
	Media m_oMovieExportMedia;
	Track m_oSoundExportTrack;
	Media m_oSoundExportMedia;

	Movie m_oExportMovie;		// Movie identifier
	Rect m_sTrackFrame;
	CodecNameSpec m_sCurrentCodec;
	short m_vFileRefNum;
	GWorldPtr m_oGWorld;
	FSSpec m_fileExportSpec;

	SoundConverter m_oSC;
	SoundDescriptionHandle m_sSndDesc;
	unsigned long m_vInputSoundFrames;
	unsigned long m_vInputSoundBytes;
	unsigned long m_vOutputSoundBytes;
	Ptr	m_vpSoundDataDestPtr;
	unsigned long m_vSampleRate;
	int m_vSampleSize;
	int m_vNumChannels;
	std::string* m_opAudioCompressionCapabilityArray;
	OSType m_vCurrentVideoCodecToUse;
	OSType m_vCurrentAudioCodecToUse;
	CompressionInfo m_vDestCompInfo;
	SoundDescriptionV1Handle	m_mySampleDesc;
	long m_maxCompressedSize;
	Handle m_pCompressedData;
	Ptr  m_pCompressedDataPtr;
	ImageDescriptionHandle m_pImageDesc;
	int m_vVideoTimeScale;
	double m_vSampleDuration;
	Handle m_VideoOptionSettingsHandle;
	Rect m_VideoOptionSettingsRect;
	unsigned long m_vQuality;
};

#endif