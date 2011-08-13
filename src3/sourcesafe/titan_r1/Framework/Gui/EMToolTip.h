/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_TOOL_TIP
#define __EM_TOOL_TIP

#include "EMGUIComponent.h"

class EMToolTip : public EMGUIComponent
{
public:
	virtual ~EMToolTip();
	virtual void Enable(bool p_vEnable) = 0;
	virtual bool IsEnabled() = 0;
	virtual void SetLabel(const char* p_vpLabel) = 0;
	virtual void SetPosition(int32 p_vX, int32 p_vY) = 0;

protected:
	EMToolTip();
};

#endif


