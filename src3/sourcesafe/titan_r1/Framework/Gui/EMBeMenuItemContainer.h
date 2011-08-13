/*******************************************************
* Creator: Martin Rudefelt
* Portability: native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifdef PLATFORM_BEOS

#ifndef __EM_BE_MENU_ITEM_CONTAINER
#define __EM_BE_MENU_ITEM_CONTAINER

#include "EMMenuItem.h"

#include "EMBeMenuItem.h"
#include "EMRect.h"

#include <string>
#include <MenuItem.h>

class EMBeMenuItemContainer : public EMMenuItem
{
public:
	EMBeMenuItemContainer(string* p_opName, uint32 p_vCommand);
	~EMBeMenuItemContainer();
//	EMRect Bounds() const;
	bool GetEnabled();
	bool GetMarked();
	const char* GetName();
	void* GetNativeView() const;
//	void Hide();
//	void Init();
	void SetEnabled(bool p_vIsEnabled);
	void SetMarked(bool p_vIsMarked);
	void SetName(char* p_vpName);
//	void Show();

private:
//	EMBeMenuItem* m_opNativeItem;
	BMenuItem* m_opNativeItem;
	uint32 m_vCommand;
};

#endif

#endif
