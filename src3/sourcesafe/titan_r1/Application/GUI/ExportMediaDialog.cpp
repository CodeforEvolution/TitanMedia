#include "ExportMediaDialog.h"

#include "EMFactory.h"

#include "EMWindow.h"
#include "EMView.h"
#include "EMBrowseWindow.h"
#include "EMButton.h"
#include "EMLabel.h"
#include "EMTextControl.h"
#include "EMListBox.h"
#include "EMComboBox.h"
#include "EMMessageBox.h"
#include "EMKeyboard.h"
#include "EMKeyboardGlobals.h"
#include "EMSettingIDs.h"
#include "EMSettingsRepository.h"
#include "DirectoryIDs.h"
//#include "ProjectManager.h"

#include "EMProgressBar.h"

#include "EMMediaGlobals.h"

#include "EMMessages.h"
#include "EMCommandRepository.h"
#include "CommandIDs.h"

#include "EMExport.h"

#include "ExportMediaSettingsDialog.h"
#ifdef PLATFORM_WINDOWS
#include <windows.h> // To get modal dialog to work.
#include "EMWinWindow.h" // To get modal dialog to work.
#endif

const uint32 EM_BUTTON_SETTINGS = 1;
const uint32 EM_BUTTON_BROWSE = 2;
const uint32 EM_BUTTON_SAVE_TEMPLATE = 3;
const uint32 EM_BUTTON_DELETE_TEMPLATE = 4;
const uint32 EM_BUTTON_EXPORT = 5;
const uint32 EM_BUTTON_CANCEL = 6;
const uint32 EM_TEXT_CONTROL_CHANGE = 7;
//const uint32 EXPORT_TYPE_COMBO_BOX_CHANGED = 8;// is in .h file

ExportMediaDialog::ExportMediaDialog() :
m_opDialogWindow(EMFactory::Instance() -> CreateEMWindow(EMRect(400, 200, 400 + 428, 200 + 141), "Export Media", EM_WINDOW_DIALOG)), 
m_opMainView(EMFactory::Instance() -> CreateView(EMRect(0, 0, 428, 141), false)),
m_opExportTypeLabel(EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
																EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 2,
																EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 60,
																EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 2 + EM_METRICS_NATIVE_LABEL_HEIGHT),
														 "Export Type:", 0)),
m_opExportToLabel(EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
															  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 45,
															  EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 60,
															  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 45 + EM_METRICS_NATIVE_LABEL_HEIGHT),
													   "Export To:", 0)),
m_opFilenameLabel(EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
															  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 70,
															  EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 60,
															  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 70 + EM_METRICS_NATIVE_LABEL_HEIGHT),
													   "Filename:", 0)),

/*m_opTemplateLabel(EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
															  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 90,
															  EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 60,
															  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 90 + EM_METRICS_NATIVE_LABEL_HEIGHT),
													   "Template:", 0)),
*/
m_opExportTypeComboBox(EMFactory::Instance() -> CreateComboBox(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 63,
																	  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP,
																	  428 - (EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + EM_METRICS_NATIVE_BUTTON_WIDTH + EM_METRICS_NATIVE_BUTTON_SPACE),
																	  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 100),
															   EXPORT_TYPE_COMBO_BOX_CHANGED, 0, false)), 
m_opExportToTextControl(EMFactory::Instance() -> CreateTextControl(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 63,
																		  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 43,
																		  428 - (EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + EM_METRICS_NATIVE_BUTTON_WIDTH + EM_METRICS_NATIVE_BUTTON_SPACE),
																		  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 43 + EM_METRICS_NATIVE_TEXT_CONTROL_HEIGHT),
																   "", EM_TEXT_CONTROL_CHANGE, 0, false)),
m_opFilenameTextControl(EMFactory::Instance() -> CreateTextControl(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 63,
																		  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 68,
																		  428 - (EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + EM_METRICS_NATIVE_BUTTON_WIDTH + EM_METRICS_NATIVE_BUTTON_SPACE),
																		  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 68 + EM_METRICS_NATIVE_TEXT_CONTROL_HEIGHT),
																   "", EM_TEXT_CONTROL_CHANGE, 0, false)),
/*
m_opTemplateListBox(EMFactory::Instance() -> CreateListBox(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 63,
																  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 87,
																  EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 63 + 259,
																  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 87 + 122),
														   0, 0, false, false)),
*/
m_opSettingsButton(EMFactory::Instance() -> CreateButton(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 333,
																EM_METRICS_NATIVE_DIALOG_MARGIN_TOP,
																EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 333 + EM_METRICS_NATIVE_BUTTON_WIDTH,
																EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + EM_METRICS_NATIVE_BUTTON_HEIGHT),
														 "Settings...", EM_BUTTON_SETTINGS, 0, 0)),
m_opBrowseButton(EMFactory::Instance() -> CreateButton(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 333,
															  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 43,
															  EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 333 + EM_METRICS_NATIVE_BUTTON_WIDTH,	
															  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 43 + EM_METRICS_NATIVE_BUTTON_HEIGHT),
													   "Browse...", EM_BUTTON_BROWSE, 0, 0)),
/*
m_opSaveTemplateButton(EMFactory::Instance() -> CreateButton(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 333,
																	EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 88,
																	EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 333 + 88,
																	EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 88 + EM_METRICS_NATIVE_BUTTON_HEIGHT),
															 "Save Template", EM_BUTTON_SAVE_TEMPLATE, 0, 0)),
m_opDeleteTemplateButton(EMFactory::Instance() -> CreateButton(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 333,
																	  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 117,
																	  EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 333 + 88,
																	  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 117 + EM_METRICS_NATIVE_BUTTON_HEIGHT),
															   "Delete Template", EM_BUTTON_DELETE_TEMPLATE, 0, 0)),
*/
m_opExportButton(EMFactory::Instance() -> CreateButton(EMRect(428 - (EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + EM_METRICS_NATIVE_BUTTON_WIDTH * 2 + EM_METRICS_NATIVE_BUTTON_SPACE),
														   141 - (EM_METRICS_NATIVE_DIALOG_MARGIN_BOTTOM + EM_METRICS_NATIVE_BUTTON_HEIGHT),
														   428 - (EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + EM_METRICS_NATIVE_BUTTON_WIDTH + EM_METRICS_NATIVE_BUTTON_SPACE),
														   141 - (EM_METRICS_NATIVE_DIALOG_MARGIN_BOTTOM)),
													"Export", EM_BUTTON_EXPORT, 0, 0, NULL, true)),
m_opCancelButton(EMFactory::Instance() -> CreateButton(EMRect(428 - (EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + EM_METRICS_NATIVE_BUTTON_WIDTH),
															  141 - (EM_METRICS_NATIVE_DIALOG_MARGIN_BOTTOM + EM_METRICS_NATIVE_BUTTON_HEIGHT),
															  428 - (EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE),
															  141 - (EM_METRICS_NATIVE_DIALOG_MARGIN_BOTTOM)),
													   "Cancel", EM_BUTTON_CANCEL, 0, 0)),

m_opMsgBox(EMFactory::Instance() -> CreateMessageBox()),
m_opBrowseWindow(NULL),
m_vShiftPressed(false),
m_vSelectedFromMenu(false)
{
	m_opDialogWindow -> AddChild(m_opMainView);
	m_opMainView -> AddChild(m_opExportTypeLabel);
	m_opMainView -> AddChild(m_opExportToLabel);
	m_opMainView -> AddChild(m_opFilenameLabel);
//	m_opMainView -> AddChild(m_opTemplateLabel);
	m_opMainView -> AddChild(m_opExportTypeComboBox);
	m_opMainView -> AddChild(m_opExportToTextControl);
	m_opMainView -> AddChild(m_opFilenameTextControl);
//	m_opMainView -> AddChild(m_opTemplateListBox);
	m_opMainView -> AddChild(m_opSettingsButton);
	m_opMainView -> AddChild(m_opBrowseButton);
//	m_opMainView -> AddChild(m_opSaveTemplateButton);
//	m_opMainView -> AddChild(m_opDeleteTemplateButton);
	m_opMainView -> AddChild(m_opExportButton);
	m_opMainView -> AddChild(m_opCancelButton);

	m_oDefaultFilename = "NotNamed";
	m_oFilenameOld = m_oDefaultFilename.c_str();
//	m_oFileExtension = ".avi";
}

ExportMediaDialog::~ExportMediaDialog()
{
	delete m_opDialogWindow;
	delete m_opMainView;
	delete m_opExportTypeLabel;
	delete m_opExportToLabel;
	delete m_opFilenameLabel;
//	delete m_opTemplateLabel;
	delete m_opExportTypeComboBox;
	delete m_opExportToTextControl;
	delete m_opFilenameTextControl;
//	delete m_opTemplateListBox;
	delete m_opSettingsButton;
	delete m_opBrowseButton;
//	delete m_opSaveTemplateButton;
//	delete m_opDeleteTemplateButton;
	delete m_opExportButton;
	delete m_opCancelButton;
}
//Compare ProjectName with CreateIn and add change in ProjectName to CreateIn

void ExportMediaDialog::AddToExportToTextControl()
{
	string oFilename(m_opFilenameTextControl -> Text());
	string oExportTo(m_opExportToTextControl -> Text());
	string oFileExtensionCheck;
	bool vSame = true;

	oFilename.erase(0, oFilename.find_last_of('.'));
	oFileExtensionCheck.append(oFilename);
	oFilename.erase();
	oFilename.append(m_opFilenameTextControl -> Text());

	//if(strcmp(oFileExtensionCheck.

	if(oFilename.find_first_of('\\') != -1)
	{
		oExportTo.append("\\");
		oFilename.erase(oFilename.find_last_of('\\'));
		oExportTo.append(oFilename);
		oFilename.erase();
		m_opExportToTextControl -> SetText(oExportTo.c_str());
		m_opFilenameTextControl -> SetText(m_oFileExtension.c_str());
	}
/*
	int vPos(oExportTo.find_last_of('\\'));

	for(int vCounter = 0; vPos + 1 < oExportTo.length() && vCounter < oFilename.length(); vCounter++)
	{
		if(oExportTo.at(++vPos) != oFilename.at(vCounter))
		{
			if(m_vExportToChanged)
			{
				vSame = false;
				break;
			}
			if(oExportTo.at(vPos) != oFilename.at((oFilename.length() == vCounter + 1) ? vCounter : vCounter + 1))
			{
				if(oExportTo.at(++vPos) != oFilename.at(vCounter))
					vSame = false;
			}
			else
				eo << "Extra test Same" << ef;
		}
		else
			eo << "Same" << ef;
	}

	int test = ((oExportTo.length() - 1) - vPos);

	if(oFilename.length() == 1)
		if(vPos != oExportTo.length() - 1)
			if(oExportTo[vPos] == oFilename[0] && (((oExportTo.length() - 1) - vPos) == 1))
				vSame = true;
			else
				vSame = false;
	
	if(vSame)
	{
		vPos = oExportTo.find_last_of('\\');
		oExportTo.erase(vPos + 1, oExportTo.length());
		oExportTo.append(oFilename);
		if(oFilename.find_first_of('\\') != -1)
		{
			oFilename.erase();
			m_opFilenameTextControl -> SetText(oFilename.c_str());
		}
	}
	else
	{
		oExportTo.append("\\");
		oExportTo.append(oFilename);
	}

	m_opExportToTextControl -> SetText(oExportTo.c_str());
*/
}

//Checking if 2MB is available on disk
bool ExportMediaDialog::CheckDiskSpaceOK(const char* p_vpDirectory)
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
int ExportMediaDialog::CreateDir(char* p_vpDirectory, char* p_vFilename)
{
#ifdef PLATFORM_WINDOWS
	string oDirectoryTemp = p_vpDirectory;//m_opCreateInTextControl -> Text();
	int vPos;

//	if(oDirectoryTemp.length() != oDirectoryTemp.find_last_of("\\") + 1)
//		oDirectoryTemp.append("\\");

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


	vPos = oDirectoryTemp.find_first_of("\\");
	if(oDirectoryTemp[vPos + 1] == '\\' && oDirectoryTemp[oDirectoryTemp.length() - 1] != '\\')
		oDirectoryTemp.append("\\");
	
	if(GetDriveType(p_vpDirectory) == DRIVE_NO_ROOT_DIR)
	{
		WIN32_FIND_DATA sFileData;
		HANDLE vFileHandle = FindFirstFile(p_vpDirectory/*oDirectoryTemp.c_str()*/, &sFileData);
		if(vFileHandle == INVALID_HANDLE_VALUE)
		{
			string oTemp = "Folder:\n";
			oTemp.append(p_vpDirectory);
			oTemp.append("\ndoes not exist!\nCreate folder?");

			int vResult = m_opMsgBox -> DisplayDialog(m_opDialogWindow -> GetNativeView(), oTemp.c_str(), "Folder don´t exist", EM_DIALOG_BUTTON_YES_NO_CANCEL | EM_DIALOG_ICON_ERROR);
			if(vResult == EM_DIALOG_NO_BUTTON)
				return RESULT_NO_BUTTON;
			if(vResult == EM_DIALOG_CANCEL_BUTTON)
				return RESULT_CANCEL_BUTTON;

			if(!CreateFullPath(p_vpDirectory))
				EMDebugger("Could Not Create Directory\nPossible fault: Parent Directory does not exist");
//			return RESULT_OK;
		}
//		return RESULT_OK;	
	}

	if(SetCurrentDirectory(p_vpDirectory))
	{
		//Check if file exist
		if(FileExist(p_vFilename))
		{
			int vFileExistResult = m_opMsgBox -> DisplayDialog(m_opDialogWindow -> GetNativeView(), "A file with that name already exist in selected folder.\nDo you want to overwrite existing file?", "File exist", EM_DIALOG_ICON_QUESTION | EM_DIALOG_BUTTON_YES_NO);
			if(vFileExistResult == EM_DIALOG_NO_BUTTON)
				return RESULT_NO_BUTTON;
		}
	}
	else
	{
		//If we get here something is wrong! Check if "m_oValidDirectory" is valid!
		EMDebugger("Could not set current directory!");
		return RESULT_NOT_OK;
	}
#endif
	return RESULT_OK;
}

//Createing the full path of the directory
bool ExportMediaDialog::CreateFullPath(const char* p_opFullPath)
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

void ExportMediaDialog::DisplayDialog()
{
	m_opExportTypeComboBox -> Notify(EXPORT_TYPE_COMBO_BOX_CHANGED);
	string oFilename;
	if(m_oFilenameOld.size() == 0)
		oFilename.append(m_oDefaultFilename);
	else
		oFilename.append(m_oFilenameOld);
	oFilename.append(m_oFileExtension);
	m_vExporting = false;
	m_opCancelButton -> SetLabel("Cancel");
	m_opFilenameTextControl -> SetText(oFilename.c_str());
	if(m_oExportToOld.size() != 0)
		m_opExportToTextControl -> SetText(m_oExportToOld.c_str());
//	m_opFilenameTextControl -> SetSelection(0, oFilename.find_last_of('.'));
	EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_STOP_PLAYBACK);
	EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_STOP);
	if(m_opDialogWindow -> Lock())
	{
		m_opDialogWindow -> Show();
#ifdef PLATFORM_WINDOWS
		EnableWindow(dynamic_cast<EMWinWindow*>(m_opDialogWindow) -> GetMainWindowHandle(), FALSE);
		SetFocus(static_cast<HWND>(m_opFilenameTextControl -> GetNativeView()));
	//	m_opFilenameTextControl -> SetSelection(0, oFilename.find_last_of('.'));
#endif
		m_opDialogWindow -> Unlock();
	}
}

void ExportMediaDialog::Export()
{
	string oFullPath(m_opExportToTextControl -> Text());
	oFullPath.append(m_opFilenameTextControl -> Text());

//	EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_SET_RENDER_TO_DISK_OUTPUT_FILENAME, const_cast<char*>(oFullPath.c_str()), NULL, NULL);
//	EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_SET_RENDERING_FLAG, &m_vRenderVideo, &m_vRenderAudio, NULL);
//	EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_START_PLAYBACK);

	EMExport oExportFunctions;
	if(oExportFunctions.Export(m_vRenderVideo, m_vRenderAudio, oFullPath.c_str()))
	{
		m_opExportButton -> SetLabel("Stop");
		m_opCancelButton -> Enable(false);
	}
	else
	{
		HideDialog();
		EMMessageBox* opMessageBox = EMFactory::Instance() -> CreateMessageBox();
		opMessageBox -> DisplayDialog(m_opParentWindow -> GetNativeView(), "You are running a shareware version - export is disabled!", "Shareware version limitation!", EM_DIALOG_BUTTON_OK | EM_DIALOG_ICON_INFORMATION);
	}
}

bool ExportMediaDialog::FileExist(char* p_vFilename)
{
#ifdef PLATFORM_WINDOWS
	WIN32_FIND_DATA sFileData;
	bool vFileExist(false);

//	char test[255];

	HANDLE vFileHandle(FindFirstFile(p_vFilename, &sFileData));
	if(vFileHandle != INVALID_HANDLE_VALUE)
	{
		vFileExist = true; //File found
		FindClose(vFileHandle);
	}
/*	else
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
			vFileHandle = FindFirstFile(p_vFilename, &sFileData);
			if(vFileHandle != INVALID_HANDLE_VALUE)
			{
				vProjectFileExist = true; //File found
				FindClose(vFileHandle);
				break;
			}
			FindClose(vFileHandle);
		}
	}
	*/
#endif
	return vFileExist;
}

EMComboBox* ExportMediaDialog::GetExportTypeComboBox()
{
	return m_opExportTypeComboBox;
}

void ExportMediaDialog::HideDialog()
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

void ExportMediaDialog::Init()
{
	m_opDialogWindow -> AddListener(this);
//	m_opMainView -> AddListener(this);
	if(m_opDialogWindow -> Lock())
	{
		//m_opMainView -> SetViewColor(EMColor(192, 192, 192, 255));
		m_opMainView -> SetDefaultColor();
		m_opDialogWindow -> Unlock();
	}
	m_opExportTypeComboBox -> AddListener(this);
	m_opSettingsButton -> AddListener(this);
	m_opBrowseButton -> AddListener(this);
//	m_opSaveTemplateButton -> AddListener(this);
//	m_opDeleteTemplateButton -> AddListener(this);
	m_opExportButton -> AddListener(this);
	m_opCancelButton -> AddListener(this);
	m_opExportToTextControl -> AddListener(this);
	m_opFilenameTextControl -> AddListener(this);
	m_opExportButton -> Enable(false);

	m_opExportTypeComboBox -> AddString("Audio");
	m_opExportTypeComboBox -> AddString("Audio & Video");
	m_opExportTypeComboBox -> AddString("Video");
	m_opExportTypeComboBox -> SelectItem (0);
	m_vExportTypeOld = 0;

	int vProjectID = *(static_cast<int*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_PROJECT_ID)));
	uint32 vDirectoryID = DIRECTORY_ID_RENDER_TARGET;
	m_oDefaultExportTo = static_cast<string*>(EMCommandRepository::Instance() -> ExecuteCommand(PROJECT_COMMAND_GET_DIRECTORY, &vProjectID, &vDirectoryID)) -> c_str();
	m_oDefaultExportTo.append(EM_FILE_SEPARATOR);
	m_opExportToTextControl -> SetText(m_oDefaultExportTo.c_str());

	//Setting TabOrder
	m_opDialogWindow -> AddTabComponent(m_opFilenameTextControl);
	m_opDialogWindow -> AddTabComponent(m_opSettingsButton);
	m_opDialogWindow -> AddTabComponent(m_opBrowseButton);
	m_opDialogWindow -> AddTabComponent(m_opExportButton);
	m_opDialogWindow -> AddTabComponent(m_opCancelButton);
	m_opDialogWindow -> AddTabComponent(m_opExportTypeComboBox);
	m_opDialogWindow -> AddTabComponent(m_opExportToTextControl);

	EMSettingsRepository::Instance() -> AddListener(this);
}

//Checking if an invalid character is use in ProjectName and CreateIn
bool ExportMediaDialog::InvalidCharExist(const char* p_vpPath, const char* p_vpFileName)
{
	string oPath(p_vpPath);
	string oFileName(p_vpFileName);
	bool vPathInvalidCharExist(false);
	bool vFileNameInvalidCharExist(false);
	int vPos(0);

	while(vPos < oPath.length() && vPos != -1)
	{
		vPos = oPath.find_first_of('\\', vPos);
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
		if(oPath[vPos + 1] == '\\' && vPos != -1)
		{
			vPathInvalidCharExist = true;
			break;
		}
		if(vPos != -1)
			vPos++;
	}
	if(oPath.find_first_of('.') == 0)
		vPathInvalidCharExist = true;
	if(oPath.find_first_of('/') != -1)
		vPathInvalidCharExist = true;
	if(oPath.find_first_of(':') != -1 && oPath.find_first_of(':') != 1)
		vPathInvalidCharExist = true;
	if(oPath.find_first_of('*') != -1)
		vPathInvalidCharExist = true;
	if(oPath.find_first_of('?') != -1)
		vPathInvalidCharExist = true;
	if(oPath.find_first_of('"') != -1)
		vPathInvalidCharExist = true;
	if(oPath.find_first_of('<') != -1)
		vPathInvalidCharExist = true;
	if(oPath.find_first_of('>') != -1)
		vPathInvalidCharExist = true;
	if(oPath.find_first_of('|') != -1)
		vPathInvalidCharExist = true;

	if(vPathInvalidCharExist)
		m_opMsgBox -> DisplayDialog(m_opDialogWindow -> GetNativeView(), "The specified path has invalid character(s)", "Invalid character(s)", EM_DIALOG_ICON_ERROR);

	if(oFileName.find_first_of('.') == 0)
		vFileNameInvalidCharExist = true;
	if(oFileName.find_first_of('\\') != -1)
		vFileNameInvalidCharExist = true;
	if(oFileName.find_first_of('/') != -1)
		vFileNameInvalidCharExist = true;
	if(oFileName.find_first_of(':') != -1)
		vFileNameInvalidCharExist = true;
	if(oFileName.find_first_of('*') != -1)
		vFileNameInvalidCharExist = true;
	if(oFileName.find_first_of('?') != -1)
		vFileNameInvalidCharExist = true;
	if(oFileName.find_first_of('"') != -1)
		vFileNameInvalidCharExist = true;
	if(oFileName.find_first_of('<') != -1)
		vFileNameInvalidCharExist = true;
	if(oFileName.find_first_of('>') != -1)
		vFileNameInvalidCharExist = true;
	if(oFileName.find_first_of('|') != -1)
		vFileNameInvalidCharExist = true;

	if(vFileNameInvalidCharExist)
		m_opMsgBox -> DisplayDialog(m_opDialogWindow -> GetNativeView(), "The File Name has invalid character(s)", "Invalid character(s)", EM_DIALOG_ICON_ERROR);

	if(vPathInvalidCharExist || vFileNameInvalidCharExist)
		return true;
	return false;
}

//Checking if directory is valid. If not, string gets the closest valid directory
bool ExportMediaDialog::IsDirectoryValid(string* p_opDirectory)
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

bool ExportMediaDialog::MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage)
{
	switch(p_vMessage)
	{
/*	case EM_DRAW:
		if(p_opSender == m_opMainView)
		{
			eo << "EM_DRAW: m_oBackgroundView" << ef;
			m_opMainView -> SetHighColor(EMColor(0, 255, 0, 255));
			m_opMainView -> FillRect(m_opMainView -> Bounds());
		}
		else
			eo << "EM_DRAW (unknown)" << ef;
		return true;
*/	case EM_BUTTON_SETTINGS:
		EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_DISPLAY_EXPORT_MEDIA_SETTINGS_DIALOG, m_opDialogWindow, reinterpret_cast<void*>(m_opExportTypeComboBox -> GetSelectionIndex()), this);
		m_opExportTypeComboBox -> Notify(EXPORT_TYPE_COMBO_BOX_CHANGED);
		return true;
	case EM_BUTTON_BROWSE:
		eo << "Browse button pressed" << ef;
		if(m_opBrowseWindow == NULL)
			m_opBrowseWindow = EMFactory::Instance() -> CreateBrowseWindow(m_opMainView -> GetNativeView(), "Browse");
		if(m_opBrowseWindow -> DisplayDialog())
			m_opExportToTextControl -> SetText(m_opBrowseWindow -> GetPath() -> c_str());

		return true;
/*	case EM_BUTTON_SAVE_TEMPLATE:
		eo << "Save Template button pressed" << ef;
		return true;
	case EM_BUTTON_DELETE_TEMPLATE:
		eo << "Delete Template button pressed" << ef;
		return true;
*/	case EM_RENDERING_COULD_NOT_START:
		if(m_vExporting)
			StopExporting(false);
		return true;
	case EM_RENDERING_HAS_FINNISHED:
		if(m_vExporting == false)
			return true;
	case EM_GUI_COMPONENT_ENTER_PRESSED:
	case EM_BUTTON_EXPORT:
		{
		eo << "Export button pressed" << ef;
			if(m_vExporting == false)
			{
				if(InvalidCharExist(m_opExportToTextControl -> Text(), m_opFilenameTextControl -> Text()))
				{
					eo << "Invalid Character found!!" << ef;
					return true;
				}

				string oDirectoryTemp = m_opExportToTextControl -> Text();
				if(oDirectoryTemp.length() != oDirectoryTemp.find_last_of("\\") + 1)
				{
					oDirectoryTemp.append("\\");
					m_opExportToTextControl -> SetText(oDirectoryTemp.c_str());
				}


				int vResult = CreateDir(const_cast<char*>(m_opExportToTextControl -> Text()), const_cast<char*>(m_opFilenameTextControl -> Text()));
				switch(vResult)
				{
				case RESULT_OK:
					m_vExporting = true;
					m_vExportTypeOld = m_opExportTypeComboBox -> GetSelectionIndex();
					m_oExportToOld = m_opExportToTextControl -> Text();
				//	m_oFilenameOld = m_
					Export();
					return true;
				case RESULT_NOT_OK:
				case RESULT_NO_BUTTON:
					eo << "Go back to dialog" << ef;
					return true;
				case RESULT_CANCEL_BUTTON:
					eo << "Close dialog and do nothing" << ef;
					break;
				}
			}
			else
			{
				StopExporting();
				return true;
			}
			HideDialog();
			return true;
		}
	case EM_GUI_COMPONENT_ESC_PRESSED:
	case EM_BUTTON_CANCEL:
		eo << "Cancel button pressed" << ef;
		if(m_vExporting)
			StopExporting();
		HideDialog();
		return true;
	case EM_TEXT_CONTROL_CHANGE:
		eo << "EM_TEXT_CONTROL_FILENAME_CHANGE" << ef;
		if(strlen(m_opFilenameTextControl -> Text()) == 0 || strlen(m_opExportToTextControl -> Text()) == 0)
			m_opExportButton -> Enable(false);
		else
			m_opExportButton -> Enable(true);
		if(p_opSender == m_opExportToTextControl)
		{
			if(m_vFilenameChanged)
			{
				m_vExportToChanged = false;
				m_vFilenameChanged = false;
			}
			else
				m_vExportToChanged = true;
		}
		if(p_opSender == m_opFilenameTextControl)
		{
			m_vFilenameChanged = true;
			AddToExportToTextControl();
		}
		return true;
	case EM_GUI_COMPONENT_GOT_FOCUS:
		eo << "EM_TEXT_CONTROL_GOT_FOCUS" << ef;
		if(p_opSender == m_opFilenameTextControl)
		{
			eo << "Filename got Focus" << ef;
			string oFilename(m_opFilenameTextControl -> Text());
			m_opFilenameTextControl -> SetSelection(0, oFilename.find_last_of('.'));
		}
		if(p_opSender == m_opDialogWindow)
			m_opExportButton -> SetFocus();
		m_opDialogWindow -> SetCurrentFocus();
		return true;
	case EM_GUI_COMPONENT_TAB_PRESSED:
		eo << "EM_GUI_COMPONENT_TAB_PRESSED" << ef;
		m_opDialogWindow -> NextTab();
		return true;
	case EM_GUI_COMPONENT_SHIFT_TAB_PRESSED:
		m_opDialogWindow -> PreviousTab();
		return true;
	case EM_TEXT_CONTROL_END_PRESSED:
		if(p_opSender == m_opFilenameTextControl)
		{
			eo << "EM_TEXT_CONTROL_END_ARROW_PRESSED" << ef;
			uint32 vModifiers = EMKeyboard::Instance() -> GetModifiers();
			if((vModifiers & EM_SHIFT_KEY) == EM_SHIFT_KEY)
			{
				int32 vStart(0);
				int32 vEnd(0);
				m_opFilenameTextControl -> GetSelection(vStart, vEnd);
				m_opFilenameTextControl -> SetSelection(vStart, strlen(m_opFilenameTextControl -> Text()) - m_oFileExtension.length());
			}
			else
				m_opFilenameTextControl -> SetSelection(strlen(m_opFilenameTextControl -> Text()) - m_oFileExtension.length(), strlen(m_opFilenameTextControl -> Text()) - m_oFileExtension.length());
			return true;
		}
		return false;
	case EM_TEXT_CONTROL_DELETE_PRESSED:
		if(p_opSender == m_opFilenameTextControl)
		{
			eo << "EM_TEXT_CONTROL_DELETE_PRESSED" << ef;
			int32 vStart(0);
			int32 vEnd(0);
			m_opFilenameTextControl -> GetSelection(vStart, vEnd);
			if(vStart == vEnd && vEnd >= strlen(m_opFilenameTextControl -> Text()) - m_oFileExtension.length())
				return true;
		}
		return false;
	case EM_TEXT_CONTROL_DOWN_ARROW_PRESSED:
	case EM_TEXT_CONTROL_RIGHT_ARROW_PRESSED:
		if(p_opSender == m_opFilenameTextControl)
		{
			eo << "EM_TEXT_CONTROL_DOWN/RIGHT_ARROW_PRESSED" << ef;
			int32 vStart(0);
			int32 vEnd(0);
			m_opFilenameTextControl -> GetSelection(vStart, vEnd);
			if(vEnd >= strlen(m_opFilenameTextControl -> Text()) - m_oFileExtension.length())
				return true;
		}
		return false;
	case EM_TEXT_CONTROL_LEFT_MOUSE_BUTTON_DOWN:
		if(p_opSender == m_opFilenameTextControl)
		{
			int32 vStart(0);
			int32 vEnd(0);
			m_opFilenameTextControl -> GetSelection(vStart, vEnd);
			if(vEnd >= strlen(m_opFilenameTextControl -> Text()) - m_oFileExtension.length() && vStart >= strlen(m_opFilenameTextControl -> Text()) - m_oFileExtension.length())
			{
				m_opFilenameTextControl -> SetSelection(strlen(m_opFilenameTextControl -> Text()) - m_oFileExtension.length(), strlen(m_opFilenameTextControl -> Text()) - m_oFileExtension.length());
				//return true;
			}
			else if(vEnd >= strlen(m_opFilenameTextControl -> Text()) - m_oFileExtension.length() && vStart < strlen(m_opFilenameTextControl -> Text()) - m_oFileExtension.length())
			{
				m_opFilenameTextControl -> SetSelection(vStart, strlen(m_opFilenameTextControl -> Text()) - m_oFileExtension.length());
				//return true;
			}

		}
		return false;
	case EM_TEXT_CONTROL_MENU_SELECTED:
		eo << "EM_TEXT_CONTROL_MENU_SELECTED" << ef;
		if(p_opSender == m_opFilenameTextControl)
		{
			m_vSelectedFromMenu = true;
			return true;
		}
		return false;
	case EM_TEXT_CONTROL_SET_SELECTION:
		eo << "EM_TEXT_CONTROL_SET_SELECTION" << ef;
		if(p_opSender == m_opFilenameTextControl)
		{
			if(m_vSelectedFromMenu)
			{
				m_vSelectedFromMenu = false;
				m_opFilenameTextControl -> SetSelection(0, strlen(m_opFilenameTextControl -> Text()) - m_oFileExtension.length());
				return true;
			}
		}
		return false;
	case EXPORT_TYPE_COMBO_BOX_CHANGED:
		{
			eo << "EXPORT_TYPE_COMBO_BOX_CHANGED" << ef;
			switch(m_opExportTypeComboBox -> GetSelectionIndex())
			{
			case 0:
				m_vRenderAudio = true;
				m_vRenderVideo = false;
				m_oFileExtension = ".wav";
				break;
			case 1:
				m_vRenderAudio = true;
				m_vRenderVideo = true;
				//m_oFileExtension = ".avi";
				m_oFileExtension = ExportMediaSettingsDialog::m_vFileExtension;
				break;
			case 2:
				m_vRenderAudio = false;
				m_vRenderVideo = true;
				//m_oFileExtension = ".avi";
				m_oFileExtension = ExportMediaSettingsDialog::m_vFileExtension;
				break;
			}
			string oFilename;
			oFilename.append(m_oDefaultFilename);
			oFilename.append(m_oFileExtension);
			m_opFilenameTextControl -> SetText(oFilename.c_str());
			return true;
		}
	case EM_CLOSE_WINDOW:
		eo << "EM_CLOSE_WINDOW" << ef;
		if(m_vExporting)
		{
			StopExporting();
			return true;
		}
		HideDialog();
		return true;
	}
	return false;
}

void ExportMediaDialog::MoveDialog()
{
	EMRect oWindowRect(m_opParentWindow -> Frame());
	EMRect oDialogRect(m_opDialogWindow -> Frame());
	float vX(oWindowRect.m_vRight - oWindowRect.GetWidth() / 2 - oDialogRect.GetWidth() / 2);
	float vY(oWindowRect.m_vBottom - oWindowRect.GetHeight() / 2 - oDialogRect.GetHeight() / 2);

	if(m_opDialogWindow -> Lock())
	{
		m_opDialogWindow -> SetFrame(EMRect(vX, vY, vX + oDialogRect.GetWidth(), vY + oDialogRect.GetHeight()));
		m_opDialogWindow -> Unlock();
	}

	MoveDialog(vX, vY);
}

void ExportMediaDialog::MoveDialog(float p_vX, float p_vY)
{
	m_opDialogWindow -> MoveTo(p_vX, p_vY);
}

//Checking if a parent folder exist
bool ExportMediaDialog::ParentFolderExist()
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

void ExportMediaDialog::SetFileExtension(char* p_vpFileExtension)
{
	m_oFileExtension.erase();
	m_oFileExtension.append(".");
	m_oFileExtension.append(p_vpFileExtension);
}

void ExportMediaDialog::SetParent(EMWindow* p_opWindow)
{
	m_opParentWindow = p_opWindow;
}

void ExportMediaDialog::StopExporting(bool p_vSendCommand)
{
	m_vExporting = false;
	if(p_vSendCommand)
		EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_STOP_PLAYBACK);
	m_opExportButton -> SetLabel("Export");
	m_opCancelButton -> SetLabel("Close");
	m_opCancelButton -> Enable(true);
}