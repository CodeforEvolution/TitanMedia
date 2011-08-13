/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_MENU_ITEM
#define __EM_MENU_ITEM

//#include "EMGUIComponent.h"

class EMGUIComponent;

class EMMenuItem// : public EMGUIComponent
{
public:
	virtual ~EMMenuItem();
	virtual bool AddToComponent(EMGUIComponent* p_opComponent);
	virtual uint32 GetCommand() = 0;
	//virtual void* GetData() = 0;
	virtual bool GetEnabled() = 0;
	virtual bool GetMarked() = 0;
	virtual const char* GetName() = 0;
	virtual void* GetNativeView() const = 0;
	//virtual void SetData(void p_upData) = 0;
	virtual void SetEnabled(bool p_vIsEnabled) = 0;
	virtual void SetMarked(bool p_vIsMarked) = 0;
	virtual void SetName(const char* p_vpName) = 0;

protected:
	EMMenuItem();
};

#endif

