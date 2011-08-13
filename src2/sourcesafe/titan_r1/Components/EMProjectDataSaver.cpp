#include "EMProjectDataSaver.h"

#include <fstream>

const char* FILE_SAVE_SIGNATURE = "EM_PROJ_V01"; // Should appear at the start of a project file
const int PROJECT_DATA_BUFFER_SIZE = 1000;

EMProjectDataSaver::EMProjectDataSaver()
{
}

void EMProjectDataSaver::CloseFile()
{
	if(!m_oOutputStream)
		EMDebugger("Trying to close a closed file");
	m_oOutputStream.close();
}

// Called when a EMProjectDataContainer has saved its data
// It writes the nuber of saved bytes
void EMProjectDataSaver::DataContainerIsFinished()
{
	SaveUInt32(m_vSavedBytes);
}

bool EMProjectDataSaver::InitializeSave(string p_oPath, bool p_vIsProjectFile)
{
eo << "EMProjectDataSaver::InitializeSave: Saving data to path: " << p_oPath.c_str() << ef;
	m_oOutputStream.open(p_oPath.c_str(), std::ios::out | std::ios::binary);//ios::binary | ios::out | ios::trunc);

	if(!m_oOutputStream)
	{
eo << "EMProjectDataSaver: Could not open stream" << ef;
		return false;
	}

	m_vSavedBytes = 0;

	if(p_vIsProjectFile)
		SaveString(FILE_SAVE_SIGNATURE);

	return true;
}

void EMProjectDataSaver::SaveBool(bool p_vValue)
{
	m_oOutputStream.write((/*unsigned*/ char*) &p_vValue, sizeof(bool));
	m_vSavedBytes += sizeof(bool);
}

void EMProjectDataSaver::SaveBytes(const char* p_vArray, uint32 p_vLength)
{
	m_oOutputStream.write(p_vArray, p_vLength);
	m_vSavedBytes += p_vLength;
}

void EMProjectDataSaver::SaveFloat(float p_vValue)
{
	m_oOutputStream.write((/*unsigned*/ char*) &p_vValue, sizeof(float));
	m_vSavedBytes += sizeof(float);
}

// Use with caution since the size of an "int" may vary with different platforms
void EMProjectDataSaver::SaveInt(int p_vValue)
{
	m_oOutputStream.write((/*unsigned*/ char*) &p_vValue, sizeof(int));
	m_vSavedBytes += sizeof(int);
}

void EMProjectDataSaver::SaveInt16(int16 p_vValue)
{
	m_oOutputStream.write((/*unsigned*/ char*) &p_vValue, sizeof(int16));
	m_vSavedBytes += sizeof(int16);
}

void EMProjectDataSaver::SaveInt32(int32 p_vValue)
{
	m_oOutputStream.write((/*unsigned*/ char*) &p_vValue, sizeof(int32));
	m_vSavedBytes += sizeof(int32);
}

void EMProjectDataSaver::SaveInt64(int64 p_vValue)
{
	m_oOutputStream.write((/*unsigned*/ char*) &p_vValue, sizeof(int64));
	m_vSavedBytes += sizeof(int64);
}

void EMProjectDataSaver::SaveInt8(int8 p_vValue)
{
	m_oOutputStream.write((/*unsigned*/ char*) &p_vValue, sizeof(int8));
	m_vSavedBytes += sizeof(int8);
}

void EMProjectDataSaver::SaveString(const char* p_oString)
{
	string oString = p_oString;
	uint32 vStringLength = static_cast<int>(oString.length());
	SaveUInt32(vStringLength);
	m_oOutputStream << oString;
	m_vSavedBytes += vStringLength * sizeof(uchar);
}

void EMProjectDataSaver::SaveUInt16(uint16 p_vValue)
{
	m_oOutputStream.write((/*unsigned*/ char*) &p_vValue, sizeof(uint16));
	m_vSavedBytes += sizeof(uint16);
}

void EMProjectDataSaver::SaveUInt32(uint32 p_vValue)
{
	m_oOutputStream.write((/*unsigned*/ char*) &p_vValue, sizeof(uint32));
	m_vSavedBytes += sizeof(uint32);
}

void EMProjectDataSaver::SaveUInt64(uint64 p_vValue)
{
	m_oOutputStream.write((/*unsigned*/ char*) &p_vValue, sizeof(uint64));
	m_vSavedBytes += sizeof(uint64);
}

void EMProjectDataSaver::SaveUInt8(uint8 p_vValue)
{
	m_oOutputStream.write((/*unsigned*/ char*) &p_vValue, sizeof(uint8));
	m_vSavedBytes += sizeof(uint8);
}

// Called before a new EMProjectDataContainer starts to load its data
void EMProjectDataSaver::PrepareForNewDataContainer()
{
	m_vSavedBytes = 0;
}



