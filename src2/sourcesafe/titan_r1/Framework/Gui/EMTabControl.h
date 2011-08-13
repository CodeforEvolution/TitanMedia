/*******************************************************
* This code is the copyrighted property of Elegant Media
*-------------------------------------------------------
* Creator: Johan Dunberg
* Component: Framework, GUI
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_TAB_CONTROL
#define __EM_TAB_CONTROL

#include "EMGUIComponent.h"

class EMView;

class EMTabControl : public EMGUIComponent
{
public:
	virtual ~EMTabControl();
	virtual void AddChild(EMGUIComponent* p_opView) = 0;
	virtual bool AddTab(char* p_vTabName, EMView* p_opTabView) = 0;
	virtual void Enable(bool p_vEnable) = 0;
	virtual int GetTab() = 0;
	virtual void HideTab() = 0;
	virtual bool IsEnabled() = 0;
	virtual void SetCommand(uint32 p_vCommand) = 0;
	virtual bool SetTab(int p_vTabIndex) = 0;
	virtual void ShowTab() = 0;
//	virtual void SetLabel(const char* p_vpLabel) = 0;

protected:
	EMTabControl();
};

#endif

