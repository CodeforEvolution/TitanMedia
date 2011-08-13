/*******************************************************
* Creator: Jesper Svensson
* Portability: native
*-------------------------------------------------------
* Displays the input comming from the
* EMBeVideoConsumerNode
*******************************************************/

#include "EMGlobals.h"

#ifdef PLATFORM_BEOS

#include <MediaKit.h>

#ifndef __EM_BE_VIDEO_DISPLAYER
#define __EM_BE_VIDEO_DISPLAYER

//#include "EMThread.h"
//#include "EMThreadListener.h"

class EMVideoPreview;

class EMBeVideoDisplayer
{
public:
	EMBeVideoDisplayer();
	~EMBeVideoDisplayer();
	void HandleNativeBuffer(BBitmap* m_spBitmap);
	bool LockPreviewWindow();
	void UnlockPreviewWindow();
	
private:
	EMVideoPreview* m_opPreviewWindow;
	bool m_vWindowShown;
	bool m_vDisableDrawBitmap;
	sem_id m_vSemaphore;
};

#endif

#endif
