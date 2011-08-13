/*******************************************************
* Creator: Martin Rudefelt
* Portability: native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifdef PLATFORM_BEOS

#ifndef __EM_BE_SCROLLBAR
#define __EM_BE_SCROLLBAR

#include <Rect.h>
#include <ScrollBar.h>

class EMBeScrollbarContainer;
class EMListener;

class EMBeScrollbar : public BScrollBar
{
public:
	EMBeScrollbar(EMBeScrollbarContainer* p_opContainer, BRect p_oFrame, float p_vMinValue, float p_vMaxValue, orientation p_vOrientation);
	void Draw(BRect p_oUpdateRect);
	void MouseDown(BPoint p_oPoint);
	void ValueChanged(float p_vValue);
	
private:
	EMBeScrollbarContainer* m_opContainer;
};

#endif

#endif
