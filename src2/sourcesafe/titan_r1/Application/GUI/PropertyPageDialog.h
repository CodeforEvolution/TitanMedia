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

#ifndef __PROPERTY_PAGE_DIALOG
#define __PROPERTY_PAGE_DIALOG

#include "EMListener.h"
class EMFactory;

class EMWindow;
class EMView;
class EMButton;

class EMListenerRepository;

class PropertyPageDialog : public EMListener
{
public:
	PropertyPageDialog();
	~PropertyPageDialog();
	void DisplayDialog();
	void HideDialog();
	void Init();
	bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage);
	void MoveDialog();
	void MoveDialog(float p_vX, float p_vY);
	void SetParent(EMWindow* p_opWindow);

private:
	EMButton* m_opCloseButton;
	EMWindow* m_opDialogWindow;
	EMView* m_opMainView;
	EMWindow* m_opParentWindow;
};


#endif