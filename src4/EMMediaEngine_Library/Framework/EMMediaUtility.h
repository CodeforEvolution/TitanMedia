#ifndef __EM_MEDIA_UTILITY
#define __EM_MEDIA_UTILITY

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

class EMMediaFormat;

class __declspec(dllexport) EMMediaUtility
{
public:
	static EMMediaUtility* Instance();
	static void Delete();

	virtual int64 BytesPerFrame(const EMMediaFormat* p_opFormat) = 0;
	virtual int64 BytesToFrames(int64 p_vBytes, const EMMediaFormat* p_opFormat) = 0;
	virtual float FramesPerSecond(const EMMediaFormat* p_opFormat) = 0;
	virtual int64 FramesToBytes(int64 p_vFrames, const EMMediaFormat* p_opFormat) = 0;
	virtual int64 FramesToTime(int64 p_vFrames, const EMMediaFormat* p_opFormat) = 0;
	virtual int64 FramesPerBuffer(const EMMediaFormat* p_opFormat) = 0;
//	virtual int64 FramesInBuffer(void* p_opBuffer, const EMMediaFormat* p_opFormat, EMMediaType p_eReturnType) = 0;
	virtual int64 TimeToFrames(int64 p_vTime, const EMMediaFormat* p_opFormat) = 0;
//	virtual BMediaRoster* GetRosterE() = 0;
//	virtual void DisplayBuffer(BBuffer* opBuffer) = 0;
	virtual int64 FindMaxNum(signed short* array, int64 length, int64 num) = 0;

	virtual const char* ParseFullPath(const char* p_vpFileName, uint32 p_vDirectoryID) = 0;
	virtual const char* GetFileName(const char* p_vpFullPath) = 0;
	virtual const char* GetPathName(const char* p_vpFullPath) = 0;
	virtual const char* GetStringAfterLast(const char* p_vpSourceString, char p_vDelimiter) = 0;
	virtual const char* GetStringBeforeLast(const char* p_vpSourceString, char p_vDelimiter) = 0;
	virtual const char* MakePeaksFileName(const char* p_vpSourceFileName) = 0;
	virtual const char* MakeExtractFileName(const char* p_vpSourceFileName) = 0;
	virtual const char* MakeConvertFileName(const char* p_vpSourceFileName) = 0;
	virtual const char* MakeRenderFileName(uint32 p_vSequenceNumber) = 0;
	virtual const char* MakeAudioRecordingFileName(uint32 p_vSequenceNumber, int32 p_vUniqueID) = 0;
	virtual const char* MakeVideoRecordingFileName(uint32 p_vSequenceNumber, int32 p_vUniqueID) = 0;
	
	virtual EMMediaFormat* GetSystemAudioFormat() = 0;
	virtual EMMediaFormat* GetSystemVideoFormat() = 0;

protected:
	EMMediaUtility();
	~EMMediaUtility();
	static EMMediaUtility* m_opInstance;
};

#endif