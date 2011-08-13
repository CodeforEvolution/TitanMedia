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

#ifndef __REGISTRATION_DIALOG
#define __REGISTRATION_DIALOG

#include "EMListener.h"

#include <string>

class EMButton;
class EMFactory;
class EMLabel;
class EMListenerRepository;
class EMTextControl;
class EMView;
class EMWindow;
class TrackWindow;

class RegistrationDialog : public EMListener
{
public:
	RegistrationDialog();
	~RegistrationDialog();
	void CreateRegKeyFile();
	void DisplayDialog();
	void HideDialog();
	void Init(char* p_vpProjectFile, uint32 p_vStartupFlags, TrackWindow* p_opTrackWindow);
	bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage);
	void MoveDialog();
	void MoveDialog(float p_vX, float p_vY);
	void SetParent(EMWindow* p_opWindow);

private:
	void ContinueStartup();

private:
	// Components
	EMTextControl*		m_opBassTextControl;
	EMButton*			m_opCancelButton;
	EMLabel*			m_opCaseLabel;
	EMWindow*			m_opDialogWindow;
	EMLabel*			m_opEnterLabel;
	EMView*				m_opMainView;
	EMButton*			m_opOK_Button;
	EMWindow*			m_opParentWindow;
	EMTextControl*		m_opRegistrationTextControl;
	string m_oProjectFile;
	TrackWindow* m_opTrackWindow;
	uint32 m_vStartupFlags;
};

#endif