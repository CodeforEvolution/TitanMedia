/*******************************************************
* Creator: Martin Rudefelt
* Portability: native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifdef PLATFORM_BEOS

#ifndef __EM_BE_BOX
#define __EM_BE_BOX

#include "EMGUIGlobals.h"

#include <Box.h>
#include <Rect.h>

class EMBeBox : public BBox
{
public:
	EMBeBox(BRect p_oFrame, const char* p_vName, uint32 p_vResizingMode, uint32 p_vFlags, border_style p_vBorder, EMBorderStyle p_vSpecialBorder);
	void Draw(BRect m_oUpdateRect);
	
private:
	uint32 m_vSpecialBorder;
};

#endif

#endif

