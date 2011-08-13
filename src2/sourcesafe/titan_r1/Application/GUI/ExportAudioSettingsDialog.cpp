#include "ExportAudioSettingsDialog.h"

#include "EMFactory.h"

#include "EMWindow.h"
#include "EMView.h"
#include "EMButton.h"
#include "EMTextControl.h"
#include "EMLabel.h"
#include "EMComboBox.h"
#include "EMCommandRepository.h"
#include "CommandIDs.h"

#include "EMMessages.h"

#ifdef PLATFORM_WINDOWS
#include <windows.h> // To get modal dialog to work.
#include "EMWinWindow.h" // To get modal dialog to work.
#endif

const uint32 OK_BUTTON = 1;
const uint32 CANCEL_BUTTON = 2;

ExportAudioSettingsDialog::ExportAudioSettingsDialog() :
m_opDialogWindow(EMFactory::Instance() -> CreateEMWindow(EMRect(0, 0, 343, 170), "Export Audio Settings", EM_WINDOW_DIALOG)), 
m_opMainView(EMFactory::Instance() -> CreateView(EMRect(0, 0, 343, 170), false)),
m_opFamilyLabel(EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
															 EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 2,
															 EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 50,
															 EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 2 + EM_METRICS_NATIVE_LABEL_HEIGHT),
													  "Family:", 0)),
m_opEncoderLabel(EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
																EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 27,
																EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 70,
																EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 27 + EM_METRICS_NATIVE_LABEL_HEIGHT),
														 "Encoder:", 0)),
m_opFrequencyLabel(EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
																EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 52,
																EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 70,
																EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 52 + EM_METRICS_NATIVE_LABEL_HEIGHT),
														 "Frequency:", 0)),
m_opBitRateLabel(EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
																EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 77,
																EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 70,
																EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 77 + EM_METRICS_NATIVE_LABEL_HEIGHT),
														 "Bit Rate:", 0)),
m_opChannelsLabel(EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
																EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 102,
																EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 70,
																EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 102 + EM_METRICS_NATIVE_LABEL_HEIGHT),
														 "Channels:", 0)),
m_opFamilyComboBox(EMFactory::Instance() -> CreateComboBox(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 73,
																   EM_METRICS_NATIVE_DIALOG_MARGIN_TOP,
																   EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 73 + 250,
																   EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 100),
															0, 0, true)),
m_opEncoderComboBox(EMFactory::Instance() -> CreateComboBox(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 73,
																   EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 25,
																   EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 73 + 250,
																   EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 25 + 100),
															0, 0, true)),
m_opFrequencyTextControl(EMFactory::Instance() -> CreateTextControl(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 73,
																   EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 50,
																   EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 73 + 50,
																   EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 50 + EM_METRICS_NATIVE_TEXT_CONTROL_HEIGHT),
															"22000", 0, 0, false, EM_ALIGNMENT_RIGHT, true)),
m_opBitRateComboBox(EMFactory::Instance() ->CreateComboBox(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 73,
																   EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 75,
																   EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 73 + 100,
																   EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 75 + 100),
															0, 0, false)),
m_opChannelsComboBox(EMFactory::Instance() -> CreateComboBox(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 73,
																   EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 100,
																   EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 73 + 100,
																   EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 100 + 100),
															0, 0, true)),
m_opOK_Button(EMFactory::Instance() -> CreateButton(EMRect(343 - (EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + EM_METRICS_NATIVE_BUTTON_WIDTH * 2 + EM_METRICS_NATIVE_BUTTON_SPACE),
														   170 - (EM_METRICS_NATIVE_DIALOG_MARGIN_BOTTOM + EM_METRICS_NATIVE_BUTTON_HEIGHT),
														   343 - (EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + EM_METRICS_NATIVE_BUTTON_WIDTH + EM_METRICS_NATIVE_BUTTON_SPACE),
														   170 - (EM_METRICS_NATIVE_DIALOG_MARGIN_BOTTOM)),
													"OK", OK_BUTTON, 0, 0, NULL, true)),
m_opCancelButton(EMFactory::Instance() -> CreateButton(EMRect(343 - (EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + EM_METRICS_NATIVE_BUTTON_WIDTH),
															  170 - (EM_METRICS_NATIVE_DIALOG_MARGIN_BOTTOM + EM_METRICS_NATIVE_BUTTON_HEIGHT),
															  343 - (EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE),
															  170 - (EM_METRICS_NATIVE_DIALOG_MARGIN_BOTTOM)),
													   "Cancel", CANCEL_BUTTON, 0, 0))
{
	m_opDialogWindow -> AddChild(m_opMainView);
	m_opMainView -> AddChild(m_opFamilyLabel);
	m_opMainView -> AddChild(m_opEncoderLabel);
	m_opMainView -> AddChild(m_opFrequencyLabel);
	m_opMainView -> AddChild(m_opBitRateLabel);
	m_opMainView -> AddChild(m_opChannelsLabel);
	m_opMainView -> AddChild(m_opFamilyComboBox);
	m_opMainView -> AddChild(m_opEncoderComboBox);
	m_opMainView -> AddChild(m_opFrequencyTextControl);
	m_opMainView -> AddChild(m_opBitRateComboBox);
	m_opMainView -> AddChild(m_opChannelsComboBox);
	m_opMainView -> AddChild(m_opOK_Button);
	m_opMainView -> AddChild(m_opCancelButton);
}
ExportAudioSettingsDialog::~ExportAudioSettingsDialog()
{
	delete m_opDialogWindow;
	delete m_opMainView;
	delete m_opFamilyLabel;
	delete m_opEncoderLabel;
	delete m_opFrequencyLabel;
	delete m_opBitRateLabel;
	delete m_opChannelsLabel;
	delete m_opFamilyComboBox;
	delete m_opEncoderComboBox;
	delete m_opFrequencyTextControl;
	delete m_opBitRateComboBox;
	delete m_opChannelsComboBox;
	delete m_opOK_Button;
	delete m_opCancelButton;
}

void ExportAudioSettingsDialog::DisplayDialog()
{
	if(m_opDialogWindow -> Lock())
	{
		m_opDialogWindow -> Show();
#ifdef PLATFORM_WINDOWS
		EnableWindow(static_cast<HWND>(m_opParentWindow -> GetNativeView()), FALSE);
		SetFocus(static_cast<HWND>(m_opDialogWindow -> GetNativeView()));
#endif
		m_opDialogWindow -> Unlock();
	}
}

void ExportAudioSettingsDialog::HideDialog()
{
	if(m_opDialogWindow -> Lock())
	{
#ifdef PLATFORM_WINDOWS
		EnableWindow(static_cast<HWND>(m_opParentWindow -> GetNativeView()), TRUE);
		SetFocus(static_cast<HWND>(m_opParentWindow -> GetNativeView()));
#endif
		m_opDialogWindow -> Hide();
		m_opDialogWindow -> Unlock();
	}
}

void ExportAudioSettingsDialog::Init()
{
	m_opDialogWindow -> AddListener(this);
	m_opMainView -> AddListener(this);
	if(m_opDialogWindow -> Lock())
	{
		//m_opMainView -> SetViewColor(EMColor(192, 192, 192, 255));
		m_opMainView -> SetDefaultColor();
		m_opDialogWindow -> Unlock();
	}
	m_opFamilyComboBox -> AddListener(this);
	m_opEncoderComboBox -> AddListener(this);
	m_opFrequencyTextControl -> AddListener(this);
	m_opBitRateComboBox -> AddListener(this);
	m_opChannelsComboBox -> AddListener(this);
	m_opOK_Button -> AddListener(this);
	m_opCancelButton -> AddListener(this);

	//Setting TabOrder
	m_opDialogWindow -> AddTabComponent(m_opFamilyComboBox);
	m_opDialogWindow -> AddTabComponent(m_opEncoderComboBox);
	m_opDialogWindow -> AddTabComponent(m_opFrequencyTextControl);
	m_opDialogWindow -> AddTabComponent(m_opBitRateComboBox);
	m_opDialogWindow -> AddTabComponent(m_opChannelsComboBox);
	m_opDialogWindow -> AddTabComponent(m_opOK_Button);
	m_opDialogWindow -> AddTabComponent(m_opCancelButton);

	m_opFamilyComboBox -> AddString("Wave (.wav)");
	m_opFamilyComboBox -> SelectItem(0);

	m_opEncoderComboBox -> AddString("PCM (uncompressed)");
	m_opEncoderComboBox -> SelectItem(0);

	m_opBitRateComboBox -> AddString("8");
	m_opBitRateComboBox -> AddString("16");
	m_opBitRateComboBox -> SelectItem(0);

	m_opChannelsComboBox -> AddString("Mono");
	m_opChannelsComboBox -> AddString("Stereo");
	m_opChannelsComboBox -> SelectItem(0);
}

bool ExportAudioSettingsDialog::MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage)
{
	switch(p_vMessage)
	{
	case EM_GUI_COMPONENT_ENTER_PRESSED:
	case OK_BUTTON:
		{
			eo << "OK button pressed" << ef;
			int32 vFrequency(atoi(m_opFrequencyTextControl -> Text()));
			int vBitRate;
			bool vMono;
			if(m_opBitRateComboBox -> GetSelectionIndex() == 0)
				vBitRate = 8;
			else
				vBitRate = 16;
			if(m_opChannelsComboBox -> GetSelectionIndex() == 0)
				vMono = true;
			else 
				vMono = false;
			EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_SET_AUDIO_RENDER_PARAMETERS, &vFrequency, &vBitRate, &vMono);
		//	eo << "Getting TextControl value: " << m_opTextControl -> Text() << ef;
			HideDialog();
			return true;
		}
	case EM_GUI_COMPONENT_ESC_PRESSED:
	case CANCEL_BUTTON:
		eo << "Cancel button pressed" << ef;
		HideDialog();
		return true;
	case EM_GUI_COMPONENT_TAB_PRESSED:
		m_opDialogWindow -> NextTab();
		return true;
	case EM_GUI_COMPONENT_SHIFT_TAB_PRESSED:
		m_opDialogWindow -> PreviousTab();
		return true;
	case EM_GUI_COMPONENT_GOT_FOCUS:
		if(p_opSender == m_opDialogWindow)
			m_opFamilyComboBox -> SetFocus();
		m_opDialogWindow -> SetCurrentFocus();
		return true;
	case EM_CLOSE_WINDOW:
		if(p_opSender == m_opDialogWindow)
		{
			eo << "EM_CLOSE_WINDOW" << ef;
			HideDialog();
			return true;
		}
		break;
	}
	return false;
}

void ExportAudioSettingsDialog::MoveDialog()
{
	EMRect oWindowRect(m_opMainAppWindow -> Frame());
	EMRect oDialogRect(m_opDialogWindow -> Frame());
	float vX(oWindowRect.m_vRight - oWindowRect.GetWidth() / 2 - oDialogRect.GetWidth() / 2);
	float vY(oWindowRect.m_vBottom - oWindowRect.GetHeight() / 2 - oDialogRect.GetHeight() / 2);

	MoveDialog(vX, vY);
}

void ExportAudioSettingsDialog::MoveDialog(float p_vX, float p_vY)
{
	m_opDialogWindow -> MoveTo(p_vX, p_vY);
}

void ExportAudioSettingsDialog::SetMainWindow(EMWindow* p_opWindow)
{
	m_opMainAppWindow = p_opWindow;
}

void ExportAudioSettingsDialog::SetParent(EMWindow* p_opWindow)
{
	m_opParentWindow = p_opWindow;
}