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

#ifndef __EXPORT_MEDIA_SETTINGS_DIALOG
#define __EXPORT_MEDIA_SETTINGS_DIALOG

#include "EMListener.h"
#include <map>
typedef pair<int, int> ResolutionPair;
class EMFactory;

class EMWindow;
class EMView;
class EMGroupBox;
class EMButton;
class EMLabel;
class EMListenerRepository;
class EMCheckBox;
class EMComboBox;
class EMTextControl;
class EMRadioButton;
class ExportMediaDialog;

class ExportMediaSettingsDialog : public EMListener
{
public:
	ExportMediaSettingsDialog();
	~ExportMediaSettingsDialog();
	void DisplayDialog();
	void EnableAudioVideo(bool p_vEnableAudio, bool p_vEnableVideo);
	void HideDialog();
	void Init();
	bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage);
	void MoveDialog();
	void MoveDialog(float p_vX, float p_vY);
	void SetMainWindow(EMWindow* p_opWindow);
	void SetParent(EMWindow* p_opWindow);
	void SetExportMediaDialog(void* p_opExportMediaDialog);

public:
	static char m_vFileExtension[5];

private:
	// Controls pointers
	EMComboBox*	   m_opAudioEncoderComboBox;
	EMLabel*	   m_opAudioEncoderLabel;
	EMComboBox*	   m_opAudioFormatComboBox;
	EMLabel*	   m_opAudioFormatLabel;
	EMGroupBox*	   m_opAudioGroupBox;
	EMButton*	   m_opAudioSettingsButton;
	EMView*        m_opAudioView;
//	EMComboBox*	   m_opBitDepthComboBox;
//	EMLabel*	   m_opBitDepthLabel;
	EMButton*	   m_opCancelButton;
//	EMComboBox*	   m_opChannelsComboBox;
//	EMLabel*	   m_opChannelsLabel;
	EMWindow*	   m_opDialogWindow;
	EMComboBox*	   m_opDimensionComboBox;
	EMLabel*	   m_opDimensionLabel;
	EMRadioButton* m_opDimensionPreDefineRadioButton;
	EMRadioButton* m_opDimensionUserDefineRadioButton;
	EMComboBox*	   m_opFrameRateComboBox;
	EMLabel*	   m_opFrameRateLabel;
	EMRadioButton* m_opFrameRatePreDefineRadioButton;
	EMRadioButton* m_opFrameRateUserDefineRadioButton;
	EMTextControl* m_opFrameRateTextControl;
	EMLabel*	   m_opHeightLabel;
	EMTextControl* m_opHeightTextControl;
	EMWindow*	   m_opMainAppWindow;
	EMView*		   m_opMainView;
	EMButton*	   m_opOK_Button;
	EMWindow*	   m_opParentWindow;
	EMLabel*	   m_opProcentageLabel;
	EMLabel*	   m_opQualityLabel;
	EMTextControl* m_opQualityTextControl;
	EMCheckBox*	   m_opVideoAspectRatiosCheckBox;
	EMComboBox*	   m_opVideoAspectRatiosComboBox;
	EMComboBox*	   m_opVideoEncoderComboBox;
	EMLabel*	   m_opVideoEncoderLabel;
	EMComboBox*	   m_opVideoFamilyComboBox;
	EMLabel*	   m_opVideoFamilyLabel;
	EMButton*	   m_opVideoSettingsButton;
	EMGroupBox*	   m_opVideoGroupBox;
	EMView*		   m_opVideoView;
	EMLabel*	   m_opWidthLabel;
	EMTextControl* m_opWidthTextControl;

	// Other variables
	map<string, uint32> m_oAudioEncoderStringIDMap;
	map<string, uint32> m_oAudioFormatStringIDMap;
	map<string, uint32> m_oVideoEncoderStringIDMap;
	map<int, ResolutionPair> m_oResolutionMap;
	map<int, float> m_oFrameRateMap;
	map<int, ResolutionPair> m_oAspectRatiosMap;
	int m_vAudioEncoderOld;
	int m_vAudioFormatOld;
	bool m_vDimensionDefineOld;
	int m_vDimensionPreDefineOld;
	string m_oDimensionUserDefineWidthOld;
	string m_oDimensionUserDefineHeightOld;
	bool m_vVideoAspectRatiosToggleOld;
	int m_vVideoAspectRatiosOld;
	bool m_vVideoFrameRateDefineOld;
	int m_vVideoFrameRatePreDefineOld;
	string m_oVideoFrameRateUserDefineOld;
	int m_vVideoFamilyOld;
	int m_vVideoEncoderOld;
	string m_oQualityOld;
	bool m_vAspectRatiosIsChecked;
	bool m_vWidthChanged;
	bool m_vHeightChanged;
	bool m_vFrameRateChanged;
	int m_vDecimalPosition;
//	bool m_vDecimalExist;
	int m_vFrameRateLength;
//	bool m_vNoNeedToCheck;
	ExportMediaDialog* m_opExportMediaDialog;
};

#endif