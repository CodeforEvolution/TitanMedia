/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __NEW_PROJECT_DIALOG
#define __NEW_PROJECT_DIALOG

#include "EMDefaultSettingsContainer.h"
#include "EMListener.h"
#include "EMSlimLabel.h"
#include "EMSlimValueBox.h"

class EMButton;
class EMListenerRepository;
class EMTextControl;
class EMView;
class EMWindow;

class NewProjectDialog : public EMListener, public EMDefaultSettingsContainer
{
public:
	NewProjectDialog();
	~NewProjectDialog();
	void DisplayDialog();
	void Draw(EMRect p_oClippingRect);
	void Init();
	bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage);
	void MouseDown(float p_vX, float p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree);
	void MouseMoved(float p_vX, float p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree);
	void MouseUp(float p_vX, float p_vY);
	void SetDefaultSettings();

private:
	bool AllValuesOk();

private:
	EMView* m_opView;
	EMSlimLabel m_oLabelAudioSampleRate;
	EMSlimLabel m_oLabelFps;
	EMSlimLabel m_oLabelHeight;
	EMSlimLabel m_oLabelWidth;
	EMSlimLabel m_oLabelVideoFormat;
	EMButton* m_opCancelButton;
	EMButton* m_opOkButton;
	EMTextControl* m_opTextControlFps;
	EMTextControl* m_opTextControlHeight;
	EMTextControl* m_opTextControlWidth;
	EMWindow* m_opWindow;
	EMSlimValueBox m_oValueBoxSampleRate;
	EMSlimValueBox m_oValueBoxVideoFormat;
	float m_vFps;
	int m_vHeight;
	bool m_vIsInitialized;
	int m_vSampleRate;
	uint32 m_vVideoModeMessage;
	int m_vWidth;
};

#endif