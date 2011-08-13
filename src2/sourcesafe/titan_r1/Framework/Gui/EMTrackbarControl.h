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

#ifndef __EM_TRACKBAR_CONTROL
#define __EM_TRACKBAR_CONTROL

#include "EMGUIComponent.h"

class EMView;

class EMTrackbarControl : public EMGUIComponent
{
public:
	virtual ~EMTrackbarControl();
	virtual void AddChild(EMGUIComponent* p_opView) = 0;
	virtual void Enable(bool p_vEnable) = 0;
	virtual int32 GetPosition() = 0;
	virtual bool IsEnabled() = 0;
	virtual void SetCommand(uint32 p_vCommand) = 0;
	virtual void SetRange(int32 p_vStart, int32 p_vEnd) = 0;
	virtual void SetPosition(int32 p_vPosition) = 0;
	virtual void SetTick(int32 p_vPosition) = 0;

protected:
	EMTrackbarControl();
};

#endif
