/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_BUTTON
#define __EM_BUTTON

#include "EMGUIComponent.h"

class EMButton : public EMGUIComponent
{
public:
	virtual ~EMButton();
	virtual void Enable(bool p_vEnable) = 0;
	virtual bool IsEnabled() = 0;
	virtual void SetCommand(uint32 p_vCommand) = 0;
	virtual void SetLabel(const char* p_vpLabel) = 0;

protected:
	EMButton();
};

#endif


