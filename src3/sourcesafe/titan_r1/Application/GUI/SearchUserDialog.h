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

#ifndef __SEARCH_USER_DIALOG
#define __SEARCH_USER_DIALOG

#include "EMListener.h"

class EMButton;
class EMFactory;
class EMLabel;
class EMListenerRepository;
class EMListViewControl;
class EMTextControl;
class EMView;
class EMWindow;
class SystemSettingsDialog;

class SearchUserDialog : public EMListener
{
public:
	SearchUserDialog();
	~SearchUserDialog();
	uint64 ConvertToID_Uint64(const char* p_vpUserID);
	void ConvertToID_String(uint64 p_vUserID, char* p_vpUserID);
	void DisplayDialog();
	void HideDialog();
	void Init();
	bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage);
	void MoveDialog();
	void MoveDialog(float p_vX, float p_vY);
	void SetMainWindow(EMWindow* p_opWindow);
	void SetParent(EMWindow* p_opWindow);
	void SetSystemSettingsDialog(void* p_opSystemSettingsDialog);

private:
	// Components
	EMButton*		   m_opChooseButton;
	EMButton*		   m_opCloseButton;
	EMWindow*		   m_opDialogWindow;
	EMView*			   m_opMainView;
	EMLabel*		   m_opNicknameLabel;
	EMTextControl*	   m_opNicknameTextControl;
	EMWindow*		   m_opParentWindow;
	EMLabel*		   m_opResultLabel;
	EMListViewControl* m_opResultListViewControl;
	EMButton*		   m_opSearchButton;
//	EMLabel*		   m_opUserIDLabel;
//	EMTextControl*	   m_opUserIDTextControl;

	// Variables
	EMWindow*			  m_opMainAppWindow;
	SystemSettingsDialog* m_opSystemSettingsDialog;
	uint32				  m_vNumberOfUsersFound;
};

#endif