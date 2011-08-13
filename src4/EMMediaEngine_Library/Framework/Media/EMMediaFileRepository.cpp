#include "EMMediaFileRepository.h"

#include "EMMediaFile.h"
#include "EMMediaFileManager.h"
#include "EMMediaItemContainer.h"
#include "EMMediaEngine.h"
#include "EMMediaProject.h"

EMMediaFileRepository* EMMediaFileRepository::m_opInstance = NULL;

EMMediaFileRepository* EMMediaFileRepository::Instance()
{
	if(m_opInstance == NULL)
		m_opInstance = EM_new EMMediaFileRepository();
	return m_opInstance;
}

void EMMediaFileRepository::Delete()
{
	delete m_opInstance;
	m_opInstance = NULL;
}

EMMediaFileRepository::EMMediaFileRepository()
	:	EMMediaItemContainer<EMMediaFile>(EM_TYPE_UNKNOWN) 
{
}

EMMediaFileRepository::~EMMediaFileRepository()
{
}

void EMMediaFileRepository::OnItemAdd()
{
	EMMediaEngine::Instance() -> GetMediaProject() -> SetDirty(true);
}

void EMMediaFileRepository::OnItemRemove()
{
	EMMediaEngine::Instance() -> GetMediaProject() -> SetDirty(true);
}

bool EMMediaFileRepository::AddFile(const string* p_opFileName, bool p_vNeverRam)
{	
	EMMediaFile* opMediaFile = EMMediaFile::Create(p_opFileName);
	if(opMediaFile != NULL)
	{
		if(! opMediaFile -> InitCheckE())
		{
			delete opMediaFile;
			EMDebugger("ERROR! Could not initialize EMMediaFile!");
			return false;
		}
		LockContainer();
		try
		{
			Add(opMediaFile);
		}
		catch(...)
		{
			EMDebugger("ERROR! Caught an exception in EMMediaFileRepository::AddFile");
		}
		UnlockContainer();
	}
	else
		EMDebugger("ERROR! Could not create EMMediaFile object!");
	return true;
}

void EMMediaFileRepository::RemoveFile(EMMediaFile* p_opFile)
{
	LockContainer();
	try
	{
		Remove(p_opFile);
	}
	catch(...)
	{
		EMDebugger("ERROR! Caught an exception in EMMediaFileRepository::RemoveFile");
	}
	UnlockContainer();
}

EMMediaFile* EMMediaFileRepository::FindFileName(const string* p_opFileName)
{
	LockContainer();
	try
	{
		Rewind();
		while(Current() != NULL)
		{
			string oFileName1(*(Current() -> GetFileName()));
			string oFileName2(*p_opFileName);
			if(oFileName1 == oFileName2)
			{
				EMMediaFile* opReturn = Current();
				UnlockContainer();
				return opReturn;
			}
			Next();
		}
	}
	catch(...)
	{
		EMDebugger("ERROR! Caught an exception in EMMediaFileRepository::FindFileName");
	}
	UnlockContainer();
	return NULL;
}
