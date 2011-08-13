/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __VIDEO_G_CLIP_CONTAINER
#define __VIDEO_G_CLIP_CONTAINER

#include "EMGClipContainer.h"
#include "EMListener.h"
#include "EMRect.h"
#include "FXNode.h"

class EMView;

class VideoGClipContainer : public EMGClipContainer, public EMListener
{
public:
	VideoGClipContainer(EMRect p_oFrame, EMView* p_opView);
	~VideoGClipContainer();
	void AddClip(EMGClip* p_opClip);
	void BuildFXNodes(list<int32>* p_opMediaEffectIDs);
	void ClearClips();
	EMGClipContainer* CreateInstance();
	bool DeselectAll(EMGClipContainer* p_opNewSelectionOwner);
	void DrawAfterClips(EMRect p_oClippingRect, EMView* p_opView, EMEditMode p_eEditMode);
	void DrawSelection(EMRect p_oClippingRect, EMView* p_opView);
	void DrawTrack(EMRect p_oClippingRect, EMView* p_opView, EMEditMode p_eEditMode);
	uint8 GetLowerBorderSize();
	uint8 GetUpperBorderSize();
	bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage);
	void MouseDoubleClicked(EMView* p_opView, int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree, EMEditMode p_eEditMode, EMGClip* p_opClip);
	void MouseDown(EMView* m_opView, int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree, EMEditMode p_eEditMode);
	void MouseMoved(EMView* m_opView, int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree, EMEditMode p_eEditMode);
	void MouseUp(EMView* m_opView, int32 p_vX, int32 p_vY, EMEditMode p_eEditMode);
	void RemoveClip(EMGClip* p_opClip);
	void SetCurrentFXChain(const char* p_opString);
	bool SetFrame(EMRect p_oFrame);
	void UpdateFXNodes();
	//void UpdateRepositories();
	void UpdateValueBox();

private:
	void InitValueBox();

private:
	list<FXNode> m_oTrackFXNodes;
	list<FXNode>* m_opCurrentFXChain;
	EMSlimValueBox* m_opFXValueBox;
	uint32 m_vMessageFXChainChanged;
	bool m_vMouseIsDown;
};

#endif



