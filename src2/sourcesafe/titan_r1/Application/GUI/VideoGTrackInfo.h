/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __VIDEO_G_TRACK_INFO
#define __VIDEO_G_TRACK_INFO

#include "GTrackInfo.h"

class EMGTrack;
class EMView;
class TrackInfoButtonBar;

class VideoGTrackInfo : public GTrackInfo
{
public:
	VideoGTrackInfo(EMView* p_opView, TrackInfoButtonBar* p_opButtonBar);
//	void CalculateAndSetHidden(bool p_vClipEdit);
	EMGTrackInfo* CreateInstance();
	void EditModeChanged(EMEditMode p_eEditMode);
	EMGTrack* GetTransitionTrack();
	EMMediaType GetType();
	bool IsHidden(bool p_vClipEdit);
	void SetTransitionTrack(EMGTrack* p_opTransitionTrack);
//	void SetUserMode(bool p_vExpertMode);

private:
	EMGTrack* m_opTransitionTrack;
};

#endif

