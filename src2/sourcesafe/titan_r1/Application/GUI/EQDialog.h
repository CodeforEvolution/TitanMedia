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

#ifndef __EQ_DIALOG
#define __EQ_DIALOG

#include "EMListener.h"

class EMButton;
class EMEqualizerInterface;
class EMFactory;
class EMLabel;
class EMTextControl;
class EMTrackbarControl;
class EMView;
class EMWindow;

class EMListenerRepository;

class EQDialog : public EMListener
{
public:
	EQDialog();
	~EQDialog();
	void DisplayDialog();
	void HideDialog();
	void Init();
	bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage);
	void MoveDialog();
	void MoveDialog(float p_vX, float p_vY);
	void SetEqualizer(EMEqualizerInterface* p_opEQ);
	void SetParent(EMWindow* p_opWindow);
	char* SetValidValue(char* p_vpValue);

private:
	// Components
	EMLabel*			m_opBassLabel;
	EMTextControl*		m_opBassTextControl;
	EMTrackbarControl*	m_opBassTrackbarControl;
	EMButton*			m_opCancelButton;
	EMWindow*			m_opDialogWindow;
	EMLabel*			m_opMiddleLabel;
	EMTextControl*		m_opMiddleTextControl;
	EMTrackbarControl*	m_opMiddleTrackbarControl;
	EMLabel*			m_opTrebleLabel;
	EMTextControl*		m_opTrebleTextControl;
	EMTrackbarControl*	m_opTrebleTrackbarControl;
	EMView*				m_opMainView;
	EMButton*			m_opOK_Button;
	EMWindow*			m_opParentWindow;
	
	// Variables
	bool				  m_vAlreadyChanged;
	EMEqualizerInterface* m_opEQ;
	char				  m_vTemp[5];
	int8 m_vBass;
	int8 m_vMiddle;
	int8 m_vTreble;


};


#endif