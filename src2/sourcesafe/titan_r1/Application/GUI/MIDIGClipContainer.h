/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __MIDI_G_CLIP_CONTAINER
#define __MIDI_G_CLIP_CONTAINER

#include "EMGClipContainer.h"
#include "EMListener.h"
#include "EMMIDIGlobals.h"
#include "EMRect.h"
#include "SelectionButton.h"

#include <list>

class EMGRubberBand;
class EMListenerRepository;
class EMMediaMIDIEvent;
class EMView;
class MIDIGClip;

class MIDIGClipContainer : public EMGClipContainer, public EMListener
{
public:
	MIDIGClipContainer(EMRect p_oFrame, EMView* p_opView);
	~MIDIGClipContainer();
	EMGClipContainer* CreateInstance();
	bool DeselectAll(EMGClipContainer* p_opNewSelectionOwner);
	void DrawAfterClips(EMRect p_oClippingRect, EMView* p_opView, EMEditMode p_eEditMode);
	void DrawSelection(EMRect p_oClippingRect, EMView* p_opView);
	void DrawTrack(EMRect p_oClippingRect, EMView* p_opView, EMEditMode p_eEditMode);
	ClipEditDisplayMode GetDisplayMode();
	uint8 GetLowerBorderSize();
	int GetLowestKey();
	EMGRubberBand* GetRubberband();
	list<EMMediaMIDIEvent*>* GetSelectedMIDIEvents();
	uint8 GetUpperBorderSize();
	bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage);
	void MouseDoubleClicked(EMView* p_opView, int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree, EMEditMode p_eEditMode, EMGClip* p_opClip);
	void MouseDown(EMView* p_opView, int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree, EMEditMode p_eEditMode);
	void MouseMoved(EMView* p_opView, int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree, EMEditMode p_eEditMode);
	void MouseUp(EMView* p_opView, int32 p_vX, int32 p_vY, EMEditMode p_eEditMode);
	void RemoveClip(EMGClip* p_opClip);
	bool SetFrame(EMRect p_oFrame);
	void SetTrack(EMGTrack* p_opTrack);
	void UpdateEventsInClips();
	void UpdateSelection();
	void UpdateTimeScale(int64 p_vTimeScale);

private:
	void BuildRubberBand(EMGRubberBand* p_opRubberBand, list<EMMediaMIDIEvent*>* p_opEvents, EMMIDIEventType p_eType, 
			EMMIDIControlChangeType p_eControlChangeType = EM_MIDI_CONTROL_CHANGE_TYPE_BANK_SELECT_MSB);
	EMRect CalculateEventFrame(list<EMMediaMIDIEvent*>* p_opEvents);
	void ChangeDisplayMode();
	void Scroll(int p_vLowestKey);

private:
	ClipEditDisplayMode m_eDisplayMode;
	static EMRect m_oMovedRect;
	static EMRect m_oOldMovedRect;
	MIDIGClip* m_opMouseDownClip;
	EMMediaMIDIEvent* m_opMouseDownEvent;
	EMGRubberBand* m_opRubberBand;
	SelectionButton* m_opSelectionButton;
	//EMView* m_opView;
	static EMRect m_oSelectedMIDIEventFrame;
	list<EMMediaMIDIEvent*> m_oSelectedMIDIEvents;
	int64 m_vDrawEventsLength;
	uint8 m_vDrawEventsVelocity;
	bool m_vHasResized;
	//int32 m_vKeyOffset;
	int64 m_vOriginalLength;
	int64 m_vOriginalPosition;
	int64 m_vMouseDownPos;
	int32 m_vMouseDownY;
	bool m_vMouseIsDown;
	int64 m_vMovedPosition;
	bool m_vMoveDrawedEvent;
	bool m_vResizeLeft;
	int64 m_vTimeZoomScale; // Cached value
};

#endif



