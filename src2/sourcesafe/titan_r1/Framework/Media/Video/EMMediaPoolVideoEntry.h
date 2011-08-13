/*******************************************************
* Creator: Richard Kronfalt
* Portability: Non-Native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_MEDIA_POOL_VIDEO_ENTRY
#define __EM_MEDIA_POOL_VIDEO_ENTRY

#include "EMMediaGlobals.h"
#include "EMMediaPoolEntry.h"
#include "EMBitmap.h"
#include "EMWinBitmap.h"

#include "EMVideoFile.h"
#include "EMWinVideoFile.h"

#include <string>

class EMProjectDataLoader;
class EMProjectDataSaver;

class EMMediaPoolVideoEntry : public EMMediaPoolEntry
{
public:
	EMMediaPoolVideoEntry(string p_oFileName);
	EMMediaPoolVideoEntry(EMProjectDataLoader* p_opLoader, uint32 vUndecidedType); //Used for instantiating entries from a loaddata stream
	bool SaveData(EMProjectDataSaver* p_opSaver);
	bool InitCheckE();
	~EMMediaPoolVideoEntry();
	EMBitmap* GetVideoContent(int64 p_vTime, int16 p_vHeight);
	void GetContent(void* p_upArray, int32& p_vArrayCapacityBytes, int64 p_vMediaTime);
	void GetLowResContent(int8* p_vpArray, int64 p_vArrayLength);
	EMMediaFormat* GetFormat() const;
	int64 GetLength() const;
	int64 GetSize() const;
	int64 GetFrameRate() const;
	int64 GetNumChannels() const;
	int64 BitDepth() const;
	float* GetAspectRatio() const;
	int64* NumberOfFramesInWFPS(float p_vFramerateCurrentFPS);	
	string GetFormatString();
	const char* GetVideoDimension() const;
	void CloseAVIStream();
private:
	EMWinVideoFile* m_opVideoFile;

};

#endif
