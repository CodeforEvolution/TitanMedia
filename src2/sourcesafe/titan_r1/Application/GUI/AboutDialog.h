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

#ifndef __ABOUT_DIALOG
#define __ABOUT_DIALOG

#include "EMListener.h"

class EMBitmap;
class EMButton;
class EMFactory;
class EMTextControl;
class EMView;
class EMWindow;

class EMListenerRepository;

class AboutDialog : public EMListener
{
public:
	AboutDialog();
	~AboutDialog();
	void DisplayDialog();
	void HideDialog();
	void Init();
	bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage);
	void MoveDialog();
	void MoveDialog(float p_vX, float p_vY);
	void SetParent(EMWindow* p_opWindow);

private:
	EMBitmap* m_opSplashBitmap;
	EMButton* m_opCloseButton;
	EMWindow* m_opDialogWindow;
	EMButton* m_opHomepageButton;
	EMView* m_opMainView;
	EMWindow* m_opParentWindow;
	EMButton* m_opPurchaseButton;

	bool vDraw;
};


#endif