/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* Contains some native code
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_SLIM_VALUE_BOX
#define __EM_SLIM_VALUE_BOX

#include "EMGUIGlobals.h"
#include "EMListener.h"
#include "EMRect.h"

#include <vector>
#include <string>

class EMListenerRepository;
class EMPopUpMenu;
class EMToolTip;
class EMView;

const int MAX_STRING_LENGTH = 255;

enum ValueBoxMode
{
	VALUE_BOX_DRAW_NORMAL,
	VALUE_BOX_DRAW_BLANK
};

class EMSlimValueBox : public EMListener
{
public:
	EMSlimValueBox(EMView* p_opView, EMRect p_oFrame, 
				int32 p_vMinValue, int32 p_vMaxValue, int32 p_vDefaultValue,
				uint32 p_vCommandID = 0, 
				void* p_opCommandParameterOne = NULL, 
				void* p_opCommandParameterTwo = NULL,
				void* p_opCommandParameterThree = NULL,
				bool p_vLeftBorder = true,
				bool p_vTopBorder = true, bool p_vRightBorder = true, 
				bool p_vBottomBorder = true, uint32 p_vSettingID = 0,
				bool p_vAllowDecimals = false,
				const char* p_vpToolTipLabel = NULL
				);
	EMSlimValueBox(EMView* p_opView, EMRect p_oFrame, 
//				vector<string> p_oValues,
				uint32 p_vCommandID = 0, 
				void* p_opCommandParameterOne = NULL, 
				void* p_opCommandParameterTwo = NULL,
				void* p_opCommandParameterThree = NULL,
				bool p_vLeftBorder = true,
				bool p_vTopBorder = true, bool p_vRightBorder = true, 
				bool p_vBottomBorder = true, uint32 p_vSettingID = 0,
				bool p_vAllowDecimals = false,
				const char* p_vpToolTipLabel = NULL
				);
	~EMSlimValueBox();
	bool Contains(int32 p_vX, int32 p_vY);
	void Draw(EMRect p_oClippingRect);
	EMRect Frame();
	bool GetEnabled();
	void* GetValue();
	int GetValueIndex();
	bool Intersects(EMRect p_oRect);
	bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage);
	bool MouseDown(int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree);
	bool MouseMoved(int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree, bool p_vTriggerCommand = false);
	bool MouseUp(int32 p_vX, int32 p_vY);
	void SetCommand(uint32 p_vCommandID, void* p_opCommandParameterOne = NULL, void* p_opCommandParameterTwo = NULL, void* p_opCommandParameterThree = NULL);
	void SetEnabled(bool p_vEnabled);
	void SetFrame(EMRect p_oFrame);
	void SetMode(ValueBoxMode p_eMode);
	void SetValue(int32 p_vValue);
	void SetValue(string p_oValue);
	//void SetValues(vector<string> p_oValues);
	void SetValues(list<string*>* p_opValues, bool p_vTransferOwnership);
	void SetVisible(bool p_vVisible);
	static bool ValueBoxIsActive();

private:
	void CalculateValuePosition();
	void ClearValues();
	void ExecuteCommand();

private:
	ValueBoxMode m_eMode;
	EMRect m_oFrame;
	string m_oCurrentValue;
	EMRect m_oInsideBorder;
	void* m_opCommandParameterOne;
	void* m_opCommandParameterTwo;
	void* m_opCommandParameterThree;
	EMPopUpMenu* m_opPopUpMenu;
	EMToolTip* m_opToolTip;
	EMView* m_opView;
	vector<string*> m_oValues;
	bool m_vAllowDecimals;
	bool m_vBottomBorder;
	uint32 m_vCommandID;
	int32 m_vCurrentValue;
	int32 m_vDefaultValue;
	bool m_vEnabled;
	bool m_vLeftBorder;
	int32 m_vMaxValue;
	int32 m_vMinValue;
	int32 m_vMouseDownValue;
	int32 m_vMouseDownY;
	bool m_vMouseIsDown;
	bool m_vOwnsValues;
	char m_vpCurrentValue[MAX_STRING_LENGTH];
	bool m_vRightBorder;
	uint32 m_vSettingID;
	bool m_vTopBorder;
	bool m_vUseValueRange;
	static bool m_vValueBoxIsActive;
	int32 m_vValueLeft;
	int32 m_vValueTop;
	bool m_vVisible;
};

#endif
