/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_POP_UP_MENU
#define __EM_POP_UP_MENU

#include "EMGUIComponent.h"

#include <string>

class EMMenu;
class EMMenuItem;
class EMView;

class EMPopUpMenu : public EMGUIComponent
{
public:
	virtual ~EMPopUpMenu();
	virtual bool AddItem(EMMenu* p_opMenu) = 0;
	virtual bool AddItem(EMMenuItem* p_opMenuItem) = 0;
	virtual bool AddSeparatorItem() = 0;
	virtual void PopUp(int32 p_vX, int32 p_vY, EMView* p_opView) = 0;

protected:
	EMPopUpMenu();
};

#endif

