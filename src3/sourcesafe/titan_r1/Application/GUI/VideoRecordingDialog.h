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

#ifndef __VIDEO_RECORDING_DIALOG
#define __VIDEO_RECORDING_DIALOG

#include "EMListener.h"
class EMFactory;

class EMWindow;
class EMView;
class EMTabControl;
class EMButton;
class EMLabel;
class EMTextControl;

class EMListenerRepository;

class VideoRecordingDialog : public EMListener
{
public:
	VideoRecordingDialog();
	~VideoRecordingDialog();
	void DisplayDialog();
	void HideDialog();
	void Init();
	bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage);
	void MoveDialog(float p_vX, float p_vY);

private:
	EMWindow* m_opDialogWindow;
	EMView* m_opMainView;
	EMTabControl* m_opTabControl;
	
	EMView* m_opMarkTabView;
	EMLabel* m_opInLabel;
	EMLabel* m_opOutLabel;
	EMLabel* m_opDurationLabel;
	EMTextControl* m_opInTextControl;
	EMTextControl* m_opOutTextControl;
	EMTextControl* m_opDurationTextControl;

	EMView* m_opFileTabView;
	EMLabel* m_opFilenameLabel;
	EMLabel* m_opCopyRightLabel;
	EMTextControl* m_opFilenameTextControl;
	EMTextControl* m_opCopyRightTextControl;
};


#endif