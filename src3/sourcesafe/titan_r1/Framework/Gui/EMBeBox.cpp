#include "EMBeBox.h"

#ifdef PLATFORM_BEOS

#include <Window.h>

EMBeBox::EMBeBox(BRect p_oFrame, const char* p_vName, uint32 p_vResizingMode, uint32 p_vFlags, border_style p_vBorder, EMBorderStyle p_vSpecialBorder) :
BBox(p_oFrame, p_vName, p_vResizingMode, p_vFlags, p_vBorder),
m_vSpecialBorder(p_vSpecialBorder)
{
}

void EMBeBox::Draw(BRect m_oUpdateRect)
{
	if(m_vSpecialBorder == EM_BORDER_PLAIN)
	{
		if(Window() -> Lock())
		{
			StrokeRect(Bounds());
			Window() -> Unlock();
		}
	}
	else
		BBox::Draw(m_oUpdateRect);
}

#endif