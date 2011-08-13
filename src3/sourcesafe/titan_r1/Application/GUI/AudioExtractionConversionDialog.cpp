#include "AudioExtractionConversionDialog.h"

#include "EMButton.h"
#include "EMFactory.h"
#include "EMGUIUtilities.h"
#include "EMLabel.h"
#include "EMMessages.h"
#include "EMProgressBar.h"
#include "EMView.h"
#include "EMWindow.h"

#include "EMCommandRepository.h"
#include "CommandIDs.h"

#ifdef PLATFORM_WINDOWS
#include <windows.h> // To get modal dialog to work.
#include "EMWinWindow.h" // To get modal dialog to work.
//#include <mmsystem.h>
#endif

AudioExtractionConversionDialog* AudioExtractionConversionDialog::m_opInstance = NULL;

const uint32 CANCEL_BUTTON = 1;

AudioExtractionConversionDialog::AudioExtractionConversionDialog() :
m_opDialogWindow(EMFactory::Instance() -> CreateEMWindow(EMRect(0, 0, 300, 110), "Audio Extraction Conversion", EM_WINDOW_POPUP_BORDER)), 
m_opMainView(EMFactory::Instance() -> CreateView(EMRect(0, 0, 300, 110), false)),
m_opTopLabel(EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
														 EM_METRICS_NATIVE_DIALOG_MARGIN_TOP,
														 300 - EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
														 EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + EM_METRICS_NATIVE_LABEL_HEIGHT),
												  "", 0)),
m_opProgressBar(EMFactory::Instance() -> CreateProgressBar(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
																  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 20,
																  300 - EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
																  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 20 + EM_METRICS_NATIVE_PROGRESS_BAR_HEIGHT),
														   0)),
m_opBottomLabel(EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
															EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 50,
															300 - EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
															EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 50 + EM_METRICS_NATIVE_LABEL_HEIGHT),
													 "", 0)),
m_opETA_Label(EMFactory::Instance() ->	CreateLabel(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
														   EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 75,
														   300 - EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
														   EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 75 + EM_METRICS_NATIVE_LABEL_HEIGHT),
													"", 0)),
m_vpETA_Text(EM_new char[sizeof(char) * 256]),
m_vpTime(EM_new char[sizeof(char) * 10]),
m_vpTotTime(EM_new char[sizeof(char) * 10])
{
	m_opCancelButton = EMFactory::Instance() ->CreateButton(EMRect((m_opMainView ->Frame().GetWidth() / 2) - (EM_METRICS_NATIVE_BUTTON_WIDTH / 2),
																	 110,
																	 (m_opMainView ->Frame().GetWidth() / 2) + (EM_METRICS_NATIVE_BUTTON_WIDTH / 2),
																	 110 + EM_METRICS_NATIVE_BUTTON_HEIGHT),
															 "Cancel", CANCEL_BUTTON, 0, 0);

	m_opDialogWindow -> AddChild(m_opMainView);
	m_opMainView -> AddChild(m_opTopLabel);
	m_opMainView -> AddChild(m_opProgressBar);
	m_opMainView -> AddChild(m_opBottomLabel);
	m_opMainView -> AddChild(m_opETA_Label);
	m_opMainView -> AddChild(m_opCancelButton);
}
AudioExtractionConversionDialog::~AudioExtractionConversionDialog()
{
	delete m_opTopLabel;
	delete m_opProgressBar;
	delete m_opBottomLabel;
	delete m_opETA_Label;
	delete m_opCancelButton;
	delete m_opDialogWindow;
	delete m_opMainView;
}

void AudioExtractionConversionDialog::AddCancelButton(bool p_vAdd, uint32 p_vCommandID)
{
	m_vCommandID = p_vCommandID;
	if(m_opDialogWindow -> Lock())
	{
		if(p_vAdd)
		{
			m_opDialogWindow -> SetFrame(EMRect(0, 0, 300, 142), true);
			m_opMainView -> SetFrame(EMRect(0, 0, 300, 142));
		}
		else
		{
			m_opDialogWindow -> SetFrame(EMRect(0, 0, 300, 110), true);
			m_opMainView -> SetFrame(EMRect(0, 0, 300, 110));
		}
		m_opDialogWindow -> Unlock();
	}
}

void AudioExtractionConversionDialog::Delete()
{
	if(m_opInstance)
		delete m_opInstance;
}

void AudioExtractionConversionDialog::DisplayDialog()
{
	if(m_opDialogWindow -> Lock())
	{
		m_opDialogWindow -> Show();
#ifdef PLATFORM_WINDOWS
		EnableWindow(dynamic_cast<EMWinWindow*>(m_opDialogWindow) -> GetMainWindowHandle(), FALSE);
		SetFocus(static_cast<HWND>(m_opDialogWindow -> GetNativeView()));
#endif
		m_opDialogWindow -> Unlock();
	}
	m_vCountWhen = 0;
}

void AudioExtractionConversionDialog::HideDialog()
{
	if(m_opDialogWindow -> Lock())
	{
#ifdef PLATFORM_WINDOWS
		EnableWindow(dynamic_cast<EMWinWindow*>(m_opDialogWindow) -> GetMainWindowHandle(), TRUE);
		SetFocus(dynamic_cast<EMWinWindow*>(m_opDialogWindow) -> GetMainWindowHandle());
#endif
		m_opDialogWindow -> Hide();
		m_opProgressBar -> SetPosition(0);
		m_opDialogWindow -> Unlock();
	}
}

void AudioExtractionConversionDialog::Init()
{
	m_opDialogWindow -> AddListener(this);
	m_opMainView -> AddListener(this);
	if(m_opDialogWindow -> Lock())
	{
		//m_opMainView -> SetViewColor(EMColor(192, 192, 192, 255));
		m_opMainView -> SetDefaultColor();
		m_opDialogWindow -> Unlock();
	}
	m_opProgressBar -> SetEndValue(1000);
}

AudioExtractionConversionDialog* AudioExtractionConversionDialog::Instance()
{
	if(m_opInstance)
		return m_opInstance;
	return m_opInstance = EM_new AudioExtractionConversionDialog;
}

bool AudioExtractionConversionDialog::MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage)
{
	switch(p_vMessage)
	{
	case CANCEL_BUTTON:
		EMCommandRepository::Instance() -> ExecuteCommand(m_vCommandID);
		HideDialog();
		break;
/*	case EM_CLOSE_WINDOW:
		if(p_opSender == m_opDialogWindow)
		{
			eo << "EM_CLOSE_WINDOW" << ef;
			{
			int test = 0;
			for(int i = 0; i<1000 ; i++)
			{
				test = i;
				EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_SET_VALUE_IN_AUDIO_EXTRACTION_CONVERSION_DIALOG, &test);
				Sleep(1);
			}
			EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_SET_BOTTOM_LABEL_IN_AUDIO_EXTRACTION_CONVERSION_DIALOG, "Bakåt");
			while(i)
			{
				test = i--;
				EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_SET_VALUE_IN_AUDIO_EXTRACTION_CONVERSION_DIALOG, &test);
				Sleep(1);
			}

		}
		EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_CLOSE_AUDIO_EXTRACTION_CONVERSION_DIALOG);

//			HideDialog();
			return true;
		}
		break;
*/	}
	return false;
}

void AudioExtractionConversionDialog::MoveDialog()
{
	EMRect oWindowRect(m_opParentWindow -> Frame());
	EMRect oDialogRect(m_opDialogWindow -> Frame());
	float vX(oWindowRect.m_vRight - oWindowRect.GetWidth() / 2 - oDialogRect.GetWidth() / 2);
	float vY(oWindowRect.m_vBottom - oWindowRect.GetHeight() / 2 - oDialogRect.GetHeight() / 2);

	MoveDialog(vX, vY);
}

void AudioExtractionConversionDialog::MoveDialog(float p_vX, float p_vY)
{
	m_opDialogWindow -> MoveTo(p_vX, p_vY);
}

void AudioExtractionConversionDialog::SetBottomLabel(const char* p_vpText)
{
	m_opBottomLabel -> SetLabel(p_vpText);
}

void AudioExtractionConversionDialog::SetCommand(uint32 p_vCommandID)
{
	if(p_vCommandID == 0)
		AddCancelButton(false);
	else
		AddCancelButton(true, p_vCommandID);
}

void AudioExtractionConversionDialog::SetParent(EMWindow* p_opWindow)
{
	m_opParentWindow = p_opWindow;
}

void AudioExtractionConversionDialog::SetTopLabel(const char* p_vpText)
{
	m_opTopLabel -> SetLabel(p_vpText);
}

void AudioExtractionConversionDialog::SetValue(uint16 p_vValue)
{
	m_opProgressBar -> SetPosition(p_vValue);
	if(m_vCountWhen == 0)
	{
		m_vStartTime = timeGetTime();
		m_opETA_Label -> SetLabel("Time remaining: --:--:--");
		m_vCountWhen += 25;
	}
	else if(m_vCountWhen <= p_vValue)
	{
		m_vCurrentTime = timeGetTime() - m_vStartTime;
	/*	int vHours = static_cast<int>(m_vCurrentTime / 600000L);
		int vMinutes = static_cast<int>(m_vCurrentTime / 60000L);
		int vSeconds = static_cast<int>((m_vCurrentTime  % 60000L) / 1000L);
		sprintf(m_vpTime, "%02d:%02d:%02d", vHours, vMinutes, vSeconds);
*/
		eo << " *** CurrentTime = " << m_vCurrentTime << ef;
		m_vTotTime = m_vCurrentTime * static_cast<float>((static_cast<float>(1000) / static_cast<float>(m_vCountWhen)));

		int vHours = static_cast<int>((m_vTotTime - m_vCurrentTime) / 3600000L);
		int vMinutes = static_cast<int>(((m_vTotTime - m_vCurrentTime) % 3600000L) / 60000L);
		int vSeconds = static_cast<int>(((m_vTotTime - m_vCurrentTime) % 60000L) / 1000L);
		sprintf(m_vpTotTime, "%02d:%02d:%02d", vHours, vMinutes, vSeconds);

		sprintf(m_vpETA_Text, "Time remaining: %s", m_vpTotTime);
		m_opETA_Label -> SetLabel(m_vpETA_Text);
		while(m_vCountWhen <= p_vValue)
			m_vCountWhen += 25;
	}
	
#ifdef PLATFORM_WINDOWS
	UpdateWindow(static_cast<HWND>(m_opDialogWindow -> GetNativeView()));
#endif
}