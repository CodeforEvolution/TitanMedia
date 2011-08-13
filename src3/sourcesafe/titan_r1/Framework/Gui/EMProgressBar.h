/*******************************************************
* This code is the copyrighted property of Elegant Media
*-------------------------------------------------------
* Creator: Johan Dunberg
* Component: Framework
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/
#include "EMGlobals.h"

#ifndef __EM_PROGRESS_BAR
#define __EM_PROGRESS_BAR

#include "EMGUIComponent.h"

class EMProgressBar : public EMGUIComponent
{
public:
	~EMProgressBar();
	virtual uint16 GetPosition() = 0;
	virtual bool IsFinnished() = 0;
	virtual void SetEndValue(uint16 p_vEnd) = 0;
	virtual void SetPosition(uint16 p_vPosition) = 0;

protected:
	EMProgressBar();
};

#endif

