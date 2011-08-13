#include "EMGlobals.h"

#ifdef PLATFORM_BEOS

#include "EMBeVideoDisplayer.h"
#include "EMVideoPreview.h"
#include "EMBeBitmapContainer.h"
#include "EMBitmap.h"
#include "EMMediaEngine.h"

#include "EMSettingsRepository.h"
#include "EMSettingIDs.h"
#include "EMMessages.h"
#include "EMBeMediaUtility.h"

#include <Bitmap.h>

EMBeVideoDisplayer::EMBeVideoDisplayer()
	:	m_opPreviewWindow(NULL),
		m_vWindowShown(false),
		m_vDisableDrawBitmap(false)
{
	EMBeMediaUtility::push(this, "EMBeVideoDisplayer");
	m_vSemaphore = create_sem(1, "Video displayer semaphore");
}

EMBeVideoDisplayer::~EMBeVideoDisplayer()
{
	//delete m_opPreviewWindow;
	delete_sem(m_vSemaphore);
	EMBeMediaUtility::pop("EMBeVideoDisplayer");
}

void EMBeVideoDisplayer::HandleNativeBuffer(BBitmap* p_opBitmap)
{
	if(acquire_sem_etc(m_vSemaphore, 1, B_RELATIVE_TIMEOUT, 0) != B_WOULD_BLOCK)
	{	
		if(! m_vWindowShown)
		{
			m_opPreviewWindow = static_cast<EMVideoPreview*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_VIDEO_PREVIEW));	
			m_vWindowShown = true;
		}
		
		EMBitmap* opEMBeBitmapContainer = new EMBeBitmapContainer(p_opBitmap); //Should be deleted somewhere!?
//		delete opEMBeBitmapContainer;
//		BRect hej = p_opBitmap -> Bounds();
		
		m_opPreviewWindow -> DrawBitmap(opEMBeBitmapContainer);

		release_sem_etc(m_vSemaphore, 1, B_DO_NOT_RESCHEDULE);
	}
	else
		EMDebugger("Wouldblock in video disp");
	m_opPreviewWindow -> Unlock();//UNLOCK buffer
}

bool EMBeVideoDisplayer::LockPreviewWindow()
{
	if(! m_vWindowShown)
	{
		m_opPreviewWindow = static_cast<EMVideoPreview*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_VIDEO_PREVIEW));	
		m_vWindowShown = true;
	}

	return m_opPreviewWindow -> Lock();	
}

void EMBeVideoDisplayer::UnlockPreviewWindow()
{
	if(! m_vWindowShown)
	{
		m_opPreviewWindow = static_cast<EMVideoPreview*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_VIDEO_PREVIEW));	
		m_vWindowShown = true;
	}

	m_opPreviewWindow -> Unlock();	
}

#endif