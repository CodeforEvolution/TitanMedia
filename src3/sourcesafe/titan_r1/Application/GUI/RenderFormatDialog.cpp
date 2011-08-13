#include "RenderFormatDialog.h"

#include "CommandIDs.h"
#include "EMButton.h"
#include "EMFactory.h"
#include "EMGUIGlobals.h"
#include "EMListenerRepository.h"
#include "EMMediaGlobals.h"
#include "EMMessages.h"
#include "EMResourceRepository.h"
#include "EMSettingIDs.h"
#include "EMSettingsRepository.h"
#include "EMTextControl.h"
#include "EMView.h"
#include "EMWindow.h"
#include "ResourceIDs.h"

#include <string>
#include <vector>

const uint32 MESSAGE_BUTTON_OK = 1;
const uint32 MESSAGE_BUTTON_CANCEL = 2;
const uint32 MESSAGE_ENCODER_FAMILY_CHANGED = 3;

RenderFormatDialog::RenderFormatDialog() :
m_opView(EMFactory::Instance() -> CreateView(EMRect(0, 0, 345, 205), EM_FOLLOW_LEFT_RIGHT | EM_FOLLOW_TOP_BOTTOM, false)),
m_oLabelAudioEncoder(m_opView, EMRect(5, 20 * 2 + 5 * 3, 95, 20 * 3 + 5 * 3), NULL, EM_COLOR_VIEW_BACKGROUND, false),
m_oLabelEncoderFamily(m_opView, EMRect(5, 5, 95, 20 + 5), NULL, EM_COLOR_VIEW_BACKGROUND, false),
m_oLabelFps(m_opView, EMRect(5, 20 * 5 + 5 * 6, 95, 20 * 6 + 5 * 6), NULL, EM_COLOR_VIEW_BACKGROUND, false),
m_oLabelHeight(m_opView, EMRect(5, 20 * 4 + 5 * 5, 95, 20 * 5 + 5 * 5), NULL, EM_COLOR_VIEW_BACKGROUND, false),
m_oLabelQuality(m_opView, EMRect(5, 20 * 6 + 5 * 7, 95, 20 * 7 + 5 * 7), NULL, EM_COLOR_VIEW_BACKGROUND, false),
m_oLabelVideoEncoder(m_opView, EMRect(5, 20 + 5 * 2, 95, 20 * 2 + 5 * 2), NULL, EM_COLOR_VIEW_BACKGROUND, false),
m_oLabelWidth(m_opView, EMRect(5, 20 * 3 + 5 * 4, 95, 20 * 4 + 5 * 4), NULL, EM_COLOR_VIEW_BACKGROUND, false),
m_opCancelButton(EMFactory::Instance() -> CreateButton(EMRect(300, 20 * 7 + 5 * 8, 340, 20 * 8 + 5 * 8), "Cancel", MESSAGE_BUTTON_CANCEL, 0, 0)),
m_opOkButton(EMFactory::Instance() -> CreateButton(EMRect(255, 20 * 7 + 5 * 8, 295, 20 * 8 + 5 * 8), "OK", MESSAGE_BUTTON_OK, 0, 0)),
m_opTextControlFps(EMFactory::Instance() -> CreateTextControl(EMRect(100, 20 * 5 + 5 * 6, 150, 20 * 6 + 5 * 6), "15", 0, 0, false)),
m_opTextControlHeight(EMFactory::Instance() -> CreateTextControl(EMRect(100, 20 * 4 + 5 * 5, 150, 20 * 5 + 5 * 5), "240", 0, 0, false)),
m_opTextControlWidth(EMFactory::Instance() -> CreateTextControl(EMRect(100, 20 * 3 + 5 * 4, 150, 20 * 4 + 5 * 4), "320", 0, 0, false)),
m_opWindow(EMFactory::Instance() -> CreateEMWindow(EMRect(100, 100, 445, 305), "RenderFormat Dialog", EM_WINDOW_DIALOG)),
m_oValueBoxAudioEncoder(m_opView, EMRect(100, 20 * 2 + 5 * 3, 340, 20 * 3 + 5 * 3), vector<string>(), 0, NULL, NULL, NULL, true, true, true, true, 0, false),
m_oValueBoxEncoderFamily(m_opView, EMRect(100, 5, 340, 20 + 5), vector<string>(), 0, NULL, NULL, NULL, true, true, true, true, 0, false),
m_oValueBoxQuality(m_opView, EMRect(100, 20 * 6 + 5 * 7, 150, 20 * 7 + 5 * 7), 0, 100, 50, 0, NULL, NULL, NULL, true, true, true, true, 0, false),
m_oValueBoxVideoEncoder(m_opView, EMRect(100, 20 + 5 * 2, 340, 20 * 2 + 5 * 2), vector<string>(), 0, NULL, NULL, NULL, true, true, true, true, 0, false),
m_vEncoderFamilyMessage(MESSAGE_ENCODER_FAMILY_CHANGED),
m_vIsInitialized(false)
{
	
	m_opWindow -> Show(); // Yuck, why do I have to do this
	m_opWindow -> Hide();
}

RenderFormatDialog::~RenderFormatDialog()
{
	delete m_opCancelButton;
	delete m_opOkButton;
	delete m_opView;
	delete m_opTextControlFps;
	delete m_opTextControlHeight;
	delete m_opTextControlWidth;
}

bool RenderFormatDialog::AllValuesOk()
{
	string oFpsString(m_opTextControlFps -> Text());
	string oWidthString(m_opTextControlWidth -> Text());
	string oHeightString(m_opTextControlHeight -> Text());
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

	return vOk;
}

void RenderFormatDialog::DisplayDialog()
{
	if(m_opWindow -> Lock())
	{
		if(!m_vIsInitialized)
		{
			m_oValueBoxEncoderFamily.SetCommand(COMMAND_MESSAGE_SENDER, &m_vEncoderFamilyMessage, this, NULL);
			Init();
			m_vIsInitialized = true;
		}
		InitValues();
		m_opWindow -> Unlock();
	}
		if(m_opWindow -> IsHidden())
			m_opWindow -> Show();
}

void RenderFormatDialog::Draw(EMRect p_oClippingRect)
{
	m_opView -> SetHighColor(EM_COLOR_VIEW_BACKGROUND);
	m_opView -> FillRect(m_opView -> Bounds());
	m_oLabelAudioEncoder.Draw(p_oClippingRect);
	m_oLabelEncoderFamily.Draw(p_oClippingRect);
	m_oLabelFps.Draw(p_oClippingRect);
	m_oLabelHeight.Draw(p_oClippingRect);
	m_oLabelQuality.Draw(p_oClippingRect);
	m_oLabelVideoEncoder.Draw(p_oClippingRect);
	m_oLabelWidth.Draw(p_oClippingRect);
	m_oValueBoxAudioEncoder.Draw(p_oClippingRect);
	m_oValueBoxEncoderFamily.Draw(p_oClippingRect);
	m_oValueBoxQuality.Draw(p_oClippingRect);
	m_oValueBoxVideoEncoder.Draw(p_oClippingRect);
}

void RenderFormatDialog::Init()
{
;//cout_commented_out_4_release << "RenderFormatDialog::Init" << endl;
	m_oLabelAudioEncoder.SetLabel(EMResourceRepository::GetString(RES_RF_LABEL_AUDIO_ENCODER));
	m_oLabelEncoderFamily.SetLabel(EMResourceRepository::GetString(RES_RF_LABEL_ENCODER_FAMILY));
	m_oLabelFps.SetLabel(EMResourceRepository::GetString(RES_RF_LABEL_VIDEO_FPS));
	m_oLabelHeight.SetLabel(EMResourceRepository::GetString(RES_RF_LABEL_VIDEO_HEIGHT));
	m_oLabelQuality.SetLabel(EMResourceRepository::GetString(RES_RF_LABEL_VIDEO_QUALITY));
	m_oLabelVideoEncoder.SetLabel(EMResourceRepository::GetString(RES_RF_LABEL_VIDEO_ENCODER));
	m_oLabelWidth.SetLabel(EMResourceRepository::GetString(RES_RF_LABEL_VIDEO_WIDTH));
	m_opWindow -> SetTitle(EMResourceRepository::GetString(RES_DW_TITLE_RENDER_FORMAT) -> c_str());

	m_opWindow -> AddChild(m_opView);
	m_opView -> AddChild(m_opCancelButton);
	m_opView -> AddChild(m_opOkButton);
	m_opView -> AddChild(m_opTextControlFps);
	m_opView -> AddChild(m_opTextControlHeight);
	m_opView -> AddChild(m_opTextControlWidth);

	m_opView -> AddListener(this);
	m_opWindow -> AddListener(this);
	m_opCancelButton -> AddListener(this);
	m_opOkButton -> AddListener(this);	
}

void RenderFormatDialog::InitValues()
{
	
	vector<string> oValueBoxValues;
	list<EMCodecFormat*>* opCodecFormats = static_cast<list<EMCodecFormat*>*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_CODECS, NULL, NULL));
	list<EMCodecFormat*>::iterator oIterator = opCodecFormats -> begin();
	while(oIterator != opCodecFormats -> end())
	{
		oValueBoxValues.push_back((*oIterator) -> m_oFormatLongName);
		oIterator++;
	}
	m_oValueBoxEncoderFamily.SetValues(oValueBoxValues);

	char vTextControlValue[30];
	m_vFps = *(static_cast<float*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_VIDEO_RENDER_FRAMERATE)));
	sprintf(vTextControlValue, "%f", m_vFps);
	m_opTextControlFps -> SetText(vTextControlValue);

	m_vHeight = *(static_cast<int*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_VIDEO_RENDER_FORMAT_HEIGHT)));
	sprintf(vTextControlValue, "%d", m_vHeight);
	m_opTextControlHeight -> SetText(vTextControlValue);

	m_vWidth = *(static_cast<int*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_VIDEO_RENDER_FORMAT_WIDTH)));
	sprintf(vTextControlValue, "%d", m_vWidth);
	m_opTextControlWidth -> SetText(vTextControlValue);

	string oFamily(*(static_cast<string*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_RENDER_CODEC_FAMILY))));
;//cout_commented_out_4_release << "oFamily: " << oFamily << endl;
	if(oFamily != "unknown")
	{
		m_oValueBoxEncoderFamily.SetValue(oFamily);
;//cout_commented_out_4_release << "Setting family to: " << oFamily << endl;
		SetValueBoxes();
		string oAudioCodec(*(static_cast<string*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_AUDIO_RENDER_CODEC))));
		m_oValueBoxAudioEncoder.SetValue(oAudioCodec);
;//cout_commented_out_4_release << "Setting audio codec to: " << oAudioCodec << endl;
		string oVideoCodec(*(static_cast<string*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_VIDEO_RENDER_CODEC))));
		m_oValueBoxVideoEncoder.SetValue(oVideoCodec);
;//cout_commented_out_4_release << "Setting video codec to: " << oVideoCodec << endl;
	}
	else
		SetValueBoxes();
		
	m_oValueBoxQuality.SetValue(*(static_cast<int*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_RENDER_CODEC_QUALITY))));
}

bool RenderFormatDialog::MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage)
{
	float vX;
	float vY;
	bool vButtonOne;
	bool vButtonTwo;
	bool vButtonThree;

	switch(p_vMessage)
	{
	case EM_DRAW:
;//cout_commented_out_4_release << "EM_DRAW";
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
	case MESSAGE_ENCODER_FAMILY_CHANGED:
		if(m_opWindow -> Lock())
		{
			SetValueBoxes();
			m_opView -> Invalidate(m_oValueBoxAudioEncoder.Frame());
			m_opView -> Invalidate(m_oValueBoxVideoEncoder.Frame());
			m_opWindow -> Unlock();
			return true;
		}
	case MESSAGE_BUTTON_OK:
		if(AllValuesOk())
		{
			if(m_opWindow -> Lock())
			{
;//cout_commented_out_4_release << "values: " << m_vHeight << ", " << m_vWidth << ", " << m_vFps << ", " << *(static_cast<string*>(m_oValueBoxVideoEncoder.GetValue())) << ", " << *(static_cast<string*>(m_oValueBoxAudioEncoder.GetValue())) << ", ";
;//cout_commented_out_4_release << *(static_cast<string*>(m_oValueBoxEncoderFamily.GetValue())) << ", " << *(static_cast<int*>(m_oValueBoxQuality.GetValue())) << endl;
				EMSettingsRepository::Instance() -> SetSetting(SETTING_VIDEO_RENDER_FORMAT_HEIGHT, EM_SETTING_INT, EM_new int(m_vHeight));
				EMSettingsRepository::Instance() -> SetSetting(SETTING_VIDEO_RENDER_FORMAT_WIDTH, EM_SETTING_INT, EM_new int(m_vWidth));
				EMSettingsRepository::Instance() -> SetSetting(SETTING_VIDEO_RENDER_FRAMERATE, EM_SETTING_FLOAT, EM_new float(m_vFps));
				EMSettingsRepository::Instance() -> SetSetting(SETTING_VIDEO_RENDER_CODEC, EM_SETTING_STRING, EM_new string(*(static_cast<string*>(m_oValueBoxVideoEncoder.GetValue()))));
				EMSettingsRepository::Instance() -> SetSetting(SETTING_AUDIO_RENDER_CODEC, EM_SETTING_STRING, EM_new string(*(static_cast<string*>(m_oValueBoxAudioEncoder.GetValue()))));
				EMSettingsRepository::Instance() -> SetSetting(SETTING_RENDER_CODEC_FAMILY, EM_SETTING_STRING, EM_new string(*(static_cast<string*>(m_oValueBoxEncoderFamily.GetValue()))));
				EMSettingsRepository::Instance() -> SetSetting(SETTING_RENDER_CODEC_QUALITY, EM_SETTING_INT, EM_new int(*(static_cast<int*>(m_oValueBoxQuality.GetValue()))));

				m_opWindow -> Hide();
				m_opWindow -> Unlock();
			}
		}
		return true;
	default:
		return false;
	}
}

void RenderFormatDialog::MouseDown(float p_vX, float p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree)
{
//	if(m_opView -> LockWindow())
	{
		if(m_oValueBoxAudioEncoder.MouseDown(p_vX, p_vY, p_vButtonOne, p_vButtonTwo, p_vButtonThree))
			m_oValueBoxAudioEncoder.Draw(m_oValueBoxAudioEncoder.Frame());
		else if(m_oValueBoxEncoderFamily.MouseDown(p_vX, p_vY, p_vButtonOne, p_vButtonTwo, p_vButtonThree))
			m_oValueBoxEncoderFamily.Draw(m_oValueBoxEncoderFamily.Frame());
		else if(m_oValueBoxQuality.MouseDown(p_vX, p_vY, p_vButtonOne, p_vButtonTwo, p_vButtonThree))
			m_oValueBoxQuality.Draw(m_oValueBoxQuality.Frame());
		else if(m_oValueBoxVideoEncoder.MouseDown(p_vX, p_vY, p_vButtonOne, p_vButtonTwo, p_vButtonThree))
			m_oValueBoxVideoEncoder.Draw(m_oValueBoxVideoEncoder.Frame());
//		m_opView -> UnlockWindow();
	}
}

void RenderFormatDialog::MouseMoved(float p_vX, float p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree)
{
//	if(m_opView -> LockWindow())
	{
		if(m_oValueBoxEncoderFamily.MouseMoved(p_vX, p_vY, p_vButtonOne, p_vButtonTwo, p_vButtonThree))
			m_oValueBoxEncoderFamily.Draw(m_oValueBoxEncoderFamily.Frame());
		else if(m_oValueBoxAudioEncoder.MouseMoved(p_vX, p_vY, p_vButtonOne, p_vButtonTwo, p_vButtonThree))
			m_oValueBoxAudioEncoder.Draw(m_oValueBoxAudioEncoder.Frame());
		else if(m_oValueBoxQuality.MouseMoved(p_vX, p_vY, p_vButtonOne, p_vButtonTwo, p_vButtonThree))
			m_oValueBoxQuality.Draw(m_oValueBoxQuality.Frame());
		else if(m_oValueBoxVideoEncoder.MouseMoved(p_vX, p_vY, p_vButtonOne, p_vButtonTwo, p_vButtonThree))
			m_oValueBoxVideoEncoder.Draw(m_oValueBoxVideoEncoder.Frame());
//		m_opView -> UnlockWindow();
	}
}

void RenderFormatDialog::MouseUp(float p_vX, float p_vY)
{
//	if(m_opView -> LockWindow())
	{
		if(m_oValueBoxAudioEncoder.MouseUp(p_vX, p_vY))
			m_oValueBoxAudioEncoder.Draw(m_oValueBoxAudioEncoder.Frame());
		else if(m_oValueBoxEncoderFamily.MouseUp(p_vX, p_vY))
			m_oValueBoxEncoderFamily.Draw(m_oValueBoxEncoderFamily.Frame());
		else if(m_oValueBoxQuality.MouseUp(p_vX, p_vY))
			m_oValueBoxQuality.Draw(m_oValueBoxQuality.Frame());
		else if(m_oValueBoxVideoEncoder.MouseUp(p_vX, p_vY))
			m_oValueBoxVideoEncoder.Draw(m_oValueBoxVideoEncoder.Frame());
//		m_opView -> UnlockWindow();
	}
}

void RenderFormatDialog::SetValueBoxes()
{
	vector<string> oValueBoxValues;
	list<EMCodecFormat*>* opCodecFormats = static_cast<list<EMCodecFormat*>*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_CODECS, NULL, NULL));
	list<EMCodecFormat*>::iterator oFamilyIterator = opCodecFormats -> begin();
	int vFamilyIndex = m_oValueBoxEncoderFamily.GetValueIndex();
	for(int vIndex = 0; vIndex < vFamilyIndex; vIndex++)
		oFamilyIterator++;
	list<EMCodecInfo> oAudioCodecs((*oFamilyIterator) -> m_oRawAudioCodecShortNames);
	list<EMCodecInfo> oVideoCodecs((*oFamilyIterator) -> m_oRawVideoCodecShortNames);

	list<EMCodecInfo>::iterator oCodecIterator = oAudioCodecs.begin();
	while(oCodecIterator != oAudioCodecs.end())
	{
		oValueBoxValues.push_back((*oCodecIterator).m_oLongName);
		oCodecIterator++;
	}
	m_oValueBoxAudioEncoder.SetValues(oValueBoxValues);

	oValueBoxValues.clear();
	oCodecIterator = oVideoCodecs.begin();
	while(oCodecIterator != oVideoCodecs.end())
	{
		oValueBoxValues.push_back((*oCodecIterator).m_oLongName);
		oCodecIterator++;
	}
	m_oValueBoxVideoEncoder.SetValues(oValueBoxValues);
}







