#include "EMGlobals.h"

#include "EMMediaFileManager.h"
#include "EMBeMediaFileManager.h"
#include "EMMediaUtility.h"
#include "EMMediaIDManager.h"
#include "EMWinMediaFileManager.h"
#include "EMSemaphore.h"

EMMediaFileManager* EMMediaFileManager::m_opInstance = NULL;
EMSemaphore* EMMediaFileManager::m_opSemaphore = EMSemaphore::CreateEMSemaphore();
//sem_id EMMediaFileManager::m_vSemaphore = create_sem(1, "File managment sem");

EMMediaFileManager* EMMediaFileManager::Instance()
{
	if(m_opInstance == NULL)
#ifdef PLATFORM_BEOS
		m_opInstance = EM_new EMBeMediaFileManager();
#endif
#ifdef PLATFORM_WINDOWS
		m_opInstance = EM_new EMWinMediaFileManager();
#endif
	
	return m_opInstance;
}

EMMediaFileManager::EMMediaFileManager()
{
	m_vID = EMMediaIDManager::MakeID();
	m_opSemaphore = EMSemaphore::CreateEMSemaphore("File management semaphore");
}

EMMediaFileManager::~EMMediaFileManager()
{
	delete m_opSemaphore;
}

void EMMediaFileManager::Delete()
{
	delete m_opInstance;
	m_opInstance = NULL;
}

int32 EMMediaFileManager::GetID() const
{
	return m_vID;
}

void EMMediaFileManager::Lock()
{
	m_opSemaphore -> Acquire();
}

void EMMediaFileManager::Unlock()
{
	m_opSemaphore -> Release();
}