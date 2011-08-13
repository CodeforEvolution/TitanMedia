/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_SLIM_EDIT_LABEL
#define __EM_SLIM_EDIT_LABEL

#include "EMGUIGlobals.h"
#include "EMListener.h"
#include "EMListenerRepository.h"
#include "EMRect.h"

#include <string>

class EMListenerRepository;
class EMTextControl;
class EMTextView;
class EMView;

class EMSlimEditLabel : public EMListener, public EMListenerRepository
{
public:
	EMSlimEditLabel(EMRect p_oFrame, EMColor p_oBackgroundColor, EMView* p_opView,
				bool p_vLeftBorder = true,
				bool p_vTopBorder = true, bool p_vRightBorder = true, 
				bool p_vBottomBorder = true, int32 p_vLabelInset = 0);
	void CalculatePositions();
	static void CleanUp();
	bool Contains(int32 p_vX, int32 p_vY);
	bool Intersects(EMRect p_oRect);
	void Draw(EMRect p_oClippingRect);
	EMRect Frame();
	string GetLabel();
	static EMTextControl* GetTextControl(EMRect p_oFrame);
	bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage);
	bool MouseDown(int32 p_vX, int32 p_vY);
	bool MouseDoubleClick(int32 p_vX, int32 p_vY);
	void SetBackgroundColor(EMColor p_oColor);
	void SetFrame(EMRect p_oFrame);
	void SetLabel(const char* p_oLabel);

private:
	EMColor m_oBackgroundColor;
	EMRect m_oFrame;
	EMRect m_oInsideBorder;
	string m_oLabel;
	static EMTextControl* m_opTextControl;
//	static EMTextView* m_opTextView;
	EMView* m_opView;
	bool m_vBottomBorder;
	bool m_vIsEditing;
	int32 m_vLabelInset;
	bool m_vLeftBorder;
	int32 m_vOnStringLeft;
	int32 m_vOnStringTop;
	bool m_vRightBorder;
	bool m_vTopBorder;
};

#endif