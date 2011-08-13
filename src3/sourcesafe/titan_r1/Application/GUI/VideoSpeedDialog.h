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

#ifndef __VIDEO_SPEED_DIALOG
#define __VIDEO_SPEED_DIALOG

#include "EMListener.h"
#include <list>

class EMButton;
class EMFactory;
class EMGClip;
class EMLabel;
class EMTextControl;
class EMView;
class EMWindow;

class EMListenerRepository;

class VideoSpeedDialog : public EMListener
{
public:
	VideoSpeedDialog();
	~VideoSpeedDialog();
	void DisplayDialog();
	void HideDialog();
	void Init();
	bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage);
	void MoveDialog();
	void MoveDialog(float p_vX, float p_vY);
	void SetParent(EMWindow* p_opWindow);

private:
	// Components
	EMWindow* m_opDialogWindow;
	EMView* m_opMainView;
	EMLabel* m_opStartLabel;
	EMLabel* m_opStopLabel;
	EMLabel* m_opDurationLabel;
	EMLabel* m_opSpeedLabel;
	EMLabel* m_opPercentCharacterLabel;
	EMTextControl* m_opStartTextControl;
	EMTextControl* m_opStopTextControl;
	EMTextControl* m_opDurationTextControl;
	EMTextControl* m_opSpeedTextControl;
	EMButton* m_opOK_Button;
	EMButton* m_opCancelButton;
	EMWindow* m_opParentWindow;

	// Variables
	list<EMGClip*>* m_opClips;
	float m_vMagicSpeed;
};



#endif