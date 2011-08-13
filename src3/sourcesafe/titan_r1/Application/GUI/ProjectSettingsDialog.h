/*******************************************************
* This code is the copyrighted property of Elegant Media
*-------------------------------------------------------
* Creator: Johan Dunberg
* Component: Application, Dialog
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/
#include "EMGlobals.h"

#ifndef __PROJECT_SETTINGS_DIALOG
#define __PROJECT_SETTINGS_DIALOG

#include "EMListener.h"
#include <map>
typedef pair<int, int> ResolutionPair;

class EMButton;
class EMCheckBox;
class EMComboBox;
class EMFactory;
class EMGroupBox;
class EMLabel;
class EMListenerRepository;
class EMRadioButton;
class EMTabControl;
class EMTextControl;
class EMTrackbarControl;
class EMView;
class EMWindow;

class ProjectSettingsDialog : public EMListener
{
public:
	ProjectSettingsDialog();
	~ProjectSettingsDialog();
	void DisplayDialog();
	void EnableMidiNote(bool p_vEnable);
	void HideDialog();
	void Init();
	bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage);
	void MoveDialog();
	void MoveDialog(float p_vX, float p_vY);
	void SaveSettings();
	void SetMidiKeyInComboBox(EMComboBox* p_opComboBox);
	void SetNumberInComboBox(EMComboBox* p_opComboBox, int16 p_vNumberOfNumbers, int16 p_vStartValue = 0);
	void SetParent(EMWindow* p_opWindow);
	char* SetValidValue(char* p_vpValue);

private:
	// Main View
	EMWindow*			m_opDialogWindow;
	EMView*				m_opMainView;
	EMTabControl*		m_opTabControl;
	EMButton*			m_opOK_Button;
	EMButton*			m_opCancelButton;
	EMWindow*			m_opParentWindow;

	// Video Tab - Components
	EMGroupBox*			m_opBackgroundColorGroupBox;
	EMView*				m_opBackgroundColorView;
	EMLabel*			m_opBackgroundColorBlueLabel;
	EMTextControl*		m_opBackgroundColorBlueTextControl;
	EMTrackbarControl*	m_opBackgroundColorBlueTrackbarControl;
	EMLabel*			m_opBackgroundColorGreenLabel;
	EMTextControl*		m_opBackgroundColorGreenTextControl;
	EMTrackbarControl*	m_opBackgroundColorGreenTrackbarControl;
	EMLabel*			m_opBackgroundColorRedLabel;
	EMTextControl*		m_opBackgroundColorRedTextControl;
	EMTrackbarControl*	m_opBackgroundColorRedTrackbarControl;
	EMView*				m_opBackgroundColorPreviewView;
	EMComboBox*			m_opDimensionComboBox;
	EMGroupBox*			m_opDimensionGroupBox;
	EMRadioButton*		m_opDimensionPreDefineRadioButton;
	EMRadioButton*		m_opDimensionUserDefineRadioButton;
	EMView*				m_opDimensionView;
	EMGroupBox*			m_opFrameRateGroupBox;
	EMComboBox*			m_opFrameRateComboBox;
	EMRadioButton*		m_opFrameRatePreDefineRadioButton;
	EMTextControl*		m_opFrameRateTextControl;
	EMRadioButton*		m_opFrameRateUserDefineRadioButton;
	EMView*				m_opFrameRateView;
	EMLabel*			m_opHeightLabel;
	EMTextControl*		m_opHeightTextControl;
	EMCheckBox*			m_opVideoAspectRatiosCheckBox;
	EMComboBox*			m_opVideoAspectRatiosComboBox;
	EMView*				m_opVideoView;
	EMLabel*			m_opWidthLabel;
	EMTextControl*		m_opWidthTextControl;
	// Video Tab - Variables
	map<int, ResolutionPair> m_oAspectRatiosMap;
	map<int, float> m_oFrameRateMap;
	map<int, ResolutionPair> m_oResolutionMap;
	bool m_vAlreadyChanged;
	bool m_vAspectRatiosIsChecked;
	bool m_vSelectionStartBeforeDecimal;
	bool m_vFrameRateChanged;
	int m_vDecimalPosition;
//	bool m_vDecimalExist;
	int m_vFrameRateLength;
	bool m_vHeightChanged;
//	bool m_vNoNeedToCheck;
	int m_vSelectionStart;
	char m_vTemp[4];
	bool m_vWidthChanged;

	// Metronome Tab - Components
	EMComboBox*		m_opMetronomeCountInComboBox;
	EMLabel*		m_opMetronomeCountInLabel;
	EMComboBox*		m_opMetronomeFirstBeatDurationComboBox;
	EMLabel*		m_opMetronomeFirstBeatDurationLabel;
	EMGroupBox*		m_opMetronomeFirstBeatGroupBox;
	EMComboBox*		m_opMetronomeFirstBeatKeyComboBox;
	EMLabel*		m_opMetronomeFirstBeatKeyLabel;
	EMComboBox*		m_opMetronomeFirstBeatVelocityComboBox;
	EMLabel*		m_opMetronomeFirstBeatVelocityLabel;
	EMView*			m_opMetronomeFirstBeatView;
	EMComboBox*		m_opMetronomeMidiNoteChannelComboBox;
	EMLabel*		m_opMetronomeMidiNoteChannelLabel;
	EMComboBox*		m_opMetronomeMidiNoteDeviceComboBox;
	EMLabel*		m_opMetronomeMidiNoteDeviceLabel;
	EMGroupBox*		m_opMetronomeMidiNoteGroupBox;
	EMView*			m_opMetronomeMidiNoteView;
	EMComboBox*		m_opMetronomeOtherBeatDurationComboBox;
	EMLabel*		m_opMetronomeOtherBeatDurationLabel;
	EMGroupBox*		m_opMetronomeOtherBeatGroupBox;
	EMComboBox*		m_opMetronomeOtherBeatKeyComboBox;
	EMLabel*		m_opMetronomeOtherBeatKeyLabel;
	EMComboBox*		m_opMetronomeOtherBeatVelocityComboBox;
	EMLabel*		m_opMetronomeOtherBeatVelocityLabel;
	EMView*			m_opMetronomeOtherBeatView;
	EMRadioButton*	m_opMetronomeUseDuringBothRadioButton;
	EMGroupBox*		m_opMetronomeUseDuringGroupBox;
	EMRadioButton*	m_opMetronomeUseDuringPlaybackRadioButton;
	EMRadioButton*	m_opMetronomeUseDuringRecordingRadioButton;
	EMView*			m_opMetronomeUseDuringView;
	EMCheckBox*		m_opMetronomeUseMIDINoteCheckBox;
	EMCheckBox*		m_opMetronomeUsePCSpeakerCheckBox;
	EMView*			m_opMetronomeView;
	// Metronome Tab - Variables
	map<string, int32>	m_oMidiDeviceMap;

};

#endif