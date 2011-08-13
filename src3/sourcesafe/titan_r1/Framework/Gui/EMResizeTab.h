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

#ifndef __EM_RESIZE_TAB
#define __EM_RESIZE_TAB

#include "EMRect.h"
class EMView;

class EMResizeTab
{
public:
	~EMResizeTab();
	virtual EMView* GetView() = 0;
	static EMResizeTab* Create(void* p_opParentNativeView, EMRect p_oParentRect);

protected:
	EMResizeTab();
};

#endif
