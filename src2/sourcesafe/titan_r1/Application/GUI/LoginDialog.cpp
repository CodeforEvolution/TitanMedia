#include "LoginDialog.h"

#include "ColaborationView.h"
#include "EMButton.h"
#include "EMCheckBox.h"
#include "EMFactory.h"
#include "EMLabel.h"
#include "EMMessages.h"
#include "EMSettingIDs.h"
#include "EMSettingsRepository.h"
#include "EMTextControl.h"
#include "EMView.h"
#include "EMWindow.h"

#ifdef PLATFORM_WINDOWS
#include <windows.h> // To get modal dialog to work.
#include "EMWinWindow.h" // To get modal dialog to work.
#endif

const uint32 OK_BUTTON = 1;
const uint32 CANCEL_BUTTON = 2;

LoginDialog::LoginDialog() :
m_opDialogWindow(EMFactory::Instance() -> CreateEMWindow(EMRect(0, 0, 200, 120), "Enter Password", EM_WINDOW_DIALOG)), 
m_opMainView(EMFactory::Instance() -> CreateView(EMRect(0, 0, 200, 120), false)),
m_opUserIDLabel(EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
															EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 3,
															EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 50,
															EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + EM_METRICS_NATIVE_LABEL_HEIGHT),
													 "User ID:", 0)),
m_opUserIDTextControl(EMFactory::Instance() -> CreateTextControl(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 55,
																		EM_METRICS_NATIVE_DIALOG_MARGIN_TOP,
																		EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 55 + 100,
																		EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + EM_METRICS_NATIVE_TEXT_CONTROL_HEIGHT),
																 "1234567890", 0, 0, false, EM_ALIGNMENT_RIGHT, true)),
m_opPasswordLabel(EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
															  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 28,
															  EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 50,
															  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 28 + EM_METRICS_NATIVE_LABEL_HEIGHT),
													   "Password:", 0)),
m_opPasswordTextControl(EMFactory::Instance() -> CreateTextControl(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 55,
																		  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 25,
																		  EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 55 + 100,
																		  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 25 + EM_METRICS_NATIVE_TEXT_CONTROL_HEIGHT),
																   "1234567890", 0, 0, false, EM_ALIGNMENT_LEFT, false, true)),
m_opSavePasswordCheckBox(EMFactory::Instance() ->CreateCheckBox(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
																		EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 50,
																		EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 100,
																		EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 50 + EM_METRICS_NATIVE_CHECKBOX_HEIGHT),
																 "Save Password", 0, 0, 0)),
m_opOK_Button(EMFactory::Instance() -> CreateButton(EMRect(200 - (EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + EM_METRICS_NATIVE_BUTTON_WIDTH * 2 + EM_METRICS_NATIVE_BUTTON_SPACE),
														   120 - (EM_METRICS_NATIVE_DIALOG_MARGIN_BOTTOM + EM_METRICS_NATIVE_BUTTON_HEIGHT),
														   200 - (EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + EM_METRICS_NATIVE_BUTTON_WIDTH + EM_METRICS_NATIVE_BUTTON_SPACE),
														   120 - (EM_METRICS_NATIVE_DIALOG_MARGIN_BOTTOM)),
													"OK", OK_BUTTON, 0, 0, NULL, true)),
m_opCancelButton(EMFactory::Instance() -> CreateButton(EMRect(200 - (EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + EM_METRICS_NATIVE_BUTTON_WIDTH),
															  120 - (EM_METRICS_NATIVE_DIALOG_MARGIN_BOTTOM + EM_METRICS_NATIVE_BUTTON_HEIGHT),
															  200 - (EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE),
															  120 - (EM_METRICS_NATIVE_DIALOG_MARGIN_BOTTOM)),
													   "Cancel", CANCEL_BUTTON, 0, 0))

{
	m_opDialogWindow -> AddChild(m_opMainView);
	m_opMainView -> AddChild(m_opUserIDLabel);
	m_opMainView -> AddChild(m_opUserIDTextControl);
	m_opMainView -> AddChild(m_opPasswordLabel);
	m_opMainView -> AddChild(m_opPasswordTextControl);
	m_opMainView -> AddChild(m_opSavePasswordCheckBox);
	m_opMainView -> AddChild(m_opOK_Button);
	m_opMainView -> AddChild(m_opCancelButton);
}
LoginDialog::~LoginDialog()
{
	delete m_opUserIDLabel;
	delete m_opUserIDTextControl;
	delete m_opPasswordLabel;
	delete m_opPasswordTextControl;
	delete m_opSavePasswordCheckBox;
	delete m_opOK_Button;
	delete m_opCancelButton;
	delete m_opDialogWindow;
	delete m_opMainView;
}

uint64 LoginDialog::ConvertToID_Uint64(const char* p_vpUserID)
{
	string oFirstUserID = p_vpUserID;
	string oSecondUserID = p_vpUserID;
	uint32 vUserID[2];

	oFirstUserID.erase(oFirstUserID.find_first_of('.'), oFirstUserID.length());
	oSecondUserID.erase(0, oSecondUserID.find_first_of('.') + 1);

	vUserID[0] = atoi(oSecondUserID.c_str());
	vUserID[1] = atoi(oFirstUserID.c_str());
	
	return *reinterpret_cast<uint64*>(vUserID);
}
void LoginDialog::ConvertToID_String(uint64 p_vUserID, char* p_vpUserID)
{
	uint32* vUserID = new uint32[2];
	string oUserID;
	char vTemp[40];
	
	memcpy(vUserID, &p_vUserID, sizeof(p_vUserID));
//	vUserID = reinterpret_cast<uint32*>(&p_vUserID);
	oUserID.append(itoa(vUserID[1], vTemp, 10));
	oUserID.append(".");
	oUserID.append(itoa(vUserID[0], vTemp, 10));

	strcpy(p_vpUserID, oUserID.c_str());
	delete [] vUserID;
}

void LoginDialog::DisplayDialog()
{
	// Getting settings
	char vTemp[40];
	ConvertToID_String(*static_cast<int64*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_NETWORK_USER_ID)), vTemp);
	m_opUserIDTextControl -> SetText(vTemp);
	if(*static_cast<bool*>(EMSettingsRepository::Instance() ->GetSetting(SETTING_NETWORK_SAVE_PASSWORD)))
	{
		m_opPasswordTextControl -> SetText(static_cast<string*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_NETWORK_USER_PASSWORD)) -> c_str());
		m_opSavePasswordCheckBox -> Check(true);

	}
	else
	{
		m_opPasswordTextControl -> SetText("");
		m_opSavePasswordCheckBox -> Check(false);
	}

	if(m_opDialogWindow -> Lock())
	{
		m_opDialogWindow -> Show();
#ifdef PLATFORM_WINDOWS
		EnableWindow(dynamic_cast<EMWinWindow*>(m_opDialogWindow) -> GetMainWindowHandle(), FALSE);
		SetFocus(static_cast<HWND>(m_opDialogWindow -> GetNativeView()));
#endif
		m_opDialogWindow -> Unlock();
	}
}

void LoginDialog::HideDialog()
{
	if(m_opDialogWindow -> Lock())
	{
#ifdef PLATFORM_WINDOWS
		EnableWindow(dynamic_cast<EMWinWindow*>(m_opDialogWindow) -> GetMainWindowHandle(), TRUE);
		SetFocus(dynamic_cast<EMWinWindow*>(m_opDialogWindow) -> GetMainWindowHandle());
#endif
		m_opDialogWindow -> Hide();
		m_opDialogWindow -> Unlock();
	}
}

void LoginDialog::Init()
{
	m_opDialogWindow -> AddListener(this);
	m_opMainView -> AddListener(this);
	if(m_opDialogWindow -> Lock())
	{
		//m_opMainView -> SetViewColor(EMColor(192, 192, 192, 255));
		m_opMainView -> SetDefaultColor();
		m_opDialogWindow -> Unlock();
	}
	m_opUserIDTextControl -> Enable(false);

	m_opPasswordTextControl -> AddListener(this);
	m_opSavePasswordCheckBox -> AddListener(this);
	m_opOK_Button -> AddListener(this);
	m_opCancelButton -> AddListener(this);
	this -> AddListener(ColaborationView::Instance());
	m_vMessage = ColaborationView::Instance() -> LoginMessage();

	// TabOrder
	m_opDialogWindow -> AddTabComponent(m_opPasswordTextControl);
	m_opDialogWindow -> AddTabComponent(m_opSavePasswordCheckBox);
	m_opDialogWindow -> AddTabComponent(m_opOK_Button);
	m_opDialogWindow -> AddTabComponent(m_opCancelButton);
}

bool LoginDialog::MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage)
{
	switch(p_vMessage)
	{
	// Buttons, Enter & Esc
	case EM_GUI_COMPONENT_ENTER_PRESSED:
	case OK_BUTTON:
		{
			//Test password here

			string oStringSetting;
			bool vBoolSetting;

			oStringSetting = m_opPasswordTextControl -> Text();
			vBoolSetting = true;
	
			EMSettingsRepository::Instance() -> SetSetting(SETTING_NETWORK_USER_PASSWORD, EM_SETTING_STRING, &oStringSetting, false, true); //So ColaborationView can get the typed password

			Notify(m_vMessage);

			if(m_opSavePasswordCheckBox -> IsChecked())
				vBoolSetting = true;
			else
			{
				// if Save password not checked do not save password, save empty string
				oStringSetting = "";
				EMSettingsRepository::Instance() -> SetSetting(SETTING_NETWORK_USER_PASSWORD, EM_SETTING_STRING, &oStringSetting, false, true);
				vBoolSetting = false;
			}
			EMSettingsRepository::Instance() -> SetSetting(SETTING_NETWORK_SAVE_PASSWORD, EM_SETTING_BOOL, &vBoolSetting, true, true);
			EMSettingsRepository::Instance() -> SaveSystemSettings();
		}
	case EM_GUI_COMPONENT_ESC_PRESSED:
	case CANCEL_BUTTON:
		HideDialog();
		return true;
	// Tab
	case EM_GUI_COMPONENT_TAB_PRESSED:
		m_opDialogWindow -> NextTab();
		return true;
	case EM_GUI_COMPONENT_SHIFT_TAB_PRESSED:
		m_opDialogWindow -> PreviousTab();
		return true;
	case EM_GUI_COMPONENT_GOT_FOCUS:
		if(p_opSender == m_opDialogWindow)
			m_opPasswordTextControl -> SetFocus();
		m_opDialogWindow -> SetCurrentFocus();
		return true;
	// Misc
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

void LoginDialog::MoveDialog()
{
	EMRect oWindowRect(m_opParentWindow -> Frame());
	EMRect oDialogRect(m_opDialogWindow -> Frame());
	float vX(oWindowRect.m_vRight - oWindowRect.GetWidth() / 2 - oDialogRect.GetWidth() / 2);
	float vY(oWindowRect.m_vBottom - oWindowRect.GetHeight() / 2 - oDialogRect.GetHeight() / 2);

	MoveDialog(vX, vY);
}

void LoginDialog::MoveDialog(float p_vX, float p_vY)
{
	m_opDialogWindow -> MoveTo(p_vX, p_vY);
}

void LoginDialog::SetParent(EMWindow* p_opWindow)
{
	m_opParentWindow = p_opWindow;
}
