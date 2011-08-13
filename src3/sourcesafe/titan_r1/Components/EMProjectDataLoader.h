/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_PROJECT_DATA_LOADER
#define __EM_PROJECT_DATA_LOADER

#include <fstream>
#include <string>

using namespace std;

#ifdef LoadString
#undef LoadString
#endif

const int PROJECT_DATA_BUFFER_SIZE = 1000;

class __declspec(dllexport) EMProjectDataLoader
{
public:
	EMProjectDataLoader();
	void CloseFile();
	void DataContainerIsFinished();
	bool InitializeLoad(string p_oPath, bool p_vIsProjectFile = true);
	bool LoadBool();
	void LoadBytes(char* p_vpArray, uint32 p_vLength);
	float LoadFloat();
	int LoadInt();
	int16 LoadInt16();
	int32 LoadInt32();
	int64 LoadInt64();
	int8 LoadInt8();
	const char* LoadString();
	uint16 LoadUInt16();
	uint32 LoadUInt32();
	uint64 LoadUInt64();
	uint8 LoadUInt8();
	void PrepareForNewDataContainer();

private:
	ifstream m_oInputStream;
	uint32 m_vActualLoadedBytes;
	char m_vDataBuffer[PROJECT_DATA_BUFFER_SIZE];
	uint32 m_vExpectedLoadedBytes;
};

#endif