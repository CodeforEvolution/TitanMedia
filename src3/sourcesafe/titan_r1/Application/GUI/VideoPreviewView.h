/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
*
*******************************************************/

#include "EMGlobals.h"

#ifndef __VIDEO_PREVIEW_VIEW
#define __VIDEO_PREVIEW_VIEW

#include "EMListener.h"
#include "EMRect.h"
#include "EMThreadListener.h"
#include "EMVideoPreview.h"

class EMListenerRepository;
class EMView;
class EMWindow;

class VideoPreviewView : public EMVideoPreview, public EMListener, public EMThreadListener
{
public:
	VideoPreviewView();
	~VideoPreviewView();
	EMRect Bounds();
	void DrawBitmap(EMBitmap* p_opBitmap); // Needed?
	EMView* GetView();
	void HidePreview();
	void Init();
	bool Lock(); // Needed?
	//void LockIfNeededAndDrawBitmap(EMBitmap* p_opBitmap);
	bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage);
	void ShowPreview();
	void ThreadRun(EMThread* p_opThread);
	void Unlock(); // Needed?

private:
	void CalculateBitmapBounds();

private:
	EMRect m_oBitmapBounds;
	EMBitmap* m_opBitmap;
	EMView* m_opView;
	float m_vFramerate;
	int m_vHeight;
	int m_vOriginalHeight;
	int m_vOriginalWidth;
	int m_vWidth;
};

#endif