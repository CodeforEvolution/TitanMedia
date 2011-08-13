/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_SLIM_LABEL
#define __EM_SLIM_LABEL

#include "EMGUIGlobals.h"
#include "EMRect.h"

#include <string>

class EMView;

enum EMSlimLabelMode
{
	EM_SLIM_LABEL_DRAW_BLANK,
	EM_SLIM_LABEL_DRAW_NORMAL
};

class EMSlimLabel
{
public:
	EMSlimLabel(EMView* p_opView, EMRect p_oFrame, string* p_opLabel, EMColor p_oBackgroundColor = EM_COLOR_VIEW_BACKGROUND, bool p_vCenterText = false);
	void CalculatePositions();
	void Draw(EMRect p_oClippingRect);
	EMRect Frame();
	void SetFrame(EMRect p_oFrame);
	void SetLabel(string* p_opLabel);
	void SetMode(EMSlimLabelMode p_eMode);

private:
	EMSlimLabelMode m_eMode;
	EMColor m_oBackgroundColor;
	EMRect m_oFrame;
	string* m_opLabel;
	EMView* m_opView;
	bool m_vCenterText;
	int32 m_vLabelLeft;
	int32 m_vLabelTop;
};

#endif