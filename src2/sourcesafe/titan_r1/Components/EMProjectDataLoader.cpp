#include "EMProjectDataLoader.h"

const char* FILE_LOAD_SIGNATURE = "EM_PROJ_V01"; // Should appear at the start of a project file

EMProjectDataLoader::EMProjectDataLoader()//ifstream* p_opInputStream) :
//m_opInputStream(p_opInputStream)
{
}

void EMProjectDataLoader::CloseFile()
{
	if(!m_oInputStream)
		EMDebugger("Trying to close a closed file");
	m_oInputStream.close();
}

// Called when a EMProjectDataContainer has loaded its data to check if
// it loaded the same amount of bytes that it previously saved
void EMProjectDataLoader::DataContainerIsFinished()
{
	// We have to subtract 32 bytes from the loaded bytes since the expected
	// loaded bytes is added to the count
	m_vExpectedLoadedBytes = LoadUInt32();
	m_vActualLoadedBytes -= sizeof(uint32);
	if(m_vActualLoadedBytes != m_vExpectedLoadedBytes)
	{
		EMDebugger("The data container loaded less/more bytes than it previously saved");
	}
}

bool EMProjectDataLoader::InitializeLoad(string p_oPath, bool p_vIsProjectFile)
{
	m_oInputStream.open(p_oPath.c_str(), ios::binary | ios::in);// | std::ios::nocreate);
	if(!m_oInputStream)
		return false;

	if(p_vIsProjectFile)
	{
		string vSignature(LoadString());
		if(vSignature != FILE_LOAD_SIGNATURE)
			return false;
	}

	return true;
}

bool EMProjectDataLoader::LoadBool()
{
	bool vBool = false;
	m_oInputStream.read((/*unsigned*/ char*) &vBool, sizeof(bool));
	m_vActualLoadedBytes += sizeof(bool);
	return vBool;
}

void EMProjectDataLoader::LoadBytes(char* p_vpArray, uint32 p_vLength)
{
	m_oInputStream.read(p_vpArray, p_vLength);
	m_vActualLoadedBytes += p_vLength;
}

float EMProjectDataLoader::LoadFloat()
{
	float vValue = 0;
	m_oInputStream.read((/*unsigned*/ char*) &vValue, sizeof(float));
	m_vActualLoadedBytes += sizeof(float);
	return vValue;
}

// Use with caution since the size of an "int" may vary with different platforms
int EMProjectDataLoader::LoadInt()
{
	int vValue = 0;
	m_oInputStream.read((/*unsigned*/ char*) &vValue, sizeof(int));
	m_vActualLoadedBytes += sizeof(int);
	return vValue;
}

int16 EMProjectDataLoader::LoadInt16()
{
	int16 vValue = 0;
	m_oInputStream.read((/*unsigned*/ char*) &vValue, sizeof(int16));
	m_vActualLoadedBytes += sizeof(int16);
	return vValue;
}

int32 EMProjectDataLoader::LoadInt32()
{
	int32 vValue = 0;
	m_oInputStream.read((/*unsigned*/ char*) &vValue, sizeof(int32));
	m_vActualLoadedBytes += sizeof(int32);
	return vValue;
}

int64 EMProjectDataLoader::LoadInt64()
{
	int64 vValue = 0;
	m_oInputStream.read((/*unsigned*/ char*) &vValue, sizeof(int64));
	m_vActualLoadedBytes += sizeof(int64);
	return vValue;
}

int8 EMProjectDataLoader::LoadInt8()
{
	int8 vValue = 0;
	m_oInputStream.read((/*unsigned*/ char*) &vValue, sizeof(int8));
	m_vActualLoadedBytes += sizeof(int8);
	return vValue;
}

const char* /*__declspec(dllexport)*/ EMProjectDataLoader::LoadString()
{
//	char vDataBuffer[PROJECT_DATA_BUFFER_SIZE];
	int32 vLength = LoadUInt32();
	m_oInputStream.read((/*unsigned*/ char*) m_vDataBuffer, vLength);
	m_vDataBuffer[vLength] = '\0';
	m_vActualLoadedBytes += vLength;
	return m_vDataBuffer;//string(vDataBuffer);
}

uint16 EMProjectDataLoader::LoadUInt16()
{
	uint16 vValue = 0;
	m_oInputStream.read((/*unsigned*/ char*) &vValue, sizeof(uint16));
	m_vActualLoadedBytes += sizeof(uint16);
	return vValue;
}

uint32 EMProjectDataLoader::LoadUInt32()
{
	uint32 vValue = 0;
	m_oInputStream.read((/*unsigned*/ char*) &vValue, sizeof(vValue));
	m_vActualLoadedBytes += sizeof(vValue);
	return vValue;
}

uint64 EMProjectDataLoader::LoadUInt64()
{
	uint64 vValue = 0;
	m_oInputStream.read((/*unsigned*/ char*) &vValue, sizeof(uint64));
	m_vActualLoadedBytes += sizeof(uint64);
	return vValue;
}

uint8 EMProjectDataLoader::LoadUInt8()
{
	uint8 vValue = 0;
	m_oInputStream.read((/*unsigned*/ char*) &vValue, sizeof(uint8));
	m_vActualLoadedBytes += sizeof(uint8);
	return vValue;
}

// Called before a new EMProjectDataContainer starts to load its data
void EMProjectDataLoader::PrepareForNewDataContainer()
{
	m_vActualLoadedBytes = 0;
}



