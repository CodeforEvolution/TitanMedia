/*******************************************************
* Creator: Richard Kronfalt
* Portability: Native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

//#ifdef PLATFORM_BEOS

#ifndef __EM_BE_MEDIA_FILE
#define __EM_BE_MEDIA_FILE

#include "EMMediaGlobals.h"
#include "EMMediaFile.h"

#include <fstream>

#define CSTYLE_FILE

class EMBeMediaFile : public EMMediaFile
{
public:
	EMBeMediaFile(const string* p_opName);
	~EMBeMediaFile();
	bool Close();
	bool Open(int32 p_vDirection);
	int64 Read(void* p_upBuffer, int64 p_vLength, int64 p_vStartPosition);
	void Write(void* p_upBuffer, int64 p_vLength);
	bool SeekFromEnd(int64 p_vByteNumber);
	bool SeekFromStart(int64 p_vByteNumber);
	int64 GetPosition() const;
	int64 GetLength();

protected:
private:
	ifstream* m_opInFile;
	FILE* m_opCStyleInFile;
	ofstream* m_opOutFile;
	int64 m_vPosition;
//	sem_id m_vSemaphore;
};

#endif

//#endif
