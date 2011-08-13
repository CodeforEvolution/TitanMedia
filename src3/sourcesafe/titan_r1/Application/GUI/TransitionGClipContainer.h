/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __TRANSITION_G_CLIP_CONTAINER
#define __TRANSITION_G_CLIP_CONTAINER

#include "EMGClipContainer.h"
#include "EMRect.h"

class EMView;

class TransitionGClipContainer : public EMGClipContainer
{
public:
	TransitionGClipContainer(EMRect p_oFrame, EMView* p_opView);
	EMGClipContainer* CreateInstance();
	bool DeselectAll(EMGClipContainer* p_opNewSelectionOwner);
	void DrawAfterClips(EMRect p_oClippingRect, EMView* p_opView, EMEditMode p_eEditMode);
	void DrawSelection(EMRect p_oClippingRect, EMView* p_opView);
	void DrawTrack(EMRect p_oClippingRect, EMView* p_opView, EMEditMode p_eEditMode);
	uint8 GetLowerBorderSize();
	uint8 GetUpperBorderSize();
	void MouseDoubleClicked(EMView* p_opView, int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree, EMEditMode p_eEditMode, EMGClip* p_opClip);
	void MouseDown(EMView* p_opView, int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree, EMEditMode p_eEditMode);
	void MouseMoved(EMView* p_opView, int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree, EMEditMode p_eEditMode);
	void MouseUp(EMView* p_opView, int32 p_vX, int32 p_vY, EMEditMode p_eEditMode);
};

#endif



