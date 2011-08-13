/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __MIDI_G_TRACK_INFO
#define __MIDI_G_TRACK_INFO

#include "GTrackInfo.h"

class EMGTrack;
class EMView;
class TrackInfoButtonBar;

class MIDIGTrackInfo : public GTrackInfo
{
public:
	MIDIGTrackInfo(EMView* p_opView, TrackInfoButtonBar* p_opButtonBar);
	~MIDIGTrackInfo();
	EMGTrackInfo* CreateInstance();
	void Draw(EMRect p_oClippingRect, EMView* p_opView);
	void EditModeChanged(EMEditMode p_eEditMode);
	static MIDIGTrackInfo* GetActiveTrackInfo();
	EMMediaType GetType();
	bool IsHidden(bool p_vClipEdit);
	void MouseDown(int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree, EMView* p_opView);
	void SetActive(bool p_vActive);
//	void SetUserMode(bool p_vExpertMode);

private:
	bool m_vIsActive; // If this is the current active MIDI track
	static MIDIGTrackInfo* m_opActiveTrackInfo;
};

#endif

