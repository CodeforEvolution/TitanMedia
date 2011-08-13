#include "NewProjectDialog.h"

#include "CommandIDs.h"
#include "DirectoryIDs.h"
#include "EMApplication.h"
#include "EMBrowseWindow.h"
#include "EMButton.h"
#include "EMComboBox.h"
#include "EMFactory.h"
#include "EMLabel.h"
#include "EMListBox.h"
#include "EMMessageBox.h"
#include "EMMessages.h"
#include "EMResourceRepository.h"
#include "EMSettingIDs.h"
#include "EMSettingsRepository.h"
#include "EMTextControl.h"
#include "EMView.h"
#include "EMWindow.h"
#include "ProjectManager.h"
#include "ResourceIDs.h"

#ifdef PLATFORM_WINDOWS
#include <windows.h> // To get modal dialog to work.
#include "EMWinWindow.h" // To get modal dialog to work.
#include <windowsx.h>
#include <shlobj.h>
#endif

const uint32 EM_BUTTON_BROWSE = 1;
const uint32 EM_BUTTON_OK = 2;
const uint32 EM_BUTTON_CANCEL = 3;
const uint32 EM_TEXT_CONTROL_CHANGE = 4;
const uint32 EM_BUTTON_LOAD_PROJECT = 5;
const uint32 EM_PROJECT_LOADED = 6;

/*enum Results  // Temporary put in EMMessageBox
{
	RESULT_OK,
	RESULT_NOT_OK,
	RESULT_NO_BUTTON,
	RESULT_CANCEL_BUTTON,
};
*/
NewProjectDialog::NewProjectDialog() :
m_opDialogWindow(EMFactory::Instance() -> CreateEMWindow(EMRect(400, 200, 400 + 443, 200 + 217), "New Project", EM_WINDOW_DIALOG)), 
m_opMainView(EMFactory::Instance() -> CreateView(EMRect(0, 0, 443, 217), 0)),
m_opProjectNameLabel(EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
																 EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 3,
																 EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 68,
																 EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 3 + EM_METRICS_NATIVE_LABEL_HEIGHT),
														  "Project Name:", 0)),
m_opCreateInLabel(EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
															  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 32,
															  EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 68,
															  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 32 + EM_METRICS_NATIVE_LABEL_HEIGHT),
													   "Create In:", 0)),
m_opTemplateLabel(EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
															  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 62,
															  EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 68,
															  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 62 + EM_METRICS_NATIVE_LABEL_HEIGHT),
													   "Template:", 0)),
m_opProjectNameTextControl(EMFactory::Instance() -> CreateTextControl(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 72,
																			 EM_METRICS_NATIVE_DIALOG_MARGIN_TOP,
																			 EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 72 + 266,
																			 EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + EM_METRICS_NATIVE_TEXT_CONTROL_HEIGHT),
																	  "", EM_TEXT_CONTROL_CHANGE, 0, false)),
m_opCreateInTextControl(EMFactory::Instance() -> CreateTextControl(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 72,
																		  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 30,
																		  EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 72 + 266,
																		  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 30 + EM_METRICS_NATIVE_TEXT_CONTROL_HEIGHT),
																   "", EM_TEXT_CONTROL_CHANGE, 0, false)), 
m_opTemplateListBox(EMFactory::Instance() -> CreateListBox(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 72,
																  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 60,
																  EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 72 + 349,
																  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 60 + 130),
														   0, 0, false, false)),
/*m_opBrowseButton(EMFactory::Instance() -> CreateButton(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 347,
															  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP,
															  EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 347 + EM_METRICS_NATIVE_BUTTON_WIDTH,
															  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + EM_METRICS_NATIVE_BUTTON_HEIGHT),
													   "Browse...", EM_BUTTON_BROWSE, 0, 0, m_opMainView ->GetNativeView())),
m_opOK_Button(EMFactory::Instance() -> CreateButton(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 347,
														   EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 146,
														   EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 347 + EM_METRICS_NATIVE_BUTTON_WIDTH,
														   EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 146 + EM_METRICS_NATIVE_BUTTON_HEIGHT),
													"OK", EM_BUTTON_OK, 0, 0, m_opMainView ->GetNativeView(), false)),
m_opCancelButton(EMFactory::Instance() -> CreateButton(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 347,
															  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 146 + EM_METRICS_NATIVE_BUTTON_HEIGHT + EM_METRICS_NATIVE_BUTTON_SPACE,
															  EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 347 + EM_METRICS_NATIVE_BUTTON_WIDTH,
															  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 146 + EM_METRICS_NATIVE_BUTTON_HEIGHT + EM_METRICS_NATIVE_BUTTON_SPACE + EM_METRICS_NATIVE_BUTTON_HEIGHT),
													   "Cancel", EM_BUTTON_CANCEL, 0, 0, m_opMainView ->GetNativeView())),
*/
m_vNoProjectLoaded(true),
m_opMsgBox(EMFactory::Instance() -> CreateMessageBox()),
m_vCreateInChanged(false),
m_vProjectNameChanged(false),
m_opBrowseWindow(NULL)
{
m_opBrowseButton = EMFactory::Instance() -> CreateButton(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 347,
															  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 30,
															  EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 347 + EM_METRICS_NATIVE_BUTTON_WIDTH,
															  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 30 + EM_METRICS_NATIVE_BUTTON_HEIGHT),
													   "Browse...", EM_BUTTON_BROWSE, 0, 0, m_opDialogWindow);
m_opOK_Button = EMFactory::Instance() -> CreateButton(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 347 - (EM_METRICS_NATIVE_BUTTON_WIDTH + EM_METRICS_NATIVE_BUTTON_SPACE),
															  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 146 + EM_METRICS_NATIVE_BUTTON_HEIGHT + EM_METRICS_NATIVE_BUTTON_SPACE,
															  EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 347 - EM_METRICS_NATIVE_BUTTON_SPACE,
															  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 146 + EM_METRICS_NATIVE_BUTTON_HEIGHT + EM_METRICS_NATIVE_BUTTON_SPACE + EM_METRICS_NATIVE_BUTTON_HEIGHT),
													"OK", EM_BUTTON_OK, 0, 0, m_opDialogWindow, true);
m_opCancelButton = EMFactory::Instance() -> CreateButton(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 347,
															  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 146 + EM_METRICS_NATIVE_BUTTON_HEIGHT + EM_METRICS_NATIVE_BUTTON_SPACE,
															  EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 347 + EM_METRICS_NATIVE_BUTTON_WIDTH,
															  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 146 + EM_METRICS_NATIVE_BUTTON_HEIGHT + EM_METRICS_NATIVE_BUTTON_SPACE + EM_METRICS_NATIVE_BUTTON_HEIGHT),
													   "Cancel", EM_BUTTON_CANCEL, 0, 0, m_opDialogWindow);
m_opLoadProjectButton = EMFactory::Instance() -> CreateButton(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 347,
															  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP,
															  EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 347 + EM_METRICS_NATIVE_BUTTON_WIDTH,
															  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + EM_METRICS_NATIVE_BUTTON_HEIGHT),
													   "Load Project...", EM_BUTTON_LOAD_PROJECT, 0, 0, m_opDialogWindow);


	//"All Files\0*.*\0Media Files\0*.wav;*.avi;*.mpg;*.mov\0Audio File\0*.wav\0Video Files\0*.avi;*.mpg;*.mov\0"
/*	vector<string> test;
	test.push_back(string("All Files"));
	test.push_back(string("*.*"));
	test.push_back(string("MediaFiles"));
	test.push_back(string("*.wav;"));
	test.push_back(string("*.avi;"));
	test.push_back(string("*.mpg;"));
	test.push_back(string("*.mov"));
	test.push_back(string("AudioFiles"));
	test.push_back(string("*.wav"));
	test.push_back(string("Video Files"));
	test.push_back(string("*.avi;"));
	test.push_back(string("*.mpg;"));
	test.push_back(string("*.mov"));
*/
	vector<string> oVisibleExtensions;
	oVisibleExtensions.push_back(string("Project Files"));
	oVisibleExtensions.push_back(string("*.tpf"));



	m_opFileWindow = EMFactory::Instance() ->CreateFileWindow(FILE_PANEL_LOAD_FILE, "Load Project", m_opDialogWindow ->GetNativeView(), false, "", &oVisibleExtensions, "", EM_PROJECT_LOADED);
	m_opDialogWindow -> AddChild(m_opMainView);
	m_opMainView -> AddChild(m_opCreateInLabel);
	m_opMainView -> AddChild(m_opProjectNameLabel);
	m_opMainView -> AddChild(m_opTemplateLabel);
	m_opMainView -> AddChild(m_opCreateInTextControl);
	m_opMainView -> AddChild(m_opProjectNameTextControl);
	m_opMainView -> AddChild(m_opTemplateListBox);
	m_opMainView -> AddChild(m_opBrowseButton);
	m_opMainView -> AddChild(m_opOK_Button);
	m_opMainView -> AddChild(m_opCancelButton);
	m_opMainView -> AddChild(m_opLoadProjectButton);
}

NewProjectDialog::~NewProjectDialog()
{
	delete m_opDialogWindow;
	delete m_opMainView;
	delete m_opCreateInLabel;
	delete m_opProjectNameLabel;
	delete m_opTemplateLabel;
	delete m_opCreateInTextControl;
	delete m_opProjectNameTextControl;
	delete m_opTemplateListBox;
	delete m_opBrowseButton;
	delete m_opOK_Button;
	delete m_opCancelButton;
	delete m_opLoadProjectButton;
}

//Compare ProjectName with CreateIn and add change in ProjectName to CreateIn
void NewProjectDialog::AddToCreateInProjectTextControl()
{
	string oProjectName(m_opProjectNameTextControl -> Text());
	string oCreateIn(m_opCreateInTextControl -> Text());
	bool vSame = true;

	int vPos(oCreateIn.find_last_of('\\'));

	for(int vCounter = 0; vPos + 1 < oCreateIn.length() && vCounter < oProjectName.length(); vCounter++)
	{
		if(oCreateIn.at(++vPos) != oProjectName.at(vCounter))
		{
			if(m_vCreateInChanged)
			{
				vSame = false;
				break;
			}
			if(oCreateIn.at(vPos) != oProjectName.at((oProjectName.length() == vCounter + 1) ? vCounter : vCounter + 1))
			{
				if(oCreateIn.at(++vPos) != oProjectName.at(vCounter))
					vSame = false;
			}
//			else
//				eo << "Extra test Same" << ef;
		}
//		else
//			eo << "Same" << ef;
	}

	int test = ((oCreateIn.length() - 1) - vPos);

	if(oProjectName.length() == 1)
		if(vPos != oCreateIn.length() - 1)
			if(oCreateIn[vPos] == oProjectName[0] && (((oCreateIn.length() - 1) - vPos) == 1))
				vSame = true;
			else
				vSame = false;
	
	if(vSame)
	{
		vPos = oCreateIn.find_last_of('\\');
		oCreateIn.erase(vPos + 1, oCreateIn.length());
		oCreateIn.append(oProjectName);
		if(oProjectName.find_first_of('\\') != -1)
		{
			oProjectName.erase();
			m_opProjectNameTextControl -> SetText(oProjectName.c_str());
		}
	}
	else
	{
		oCreateIn.append("\\");
		oCreateIn.append(oProjectName);
	}

	m_opCreateInTextControl -> SetText(oCreateIn.c_str());
}

//Checking if 2MB is available on disk
bool NewProjectDialog::CheckDiskSpaceOK(const char* p_vpDirectory)
{
#ifdef PLATFORM_WINDOWS
	HINSTANCE vInstance = LoadLibrary("Kernel32.dll");
	typedef BOOL (WINAPI *MyFunc)(LPCSTR RootName, PULARGE_INTEGER pulA, PULARGE_INTEGER pulB, PULARGE_INTEGER pulFreeBytes);
	MyFunc pfnGetDiskFreeSpaceEx = (MyFunc)GetProcAddress(vInstance, "GetDiskFreeSpaceExA");
	FreeLibrary(vInstance); 
	ULARGE_INTEGER vFreeBytesAvailableToCaller;
	ULARGE_INTEGER vTotalNumberOfBytes;
	ULARGE_INTEGER vTotalNumberOfFreeBytes;

	int vTest = pfnGetDiskFreeSpaceEx(p_vpDirectory, &vFreeBytesAvailableToCaller, &vTotalNumberOfBytes, &vTotalNumberOfFreeBytes);
	if(vTest == 0)
	{
		eo << "GetDiskFreeSpaceEx faild!!" << ef;
		return false;
	}
	eo << "Free DiskSpace = " << (int)((vTotalNumberOfFreeBytes.QuadPart / 1024) / 1024) << "MB" << ef;
	if(((vTotalNumberOfFreeBytes.QuadPart / 1024) / 1024) > 1) // i.e. more than 2MB free disk space
		return true;
#endif
	return false;
}

//Creating project directory
int NewProjectDialog::CreateDir(const char* p_vpDirectory)
{
#ifdef PLATFORM_WINDOWS
	string oDirectoryTemp = p_vpDirectory;//m_opCreateInTextControl -> Text();
	int vPos;

	if(!IsDirectoryValid(&oDirectoryTemp))
	{
		m_opMsgBox -> DisplayDialog(m_opParentWindow -> GetNativeView(), "Can't create directory!\nInvalid drive!", "Create Directory Error!", EM_DIALOG_ICON_ERROR);
		return RESULT_NOT_OK;
	}

	if(!CheckDiskSpaceOK(oDirectoryTemp.c_str()))
	{
		m_opMsgBox -> DisplayDialog(m_opParentWindow -> GetNativeView(), "Not enough diskspace!\nChoose another drive.", "Diskspace Error!", EM_DIALOG_ICON_ERROR);
		return RESULT_NOT_OK;
	}


	if(SetCurrentDirectory(oDirectoryTemp.c_str()))
	{
		//Check parent folders for project_data.tpf
		if(SearchProjectFile())
		{
			m_opMsgBox -> DisplayDialog(m_opDialogWindow -> GetNativeView(), "A projectfile was found in a parent folder\nChoose another folder and try again", "Project file exist", EM_DIALOG_ICON_ERROR);
			return RESULT_NOT_OK;
		}
	}
	else
	{
		//If we get here something is wrong! Check if "m_oValidDirectory" is valid!
		EMDebugger("Could not set current directory!");
		return RESULT_NOT_OK;
	}

	vPos = oDirectoryTemp.find_first_of("\\");
	if(oDirectoryTemp[vPos + 1] == '\\' && oDirectoryTemp[oDirectoryTemp.length() - 1] != '\\')
		oDirectoryTemp.append("\\");
	
	WIN32_FIND_DATA sFileData;
	HANDLE vFileHandle = FindFirstFile(m_opCreateInTextControl -> Text()/*oDirectoryTemp.c_str()*/, &sFileData);
	if(vFileHandle == INVALID_HANDLE_VALUE)
	{
/*		string oTemp = "Folder:\n";
		oTemp.append(m_opCreateInTextControl -> Text());
		oTemp.append("\ndoes not exist!\nCreate folder?");

		int vResult = m_opMsgBox -> DisplayDialog(m_opDialogWindow -> GetNativeView(), oTemp.c_str(), "Folder don´t exist", EM_DIALOG_BUTTON_YES_NO_CANCEL | EM_DIALOG_ICON_ERROR);
		if(vResult == EM_DIALOG_NO_BUTTON)
			return RESULT_NO_BUTTON;
		if(vResult == EM_DIALOG_CANCEL_BUTTON)
			return RESULT_CANCEL_BUTTON;
*/
		if(!CreateFullPath(m_opCreateInTextControl -> Text()))
			EMDebugger("Could Not Create Directory\nPossible fault: Parent Directory does not exist");
		return RESULT_OK;
	}
#endif
	return RESULT_OK;
}

//Createing the full path of the directory
bool NewProjectDialog::CreateFullPath(const char* p_opFullPath)
{
#ifdef PLATFORM_WINDOWS
	string oPathTemp(p_opFullPath);
	string oValidPathTemp(p_opFullPath);
	char vTemp[255];
	int vPos(0);
	int vValidPos(0);

	if(!IsDirectoryValid(&oValidPathTemp)) //Getting the first valid directory
		return false;

	vValidPos = oValidPathTemp.find_last_of('\\');
	if(vValidPos < oValidPathTemp.length() - 1)
	{
		vValidPos = oValidPathTemp.length();
		oValidPathTemp.append("\\");
	}

	while(oValidPathTemp.length() != oPathTemp.length() + 1)
	{
		vValidPos = oValidPathTemp.find_first_of('\\', vValidPos);
//		if(vValidPos == -1)
//			vValidPos = oPathTemp.length();
//		else
			vValidPos++;
		vPos = oPathTemp.find_first_of('\\', vValidPos);
		if(vPos == -1)
			vPos = oPathTemp.length();
		oPathTemp.copy(vTemp, vPos - (vValidPos), vValidPos);
		vTemp[vPos - (vValidPos)] = 0;
		oValidPathTemp.append(vTemp);
eo << "Creating folder: " << (char*)oValidPathTemp.c_str() << ef;
		CreateDirectory(oValidPathTemp.c_str(), NULL);
		oValidPathTemp.append("\\");
	}
#endif
	return true;
}

void NewProjectDialog::DisplayDialog()
{
	if(m_opDialogWindow -> Lock())
	{
		if(!GetTemplates())
			return;
		if(EMSettingsRepository::Instance() -> SettingExists(SETTING_PROJECT_ID) == false)
		{
			m_vNoProjectLoaded = true;
			m_opDialogWindow ->SetTitle("New/Load Project");
			m_opCancelButton -> SetLabel("Quit");
		}
		else
		{
			if(*static_cast<int*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_PROJECT_ID)) == 0)
			{
				m_vNoProjectLoaded = true;
				m_opDialogWindow ->SetTitle("New/Load Project");
				m_opCancelButton -> SetLabel("Quit");
			}
			else
			{
				m_vNoProjectLoaded = false;
				m_opDialogWindow ->SetTitle("New Project");
				m_opCancelButton -> SetLabel("Cancel");
				m_opLoadProjectButton -> Hide();
			}
		}
		if(m_oOldCreateInTextControlString.size() != 0)
		{
			m_opCreateInTextControl -> SetText(m_oOldCreateInTextControlString.c_str());
			m_opProjectNameTextControl -> SetText("");
		}
		else
		{
#ifdef PLATFORM_WINDOWS
			char vPath[MAX_PATH];
			SHGetSpecialFolderPath(NULL, vPath, CSIDL_PERSONAL, false);
			m_opProjectNameTextControl -> SetText("");
			m_opCreateInTextControl -> SetText(vPath);
#endif
		}
		if(strlen(m_opProjectNameTextControl -> Text()) == 0 || strlen(m_opCreateInTextControl -> Text()) == 0)
			m_opOK_Button -> Enable(false);
		else
			m_opOK_Button -> Enable(true);

		m_opOK_Button -> Enable(false);

		m_opDialogWindow -> Show();
#ifdef PLATFORM_WINDOWS
		EnableWindow(dynamic_cast<EMWinWindow*>(m_opDialogWindow) -> GetMainWindowHandle(), FALSE);
		SetFocus(static_cast<HWND>(m_opDialogWindow -> GetNativeView()));
#endif
	
		m_opDialogWindow -> Unlock();
	}
}

bool NewProjectDialog::GetTemplates()
{
#ifdef PLATFORM_WINDOWS
	WIN32_FIND_DATA sFileData;
	string oTemp;
	int vID(0);
	uint32 vDirectoryID = DIRECTORY_ID_TEMPLATES;
	string oTemplateDirectory;
	oTemplateDirectory = *static_cast<string*>(EMCommandRepository::Instance() -> ExecuteCommand(PROJECT_COMMAND_GET_DIRECTORY, &vID, &vDirectoryID));

	if(!SetCurrentDirectory(oTemplateDirectory.c_str()))
	{
		m_opMsgBox -> DisplayDialog(m_opDialogWindow -> GetNativeView(), "Could not find template directory!\nReinstall TITAN and try again.\n\nIf the problem remains contact Elegant Media AB\nat: bugs@elegant-media.com", "Directory ERROR", EM_DIALOG_ICON_ERROR);
		eo << "Template Directory do not exist" << ef;
		return false;
	}
	HANDLE hHandle(FindFirstFile("*.tte", &sFileData));
	if(hHandle == INVALID_HANDLE_VALUE)
	{
		m_opMsgBox -> DisplayDialog(m_opDialogWindow -> GetNativeView(), "Could not find templates!\nReinstall TITAN and try again.\n\nIf the problem remains contact Elegant Media AB\nat: bugs@elegant-media.com", "Missing file, ERROR", EM_DIALOG_ICON_ERROR);
		eo << "Template files do not exist" << ef;
		return false;
	}

	m_opTemplateListBox -> Clear();

	if(strcmp(".",sFileData.cFileName) != 0 && strcmp("..",sFileData.cFileName) != 0)
	{
		eo << sFileData.cFileName << ef;
		oTemp.erase();
		oTemp.append(sFileData.cFileName);
		oTemp.erase(oTemp.find_last_of('.'), oTemp.length());
		m_opTemplateListBox -> AddString(oTemp.c_str());
	}
	while(FindNextFile(hHandle, &sFileData))
	{
		if(strcmp(".",sFileData.cFileName) != 0 && strcmp("..",sFileData.cFileName) != 0)
		{
			eo << sFileData.cFileName << ef;
			oTemp.erase();
			oTemp.append(sFileData.cFileName);
			oTemp.erase(oTemp.find_last_of('.'), oTemp.length());
			m_opTemplateListBox -> AddString(oTemp.c_str());
		}
	}
	FindClose(hHandle);
	m_opTemplateListBox -> SelectItem(0);
#endif
	return true;
}

void NewProjectDialog::HideDialog()
{
	m_vNoProjectLoaded = false;
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

void NewProjectDialog::Init()
{
	m_opDialogWindow -> AddListener(this);
	m_opMainView -> AddListener(this);
	if(m_opDialogWindow -> Lock())
	{
		//m_opMainView -> SetViewColor(EMColor(192, 192, 192, 255));
		m_opMainView -> SetDefaultColor();
		m_opDialogWindow -> Unlock();
	}
	m_opBrowseButton -> AddListener(this);
	m_opOK_Button -> AddListener(this);
	m_opCancelButton -> AddListener(this);
	m_opCreateInTextControl -> AddListener(this);
	m_opProjectNameTextControl -> AddListener(this);
	m_opLoadProjectButton -> AddListener(this);
	m_opFileWindow -> AddListener(this);
	m_opTemplateListBox -> AddListener(this);

	//Setting TabOrder
	m_opDialogWindow -> AddTabComponent(m_opProjectNameTextControl);
	m_opDialogWindow -> AddTabComponent(m_opCreateInTextControl);
	m_opDialogWindow -> AddTabComponent(m_opTemplateListBox);
	m_opDialogWindow -> AddTabComponent(m_opLoadProjectButton);
	m_opDialogWindow -> AddTabComponent(m_opBrowseButton);
	m_opDialogWindow -> AddTabComponent(m_opOK_Button);
	m_opDialogWindow -> AddTabComponent(m_opCancelButton);

#ifdef PLATFORM_WINDOWS
	char vPath[MAX_PATH];
	SHGetSpecialFolderPath(NULL, vPath, CSIDL_PERSONAL, false);
	m_opCreateInTextControl -> SetText(vPath);
#endif

//	GetTemplates();
}

//Checking if an invalid character is use in ProjectName and CreateIn
bool NewProjectDialog::InvalidCharExist()
{
	string oCreateIn(m_opCreateInTextControl -> Text());
	string oProjectName(m_opProjectNameTextControl -> Text());
	bool vCreateInInvalidCharExist(false);
	bool vProjectNameInvalidCharExist(false);
	int vPos(0);

	while(vPos < oCreateIn.length() && vPos != oCreateIn.npos)
	{
		vPos = oCreateIn.find_first_of('\\', vPos);
		if(vPos == 0)
		{
			vPos++;
			continue;
		}
		if(vPos == 1)
		{
			vPos++;
			continue;
		}
		if(oCreateIn[vPos + 1] == '\\' && vPos != oCreateIn.npos)
		{
			vCreateInInvalidCharExist = true;
			break;
		}
		if(vPos != oCreateIn.npos)
			vPos++;
	}

	// Check for "space" in each directory
	vPos = 0;
	while(vPos < oCreateIn.length() && vPos != oCreateIn.npos)
	{
		vPos = oCreateIn.find_first_of('\\', vPos);
		if(vPos == 0)
			continue;
		if(oCreateIn[vPos - 1] == ' ' && vPos != oCreateIn.npos)
			vCreateInInvalidCharExist = true;
		if(vPos != oCreateIn.npos)
			vPos++;
	}

	if(oCreateIn.find_first_of('/') != -1)
		vCreateInInvalidCharExist = true;
	if(oCreateIn.find_first_of(':') != -1 && oCreateIn.find_first_of(':') != 1)
		vCreateInInvalidCharExist = true;
	if(oCreateIn.find_first_of('*') != -1)
		vCreateInInvalidCharExist = true;
	if(oCreateIn.find_first_of('?') != -1)
		vCreateInInvalidCharExist = true;
	if(oCreateIn.find_first_of('"') != -1)
		vCreateInInvalidCharExist = true;
	if(oCreateIn.find_first_of('<') != -1)
		vCreateInInvalidCharExist = true;
	if(oCreateIn.find_first_of('>') != -1)
		vCreateInInvalidCharExist = true;
	if(oCreateIn.find_first_of('|') != -1)
		vCreateInInvalidCharExist = true;
	
	if(vCreateInInvalidCharExist)
		m_opMsgBox -> DisplayDialog(m_opDialogWindow -> GetNativeView(), "The specified path has invalid character(s)\nand/or some directories ends with a \"space\"", "Invalid character(s)", EM_DIALOG_ICON_ERROR);

	if(oProjectName.find_first_of('\\') != -1)
		vProjectNameInvalidCharExist = true;
	if(oProjectName.find_first_of('/') != -1)
		vProjectNameInvalidCharExist = true;
	if(oProjectName.find_first_of(':') != -1)
		vProjectNameInvalidCharExist = true;
	if(oProjectName.find_first_of('*') != -1)
		vProjectNameInvalidCharExist = true;
	if(oProjectName.find_first_of('?') != -1)
		vProjectNameInvalidCharExist = true;
	if(oProjectName.find_first_of('"') != -1)
		vProjectNameInvalidCharExist = true;
	if(oProjectName.find_first_of('<') != -1)
		vProjectNameInvalidCharExist = true;
	if(oProjectName.find_first_of('>') != -1)
		vProjectNameInvalidCharExist = true;
	if(oProjectName.find_first_of('|') != -1)
		vProjectNameInvalidCharExist = true;
	if(oProjectName.find_last_of(' ') == oProjectName.length() - 1)
		vProjectNameInvalidCharExist = true;

	if(vProjectNameInvalidCharExist)
		m_opMsgBox -> DisplayDialog(m_opDialogWindow -> GetNativeView(), "The Project Name has invalid character(s)\nand/or ends with a \"space\"", "Invalid character(s)", EM_DIALOG_ICON_ERROR);

	if(vCreateInInvalidCharExist || vProjectNameInvalidCharExist)
		return true;
	return false;
}

//Checking if directory is valid. If not, string gets the closest valid directory
bool NewProjectDialog::IsDirectoryValid(string* p_opDirectory)
{
	bool vUNC = false;
	string oDirectoryTemp(p_opDirectory -> c_str());
	int vPos(oDirectoryTemp.find_first_of("\\"));
	if(oDirectoryTemp[vPos + 1] == '\\')
	{
		vUNC = true;
		oDirectoryTemp.append("\\");
	}

	while(!SetCurrentDirectory(oDirectoryTemp.c_str()))
	{
		if(vUNC)
		{
			oDirectoryTemp.erase(oDirectoryTemp.length() - 1);
			if(oDirectoryTemp.find_last_of('\\') == 1)
				return false;
		}

		vPos = oDirectoryTemp.find_last_of("\\");
		if(vPos == oDirectoryTemp.length())
		{
			oDirectoryTemp.erase(oDirectoryTemp.length());
			vPos = oDirectoryTemp.find_first_of("\\");
		}
		else if(vPos == -1)
		{
			p_opDirectory -> erase();
			return false;
		}
		oDirectoryTemp.erase(vPos);
		if(vUNC)
			oDirectoryTemp.append("\\");
		if(oDirectoryTemp.at(vPos - 1) == ':')
		{
			oDirectoryTemp.append("\\");
			if(!SetCurrentDirectory(oDirectoryTemp.c_str()))
				return false;
			else
				break;
		}
	}
	p_opDirectory -> erase();
	p_opDirectory -> append(oDirectoryTemp.c_str());
	return true;
}

void NewProjectDialog::LoadTemplate()
{
	// Assume that no project is loaded
	if(m_opParentWindow -> Lock())
	{
		string oPath(m_opCreateInTextControl -> Text());
		string oTemplate(m_opTemplateListBox -> GetSelection());
		EMCommandRepository::Instance() -> ExecuteCommand(PROJECT_COMMAND_LOAD_TEMPLATE, const_cast<char*>(oPath.c_str()), const_cast<char*>(oTemplate.c_str()));

		EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_CLEAR_UNDO_STACK);				
		EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_SET_UNDO_TEXTS);				
		
		string oProjectName = oPath.substr(oPath.find_last_of(EM_FILE_SEPARATOR) + 1, oPath.length() - 1);
		//string oWindowTitle = string(EMResourceRepository::GetString(RES_SR_APPLICATION_NAME) -> c_str()) + " - " + oProjectName;
		//m_opParentWindow -> SetTitle(oWindowTitle.c_str());
		//int vNameBeginPos = oPath.find_last_of(EM_FILE_SEPARATOR, oPath.find_last_of(EM_FILE_SEPARATOR) - 1) + 1;
		//int vNameEndPos = oPath.find_last_of(EM_FILE_SEPARATOR) - 1;
		//string oProjectName = oPath.substr(vNameBeginPos, vNameEndPos - vNameBeginPos + 1);
		string oWindowTitle = string(EMResourceRepository::GetString(RES_SR_APPLICATION_NAME) -> c_str()) + " " + EMApplication::GetApplicationVersionNumber() +
										" - " + oProjectName;
		m_opParentWindow -> SetTitle(oWindowTitle.c_str());

		m_opParentWindow -> Unlock();
	}

	EMCommandRepository::Instance() -> ExecuteCommand(PROJECT_COMMAND_READY_FOR_USE);

	return;
}

bool NewProjectDialog::MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage)
{
	switch(p_vMessage)
	{
	case EM_BUTTON_BROWSE:
		{
			eo << "Browse button pressed" << ef;
			if(m_opBrowseWindow == NULL)
				m_opBrowseWindow = EMFactory::Instance() -> CreateBrowseWindow(m_opMainView -> GetNativeView(), "Browse");
			if(m_opBrowseWindow -> DisplayDialog())
			{
				m_oValidDirectory = m_opBrowseWindow -> GetPath() -> c_str();
				if(strlen(m_opProjectNameTextControl -> Text()) > 0)
				{
					int vPos(m_opBrowseWindow -> GetPath() -> find_first_of('\\'));
					//int test(m_opBrowseWindow -> GetPath() -> length());
					if(vPos != (m_opBrowseWindow -> GetPath() -> length() - 1))
						m_opBrowseWindow -> GetPath() -> append("\\");
					m_opBrowseWindow -> GetPath() -> append(m_opProjectNameTextControl -> Text());
				}
				m_opCreateInTextControl -> SetText(m_opBrowseWindow -> GetPath() -> c_str());
			}
		/*	string temp("C:\\Titan_Project2");
			int vProjectID = *(static_cast<int*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_PROJECT_ID)));
			if(m_opWindow ->Lock())
			{
				EMCommandRepository::Instance() -> ExecuteCommand(PROJECT_COMMAND_LOAD_PROJECT, &vProjectID, &temp);
				m_opWindow ->Unlock();
			}
			EMCommandRepository::Instance() -> ExecuteCommand(PROJECT_COMMAND_READY_FOR_USE);
		*/
			return true;
		}
	case EM_GUI_COMPONENT_ENTER_PRESSED:
		if(m_opOK_Button -> IsEnabled() == false)
			return true;
	case EM_BUTTON_OK:
		{
			eo << "OK button pressed" << ef;
			
			if(InvalidCharExist())
			{
				eo << "Invalid Character found!!" << ef;
				return true;
			}
			int vResult = CreateDir(m_opCreateInTextControl -> Text());
			switch(vResult)
			{
			case RESULT_OK:
				if(*static_cast<bool*>(EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_NEW_PROJECT, NULL, NULL, NULL)))
				{
					LoadTemplate();
					m_oOldCreateInTextControlString = m_opCreateInTextControl -> Text();
				}
				else
					return true;
				break;
			case RESULT_NOT_OK:
//			case RESULT_NO_BUTTON:
				eo << "Go back to dialog" << ef;
				return true;
/*			case RESULT_CANCEL_BUTTON:
				eo << "Close dialog and do nothing" << ef;
				break;
*/			}

			HideDialog();
			return true;
		}
	case EM_GUI_COMPONENT_ESC_PRESSED:
	case EM_BUTTON_CANCEL:
		eo << "Cancel button pressed" << ef;
		if(m_vNoProjectLoaded == false)
			HideDialog();
		else
		{
			switch(m_opMsgBox -> DisplayDialog(m_opDialogWindow -> GetNativeView(), "This will exit Elegant Media Titan!\nContinue?", "Titan - Warning", EM_DIALOG_ICON_WARNING | EM_DIALOG_BUTTON_OK_CANCEL))
			{
			case EM_DIALOG_OK_BUTTON:
				eo << " *** EXIT TITAN ***" << ef;
				//HideDialog(); // FIXME: Should exit application. Not hide dialog.
				EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_QUIT_APPLICATION);
				break;
			case EM_DIALOG_CANCEL_BUTTON:
				eo << "Do nothing." << ef;
				break;
			}
		}

		return true;
	case EM_BUTTON_LOAD_PROJECT:
		eo << "Load project button pressed" << ef;
		m_opFileWindow -> Show();
		return true;
	case EM_TEXT_CONTROL_CHANGE:
		eo << "EM_TEXT_CONTROL_PROJ_NAME_CHANGE" << ef;
		if(strlen(m_opProjectNameTextControl -> Text()) == 0 || strlen(m_opCreateInTextControl -> Text()) == 0)
			m_opOK_Button -> Enable(false);
		else
		{
			if(OnlyRoot())
				m_opOK_Button -> Enable(false);
			else
				m_opOK_Button -> Enable(true);
		}
		if(p_opSender == m_opCreateInTextControl)
		{
			if(m_vProjectNameChanged)
			{
				m_vCreateInChanged = false;
				m_vProjectNameChanged = false;
			}
			else
				m_vCreateInChanged = true;
		}
		if(p_opSender == m_opProjectNameTextControl)
		{
			m_vProjectNameChanged = true;
			AddToCreateInProjectTextControl();
		}
		return true;
	case EM_PROJECT_LOADED:
		// Assume that no project is loaded
		if(m_opParentWindow -> Lock())
		{
			string oPath = (*(m_opFileWindow -> GetPaths()))[0];
			EMCommandRepository::Instance() -> ExecuteCommand(PROJECT_COMMAND_LOAD_PROJECT, const_cast<char*>(oPath.c_str()));

			//string oProjectName = oPath.substr(oPath.find_last_of(EM_FILE_SEPARATOR) + 1, oPath.length() - 1);
			//string oWindowTitle = string(EMResourceRepository::GetString(RES_SR_APPLICATION_NAME) -> c_str()) + " - " + oProjectName;
			int vNameBeginPos = oPath.find_last_of(EM_FILE_SEPARATOR, oPath.find_last_of(EM_FILE_SEPARATOR) - 1) + 1;
			int vNameEndPos = oPath.find_last_of(EM_FILE_SEPARATOR) - 1;
			string oProjectName = oPath.substr(vNameBeginPos, vNameEndPos - vNameBeginPos + 1);
			string oWindowTitle = string(EMResourceRepository::GetString(RES_SR_APPLICATION_NAME) -> c_str()) + " " + EMApplication::GetApplicationVersionNumber() +
										" - " + oProjectName;
			m_opParentWindow -> SetTitle(oWindowTitle.c_str());

			EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_CLEAR_UNDO_STACK);		
			EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_SET_UNDO_TEXTS);				

			m_opParentWindow -> Unlock();
		}

		EMCommandRepository::Instance() -> ExecuteCommand(PROJECT_COMMAND_READY_FOR_USE);
		HideDialog();

		return true;
	case EM_GUI_COMPONENT_TAB_PRESSED:
		m_opDialogWindow -> NextTab();
		return true;
	case EM_GUI_COMPONENT_SHIFT_TAB_PRESSED:
		m_opDialogWindow -> PreviousTab();
		return true;
	case EM_GUI_COMPONENT_GOT_FOCUS:
		if(p_opSender == m_opDialogWindow )
			m_opProjectNameTextControl -> SetFocus();
		m_opDialogWindow -> SetCurrentFocus();
		return true;
	case EM_CLOSE_WINDOW:
		if(p_opSender == m_opDialogWindow)
		{
			if(m_vNoProjectLoaded)
			{
				switch(m_opMsgBox -> DisplayDialog(m_opDialogWindow -> GetNativeView(), "This will exit Elegant Media Titan!\nContinue?", "Titan - Warning", EM_DIALOG_ICON_WARNING | EM_DIALOG_BUTTON_OK_CANCEL))
				{
				case EM_DIALOG_OK_BUTTON:
					eo << " *** EXIT TITAN ***" << ef;
					//HideDialog(); // FIXME: Should exit application. Not hide dialog.
					EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_QUIT_APPLICATION);
					break;
				case EM_DIALOG_CANCEL_BUTTON:
					eo << "Do nothing." << ef;
					break;
				}
				return true;
			}
			eo << "EM_QUIT_PRE_REQUESTED" << ef;
			HideDialog();
			return true;
		}
		break;
	}
	return false;
}

void NewProjectDialog::MoveDialog()
{
	EMRect oWindowRect(m_opParentWindow -> Frame());
	EMRect oDialogRect(m_opDialogWindow -> Frame());
	float vX(oWindowRect.m_vRight - oWindowRect.GetWidth() / 2 - oDialogRect.GetWidth() / 2);
	float vY(oWindowRect.m_vBottom - oWindowRect.GetHeight() / 2 - oDialogRect.GetHeight() / 2);

	MoveDialog(vX, vY);
}

void NewProjectDialog::MoveDialog(float p_vX, float p_vY)
{
	m_opDialogWindow -> MoveTo(p_vX, p_vY);
}

bool NewProjectDialog::OnlyRoot()
{
	string oPath(m_opCreateInTextControl -> Text());
	if(oPath.length() <= 3)
	{
		if(IsDirectoryValid(&oPath))
			return true;
	}
	return false;
}

//Checking if a parent folder exist
bool NewProjectDialog::ParentFolderExist()
{
#ifdef PLATFORM_WINDOWS
	char vTemp[255];
	GetCurrentDirectory(255, vTemp);
	string oTemp(vTemp);
	int vPos(0);
	if(oTemp[0] == '\\' && oTemp[1] == '\\')
	{
		vPos = oTemp.find_first_of('\\', 2);
		vPos = oTemp.find_first_of('\\', vPos + 1);
		if(vPos == oTemp.length() - 1 || vPos == -1)
			return false;
	}
	WIN32_FIND_DATA sFileData;
	HANDLE vFileHandle(FindFirstFile("*.*", &sFileData));
	if(strcmp("..",sFileData.cFileName) == 0)
	{
		FindClose(vFileHandle);
		return true;
	}
	while(FindNextFile(vFileHandle, &sFileData))
	{
		if(strcmp("..",sFileData.cFileName) == 0)
		{
			FindClose(vFileHandle);
			return true;
		}
	}
#endif
	return false;
}

//Locking for a projectfile in parent directoies
bool NewProjectDialog::SearchProjectFile()
{
#ifdef PLATFORM_WINDOWS
	WIN32_FIND_DATA sFileData;
	bool vProjectFileExist(false);
	char vProjectFileName[] = "project_data.tpf";

	char test[255];

	HANDLE vFileHandle(FindFirstFile(vProjectFileName, &sFileData));
	if(vFileHandle != INVALID_HANDLE_VALUE)
	{
		vProjectFileExist = true; //File found
		FindClose(vFileHandle);
	}
	else
	{
		FindClose(vFileHandle);
		while(1)
		{
			//Go back to parent folder if one exist
			if(ParentFolderExist())
			{
				if(!SetCurrentDirectory(".."))
				{
					GetCurrentDirectory(255,test);
					eo << "Could not step back from:" << test << ef;
					break;
				}
			}
			else
				break;

			//Look after file
			vFileHandle = FindFirstFile(vProjectFileName, &sFileData);
			if(vFileHandle != INVALID_HANDLE_VALUE)
			{
				vProjectFileExist = true; //File found
				FindClose(vFileHandle);
				break;
			}
			FindClose(vFileHandle);
		}
	}
#endif
	return vProjectFileExist;
}

void NewProjectDialog::SetParent(EMWindow* p_opWindow)
{
	m_opParentWindow = p_opWindow;
}
