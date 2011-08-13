/*******************************************************
* Creator: Richard Kronfalt
* Portability: Non-Native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_MEDIA_FILE_MANAGER
#define __EM_MEDIA_FILE_MANAGER

#include <string>

class EMMediaFormat;
class EMMediaFile;
class EMWaveFile;
class EMSemaphore;

#include "EMWinQuicktimeVideoSuper.h"

class EMMediaFileManager 
{
public:
	virtual ~EMMediaFileManager();
	static EMMediaFileManager* Instance();
	static void Delete();
	int32 GetID() const;
	virtual int32 GetFileTypeForE(const string* p_opFileName) = 0; 
//	virtual bool IsWave(const string* p_opFileName) = 0;
//	virtual bool HasVideo(const string* p_opFileName) = 0;
//	virtual bool HasAudio(const string* p_opFileName) = 0;
	virtual string CreatePeaksFileE(const string* p_opFileName) = 0;
	virtual string ExtractAudioE(const string* p_oFileName) = 0;
	virtual bool CheckFileForVideoFormat(const string* p_opFileName) = 0;
	virtual bool FormatsMatch(const string* p_opFileName, const EMMediaFormat* p_oFormat) = 0;
	virtual string ConvertAudio(const string* p_oSourceFileName, const EMMediaFormat* p_opTargetFormat) = 0;
	virtual string CopyTo(const string* p_opSourceFile, const string* p_opTargetDirectory) = 0;

	static void Lock();
	static void Unlock();

protected:
	EMMediaFileManager();

private:
	static EMMediaFileManager* m_opInstance;
	int32 m_vID;
	static EMSemaphore* m_opSemaphore;

};

#endif
