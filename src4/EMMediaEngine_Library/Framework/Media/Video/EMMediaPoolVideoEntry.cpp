#include "EMGlobals.h"

#include "EMMediaPoolVideoEntry.h"
#include "EMMediaPoolEntry.h"
#include "EMProjectDataLoader.h"
#include "EMProjectDataSaver.h"

#ifdef PLATFORM_BEOS
#include "EMBeBitmapContainer.h"
#endif

EMMediaPoolVideoEntry::EMMediaPoolVideoEntry(string p_oFileName)
	:	EMMediaPoolEntry(p_oFileName, EM_TYPE_ANY_VIDEO), m_opVideoFile(NULL)
{
	m_opVideoFile = EM_new EMWinVideoFile(p_oFileName);
//	m_opVideoFile = EMVideoFile::Create(p_oFileName);
}

EMMediaPoolVideoEntry::EMMediaPoolVideoEntry(EMProjectDataLoader* p_opLoader, uint32 vUndecidedType)
	:	EMMediaPoolEntry(p_opLoader, vUndecidedType)
{
	m_opVideoFile = static_cast<EMWinVideoFile*>(EMWinVideoFile::Create(GetFileName())); //"agnus");//GetFileName());
}

EMMediaPoolVideoEntry::~EMMediaPoolVideoEntry()
{
	if(m_opVideoFile != NULL)
		delete m_opVideoFile;
}

EMBitmap* EMMediaPoolVideoEntry::GetVideoContent(int64 p_vTime, int16 p_vHeight)
{
	try
	{
#ifdef PLATFORM_BEOS
		EMBitmap* opEMBeBitmapContainer = EM_new EMBeBitmapContainer(m_opVideoFile -> ReadAndScaleData(p_vTime, p_vHeight)); //Should be deleted somewhere!?
		return opEMBeBitmapContainer;
#endif
#ifdef PLATFORM_WINDOWS
		EMBitmap* opEMBitmapContainer = EM_new EMWinBitmap(m_opVideoFile -> ReadAndScaleData(p_vTime, p_vHeight)); //Should be deleted somewhere!?
		return opEMBitmapContainer;
#endif
	}
	catch(...)
	{
	}
	return NULL;
}

void EMMediaPoolVideoEntry::CloseAVIStream()
{
	m_opVideoFile -> CloseAVIStream();
}

bool EMMediaPoolVideoEntry::InitCheckE()
{
	return m_opVideoFile->InitCheckE();
}

int64 EMMediaPoolVideoEntry::GetLength() const
{
	try
	{
		//convert from video time to audio frames ... maybe?
		m_opVideoFile -> InitCheckE();
		m_opVideoFile -> ReadFormatE();
		return m_opVideoFile -> GetDuration();

	}
	catch(...)
	{
		return 0;
	}
}

float* EMMediaPoolVideoEntry::GetAspectRatio() const
{
	return (m_opVideoFile -> GetAspectRatio());
}

int64* EMMediaPoolVideoEntry::NumberOfFramesInWFPS(float p_vFramerateCurrentFPS)
{
	try
	{
		//convert from video time to audio frames ... maybe?
		m_opVideoFile -> InitCheckE();
		return (m_opVideoFile -> NumberOfFramesInWFPS(p_vFramerateCurrentFPS));

	}
	catch(...)
	{
		return NULL;
	}

}

const char* EMMediaPoolVideoEntry::GetVideoDimension() const
{
	return m_opVideoFile -> GetDimensionString();
}


int64 EMMediaPoolVideoEntry::GetSize() const
{
	return 0;
}

int64 EMMediaPoolVideoEntry::GetFrameRate() const
{
	return 0;
}

int64 EMMediaPoolVideoEntry::GetNumChannels() const
{
	return 0;
}

int64 EMMediaPoolVideoEntry::BitDepth() const
{
	return 0;
}

void EMMediaPoolVideoEntry::GetContent(void* p_upArray, int32& p_vArrayCapacityBytes, int64 p_vMediaTime)
{
}

void EMMediaPoolVideoEntry::GetLowResContent(int8* p_vpArray, int64 p_vArrayLength)
{
}

EMMediaFormat* EMMediaPoolVideoEntry::GetFormat() const
{
	try
	{
		m_opVideoFile -> InitCheckE();
		m_opVideoFile -> ReadFormatE();
		return m_opVideoFile -> GetFormat();
	}
	catch(...)
	{
	}
	return NULL;
}

bool EMMediaPoolVideoEntry::SaveData(EMProjectDataSaver* p_opSaver)
{
	if(! EMMediaPoolEntry::SaveData(p_opSaver))
		return false;
	return true;	
}

string EMMediaPoolVideoEntry::GetFormatString()
{
#ifdef PLATFORM_WINDOWS
	SHFILEINFO sFileInfo;
	const char* vpPath = GetFileName().c_str();
	SHGetFileInfo(vpPath, 0, &sFileInfo, sizeof(sFileInfo), SHGFI_TYPENAME);
	string oDescription = string(sFileInfo.szTypeName);
#else
	string oDescription = "ERROR! Unrecognized video format on a non-Windows platform";
#endif
	return oDescription;
}