#include "EMBeScrollbar.h"

#ifdef PLATFORM_BEOS

#include "EMBeScrollbarContainer.h"
//#include "EMBeView.h"
#include "EMListener.h"
#include "EMMessages.h"

#include <list>
#include <View.h>

EMBeScrollbar::EMBeScrollbar(EMBeScrollbarContainer* p_opContainer, BRect p_oFrame, float p_vMinValue, float p_vMaxValue, orientation p_vOrientation) :
BScrollBar(p_oFrame, NULL, NULL, p_vMinValue, p_vMaxValue, p_vOrientation),
m_opContainer(p_opContainer)
{
	// Due to a probable bug in BeOS, it is safest to set the range outside the constructor
	SetRange(p_vMinValue, p_vMaxValue);
}

void EMBeScrollbar::Draw(BRect p_oUpdateRect)
{
	;//cout_commented_out_4_release << "EMBeScrollbar::Draw" << endl;
	BScrollBar::Draw(p_oUpdateRect);
}

void EMBeScrollbar::MouseDown(BPoint p_oPoint)
{
;//cout_commented_out_4_release << "EMBeScrollbar::MouseDown" << endl;
//	EMBeView::ClearLastMouseDownView();
	BScrollBar::MouseDown(p_oPoint);
}

void EMBeScrollbar::ValueChanged(float p_vValue)
{
	m_opContainer -> Notify(EM_SCROLLBAR_CHANGED);
}

#endif