/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __TRANSITION_G_TRACK_INFO
#define __TRANSITION_G_TRACK_INFO

#include "GTrackInfo.h"
//#include "EMGTrackInfo.h"

class EMGTrack;
class EMView;
class TrackInfoButtonBar;

class TransitionGTrackInfo : public GTrackInfo
{
public:
	TransitionGTrackInfo(EMView* p_opView, TrackInfoButtonBar* p_opButtonBar);
	EMGTrackInfo* CreateInstance();
//	void Draw(EMRect p_oClippingRect, EMView* p_opView);
	void EditModeChanged(EMEditMode p_eEditMode);
	//string GetTrackName();
	EMMediaType GetType();
	EMGTrack* GetVideoTrack();
	bool IsHidden(bool p_vClipEdit);
//	bool IsSelected();
//	void LayoutComponents();
/*
	void MouseDown(int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree, EMView* p_opView);
	void MouseDoubleClick(int32 p_vX, int32 p_vY, EMView* p_opView);
	void MouseMoved(int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree);
	void MouseUp(int32 p_vX, int32 p_vY);
	void SetSelected(bool p_vSelected, bool p_vNotify = true);
*/
	void SetSignal(list<float>* p_vSignal);
//	void SetTrackName(string p_oName);
	void SetUserMode(bool p_vExpertMode);
	void SetVideoTrack(EMGTrack* p_opVideoTrack);

private:
	EMGTrack* m_opVideoTrack;
};

#endif
/*
	MuteButton* GetMuteButton();
	SelectionButton* GetSelectionButton();
	SoloButton* GetSoloButton();
	string GetTrackName();
	bool IsVisible();
	bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage);
	void SetMediaTrackID(int32 p_vMediaTrackID);
	void SetRenderButton(bool p_vOn);
	void SetTrack(EMGTrack* p_opTrack);
	void SetUserMode(bool p_vExpertMode);
*/
