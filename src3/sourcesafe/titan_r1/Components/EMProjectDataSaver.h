/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_PROJECT_DATA_SAVER
#define __EM_PROJECT_DATA_SAVER

#include <fstream>

using namespace std;

class __declspec(dllexport) EMProjectDataSaver
{
public:
	EMProjectDataSaver();
	void CloseFile();
	void DataContainerIsFinished();
	bool InitializeSave(string p_oPath, bool p_vIsProjectFile = true);
	void SaveBool(bool p_vValue);
	void SaveBytes(const char* p_vArray, uint32 p_vLength);
	void SaveFloat(float p_vValue);
	void SaveInt(int p_vValue);
	void SaveInt16(int16 p_vValue);
	void SaveInt32(int32 p_vValue);
	void SaveInt64(int64 p_vValue);
	void SaveInt8(int8 p_vValue);
	void SaveString(const char* p_oString);
	void SaveUInt16(uint16 p_vValue);
	void SaveUInt32(uint32 p_vValue);
	void SaveUInt64(uint64 p_vValue);
	void SaveUInt8(uint8 p_vValue);
	void PrepareForNewDataContainer();

private:
	ofstream m_oOutputStream;
	uint32 m_vSavedBytes;
};

#endif