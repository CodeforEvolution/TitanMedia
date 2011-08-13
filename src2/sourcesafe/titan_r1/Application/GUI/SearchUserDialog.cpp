#include "SearchUserDialog.h"

#include "ColaborationView.h"
#include "CommandIDs.h"
#include "EMButton.h"
#include "EMFactory.h"
#include "EMLabel.h"
#include "EMListViewControl.h"
#include "EMMessages.h"
#include "EMNetworkEngine.h"
#include "EMSettingIDs.h"
#include "EMSettingsRepository.h"
#include "EMTextControl.h"
#include "EMView.h"
#include "EMWindow.h"
#include "SystemSettingsDialog.h"

#include <string>

#ifdef PLATFORM_WINDOWS
#include <windows.h> // To get modal dialog to work.
#include "EMWinWindow.h" // To get modal dialog to work.
#endif

const uint32 SEARCH_BUTTON = 1;
const uint32 CHOOSE_BUTTON = 2;
const uint32 CLOSE_BUTTON = 3;

SearchUserDialog::SearchUserDialog() :
m_opDialogWindow(EMFactory::Instance() -> CreateEMWindow(EMRect(0, 0, 230, 250), "Search/Add User", EM_WINDOW_DIALOG)), 
m_opMainView(EMFactory::Instance() -> CreateView(EMRect(0, 0, 230, 250), 0)),
m_opNicknameLabel(EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
																 EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 3,
																 EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 52,
																 EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 3 + EM_METRICS_NATIVE_LABEL_HEIGHT),
														  "Nickname:", 0)),
/*m_opUserIDLabel(EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
															  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 28,
															  EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 52,
															  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 28 + EM_METRICS_NATIVE_LABEL_HEIGHT),
													   "UserID:", 0)),
*/
m_opResultLabel(EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
															  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 58,
															  EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 52,
															  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 58 + EM_METRICS_NATIVE_LABEL_HEIGHT),
													   "Result:", 0)),
m_opNicknameTextControl(EMFactory::Instance() -> CreateTextControl(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 57,
																			 EM_METRICS_NATIVE_DIALOG_MARGIN_TOP,
																			 EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 57 + 153,
																			 EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + EM_METRICS_NATIVE_TEXT_CONTROL_HEIGHT),
																	  "", 0, 0, false)),
/*m_opUserIDTextControl(EMFactory::Instance() -> CreateTextControl(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 57,
																		  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 25,
																		  EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 57 + 153,
																		  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 25 + EM_METRICS_NATIVE_TEXT_CONTROL_HEIGHT),
																   "", 0, 0, false)),
*/
m_opSearchButton(EMFactory::Instance() -> CreateButton(EMRect(230 - (EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + EM_METRICS_NATIVE_BUTTON_WIDTH),
															  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 50,
															  230 - (EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE),
															  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 50 + EM_METRICS_NATIVE_BUTTON_HEIGHT),
													   "Search", SEARCH_BUTTON, 0, 0, NULL, true)),
m_opResultListViewControl(EMFactory::Instance() ->CreateListViewControl(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
																				EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 78,
																				230 - EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
																				250 - (EM_METRICS_NATIVE_DIALOG_MARGIN_BOTTOM + EM_METRICS_NATIVE_BUTTON_HEIGHT + EM_METRICS_NATIVE_BUTTON_SPACE)),
																				"Result", 0, 0, 0, EM_LIST_VIEW_STYLE_REPORT)),
m_opChooseButton(EMFactory::Instance() -> CreateButton(EMRect(230 - (EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + EM_METRICS_NATIVE_BUTTON_WIDTH * 2 + EM_METRICS_NATIVE_BUTTON_SPACE),
														   250 - (EM_METRICS_NATIVE_DIALOG_MARGIN_BOTTOM + EM_METRICS_NATIVE_BUTTON_HEIGHT),
														   230 - (EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + EM_METRICS_NATIVE_BUTTON_WIDTH + EM_METRICS_NATIVE_BUTTON_SPACE),
														   250 - (EM_METRICS_NATIVE_DIALOG_MARGIN_BOTTOM)),
													"Choose", CHOOSE_BUTTON, 0, 0)),
m_opCloseButton(EMFactory::Instance() -> CreateButton(EMRect(230 - (EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + EM_METRICS_NATIVE_BUTTON_WIDTH),
															  250 - (EM_METRICS_NATIVE_DIALOG_MARGIN_BOTTOM + EM_METRICS_NATIVE_BUTTON_HEIGHT),
															  230 - (EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE),
															  250 - (EM_METRICS_NATIVE_DIALOG_MARGIN_BOTTOM)),
													   "Close", CLOSE_BUTTON, 0, 0))

{
	m_opDialogWindow -> AddChild(m_opMainView);
	m_opMainView -> AddChild(m_opNicknameLabel);
//	m_opMainView -> AddChild(m_opUserIDLabel);
	m_opMainView -> AddChild(m_opResultLabel);
	m_opMainView -> AddChild(m_opNicknameTextControl);
//	m_opMainView -> AddChild(m_opUserIDTextControl);
	m_opMainView -> AddChild(m_opSearchButton);
	m_opMainView -> AddChild(m_opResultListViewControl);
	m_opMainView -> AddChild(m_opChooseButton);
	m_opMainView -> AddChild(m_opCloseButton);

}

SearchUserDialog::~SearchUserDialog()
{
	delete m_opNicknameLabel;
//	delete m_opUserIDLabel;
	delete m_opResultLabel;
	delete m_opNicknameTextControl;
//	delete m_opUserIDTextControl;
	delete m_opSearchButton;
	delete m_opResultListViewControl;
	delete m_opChooseButton;
	delete m_opCloseButton;

	delete m_opMainView;
	delete m_opDialogWindow;
}

uint64 SearchUserDialog::ConvertToID_Uint64(const char* p_vpUserID)
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
void SearchUserDialog::ConvertToID_String(uint64 p_vUserID, char* p_vpUserID)
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

void SearchUserDialog::DisplayDialog()
{
	EMNetworkEngine::Instance() -> AddListener(this);
	m_opResultListViewControl -> DeleteAllItems();
	m_vNumberOfUsersFound = 0;
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

void SearchUserDialog::HideDialog()
{
	if(m_opDialogWindow -> Lock())
	{
#ifdef PLATFORM_WINDOWS
		EnableWindow(dynamic_cast<EMWinWindow*>(m_opDialogWindow) -> GetMainWindowHandle(), TRUE);
		SetFocus(dynamic_cast<EMWinWindow*>(m_opDialogWindow) -> GetMainWindowHandle());
#endif
		m_opDialogWindow -> Hide();
		m_opDialogWindow -> Unlock();
		EMNetworkEngine::Instance() -> RemoveListener(this);
	}
}

void SearchUserDialog::Init()
{
	m_opDialogWindow -> AddListener(this);
	m_opMainView -> AddListener(this);
	if(m_opDialogWindow -> Lock())
	{
		//m_opMainView -> SetViewColor(EMColor(0, 255, 0, 255));
		m_opMainView -> SetDefaultColor();
		m_opDialogWindow -> Unlock();
	}
	m_opResultListViewControl -> AddColumn("Nickname", m_opResultListViewControl -> Frame().GetWidth() / 2 - 1);
	m_opResultListViewControl -> AddColumn("UserID", m_opResultListViewControl -> Frame().GetWidth() / 2 - 1);

	m_opNicknameTextControl -> AddListener(this);
//	m_opUserIDTextControl -> AddListener(this);
	m_opSearchButton -> AddListener(this);
	m_opResultListViewControl -> AddListener(this);
	m_opChooseButton -> AddListener(this);
	m_opCloseButton -> AddListener(this);

	// Tab order
	m_opDialogWindow -> AddTabComponent(m_opNicknameTextControl);
//	m_opDialogWindow -> AddTabComponent(m_opUserIDTextControl);
	m_opDialogWindow -> AddTabComponent(m_opSearchButton);
	m_opDialogWindow -> AddTabComponent(m_opResultListViewControl);
	m_opDialogWindow -> AddTabComponent(m_opChooseButton);
	m_opDialogWindow -> AddTabComponent(m_opCloseButton);
	
}

bool SearchUserDialog::MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage)
{
	switch(p_vMessage)
	{
	case EM_GUI_COMPONENT_ENTER_PRESSED:
	case SEARCH_BUTTON:
		{
			uint64 vUserIDConverted(0);
			bool vOnline(false);

			m_opResultListViewControl -> DeleteAllItems();
			m_vNumberOfUsersFound = 0;
			m_opResultListViewControl -> AddItem("Searching...");
			EMCommandRepository::Instance() -> ExecuteCommand(NETWORK_COMMAND_FIND_USER, &vUserIDConverted, strlen(m_opNicknameTextControl -> Text()) == 0 ? NULL : const_cast<char*>(m_opNicknameTextControl -> Text()), &vOnline);
		return true;
		}
	case EM_NETWORK_USER_FOUND:
		{
			char vNick[80];
			uint64 vUserID = *static_cast<uint64*>(EMCommandRepository::Instance() -> ExecuteCommand(NETWORK_INFO_COMMAND_GET_NEXT_FOUND_USER, const_cast<char*>(vNick)));
			if(m_vNumberOfUsersFound == 0)
				m_opResultListViewControl -> DeleteItem(0);
			else
				m_opResultListViewControl -> DeleteItem(m_vNumberOfUsersFound);
			m_opResultListViewControl -> AddItem(vNick);
			ConvertToID_String(vUserID, vNick);
			m_opResultListViewControl -> AddSubItem(vNick, 1);
			m_opResultListViewControl -> AddItem("Searching...");
			m_vNumberOfUsersFound++;
		}
		return true;
	case EM_NETWORK_USER_SEARCH_ENDED:
		if(m_vNumberOfUsersFound == 0)
		{
			m_opResultListViewControl -> DeleteItem(0);
			m_opResultListViewControl -> AddItem(" > No User Found <");
		}
		return true;
	case CHOOSE_BUTTON:
		{
			if(m_vNumberOfUsersFound > 0)
				m_opSystemSettingsDialog -> SetUserIDString(m_opResultListViewControl -> GetSelectionSubItem(1));
			HideDialog();
			return true;
		}
	case EM_GUI_COMPONENT_ESC_PRESSED:
	case CLOSE_BUTTON:
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
			m_opNicknameTextControl -> SetFocus();
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

void SearchUserDialog::MoveDialog()
{
	EMRect oWindowRect(m_opMainAppWindow -> Frame());
	EMRect oDialogRect(m_opDialogWindow -> Frame());
	float vX(oWindowRect.m_vRight - oWindowRect.GetWidth() / 2 - oDialogRect.GetWidth() / 2);
	float vY(oWindowRect.m_vBottom - oWindowRect.GetHeight() / 2 - oDialogRect.GetHeight() / 2);

	MoveDialog(vX, vY);
}

void SearchUserDialog::MoveDialog(float p_vX, float p_vY)
{
	m_opDialogWindow -> MoveTo(p_vX, p_vY);
}

void SearchUserDialog::SetMainWindow(EMWindow* p_opWindow)
{
	m_opMainAppWindow = p_opWindow;
}

void SearchUserDialog::SetParent(EMWindow* p_opWindow)
{
	m_opParentWindow = p_opWindow;
}

void SearchUserDialog::SetSystemSettingsDialog(void* p_opSystemSettingsDialog)
{
	m_opSystemSettingsDialog = static_cast<SystemSettingsDialog*>(p_opSystemSettingsDialog);
}
