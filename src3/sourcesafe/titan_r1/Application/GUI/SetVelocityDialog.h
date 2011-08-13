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

#ifndef __SET_VELOCITY_DIALOG
#define __SET_VELOCITY_DIALOG

#include "EMListener.h"
class EMFactory;

class EMWindow;
class EMView;
class EMButton;
class EMLabel;
class EMTextControl;
class EMCheckBox;

class EMListenerRepository;

class SetVelocityDialog : public EMListener
{
public:
	SetVelocityDialog();
	~SetVelocityDialog();
	void DisplayDialog();
	void HideDialog();
	void Init();
	bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage);
	void MoveDialog(float p_vX, float p_vY);

private:
	EMButton* m_opCancelButton;
	EMWindow* m_opDialogWindow;
	EMLabel* m_opLabel;
	EMView* m_opMainView;
	EMButton* m_opOK_Button;
	EMCheckBox* m_opProcentageCheckBox;
	EMLabel* m_opProcentageLabel;
	EMTextControl* m_opTextControl;
};

#endif