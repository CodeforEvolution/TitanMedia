/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_GROUP_BOX
#define __EM_GROUP_BOX

#include "EMGUIComponent.h"

class EMGroupBox : public EMGUIComponent
{
public:
	virtual ~EMGroupBox();
	virtual void AddChild(EMGUIComponent* p_opView) = 0;
	virtual void Enable(bool p_vEnable) = 0;
	virtual bool IsEnabled() = 0;
	virtual void SetLabel(const char* p_vpLabel) = 0;

protected:
	EMGroupBox();
};

#endif


