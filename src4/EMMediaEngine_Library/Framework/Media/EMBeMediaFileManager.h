/*******************************************************
* Creator: Richard Kronfalt
* Portability: Native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifdef PLATFORM_BEOS

#ifndef __EM_BE_MEDIA_FILE_MANAGER
#define __EM_BE_MEDIA_FILE_MANAGER

//TEST
#include <Buffer.h>
#include <MediaDefs.h>
#include <MediaDecoder.h>
//END TEST

#include "EMMediaFileManager.h"
#include "BlockFIFO.h"

class EMMediaFile;
class EMMediaFormat;
class EMWaveFileWriter;

class EMBeMediaFileManager : public EMMediaFileManager
{
public:
	EMBeMediaFileManager();
	~EMBeMediaFileManager();
	int32 GetFileTypeForE(const string* p_opFileName);
	bool IsWave(const string* p_opFileName);
	bool HasVideo(const string* p_opFileName);
	bool HasAudio(const string* p_opFileName);
	bool CheckFileForVideoFormat(const string* p_opFileName);
	string CreatePeaksFileE(const string* p_oFileName);
	string ExtractAudioE(const string* p_oFileName);
	bool FileExists(const string* p_opFileName);
	int64 CompareNumFramesInFile(const string* p_opFileName, int64 p_vNumFramesToCompareWith);
	bool FormatsMatch(const string* p_opFileName, const EMMediaFormat* p_oFormat);
	string ConvertAudio(const string* p_oSourceFileName, const EMMediaFormat* p_opTargetFormat);
	string CopyTo(const string* p_opSourceFile, const string* p_opTargetDirectory);

private:
	bool PeaksFileExistsFor(const string* p_opFileName);
	bool ExtractFileExistsFor(const string* p_opFileName);
	bool ConvertFileExistsFor(const string* p_opSourceFileName);

//TEST
	BMediaBufferDecoder			mAudioDecoder;
	uint64						mCurrrentBuffer;
	size_t						mBufferSize;
	size_t						mDecodeBufferSize;

	char						*mDecodeBuffer;
	BBlockFIFO					*mFIFO;

	media_multi_audio_format	mFormat;
	EMWaveFileWriter* m_opNewFile;
//END TEST

};

#endif

#endif
