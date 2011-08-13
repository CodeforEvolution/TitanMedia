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

#ifndef __SYSTEM_SETTINGS_DIALOG
#define __SYSTEM_SETTINGS_DIALOG

#include "EMListener.h"

class EMButton;
class EMCheckBox;
class EMComboBox;
class EMFactory;
class EMGroupBox;
class EMLabel;
class EMRadioButton;
class EMTabControl;
class EMTextControl;
class EMView;
class EMWindow;

class EMListenerRepository;

class SystemSettingsDialog : public EMListener
{
public:
	SystemSettingsDialog();
	~SystemSettingsDialog();
	uint64 ConvertToID_Uint64(const char* p_vpUserID);
	void ConvertToID_String(uint64 p_vUserID, char* p_vpUserID);
	void DisplayDialog();
//	void EnableMidiNote(bool p_vEnable);
	void HideDialog();
	void Init();
	bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage);
	void MoveDialog();
	void MoveDialog(float p_vX, float p_vY);
	void SaveSettings();
//	void SetMidiKeyInComboBox(EMComboBox* p_opComboBox);
//	void SetNumberInComboBox(EMComboBox* p_opComboBox, int16 p_vNumberOfNumbers, int16 p_vStartValue = 0);
	void SetParent(EMWindow* p_opWindow);
	char* SetValidValue(char* p_vpValue, int32 p_vMin, int32 p_vMax);

	// Network Tab
	void SetUserIDString(char* p_vpUserID);

private:
	// Main view - Components
	EMButton*		m_opCancelButton;
	EMWindow*		m_opDialogWindow;
	EMView*			m_opMainView;
	EMButton*		m_opOK_Button;
	EMWindow*		m_opParentWindow;
	EMTabControl*	m_opTabControl;

	// Network Tab - Components
	EMGroupBox*		m_opHostGroupBox;
	EMLabel*		m_opHostLabel;
	EMTextControl*	m_opHostTextControl;
	EMView*			m_opHostView;
	EMRadioButton*	m_opInternetRadioButton;
	EMRadioButton*	m_opIntranetRadioButton;
	EMView*			m_opNetworkView;
	EMLabel*		m_opPasswordLabel;
	EMTextControl*	m_opPasswordTextControl;
	EMRadioButton*	m_opProtTCP_RadioButton;
	EMLabel*		m_opProtTCP_PortLabel;
	EMTextControl*	m_opProtTCP_PortTextControl;
	EMRadioButton*	m_opProtUDP_RadioButton;
	EMLabel*		m_opProtUDP_PortLabel;
	EMTextControl*	m_opProtUDP_PortTextControl;
	EMCheckBox*		m_opSavePasswordCheckBox;
	EMButton*		m_opSelectUserButton;
	EMGroupBox*		m_opUserGroupBox;
	EMLabel*		m_opUserIDLabel;
	EMTextControl*	m_opUserIDTextControl;
	EMView*			m_opUserView;
	// Network - Variables
	int m_vDecimalPosition;
	int m_vFrameRateLength;
	bool m_vSelectionStartBeforeDecimal;

	// Audio Tab - Components
	EMLabel*		m_opAudioBufferSizeLabel;
	EMLabel*		m_opAudioBufferSizeSuffixLabel;
	EMTextControl*	m_opAudioBufferSizeTextControl;
	EMLabel*		m_opAudioReadAheadLabel;
	EMLabel*		m_opAudioReadAheadSuffixLabel;
	EMTextControl*	m_opAudioReadAheadTextControl;
	EMView*			m_opAudioView;
	// Audio Tab - Variables
	bool			m_vAlreadyChanged;
	char			m_vTemp[10];

	// GIU Tab - Components
	EMLabel*		m_opGUISnapThresholdLabel;
	EMLabel*		m_opGUISnapThresholdSuffixLabel;
	EMTextControl*	m_opGUISnapThresholdTextControl;
	EMView*			m_opGUIView;

/*	// Metronome Tab - Components
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
	EMCheckBox*		m_opMetronomeUseMIDINoteCheckBox;
	EMCheckBox*		m_opMetronomeUsePCSpeakerCheckBox;
	EMView*			m_opMetronomeView;
	*/
};

#endif