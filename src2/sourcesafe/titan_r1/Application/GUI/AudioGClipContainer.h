/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __AUDIO_G_CLIP_CONTAINER
#define __AUDIO_G_CLIP_CONTAINER

#include "EMGClipContainer.h"
#include "EMListener.h"
#include "EMRect.h"
#include "FXNode.h"
#include "SelectionButton.h"

#include <list>

class AudioGClip;
class EMGRubberBand;
class EMView;
class EMSlimValueBox;

class AudioGClipContainer : public EMGClipContainer, public EMListener
{
public:
	AudioGClipContainer(EMRect p_oFrame, EMView* p_opView);
	~AudioGClipContainer();
	void AddClip(EMGClip* p_opClip);
	void BuildFXNodes(list<int32>* p_opMediaEffectIDs);
	void ClearClips();
	EMGClipContainer* CreateInstance();
	bool DeselectAll(EMGClipContainer* p_opNewSelectionOwner);
	void DrawAfterClips(EMRect p_oClippingRect, EMView* p_opView, EMEditMode p_eEditMode);
	void DrawSelection(EMRect p_oClippingRect, EMView* p_opView);
	void DrawTrack(EMRect p_oClippingRect, EMView* p_opView, EMEditMode p_eEditMode);
	ClipEditDisplayMode GetDisplayMode();
	uint8 GetLowerBorderSize();
	EMGRubberBand* GetRubberband();
	list<AudioGClip*>* GetSelectedClips(int64* p_vpSelectionBegin, int64* p_vpSelectionEnd);
	uint8 GetUpperBorderSize();
	bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage);
	void MouseDoubleClicked(EMView* p_opView, int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree, EMEditMode p_eEditMode, EMGClip* p_opClip);
	void MouseDown(EMView* p_opView, int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree, EMEditMode p_eEditMode);
	void MouseMoved(EMView* p_opView, int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree, EMEditMode p_eEditMode);
	void MouseUp(EMView* p_opView, int32 p_vX, int32 p_vY, EMEditMode p_eEditMode);
	void RemoveClip(EMGClip* p_opClip);
	void SetCurrentFXChain(const char* p_opString);
	bool SetFrame(EMRect p_oFrame);
	void SetTrack(EMGTrack* p_opTrack);
	void UpdateFXNodes();
	//void UpdateRepositories();
	void UpdateTimeScale(int64 p_vTimeScale);
	void UpdateValueBox();

private:
	void InitValueBox();
	void ChangeDisplayMode();

private:
	ClipEditDisplayMode m_eDisplayMode;
	list<AudioGClip*> m_oAffectedClips; // Used only to be able to return clips in GetSelectedClips
	list<FXNode> m_oTrackFXNodes;
	list<FXNode>* m_opCurrentFXChain;
	EMSlimValueBox* m_opFXValueBox;
	EMGRubberBand* m_opRubberBand;
	SelectionButton* m_opSelectionButton;
	//EMView* m_opView;
	bool m_vHasBeenDoubleClicked;
	uint32 m_vMessageFXChainChanged;
	int32 m_vMouseDownY;
	bool m_vMouseIsDown;
};

#endif




