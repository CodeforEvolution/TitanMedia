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

#ifndef __EXPORT_AUDIO_SETTINGS_DIALOG
#define __EXPORT_AUDIO_SETTINGSDIALOG

#include "EMListener.h"
class EMFactory;

class EMWindow;
class EMView;
class EMButton;
class EMTextControl;
class EMLabel;
class EMComboBox;

class EMListenerRepository;

class ExportAudioSettingsDialog : public EMListener
{
public:
	ExportAudioSettingsDialog();
	~ExportAudioSettingsDialog();
	void DisplayDialog();
	void HideDialog();
	void Init();
	bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage);
	void MoveDialog();
	void MoveDialog(float p_vX, float p_vY);
	void SetMainWindow(EMWindow* p_opWindow);
	void SetParent(EMWindow* p_opWindow);

private:
	EMWindow* m_opDialogWindow;
	EMView* m_opMainView;
	EMButton* m_opOK_Button;
	EMButton* m_opCancelButton;
	EMLabel* m_opEncoderLabel;
	EMLabel* m_opFamilyLabel;
	EMLabel* m_opFrequencyLabel;
	EMLabel* m_opBitRateLabel;
	EMLabel* m_opChannelsLabel;
	EMComboBox* m_opEncoderComboBox;
	EMComboBox* m_opFamilyComboBox;
	EMTextControl* m_opFrequencyTextControl;
	EMComboBox* m_opBitRateComboBox;
	EMComboBox* m_opChannelsComboBox;

	EMWindow* m_opParentWindow;
	EMWindow* m_opMainAppWindow;
};

#endif