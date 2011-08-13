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

#ifndef __HSV_VALUE_DIALOG
#define __HSV_VALUE_DIALOG

#include "EMListener.h"

class EMButton;
class EMFactory;
class EMHSVFilter;
class EMLabel;
class EMTextControl;
class EMTrackbarControl;
class EMView;
class EMWindow;

class EMListenerRepository;

class HSV_ValueDialog : public EMListener
{
public:
	HSV_ValueDialog();
	~HSV_ValueDialog();
	void DisplayDialog(EMHSVFilter* p_opHSVFilter);
	void HideDialog();
	void Init();
	bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage);
	void MoveDialog();
	void MoveDialog(float p_vX, float p_vY);
	void SetParent(EMWindow* p_opWindow);
	char* SetValidValue(char* p_vpValue);

private:
	// Components
	EMButton*			m_opCancelButton;
	EMWindow*			m_opDialogWindow;
	EMLabel*			m_opHueLabel;
	EMLabel*			m_opHueProcentLabel;
	EMTextControl*		m_opHueTextControl;
	EMTrackbarControl*	m_opHueTrackbarControl;
	EMView*				m_opMainView;
	EMButton*			m_opOK_Button;
	EMWindow*			m_opParentWindow;
	EMLabel*			m_opSaturationLabel;
	EMLabel*			m_opSaturationProcentLabel;
	EMTextControl*		m_opSaturationTextControl;
	EMTrackbarControl*	m_opSaturationTrackbarControl;
	EMLabel*			m_opValueLabel;
	EMLabel*			m_opValueProcentLabel;
	EMTextControl*		m_opValueTextControl;
	EMTrackbarControl*	m_opValueTrackbarControl;

	// Variables
	EMHSVFilter*	m_opHSVFilter;
	bool			m_vAlreadyChanged;
	float			m_vHue;
	float			m_vSaturation;
	char			m_vTemp[5];
	float			m_vValue;

};


#endif