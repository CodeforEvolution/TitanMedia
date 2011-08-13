#include "EMSlimButton.h"

#include "EMBitmap.h"
#include "EMCommandRepository.h"
#include "EMFactory.h"
#include "EMThreadRepository.h"
#include "EMToolTip.h"
#include "EMView.h"

#define SLIM_BUTTON_BORDER_SIZE 1

const int FLASH_TIME = 4; // The time the buttons stays on during a call to Flash()
const int32 ON_OFFSET = 1; // The text offset that simulates the text being "pressed in" into the screen

// Does not get ownership of the bitmaps and strings (EMResourceRepository probably owns them)
EMSlimButton::EMSlimButton(EMView* p_opView, EMRect p_oFrame, string* p_opOnString, string* p_opOffString, 
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
m_eMode(EM_SLIM_BUTTON_STAY_DOWN),
m_oFrame(p_oFrame),
m_oInsideBorder(p_oFrame),
m_oOffColor(p_oOffColor),
m_opCommandParameterOne(p_opCommandParameterOne),
m_opCommandParameterTwo(p_opCommandParameterTwo),
m_opCommandParameterThree(p_opCommandParameterThree),
m_opDisabledOffBitmap(p_opDisabledOffBitmap),
m_opDisabledOnBitmap(p_opDisabledOnBitmap),
m_opOffBitmap(p_opOffBitmap),
m_opOffString(p_opOffString),
m_opOnBitmap(p_opOnBitmap),
m_oOnColor(p_oOnColor),
m_opOnString(p_opOnString),
m_opToolTip(NULL),
m_opView(p_opView),
m_vBottomBorder(p_vBottomBorder),
m_vCommandID(p_vCommandID),
m_vEnabled(true),
m_vFlash(false),
m_vFlashCounter(-1),
m_vLeftBorder(p_vLeftBorder),
m_vMousePressed(false),
m_vOn(false),
m_vRightBorder(p_vRightBorder),
m_vTopBorder(p_vTopBorder),
m_vVisible(true)
{
/*
	if(m_opDisabledOffBitmap != NULL || m_opDisabledOnBitmap != NULL || m_opOffBitmap != NULL || m_opOnBitmap != NULL)
		if(m_opDisabledOffBitmap == NULL || m_opDisabledOnBitmap == NULL || m_opOffBitmap == NULL || m_opOnBitmap == NULL)
			EMDebugger("EMSlimButton: Some but not all bitmap pointers points to a bitmap");
*/
	if(m_opView -> LockWindow())
	{
		CalculatePositions();
		m_opView -> UnlockWindow();
	}

	if(m_vLeftBorder)
		m_oInsideBorder.m_vLeft += 1;
	if(m_vTopBorder)
		m_oInsideBorder.m_vTop += 1;
	if(m_vRightBorder)
		m_oInsideBorder.m_vRight -= 1;
	if(m_vBottomBorder)
		m_oInsideBorder.m_vBottom -= 1;

	if(p_vpToolTipLabel != NULL)
		m_opToolTip = EMFactory::Instance() -> CreateToolTip(p_oFrame, p_vpToolTipLabel, p_opView -> GetNativeView(), true);
}

EMSlimButton::~EMSlimButton()
{
	delete m_opToolTip;
}

void EMSlimButton::CalculatePositions()
{
	if(m_opOnString != NULL)
	{
		int32 vAscent;
		int32 vDescent;
		if(m_opView -> LockWindow())
		{
			m_opView -> CalculateStringHeight(m_opOnString -> c_str(), vAscent, vDescent);
			int32 vYSpacing = (m_oFrame.GetHeight() - (vAscent + vDescent)) / 2;
			m_vOnStringTop = m_oFrame.m_vTop + vYSpacing + vAscent - 1/* + SLIM_BUTTON_BORDER_SIZE*/;
			m_vOnStringLeft = m_oFrame.m_vLeft + ((m_oFrame.GetWidth() - m_opView -> CalculateStringWidth(m_opOnString -> c_str())) / 2);
			m_opView -> UnlockWindow();
		}
	}
	if(m_opOffString != NULL)
	{
		int32 vAscent;
		int32 vDescent;
		if(m_opView -> LockWindow())
		{
			m_opView -> CalculateStringHeight(m_opOffString -> c_str(), vAscent, vDescent);
			int32 vYSpacing = (m_oFrame.GetHeight() - (vAscent + vDescent)) / 2;
			m_vOffStringTop = m_oFrame.m_vTop + vYSpacing + vAscent - 1/* + SLIM_BUTTON_BORDER_SIZE*/;
			m_vOffStringLeft = m_oFrame.m_vLeft + ((m_oFrame.GetWidth() - m_opView -> CalculateStringWidth(m_opOffString -> c_str())) / 2);
			m_opView -> UnlockWindow();
		}
	}
}

bool EMSlimButton::Contains(int32 p_vX, int32 p_vY)
{
	return m_oFrame.Contains(p_vX, p_vY);
}

void EMSlimButton::Draw(EMRect p_oClippingRect, EMView* p_opView)
{
	if(!m_vVisible)
		return;
	
	if(!m_oFrame.Intersects(p_oClippingRect))
		return;

	if(m_eMode != EM_SLIM_BUTTON_DRAW_BLANK || m_eMode != EM_SLIM_BUTTON_DRAW_LABEL)
	{
		p_opView -> SetHighColor(EM_COLOR_VIEW_BACKGROUND);
		p_opView -> FillRect(m_oFrame);
	}
	if(m_eMode != EM_SLIM_BUTTON_DRAW_BLANK)
	{
		if(m_eMode != EM_SLIM_BUTTON_DRAW_LABEL)
		{
			if(m_vOn)
			{
				p_opView -> SetHighColor(EMColor(245, 245, 255, 255));
				if(m_vRightBorder)
					p_opView -> StrokeLine(m_oFrame.m_vRight, m_oFrame.m_vTop, m_oFrame.m_vRight, m_oFrame.m_vBottom);
				if(m_vBottomBorder)
					p_opView -> StrokeLine(m_oFrame.m_vLeft, m_oFrame.m_vBottom, m_oFrame.m_vRight, m_oFrame.m_vBottom);
				p_opView -> SetHighColor(EM_COLOR_VIEW_BACKGROUND_SHADOW);
				if(m_vLeftBorder)
					p_opView -> StrokeLine(m_oFrame.m_vLeft, m_oFrame.m_vTop, m_oFrame.m_vLeft, m_oFrame.m_vBottom);
				if(m_vTopBorder)
					p_opView -> StrokeLine(m_oFrame.m_vLeft, m_oFrame.m_vTop, m_oFrame.m_vRight, m_oFrame.m_vTop);
			}
			else		
			{
				p_opView -> SetHighColor(EM_COLOR_VIEW_BACKGROUND_SHADOW);
				if(m_vRightBorder)
					p_opView -> StrokeLine(m_oFrame.m_vRight, m_oFrame.m_vTop, m_oFrame.m_vRight, m_oFrame.m_vBottom);
				if(m_vBottomBorder)
					p_opView -> StrokeLine(m_oFrame.m_vLeft, m_oFrame.m_vBottom, m_oFrame.m_vRight, m_oFrame.m_vBottom);
				p_opView -> SetHighColor(EMColor(245, 245, 255, 255));
				if(m_vLeftBorder)
					p_opView -> StrokeLine(m_oFrame.m_vLeft, m_oFrame.m_vTop, m_oFrame.m_vLeft, m_oFrame.m_vBottom);
				if(m_vTopBorder)
					p_opView -> StrokeLine(m_oFrame.m_vLeft, m_oFrame.m_vTop, m_oFrame.m_vRight, m_oFrame.m_vTop);
			}
		}
		
		if(m_opOnBitmap != NULL) // If this one is not NULL, neither are the other bitmap pointers
		{
			if(!m_vEnabled)
			{
				if(m_vOn)
					p_opView -> DrawBitmap(m_opDisabledOnBitmap, m_oInsideBorder.m_vLeft, m_oInsideBorder.m_vTop);
				else
					p_opView -> DrawBitmap(m_opDisabledOffBitmap, m_oInsideBorder.m_vLeft, m_oInsideBorder.m_vTop);
			}
			else
			{
				if(m_vOn)
					p_opView -> DrawBitmap(m_opOnBitmap, m_oInsideBorder.m_vLeft, m_oInsideBorder.m_vTop);
				else
					p_opView -> DrawBitmap(m_opOffBitmap, m_oInsideBorder.m_vLeft, m_oInsideBorder.m_vTop);
			}
		}
		else
		{
			if(m_eMode != EM_SLIM_BUTTON_DRAW_LABEL)
			{
				if(!m_vEnabled)
				{
					p_opView -> SetHighColor(EM_COLOR_BUTTON_BACKGROUND_DISABLED);
					p_opView -> SetLowColor(EM_COLOR_BUTTON_BACKGROUND_DISABLED);
				}
				else if(m_vOn)
				{
					p_opView -> SetHighColor(m_oOnColor);
					p_opView -> SetLowColor(m_oOnColor);
				}
				else
				{
					p_opView -> SetHighColor(m_oOffColor);
					p_opView -> SetLowColor(m_oOffColor);
				}
				p_opView -> FillRect(m_oInsideBorder);
			}
			else
				p_opView -> SetLowColor(EM_COLOR_VIEW_BACKGROUND);
			if(!m_vEnabled)
				p_opView -> SetHighColor(EM_COLOR_BUTTON_TEXT_DISABLED);
			else
				p_opView -> SetHighColor(EM_COLOR_BUTTON_TEXT_ENABLED);
			if(m_vOn && m_opOnString != NULL)
				p_opView -> DrawString(m_vOnStringLeft + ON_OFFSET, m_vOnStringTop + ON_OFFSET, m_opOnString -> c_str());
			else if(!m_vOn && m_opOffString != NULL)
				p_opView -> DrawString(m_vOffStringLeft, m_vOffStringTop, m_opOffString -> c_str());
		}
	}
}	
	
void EMSlimButton::ExecuteCommand()
{
	if(m_vCommandID == COMMAND_ID_NULL)
		return;
	if(m_opCommandParameterOne == NULL)
		EMCommandRepository::Instance() -> ExecuteCommand(m_vCommandID);
	else
	{
		// If there is no second parameter, it sends itself as the second parameter
		if(m_opCommandParameterTwo == NULL)
			EMCommandRepository::Instance() -> ExecuteCommand(m_vCommandID, m_opCommandParameterOne, this);
		// If there is no third parameter, it sends itself as the third parameter
		else if(m_opCommandParameterThree == NULL)
			EMCommandRepository::Instance() -> ExecuteCommand(m_vCommandID, m_opCommandParameterOne, m_opCommandParameterTwo, this);
		else
			EMCommandRepository::Instance() -> ExecuteCommand(m_vCommandID, m_opCommandParameterOne, m_opCommandParameterTwo, m_opCommandParameterThree);
	}
}

void EMSlimButton::Flash()
{
	// Flash has no effect if the button is pressed
	if(m_vMousePressed)
		return;
		
	if(m_vFlash) // A flash is already in progress
			m_vFlashCounter = FLASH_TIME;
	else
	{
		if(m_opView -> LockWindow())
		{
			m_vFlash = true;
			m_vFlashCounter = FLASH_TIME;
			SetOn(true);
			m_opView -> BeginPaint();
			Draw(Frame(), m_opView);
			m_opView -> EndPaint();
			EMThreadRepository::Instance() -> GetThread(GUI_SERVICE_THREAD_NAME) -> AddListener(this);
			m_opView -> UnlockWindow();
		}
	}
}

EMRect EMSlimButton::Frame()
{
	return m_oFrame;
}

bool EMSlimButton::GetEnabled()
{
	return m_vEnabled;
}

// Get it on!
bool EMSlimButton::GetOn()
{
	return m_vOn;
}

bool EMSlimButton::GetVisible()
{
	return m_vVisible;
}

bool EMSlimButton::Intersects(EMRect p_oRect)
{
	return m_oFrame.Intersects(p_oRect);
}

// Returns true if it should be invalidated
bool EMSlimButton::MouseDown(int32 p_vX, int32 p_vY)
{
//;//cout_commented_out_4_release << "EMSlimButton::MouseDown" << endl;
	if(!m_vEnabled || !m_vVisible)
		return false;
	if(!m_oFrame.Contains(p_vX, p_vY))
		return false;
	// Mouse button cannot be pressed during a flash
	if(m_vFlash)
		return false;
		
	if(m_eMode == EM_SLIM_BUTTON_DRAW_BLANK || m_eMode == EM_SLIM_BUTTON_DRAW_LABEL)
	{
;//cout_commented_out_4_release << "m_eMode == EM_SLIM_BUTTON_DRAW_BLANK/EM_SLIM_BUTTON_DRAW_LABEL" << endl;
		return false;
	}
	
	if(m_vOn && m_eMode == EM_SLIM_BUTTON_RADIO)
		return false;
		
	m_vMousePressed = true;
	// Only toggle button status if there is no command, assume that the command switches the button state
//	if(!m_vCommandID)
	{
;//cout_commented_out_4_release << "no command ID" << endl;
		ToggleOn();
	}
//	else
	{
;//cout_commented_out_4_release << "execute command" << endl; 
		ExecuteCommand();
	}
	if(m_eMode == EM_SLIM_BUTTON_FOLLOW_MOUSE_REPEAT_COMMAND)
	{
		m_vFlashCounter = -1;
		EMThreadRepository::Instance() -> GetThread(GUI_SERVICE_THREAD_NAME) -> AddListener(this);
	}
	return true;
}

bool EMSlimButton::MouseUp(int32 p_vX, int32 p_vY)
{
	if(!m_vMousePressed)
		return false;

	m_vMousePressed = false;
	if(m_eMode == EM_SLIM_BUTTON_FOLLOW_MOUSE)
	{
		ToggleOn();
		return true;
	}
	else if(m_eMode == EM_SLIM_BUTTON_FOLLOW_MOUSE_REPEAT_COMMAND)
	{
		ToggleOn();
		EMThreadRepository::Instance() -> GetThread(GUI_SERVICE_THREAD_NAME) -> RemoveListener(this);
		return true;
	}
	
	return false;
}

void EMSlimButton::SetBitmaps(EMBitmap* p_opOnBitmap, EMBitmap* p_opOffBitmap, EMBitmap* p_opDisabledOnBitmap, EMBitmap* p_opDisabledOffBitmap)
{
	m_opDisabledOffBitmap = p_opDisabledOffBitmap;
	m_opDisabledOnBitmap = p_opDisabledOnBitmap;
	m_opOffBitmap = p_opOffBitmap;
	m_opOnBitmap = p_opOnBitmap;
}

void EMSlimButton::SetColors(EMColor p_oOnColor, EMColor p_oOffColor)
{
	m_oOnColor = p_oOnColor;
	m_oOffColor = p_oOffColor;
}

void EMSlimButton::SetCommand(uint32 p_vCommandID, void* p_opCommandParameterOne, void* p_opCommandParameterTwo, void* p_opCommandParameterThree)
{
	m_vCommandID = p_vCommandID;
	m_opCommandParameterOne = p_opCommandParameterOne;
	m_opCommandParameterTwo = p_opCommandParameterTwo;
	m_opCommandParameterThree = p_opCommandParameterThree;
}

void EMSlimButton::SetEnabled(bool p_vEnabled)
{
	m_vEnabled = p_vEnabled;
}

void EMSlimButton::SetFrame(EMRect p_oFrame)
{
	if(m_oFrame == p_oFrame)
		return;

	m_oFrame = p_oFrame;
	m_oInsideBorder = m_oFrame;
	
	if(m_vLeftBorder)
		m_oInsideBorder.m_vLeft += 1;
	if(m_vTopBorder)
		m_oInsideBorder.m_vTop += 1;
	if(m_vRightBorder)
		m_oInsideBorder.m_vRight -= 1;
	if(m_vBottomBorder)
		m_oInsideBorder.m_vBottom -= 1;

	CalculatePositions();

	if(m_opToolTip != NULL)
		m_opToolTip -> SetFrame(p_oFrame);
}

void EMSlimButton::SetMode(EMSlimButtonMode p_eMode)
{
	m_eMode = p_eMode;
}

bool EMSlimButton::SetOn(bool p_vOn)
{
//EMDebugger("ÖJKN");
;//cout_commented_out_4_release << "EMSlimButton::SetOn: p_vOn: " << p_vOn << ", m_vOn: " << m_vOn << endl;
	if(m_vOn == p_vOn)
		return false;
	m_vOn = p_vOn;
;//cout_commented_out_4_release << "EMSlimButton::SetOn: " << m_vOn << endl;
	return true;
}

void EMSlimButton::SetOnString(string* p_opString)
{
	m_opOnString = p_opString;
	CalculatePositions();
}

void EMSlimButton::SetOffString(string* p_opString)
{
	m_opOffString = p_opString;
	CalculatePositions();
}

void EMSlimButton::SetVisible(bool p_vVisible)
{
	m_vVisible = p_vVisible;
}

void EMSlimButton::ThreadRun(EMThread* p_opThread)
{
//;//cout_commented_out_4_release << "EMSlimButton::ThreadRun: " << find_thread(NULL) << ", " << m_vFlashCounter << endl;
	if(m_vFlash)
	{
			m_vFlashCounter--;
			if(m_vFlashCounter == 0)
			{
				m_vFlash = false;
				EMThreadRepository::Instance() -> GetThread(GUI_SERVICE_THREAD_NAME) -> RemoveListener(this);
				SetOn(false);
				if(m_opView -> LockWindow())
				{
					m_opView -> BeginPaint();
					Draw(Frame(), m_opView);
					m_opView -> EndPaint();
					m_opView -> UnlockWindow();
				}
			}
			return;			
	}
	if(m_eMode == EM_SLIM_BUTTON_FOLLOW_MOUSE_REPEAT_COMMAND)
		ExecuteCommand();
}

void EMSlimButton::ToggleOn()
{
	m_vOn = !m_vOn;
;//cout_commented_out_4_release << "EMSlimButton::ToggleOn: m_vOn: " << m_vOn << endl;
}


