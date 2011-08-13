/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_CHECK_BOX
#define __EM_CHECK_BOX

#include "EMGUIComponent.h"

class EMCheckBox : public EMGUIComponent
{
public:
	virtual ~EMCheckBox();
	virtual void Check(bool p_vCheck) = 0;
	virtual void SetCommand(uint32 p_vCommand) = 0;
	virtual void SetLabel(const char* p_vpLabel) = 0;
	virtual void Enable(bool p_vEnable) = 0;
	virtual bool IsChecked() = 0;
	virtual bool IsEnabled() = 0;

protected:
	EMCheckBox();
};

#endif


