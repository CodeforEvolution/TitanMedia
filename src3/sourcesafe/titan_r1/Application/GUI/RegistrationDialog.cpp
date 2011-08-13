#include "RegistrationDialog.h"

#include "CommandIDs.h"
#include "DirectoryIDs.h"
#include "EMApplication.h"
#include "EMButton.h"
#include "EMCommandLineIDs.h"
#include "EMCommandRepository.h"
#include "EMFactory.h"
#include "EMLabel.h"
#include "EMMessageBox.h"
#include "EMMessages.h"
#include "EMResourceRepository.h"
#include "EMSettingIDs.h"
#include "EMSettingsRepository.h"
#include "EMTextControl.h"
#include "EMView.h"
#include "EMWindow.h"
#include "ResourceIDs.h"
#include "r.h"
#include "TrackWindow.h"

#include <stdio.h>

#ifdef PLATFORM_WINDOWS
#include <windows.h> // To get modal dialog to work.
#include "EMWinWindow.h" // To get modal dialog to work.
#endif

const uint32 OK_BUTTON = 1;
const uint32 CANCEL_BUTTON = 2;
const uint32 TEXT_CONTROL_CHANGED = 3;

RegistrationDialog::RegistrationDialog() :
m_opDialogWindow(EMFactory::Instance() -> CreateEMWindow(EMRect(0, 0, 300, 120), "Registration", EM_WINDOW_DIALOG)), 
m_opMainView(EMFactory::Instance() -> CreateView(EMRect(0, 0, 300, 120), false)),
m_opEnterLabel(EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
														   EM_METRICS_NATIVE_DIALOG_MARGIN_TOP,
														   300 - EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
														   EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + EM_METRICS_NATIVE_LABEL_HEIGHT),
													"Enter registration code:", 0)),
m_opRegistrationTextControl(EMFactory::Instance() -> CreateTextControl(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
																			  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 20,
																			  300 - EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
																			  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 20 + EM_METRICS_NATIVE_TEXT_CONTROL_HEIGHT),
																	   "", 0, 0, false)),
m_opCaseLabel(EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
														  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 45,
														  300 - EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
														  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 45 + EM_METRICS_NATIVE_LABEL_HEIGHT),
													"Note! The registration code is Case Sensitive.", 0)),
m_opOK_Button(EMFactory::Instance() -> CreateButton(EMRect(300 - (EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + EM_METRICS_NATIVE_BUTTON_WIDTH * 2 + EM_METRICS_NATIVE_BUTTON_SPACE),
														   120 - (EM_METRICS_NATIVE_DIALOG_MARGIN_BOTTOM + EM_METRICS_NATIVE_BUTTON_HEIGHT),
														   300 - (EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + EM_METRICS_NATIVE_BUTTON_WIDTH + EM_METRICS_NATIVE_BUTTON_SPACE),
														   120 - (EM_METRICS_NATIVE_DIALOG_MARGIN_BOTTOM)),
													"OK", OK_BUTTON, 0, 0, NULL, true)),
m_opCancelButton(EMFactory::Instance() -> CreateButton(EMRect(300 - (EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + EM_METRICS_NATIVE_BUTTON_WIDTH),
															  120 - (EM_METRICS_NATIVE_DIALOG_MARGIN_BOTTOM + EM_METRICS_NATIVE_BUTTON_HEIGHT),
															  300 - (EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE),
															  120 - (EM_METRICS_NATIVE_DIALOG_MARGIN_BOTTOM)),
													   "Cancel", CANCEL_BUTTON, 0, 0))
{
	m_opDialogWindow -> AddChild(m_opMainView);
	m_opMainView -> AddChild(m_opEnterLabel);
	m_opMainView -> AddChild(m_opRegistrationTextControl);
	m_opMainView -> AddChild(m_opCaseLabel);
	m_opMainView -> AddChild(m_opOK_Button);
	m_opMainView -> AddChild(m_opCancelButton);
}
RegistrationDialog::~RegistrationDialog()
{
	delete m_opEnterLabel;
	delete m_opRegistrationTextControl;
	delete m_opCaseLabel;
	delete m_opOK_Button;
	delete m_opCancelButton;

	delete m_opDialogWindow;
	delete m_opMainView;
}

void RegistrationDialog::ContinueStartup()
{
	if(m_oProjectFile != "")
	{
		if(m_opTrackWindow -> GetWindow() -> Lock())
		{
			uint32 vProjectID = reinterpret_cast<uint32>(EMCommandRepository::Instance() -> ExecuteCommand(PROJECT_COMMAND_LOAD_PROJECT, const_cast<char*>(m_oProjectFile.c_str())));
			if(vProjectID > 0)
			{
				int vNameBeginPos = m_oProjectFile.find_last_of(EM_FILE_SEPARATOR, m_oProjectFile.find_last_of(EM_FILE_SEPARATOR) - 1) + 1;
				int vNameEndPos = m_oProjectFile.find_last_of(EM_FILE_SEPARATOR) - 1;
				string oProjectName = m_oProjectFile.substr(vNameBeginPos, vNameEndPos - vNameBeginPos + 1);
				string oWindowTitle = string(EMResourceRepository::GetString(RES_SR_APPLICATION_NAME) -> c_str()) + " " + EMApplication::GetApplicationVersionNumber() +
												" - " + oProjectName;
				m_opTrackWindow -> GetWindow() -> SetTitle(oWindowTitle.c_str());

				EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_CLEAR_UNDO_STACK);		
				EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_SET_UNDO_TEXTS);				

				m_opTrackWindow -> GetWindow() -> Unlock();

				EMCommandRepository::Instance() -> ExecuteCommand(PROJECT_COMMAND_READY_FOR_USE);
			}
			else
			{
				m_opTrackWindow -> GetWindow() -> Unlock();
				EMFactory::Instance() -> CreateMessageBox() -> DisplayDialog(NULL, "Could not load the specified project file.\nPerhaps the file is in an incorrect format or is damaged.", "Load Project Failed", EM_DIALOG_BUTTON_OK | EM_DIALOG_ICON_ERROR);
				EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_DISPLAY_NEW_PROJECT_DIALOG);
			}
		}
	}
	
	else if((m_vStartupFlags & EM_CL_DISPLAY_NEW_PROJECT_DIALOG) != 0)
	{
		// Creating a "dummy" project (for debug purposes)
		eo << "Creating dummy project..." << ef;
		string oCreatePath("c:\\titan_project2");
		EMCommandRepository::Instance() -> ExecuteCommand(PROJECT_COMMAND_LOAD_TEMPLATE, const_cast<char*>(oCreatePath.c_str()));
		EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_SAVE_PROJECT);
		eo << "Dummy project created and saved" << ef;
	}

	else
		EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_DISPLAY_NEW_PROJECT_DIALOG);
}

void RegistrationDialog::CreateRegKeyFile()
{
	FILE* vRegKeyFile = NULL;
	InitCRC();
	int vProjectID(0);//(*static_cast<int*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_PROJECT_ID)));
	uint32 vDirectoryID(DIRECTORY_ID_APPLICATION);
	string oAppDirFileName = *static_cast<string*>(EMCommandRepository::Instance() -> ExecuteCommand(PROJECT_COMMAND_GET_DIRECTORY, &vProjectID, &vDirectoryID)) + "\\TitanRegKey.trk";

	if((vRegKeyFile = fopen(oAppDirFileName.c_str(), "wb")) != NULL)
	{
		const char* vRegKey = m_opRegistrationTextControl -> Text(); 
		fwrite(vRegKey, strlen(vRegKey), 1, vRegKeyFile);
		fclose(vRegKeyFile);
		//bool vBoolSetting(false);
		//EMSettingsRepository::Instance() -> SetSetting(SETTING_SOFTWARE_IS_CRIPPLED, EM_SETTING_BOOL, &vBoolSetting, true, true);
	}

	//****-Made by Jesper(2001-11-20) for checking that registration is valid
	int codeEnd;
	LPVOID decryptionStart;
	DWORD oldprotect;
	
	typedef LPVOID (* FunctionType)(LPVOID);
	FunctionType ComputeSum;
	int vChecksum = 0;
	int vChecksum2 = 0;
	BYTE* ComputeKey;
	int bytesReadCryptFile = 0;
	int bytesReadKeyFile = 0;
	char* opPtr;
	int lengthToCopy = 512;//codeEnd - codeStart;
	int vMustard = 33;

	ComputeSum = (FunctionType) new BYTE[lengthToCopy];
	ComputeKey = new BYTE[255];
	//Get project path
	oAppDirFileName = *static_cast<string*>(EMCommandRepository::Instance() -> ExecuteCommand(PROJECT_COMMAND_GET_DIRECTORY, &vProjectID, &vDirectoryID)) + "\\TitanRegChunk.trc";
	string oAppDirFileName2 = *static_cast<string*>(EMCommandRepository::Instance() -> ExecuteCommand(PROJECT_COMMAND_GET_DIRECTORY, &vProjectID, &vDirectoryID)) + "\\TitanRegKey.trk";

	//Read in the serial key
	FILE* fp3 = fopen(oAppDirFileName2.c_str(),"rb");
	if(fp3 != NULL)
	{
		bytesReadCryptFile = fread(ComputeKey, 1, 255, fp3);
		fclose(fp3);
	}
	else
		return;
	char* key = new char[bytesReadCryptFile+1];
	key[bytesReadCryptFile] = '\0';
	memcpy(key, ComputeKey, bytesReadCryptFile);

	FILE* fp2 = fopen(oAppDirFileName.c_str(),"rb");
	if(fp2 != NULL)
	{
		bytesReadCryptFile = fread(ComputeSum, 1, lengthToCopy, fp2);
		fclose(fp2);
	}
	else
		return;
	opPtr = (char*)ComputeSum;

	fp2 = fopen(oAppDirFileName2.c_str(),"rb");
	if(fp2 != NULL)
	{
		bytesReadKeyFile = fread(ComputeKey, 1, 255, fp2);
		fclose(fp2);
	}
	else
		return;

	//Calculate checksum
	LPVOID vCheksumPointerVoid = ComputeSum;
	char* vCheksumPointer = (char*) vCheksumPointerVoid;
	for(int index = 0; index < bytesReadCryptFile; index ++)
	{
		vChecksum += (BYTE)vCheksumPointer[index];
	}

	SetCryptKeys(key);

	//Chabbelkod starts here (for the crackers)**************************************
	decryptionStart = (void*)0xff00;
	if(decryptionStart && vChecksum)
		oldprotect = 0xff00 + vChecksum;
	else
		oldprotect = 0x0000;
	decryptionStart = (void*)(oldprotect * lengthToCopy / 4 + (BYTE)&decryptionStart);
	codeEnd = oldprotect;
	for(index = 0; index < 18; index ++)
		codeEnd += (BYTE)vCheksumPointer[index] + vChecksum;
	decryptionStart = (void*)0x00ff;
	//Chabbelkod Stops here**********************************************************

	for(int aroundandaround = 0; aroundandaround < 32; aroundandaround ++)
	{
		DecryptBlock((unsigned char*)opPtr);
		opPtr += 16;
	}
	delete [] key;
	for(index = 0; index < 44; index ++)
	{
		vChecksum2 += (BYTE)vCheksumPointer[index];
	}
	//Chabbelkod starts here (for the crackers)**************************************
	decryptionStart = (void*)0xf0f0;
	if(decryptionStart && vChecksum2)
		oldprotect = 0xf0f + vChecksum2;
	else
		oldprotect = 0x000f;
	decryptionStart = (void*)(oldprotect * lengthToCopy / 4 + (BYTE)&decryptionStart);
	codeEnd = oldprotect;
	for(index = 0; index < 18; index ++)
		codeEnd += (BYTE)vCheksumPointer[index] + vChecksum2;
	decryptionStart = (void*)0xff0f;
	//Chabbelkod Stops here**********************************************************

	 vMustard = (vChecksum2 - 4164);
	 bool vIsCrippled = vMustard != 0;
	// FIXME: Check if the software is registered and other copy protection stuff here
	EMSettingsRepository::Instance() -> SetSetting(SETTING_SOFTWARE_IS_CRIPPLED, EM_SETTING_BOOL, &vIsCrippled, false, true);

	if(vIsCrippled)
		DeleteFile(oAppDirFileName2.c_str());
	//****End of section


}

void RegistrationDialog::DisplayDialog()
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
}
void RegistrationDialog::HideDialog()
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

void RegistrationDialog::Init(char* p_vpProjectFile, uint32 p_vStartupFlags, TrackWindow* p_opTrackWindow)
{
	m_oProjectFile = p_vpProjectFile;
	m_vStartupFlags = p_vStartupFlags;
	m_opTrackWindow = p_opTrackWindow;

	m_opDialogWindow -> AddListener(this);
	m_opMainView -> AddListener(this);
	if(m_opDialogWindow -> Lock())
	{
		//m_opMainView -> SetViewColor(EMColor(0, 192, 0, 255));
		m_opMainView -> SetDefaultColor();
		m_opDialogWindow -> Unlock();
	}

	m_opEnterLabel -> AddListener(this);
	m_opRegistrationTextControl -> AddListener(this);
	m_opCaseLabel -> AddListener(this);
	m_opOK_Button -> AddListener(this);
	m_opCancelButton -> AddListener(this);

	// TabOrder
	m_opDialogWindow -> AddTabComponent(m_opRegistrationTextControl);
	m_opDialogWindow -> AddTabComponent(m_opOK_Button);
	m_opDialogWindow -> AddTabComponent(m_opCancelButton);
}

bool RegistrationDialog::MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage)
{
	switch(p_vMessage)
	{
	case EM_GUI_COMPONENT_ENTER_PRESSED:
	case OK_BUTTON:
		CreateRegKeyFile();
	case EM_GUI_COMPONENT_ESC_PRESSED:
	case EM_CLOSE_WINDOW:
	case CANCEL_BUTTON:
		HideDialog();
		ContinueStartup();
		//EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_DISPLAY_NEW_PROJECT_DIALOG);
		return true;
	case TEXT_CONTROL_CHANGED:
		return false;
	case EM_GUI_COMPONENT_TAB_PRESSED:
		m_opDialogWindow -> NextTab();
		return true;
	case EM_GUI_COMPONENT_SHIFT_TAB_PRESSED:
		m_opDialogWindow -> PreviousTab();
		return true;
	case EM_GUI_COMPONENT_GOT_FOCUS:
		if(p_opSender == m_opDialogWindow)
			m_opRegistrationTextControl -> SetFocus();
		m_opDialogWindow -> SetCurrentFocus();
		return true;
/*	case EM_CLOSE_WINDOW:
		if(p_opSender == m_opDialogWindow)
		{
			eo << "EM_CLOSE_WINDOW" << ef;
			HideDialog();
			return true;
		}
		break;
*/	}
	return false;
}

void RegistrationDialog::MoveDialog()
{
	EMRect oWindowRect(m_opParentWindow -> Frame());
	EMRect oDialogRect(m_opDialogWindow -> Frame());
	float vX(oWindowRect.m_vRight - oWindowRect.GetWidth() / 2 - oDialogRect.GetWidth() / 2);
	float vY(oWindowRect.m_vBottom - oWindowRect.GetHeight() / 2 - oDialogRect.GetHeight() / 2);

	MoveDialog(vX, vY);
}

void RegistrationDialog::MoveDialog(float p_vX, float p_vY)
{
	m_opDialogWindow -> MoveTo(p_vX, p_vY);
}

void RegistrationDialog::SetParent(EMWindow* p_opWindow)
{
	m_opParentWindow = p_opWindow;
}
