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

#ifndef __EM_WAVE_FILE_WRITER
#define __EM_WAVE_FILE_WRITER

#include "EMGlobals.h"
#include "EMMediaGlobals.h"
#include "EMWaveFileReader.h" //To get the chunk-declarations

#include <string>
#include <fstream>

class EMMediaFormat;
class EMSemaphore;
class EMMediaDataBuffer;

class __declspec(dllexport) EMWaveFileWriter 
{
public:
	EMWaveFileWriter(string p_oFileName);
	~EMWaveFileWriter();
	int32 GetID() const;
	bool InitCheckE();
	void WriteHeaderE(EMMediaFormat* p_oFormat); //Creates the header and closes the file
	void WriteData(void* p_upData, int64 p_vNumBytes);

private:
	string m_oFileName;
	EMMediaFile* m_opFile;
	int32 m_vID;
	bool m_vIsInitialized;
	EMSemaphore* m_opSem;
};

#endif
