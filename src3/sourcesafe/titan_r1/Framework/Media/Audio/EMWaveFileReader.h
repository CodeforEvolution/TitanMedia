/*******************************************************
* Creator: Richard Kronfalt
* Portability: Native
*-------------------------------------------------------
* 
* This class opens a file, finds out which format it is
* and reads from it, filling a buffer with data.
*
* It can also seek to a frame as well as time.
*
* Richard Kronfält, 00-11-82
*******************************************************/

#ifndef __EM_WAVE_FILE_READER
#define __EM_WAVE_FILE_READER

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#include <string>
#include <fstream>

class EMMediaFormat;
class EMSemaphore;

class EMWaveChunk
{
public:
	char id[4];				//	4
	int32 size;				//	4
};	//				TOTAL:		8

class EMWaveRiff
{
public:
	char format[4];
};

class EMWaveFormat
{
public:
	int16 format;			//	2
	int16 chans;			//	2
	int32 samplerate;		//	4
	int32 byterate;			//	4
	int16 blockalign;		//	2
	int16 bitspersample;	//	2
};	//				TOTAL:		24

class EMWaveHeader
{
public:
	EMWaveChunk RiffTag;
	EMWaveRiff WaveTag;
	EMWaveChunk FormatTag;
	EMWaveFormat Format;
};

class EMMediaFile;

class EMWaveFileReader 
{
public:
	EMWaveFileReader(string p_oFileName);
	~EMWaveFileReader();
	EMMediaFormat* GetFormat();
	int64 CurrentBytePosition();
	int32 GetID() /*const*/;
	bool InitCheckE();
	int64 NumberOfFramesInFile() /*const*/;
	void ReadData(void* p_vpBuffer, int64& p_vInOutNumBytes, int64 p_vBytePosition, int64 p_vBufferOffset = 0, int32 p_vNumBytesPerSample = 2);
	bool HasFormat() /*const*/;

	void SeekToFrame(int64 p_vToFrame);
	void SeekToTime(int64 p_vToTime);
	int64 GetSize() /*const*/;
	string GetFileName() /*const*/;

protected:
private:
	EMMediaFormat* ReadFormatE();
	string m_oFileName;
	EMMediaFile* m_opFile;
	int32 m_vID;
	bool m_vIsInitialized;
	bool m_vHasFormat;
	int64 m_vSizeBytes;
	int64 m_vHeaderLength;
	EMMediaFormat* m_opFormat;
	EMSemaphore* m_opSem;
	int64 m_vCurrentPosition;
};

#endif
