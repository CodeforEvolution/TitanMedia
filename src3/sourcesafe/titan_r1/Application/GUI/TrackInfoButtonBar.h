/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __TRACK_INFO_BUTTON_BAR
#define __TRACK_INFO_BUTTON_BAR

#include "EMListener.h"
#include "EMListenerRepository.h"
#include "EMProjectDataContainer.h"
#include "EMRect.h"
#include "EMSlimButton.h"
#include "EMSlimValueBox.h"

#include <string>

class EMListenerRepository;
class EMView;

class TrackInfoButtonBar : public EMListener, public EMListenerRepository, public EMProjectDataContainer
{
public:
	TrackInfoButtonBar();
	~TrackInfoButtonBar();
	void Draw(EMRect p_oClippingRect);
	bool GetAudioTrackFilterOn();
	bool GetChannelOn();
	bool GetEffectTrackFilterOn();
	bool GetExpandOn();
	bool GetInputOn();
	bool GetKeyOffsetOn();
	bool GetMIDITrackFilterOn();
	bool GetOutputOn();
	bool GetPanOn();
	bool GetSignalOn();
	bool GetTimeOffsetOn();
	bool GetTransitionTrackFilterOn();
	bool GetVelocityOffsetOn();
	EMView* GetView();
	bool GetVideoTrackFilterOn();
	bool GetVolumeOn();
	void Init();
	static TrackInfoButtonBar* Instance();
	bool IsDirty();
	bool LoadData(void* p_upProjectDataLoader);
	bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage);
	EMRect PreferredBounds();
	void ReadyForUse();
	bool SaveData(void* p_upProjectDataSaver);;
	void SetExpand();
	
private:
	void SetUserMode();
	void LayoutButtons();
	void MouseDown(int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree);
//	void MouseMoved(int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree);
//	void MouseUp(int32 p_vX, int32 p_vY);

private:
	EMView* m_opView;
	EMSlimButton m_oAudioTrackFilterButton;
	EMSlimButton m_oChannelButton;
	EMRect m_oDynamicRect;
	EMSlimButton m_oEffectTrackFilterButton;
	EMRect m_oEmptySpaceOne;
//	EMRect m_oEmptySpaceTwo;
	EMSlimButton m_oExpandButton;
	EMSlimButton m_oInputButton;
	static TrackInfoButtonBar* m_opInstance;
	EMSlimButton m_oKeyOffsetButton;
	EMSlimButton m_oMIDITrackFilterButton;
	EMSlimButton m_oOutputButton;
	EMSlimButton m_oPanButton;
	EMSlimButton m_oSignalButton;
	EMSlimButton m_oTimeOffsetButton;
	EMSlimButton m_oTransitionTrackFilterButton;
	EMSlimButton m_oVelocityOffsetButton;
	EMSlimButton m_oVideoTrackFilterButton;
	EMSlimButton m_oVolumeButton;
	bool m_vAudioEnabled;
	bool m_vExpertMode;
	bool m_vIsDirty;
	bool m_vIsExpanded;
	bool m_vMIDIEnabled;
	bool m_vVideoEnabled;
};

#endif