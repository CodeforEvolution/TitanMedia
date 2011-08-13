/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __TRACK_WINDOW_TOOLBAR
#define __TRACK_WINDOW_TOOLBAR

#include "EMListener.h"
#include "EMProjectDataContainer.h"
#include "EMRect.h"
#include "EMSlimButton.h"
#include "EMSlimLabel.h"
#include "EMSlimValueBox.h"
#include "TimeDisplay.h"

class EMListenerRepository;
class EMView;

class TrackWindowToolBar : public EMListener, public EMProjectDataContainer
{
public:
	TrackWindowToolBar();
	~TrackWindowToolBar();
	EMRect CalculateMinimumBounds();
	bool ClearData();
	void Draw(EMRect p_oClippingRect);
	EMView* GetView();
	void Init();
	void LayoutComponents();
	void LoadComplete();
//	bool LoadData(void* p_upProjectDataLoader);
	bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage);
	void MouseDown(int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree);
	void MouseMoved(int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree);
	void MouseUp(int32 p_vX, int32 p_vY);
	void ReadyForUse();
//	bool SaveData(void* p_upProjectDataSaver);;
	void SetFastForward();
//	void SetLoop();
	void SetPlay();
	void SetProjectBegin();
	void SetProjectEnd();
	void SetRecord();
	void SetRegionBegin();
	void SetRegionEnd();
	void SetRewind();
	void SetStop();

private:
	void SetMIDIEnabled();

private:
	EMView* m_opView;
//	EMSlimButton m_oAffectAllButton;
	EMSlimButton m_oAutoQuantizeButton;
	EMSlimButton m_oEditClipsButton;
	EMSlimButton m_oEditFXButton;
	EMSlimLabel m_oEditModeLabel;
	EMSlimButton m_oEditTracksButton;
	EMSlimLabel m_oMeasuresLabel;
	EMSlimLabel m_oPositionLabel;
	EMSlimLabel m_oQuantizeLabel;
	EMSlimValueBox m_oQuantizeModeValueBox;
//	EMSlimButton m_oRelativeButton;
	EMSlimLabel m_oSignatureLabel;
	EMSlimValueBox m_oSignatureValueBox;
	EMSlimButton m_oSnapEnableButton;
	EMSlimLabel m_oSnapLabel;
	EMSlimValueBox m_oSnappingModeValueBox;
	EMSlimLabel m_oTempoLabel;
	EMSlimValueBox m_oTempoValueBox;
	TimeDisplay m_oTimeDisplayOne;
	TimeDisplay m_oTimeDisplayTwo;
	EMSlimButton m_oTransportBeginningButton;
	EMSlimButton m_oTransportEndButton;
	EMSlimButton m_oTransportFastForwardButton;
	EMSlimLabel m_oTransportLabel;
	EMSlimButton m_oTransportLoopButton;
	EMSlimButton m_oTransportPlayButton;
	EMSlimButton m_oTransportRecordButton;
	EMSlimButton m_oTransportRegionStartButton;
	EMSlimButton m_oTransportRewindButton;
	EMSlimButton m_oTransportRegionEndButton;
	EMSlimButton m_oTransportStopButton;
	bool m_vLoopButtonEnabled;
	bool m_vMIDIEnabled;
//	bool m_vRelativeMove; // Cached setting
	string* m_vSignature; // Cached setting
	float m_vTempo; // Cached setting
};

#endif

