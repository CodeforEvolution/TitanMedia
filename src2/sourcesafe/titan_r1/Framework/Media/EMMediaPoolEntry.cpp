#include "EMMediaPoolEntry.h"

#include "EMMediaIDManager.h"
#include "EMMediaUtility.h"
#include "EMMediaFormat.h"
#include "EMProjectDataSaver.h"
#include "EMProjectDataLoader.h"
#include "DirectoryIDs.h"

EMMediaPoolEntry::EMMediaPoolEntry(string p_oFileName, EMMediaType p_eType)
	:	m_oFileName(p_oFileName),
		m_eType(p_eType),
		m_vUsed(0)
{
	m_vID = EMMediaIDManager::MakeID();
}

EMMediaPoolEntry::EMMediaPoolEntry(EMProjectDataLoader* p_opLoader, uint32 vUndecidedType)
{
	uint32 vID = static_cast<uint32>(p_opLoader -> LoadUInt32());
	m_vID = (int32) vID;
	m_vUsed = 0; //static_cast<int32>(p_opLoader -> LoadUInt32());
	
//	uint32 vType = p_opLoader -> LoadUInt32();

	if((vUndecidedType & EM_TYPE_RAW_AUDIO) == EM_TYPE_RAW_AUDIO) m_eType = EM_TYPE_RAW_AUDIO;
	else if((vUndecidedType & EM_TYPE_ENCODED_AUDIO) == EM_TYPE_RAW_AUDIO) m_eType = EM_TYPE_ENCODED_AUDIO;
	else if((vUndecidedType & EM_TYPE_ANY_AUDIO) == EM_TYPE_RAW_AUDIO) m_eType = EM_TYPE_ANY_AUDIO;
	else if((vUndecidedType & EM_TYPE_UNKNOWN_AUDIO) == EM_TYPE_UNKNOWN_AUDIO) m_eType = EM_TYPE_UNKNOWN_AUDIO;

	else if((vUndecidedType & EM_TYPE_RAW_VIDEO) == EM_TYPE_RAW_VIDEO) m_eType = EM_TYPE_RAW_VIDEO;
	else if((vUndecidedType & EM_TYPE_ENCODED_VIDEO) == EM_TYPE_ENCODED_VIDEO) m_eType = EM_TYPE_ENCODED_VIDEO;
	else if((vUndecidedType & EM_TYPE_ANY_VIDEO) == EM_TYPE_ANY_VIDEO) m_eType = EM_TYPE_ANY_VIDEO;
	else if((vUndecidedType & EM_TYPE_UNKNOWN_VIDEO) == EM_TYPE_UNKNOWN_VIDEO) m_eType = EM_TYPE_UNKNOWN_VIDEO;
	else if((vUndecidedType & EM_TYPE_BMP) == EM_TYPE_BMP) m_eType = EM_TYPE_BMP;
	else if((vUndecidedType & EM_TYPE_GIF) == EM_TYPE_GIF) m_eType = EM_TYPE_GIF;
	else if((vUndecidedType & EM_TYPE_JPG) == EM_TYPE_JPG) m_eType = EM_TYPE_JPG;
	else if((vUndecidedType & EM_TYPE_TGA) == EM_TYPE_TGA) m_eType = EM_TYPE_TGA;
	else 
		EMDebugger("ERROR! Unknown type saved in project file!");

	if((m_eType & EM_TYPE_ANY_AUDIO) > 0)
		m_oFileName = EMMediaUtility::Instance() -> ParseFullPath(p_opLoader -> LoadString(), DIRECTORY_ID_AUDIO_DATA_USED);
	else
		m_oFileName = string(p_opLoader -> LoadString());
}

EMMediaPoolEntry::~EMMediaPoolEntry()
{
}

int32 EMMediaPoolEntry::GetID() const
{
	return m_vID;
}

string EMMediaPoolEntry::GetFileName() const
{
	return m_oFileName;
}

EMMediaType EMMediaPoolEntry::GetType() const
{
	return m_eType;
}

void EMMediaPoolEntry::IncreaseUsed()
{
	m_vUsed++;
}

void EMMediaPoolEntry::DecreaseUsed()
{
	if(m_vUsed <= 0)
		EMDebugger("Tried to decrease used-counter below zero!");
	m_vUsed--;
}

int32 EMMediaPoolEntry::GetUsage() const
{
	return m_vUsed;
}

bool EMMediaPoolEntry::SaveData(EMProjectDataSaver* p_opSaver)
{
	p_opSaver -> SaveUInt32(static_cast<uint32>(m_vID));
//	p_opSaver -> SaveUInt32(static_cast<uint32>(0)); //m_vUsed));

	if((m_eType & EM_TYPE_ANY_AUDIO) > 0) //If it's an audio file, we can use a project-relative path when loading the project back in
		p_opSaver -> SaveString(EMMediaUtility::Instance() -> GetFileName(GetFileName().c_str()));
	else //But if it's a video file, we can't (since those files aren't copied in to the project!)
		p_opSaver -> SaveString(GetFileName().c_str());

	return true;
}
