/*******************************************************
* This code is the copyrighted property of Elegant Media
*-------------------------------------------------------
* Creator: Johan Dunberg
* Component: Application
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/
#include "EMGlobals.h"

#ifndef __COLABORATION_VIEW
#define __COLABORATION_VIEW

#include "EMListener.h"
#include <map>

class EMButton;
class EMComboBox;
class EMFileWindow;
class EMLabel;
class EMListViewControl;
class EMMessageBox;
class EMTabControl;
class EMTextControl;
class EMView;
class EMNetworkEngine;

class ColaborationView : public EMListener
{
public:
	ColaborationView();
	~ColaborationView();
	void AddUser(char* p_vpUserName, uint64 p_vUserID, uint32 p_vUserStatus);
	uint64 ConvertToID_Uint64(const char* p_vpUserID);
	void ConvertToID_String(uint64 p_vUserID, char* p_vpUserID);
	EMView* GetView();
	void EnableColaborationUI(bool p_vEnable);
	void Hide();
	void Init();
	static ColaborationView* Instance();
	bool Lock();
	uint32 LoginMessage();
	bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage);
	void Show();
	void Unlock();

protected:
	bool DeleteUser();
	void LayoutViews();
	void SaveFile(char* p_vpFullPath);
	void SetUsersToSendTo(int p_vpIndex);
	void UpdateUserlist();

private:
	EMButton* m_opAddButton;
	EMButton* m_opAssignButton;
	EMListViewControl* m_opChatListViewControl;
	EMTextControl* m_opChatTextControl;
	EMView* m_opChatView;
	EMButton* m_opConnectButton;
	EMButton* m_opCreateButton;
	EMFileWindow* m_opFileWindow;
	static ColaborationView* m_opInstance;
	EMListViewControl* m_opMemberListViewControl;
	EMView* m_opMemberView;
	EMView* m_opMainView;
	EMButton* m_opNameRemoveButton;
	EMLabel* m_opProjectLabel;
	EMComboBox* m_opProjectComboBox;
	EMButton* m_opProjectRemoveButton;
	EMLabel* m_opSendToLabel;
	EMComboBox* m_opSendToComboBox;
	EMTabControl* m_opTabControl;

	EMMessageBox* m_opMsgBox;

	map<int, uint64> m_oComboBoxIDUserIDMap;
//	map<uint64, int> m_oUserIDComboBoxIDMap;
	map<uint64, string> m_oUserIDUserNameMap;
	uint64* m_vpUserIDToSend;
	bool vPutInMediaPool;
	uint32 m_vFileID;
	bool m_vAlreadySet;
};

#endif