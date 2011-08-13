#include "EMBeTextViewContainer.h"

#ifdef PLATFORM_BEOS

#include "EMGUIGlobals.h"

EMBeTextViewContainer::EMBeTextViewContainer(EMRect p_oFrame, const char* p_vpName, uint32 p_vResizingMode)
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
	BRect oRect2(0, 0, p_oFrame.GetWidth(), p_oFrame.GetHeight());

	m_opTextView = new EMBeTextView(this, oRect, oRect2, p_vpName, vResizingMode);
}

EMBeTextViewContainer::~EMBeTextViewContainer()
{
}

EMRect EMBeTextViewContainer::Frame() const
{
	BRect oRect = m_opTextView -> Frame();
	return EMRect(oRect.left, oRect.top, oRect.right, oRect.bottom);
}

void* EMBeTextViewContainer::GetNativeView() const
{
	return static_cast<void*>(m_opTextView);
}

void EMBeTextViewContainer::Hide()
{
	m_opTextView -> Hide();
}

void EMBeTextViewContainer::InitComponent()
{
}

void EMBeTextViewContainer::Insert(const char* p_vpText)
{
	m_opTextView -> Insert(p_vpText);
}

void EMBeTextViewContainer::Invalidate()
{
	m_opTextView -> Invalidate();
}

void EMBeTextViewContainer::SelectAll()
{
	m_opTextView -> SelectAll();
}

void EMBeTextViewContainer::SetFocus()
{
	m_opTextView -> MakeFocus();
//	m_opTextView -> 
}

void EMBeTextViewContainer::SetFrame(EMRect p_oFrame)
{
	if(Frame() == p_oFrame)
		return;
	m_opTextView -> ResizeTo(p_oFrame.GetWidth(), p_oFrame.GetHeight());
	m_opTextView -> MoveTo(p_oFrame.m_vLeft, p_oFrame.m_vTop);
}

void EMBeTextViewContainer::SetText(const char* p_vpText)
{
	m_opTextView -> SetText(p_vpText);
/*
	m_opTextView -> ScrollToOffset(0);
	rgb_color oColor;
	oColor.red = 255;
	oColor.blue = 0;
	oColor.green = 0;
	m_opTextView -> SetFontAndColor(be_bold_font, B_FONT_ALL, &oColor);
	m_opTextView -> Hide();
	m_opTextView -> Show();
*/
}

void EMBeTextViewContainer::Show()
{
	m_opTextView -> Show();
}

void EMBeTextViewContainer::SetMaxBytes(int32 p_vMaxBytes)
{
	m_opTextView -> SetMaxBytes(p_vMaxBytes);
}

void EMBeTextViewContainer::SetWordWrap(bool p_vWrap)
{
	m_opTextView -> SetWordWrap(p_vWrap);
}

const char* EMBeTextViewContainer::Text()
{
	return m_opTextView -> Text();
}

#endif


