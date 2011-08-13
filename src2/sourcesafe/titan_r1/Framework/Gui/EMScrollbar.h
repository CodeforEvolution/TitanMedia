/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_SCROLLBAR
#define __EM_SCROLLBAR

#include "EMGUIComponent.h"
#include "EMGUIGlobals.h"

class EMScrollbar : public EMGUIComponent
{
public:
	virtual ~EMScrollbar();
	virtual void Enable(bool p_vEnable) = 0;
	virtual void GetRange(int32& p_vMin, int32& p_vMax) = 0;
	virtual void GetSteps(int32& p_vSmallStep, int32& p_vBigStep) = 0;
	virtual int32 GetValue() = 0;
	virtual bool IsEnabled() = 0;
	virtual EMOrientation Orientation() = 0;
	virtual void SetFrame(EMRect p_oBounds) = 0;
	virtual void SetPageAndRange(uint32 p_vPage, uint32 p_vMin, uint32 p_vMax) = 0;
//	virtual void SetProportion(int32 p_vProportion) = 0;
//	virtual void SetRange(int32 p_vMin, int32 p_vMax) = 0;
	virtual void SetSteps(int32 p_vSmallStep, int32 p_vBigStep) = 0;
	virtual void SetValue(int32 p_vValue) = 0;
	
protected:
	EMScrollbar();
};

#endif