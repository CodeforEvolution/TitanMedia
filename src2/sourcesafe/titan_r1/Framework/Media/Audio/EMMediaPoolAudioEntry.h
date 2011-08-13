/*******************************************************
* Creator: Richard Kronfalt
* Portability: Non-Native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_MEDIA_POOL_AUDIO_ENTRY
#define __EM_MEDIA_POOL_AUDIO_ENTRY

#include "EMMediaGlobals.h"
#include "EMMediaPoolEntry.h"

#include "EMWaveFileReader.h"
#include "EMWaveFileWriter.h"
#include "EMBitmap.h"
#include <string>

class EMMediaFormat;
class EMProjectDataLoader;
class EMProjectDataSaver;

class EMMediaPoolAudioEntry : public EMMediaPoolEntry
{
public:
	EMMediaPoolAudioEntry(string p_oFileName);
	EMMediaPoolAudioEntry(EMProjectDataLoader* p_opLoader, uint32 vUndecidedType); //Used for instantiating entries from a loaddata stream
	bool SaveData(EMProjectDataSaver* p_opSaver);

	~EMMediaPoolAudioEntry();
	int64 GetLength() const;
	int64 GetSize() const;
	int64 GetFrameRate() const;
	int64 GetNumChannels() const;
	int64 BitDepth() const;
	void GetHiResContent(void* p_upArray, int32& p_vArrayCapacityBytes, int64 p_vMediaTime);
	void GetLowResContent(uint8* p_vpArray, int64 p_vArrayLength);
	int64 GetNumSamples() const;
	EMMediaFormat* GetFormat() const;
	EMWaveFileReader* GetHiResFile() const;
	bool InitCheckE();
	string GetFormatString();

private:
	EMWaveFileReader* m_opHiResWaveFile;
	EMWaveFileReader* m_opLowResWaveFile;
};

#endif
