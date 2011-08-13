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

#ifndef __EM_RADIO_BUTTON
#define __EM_RADIO_BUTTON

#include "EMGUIComponent.h"

class EMRadioButton : public EMGUIComponent
{
public:
	virtual ~EMRadioButton();
	virtual void Check(bool p_vCheck) = 0;
	virtual void Enable(bool p_vEnable) = 0;
	virtual bool IsChecked() = 0;
	virtual bool IsEnabled() = 0;
	virtual void SetCommand(uint32 p_vCommand) = 0;
	virtual void SetLabel(const char* p_vpLabel) = 0;

protected:
	EMRadioButton();
};

#endif

