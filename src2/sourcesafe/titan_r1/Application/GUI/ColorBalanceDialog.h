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

#ifndef __COLOR_BALANCE_DIALOG
#define __COLOR_BALANCE_DIALOG

#include "EMListener.h"

class EMButton;
class EMFactory;
class EMLabel;
class EMRGBFilter;
class EMTextControl;
class EMTrackbarControl;
class EMView;
class EMWindow;

class EMListenerRepository;

class ColorBalanceDialog : public EMListener
{
public:
	ColorBalanceDialog();
	~ColorBalanceDialog();
	void DisplayDialog(EMRGBFilter* p_opRGBFilter);
	void HideDialog();
	void Init();
	bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage);
	void MoveDialog();
	void MoveDialog(float p_vX, float p_vY);
	void SetParent(EMWindow* p_opWindow);
	char* SetValidValue(char* p_vpValue);

private:
	// Components
	EMLabel*			m_opBlueLabel;
	EMTextControl*		m_opBlueTextControl;
	EMTrackbarControl*	m_opBlueTrackbarControl;
	EMButton*			m_opCancelButton;
	EMWindow*			m_opDialogWindow;
	EMLabel*			m_opGreenLabel;
	EMTextControl*		m_opGreenTextControl;
	EMTrackbarControl*	m_opGreenTrackbarControl;
	EMView*				m_opMainView;
	EMButton*			m_opOK_Button;
	EMWindow*			m_opParentWindow;
	EMLabel*			m_opRedLabel;
	EMTextControl*		m_opRedTextControl;
	EMTrackbarControl*	m_opRedTrackbarControl;
	
	// Variables
	EMRGBFilter*	m_opRGBFilter;
	bool			m_vAlreadyChanged;
	float			m_vBlue;
	float			m_vGreen;
	float			m_vRed;
	char			m_vTemp[5];

};


#endif