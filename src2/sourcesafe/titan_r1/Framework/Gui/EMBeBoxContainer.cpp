#include "EMBeBoxContainer.h"

#ifdef PLATFORM_BEOS

#include "EMBeBox.h"

#include <Rect.h>
#include <View.h>
#include <Window.h>

EMBeBoxContainer::EMBeBoxContainer(EMRect p_oFrame, uint32 p_vResizingMode, EMBorderStyle p_vStyle, const char* p_vLabel)
{
	uint32 vResizingMode = 0;
	BRect oRect;
	
	oRect.left = p_oFrame.m_vLeft;
	oRect.top = p_oFrame.m_vTop;
	oRect.right = p_oFrame.m_vRight;
	oRect.bottom = p_oFrame.m_vBottom;
	
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
	if((p_vResizingMode & EM_FOLLOW_LEFT_RIGHT) == EM_FOLLOW_LEFT_RIGHT)
		vResizingMode = vResizingMode | B_FOLLOW_LEFT_RIGHT;
	if((p_vResizingMode & EM_FOLLOW_TOP_BOTTOM) == EM_FOLLOW_TOP_BOTTOM)
		vResizingMode = vResizingMode | B_FOLLOW_TOP_BOTTOM;

	border_style vStyle;
	switch(p_vStyle)
	{
	case EM_BORDER_PLAIN:
		vStyle = B_NO_BORDER; // Will instead be handled by overridden draw
		break;
	case EM_BORDER_PLAIN_SHADOWED:
		vStyle = B_PLAIN_BORDER;
		break;
	case EM_BORDER_BEVELED:
		vStyle = B_FANCY_BORDER;
		break;
	case EM_BORDER_NONE:
		vStyle = B_NO_BORDER;
		break;
	default:
		vStyle = B_NO_BORDER;
		break;
	}
	
	auto_ptr<EMBeBox> opBox(new EMBeBox(oRect, NULL, vResizingMode, B_WILL_DRAW, vStyle, p_vStyle));
	m_opBox = opBox;
	m_opBox -> SetViewColor(B_TRANSPARENT_COLOR); // To avoid the background to be redrawn on every update = flicker

	if(p_vLabel != NULL)
		m_opBox -> SetLabel(p_vLabel);
}

EMBeBoxContainer::~EMBeBoxContainer()
{
	if(m_opBox -> Window() != NULL)
		m_opBox -> Window() -> RemoveChild(m_opBox.get());
}

void EMBeBoxContainer::AddChild(EMGUIComponent* p_opView)
{
	m_opBox -> AddChild((BView*) p_opView -> GetNativeView());
	p_opView -> InitComponent();
}

EMRect EMBeBoxContainer::Frame() const
{
	BRect oRect = m_opBox -> Frame();
	return EMRect(oRect.left, oRect.top, oRect.right, oRect.bottom);
}

void* EMBeBoxContainer::GetNativeView() const
{
	return static_cast<void*>(m_opBox.get());
}

void EMBeBoxContainer::Hide()
{
	m_opBox -> Hide();
}

void EMBeBoxContainer::SetColor(EMColor p_sColor)
{
	rgb_color sColor;
	sColor.red = p_sColor.m_vRed;
	sColor.blue = p_sColor.m_vBlue;
	sColor.green = p_sColor.m_vGreen;
	sColor.alpha = p_sColor.m_vAlpha;	
	m_opBox -> SetViewColor(sColor);
}

void EMBeBoxContainer::Show()
{
	m_opBox -> Show();
}

#endif