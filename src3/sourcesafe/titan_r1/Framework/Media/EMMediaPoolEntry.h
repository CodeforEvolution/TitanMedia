/*******************************************************
* Creator: Richard Kronfalt
* Portability: Non-Native
*-------------------------------------------------------
* 
* This class simply represents a media file.
* It exists in one instance per "physical" file.
*
* Richard Kronfält, 2000-12-02
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_MEDIA_POOL_ENTRY
#define __EM_MEDIA_POOL_ENTRY

#include "EMMediaGlobals.h"
#include "EMBitmap.h"
#include <string>

class EMProjectDataSaver;
class EMProjectDataLoader;
class EMMediaFormat;

class EMMediaPoolEntry 
{
public:
	virtual ~EMMediaPoolEntry();
//	virtual EMBitmap* GetVideoContent(int64 p_vTime, int16 p_vHeight) = 0;
	virtual EMMediaFormat* GetFormat() const = 0;
	virtual int64 GetLength() const = 0;
	virtual int64 GetSize() const = 0;
	virtual int64 GetFrameRate() const = 0;
	virtual string GetFormatString() = 0;

	virtual bool InitCheckE() = 0;

	virtual int64 BitDepth() const = 0;
	int32 GetID() const;
	string GetFileName() const;
	EMMediaType GetType() const;
	void IncreaseUsed();
	void DecreaseUsed();

	virtual int64* NumberOfFramesInWFPS(float p_vFramerateCurrentFPS) {return NULL;};	
	
	int32 GetUsage() const;
	virtual bool SaveData(EMProjectDataSaver* p_opSaver);

protected:
	EMMediaPoolEntry(string p_oFileName, EMMediaType p_eType);
	EMMediaPoolEntry(EMProjectDataLoader* p_opLoader, uint32 vUndecidedType); //Used for instantiating entries from a loaddata stream
	int32 m_vID;
	string m_oFileName;
	EMMediaType m_eType;
	int32 m_vUsed;
};

#endif
