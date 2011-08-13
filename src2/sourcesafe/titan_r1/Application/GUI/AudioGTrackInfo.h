/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __AUDIO_G_TRACK_INFO
#define __AUDIO_G_TRACK_INFO

#include "GTrackInfo.h"

class EMGTrack;
class EMView;
class TrackInfoButtonBar;

class AudioGTrackInfo : public GTrackInfo
{
public:
	AudioGTrackInfo(EMView* p_opView, TrackInfoButtonBar* p_opButtonBar);//, EMGTrack* p_opTrack);
//	~AudioGTrackInfo();
//	void CalculateAndSetHidden(bool p_vClipEdit);
	EMGTrackInfo* CreateInstance();
	void EditModeChanged(EMEditMode p_eEditMode);
	EMMediaType GetType();
	bool IsHidden(bool p_vClipEdit);
//	void SetUserMode(bool p_vExpertMode);

private:

private:
};

#endif

