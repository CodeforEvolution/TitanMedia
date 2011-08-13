#include "EMSlimValueBox.h"

#include "EMCommandRepository.h"
#include "EMFactory.h"
#include "EMMenuItem.h"
#include "EMPopUpMenu.h"
#include "EMSettingsRepository.h"
#include "EMToolTip.h"
#include "EMView.h"

#include <algorithm>
 
const int SLIM_VALUE_BOX_MOUSE_SENSITIVITY = 4;

#define SLIM_VALUE_BOX_BORDER_SIZE 1
#define SLIM_VALUE_BOX_LIMIT_MARK_SIZE 2
#define SLIM_VALUE_BOX_LIMIT_MARK_INSET 1

bool EMSlimValueBox::m_vValueBoxIsActive = false;

EMSlimValueBox::EMSlimValueBox(EMView* p_opView, EMRect p_oFrame, 
				int32 p_vMinValue, int32 p_vMaxValue, int32 p_vDefaultValue,
				uint32 p_vCommandID, 
				void* p_opCommandParameterOne, 
				void* p_opCommandParameterTwo,
				void* p_opCommandParameterThree,
				bool p_vLeftBorder,
				bool p_vTopBorder, bool p_vRightBorder, 
				bool p_vBottomBorder, uint32 p_vSettingID,
				bool p_vAllowDecimals,
				const char* p_vpToolTipLabel
				) :
m_eMode(VALUE_BOX_DRAW_NORMAL),
m_oFrame(p_oFrame),
m_oInsideBorder(p_oFrame),
m_opCommandParameterOne(p_opCommandParameterOne),
m_opCommandParameterTwo(p_opCommandParameterTwo),
m_opCommandParameterThree(p_opCommandParameterThree),
m_opPopUpMenu(NULL),
m_opToolTip(NULL),
m_opView(p_opView),
m_vAllowDecimals(p_vAllowDecimals),
m_vBottomBorder(true),//p_vBottomBorder),
m_vCommandID(p_vCommandID),
m_vCurrentValue(p_vDefaultValue),
m_vDefaultValue(p_vDefaultValue),
m_vEnabled(true),
m_vLeftBorder(true),//p_vLeftBorder),
m_vMaxValue(p_vMaxValue),
m_vMinValue(p_vMinValue),
m_vMouseIsDown(false),
m_vOwnsValues(false),
m_vRightBorder(true),//p_vRightBorder),
m_vSettingID(p_vSettingID),
m_vTopBorder(true),//p_vTopBorder),
m_vUseValueRange(true),
m_vVisible(true)
{
	sprintf(m_vpCurrentValue, "%ld", m_vDefaultValue);
	if(m_vLeftBorder)
		m_oInsideBorder.m_vLeft += 1;
	if(m_vTopBorder)
		m_oInsideBorder.m_vTop += 1;
	if(m_vRightBorder)
		m_oInsideBorder.m_vRight -= 1;
	if(m_vBottomBorder)
		m_oInsideBorder.m_vBottom -= 1;
	if(m_opView -> LockWindow())
	{
		CalculateValuePosition();
		m_opView -> UnlockWindow();
	}

	if(p_vpToolTipLabel != NULL)
		m_opToolTip = EMFactory::Instance() -> CreateToolTip(p_oFrame, p_vpToolTipLabel, p_opView -> GetNativeView(), true);
}

EMSlimValueBox::EMSlimValueBox(EMView* p_opView, EMRect p_oFrame, 
//				vector<string> p_oValues,
				uint32 p_vCommandID, 
				void* p_opCommandParameterOne, 
				void* p_opCommandParameterTwo,
				void* p_opCommandParameterThree,
				bool p_vLeftBorder,
				bool p_vTopBorder, bool p_vRightBorder, 
				bool p_vBottomBorder, uint32 p_vSettingID,
				bool p_vAllowDecimals,
				const char* p_vpToolTipLabel
				) :
m_eMode(VALUE_BOX_DRAW_NORMAL),
m_oFrame(p_oFrame),
m_oInsideBorder(p_oFrame),
m_opCommandParameterOne(p_opCommandParameterOne),
m_opCommandParameterTwo(p_opCommandParameterTwo),
m_opCommandParameterThree(p_opCommandParameterThree),
m_opToolTip(NULL),
m_opView(p_opView),
//m_oValues(p_oValues),
m_vAllowDecimals(p_vAllowDecimals),
m_vBottomBorder(p_vBottomBorder),
m_vCommandID(p_vCommandID),
m_vEnabled(true),
m_vLeftBorder(p_vLeftBorder),
m_vMouseIsDown(false),
m_vOwnsValues(false),
m_vRightBorder(p_vRightBorder),
m_vSettingID(p_vSettingID),
m_vTopBorder(p_vTopBorder),
m_vUseValueRange(false),
m_vVisible(true)
{
/*
	vector<string>::iterator oIterator = find(m_oValues.begin(), m_oValues.end(), m_oCurrentValue);
	if(oIterator == m_oValues.end())
		EMDebugger("The default value is not equal to any of the values in the vector");
*/		
	m_vMinValue = 0;
	if(m_oValues.size() == 0)
	{
		m_vMaxValue = 0;
		m_vCurrentValue = 0;
	}
	else
		m_vMaxValue = m_oValues.size() - 1;
	
/*
	// Find the position of the current value (is there a better way to do this?)
	for(m_vCurrentValue = 0; m_vCurrentValue < static_cast<int32>(m_oValues.size()); m_vCurrentValue++)
	{
		if(m_oCurrentValue == m_oValues[m_vCurrentValue])
			break;
	}
*/

//	if(p_oValues.size() == 0)
		m_oCurrentValue = string("<empty>");
//	else
//		m_oCurrentValue = p_oValues[0];

	sprintf(m_vpCurrentValue, "%s", m_oCurrentValue.c_str());

	if(m_vLeftBorder)
		m_oInsideBorder.m_vLeft += 1;
	if(m_vTopBorder)
		m_oInsideBorder.m_vTop += 1;
	if(m_vRightBorder)
		m_oInsideBorder.m_vRight -= 1;
	if(m_vBottomBorder)
		m_oInsideBorder.m_vBottom -= 1;

	if(m_opView -> LockWindow())
	{
		CalculateValuePosition();
		m_opView -> UnlockWindow();
	}

	if(p_vpToolTipLabel != NULL)
		m_opToolTip = EMFactory::Instance() -> CreateToolTip(p_oFrame, p_vpToolTipLabel, p_opView -> GetNativeView(), true);
}

EMSlimValueBox::~EMSlimValueBox()
{
	delete m_opToolTip;

	ClearValues();
}

void EMSlimValueBox::CalculateValuePosition()
{
	int32 vAscent;
	int32 vDescent;
	if(m_opView -> LockWindow())
	{
		m_opView -> SetFont(EM_PLAIN); // FIXME: I should not have to set the font here to get correct values
		m_opView -> CalculateStringHeight(m_vpCurrentValue, vAscent, vDescent);
		int32 vYSpacing = (m_oFrame.GetHeight() - (vAscent + vDescent)) / 2;
		m_vValueTop = m_oFrame.m_vTop + vYSpacing + vAscent + SLIM_VALUE_BOX_BORDER_SIZE;
		m_vValueLeft = m_oFrame.m_vLeft + ((m_oFrame.GetWidth() - m_opView -> CalculateStringWidth(m_vpCurrentValue)) / 2);
		m_opView -> UnlockWindow();
	}
}

void EMSlimValueBox::ClearValues()
{
	if(m_vOwnsValues)
		for(int32 vIndex = 0; vIndex < static_cast<int32>(m_oValues.size()); vIndex++)
			delete m_oValues[vIndex];
	m_oValues.clear();
}

bool EMSlimValueBox::Contains(int32 p_vX, int32 p_vY)
{
	return m_oFrame.Contains(p_vX, p_vY);
}

void EMSlimValueBox::Draw(EMRect p_oClippingRect)
{
	if(!m_vVisible)
		return;

	if(!m_oFrame.Intersects(p_oClippingRect))
		return;
	
	if(m_eMode == VALUE_BOX_DRAW_NORMAL)
	{
		m_opView -> SetHighColor(EMColor(245, 245, 255, 255));
		m_opView -> StrokeLine(m_oFrame.m_vRight, m_oFrame.m_vTop, m_oFrame.m_vRight, m_oFrame.m_vBottom);
		m_opView -> StrokeLine(m_oFrame.m_vLeft, m_oFrame.m_vBottom, m_oFrame.m_vRight, m_oFrame.m_vBottom);
		m_opView -> SetHighColor(EM_COLOR_VIEW_BACKGROUND_SHADOW);
		m_opView -> StrokeLine(m_oFrame.m_vLeft, m_oFrame.m_vTop, m_oFrame.m_vLeft, m_oFrame.m_vBottom);
		m_opView -> StrokeLine(m_oFrame.m_vLeft, m_oFrame.m_vTop, m_oFrame.m_vRight, m_oFrame.m_vTop);
/*
	m_opView -> SetHighColor(EM_COLOR_VALUE_BOX_BORDER);
	if(m_vLeftBorder)
		m_opView -> StrokeLine(m_oFrame.m_vLeft, m_oFrame.m_vTop, m_oFrame.m_vLeft, m_oFrame.m_vBottom);
	if(m_vTopBorder)
		m_opView -> StrokeLine(m_oFrame.m_vLeft, m_oFrame.m_vTop, m_oFrame.m_vRight, m_oFrame.m_vTop);
	if(m_vRightBorder)
		m_opView -> StrokeLine(m_oFrame.m_vRight, m_oFrame.m_vTop, m_oFrame.m_vRight, m_oFrame.m_vBottom);
	if(m_vBottomBorder)
		m_opView -> StrokeLine(m_oFrame.m_vLeft, m_oFrame.m_vBottom, m_oFrame.m_vRight, m_oFrame.m_vBottom);
*/
		if(!m_vEnabled)
		{
			m_opView -> SetHighColor(EM_COLOR_VALUE_BOX_BACKGROUND_DISABLED);
			m_opView -> SetLowColor(EM_COLOR_VALUE_BOX_BACKGROUND_DISABLED);
		}
		else
		{
			m_opView -> SetHighColor(EM_COLOR_VALUE_BOX_BACKGROUND);
			m_opView -> SetLowColor(EM_COLOR_VALUE_BOX_BACKGROUND);
		}
		m_opView -> FillRect(m_oInsideBorder);
	
		if(m_vCurrentValue == m_vMaxValue)
		{
			m_opView -> SetHighColor(EM_COLOR_VALUE_BOX_MAX_LIMIT_MARK);
			m_opView -> FillRect(EMRect(m_oInsideBorder.m_vLeft + SLIM_VALUE_BOX_LIMIT_MARK_INSET, 
							m_oInsideBorder.m_vTop + SLIM_VALUE_BOX_LIMIT_MARK_INSET, 
							m_oInsideBorder.m_vLeft + SLIM_VALUE_BOX_LIMIT_MARK_INSET + SLIM_VALUE_BOX_LIMIT_MARK_SIZE,
							m_oInsideBorder.m_vTop + SLIM_VALUE_BOX_LIMIT_MARK_INSET + SLIM_VALUE_BOX_LIMIT_MARK_SIZE));
		}
		if(m_vCurrentValue == m_vMinValue)
		{
			m_opView -> SetHighColor(EM_COLOR_VALUE_BOX_MIN_LIMIT_MARK);
			m_opView -> FillRect(EMRect(m_oInsideBorder.m_vLeft + SLIM_VALUE_BOX_LIMIT_MARK_INSET, 
							m_oInsideBorder.m_vBottom - SLIM_VALUE_BOX_LIMIT_MARK_INSET - SLIM_VALUE_BOX_LIMIT_MARK_SIZE, 
							m_oInsideBorder.m_vLeft + SLIM_VALUE_BOX_LIMIT_MARK_INSET + SLIM_VALUE_BOX_LIMIT_MARK_SIZE,
							m_oInsideBorder.m_vBottom - SLIM_VALUE_BOX_LIMIT_MARK_INSET));
		}

		if(!m_vEnabled)
			m_opView -> SetHighColor(EM_COLOR_VALUE_BOX_TEXT_DISABLED);
		else
			m_opView -> SetHighColor(EM_COLOR_VALUE_BOX_TEXT);
		m_opView -> ConstrainClippingRect(m_oInsideBorder);	
		m_opView -> DrawString(m_vValueLeft, m_vValueTop, m_vpCurrentValue);
		m_opView -> ConstrainClippingRect(EMRect());	
	
		if(m_vMouseIsDown)
		{
			m_opView -> SetHighColor(EM_COLOR_VALUE_CHANGING);
			m_opView -> StrokeRect(m_oInsideBorder);
		}
	}
	
	else if(m_eMode == VALUE_BOX_DRAW_BLANK)
	{
		m_opView -> SetHighColor(EM_COLOR_VIEW_BACKGROUND);
		m_opView -> FillRect(m_oFrame);
	}
}

void EMSlimValueBox::ExecuteCommand()
{
	if(m_vCommandID == COMMAND_ID_NULL)
		return;
	if(m_opCommandParameterOne == NULL)
		EMCommandRepository::Instance() -> ExecuteCommand(m_vCommandID);
	else
	{
		// If there is no second parameter, it sends its value as the second parameter
		if(m_opCommandParameterTwo == NULL)
		{
			if(m_vUseValueRange)
				EMCommandRepository::Instance() -> ExecuteCommand(m_vCommandID, m_opCommandParameterOne, &m_vCurrentValue);
			else
				EMCommandRepository::Instance() -> ExecuteCommand(m_vCommandID, m_opCommandParameterOne, &m_oCurrentValue);
		}
		// If there is no third parameter, it sends its value as the third parameter
		else if(m_opCommandParameterThree == NULL)
		{
			if(m_vUseValueRange)
				EMCommandRepository::Instance() -> ExecuteCommand(m_vCommandID, m_opCommandParameterOne, m_opCommandParameterTwo, &m_vCurrentValue);
			else
				EMCommandRepository::Instance() -> ExecuteCommand(m_vCommandID, m_opCommandParameterOne, m_opCommandParameterTwo, &m_oCurrentValue);
		}
		else
			EMCommandRepository::Instance() -> ExecuteCommand(m_vCommandID, m_opCommandParameterOne, m_opCommandParameterTwo, m_opCommandParameterThree);
	}
}

EMRect EMSlimValueBox::Frame()
{
	return m_oFrame;
}

bool EMSlimValueBox::GetEnabled()
{
	return m_vEnabled;
}

void* EMSlimValueBox::GetValue()
{
	if(m_vUseValueRange)
		return &m_vCurrentValue;
	return &m_oCurrentValue;
}

int EMSlimValueBox::GetValueIndex()
{
	return m_vCurrentValue;
}

bool EMSlimValueBox::Intersects(EMRect p_oRect)
{
	return m_oFrame.Intersects(p_oRect);
}

bool EMSlimValueBox::MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage)
{
	if(dynamic_cast<EMPopUpMenu*>(p_opSender) != m_opPopUpMenu)
		return false;
	delete m_opPopUpMenu;
	SetValue(p_vMessage - 1);
	if(m_vSettingID != 0)
	{
		if(m_vUseValueRange)
		{
			float vFloat;
			int32 vInt32;
			if(m_vAllowDecimals)
			{
				vFloat = m_vCurrentValue;
				EMSettingsRepository::Instance() -> SetSetting(m_vSettingID, EM_SETTING_FLOAT, &vFloat);
			}
			else
			{
				vInt32 = m_vCurrentValue;
				EMSettingsRepository::Instance() -> SetSetting(m_vSettingID, EM_SETTING_UINT32, &vInt32);
			}
		}
		else
			EMSettingsRepository::Instance() -> SetSetting(m_vSettingID, EM_SETTING_STRING, m_oValues[m_vCurrentValue]);
	}
	ExecuteCommand();

	if(m_opView -> LockWindow())
	{
		m_opView -> Invalidate(m_oInsideBorder);
		m_opView -> UnlockWindow();
	}
	return true;
}

// Returns true if it should be invalidated
bool EMSlimValueBox::MouseDown(int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree)
{
	if(!m_oFrame.Contains(p_vX, p_vY))
		return false;
		
	if(m_eMode == VALUE_BOX_DRAW_BLANK || !m_vVisible)
		return false;
		
	if(p_vButtonOne)
	{
		m_vValueBoxIsActive = true;
		m_vMouseIsDown = true;
		m_vMouseDownY = p_vY;
		m_vMouseDownValue = m_vCurrentValue;
		return true;
	}
	if(!m_vUseValueRange)
	{
		m_opPopUpMenu = EMFactory::Instance() -> CreatePopUpMenu();
		m_opPopUpMenu -> AddListener(this);
//#ifdef PLATFORM_WINDOWS
//		EMWinWindow::SetPopUpMenu(dynamic_cast<EMWinPopUpMenu*>(m_opPopUpMenu));
//#endif
		EMMenuItem* opItem;
		string* opString;

		if(m_oValues.size() > 0)
		{
			for(int vIndex = m_vMaxValue; vIndex >= m_vMinValue; vIndex--)
			{
//				opItem = EMFactory::Instance() -> CreateMenuItem(EM_new string(m_oValues[vIndex] -> c_str()), vIndex + 1);//&(m_oValues[vIndex].c_str()), 0); // FIXME: Leaks memory
				opString = m_oValues[vIndex];
				opItem = EMFactory::Instance() -> CreateMenuItem(opString, vIndex + 1);
				m_opPopUpMenu -> AddItem(opItem);
				if(vIndex == m_vCurrentValue)
					opItem -> SetMarked(true);
			}
		}
		int32 vNewX = p_vX;
		int32 vNewY = p_vY;
		m_opView -> ConvertToScreenCoordinates(vNewX, vNewY);
		m_opPopUpMenu -> PopUp(vNewX, vNewY, m_opView);
	}
	else
	{
		SetValue(m_vDefaultValue);

		float vFloat;
		int32 vInt32;
		if(m_vAllowDecimals)
		{
			vFloat = m_vCurrentValue;
			EMSettingsRepository::Instance() -> SetSetting(m_vSettingID, EM_SETTING_FLOAT, &vFloat);
		}
		else
		{
			vInt32 = m_vCurrentValue;
			EMSettingsRepository::Instance() -> SetSetting(m_vSettingID, EM_SETTING_UINT32, &vInt32);
		}

		ExecuteCommand();
		return true;
	}
	return false;
}

// Returns true if it should be invalidated
bool EMSlimValueBox::MouseMoved(int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree, bool p_vTriggerCommand)
{
	if(!m_vMouseIsDown)
		return false;
;//cout_commented_out_4_release << "EMSlimValueBox::MouseMoved: m_vMaxValue: " << m_vMaxValue << ", m_vMinValue: " << m_vMinValue << endl;
		
	if(m_vMaxValue == m_vMinValue) // Only one value exists, no use in trying to change it
		return false;

	int32 vMoveOffsetY = p_vY - m_vMouseDownY;
	
	int32 vNewValue = m_vMouseDownValue - static_cast<int32>(vMoveOffsetY / SLIM_VALUE_BOX_MOUSE_SENSITIVITY);
	if(vNewValue > m_vMaxValue)
		vNewValue = m_vMaxValue;
	else if(vNewValue < m_vMinValue)
		vNewValue = m_vMinValue;
;//cout_commented_out_4_release << "m_vMinValue: " << m_vMinValue << ", m_vMaxValue: " << m_vMaxValue << ", vNewValue: " << vNewValue << ", m_vCurrentValue: " << m_vCurrentValue << endl;
	if(vNewValue == m_vCurrentValue)
		return false; // Nothing has changed
	
	SetValue(vNewValue);

	if(p_vTriggerCommand)
		ExecuteCommand();

	return true;
/*
	m_vCurrentValue = vNewValue;

	if(m_vUseValueRange)
		sprintf(m_vpCurrentValue, "%ld", m_vCurrentValue);
	else
		sprintf(m_vpCurrentValue, "%s", (m_oValues[m_vCurrentValue]).c_str());

	CalculateValuePosition();
	return true;
*/
}

// Returns true if it should be invalidated
bool EMSlimValueBox::MouseUp(int32 p_vX, int32 p_vY)
{
	if(m_vMouseIsDown)
	{
		if(m_vMouseDownValue != m_vCurrentValue && m_vSettingID != 0)
		{
			if(m_vUseValueRange)
			{
				float vFloat;
				int32 vInt32;
				if(m_vAllowDecimals)
				{
					vFloat = m_vCurrentValue;
					EMSettingsRepository::Instance() -> SetSetting(m_vSettingID, EM_SETTING_FLOAT, &vFloat);
				}
				else
				{
					vInt32 = m_vCurrentValue;
					EMSettingsRepository::Instance() -> SetSetting(m_vSettingID, EM_SETTING_UINT32, &vInt32);
				}
			}
			else
			{
				string oValue(*(m_oValues[m_vCurrentValue]));
				EMSettingsRepository::Instance() -> SetSetting(m_vSettingID, EM_SETTING_STRING, &oValue);
			}
		}
		
		m_vMouseIsDown = false;
		m_vValueBoxIsActive = false;
		ExecuteCommand();
		// Needs to be redrawn when the mouse is released since the box
		// marking that the value is changing needs to disappear
		return true; 
	}
	return false;
}

void EMSlimValueBox::SetCommand(uint32 p_vCommandID, void* p_opCommandParameterOne, void* p_opCommandParameterTwo, void* p_opCommandParameterThree)
{
	m_vCommandID = p_vCommandID;
	m_opCommandParameterOne = p_opCommandParameterOne;
	m_opCommandParameterTwo = p_opCommandParameterTwo;
	m_opCommandParameterThree = p_opCommandParameterThree;
}

void EMSlimValueBox::SetEnabled(bool p_vEnabled)
{
	m_vEnabled = p_vEnabled;
}

void EMSlimValueBox::SetFrame(EMRect p_oFrame)
{
	if(m_oFrame == p_oFrame)
		return;
		
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

	CalculateValuePosition();

	if(m_opToolTip != NULL)
		m_opToolTip -> SetFrame(p_oFrame);
}

void EMSlimValueBox::SetMode(ValueBoxMode p_eMode)
{
	m_eMode = p_eMode;
}

void EMSlimValueBox::SetValue(int32 p_vValue)
{
	if(p_vValue < m_vMinValue || p_vValue > m_vMaxValue)
		EMDebugger("Value outside value range");

	m_vCurrentValue = p_vValue;

//	if(!m_vUseValueRange)
//		EMDebugger("The value box does not have numbers in its rage (it has strings)");
	if(m_vUseValueRange)
		sprintf(m_vpCurrentValue, "%ld", m_vCurrentValue);
	else
		sprintf(m_vpCurrentValue, "%s", (m_oValues[m_vCurrentValue]) -> c_str());
	m_oCurrentValue = m_vpCurrentValue;

	CalculateValuePosition();
}

void EMSlimValueBox::SetValue(string p_oValue)
{
	if(m_vUseValueRange)
		EMDebugger("Can not set a string as value when having a range of numbers");

	for(int32 vIndex = 0; vIndex < static_cast<int32>(m_oValues.size()); vIndex++)
		if(*(m_oValues[vIndex]) == p_oValue)
		{
			m_vCurrentValue = vIndex;
			sprintf(m_vpCurrentValue, "%s", m_oValues[m_vCurrentValue] -> c_str());
			m_oCurrentValue = m_vpCurrentValue;
			CalculateValuePosition();
			return;
		}
// Let's just pretend that nothing happened if trying to set an unknown value		
//	EMDebugger("The string does not exist in the value box");	
}

void EMSlimValueBox::SetValues(list<string*>* p_opValues, bool p_vTransferOwnership)
{
	ClearValues();

	m_vOwnsValues = p_vTransferOwnership;
	
	list<string*>::iterator oIterator = p_opValues -> begin();
	while(oIterator != p_opValues -> end())
	{
		m_oValues.push_back(*oIterator);
		oIterator++;
	}
	
	m_vCurrentValue = 0;
	m_vMaxValue = m_oValues.size() - 1;
	if(m_vMaxValue == -1)
		m_vMaxValue = 0;

	if(m_oValues.size() == 0)
		m_oCurrentValue = string("<empty>");
	else
		m_oCurrentValue = *(m_oValues[0]);

	// FIXME: Limit the nr of chars copied to MAX_STRING_LENGTH, or memory will be overwritten
	sprintf(m_vpCurrentValue, "%s", m_oCurrentValue.c_str());

	CalculateValuePosition();
}
/*
void EMSlimValueBox::SetValues(vector<string> p_oValues)
{
	m_oValues.clear();
	
//	if(p_oValues.size() == 0)
//		return; //EMDebugger("A EMSlimValueBox does not like empty vectors");

	for(int32 vIndex = 0; vIndex < static_cast<int32>(p_oValues.size()); vIndex++)
		m_oValues.push_back(EM_new string(p_oValues[vIndex]));

	m_vCurrentValue = 0;
	m_vMaxValue = m_oValues.size() - 1;
	if(m_vMaxValue == -1)
		m_vMaxValue = 0;

	if(m_oValues.size() == 0)
		m_oCurrentValue = string("<empty>");
	else
		m_oCurrentValue = *(m_oValues[0]);

	// FIXME: Limit the nr of chars copied to MAX_STRING_LENGTH, or memory will be overwritten
	sprintf(m_vpCurrentValue, "%s", m_oCurrentValue.c_str());

	CalculateValuePosition();
}
*/
void EMSlimValueBox::SetVisible(bool p_vVisible)
{
	m_vVisible = p_vVisible;
}

// Returns true if any value box is currently being changed
bool EMSlimValueBox::ValueBoxIsActive()
{
	return m_vValueBoxIsActive;
}
