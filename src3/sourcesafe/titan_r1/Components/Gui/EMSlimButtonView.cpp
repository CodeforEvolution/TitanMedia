#include "EMSlimButtonView.h"

#include "EMFactory.h"
#include "EMGUIGlobals.h"
#include "EMMessages.h"
#include "EMSlimButton.h"
#include "EMView.h"

EMSlimButtonView::EMSlimButtonView(
				EMView* p_opView, EMRect p_oFrame, 
				uint32 p_vResizingMode, 
				string* p_opOnString, string* p_opOffString, 
				uint32 p_vCommandID, 
				void* p_opCommandParameterOne, 
				void* p_opCommandParameterTwo, 
				void* p_opCommandParameterThree,
				bool p_vLeftBorder, bool p_vTopBorder, 
				bool p_vRightBorder, bool p_vBottomBorder,
				EMColor p_oOnColor,
				EMColor p_oOffColor, EMBitmap* p_opOnBitmap, 
				EMBitmap* p_opOffBitmap, EMBitmap* p_opDisabledOnBitmap,
				EMBitmap* p_opDisabledOffBitmap,
				const char* p_vpToolTipLabel
				) :
m_opView(EMFactory::Instance() -> CreateView(p_oFrame, p_vResizingMode, false))
{
	m_opView -> AddListener(this);
	m_opButton = EM_new EMSlimButton(m_opView, EMRect(0, 0, p_oFrame.GetWidth(), p_oFrame.GetHeight()), p_opOnString, p_opOffString, p_vCommandID,
		p_opCommandParameterOne, p_opCommandParameterTwo, p_opCommandParameterThree,
		p_vLeftBorder, p_vTopBorder, p_vRightBorder, p_vBottomBorder,
		p_oOnColor, p_oOffColor, p_opOnBitmap, p_opOffBitmap, p_opDisabledOnBitmap,
		p_opDisabledOffBitmap, p_vpToolTipLabel);
}

EMSlimButtonView::~EMSlimButtonView()
{
	delete m_opButton;
	delete m_opView;
}

void EMSlimButtonView::Draw(EMRect p_oClippingRect)
{
	m_opButton -> Draw(p_oClippingRect, m_opView);
}

EMSlimButton* EMSlimButtonView::GetButton()
{
	return m_opButton;
}

EMView* EMSlimButtonView::GetView()
{
	return m_opView;
}

void EMSlimButtonView::InvalidateButton()
{
	if(m_opView -> LockWindow())
	{
		m_opView -> Invalidate(m_opView -> Bounds());
		m_opView -> UnlockWindow();
	}
}

bool EMSlimButtonView::MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage)
{
	int32 vX;
	int32 vY;
	bool vButtonOne;
	bool vButtonTwo;
	bool vButtonThree;

	switch(p_vMessage)
	{
	case EM_DRAW:
//;//cout_commented_out_4_release << "EMSlimButtonView::MessageReceived: EM_DRAW" << endl;
		Draw(m_opView -> GetClippingRect());
		return true;
	case EM_MOUSE_DOWN:
		m_opView -> GetMouse(vX, vY, vButtonOne, vButtonTwo, vButtonThree);
		if(m_opButton -> MouseDown(vX, vY))
		{
//			m_opButton -> ToggleOn();
			m_opView -> Invalidate(m_opButton -> Frame());
		}
		return true;
	case EM_MOUSE_UP:
		m_opView -> GetMouse(vX, vY, vButtonOne, vButtonTwo, vButtonThree);
		if(m_opButton -> MouseUp(vX, vY))
			m_opView -> Invalidate(m_opButton -> Frame());
		return true;
	default:
		return false;
	}
}

// Do not use the buttons SetFrame directly since the view 
// must be resized as well. Also, the EMSlimButton's frame
// must be relative to its related view
void EMSlimButtonView::SetFrame(EMRect p_oFrame)
{
	m_opView -> SetFrame(p_oFrame);
	m_opButton -> SetFrame(m_opView -> Bounds());
}


