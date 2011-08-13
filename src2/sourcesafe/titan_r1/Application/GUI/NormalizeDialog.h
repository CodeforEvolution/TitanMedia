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

#ifndef __NORMALIZE_DIALOG
#define __NORMALIZE_DIALOG

#include "EMListener.h"

class CommandAudioNormalize;
class EMButton;
class EMFactory;
class EMLabel;
class EMListenerRepository;
class EMTextControl;
class EMView;
class EMWindow;


class NormalizeDialog : public EMListener
{
public:
	NormalizeDialog();
	~NormalizeDialog();
	void DisplayDialog(void* p_opCommandAudioNormalize);
	void HideDialog();
	void Init();
	bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage);
	void MoveDialog();
	void MoveDialog(float p_vX, float p_vY);
	void SetParent(EMWindow* p_opWindow);
	char* SetValidValue(char* p_vpValue);

private:
	EMButton* m_opCancelButton;
	EMWindow* m_opDialogWindow;
	EMLabel* m_opLabel;
	EMView* m_opMainView;
	EMButton* m_opOK_Button;
	EMTextControl* m_opTextControl;
	EMWindow* m_opParentWindow;
	CommandAudioNormalize* m_opCommandAudioNormalize;
	bool m_vAlreadyChanged;
	char m_vTemp[4];
};

#endif