#include "EMBeTextControlContainer.h"

#ifdef PLATFORM_BEOS

#include "EMGUIGlobals.h"

#include <TextView.h>

EMBeTextControlContainer::EMBeTextControlContainer(EMRect p_oFrame, const char* p_vpText, uint32 p_vResizingMode)
{
	uint32 vResizingMode = 0;
	
	if((p_vResizingMode & EM_FOLLOW_LEFT) == EM_FOLLOW_LEFT)
		vResizingMode = vResizingMode | B_FOLLOW_LEFT;
	if((p_vResizingMode & EM_FOLLOW_RIGHT) == EM_FOLLOW_RIGHT)
		vResizingMode = vResizingMode | B_FOLLOW_RIGHT;
	if((p_vResizingMode & EM_FOLLOW_H_CENTER) == EM_FOLLOW_H_CENTER)
		vResizingMode = vResizingMode | B_FOLLOW_H_CENTER;
	if((p_vResizingMode & EM_FOLLOW_TOP) == EM_FOLLOW_TOP)
		vResizingMode = vResizingMode | B_FOLLOW_TOP;
	if((p_vResizingMode & EM_FOLLOW_BOTTOM) == EM_FOLLOW_BOTTOM)
		vResizingMode = vResizingMode | B_FOLLOW_BOTTOM;
	if((p_vResizingMode & EM_FOLLOW_V_CENTER) == EM_FOLLOW_V_CENTER)
		vResizingMode = vResizingMode | B_FOLLOW_V_CENTER;

	BRect oRect(p_oFrame.m_vLeft, p_oFrame.m_vTop, p_oFrame.m_vRight, p_oFrame.m_vBottom);

	m_opTextControl = new EMBeTextControl(this, oRect, p_vpText, vResizingMode);
}

EMBeTextControlContainer::~EMBeTextControlContainer()
{
}

EMRect EMBeTextControlContainer::Frame() const
{
	BRect oRect = m_opTextControl -> Frame();
	return EMRect(oRect.left, oRect.top, oRect.right, oRect.bottom);
}

void* EMBeTextControlContainer::GetNativeView() const
{
	return static_cast<void*>(m_opTextControl);
}

void EMBeTextControlContainer::Hide()
{
	m_opTextControl -> Hide();
}

void EMBeTextControlContainer::InitComponent()
{
	m_opTextControl -> SetTarget(m_opTextControl);
}

void EMBeTextControlContainer::Invalidate()
{
	m_opTextControl -> Invalidate();
}

void EMBeTextControlContainer::SelectAll()
{
	m_opTextControl -> TextView() -> SelectAll();
}

void EMBeTextControlContainer::SetFocus()
{
	m_opTextControl -> MakeFocus();
}

void EMBeTextControlContainer::SetFrame(EMRect p_oFrame)
{
	if(Frame() == p_oFrame)
		return;
	m_opTextControl -> ResizeTo(p_oFrame.GetWidth(), p_oFrame.GetHeight());
	m_opTextControl -> MoveTo(p_oFrame.m_vLeft, p_oFrame.m_vTop);
}

void EMBeTextControlContainer::SetText(const char* p_vpText)
{
	m_opTextControl -> SetText(p_vpText);
}

void EMBeTextControlContainer::Show()
{
	m_opTextControl -> Show();
}

void EMBeTextControlContainer::SetMaxBytes(int32 p_vMaxBytes)
{
	m_opTextControl -> TextView() -> SetMaxBytes(p_vMaxBytes);
}
/*
void EMBeTextControlContainer::SetWordWrap(bool p_vWrap)
{
	m_opTextControl -> SetWordWrap(p_vWrap);
}
*/
const char* EMBeTextControlContainer::Text()
{
	return m_opTextControl -> Text();
}

#endif


