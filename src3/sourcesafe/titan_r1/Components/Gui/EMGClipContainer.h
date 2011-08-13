/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_G_CLIP_CONTAINER
#define __EM_G_CLIP_CONTAINER

#include "EMGClip.h"
#include "EMGUIGlobals.h"
//#include "EMListener.h"
//#include "EMListenerRepository.h"
#include "EMRect.h"

#include <list>

class EMGClip;
class EMGTrack;
//class EMListenerRepository;
class EMView;

class EMGClipContainer// : public EMListener//, public EMListenerRepository
{
public:
	virtual ~EMGClipContainer();
	virtual void AddClip(EMGClip* p_opClip);
	EMRect CalculateClipFrame(EMGClip* p_opClip);
	EMRect CalculateClipFrame(EMGClip* p_opClip, int64 p_vClipOffsetX);
	virtual void ClearClips();
	virtual EMGClipContainer* CreateInstance() = 0;
	virtual bool DeselectAll(EMGClipContainer* p_opNewSelectionOwner) = 0;
	virtual void DrawAfterClips(EMRect p_oClippingRect, EMView* p_opView, EMEditMode p_eEditMode) = 0;
	virtual void DrawSelection(EMRect p_oClippingRect, EMView* p_opView) = 0;
	virtual void DrawTrack(EMRect p_oClippingRect, EMView* p_opView, EMEditMode p_eEditMode) = 0;
	EMRect Frame();
	EMRect GetAndClearUpdateRect();
	EMGClip* GetClip(int32 p_vX, int32 p_vY);
	static EMGClipContainer* GetClipContainerWithSelection();
	list<EMGClip*>* GetClips();
	virtual uint8 GetLowerBorderSize() = 0;
	EMGTrack* GetTrack();
	virtual uint8 GetUpperBorderSize() = 0;
	virtual void MouseDoubleClicked(EMView* m_opView, int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree, EMEditMode p_eEditMode, EMGClip* p_opClip) = 0;
	virtual void MouseDown(EMView* m_opView, int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree, EMEditMode p_eEditMode) = 0;
	virtual void MouseMoved(EMView* m_opView, int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree, EMEditMode p_eEditMode) = 0;
	virtual void MouseUp(EMView* m_opView, int32 p_vX, int32 p_vY, EMEditMode p_eEditMode) = 0;
	bool OkToDrop(EMGClip* p_opClip, int64 p_vPosition, int64 p_vLength, bool p_vAlwaysIncludeSelected = false);
	virtual void RemoveClip(EMGClip* p_opClip);
	virtual bool SetFrame(EMRect p_oFrame);
	virtual void SetTrack(EMGTrack* p_opTrack);
	//virtual void UpdateRepositories();
	virtual void UpdateTimeScale(int64 p_vTimeScale);

protected:
	EMGClipContainer(EMRect p_oFrame, EMView* p_opView);

protected:
	list<EMGClip*> m_oClipRepository;
	static EMRect m_oOldSelectionRect;
	static EMGClipContainer* m_opClipContainerWithSelection;
	static EMRect m_oSelectionRect;
	EMView* m_opView;
	EMRect m_oUpdateRect;
//	static int32 m_vMouseDownX;
	static int64 m_vMouseDownXPos;
	static int64 m_vSelectionLeftXPos;
	static int64 m_vSelectionRightXPos;
//	static int32 m_vSnappedMouseDownX;
	int64 m_vTimeZoomScale; // Cached setting

private:
	EMRect m_oFrame;
	EMGTrack* m_opTrack;
};

#endif
