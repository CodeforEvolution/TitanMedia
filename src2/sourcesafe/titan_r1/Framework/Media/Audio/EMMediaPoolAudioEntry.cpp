#include "EMMediaPoolAudioEntry.h"

#include "EMMediaUtility.h"
#include "EMMediaFormat.h"
#include "EMProjectDataLoader.h"
#include "EMProjectDataSaver.h"
#include "DirectoryIDs.h"

EMMediaPoolAudioEntry::EMMediaPoolAudioEntry(string p_oFileName)
	:	EMMediaPoolEntry(p_oFileName, EM_TYPE_ANY_AUDIO)
{
	m_opHiResWaveFile = EM_new EMWaveFileReader(p_oFileName);
	string oPeaksFileName(EMMediaUtility::Instance() -> MakePeaksFileName(p_oFileName.c_str()));
	m_opLowResWaveFile = EM_new EMWaveFileReader(oPeaksFileName);
}

EMMediaPoolAudioEntry::EMMediaPoolAudioEntry(EMProjectDataLoader* p_opLoader, uint32 vUndecidedType)
	:	EMMediaPoolEntry(p_opLoader, vUndecidedType)
{
	m_opHiResWaveFile = EM_new EMWaveFileReader(GetFileName());
	string oPeaksFileName(EMMediaUtility::Instance() -> MakePeaksFileName(GetFileName().c_str()));
	m_opLowResWaveFile = EM_new EMWaveFileReader(oPeaksFileName);
}

EMMediaPoolAudioEntry::~EMMediaPoolAudioEntry()
{
	delete m_opHiResWaveFile;
	delete m_opLowResWaveFile;
}

bool EMMediaPoolAudioEntry::InitCheckE()
{
	if(! m_opHiResWaveFile -> InitCheckE())
		return false;
	return true;//m_opLowResWaveFile -> InitCheckE();
}

int64 EMMediaPoolAudioEntry::GetLength() const
{
	int64 vLength = 0;
	try
	{
//		if(! m_opHiResWaveFile -> HasFormat())
//			m_opHiResWaveFile -> ReadFormatE();
		vLength = EMMediaUtility::Instance() -> FramesToTime(m_opHiResWaveFile -> NumberOfFramesInFile(), m_opHiResWaveFile -> GetFormat());
	}
	catch(...)
	{
		return 0;
	}
	
	return vLength;
}

int64 EMMediaPoolAudioEntry::GetSize() const
{
	int64 vSize = 0;
	try
	{
//		if(! m_opHiResWaveFile -> HasFormat())
//			m_opHiResWaveFile -> ReadFormatE();
		vSize = m_opHiResWaveFile -> GetSize();
	}
	catch(...)
	{
		return 0;
	}
	return vSize;
}

int64 EMMediaPoolAudioEntry::GetFrameRate() const
{
	int64 vFrameRate = 0;
	try
	{
//		if(! m_opHiResWaveFile -> HasFormat())
//			m_opHiResWaveFile -> ReadFormatE();
		vFrameRate = static_cast<int64>(m_opHiResWaveFile -> GetFormat() -> m_vFrameRate);
	}
	catch(...)
	{
		return 0;
	}
	return vFrameRate;
}

int64 EMMediaPoolAudioEntry::GetNumChannels() const
{
	int64 vNumChannels = 0;
	try
	{
//		if(! m_opHiResWaveFile -> HasFormat())
//			m_opHiResWaveFile -> ReadFormatE();
		vNumChannels = static_cast<int64>(m_opHiResWaveFile -> GetFormat() -> m_vNumChannels);
	}
	catch(...)
	{
		return 0;
	}
	return vNumChannels;
}

int64 EMMediaPoolAudioEntry::BitDepth() const
{
	int64 vBitDepth = 0;
	try
	{
//		if(! m_opHiResWaveFile -> HasFormat())
//			m_opHiResWaveFile -> ReadFormatE();
		vBitDepth = static_cast<int64>(m_opHiResWaveFile -> GetFormat() -> m_vBytesPerSample * 8);
	}
	catch(...)
	{
		return 0;
	}
	return vBitDepth;
}

int64 EMMediaPoolAudioEntry::GetNumSamples() const
{
	int64 vNumSamples = 0;
	try
	{
//		if(! m_opHiResWaveFile -> HasFormat())
//			m_opHiResWaveFile -> ReadFormatE();
		vNumSamples = static_cast<int64>(m_opHiResWaveFile -> NumberOfFramesInFile() * m_opHiResWaveFile -> GetFormat() -> m_vNumChannels);
	}
	catch(...)
	{
		return 0;
	}
	return vNumSamples;
}

void EMMediaPoolAudioEntry::GetHiResContent(void* p_upArray, int32& p_vArrayCapacityBytes, int64 p_vMediaTime)
{
	try
	{
//		if(! m_opHiResWaveFile -> HasFormat())
//			m_opHiResWaveFile -> ReadFormatE();
			
		int64 vBytePosition = EMMediaUtility::Instance() -> FramesToBytes(EMMediaUtility::Instance() -> TimeToFrames(p_vMediaTime, m_opHiResWaveFile -> GetFormat()), m_opHiResWaveFile -> GetFormat());
		
		int64 vSizeActuallyRead = (int64) p_vArrayCapacityBytes;
		m_opHiResWaveFile -> ReadData(p_upArray, vSizeActuallyRead, vBytePosition, 0, EM_AUDIO_SAMPLESIZE);
		p_vArrayCapacityBytes = (int32) vSizeActuallyRead;
//		memset(p_upArray, 0, p_vArrayCapacityBytes);
//		p_vArrayCapacityBytes = 3;
	}
	catch(...)
	{
	}
}

void EMMediaPoolAudioEntry::GetLowResContent(uint8* p_vpArray, int64 p_vArrayLength)
{
	try
	{
//		if(! m_opLowResWaveFile -> HasFormat())
//			m_opLowResWaveFile -> ReadFormatE();
		int64 vSize = (int64) p_vArrayLength;
		m_opLowResWaveFile -> ReadData(p_vpArray, vSize, 0, 0, sizeof(signed short));
	}
	catch(...)
	{
	}
}

EMMediaFormat* EMMediaPoolAudioEntry::GetFormat() const
{
	try
	{
//		if(! m_opHiResWaveFile -> HasFormat())
//			m_opHiResWaveFile -> ReadFormatE();
		return m_opHiResWaveFile -> GetFormat();
	}
	catch(...)
	{
	}
	return NULL;
}

EMWaveFileReader* EMMediaPoolAudioEntry::GetHiResFile() const
{
	return m_opHiResWaveFile;
}

//EMWaveFile* EMMediaPoolAudioEntry::GetLoResFile() const
//{
//	return m_opLowResWaveFile;
//
//}

bool EMMediaPoolAudioEntry::SaveData(EMProjectDataSaver* p_opSaver)
{
	if(! EMMediaPoolEntry::SaveData(p_opSaver))
		return false;
	return true;	
}

string EMMediaPoolAudioEntry::GetFormatString()
{
#ifdef PLATFORM_WINDOWS
	SHFILEINFO sFileInfo;
	const char* vpPath = GetFileName().c_str();
	SHGetFileInfo(vpPath, 0, &sFileInfo, sizeof(sFileInfo), SHGFI_TYPENAME);
	string oDescription = string(sFileInfo.szTypeName);
#else
	string oDescription = "ERROR! Unrecognized audio format on non-Windows platform";
#endif
	return oDescription;
}