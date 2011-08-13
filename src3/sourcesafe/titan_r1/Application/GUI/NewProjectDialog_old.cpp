#include "NewProjectDialog.h"

#include "CommandIDs.h"
#include "EMButton.h"
#include "EMFactory.h"
#include "EMGUIGlobals.h"
#include "EMListenerRepository.h"
#include "EMMessages.h"
#include "EMResourceRepository.h"
#include "EMSettingIDs.h"
#include "EMSettingsRepository.h"
#include "EMTextControl.h"
#include "EMView.h"
#include "EMWindow.h"
#include "ResourceIDs.h"

#include <string>

//const float COMPONENT_SPACING = 5;
const uint32 MESSAGE_BUTTON_OK = 1;
const uint32 MESSAGE_BUTTON_CANCEL = 2;
const uint32 MESSAGE_VIDEO_FORMAT_CHANGED = 3;

NewProjectDialog::NewProjectDialog() :
m_opView(EMFactory::Instance() -> CreateView(EMRect(0, 0, 345, 105), static_cast<EMResizingFlags>(EM_FOLLOW_LEFT_RIGHT | EM_FOLLOW_TOP_BOTTOM), false)),
m_oLabelAudioSampleRate(m_opView, EMRect(5, 20 * 2 + 5 * 3, 55, 20 * 3 + 5 * 3), NULL, EM_COLOR_VIEW_BACKGROUND, false),
m_oLabelFps(m_opView, EMRect(260, 20 + 5 * 2, 290, 20 * 2 + 5 * 2), NULL, EM_COLOR_VIEW_BACKGROUND, false),
m_oLabelHeight(m_opView, EMRect(180, 20 + 5 * 2, 210, 20 * 2 + 5 * 2), NULL, EM_COLOR_VIEW_BACKGROUND, false),
m_oLabelWidth(m_opView, EMRect(100, 20 + 5 * 2, 130, 20 * 2 + 5 * 2), NULL, EM_COLOR_VIEW_BACKGROUND, false),
m_oLabelVideoFormat(m_opView, EMRect(5, 5, 55, 20 + 5), NULL, EM_COLOR_VIEW_BACKGROUND, false),
m_opCancelButton(EMFactory::Instance() -> CreateButton(EMRect(300, 20 * 3 + 5 * 4, 340, 20 * 4 + 5 * 4), "Cancel", MESSAGE_BUTTON_CANCEL, 0, 0)),
m_opOkButton(EMFactory::Instance() -> CreateButton(EMRect(255, 20 * 3 + 5 * 4, 295, 20 * 4 + 5 * 4), "OK", MESSAGE_BUTTON_OK, 0, 0)),
m_opTextControlFps(EMFactory::Instance() -> CreateTextControl(EMRect(295, 20 + 5 * 2, 335, 20 * 2 + 5 * 2), "15", 0, false)),
m_opTextControlHeight(EMFactory::Instance() -> CreateTextControl(EMRect(215, 20 + 5 * 2, 255, 20 * 2 + 5 * 2), "240", 0, false)),
m_opTextControlWidth(EMFactory::Instance() -> CreateTextControl(EMRect(135, 20 + 5 * 2, 175, 20 * 2 + 5 * 2), "320", 0, false)),
m_opWindow(EMFactory::Instance() -> CreateEMWindow(EMRect(100, 100, 445, 205), "New Project Dialog", EM_WINDOW_UNRESIZABLE)),
m_oValueBoxSampleRate(m_opView, EMRect(100, 20 * 2 + 5 * 3, 340, 20 * 3 + 5 * 3), vector<string>(), 0, NULL, NULL, NULL, true, true, true, true, 0, false),
m_oValueBoxVideoFormat(m_opView, EMRect(100, 5, 340, 20 + 5), vector<string>(), 0, NULL, NULL, NULL, true, true, true, true, 0, false),
m_vIsInitialized(false),
m_vVideoModeMessage(MESSAGE_VIDEO_FORMAT_CHANGED)
{
	m_opView -> AddChild(m_opCancelButton);
	m_opView -> AddChild(m_opOkButton);
	m_opView -> AddChild(m_opTextControlFps);
	m_opView -> AddChild(m_opTextControlHeight);
	m_opView -> AddChild(m_opTextControlWidth);
	
	m_opWindow -> Show(); // Yuck, why do I have to do this
	m_opWindow -> Hide();
}

NewProjectDialog::~NewProjectDialog()
{
	delete m_opCancelButton;
	delete m_opOkButton;
	delete m_opView;
	delete m_opTextControlFps;
	delete m_opTextControlHeight;
	delete m_opTextControlWidth;
}

bool NewProjectDialog::AllValuesOk()
{
	string oFpsString(m_opTextControlFps -> Text());
	string oWidthString(m_opTextControlWidth -> Text());
	string oHeightString(m_opTextControlHeight -> Text());
	string oSampleRateString(*(static_cast<string*>(m_oValueBoxSampleRate.GetValue())));
	bool vOk(true);
	if(sscanf(oFpsString.c_str(), "%f", &m_vFps) != 1)
	{
		m_opTextControlFps -> SetText("25");
		vOk = false;
	}
	else if(m_vFps < 1)
	{
		m_opTextControlFps -> SetText("1");
		vOk = false;
	}
	else if(m_vFps > 100)
	{
		m_opTextControlFps -> SetText("100");
		vOk = false;
	}
	if(sscanf(oWidthString.c_str(), "%d", &m_vWidth) != 1)
	{
		m_opTextControlWidth -> SetText("320");
		vOk = false;
	}
	else if(m_vWidth < 1)
	{
		m_opTextControlWidth -> SetText("1");
		vOk = false;
	}
	else if(m_vWidth > 10000)
	{
		m_opTextControlWidth -> SetText("1000");
		vOk = false;
	}
	if(sscanf(oHeightString.c_str(), "%d", &m_vHeight) != 1)
	{
		m_opTextControlHeight -> SetText("200");
		vOk = false;
	}
	else if(m_vHeight < 1)
	{
		m_opTextControlHeight -> SetText("1");
		vOk = false;
	}
	else if(m_vHeight > 10000)
	{
		m_opTextControlHeight -> SetText("1000");
		vOk = false;
	}
	sscanf(oSampleRateString.c_str(), "%d", &m_vSampleRate);
//;//cout_commented_out_4_release << "oFpsString: " << oFpsString << ", oWidthString: " << oWidthString << ", oHeightString: " << oHeightString << ", oSampleRateString: " << oSampleRateString << endl;
	return vOk;
}
void NewProjectDialog::DisplayDialog()
{
	if(!m_vIsInitialized)
	{
		m_oValueBoxVideoFormat.SetCommand(COMMAND_MESSAGE_SENDER, &m_vVideoModeMessage, this, NULL);
		m_vIsInitialized = true;
	}
//	if(m_opWindow -> Lock())
	{
		if(m_opWindow -> IsHidden())
			m_opWindow -> Show();
//		m_opWindow -> Unlock();
	}
}

void NewProjectDialog::Draw(EMRect p_oClippingRect)
{
	m_opView -> SetHighColor(EM_COLOR_VIEW_BACKGROUND);
	m_opView -> FillRect(m_opView -> Bounds());
	m_oLabelAudioSampleRate.Draw(p_oClippingRect);
	m_oLabelFps.Draw(p_oClippingRect);
	m_oLabelHeight.Draw(p_oClippingRect);
	m_oLabelWidth.Draw(p_oClippingRect);
	m_oLabelVideoFormat.Draw(p_oClippingRect);
	m_oValueBoxSampleRate.Draw(p_oClippingRect);
	m_oValueBoxVideoFormat.Draw(p_oClippingRect);
}

void NewProjectDialog::Init()
{
	m_oLabelAudioSampleRate.SetLabel(EMResourceRepository::GetString(RES_CP_LABEL_SAMPLE_RATE));
	m_oLabelFps.SetLabel(EMResourceRepository::GetString(RES_CP_LABEL_FPS));
	m_oLabelHeight.SetLabel(EMResourceRepository::GetString(RES_CP_LABEL_HEIGHT));
	m_oLabelWidth.SetLabel(EMResourceRepository::GetString(RES_CP_LABEL_WIDTH));
	m_oLabelVideoFormat.SetLabel(EMResourceRepository::GetString(RES_CP_LABEL_VIDEO_FORMAT));
	m_opWindow -> SetTitle(EMResourceRepository::GetString(RES_DW_TITLE_CREATE_PROJECT) -> c_str());

	m_opWindow -> AddChild(m_opView);
	m_opView -> AddListener(this);
	m_opWindow -> AddListener(this);
	m_opCancelButton -> AddListener(this);
	m_opOkButton -> AddListener(this);
	
	vector<string> oValueBoxValues;
	list<string>* opValueBoxValues = EMResourceRepository::GetStringList(RES_CP_VIDEO_MODES);
	list<string>::iterator oIterator = opValueBoxValues -> begin();
	while(oIterator != opValueBoxValues -> end())
	{
		oValueBoxValues.push_back(*oIterator);
		oIterator++;
	}
	m_oValueBoxVideoFormat.SetValues(oValueBoxValues);
	
	oValueBoxValues.clear();
	opValueBoxValues = EMResourceRepository::GetStringList(RES_CP_SAMPLE_RATES);
	oIterator = opValueBoxValues -> begin();
	while(oIterator != opValueBoxValues -> end())
	{
		oValueBoxValues.push_back(*oIterator);
		oIterator++;
	}
	m_oValueBoxSampleRate.SetValues(oValueBoxValues);
	
	EMSettingsRepository::Instance() -> SetDefaultSettingsContainer(this);
}

bool NewProjectDialog::MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage)
{
	float vX;
	float vY;
	bool vButtonOne;
	bool vButtonTwo;
	bool vButtonThree;

	switch(p_vMessage)
	{
	case EM_DRAW:
		Draw(m_opView -> GetClippingRect());
		return true;
	case EM_MOUSE_DOWN:
		m_opView -> GetMouse(vX, vY, vButtonOne, vButtonTwo, vButtonThree);
		MouseDown(vX, vY, vButtonOne, vButtonTwo, vButtonThree);
		return true;
	case EM_MOUSE_MOVED:
		m_opView -> GetMouse(vX, vY, vButtonOne, vButtonTwo, vButtonThree);
		MouseMoved(vX, vY, vButtonOne, vButtonTwo, vButtonThree);
		return true;
	case EM_MOUSE_UP:
		m_opView -> GetMouse(vX, vY, vButtonOne, vButtonTwo, vButtonThree);
		MouseUp(vX, vY);
		return true;
	case EM_QUIT_REQUESTED:
	case MESSAGE_BUTTON_CANCEL:
		if(m_opWindow -> Lock())
		{
			m_opWindow -> Hide();
			m_opWindow -> Unlock();
		}
	case MESSAGE_VIDEO_FORMAT_CHANGED:
		{
//;//cout_commented_out_4_release << "MESSAGE_VIDEO_FORMAT_CHANGED" << endl;
			if(m_opWindow -> Lock())
			{
				int32 vIndex = m_oValueBoxVideoFormat.GetValueIndex();
				// This is a hack, well live with it
				switch(vIndex)
				{
				case 0:
					m_opTextControlWidth -> SetText("320");
					m_opTextControlHeight -> SetText("240");
					m_opTextControlFps -> SetText("15");
					break;
				case 1:
					m_opTextControlWidth -> SetText("720");
					m_opTextControlHeight -> SetText("480");
					m_opTextControlFps -> SetText("29.97");
					break;
				case 2:
					m_opTextControlWidth -> SetText("720");
					m_opTextControlHeight -> SetText("486");
					m_opTextControlFps -> SetText("29.97");
					break;
				case 3:
					m_opTextControlWidth -> SetText("640");
					m_opTextControlHeight -> SetText("480");
					m_opTextControlFps -> SetText("29.97");
					break;
				case 4:
					m_opTextControlWidth -> SetText("352");
					m_opTextControlHeight -> SetText("240");
					m_opTextControlFps -> SetText("29.97");
					break;
				case 5:
					m_opTextControlWidth -> SetText("720");
					m_opTextControlHeight -> SetText("576");
					m_opTextControlFps -> SetText("25");
					break;
				case 6:
					m_opTextControlWidth -> SetText("768");
					m_opTextControlHeight -> SetText("576");
					m_opTextControlFps -> SetText("25");
					break;
				case 7:
					m_opTextControlWidth -> SetText("352");
					m_opTextControlHeight -> SetText("288");
					m_opTextControlFps -> SetText("25");
					break;
				}
				m_opWindow -> Unlock();
			}
			return true;
		}
	case MESSAGE_BUTTON_OK:
		if(AllValuesOk())
		{
			EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_CREATE_PROJECT, this, NULL);
			if(m_opWindow -> Lock())
			{
				m_opWindow -> Hide();
				m_opWindow -> Unlock();
			}
		}
		return true;
//	case EM_CREATE_PROJECT_REQUESTED:
	default:
		return false;
	}
}

void NewProjectDialog::MouseDown(float p_vX, float p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree)
{
//	if(m_opView -> LockWindow())
	{
		if(m_oValueBoxSampleRate.MouseDown(p_vX, p_vY, p_vButtonOne, p_vButtonTwo, p_vButtonThree))
			m_oValueBoxSampleRate.Draw(m_oValueBoxSampleRate.Frame());
		else if(m_oValueBoxVideoFormat.MouseDown(p_vX, p_vY, p_vButtonOne, p_vButtonTwo, p_vButtonThree))
			m_oValueBoxVideoFormat.Draw(m_oValueBoxVideoFormat.Frame());
//		m_opView -> UnlockWindow();
	}
}

void NewProjectDialog::MouseMoved(float p_vX, float p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree)
{
//	if(m_opView -> LockWindow())
	{
		if(m_oValueBoxSampleRate.MouseMoved(p_vX, p_vY, p_vButtonOne, p_vButtonTwo, p_vButtonThree))
			m_oValueBoxSampleRate.Draw(m_oValueBoxSampleRate.Frame());
		else if(m_oValueBoxVideoFormat.MouseMoved(p_vX, p_vY, p_vButtonOne, p_vButtonTwo, p_vButtonThree))
			m_oValueBoxVideoFormat.Draw(m_oValueBoxVideoFormat.Frame());
//		m_opView -> UnlockWindow();
	}
}

void NewProjectDialog::MouseUp(float p_vX, float p_vY)
{
//	if(m_opView -> LockWindow())
	{
		if(m_oValueBoxSampleRate.MouseUp(p_vX, p_vY))
			m_oValueBoxSampleRate.Draw(m_oValueBoxSampleRate.Frame());
		else if(m_oValueBoxVideoFormat.MouseUp(p_vX, p_vY))
			m_oValueBoxVideoFormat.Draw(m_oValueBoxVideoFormat.Frame());
//		m_opView -> UnlockWindow();
	}
}

void NewProjectDialog::SetDefaultSettings()
{
/*
;//cout_commented_out_4_release << "EM_CREATE_PROJECT_REQUESTED" << endl;
;//cout_commented_out_4_release << "SETTING_VIDEO_FRAMERATE: " << m_vFps;
;//cout_commented_out_4_release << "SETTING_AUDIO_FRAMERATE: " << m_vSampleRate;
;//cout_commented_out_4_release << "SETTING_VIDEO_FORMAT_WIDTH: " << m_vWidth;
;//cout_commented_out_4_release << "SETTING_VIDEO_FORMAT_HEIGHT: " << m_vHeight;
*/
//EMDebugger("NewProjectDialog::SetDefaultSettings");
	EMSettingsRepository::Instance() -> SetSetting(SETTING_VIDEO_FRAMERATE, EM_SETTING_FLOAT, new float(m_vFps), false);
	EMSettingsRepository::Instance() -> SetSetting(SETTING_AUDIO_FRAMERATE, EM_SETTING_INT32, new int32(m_vSampleRate), false);
	EMSettingsRepository::Instance() -> SetSetting(SETTING_VIDEO_FORMAT_WIDTH, EM_SETTING_INT, new int(m_vWidth), false);
	EMSettingsRepository::Instance() -> SetSetting(SETTING_VIDEO_FORMAT_HEIGHT, EM_SETTING_INT, new int(m_vHeight));		
}





