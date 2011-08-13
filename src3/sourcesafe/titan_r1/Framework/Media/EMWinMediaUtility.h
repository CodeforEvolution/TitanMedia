/*******************************************************
* Creator: Richard Kronfalt
* Portability: Native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#ifndef __EM_WIN_MEDIA_UTILITY
#define __EM_WIN_MEDIA_UTILITY

#include "EMMediaGlobals.h"
#include "EMMediaFormat.h"
#include "EMMediaUtility.h"
#include "EMMIDIGlobals.h"

#include <list>
#include <map>
#include <streams.h>
#include <objbase.h>
#include <streams.h>
#include <dmusici.h>
#include <dmusicc.h>

class EMSemaphore;

class EMWinMediaUtility : public EMMediaUtility
{
public:
	~EMWinMediaUtility();
	
	int64 BytesPerFrame(const EMMediaFormat* p_opFormat);
	int64 BytesToFrames(int64 p_vBytes, const EMMediaFormat* p_opFormat);
	float FramesPerSecond(const EMMediaFormat* p_opFormat);
	int64 FramesToBytes(int64 p_vFrames, const EMMediaFormat* p_opFormat);
	int64 FramesToTime(int64 p_vFrames, const EMMediaFormat* p_opFormat);
	int64 FramesPerBuffer(const EMMediaFormat* p_opFormat);
//	int64 FramesInBuffer(BBuffer* p_opBuffer, const EMMediaFormat* p_opFormat, EMMediaType p_eReturnType);
	int64 TimeToFrames(int64 p_vTime, const EMMediaFormat* p_opFormat);
	void* GetRosterE();
//	void DisplayBuffer(BBuffer* opBuffer);
	int64 FindMaxNum(signed short* array, int64 length, int64 num);

	const char* ParseFullPath(const char* p_vpFileName, uint32 p_vDirectoryID);
	const char* GetFileName(const char* p_vpFullPath);
	const char* GetPathName(const char* p_vpFullPath);
	const char* GetStringAfterLast(const char* p_vpSourceString, char p_vDelimiter);
	const char* GetStringBeforeLast(const char* p_vpSourceString, char p_vDelimiter);
	const char* MakePeaksFileName(const char* p_vpSourceFileName);
	const char* MakeExtractFileName(const char* p_vpSourceFileName);
	const char* MakeConvertFileName(const char* p_vpSourceFileName);
	const char* MakeRenderFileName(uint32 p_vSequenceNumber);
	const char* MakeAudioRecordingFileName(uint32 p_vSequenceNumber, int32 p_vUniqueID);
	const char* MakeVideoRecordingFileName(uint32 p_vSequenceNumber, int32 p_vUniqueID);
	
	IPin* GetFirstPin(IBaseFilter* p_opFilterToExamine);

	EMMediaFormat* GetSystemAudioFormat();
	EMMediaFormat* GetSystemVideoFormat();
	
	void ClearData();
	
	//For debugging
	static list<char*>* classes;
	static list<int64>* num;
	static list<map<int64, string> >* instances;
	static void push(const void* ptr, char* name);
	static void pop(char* name);
	static void show();

	WCHAR* StringToUnicode(const char* p_vpString);
	const char* UnicodeToString(const WCHAR* p_vpName);
	
	void SetDirectMusicInstance(IDirectMusic8* p_opInstance);
	IDirectMusic8* GetDirectMusicInstance() const;
	bool DirectMusicObjectExists() const;

protected:
	EMWinMediaUtility();
	EMMediaFormat* m_opSystemAudioFormat;
	EMMediaFormat* m_opSystemVideoFormat;

	friend EMMediaUtility;
	EMSemaphore* m_opDirectMusicProtection;
	IDirectMusic8* m_opDirectMusicInstance;

	string m_oDummyString;
};

#endif
#endif
