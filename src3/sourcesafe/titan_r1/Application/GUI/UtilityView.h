/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __UTILITY_VIEW
#define __UTILITY_VIEW

#include "EMListener.h"
#include "EMProjectDataContainer.h"
#include "EMRect.h"
#include "EMSlimButtonView.h"
#include "VideoPreviewView.h"

class AudioInputSignalView;
class ColaborationView;
class EMListenerRepository;
class EMVideoPreview;
class EMView;
class ExplorerView;
class MediaPoolView;
class TrackWindow;

class UtilityView : public EMListener, public EMProjectDataContainer
{
public:
	UtilityView();
	~UtilityView();
	void ButtonPressed(EMSlimButton* p_opButton);
	bool ClearData();
	void Draw(EMRect p_oClippingRect);
	int32 GetDefaultHeight();
	int32 GetHeight();
	EMVideoPreview* GetVideoPreviewView();
	EMView* GetView();
	void Init();
	bool IsDirty();
	bool LoadData(void* p_upProjectDataLoader);
	bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage);
	void ReadyForUse();
	bool SaveData(void* p_upProjectDataSaver);;
	void SetHeight(int32 p_vHeight);
	void SetWindow(TrackWindow* p_opWindow);
	
private:
	void DrawBorder(EMRect p_oClippingRect);
	void LayoutViews();

private:
	EMView* m_opView;
	EMSlimButtonView m_oAudioInputButtonL;
	EMSlimButtonView m_oAudioInputButtonR;
	EMSlimButtonView m_oFileButtonL;
	EMSlimButtonView m_oFileButtonR;
	EMSlimButtonView m_oInternetButtonL;
	EMSlimButtonView m_oInternetButtonR;
	EMSlimButtonView m_oMediaPoolButtonL;
	EMSlimButtonView m_oMediaPoolButtonR;
	AudioInputSignalView* m_opAudioInputSignalView;
//	ColaborationView* m_opColaborationView;
//	ExplorerView* m_opExplorerView;
	EMSlimButton* m_opLastButtonPressed;
//	MediaPoolView* m_opMediaPoolView;
	EMView* m_opVBorder;
	VideoPreviewView* m_opVideoPreviewView;
	TrackWindow* m_opWindow;
	EMSlimButtonView m_oVideoPreviewButtonL;
	EMSlimButtonView m_oVideoPreviewButtonR;
	int32 m_vHeight;
	bool m_vIsDirty;
	int32 m_vMouseDownY;
	bool m_vMouseIsDown;
	int32 m_vOriginalHeight;
};

#endif