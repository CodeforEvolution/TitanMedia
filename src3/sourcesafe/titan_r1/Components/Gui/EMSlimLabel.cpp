#include "EMSlimLabel.h"
#include "EMView.h"

EMSlimLabel::EMSlimLabel(EMView* p_opView, EMRect p_oFrame, string* p_opLabel, EMColor p_oBackgroundColor, bool p_vCenterText) :
m_eMode(EM_SLIM_LABEL_DRAW_NORMAL),
m_oBackgroundColor(p_oBackgroundColor),
m_oFrame(p_oFrame),
m_opLabel(p_opLabel),
m_opView(p_opView),
m_vCenterText(p_vCenterText)
{
	CalculatePositions();
}

void EMSlimLabel::CalculatePositions()
{
	if(m_opLabel == NULL)
		return;

	int32 vAscent;
	int32 vDescent;

	if(m_opView -> LockWindow())
	{
		m_opView -> CalculateStringHeight(m_opLabel -> c_str(), vAscent, vDescent);
		int32 vYSpacing =  ((m_oFrame.GetHeight() - (vAscent + vDescent)) / 2);
		m_vLabelTop = vYSpacing + vAscent - 1;
		if(m_vCenterText)
			m_vLabelLeft = (m_oFrame.GetWidth() - m_opView -> CalculateStringWidth(m_opLabel -> c_str())) / 2;
		else
			m_vLabelLeft = 0;
		m_opView -> UnlockWindow();
	}
}

void EMSlimLabel::Draw(EMRect p_oClippingRect)
{
	if(!m_oFrame.Intersects(p_oClippingRect))
		return;

	if(m_eMode == EM_SLIM_LABEL_DRAW_NORMAL)
	{
		m_opView -> SetHighColor(m_oBackgroundColor);
		m_opView -> SetLowColor(m_oBackgroundColor);
		m_opView -> FillRect(m_oFrame & p_oClippingRect);
		m_opView -> SetHighColor(EM_COLOR_TEXT);
//eo << "EMSlimLabel: DrawString: " << m_oFrame.m_vLeft << ", " << m_vLabelLeft << ", " << m_oFrame.m_vTop << ", " << m_vLabelTop << ef;
		m_opView -> DrawString(m_oFrame.m_vLeft + m_vLabelLeft, m_oFrame.m_vTop + m_vLabelTop, m_opLabel -> c_str());
	}
	else if(m_eMode == EM_SLIM_LABEL_DRAW_BLANK)
	{
		m_opView -> SetHighColor(EM_COLOR_VIEW_BACKGROUND);
		m_opView -> FillRect(m_oFrame);
	}
}

EMRect EMSlimLabel::Frame()
{
	return m_oFrame;
}

void EMSlimLabel::SetFrame(EMRect p_oFrame)
{
	if(m_oFrame == p_oFrame)
		return;
		
	m_oFrame = p_oFrame;
	CalculatePositions();
}

void EMSlimLabel::SetLabel(string* p_opLabel)
{
	m_opLabel = p_opLabel;
	CalculatePositions();
}

void EMSlimLabel::SetMode(EMSlimLabelMode p_eMode)
{
	m_eMode = p_eMode;
}


