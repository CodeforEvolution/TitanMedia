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

#ifndef __LOGIN_DIALOG
#define __LOGIN_DIALOG

#include "EMListener.h"
#include "EMListenerRepository.h"

class EMButton;
class EMCheckBox;
class EMFactory;
class EMLabel;
class EMTextControl;
class EMView;
class EMWindow;


class LoginDialog : public EMListener, public EMListenerRepository
{
public:
	LoginDialog();
	~LoginDialog();
	uint64 ConvertToID_Uint64(const char* p_vpUserID);
	void ConvertToID_String(uint64 p_vUserID, char* p_vpUserID);
	void DisplayDialog();
	void HideDialog();
	void Init();
	bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage);
	void MoveDialog();
	void MoveDialog(float p_vX, float p_vY);
	void SetParent(EMWindow* p_opWindow);

private:
	EMButton*		m_opCancelButton;
	EMWindow*		m_opDialogWindow;
	EMView*			m_opMainView;
	EMButton*		m_opOK_Button;
	EMWindow*		m_opParentWindow;
	EMLabel*		m_opPasswordLabel;
	EMTextControl*	m_opPasswordTextControl;
	EMCheckBox*		m_opSavePasswordCheckBox;
	EMLabel*		m_opUserIDLabel;
	EMTextControl*	m_opUserIDTextControl;
	uint32			m_vMessage;
};


#endif