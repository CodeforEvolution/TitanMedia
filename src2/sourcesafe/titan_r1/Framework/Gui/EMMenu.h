/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_MENU
#define __EM_MENU

#include "EMGUIComponent.h"

#include <string>

class EMMenuItem;

class EMMenu : public EMGUIComponent
{
public:
	virtual ~EMMenu();
	virtual bool AddItem(EMMenu* p_opMenu) = 0;
	virtual bool AddItem(EMMenuItem* p_opMenuItem) = 0;
	virtual bool AddItem(EMMenu* p_opMenu, int32 p_vIndex) = 0;
	virtual bool AddItem(EMMenuItem* p_opMenuItem, int32 p_vIndex) = 0;
	virtual bool AddSeparatorItem() = 0;
	virtual bool GetEnabled() = 0;
	virtual EMMenu* GetMenu(string* p_opName) = 0;
	virtual EMMenuItem* GetMenuItem(string* p_opName) = 0;
	virtual EMMenuItem* GetMenuItem(uint32 p_vCommand) = 0;
	virtual const char* GetName() = 0;
	virtual bool RemoveItem(EMMenu* p_opMenu) = 0;
	virtual bool RemoveItem(EMMenuItem* p_opMenuItem) = 0;
	virtual void SetEnabled(bool p_vIsEnabled) = 0;
	virtual void SetMessageTarget(void* p_opMessageTarget) = 0;

protected:
	EMMenu();
};

#endif

