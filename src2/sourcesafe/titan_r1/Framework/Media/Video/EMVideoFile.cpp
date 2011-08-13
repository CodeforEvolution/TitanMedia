
#include "EMGlobals.h"

#include "EMVideoFile.h"

#include "EMBeMediaUtility.h"
#include "EMMediaDataBuffer.h"
#include "EMMediaEngine.h"
#include "EMMediaFormat.h"
#include "EMMediaIDManager.h"
#include "EMMediaTimer.h"
#include "EMMessages.h"
#include "EMSettingIDs.h"
#include "EMSettingsRepository.h"
#include "EMSemaphore.h"
#include "EMWinVideoFile.h"
//TODO: Delete semaphore somehow!
EMSemaphore* EMVideoFile::m_opVideoImageProcessingSemaphore = NULL;

EMVideoFile* EMVideoFile::Create(string p_oFileName)
{
#ifdef PLATFORM_BEOS
	return new EMBeVideoFile(p_oFileName);
#endif

#ifdef PLATFORM_WINDOWS
	return new EMWinVideoFile(p_oFileName);
	return NULL;
#endif
}

EMVideoFile::EMVideoFile(string p_oFileName)
	:	m_oFileName(p_oFileName)
{
	m_vID = EMMediaIDManager::MakeID();
	
//	EMMediaTimer::Instance() -> AddListener(this);
//	EMMediaEngine::Instance() -> GetSettingsRepository() -> AddListener(this);
}

EMVideoFile::~EMVideoFile()
{	
//	EMMediaTimer::Instance() -> RemoveListener(this);
//	EMMediaEngine::Instance() -> GetSettingsRepository() -> RemoveListener(this);

	if(m_opVideoImageProcessingSemaphore != NULL)		//Kaneda - Buggfix
	{
		m_opVideoImageProcessingSemaphore -> Release();
		delete m_opVideoImageProcessingSemaphore;
	}
}

int32 EMVideoFile::GetID() const
{
	return m_vID;
}

bool EMVideoFile::LockVideo()
{
	if(m_opVideoImageProcessingSemaphore == NULL)
		m_opVideoImageProcessingSemaphore = EMSemaphore::CreateEMSemaphore();
	
	m_opVideoImageProcessingSemaphore -> Acquire();
	return true;
}

void EMVideoFile::UnlockVideo()
{
	if(m_opVideoImageProcessingSemaphore != NULL)
		m_opVideoImageProcessingSemaphore -> Release();
}
