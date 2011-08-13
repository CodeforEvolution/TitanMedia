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

#ifndef __QUANTIZE_DIALOG
#define __QUANTIZE_DIALOG

#include "EMListener.h"
class EMFactory;

class EMWindow;
class EMView;
class EMButton;
class EMLabel;
class EMTextControl;
class EMCheckBox;

class EMListenerRepository;

class QuantizeDialog : public EMListener
{
public:
	QuantizeDialog();
	~QuantizeDialog();
	void DisplayDialog();
	void HideDialog();
	void Init();
	bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage);
	void MoveDialog(float p_vX, float p_vY);

private:
	EMButton*		m_opCancelButton;
	EMWindow*		m_opDialogWindow;
	EMView*			m_opMainView;
	EMButton*		m_opOK_Button;
	EMCheckBox*		m_opQuantizeNoteBeginCheckBox;
	EMCheckBox*		m_opQuantizeNoteEndCheckBox;
	EMLabel*		m_opRandomizeLabel;
	EMTextControl*	m_opRandomizeTextControl;
	EMLabel*		m_opStrengthLabel;
	EMTextControl*	m_opStrengthTextControl;
	EMLabel*		m_opSwingLabel;
	EMTextControl*	m_opSwingTextControl;
};

#endif