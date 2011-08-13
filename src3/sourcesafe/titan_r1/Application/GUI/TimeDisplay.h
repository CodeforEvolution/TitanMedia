/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __TIME_DISPLAY
#define __TIME_DISPLAY

#include "EMGUIGlobals.h"
#include "EMListener.h"
#include "EMRect.h"
#include "EMThreadListener.h"

class EMToolTip;
class EMView;

enum TimeDisplayDrawingMode
{
	TIME_DISPLAY_DRAW_NORMAL,
	TIME_DISPLAY_DRAW_BLANK
};

class TimeDisplay : public EMListener, public EMThreadListener
{
public:
	TimeDisplay(EMView* p_opView, EMRect p_oFrame, EMTimeDisplayMode p_eMode);
	virtual ~TimeDisplay();
	void CalculatePositions();
	void Draw(EMRect p_oClippingRect);
	void DrawContent(EMRect p_oClippingRect);
	EMRect Frame();
	void Init();
	bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage);
	void SetFrame(EMRect p_oFrame);
	void SetMode(TimeDisplayDrawingMode p_eMode);
	void ThreadRun(EMThread* p_opThread);

private:
	TimeDisplayDrawingMode m_eDrawingMode;
	EMTimeDisplayMode m_eTimeDisplayMode;
	EMRect m_oFrame;
	EMRect m_oInsideBorder;
	string* m_opSignature; // Cached setting
	EMToolTip* m_opToolTip;
	EMView* m_opView;
	float m_vFramerate; // Cached setting
	int64 m_vPosition;
	int m_vSignatureMajor; // Cached setting
	int64 m_vSignatureMajorSpacing;
	int m_vSignatureMinor; // Cached setting
	int64 m_vSignatureMinorSpacing;
	float m_vTempo; // Cached setting
	char m_vpTime[20];
	int32 m_vTimeTop;
	int32 m_vTimeLeft;
};

#endif