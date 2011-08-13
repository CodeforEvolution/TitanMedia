/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_LABEL
#define __EM_LABEL

#include "EMGUIComponent.h"

class EMLabel : public EMGUIComponent
{
public:
	virtual ~EMLabel();
	virtual void Enable(bool p_vEnable) = 0 ;
	virtual bool IsEnabled() = 0;
	virtual void SetLabel(const char* p_vpLabel) = 0;
	virtual float GetStringWidth() = 0;

protected:
	EMLabel();
};

#endif


