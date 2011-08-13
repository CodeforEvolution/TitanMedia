/*******************************************************
* Creator: Richard Kronfalt
* Portability: Native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#ifndef __EM_WIN_MEDIA_FILE_MANAGER
#define __EM_WIN_MEDIA_FILE_MANAGER

#include "EMMediaGlobals.h"
#include "EMMediaFileManager.h"

class EMMediaFile;
class EMMediaFormat;
class EMWaveFileWriter;

#include <dshow.h>
#include <qedit.h>
#include <stdio.h>
#include <string>
#include "EMWinQuicktimeVideoSuper.h"
//#include <iostream.h>

class EMWinMediaFileManager : public EMMediaFileManager
{
public:
	EMWinMediaFileManager();
	~EMWinMediaFileManager();
	int32 GetFileTypeForE(const string* p_opFileName);
	bool IsWave(const string* p_opFileName);
	bool HasVideo(const string* p_opFileName);
	bool HasAudio(const string* p_opFileName);
	bool CheckFileForVideoFormat(const string* p_opFileName);
	string CreatePeaksFileE(const string* p_oFileName);
	string ExtractAudioE(const string* p_oFileName);
	bool FileExists(const string* p_opFileName);
	bool CompareNumFramesInFile(const string* p_opFileName, int64 p_vNumFramesToCompareWith);
	bool FormatsMatch(const string* p_opFileName, const EMMediaFormat* p_oFormat);
	string ConvertAudio(const string* p_oSourceFileName, const EMMediaFormat* p_opTargetFormat);
	string CopyTo(const string* p_opSourceFile, const string* p_opTargetDirectory);
	int32 DiscoverFileType(IGraphBuilder* opGraph);
	IPin* GetPin(IBaseFilter *pFilter, PIN_DIRECTION PinDir);
	int Initialize(char* p_cpInput, char* p_cpOutput);
	IPin* FindAndDiscardRenderer(EMMediaType p_eType);

	bool FindOriginalFormat(string p_oFileName, EMMediaFormat* p_opFormat);
//	WCHAR* CharToUnicode(char* p_oString);

private:
	bool PeaksFileExistsFor(const string* p_opFileName);
	bool ExtractFileExistsFor(const string* p_opFileName);
	bool ConvertFileExistsFor(const string* p_opSourceFileName);

//TEST
//	BMediaBufferDecoder			mAudioDecoder;
//	uint64						mCurrrentBuffer;
//	size_t						mBufferSize;
//	size_t						mDecodeBufferSize;
//	char						*mDecodeBuffer;

//	media_multi_audio_format	mFormat;
	EMWaveFileWriter* m_opNewFile;
//END TEST
 

private:
	HANDLE m_opConsoleHandle;
	IMediaControl*	m_opMediaControl;
	IMediaSeeking*	m_opMediaSeeking;
	IFileSinkFilter* m_opFileZink;
	IMediaEvent* m_opEvent;
    IGraphBuilder*	m_opGraph;
	AM_MEDIA_TYPE m_opType;

	//QuickTime Stuff
	EMWinQuicktimeVideoSuper* m_oQuickTimeVideo;
};

#endif

#endif
