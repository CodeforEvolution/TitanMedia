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

#ifndef __TRANSITION_DIALOG
#define __TRANSITION_DIALOG

#include "EMListener.h"
#include "EMMediaTransitionDialogIDs.h"
#include <map>

class EMButton;
class EMCheckBox;
class EMComboBox;
class EMGClip;
class EMGroupBox;
class EMLabel;
class EMListenerRepository;
class EMRadioButton;
class EMTextControl;
class EMTrackbarControl;
class EMView;
class EMWindow;

class TransitionDialog : public EMListener
{
public:
	TransitionDialog();
	~TransitionDialog();
	void DisplayDialog();
	void HideDialog();
	void Init();
	bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage);
	void MoveDialog();
	void MoveDialog(float p_vX, float p_vY);
	void SetParent(EMWindow* p_opWindow);
	void SetTrackView(EMView* m_opView);
	void SetTransitionClip(EMGClip* p_opClip);

private:
	void AddList(list<string*>* p_opTransitionList);
	void EnableColorKeyColorSettings(bool p_vEnable);
	void EnableColorKeyHueSettings(bool p_vEnable);
	void EnableColorKeyLuminanceSettings(bool p_vEnable);
	void EnableColorKeyToleranceSettings(bool p_vEnable);
	void EnableColorKeySettings(bool p_vEnable);
	void EnableSettings(bool p_vEnble);
	void HueToRGB(float* p_vpHue, unsigned char* p_vpRGB);
	char* SetValidValue(char* p_vpValue, int32 p_vMin, int32 p_vMax);

private:
	// Main - Controls
	EMButton*	m_opCancelButton;
	EMWindow*	m_opDialogWindow;
	EMComboBox*	m_opComboBox;
	EMView*		m_opMainView;
	EMButton*	m_opOK_Button;
	EMWindow*	m_opParentWindow;
	EMGroupBox* m_opSettingsGroupBox;
	EMView*		m_opTrackView;

	// Wipe Transition Settings - Controls
	EMView*			m_opSettingsView;
	EMButton*		m_opDefaultButton;
	EMLabel*		m_opBLabel;
	EMGroupBox*		m_opBorderGroupBox;
	EMView*			m_opBorderView;
	EMTextControl*	m_opBTextControl;
	EMLabel*		m_opColorLabel;
	EMLabel*		m_opGLabel;
	EMTextControl*	m_opGTextControl;
	EMGroupBox*		m_opOffsetGroupBox;
	EMLabel*		m_opOffXLabel;
	EMTextControl*	m_opOffXTextControl;
	EMLabel*		m_opOffYLabel;
	EMTextControl*	m_opOffYTextControl;
	EMView*			m_opOffsetView;
	EMGroupBox*		m_opReplicateGroupBox;
	EMLabel*		m_opRepXLabel;
	EMTextControl*	m_opRepXTextControl;
	EMLabel*		m_opRepYLabel;
	EMTextControl*	m_opRepYTextControl;
	EMView*			m_opReplicateView;
	EMLabel*		m_opRLabel;
	EMTextControl*	m_opRTextControl;
	EMLabel*		m_opSoftnessLabel;
	EMTextControl*	m_opSoftnessTextControl;
	EMCheckBox*		m_opSwapCheckBox;
	EMLabel*		m_opWidthLabel;
	EMTextControl*	m_opWidthTextControl;

	// EM_ColorKey Transition Settings - Controls
	EMView*				m_opColorKeySettingsView;
	EMGroupBox*			m_opColorKeyColorGroupBox;
	EMView*				m_opColorKeyColorView;
	EMRadioButton*		m_opColorKeyUseColorRadioButton;
	EMRadioButton*		m_opColorKeyUseHueRadioButton;
	EMRadioButton*		m_opColorKeyUseLuminanceRadioButton;
	EMRadioButton*		m_opColorKeyUseBlueAndGreenScreenRadioButton;
	EMLabel*			m_opColorKeyColorBlueLabel;
	EMTextControl*		m_opColorKeyColorBlueTextControl;
	EMTrackbarControl*	m_opColorKeyColorBlueTrackbarControl;
	EMLabel*			m_opColorKeyColorGreenLabel;
	EMTextControl*		m_opColorKeyColorGreenTextControl;
	EMTrackbarControl*	m_opColorKeyColorGreenTrackbarControl;
	EMLabel*			m_opColorKeyColorRedLabel;
	EMTextControl*		m_opColorKeyColorRedTextControl;
	EMTrackbarControl*	m_opColorKeyColorRedTrackbarControl;
	EMView*				m_opColorKeyColorPreviewView;
	EMGroupBox*			m_opColorKeyHueGroupBox;
	EMView*				m_opColorKeyHueView;
	EMLabel*			m_opColorKeyHueLabel;
	EMTextControl*		m_opColorKeyHueTextControl;
	EMTrackbarControl*	m_opColorKeyHueTrackbarControl;
	EMGroupBox*			m_opColorKeyLuminanceGroupBox;
	EMView*				m_opColorKeyLuminanceView;
	EMLabel*			m_opColorKeyLuminanceLabel;
	EMTextControl*		m_opColorKeyLuminanceTextControl;
	EMTrackbarControl*	m_opColorKeyLuminanceTrackbarControl;
	EMLabel*			m_opColorKeyToleranceLabel;
	EMTextControl*		m_opColorKeyToleranceTextControl;
	EMTrackbarControl*	m_opColorKeyToleranceTrackbarControl;
	EMCheckBox*			m_opColorKeyInvertCheckBox;
	EMCheckBox*			m_opColorKeySwapCheckBox;
	EMButton*			m_opColorKeyDefaultButton;
	EMView*				m_opColorKeyHuePreviewView;

	// EM_ColorKey Transition Settings - Variable
	bool m_vAlreadyChanged;
	char m_vTemp[10];

	// Main - Variables
	EMGClip*	m_opTransitionClip;
	map<string, int32> m_oTransitionStringIDMap;
	map<int32, string> m_oTransitionIDStringMap;
	int32			m_vpTransitionValues[10];
	int32			m_vTransitionID;
	EMDialogParameters m_oTransitionParameters;
};


#endif