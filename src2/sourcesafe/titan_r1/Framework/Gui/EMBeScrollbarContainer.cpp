#include "EMBeScrollbarContainer.h"

#ifdef PLATFORM_BEOS

#include "EMListener.h"

#include <Rect.h>
#include <View.h>

EMBeScrollbarContainer::EMBeScrollbarContainer(EMRect p_oFrame, float p_vMinValue, float p_vMaxValue, EMOrientation p_vOrientation)
{
	BRect oRect;

/*
	if(p_vOrientation == EM_VERTICAL)
		oRect.right = oRect.left + B_V_SCROLL_BAR_WIDTH; // The standard BeOS scrollbar size
	else
		oRect.bottom = oRect.top + B_V_SCROLL_BAR_WIDTH; // The standard BeOS scrollbar size, should really be B_H_SCROLL_BAR_HEIGTH
*/
	if(p_vOrientation == EM_HORIZONTAL)
	{
//		oRect.left = p_oFrame.m_vLeft + 1;
		oRect.top = p_oFrame.m_vTop + 1;
//		oRect.right = p_oFrame.m_vRight - 1;
//		oRect.bottom = p_oFrame.m_vBottom - 1;
	}
	else
		oRect.bottom = p_oFrame.m_vBottom - 1;
	orientation vOrientation;
	if(p_vOrientation == EM_VERTICAL)
		vOrientation = B_VERTICAL;
	else
		vOrientation = B_HORIZONTAL;
		
	m_opScrollbar = new EMBeScrollbar(this, oRect, p_vMinValue, p_vMaxValue, vOrientation);
}

EMBeScrollbarContainer::~EMBeScrollbarContainer()
{
}

EMRect EMBeScrollbarContainer::Frame() const
{
	BRect oRect = m_opScrollbar -> Frame();
//	return EMRect(oRect.left - 1, oRect.top - 1, oRect.right + 1, oRect.bottom + 1);
	if(m_opScrollbar -> Orientation() == B_HORIZONTAL)
		return EMRect(oRect.left, oRect.top - 1, oRect.right, oRect.bottom);
	return EMRect(oRect.left, oRect.top, oRect.right, oRect.bottom);	
}

void* EMBeScrollbarContainer::GetNativeView() const
{
	return static_cast<void*>(m_opScrollbar);
}

void EMBeScrollbarContainer::GetRange(float& p_vMin, float& p_vMax)
{
	m_opScrollbar -> GetRange(&p_vMin, &p_vMax);
}

void EMBeScrollbarContainer::GetSteps(float& p_vSmallStep, float& p_vBigStep)
{
	m_opScrollbar -> GetSteps(&p_vSmallStep, &p_vBigStep);
}

float EMBeScrollbarContainer::GetValue()
{
	return m_opScrollbar -> Value();
}

void EMBeScrollbarContainer::Hide()
{
	m_opScrollbar -> Hide();
}

EMOrientation EMBeScrollbarContainer::Orientation()
{
	if(m_opScrollbar -> Orientation() == B_HORIZONTAL)
		return EM_HORIZONTAL;
	return EM_VERTICAL;
}

void EMBeScrollbarContainer::SetFrame(EMRect p_oFrame)
{
	if(Frame() == p_oFrame)
		return;
/*
	if(m_opScrollbar -> Orientation() == B_VERTICAL)
		p_oFrame.m_vRight = p_oFrame.m_vLeft + B_V_SCROLL_BAR_WIDTH; // The standard BeOS scrollbar size
	else
		p_oFrame.m_vBottom = p_oFrame.m_vTop + B_V_SCROLL_BAR_WIDTH; // The standard BeOS scrollbar size, should really be B_H_SCROLL_BAR_HEIGTH
*/
	if(m_opScrollbar -> Orientation() == B_HORIZONTAL)
	{
//		p_oFrame.m_vLeft++;
		p_oFrame.m_vTop++;
//		p_oFrame.m_vRight--;
//		p_oFrame.m_vBottom--;
	}
	
	m_opScrollbar -> ResizeTo(p_oFrame.GetWidth(), p_oFrame.GetHeight());
	m_opScrollbar -> MoveTo(p_oFrame.m_vLeft, p_oFrame.m_vTop);
}

void EMBeScrollbarContainer::SetProportion(float p_vProportion)
{
	m_opScrollbar -> SetProportion(p_vProportion);
}

void EMBeScrollbarContainer::SetRange(float p_vMin, float p_vMax)
{
	if(p_vMax <= p_vMin)
		m_opScrollbar -> SetRange(0, 0); // This should be handled by BeOS, oh well
	else
		m_opScrollbar -> SetRange(p_vMin, p_vMax);
}

void EMBeScrollbarContainer::SetSteps(float p_vSmallStep, float p_vBigStep)
{
	m_opScrollbar -> SetSteps(p_vSmallStep, p_vBigStep);
}

void EMBeScrollbarContainer::SetValue(float p_vValue)
{
	m_opScrollbar -> SetValue(p_vValue);
}

void EMBeScrollbarContainer::Show()
{
	m_opScrollbar -> Show();
}

#endif
