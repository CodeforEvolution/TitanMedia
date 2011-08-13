#include "ColaborationView.h"

#include "CommandIDs.h"
#include "DirectoryIDs.h"
#include "EMButton.h"
#include "EMCommandRepository.h"
#include "EMComboBox.h"
#include "EMDragDrop.h"
#include "EMFactory.h"
#include "EMLabel.h"
#include "EMListViewControl.h"
#include "EMMessageBox.h"
#include "EMMessages.h"
#include "EMNetworkEngine.h"
#include "EMSettingIDs.h"
#include "EMSettingsRepository.h"
#include "EMTabControl.h"
#include "EMTextControl.h"
#define SONORK_CLIENT_BUILD // To get sonork to work with Titan
#include "EMUserInfo.h"
#include "EMView.h"
#include "TrackWindow.h"
#include "ProjectManager.h"

ColaborationView* ColaborationView::m_opInstance = NULL;

const uint32 ADD_BUTTON_PRESSED = 1;
const uint32 ASSIGN_BUTTON_PRESSED = 2;
const uint32 CREATE_BUTTON_PRESSED = 3;
const uint32 NAME_REMOVE_BUTTON_PRESSED = 4;
const uint32 PROJECT_REMOVE_BUTTON_PRESSED = 5;
const uint32 SEND_TO_COMBO_BOX_SELECTION_CHANGED = 6;
const uint32 CONNECT_BUTTON_PRESSED = 7;
const uint32 SAVE_FILE = 8;
const uint32 OK_TO_CONNECT = 9;

#define COL_MARGIN_SIDE 3
#define COL_MARGIN_TOP 3
#define COL_MARGIN_BOTTOM 3

ColaborationView::ColaborationView() : 
m_opMainView(EMFactory::Instance() -> CreateView(EMRect(0, 0, 200, 200), EM_FOLLOW_LEFT | EM_FOLLOW_TOP)),
m_opTabControl(EMFactory::Instance() -> CreateTabControl(EMRect(0, 0, 200, 200), "ListView", false, 0, EM_FOLLOW_LEFT | EM_FOLLOW_TOP, 0)),
m_opChatView(EMFactory::Instance() -> CreateView(EMRect(0, 0, 0, 0), EM_FOLLOW_LEFT | EM_FOLLOW_TOP, false)),
m_opSendToLabel(EMFactory::Instance() -> CreateLabel(EMRect(COL_MARGIN_SIDE,
															COL_MARGIN_TOP + 4,
															COL_MARGIN_SIDE + 47,
															COL_MARGIN_TOP + EM_METRICS_NATIVE_LABEL_HEIGHT + 4),
														"Send To:", 0)),
m_opSendToComboBox(EMFactory::Instance() ->CreateComboBox(EMRect(COL_MARGIN_SIDE + 52,
																 COL_MARGIN_TOP,
																 COL_MARGIN_SIDE + 150,
																 COL_MARGIN_TOP + 100),
														  SEND_TO_COMBO_BOX_SELECTION_CHANGED, 0, false)),
m_opChatListViewControl(EMFactory::Instance() -> CreateListViewControl(EMRect(COL_MARGIN_SIDE,
																			  COL_MARGIN_TOP + 25,
																			  COL_MARGIN_SIDE + 325,
																			  COL_MARGIN_TOP + 156),
																	   "ColaborationView", 0, EM_FOLLOW_RIGHT | EM_FOLLOW_BOTTOM, 0, EM_LIST_VIEW_STYLE_REPORT)),
m_opChatTextControl(EMFactory::Instance() -> CreateTextControl(EMRect(COL_MARGIN_SIDE,
																			  COL_MARGIN_TOP + 190,
																			  COL_MARGIN_SIDE + 325,
																			  COL_MARGIN_TOP + 190 + EM_METRICS_NATIVE_TEXT_CONTROL_HEIGHT),
																	   "Type your message here", 0, EM_FOLLOW_RIGHT | EM_FOLLOW_BOTTOM, 0)),

m_opMemberView(EMFactory::Instance() -> CreateView(EMRect(0, 0, 0, 0), 0, false)),
m_opProjectLabel(EMFactory::Instance() -> CreateLabel(EMRect(COL_MARGIN_SIDE,
															COL_MARGIN_TOP + 2,
															COL_MARGIN_SIDE + 38,
															COL_MARGIN_TOP + EM_METRICS_NATIVE_LABEL_HEIGHT),
														"Project:", 0)),
m_opProjectComboBox(EMFactory::Instance() -> CreateComboBox(EMRect(COL_MARGIN_SIDE + 43,
																 COL_MARGIN_TOP,
																 COL_MARGIN_SIDE + 150,
																 COL_MARGIN_TOP + 100),
														  0, 0, false)),
m_opCreateButton(EMFactory::Instance() -> CreateButton(EMRect(COL_MARGIN_SIDE +170,
															  COL_MARGIN_TOP,
															  COL_MARGIN_SIDE + 170 + EM_METRICS_NATIVE_BUTTON_WIDTH,
															  COL_MARGIN_TOP + EM_METRICS_NATIVE_BUTTON_HEIGHT),
													   "Create...", CREATE_BUTTON_PRESSED, 0, 0)),
m_opProjectRemoveButton(EMFactory::Instance() -> CreateButton(EMRect(COL_MARGIN_SIDE + 170 + EM_METRICS_NATIVE_BUTTON_WIDTH + EM_METRICS_NATIVE_BUTTON_SPACE,
															  COL_MARGIN_TOP,
															  COL_MARGIN_SIDE + 170 + EM_METRICS_NATIVE_BUTTON_WIDTH + EM_METRICS_NATIVE_BUTTON_SPACE + EM_METRICS_NATIVE_BUTTON_WIDTH,
															  COL_MARGIN_TOP + EM_METRICS_NATIVE_BUTTON_HEIGHT),
													   "Remove", PROJECT_REMOVE_BUTTON_PRESSED, 0, 0)),
m_opMemberListViewControl(EMFactory::Instance() -> CreateListViewControl(EMRect(COL_MARGIN_SIDE,
																			  COL_MARGIN_TOP/* + 30*/,
																			  COL_MARGIN_SIDE + 325,
																			  COL_MARGIN_TOP + 150),
																	   "ColaborationView", 0, EM_FOLLOW_RIGHT | EM_FOLLOW_BOTTOM, 0, EM_LIST_VIEW_STYLE_REPORT)),
m_opConnectButton(EMFactory::Instance() -> CreateButton(EMRect(COL_MARGIN_SIDE,
															  COL_MARGIN_TOP + 190,
															  COL_MARGIN_SIDE + EM_METRICS_NATIVE_BUTTON_WIDTH,
															  COL_MARGIN_TOP + 190 + EM_METRICS_NATIVE_BUTTON_HEIGHT),
													  "Connect", CONNECT_BUTTON_PRESSED, 0, 0)),
m_opAssignButton(EMFactory::Instance() -> CreateButton(EMRect(COL_MARGIN_SIDE,
															  COL_MARGIN_TOP + 190,
															  COL_MARGIN_SIDE + EM_METRICS_NATIVE_BUTTON_WIDTH,
															  COL_MARGIN_TOP + 190 + EM_METRICS_NATIVE_BUTTON_HEIGHT),
													  "Assign", ASSIGN_BUTTON_PRESSED, 0, 0)),
m_opAddButton(EMFactory::Instance() -> CreateButton(EMRect(COL_MARGIN_SIDE + EM_METRICS_NATIVE_BUTTON_WIDTH + EM_METRICS_NATIVE_BUTTON_SPACE,
														   COL_MARGIN_TOP + 190,
														   COL_MARGIN_SIDE + EM_METRICS_NATIVE_BUTTON_WIDTH + EM_METRICS_NATIVE_BUTTON_SPACE + EM_METRICS_NATIVE_BUTTON_WIDTH,
														   COL_MARGIN_TOP + 190 + EM_METRICS_NATIVE_BUTTON_HEIGHT),
													"Add...", ADD_BUTTON_PRESSED, 0, 0)),
m_opNameRemoveButton(EMFactory::Instance() -> CreateButton(EMRect(COL_MARGIN_SIDE + 2 * (EM_METRICS_NATIVE_BUTTON_SPACE + EM_METRICS_NATIVE_BUTTON_WIDTH),
																  COL_MARGIN_TOP + 190,
																  COL_MARGIN_SIDE + EM_METRICS_NATIVE_BUTTON_WIDTH + 2 * (EM_METRICS_NATIVE_BUTTON_SPACE + EM_METRICS_NATIVE_BUTTON_WIDTH),
																  COL_MARGIN_TOP + 190 + EM_METRICS_NATIVE_BUTTON_HEIGHT),
														   "Remove", NAME_REMOVE_BUTTON_PRESSED, 0, 0)),
m_opMsgBox(EMFactory::Instance() -> CreateMessageBox()),
m_vpUserIDToSend(NULL),
m_opFileWindow(NULL),
m_vAlreadySet(false)
{

}

ColaborationView::~ColaborationView()
{
	if(m_vpUserIDToSend != NULL)
		delete [] m_vpUserIDToSend;

	delete m_opMainView;
	delete m_opTabControl;
	delete m_opChatView;
	delete m_opMemberView;
	delete m_opSendToLabel;
	delete m_opCreateButton;
	delete m_opProjectRemoveButton;
	delete m_opNameRemoveButton;
	delete m_opAddButton;
	delete m_opAssignButton;
	delete m_opChatListViewControl;
	delete m_opChatTextControl;
}

void ColaborationView::AddUser(char* p_vpUserName, uint64 p_vUserID, uint32 p_vUserStatus)
{
	char vTemp[255];
	m_opMemberListViewControl -> AddItem(p_vpUserName);
	m_opSendToComboBox -> AddString(p_vpUserName);
	m_oComboBoxIDUserIDMap[m_opSendToComboBox -> GetNrItems() - 1] = p_vUserID;
	m_oUserIDUserNameMap[p_vUserID] = p_vpUserName;
	if(p_vUserStatus != 0)
		m_opMemberListViewControl -> AddSubItem("Yes", 1);
	else
		m_opMemberListViewControl -> AddSubItem("No", 1);
	m_opMemberListViewControl -> AddSubItem(_ui64toa(p_vUserID, vTemp, 10), 2);
}

uint64 ColaborationView::ConvertToID_Uint64(const char* p_vpUserID)
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
void ColaborationView::ConvertToID_String(uint64 p_vUserID, char* p_vpUserID)
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

bool ColaborationView::DeleteUser()
{
	uint64 vUserIDToDelete(_atoi64( m_opMemberListViewControl -> GetSelectionSubItem(-1)));
	string oMessage("Do you want to delete user: ");
	oMessage.append(m_oUserIDUserNameMap[vUserIDToDelete]);
	if(m_opMsgBox ->DisplayDialog(m_opMainView -> GetNativeView(), oMessage.c_str(), "Delete user?", EM_DIALOG_BUTTON_YES_NO | EM_DIALOG_ICON_QUESTION) == EM_DIALOG_NO_BUTTON)
		return false;
	EMCommandRepository::Instance() -> ExecuteCommand(NETWORK_COMMAND_DELETE_USER, &vUserIDToDelete);
/*	uint64 vSelectedSendToUserID(0);
	if(m_opSendToComboBox -> GetSelectionIndex() != 0)
		vSelectedSendToUserID = m_oComboBoxIDUserIDMap[m_opSendToComboBox -> GetSelectionIndex()];
	if(vSelectedSendToUserID == vUserIDToDelete)
		vSelectedSendToUserID = 0;
	m_oUserIDUserNameMap.erase(m_oUserIDUserNameMap.find(vUserIDToDelete));
	//m_oUserIDComboBoxIDMap.clear();
	m_oComboBoxIDUserIDMap.erase(m_oComboBoxIDUserIDMap.begin(), m_oComboBoxIDUserIDMap.end());
	m_opSendToComboBox -> Clear();

	m_opSendToComboBox -> AddString("< ALL >");
	int vNumberOfItems = m_opMemberListViewControl -> GetItemCount();
	uint64 vUserID(0);
	int8 vOffset(1);
	for(int i = 0; i < vNumberOfItems; i++)
	{
		vUserID = _atoi64(m_opMemberListViewControl -> GetSubItem(i, -1));
		if(vUserID != vUserIDToDelete)
		{
			m_oComboBoxIDUserIDMap[i + vOffset] = vUserID;
			m_opSendToComboBox -> AddString(m_opMemberListViewControl -> GetSubItem(i, 0));
			if(vUserID == vSelectedSendToUserID)
				m_opSendToComboBox -> SelectItem(i + vOffset);
		}
		else
			vOffset = 0;
	}
	if(vSelectedSendToUserID == 0)
		m_opSendToComboBox -> SelectItem(0);
	m_opMemberListViewControl -> DeleteSelectedItem();
*/
	return true;
}

void ColaborationView::EnableColaborationUI(bool p_vEnable)
{
	m_opCreateButton -> Enable(p_vEnable);
	m_opProjectRemoveButton -> Enable(p_vEnable);
	m_opAddButton -> Enable(p_vEnable);
	m_opAssignButton -> Enable(p_vEnable);
	m_opNameRemoveButton -> Enable(p_vEnable);
	m_opChatTextControl -> Enable(p_vEnable);
	m_opSendToComboBox -> Enable(p_vEnable);
	m_opAssignButton -> Enable(p_vEnable);
	m_opMemberListViewControl -> Enable(p_vEnable);
	m_opChatListViewControl -> Enable(p_vEnable);
	if(p_vEnable)
		m_opConnectButton -> SetLabel("Disconnect");
	else
		m_opConnectButton -> SetLabel("Connect");
}

EMView* ColaborationView::GetView()
{
	return m_opMainView;
}

void ColaborationView::Hide()
{
}

void ColaborationView::Init()
{
	m_opMainView -> AddChild(m_opTabControl);
	m_opTabControl -> AddChild(m_opChatView);
	m_opTabControl -> AddChild(m_opMemberView);

	m_opMainView -> AddListener(this);
	m_opChatView -> AddListener(this);
	m_opCreateButton -> AddListener(this);
	m_opProjectRemoveButton -> AddListener(this);
	m_opAddButton -> AddListener(this);
	m_opAssignButton -> AddListener(this);
	m_opNameRemoveButton -> AddListener(this);
	m_opChatTextControl -> AddListener(this);
	m_opSendToComboBox -> AddListener(this);
	m_opMemberListViewControl -> AddListener(this);
	m_opConnectButton -> AddListener(this);

	m_opChatView -> AddChild(m_opSendToLabel);
	m_opChatView -> AddChild(m_opSendToComboBox);
	m_opChatView -> AddChild(m_opChatListViewControl);
	m_opChatView -> AddChild(m_opChatTextControl);

//	m_opMemberView -> AddChild(m_opProjectLabel);
//	m_opMemberView -> AddChild(m_opProjectComboBox);
//	m_opMemberView -> AddChild(m_opCreateButton);
//	m_opMemberView -> AddChild(m_opProjectRemoveButton);
	m_opMemberView -> AddChild(m_opMemberListViewControl);
	m_opMemberView -> AddChild(m_opConnectButton);
//	m_opMemberView -> AddChild(m_opAssignButton);
	m_opMemberView -> AddChild(m_opAddButton);
	m_opMemberView -> AddChild(m_opNameRemoveButton);

	m_opChatListViewControl -> AddColumn("Name", 75);
	m_opChatListViewControl -> AddColumn("Message", 200);
	m_opChatListViewControl -> AddColumn("Sent To", 75);
	m_opChatListViewControl -> SetUniqueIndexColumn(-1); // Sort disabled

	m_opMemberListViewControl -> AddColumn("Name", 75);
	m_opMemberListViewControl -> AddColumn("Online", 75, EM_ALIGNMENT_CENTER);
//	m_opMemberListViewControl -> AddColumn("Assigned", 0);
//	m_opMemberListViewControl -> AddColumn("In Project", 0);
	m_opMemberListViewControl -> AddColumn("User ID", 0, EM_ALIGNMENT_RIGHT);
	m_opMemberListViewControl -> SetUniqueIndexColumn(2);

	m_opMemberView -> AddListener(this);
//	m_opView -> SetViewColor(EMColor(192, 192, 192, 255));
	m_opChatView -> SetViewColor(EMColor(0, 192, 192, 255));
	m_opMainView -> SetDefaultColor();
//	m_opMemberView -> SetDefaultColor();
	m_opChatView -> SetDefaultColor();
	m_opTabControl -> AddTab("Member", m_opMemberView);
	m_opTabControl -> AddTab("Chat", m_opChatView);

	// This enables network colaboration. From here to...
	if(EMNetworkEngine::Instance() -> IsConnected())
	{
		eo << "Connected" << ef;
/*		int32 vNumberOfUsers(*(static_cast<int32*>(EMCommandRepository::Instance() -> ExecuteCommand(NETWORK_INFO_COMMAND_NUMBER_OF_USERS, NULL, NULL, NULL))));
		uint64* vpUserID = EM_new uint64[sizeof(uint64) * vNumberOfUsers];
		uint32* vpUserStatus = EM_new uint32[sizeof(uint32) * vNumberOfUsers];
		char** vppUserNames = EM_new char*[sizeof(char*) * vNumberOfUsers];

		EMCommandRepository::Instance() -> ExecuteCommand(NETWORK_INFO_COMMAND_GET_USER_INFOS, vpUserID, vppUserNames, vpUserStatus);

		m_opSendToComboBox -> AddString("< ALL >");
		m_opSendToComboBox -> SelectItem(0);

//		char vTemp[255];

		for(int i = 0; i < vNumberOfUsers; i++)
		{
/*			m_opMemberListViewControl -> AddItem(vppUserNames[i]);
			m_opSendToComboBox -> AddString(vppUserNames[i]);
			m_oComboBoxIDUserIDMap[i + 1] = vpUserID[i];
			//m_oUserIDComboBoxIDMap[vpUserID[i]] = i + 1;
			m_oUserIDUserNameMap[vpUserID[i]] = vppUserNames[i];
			if(vpUserStatus[i] != 0)
				m_opMemberListViewControl -> AddSubItem("Yes", 1);
			else
				m_opMemberListViewControl -> AddSubItem("No", 1);
			m_opMemberListViewControl -> AddSubItem(_ui64toa(vpUserID[i], vTemp, 10), 2);
*			AddUser(vppUserNames[i], vpUserID[i], vpUserStatus[i]);
		}
		delete [] vpUserID;
		delete [] vpUserStatus;
		delete [] vppUserNames;
		EnableColaborationUI(true);
*/		m_opSendToComboBox -> Notify(SEND_TO_COMBO_BOX_SELECTION_CHANGED);
	}
	else
	{
		eo << " *** NOT CONNECTED!!!! ***" << ef;
		EnableColaborationUI(false);
	}

	EMNetworkEngine::Instance() -> AddListener(this);
	
/*	int32 vNumberOfUsers(*(static_cast<int32*>(EMCommandRepository::Instance() -> ExecuteCommand(NETWORK_INFO_COMMAND_NUMBER_OF_USERS, NULL, NULL, NULL))));
	uint64* vpUserID = EM_new uint64[sizeof(uint64) * vNumberOfUsers];
	uint32* vpUserStatus = EM_new uint32[sizeof(uint32) * vNumberOfUsers];
	char** vppUserNames = EM_new char*[sizeof(char*) * vNumberOfUsers];

	EMCommandRepository::Instance() -> ExecuteCommand(NETWORK_INFO_COMMAND_GET_USER_INFOS, vpUserID, vppUserNames, vpUserStatus);

	m_opSendToComboBox -> AddString("< ALL >");
	m_opSendToComboBox -> SelectItem(0);

	char vTemp[255];

	for(int i = 0; i < vNumberOfUsers; i++)
	{
		m_opMemberListViewControl -> AddItem(vppUserNames[i]);
		m_opSendToComboBox -> AddString(vppUserNames[i]);
		m_oComboBoxIDUserIDMap[i + 1] = vpUserID[i];
		//m_oUserIDComboBoxIDMap[vpUserID[i]] = i + 1;
		m_oUserIDUserNameMap[vpUserID[i]] = vppUserNames[i];
		if(vpUserStatus[i] != 0)
			m_opMemberListViewControl -> AddSubItem("Yes", 1);
		else
			m_opMemberListViewControl -> AddSubItem("No", 1);
		m_opMemberListViewControl -> AddSubItem(_ui64toa(vpUserID[i], vTemp, 10), 4);

	}
	delete [] vpUserID;
	delete [] vpUserStatus;
	delete [] vppUserNames;
*/	
	// ...here
	
/*	// This disables network colaboration. From here to...
	m_opConnectButton -> Enable(false);
	EnableColaborationUI(false);
*/	// ...here
	EMDragDrop::Instance() -> RegisterTarget(m_opMemberListViewControl, EM_LIST_VIEW_CONTROL);
	m_opSendToComboBox -> Notify(SEND_TO_COMBO_BOX_SELECTION_CHANGED);
}

ColaborationView* ColaborationView::Instance()
{
	if(m_opInstance)
		return m_opInstance;
	return m_opInstance = EM_new ColaborationView;
}

void ColaborationView::LayoutViews()
{
	EMRect oRect;

	m_opTabControl -> SetFrame(m_opMainView -> Bounds());

	oRect = m_opChatView -> Frame();

	m_opChatListViewControl ->SetFrame(EMRect(COL_MARGIN_SIDE,
											  COL_MARGIN_TOP + 25,
											  oRect.m_vRight - oRect.m_vLeft - COL_MARGIN_SIDE,
											  oRect.m_vBottom - oRect.m_vTop - (COL_MARGIN_BOTTOM + EM_METRICS_NATIVE_BUTTON_HEIGHT + 3)));

	m_opChatTextControl ->SetFrame(EMRect(COL_MARGIN_SIDE,
										  oRect.m_vBottom - oRect.m_vTop - (COL_MARGIN_BOTTOM + EM_METRICS_NATIVE_TEXT_CONTROL_HEIGHT),
										  oRect.m_vRight - oRect.m_vLeft - COL_MARGIN_SIDE,
										  oRect.m_vBottom - oRect.m_vTop - COL_MARGIN_BOTTOM));

	oRect = m_opMemberView -> Frame();

	m_opCreateButton -> SetFrame(EMRect(oRect.m_vRight - oRect.m_vLeft - (COL_MARGIN_SIDE + EM_METRICS_NATIVE_BUTTON_WIDTH + EM_METRICS_NATIVE_BUTTON_SPACE + EM_METRICS_NATIVE_BUTTON_WIDTH),
									 COL_MARGIN_BOTTOM,
									 oRect.m_vRight - oRect.m_vLeft - (COL_MARGIN_SIDE + EM_METRICS_NATIVE_BUTTON_SPACE + EM_METRICS_NATIVE_BUTTON_WIDTH),
									 COL_MARGIN_BOTTOM + EM_METRICS_NATIVE_BUTTON_HEIGHT));
	
	m_opProjectRemoveButton -> SetFrame(EMRect(oRect.m_vRight - oRect.m_vLeft - (COL_MARGIN_SIDE + EM_METRICS_NATIVE_BUTTON_WIDTH),
											COL_MARGIN_BOTTOM,
											oRect.m_vRight - oRect.m_vLeft - COL_MARGIN_SIDE,
											COL_MARGIN_BOTTOM + EM_METRICS_NATIVE_BUTTON_HEIGHT));

	m_opMemberListViewControl ->SetFrame(EMRect(COL_MARGIN_SIDE,
											  COL_MARGIN_TOP/* + 30*/,
											  oRect.m_vRight - oRect.m_vLeft - COL_MARGIN_SIDE,
											  oRect.m_vBottom - oRect.m_vTop - (COL_MARGIN_BOTTOM + EM_METRICS_NATIVE_BUTTON_HEIGHT + 8)));

	m_opConnectButton -> SetFrame(EMRect(COL_MARGIN_SIDE,
										oRect.m_vBottom - oRect.m_vTop - (COL_MARGIN_BOTTOM + EM_METRICS_NATIVE_BUTTON_HEIGHT),
										COL_MARGIN_SIDE + EM_METRICS_NATIVE_BUTTON_WIDTH,
										oRect.m_vBottom - oRect.m_vTop - COL_MARGIN_BOTTOM));

	m_opAssignButton -> SetFrame(EMRect(oRect.m_vRight - oRect.m_vLeft - (COL_MARGIN_SIDE + EM_METRICS_NATIVE_BUTTON_WIDTH + 2 * (EM_METRICS_NATIVE_BUTTON_SPACE + EM_METRICS_NATIVE_BUTTON_WIDTH)),
										oRect.m_vBottom - oRect.m_vTop - (COL_MARGIN_BOTTOM + EM_METRICS_NATIVE_BUTTON_HEIGHT),
										oRect.m_vRight - oRect.m_vLeft - (COL_MARGIN_SIDE + 2 * (EM_METRICS_NATIVE_BUTTON_SPACE + EM_METRICS_NATIVE_BUTTON_WIDTH)),
										oRect.m_vBottom - oRect.m_vTop - COL_MARGIN_BOTTOM));
	
	m_opAddButton -> SetFrame(EMRect(oRect.m_vRight - oRect.m_vLeft - (COL_MARGIN_SIDE + EM_METRICS_NATIVE_BUTTON_WIDTH + EM_METRICS_NATIVE_BUTTON_SPACE + EM_METRICS_NATIVE_BUTTON_WIDTH),
									 oRect.m_vBottom - oRect.m_vTop - (COL_MARGIN_BOTTOM + EM_METRICS_NATIVE_BUTTON_HEIGHT),
									 oRect.m_vRight - oRect.m_vLeft - (COL_MARGIN_SIDE + EM_METRICS_NATIVE_BUTTON_SPACE + EM_METRICS_NATIVE_BUTTON_WIDTH),
									 oRect.m_vBottom - oRect.m_vTop - COL_MARGIN_BOTTOM));
	
	m_opNameRemoveButton -> SetFrame(EMRect(oRect.m_vRight - oRect.m_vLeft - (COL_MARGIN_SIDE + EM_METRICS_NATIVE_BUTTON_WIDTH),
											oRect.m_vBottom - oRect.m_vTop - (COL_MARGIN_BOTTOM + EM_METRICS_NATIVE_BUTTON_HEIGHT),
											oRect.m_vRight - oRect.m_vLeft - COL_MARGIN_SIDE,
											oRect.m_vBottom - oRect.m_vTop - COL_MARGIN_BOTTOM));

}

bool ColaborationView::Lock()
{
	return m_opMainView -> LockWindow();
}

uint32 ColaborationView::LoginMessage()
{
	return OK_TO_CONNECT;
}

bool ColaborationView::MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage)
{
	switch(p_vMessage)
	{
/*	case EM_DRAW:
		if(p_opSender == m_opMainView)
		{
//			eo << "EM_DRAW: m_oBackgroundView" << ef;
			m_opMainView -> SetHighColor(EMColor(192, 192, 192, 255));
			m_opMainView -> FillRect(m_opMainView -> Bounds());
		}
		if(p_opSender == m_opChatView)
		{
//			eo << "EM_DRAW: m_oBackgroundView" << ef;
			m_opChatView -> SetHighColor(EMColor(192, 192, 192, 255));
			m_opChatView -> FillRect(m_opChatView -> Bounds());
		}
		if(p_opSender == m_opMemberView)
		{
//			eo << "EM_DRAW: m_oBackgroundView" << ef;
			m_opMemberView -> SetHighColor(EMColor(192, 192, 192, 255));
			m_opMemberView -> FillRect(m_opMemberView -> Bounds());
		}
		return true;
*/	case EM_VIEW_BOUNDS_CHANGED: // Most often due to the user resizing the window
		LayoutViews();
		m_opMainView -> Invalidate(m_opMainView -> Bounds());
		return true;
	case CONNECT_BUTTON_PRESSED:
		eo << "CONNECT_BUTTON_PRESSED" << ef;
		if(EMNetworkEngine::Instance() -> IsConnected())
		{
			eo << "Connected" << ef;
			if(EMNetworkEngine::Instance() -> Disconnect())
			{
				eo << "Should be disconnected now" << ef;
				EnableColaborationUI(false);
			}
			else
				eo << "Counld not disconnect" << ef;
		}
		else
		{
			EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_DISPLAY_LOGIN_DIALOG);
			//EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_DISPLAY_REGISTRATION_DIALOG, "Testar", "Framåt");
		/*	if(EMNetworkEngine::Instance() -> Connect())
			{
				eo << "Should be connected now" << ef;
				int32 vNumberOfUsers(*(static_cast<int32*>(EMCommandRepository::Instance() -> ExecuteCommand(NETWORK_INFO_COMMAND_NUMBER_OF_USERS, NULL, NULL, NULL))));
				uint64* vpUserID = EM_new uint64[sizeof(uint64) * vNumberOfUsers];
				uint32* vpUserStatus = EM_new uint32[sizeof(uint32) * vNumberOfUsers];
				char** vppUserNames = EM_new char*[sizeof(char*) * vNumberOfUsers];

				EMCommandRepository::Instance() -> ExecuteCommand(NETWORK_INFO_COMMAND_GET_USER_INFOS, vpUserID, vppUserNames, vpUserStatus);

				m_opSendToComboBox -> Clear();
				m_opMemberListViewControl -> DeleteAllItems();
				m_opChatListViewControl -> DeleteAllItems();
				m_opSendToComboBox -> AddString("< ALL >");
				m_opSendToComboBox -> SelectItem(0);

				char vTemp[255];

				for(int i = 0; i < vNumberOfUsers; i++)
				{
					m_opMemberListViewControl -> AddItem(vppUserNames[i]);
					m_opSendToComboBox -> AddString(vppUserNames[i]);
					m_oComboBoxIDUserIDMap[i + 1] = vpUserID[i];
					//m_oUserIDComboBoxIDMap[vpUserID[i]] = i + 1;
					m_oUserIDUserNameMap[vpUserID[i]] = vppUserNames[i];
					if(vpUserStatus[i] != 0)
						m_opMemberListViewControl -> AddSubItem("Yes", 1);
					else
						m_opMemberListViewControl -> AddSubItem("No", 1);
					m_opMemberListViewControl -> AddSubItem(_ui64toa(vpUserID[i], vTemp, 10), 4);

				}
				delete [] vpUserID;
				delete [] vpUserStatus;
				delete [] vppUserNames;

				EnableColaborationUI(true);
			}
			else
				eo << "Counld not connect" << ef;
			*/	
		}

		return true;
	case ASSIGN_BUTTON_PRESSED:
		eo << "ASSIGN_BUTTON_PRESSED" << ef;
		return true;
	case ADD_BUTTON_PRESSED:
		eo << "ADD_BUTTON_PRESSED" << ef;
		EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_DISPLAY_SEARCH_ADD_USER_DIALOG);
		return true;
	case CREATE_BUTTON_PRESSED:
		eo << "CREATE_BUTTON_PRESSED" << ef;
		return true;
	case NAME_REMOVE_BUTTON_PRESSED:
		eo << "NAME_REMOVE_BUTTON_PRESSED" << ef;
		DeleteUser();
		return true;
	case EM_LIST_VIEW_ITEM_DELETED:
		eo << "EM_LIST_VIEW_ITEM_DELETED" << ef;
		return DeleteUser();
	case PROJECT_REMOVE_BUTTON_PRESSED:
		eo << "PROJECT_REMOVE_BUTTON_PRESSED" << ef;
		return true;
	case OK_TO_CONNECT:
		{
			eo << "OK_TO_CONNECT" << ef;
			//Setup network connection
			if(EMNetworkEngine::Instance() != NULL)
			{
				bool vUseTCP = *static_cast<bool*>(EMSettingsRepository::Instance() ->GetSetting(SETTING_NETWORK_USE_TCP));
				uint32 vPort(0);
				if(vUseTCP)
					vPort = *static_cast<uint32*>(EMSettingsRepository::Instance() ->GetSetting(SETTING_NETWORK_TCP_PORT));
				else
					vPort = *static_cast<uint32*>(EMSettingsRepository::Instance() ->GetSetting(SETTING_NETWORK_UDP_PORT));
				EMNetworkEngine::Instance() -> Initialize(static_cast<string*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_NETWORK_HOST)) -> c_str(),
														  vPort,
														  vUseTCP,
														  *static_cast<bool*>(EMSettingsRepository::Instance() ->GetSetting(SETTING_NETWORK_USE_INTERNET)));
			}

			char vUserID[40];
			char vPassword[40];

			ConvertToID_String(*static_cast<uint64*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_NETWORK_USER_ID)), vUserID);
			strcpy(vPassword, static_cast<string*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_NETWORK_USER_PASSWORD)) -> c_str());

			if(EMNetworkEngine::Instance() -> Connect(vUserID, vPassword))
			{
				eo << "Should be connected now" << ef;
				if(EMNetworkEngine::Instance() ->IsConnected())
					eo << "CONNECTED" << ef;
				else 
				{
					eo << "NOT CONNECTED" << ef;
					return true;
				}
/*				int32 vNumberOfUsers(*(static_cast<int32*>(EMCommandRepository::Instance() -> ExecuteCommand(NETWORK_INFO_COMMAND_NUMBER_OF_USERS, NULL, NULL, NULL))));
				uint64* vpUserID = EM_new uint64[sizeof(uint64) * vNumberOfUsers];
				uint32* vpUserStatus = EM_new uint32[sizeof(uint32) * vNumberOfUsers];
				char** vppUserNames = EM_new char*[sizeof(char*) * vNumberOfUsers];

				EMCommandRepository::Instance() -> ExecuteCommand(NETWORK_INFO_COMMAND_GET_USER_INFOS, vpUserID, vppUserNames, vpUserStatus);

				m_opSendToComboBox -> Clear();
				m_opMemberListViewControl -> DeleteAllItems();
				m_opChatListViewControl -> DeleteAllItems();
				m_opSendToComboBox -> AddString("< ALL >");
				m_opSendToComboBox -> SelectItem(0);

				char vTemp[255];

				for(int i = 0; i < vNumberOfUsers; i++)
				{
	/*				m_opMemberListViewControl -> AddItem(vppUserNames[i]);
					m_opSendToComboBox -> AddString(vppUserNames[i]);
					m_oComboBoxIDUserIDMap[i + 1] = vpUserID[i];
					//m_oUserIDComboBoxIDMap[vpUserID[i]] = i + 1;
					m_oUserIDUserNameMap[vpUserID[i]] = vppUserNames[i];
					if(vpUserStatus[i] != 0)
						m_opMemberListViewControl -> AddSubItem("Yes", 1);
					else
						m_opMemberListViewControl -> AddSubItem("No", 1);
					m_opMemberListViewControl -> AddSubItem(_ui64toa(vpUserID[i], vTemp, 10), 4);
	*				AddUser(vppUserNames[i], vpUserID[i], vpUserStatus[i]);
				}
				delete [] vpUserID;
				delete [] vpUserStatus;
				delete [] vppUserNames;
	
				m_opSendToComboBox -> Notify(SEND_TO_COMBO_BOX_SELECTION_CHANGED);
*/
				UpdateUserlist();
				EnableColaborationUI(true);
			}
			else
				eo << "Counld not connect" << ef;
			return true;
		}
	case EM_TEXT_CONTROL_SET_SELECTION:
		if(p_opSender == m_opChatTextControl)
		{
			if(m_vAlreadySet)
				m_vAlreadySet = false;
			else
			{
				m_vAlreadySet = true;
				m_opChatTextControl -> SetSelection(0, -1);
			}
			return true;
		}
		return false;
	case SEND_TO_COMBO_BOX_SELECTION_CHANGED:
		eo << "SEND_TO_COMBO_BOX_SELECTION_CHANGED" << ef;
		SetUsersToSendTo(m_opSendToComboBox -> GetSelectionIndex());
		return true;
	case EM_GUI_COMPONENT_ENTER_PRESSED:
		if(p_opSender == m_opChatTextControl)
		{
			eo << "EM_GUI_COMPONENT_ENTER_PRESSED" << ef;
			m_opChatListViewControl -> AddItem("You");
			m_opChatListViewControl -> AddSubItem(m_opChatTextControl -> Text(), 1);
			m_opChatListViewControl -> AddSubItem(m_opSendToComboBox -> GetSelection().c_str(), 2);
	
//			for(int i = 0; m_vpUserIDToSend[i] != 0; i++)
			EMCommandRepository::Instance() -> ExecuteCommand(NETWORK_COMMAND_SEND_MESSAGE, &m_vpUserIDToSend, static_cast<void*>(const_cast<char*>(m_opChatTextControl -> Text())), NULL);
			m_opChatTextControl -> SetText("");
			m_opChatListViewControl -> Scroll(-1);
		}
		return true;
	case EM_GUI_COMPONENT_ESC_PRESSED:
		if(p_opSender == m_opChatTextControl)
		{
			eo << "EM_GUI_COMPONENT_ESC_PRESSED" << ef;
			m_opChatTextControl -> SetText("");
			return true;
		}
		return false;
	case EM_NETWORK_MESSAGE_ARRIVED:
		{
			eo << "EM_NETWORK_MESSAGE_ARRIVED" << ef;
			char vpMessage[1024];
			uint64 vUserID(*static_cast<uint64*>(EMCommandRepository::Instance() -> ExecuteCommand(NETWORK_INFO_COMMAND_GET_MESSAGE, vpMessage, NULL, NULL)));
			m_opChatListViewControl -> AddItem(m_oUserIDUserNameMap[vUserID].c_str());
			m_opChatListViewControl -> AddSubItem(vpMessage, 1);
			m_opChatListViewControl -> Scroll(-1);
			return true;
		}
	//	return false;
	case EM_NETWORK_USER_CHANGED:
		{
			eo << "EM_NETWORK_USER_CHANGED" << ef;
			uint64 vUserID(0);
			uint64 vTempUserID(0);
			uint32 vUserStatus(0);
			EMCommandRepository::Instance() -> ExecuteCommand(NETWORK_INFO_COMMAND_USER_STATUS_MESSAGE, &vUserID, &vUserStatus, NULL);
			for(int i = 0; i < m_opMemberListViewControl -> GetItemCount(); i++)
			{
				vTempUserID = _atoi64(m_opMemberListViewControl -> GetSubItem(i, -1));
				if(vTempUserID == vUserID)
				{
					if(vUserStatus != 0)
						m_opMemberListViewControl -> ChangeSubItem(i, 1, "Yes");  // It set the sub item, should change this name
					else
						m_opMemberListViewControl -> ChangeSubItem(i, 1, "No");   // It set the sub item, should change this name
				}
			}
			return true;
		}
	case EM_NETWORK_USER_ADDED:
	case EM_NETWORK_USER_DELETED:
		UpdateUserlist();
		return true;
	case EM_NETWORK_FILE_ARRIVED:
		{
			eo << "EM_NETWORK_FILE_ARRIVED" << ef;
			m_vFileID = 0;
			char* vpFileName = NULL;
			bool vToMediaPool(false);
			EMCommandRepository::Instance() -> ExecuteCommand(NETWORK_INFO_COMMAND_GET_NEXT_FILE_INFO, &m_vFileID, &vpFileName, &vToMediaPool);
			if(vToMediaPool)
			{
				int vProjectID = *(static_cast<int*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_PROJECT_ID)));
				uint32 vDirectoryID = DIRECTORY_ID_DOWNLOAD;
				string oFullPath(static_cast<string*>(EMCommandRepository::Instance() -> ExecuteCommand(PROJECT_COMMAND_GET_DIRECTORY, &vProjectID, &vDirectoryID)) -> c_str());
				oFullPath.append(EM_FILE_SEPARATOR);
				oFullPath.append(vpFileName);
				SaveFile(const_cast<char*>(oFullPath.c_str()));
		/*		switch(*static_cast<int32*>(EMCommandRepository::Instance() -> ExecuteCommand(NETWORK_COMMAND_RECEIVE_FILE, &m_vFileID, const_cast<char*>(oFullPath.c_str()), NULL)))
				{
				case 0:
					eo << "ERROR" << ef;
				case 1:
					eo << "OK, file aready downloaded" << ef;
				case 2:
					eo << "Downloading now" << ef;
				}
		*/
			}
			else
			{
				if(m_opFileWindow)
					delete m_opFileWindow;
				m_opFileWindow = EMFactory::Instance() -> CreateFileWindow(FILE_PANEL_SAVE_FILE, "Save Download", m_opMainView ->GetNativeView(), false, vpFileName, NULL, NULL, SAVE_FILE);
				m_opFileWindow -> AddListener(this);
				m_opFileWindow -> Show();
			}
			return true;
		}
	case SAVE_FILE:
		{
			vector<string>* opPath = m_opFileWindow -> GetPaths();
			SaveFile(const_cast<char*>(opPath -> at(0).c_str()));
		/*	switch(*static_cast<int32*>(EMCommandRepository::Instance() -> ExecuteCommand(NETWORK_COMMAND_RECEIVE_FILE, &m_vFileID, const_cast<char*>(opPath -> at(0).c_str()), NULL)))
			{
			case 0:
				eo << "ERROR" << ef;
			case 1:
				eo << "OK, file aready downloaded" << ef;
			case 2:
				eo << "Downloading now" << ef;
			}
	*/		return true;

	
		}
	case EM_NETWORK_FILE_DONE:
		{
			eo << "EM_NETWORK_FILE_DONE" << ef;
			char* vpFullPath;
			bool vToMediaPool(false);
			EMCommandRepository::Instance() -> ExecuteCommand(NETWORK_INFO_COMMAND_GET_NEXT_DOWNLOADED_FILE, &vpFullPath, &vToMediaPool, NULL);
			if(vToMediaPool)
			{
				int32 vEntryIDs[2];
				// FIXME: HACK: Should call COMMAND_ADD_TO_MEDIA_POOL with only full path an not EntryIDs. The command should take care of it, or?
				EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_IMPORT_MEDIA, vpFullPath, vEntryIDs);
				EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_ADD_TO_MEDIA_POOL_VIEW, vpFullPath, vEntryIDs);
			}
			return true;
		}
	case EM_NETWORK_LOST_CONNECTION:
		EnableColaborationUI(false);
		return true;
	case EM_DRAG_ENTER:
		eo << "ColaborationView -> EM_BEGIN_DRAG" << ef;
		m_opMemberListViewControl -> EnableHoverSelect(true, 1);
		return false;
	case EM_DRAG_LEAVE:
		m_opMemberListViewControl -> EnableHoverSelect(false);
		return false;
	case EM_DROP:
		if(p_opSender == m_opMemberListViewControl)
		{
			eo << "EM_DROP at: " << (char*)m_opMemberListViewControl -> GetSelection().c_str() << ef;
			eo << "Data dropped File name: " << (char*)EMDragDrop::Instance() -> GetData() -> m_opString.c_str() << ef;
			uint64 vTempUserID = _atoi64(m_opMemberListViewControl -> GetSubItem(m_opMemberListViewControl -> GetSelectionID(), -1));
			string oTempString(EMDragDrop::Instance() -> GetData() -> m_opString);
		//	int vDelete(oTempString.find_first_of('|'));
		//	if(vDelete != -1)
		//		oTempString.erase(vDelete, 1);
			EMCommandRepository::Instance() -> ExecuteCommand(NETWORK_COMMAND_SEND_FILE, &vTempUserID, const_cast<char*>(oTempString.c_str()), NULL);
			m_opMemberListViewControl -> EnableHoverSelect(false);
			return false;
		}
	}
	return false;
}

void ColaborationView::SaveFile(char* p_vpFullPath)
{
	switch(*static_cast<int32*>(EMCommandRepository::Instance() -> ExecuteCommand(NETWORK_COMMAND_RECEIVE_FILE, &m_vFileID, const_cast<char*>(p_vpFullPath), NULL)))
	{
	case 0:
		eo << "ERROR" << ef;
	case 1:
		eo << "OK, file aready downloaded" << ef;
	case 2:
		eo << "Downloading now" << ef;
	}
}

void ColaborationView::SetUsersToSendTo(int p_vpIndex)
{
	if(m_vpUserIDToSend != NULL)
		delete [] m_vpUserIDToSend;
	
	if(p_vpIndex != 0)
	{
		m_vpUserIDToSend = new uint64[sizeof(uint64) * 2];
		m_vpUserIDToSend[0] = m_oComboBoxIDUserIDMap[p_vpIndex];
		m_vpUserIDToSend[1] = 0;
	}
	else
	{
		int vSize(m_oComboBoxIDUserIDMap.size());
		m_vpUserIDToSend = new uint64[sizeof(uint64) * (vSize + 1)];
		for(int i = 0; i < vSize; i++)
			m_vpUserIDToSend[i] = m_oComboBoxIDUserIDMap[i + 1];
		m_vpUserIDToSend[i] = 0;
	}

}

void ColaborationView::Show()
{
/*	if(m_opWindow -> Lock())
	{
		m_opWindow -> Show();
		m_opWindow -> Unlock();
	}
*/
}

void ColaborationView::UpdateUserlist()
{
	int32 vNumberOfUsers(*(static_cast<int32*>(EMCommandRepository::Instance() -> ExecuteCommand(NETWORK_INFO_COMMAND_NUMBER_OF_USERS, NULL, NULL, NULL))));
	uint64* vpUserID = EM_new uint64[sizeof(uint64) * vNumberOfUsers];
	uint32* vpUserStatus = EM_new uint32[sizeof(uint32) * vNumberOfUsers];
	char** vppUserNames = EM_new char*[sizeof(char*) * vNumberOfUsers];

	EMCommandRepository::Instance() -> ExecuteCommand(NETWORK_INFO_COMMAND_GET_USER_INFOS, vpUserID, vppUserNames, vpUserStatus);

	m_opSendToComboBox -> Clear();
	m_opMemberListViewControl -> DeleteAllItems();
	m_opChatListViewControl -> DeleteAllItems();
	m_oComboBoxIDUserIDMap.erase(m_oComboBoxIDUserIDMap.begin(), m_oComboBoxIDUserIDMap.end());
	m_oUserIDUserNameMap.erase(m_oUserIDUserNameMap.begin(), m_oUserIDUserNameMap.end());
	m_opSendToComboBox -> AddString("< ALL >");
	m_opSendToComboBox -> SelectItem(0);

//	char vTemp[255];

	for(int i = 0; i < vNumberOfUsers; i++)
	{
/*		m_opMemberListViewControl -> AddItem(vppUserNames[i]);
		m_opSendToComboBox -> AddString(vppUserNames[i]);
		m_oComboBoxIDUserIDMap[i + 1] = vpUserID[i];
		//m_oUserIDComboBoxIDMap[vpUserID[i]] = i + 1;
		m_oUserIDUserNameMap[vpUserID[i]] = vppUserNames[i];
		if(vpUserStatus[i] != 0)
			m_opMemberListViewControl -> AddSubItem("Yes", 1);
		else
			m_opMemberListViewControl -> AddSubItem("No", 1);
		m_opMemberListViewControl -> AddSubItem(_ui64toa(vpUserID[i], vTemp, 10), 4);
*/		AddUser(vppUserNames[i], vpUserID[i], vpUserStatus[i]);
	}
	delete [] vpUserID;
	delete [] vpUserStatus;
	delete [] vppUserNames;

	m_opSendToComboBox -> Notify(SEND_TO_COMBO_BOX_SELECTION_CHANGED);
}