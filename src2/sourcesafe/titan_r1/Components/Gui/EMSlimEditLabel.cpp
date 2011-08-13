#include "EMSlimEditLabel.h"

#include "EMFactory.h"
#include "EMMessages.h"
#include "EMTextControl.h"
#include "EMView.h"

EMTextControl* EMSlimEditLabel::m_opTextControl = NULL;

EMSlimEditLabel::EMSlimEditLabel(EMRect p_oFrame, EMColor p_oBackgroundColor, EMView* p_opView,
				bool p_vLeftBorder,	bool p_vTopBorder, bool p_vRightBorder, 
				bool p_vBottomBorder, int32 p_vLabelInset) :
m_oBackgroundColor(p_oBackgroundColor),
m_oFrame(p_oFrame),
m_oInsideBorder(p_oFrame),
m_opView(p_opView),
m_vBottomBorder(p_vBottomBorder),
m_vIsEditing(false),
m_vLabelInset(p_vLabelInset),
m_vLeftBorder(p_vLeftBorder),
m_vRightBorder(p_vRightBorder),
m_vTopBorder(p_vTopBorder)
{
	if(m_vLeftBorder)
		m_oInsideBorder.m_vLeft += 1;
	if(m_vTopBorder)
		m_oInsideBorder.m_vTop += 1;
	if(m_vRightBorder)
		m_oInsideBorder.m_vRight -= 1;
	if(m_vBottomBorder)
		m_oInsideBorder.m_vBottom -= 1;
}

void EMSlimEditLabel::CalculatePositions()
{
	int32 vAscent;
	int32 vDescent;
	
	m_opView -> CalculateStringHeight(m_oLabel.c_str(), vAscent, vDescent);
	int32 vYSpacing =  m_oInsideBorder.m_vTop + ((m_oInsideBorder.GetHeight() - (vAscent + vDescent)) / 2);
	int32 vXSpacing =  m_oInsideBorder.m_vLeft + ((m_oInsideBorder.GetHeight() - (vAscent + vDescent)) / 2);
	m_vOnStringTop = vYSpacing + vAscent - 1;
	m_vOnStringLeft = vXSpacing + m_vLabelInset;//m_oFrame.m_vLeft + ((m_oFrame.GetWidth() - m_opView -> CalculateStringWidth(m_opOnString -> c_str())) / 2);
}

// This method should be called when exiting the application
void EMSlimEditLabel::CleanUp()
{
	delete m_opTextControl;
}

bool EMSlimEditLabel::Contains(int32 p_vX, int32 p_vY)
{
	return m_oFrame.Contains(p_vX, p_vY);
}

void EMSlimEditLabel::Draw(EMRect p_oClippingRect)
{
	if(!m_oFrame.Intersects(p_oClippingRect) || m_vIsEditing)
		return;

	m_opView -> SetHighColor(EM_COLOR_BUTTON_BORDER);
	if(m_vLeftBorder)
		m_opView -> StrokeLine(m_oFrame.m_vLeft, m_oFrame.m_vTop, m_oFrame.m_vLeft, m_oFrame.m_vBottom);
	if(m_vTopBorder)
		m_opView -> StrokeLine(m_oFrame.m_vLeft, m_oFrame.m_vTop, m_oFrame.m_vRight, m_oFrame.m_vTop);
	if(m_vRightBorder)
		m_opView -> StrokeLine(m_oFrame.m_vRight, m_oFrame.m_vTop, m_oFrame.m_vRight, m_oFrame.m_vBottom);
	if(m_vBottomBorder)
		m_opView -> StrokeLine(m_oFrame.m_vLeft, m_oFrame.m_vBottom, m_oFrame.m_vRight, m_oFrame.m_vBottom);

	m_opView -> SetHighColor(m_oBackgroundColor);
	m_opView -> SetLowColor(m_oBackgroundColor);
	m_opView -> FillRect(m_oInsideBorder & p_oClippingRect);
	m_opView -> SetHighColor(EM_COLOR_TEXT);
	m_opView -> DrawString(m_vOnStringLeft, m_vOnStringTop, m_oLabel.c_str());
//;//cout_commented_out_4_release << "EMSlimEditLabel::Draw:" << m_vOnStringLeft << ", " << m_vOnStringTop << ", ";
//m_oFrame.Print();
}

EMRect EMSlimEditLabel::Frame()
{
	return m_oFrame;
}

string EMSlimEditLabel::GetLabel()
{
	return m_oLabel;
}

EMTextControl* EMSlimEditLabel::GetTextControl(EMRect p_oFrame)
{
	if(m_opTextControl != NULL)
		delete m_opTextControl;
	m_opTextControl = EMFactory::Instance() ->CreateTextControl(p_oFrame, "", 0, EM_FOLLOW_NONE, true);
	return m_opTextControl;
}

bool EMSlimEditLabel::Intersects(EMRect p_oRect)
{
	return m_oFrame.Intersects(p_oRect);
}

bool EMSlimEditLabel::MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage)
{
;//cout_commented_out_4_release << "EMSlimEditLabel::MessageReceived" << endl;

	if(p_opSender != m_opTextControl)
		return false;
/*
	if(p_vMessage == EM_TEXT_CHANGED)
	{
		;//cout_commented_out_4_release << "EM_TEXT_CHANGED" << endl;
		SetLabel(string(m_opTextControl -> Text()));
//		m_opView -> RemoveChild(m_opTextControl);
//		delete m_opTextControl;
		m_vIsEditing = false;
		return true;
	}
*/
	if(p_vMessage == EM_TEXT_CHANGED && m_vIsEditing)
	{
		if(m_opView -> LockWindow())
		{
			SetLabel(m_opTextControl -> Text());
			m_vIsEditing = false;
			Notify(EM_TEXT_CHANGED);
			m_opView -> UnlockWindow();
			return true;
		}
	}
	if(p_vMessage == EM_TEXT_CHANGE_UNDOED && m_vIsEditing)
	{
		m_vIsEditing = false;
		return true;
	}

	return false;
}

bool EMSlimEditLabel::MouseDown(int32 p_vX, int32 p_vY)
{
/*
	if(!m_oFrame.Contains(p_vX, p_vY) && m_vIsEditing)
	{
		SetLabel(string(m_opTextControl -> Text()));
		m_opView -> RemoveChild(m_opTextControl);
		delete m_opTextControl;
		m_vIsEditing = false;
		return true;
	}
*/
	return false;
}	
			
bool EMSlimEditLabel::MouseDoubleClick(int32 p_vX, int32 p_vY)
{
	if(!m_oFrame.Contains(p_vX, p_vY))
		return false;

	EMRect oEditFrame(m_oFrame);
	oEditFrame.m_vBottom += 4; // Adjust for a larger font
	// Offset the frame since adding a child does not take scroll offsets into consideration
	oEditFrame.OffsetBy(-1 * (m_opView -> Bounds()).m_vLeft, -1 * (m_opView -> Bounds()).m_vTop);
	EMTextControl* opTextControl = GetTextControl(oEditFrame);
	opTextControl -> AddListener(this);
//	opTextControl -> SetFrame(m_oFrame);
	/*opTextControl*/m_opView -> AddChild(m_opTextControl);
//	opTextControl -> SetWordWrap(false);
	opTextControl -> SetText(m_oLabel.c_str());
	opTextControl -> SelectAll();
	opTextControl -> SetFocus();
	m_vIsEditing = true;
	return true;
}

void EMSlimEditLabel::SetBackgroundColor(EMColor p_oColor)
{
	m_oBackgroundColor = p_oColor;
}

void EMSlimEditLabel::SetFrame(EMRect p_oFrame)
{
//	m_opTextControl -> SetFrame(p_oFrame);
	m_oFrame = p_oFrame;
	m_oInsideBorder = p_oFrame;
	if(m_vLeftBorder)
		m_oInsideBorder.m_vLeft += 1;
	if(m_vTopBorder)
		m_oInsideBorder.m_vTop += 1;
	if(m_vRightBorder)
		m_oInsideBorder.m_vRight -= 1;
	if(m_vBottomBorder)
		m_oInsideBorder.m_vBottom -= 1;
}

void EMSlimEditLabel::SetLabel(const char* p_oLabel)
{
	m_oLabel = p_oLabel;
	CalculatePositions();
}



