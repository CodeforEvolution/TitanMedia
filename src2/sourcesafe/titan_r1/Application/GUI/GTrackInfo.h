/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __G_TRACK_INFO
#define __G_TRACK_INFO

#include "CommandIDs.h"
#include "EMCommandRepository.h"
#include "EMGTrackInfo.h"
#include "EMListener.h"
#include "EMSignalMeter.h"
#include "EMSlimButton.h"
#include "EMSlimEditLabel.h"
#include "EMSlimValueBox.h"
#include "MuteButton.h"
#include "SelectionButton.h"
#include "SoloButton.h"

#include <list>

class EMListenerRepository;
class EMMenuBar;
class EMView;
class MuteButton;
class SoloButton;
class TrackInfoButtonBar;

const int32 BOTTOM_BORDER_SIZE = 3;
const int32 TINY_SIGNAL_METER_WIDTH = 30;

class GTrackInfo : public EMGTrackInfo, public EMListener
{
public:
	GTrackInfo(EMView* p_opView, TrackInfoButtonBar* p_opButtonBar);//, EMGTrack* p_opTrack);
	virtual ~GTrackInfo();
	void Draw(EMRect p_oClippingRect, EMView* p_opView);
	EMSlimButton* GetArmButton();
	MuteButton* GetMuteButton();
	SelectionButton* GetSelectionButton();
	SoloButton* GetSoloButton();
	string GetTrackName();
	bool IsSelected();
//	bool IsVisible();
	void LayoutComponents();
	bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage);
	void MouseDown(int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree, EMView* p_opView);
	void MouseDoubleClick(int32 p_vX, int32 p_vY, EMView* p_opView);
	void MouseMoved(int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree);
	void MouseUp(int32 p_vX, int32 p_vY);
	void SetMediaTrackID(int32 p_vMediaTrackID);
	void SetRenderButton(bool p_vOn);
	void SetSelected(bool p_vSelected, bool p_vNotify = true);
	void SetSignal(float p_vSignal);
	void SetSignal(float* p_vpSignal);//list<int32>* p_vSignal);
	void SetTrack(EMGTrack* p_opTrack);
	void SetTrackName(string p_oName);
	virtual void SetUserMode(bool p_vExpertMode);

protected:
	EMZoomScale m_ePreviousZoomScale;
	EMSlimButton m_oArmButton;
	EMSlimEditLabel m_oEditLabel;
	EMSlimValueBox m_oInputValueBox;
	EMSlimValueBox m_oKeyOffsetValueBox;
	EMSlimButton m_oLockButton;
	MuteButton m_oMuteButton;
	EMSlimValueBox m_oOutputValueBox;
	EMSlimValueBox m_oPanValueBox;
	TrackInfoButtonBar* m_opButtonBar;
	EMView* m_opView;
	EMSlimButton m_oRenderButton;
	SelectionButton m_oSelectionButton;
	EMSignalMeter m_oSignalMeter;
	SoloButton m_oSoloButton;
	EMSlimValueBox m_oTimeOffsetValueBox;
	EMSlimValueBox m_oVelocityOffsetValueBox;
	EMSlimValueBox m_oVolumeValueBox;
	EMRect m_oEmptySpaceOne; // The region beneath the name and row of buttons
	EMRect m_oEmptySpaceTwo; // The region beneath the value boxes
	bool m_vPreviousSignalOn;
	bool m_vUpdateAll; // Makes sure that everything is initialized at start

private:
	uint32 m_vMessageInputChanged;
	uint32 m_vMessageOutputChanged;
};

#endif

