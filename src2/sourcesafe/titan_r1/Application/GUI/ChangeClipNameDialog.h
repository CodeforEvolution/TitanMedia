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

#ifndef __CHANGE_CLIP_NAME_DIALOG
#define __CHANGE_CLIP_NAME_DIALOG

#include "EMListener.h"
class EMFactory;

class EMWindow;
class EMView;
class EMButton;
class EMTextControl;
class EMGClip;

class EMListenerRepository;

class ChangeClipNameDialog : public EMListener
{
public:
	ChangeClipNameDialog();
	~ChangeClipNameDialog();
	void DisplayDialog(EMGClip* p_opClip, EMView* p_opClipView);
	string GetText();
	void HideDialog();
	void Init();
	bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage);
	void MoveDialog(float p_vX, float p_vY);
	void SetText(string p_oText);

private:
	EMWindow* m_opDialogWindow;
	EMView* m_opMainView;
	EMTextControl* m_opNameTextControl;
	EMButton* m_opOK_Button;
	EMButton* m_opCancelButton;

	bool m_vFinished;
	EMView* m_opClipView;
	EMGClip* m_opClip;
	bool m_vEditText;

};


#endif