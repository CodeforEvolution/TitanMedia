/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
*
*******************************************************/

#include "EMGlobals.h"

#ifndef __VIDEO_PREVIEW_WINDOW
#define __VIDEO_PREVIEW_WINDOW

#include "EMListener.h"
#include "EMVideoPreview.h"

class EMListenerRepository;
class EMView;
class EMWindow;

class VideoPreviewWindow : public EMVideoPreview, public EMListener
{
public:
	VideoPreviewWindow();
	~VideoPreviewWindow();
	void DrawBitmap(EMBitmap* p_opBitmap);
	void HidePreview();
	bool Lock();
	bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage);
	void ShowPreview();
	void Unlock();

private:
	EMBitmap* m_opBitmap;
	EMWindow* m_opWindow;
	EMView* m_opView;
};

#endif