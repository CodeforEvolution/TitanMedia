/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __TRACK_INFO
#define __TRACK_INFO

#include "EMGTrackInfo.h"
#include "EMGUIGlobals.h"
#include "EMListener.h"
#include "EMProjectDataContainer.h"
#include "EMRect.h"
#include "EMThreadListener.h"

#include <list>
#include <map>

class EMGTrack;
class EMListenerRepository;
class EMScrollbar;
class EMView;

class TrackInfo : public EMListener, public EMThreadListener, public EMProjectDataContainer
{
public:
	TrackInfo();
	~TrackInfo();
	void CalculateAndSetTrackInfos();
	EMRect CalculateTrackInfoFrame(EMGTrack* p_opTrack);
	void CalculateTrackInfoFramesAndUpdate(EMGTrack* p_opTrack);
	bool ClearData();
	void Draw(EMRect p_oClippingRect);
	void EnableMenuItems();
	EMView* GetView();
	void Init();
//	bool IsDirty();
	void LoadComplete();
	bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage);
	void PrepareToClearData();
	void ReadyForUse();
//	void SaveComplete();
	void SetScrollbar(EMScrollbar* p_opScrollbarVertical);
//	void SetFrame(EMRect p_oFrame);
	void ThreadRun(EMThread* p_opThread);

private:
	void ChangeEditMode();
	void MouseDown(int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree);
	void MouseDoubleClick(int32 p_vX, int32 p_vY);
	void MouseMoved(int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree);
	void MouseUp(int32 p_vX, int32 p_vY);
	void SetUserMode();

private:
	EMEditMode m_eEditMode;
//	map<int32, list<float>*> m_oAudioSignalValues;
//	map<int32, bool> m_oMIDISignalValues;
	EMRect m_oPreviousSelectRect;
	list<EMGTrackInfo*> m_oAudioTrackInfos;
	EMRect m_oEmptySpace;
	list<EMGTrackInfo*> m_oMIDITrackInfos;
	EMGTrack* m_opMouseDownTrack;
	EMScrollbar* m_opScrollbarVertical;
	EMView* m_opView;
	float m_vAudioTrackSignalValues[100][5];
	bool m_vDrawedOnButtonBar;
	bool m_vExpertMode;
//	bool m_vIsDirty;
//	int32 m_vMIDITrackIDs[100];
	float m_vMIDITrackSignalValues[100];
	int32 m_vMouseDownY;
	int32 m_vScrolledOffsetY; // Used for scrolling the display
	bool m_vSignal; // Cached setting
};

#endif

