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

#ifndef __WIPE_TRANSITION_PROPERTIES_DIALOG
#define __WIPE_TRANSITION_PROPERTIES_DIALOG

#include "EMListener.h"
class EMFactory;

class EMButton;
class EMCheckBox;
class EMGroupBox;
class EMLabel;
class EMTextControl;
class EMView;
class EMWindow;

class EMListenerRepository;

class WipeTransitionPropertiesDialog : public EMListener
{
public:
	WipeTransitionPropertiesDialog();
	~WipeTransitionPropertiesDialog();
	void DisplayDialog();
	void HideDialog();
	void Init();
	bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage);
	void MoveDialog();
	void MoveDialog(float p_vX, float p_vY);
	void SetMainWindow(EMWindow* p_opWindow);
	void SetParent(EMWindow* p_opWindow);
	void SetTransition(int32 p_vTransitionID, char* p_vTransitionName);

private:
	string			m_oDialogTitle;
	EMLabel*		m_opBLabel;
	EMGroupBox*		m_opBorderGroupBox;
	EMView*			m_opBorderView;
	EMTextControl*	m_opBTextControl;
	EMButton*		m_opCancelButton;
	EMLabel*		m_opColorLabel;
	EMButton*		m_opDefaultButton;
	EMWindow*		m_opDialogWindow;
	EMLabel*		m_opGLabel;
	EMTextControl*	m_opGTextControl;
	EMWindow*		m_opMainAppWindow;
	EMView*			m_opMainView;
	EMGroupBox*		m_opOffsetGroupBox;
	EMLabel*		m_opOffXLabel;
	EMTextControl*	m_opOffXTextControl;
	EMLabel*		m_opOffYLabel;
	EMTextControl*	m_opOffYTextControl;
	EMView*			m_opOffsetView;
	EMButton*		m_opOK_Button;
	EMWindow*		m_opParentWindow;
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
	int32*			m_vpTransitionValues;
	int32			m_vTransitionID;
};


#endif