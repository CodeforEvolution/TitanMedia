/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
*
*******************************************************/

#include "EMGlobals.h"

#ifndef __MIDI_G_CLIP
#define __MIDI_G_CLIP

//#include "EMGRubberBand.h"
//#include "EMListener.h"
#include "GClip.h"

#include <list>

class EMGClip;
class EMGTrack;
//class EMListenerRepository;
class EMMediaMIDIEvent;
class EMView;

class MIDIGClip : public GClip//, public EMListener
{
public:
	MIDIGClip(string p_oClipName, int64 p_vPosition, int64 p_vLength, EMGTrack* p_opTrack, int32 p_vMediaClipID, EMView* p_opView);
	virtual ~MIDIGClip();
	EMRect CalculateFrame(uint32 p_vTrackOffset, int64 p_vClipOffsetX);
	EMGClip* CloneClip(int32 p_vTrackOffset, int64 p_vPositionOffset);
	EMGClip* CreateInstance(string p_oClipName, int64 p_vPosition, int64 p_vLength, EMGTrack* p_opTrack, int32 p_vMediaClipID, EMView* p_opView);
	void DeselectAll();
	void DrawClip(EMRect p_oClippingRect, EMView* p_opView, bool p_vClipEditMode, int64 p_vZoomScale, bool p_vDrawContent);
	void DrawMovedClip(EMRect p_oClippingRect, EMView* p_opView, int64 p_vZoomScale, bool p_vDrawContent);
	EMMediaMIDIEvent* GetMIDIEvent(int32 p_vX, int32 p_vY);
	list<EMMediaMIDIEvent*>* GetMIDIEvents();
	void GetMIDIEvents(EMRect p_oRect, bool p_vIntersectOK, list<EMMediaMIDIEvent*>* p_opEvents);
	EMMediaType GetType();
	//EMGRubberBand* GetVelocityRubberBand();
	//bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage);
	//bool MouseDown(EMView* p_opView, int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree);
	//bool MouseMoved(EMView* p_opView, int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree);
	//bool MouseUp(EMView* p_opView, int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree);
	bool MoveClip(int32 p_vTrackOffset, int64 p_vPositionOffset);
	//void SetFrame(EMRect p_oFrame);
	void SetLabel(char* p_oLabel, EMView* p_opView);
	void UpdateContent();
	//void UpdateMIDIEvents();

private:
	void CalculateNamePosition(EMView* p_opView);

private:
	EMRect m_oInsideBorder;
	EMRect m_oLabelBackground;
	list<EMMediaMIDIEvent*> m_oMidiEvents;
	//EMGRubberBand m_oVelocityRubberBand;
	list<EMMediaMIDIEvent*>* m_opMidiEvents;
	bool m_vIsInitialized;
	bool m_vNamePositionUpdated;
	int32 m_vStringLeft;
	int32 m_vStringTop;
};

#endif
