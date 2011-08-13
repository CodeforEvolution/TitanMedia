/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __RENDER_FORMAT_DIALOG
#define __RENDER_FORMAT_DIALOG

#include "EMListener.h"
#include "EMSlimLabel.h"
#include "EMSlimValueBox.h"

#include <list>
#include <string>

class EMButton;
class EMListenerRepository;
class EMTextControl;
class EMView;
class EMWindow;

class RenderFormatDialog : public EMListener
{
public:
	RenderFormatDialog();
	~RenderFormatDialog();
	void DisplayDialog();
	void Draw(EMRect p_oClippingRect);
	void Init();
	bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage);
	void MouseDown(float p_vX, float p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree);
	void MouseMoved(float p_vX, float p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree);
	void MouseUp(float p_vX, float p_vY);

private:
	bool AllValuesOk();
	void InitValues();
	void SetValueBoxes();

private:
	EMView* m_opView;
//	list<string*> m_oEncoderFamilyValues;
//	list<string*> m_oEncoderValues;
	EMSlimLabel m_oLabelAudioEncoder;
	EMSlimLabel m_oLabelEncoderFamily;
	EMSlimLabel m_oLabelFps;
	EMSlimLabel m_oLabelHeight;
	EMSlimLabel m_oLabelQuality;
	EMSlimLabel m_oLabelVideoEncoder;
	EMSlimLabel m_oLabelWidth;
	EMButton* m_opCancelButton;
	EMButton* m_opOkButton;
	EMTextControl* m_opTextControlFps;
	EMTextControl* m_opTextControlHeight;
	EMTextControl* m_opTextControlWidth;
	EMWindow* m_opWindow;
	EMSlimValueBox m_oValueBoxAudioEncoder;
	EMSlimValueBox m_oValueBoxEncoderFamily;
	EMSlimValueBox m_oValueBoxQuality;
	EMSlimValueBox m_oValueBoxVideoEncoder;
	uint32 m_vEncoderFamilyMessage;
	float m_vFps;
	int m_vHeight;
	bool m_vIsInitialized;
	int m_vWidth;
};

#endif