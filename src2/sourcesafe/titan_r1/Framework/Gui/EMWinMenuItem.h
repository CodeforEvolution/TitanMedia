/*******************************************************
* Creator: Martin Rudefelt
* Portability: native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#ifndef __EM_WIN_MENU_ITEM
#define __EM_WIN_MENU_ITEM

#include "EMMenuItem.h"

#include <string>
#include <windows.h>

class EMGUIComponent;

class EMWinMenuItem : public EMMenuItem
{
public:
	EMWinMenuItem(string* p_opName, uint32 p_vCommand);
	~EMWinMenuItem();
	bool AddToComponent(EMGUIComponent* p_opComponent);
	uint32 GetCommand();
	//void* GetData();
	bool GetEnabled();
	bool GetMarked();
	const char* GetName();
	void* GetNativeView() const;
	//void SetData(void p_upData);
	void SetEnabled(bool p_vIsEnabled);
	void SetMarked(bool p_vIsMarked);
	void SetName(const char* p_vpName);

private:
	string m_oName;
	HMENU m_opParentMenu;
	//void* m_upData;
	uint32 m_vCommand;
	//uint32 m_vID;
	bool m_vIsEnabled;
	bool m_vIsMarked;
};

#endif

#endif
