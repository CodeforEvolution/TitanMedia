/*******************************************************
* This code is the copyrighted property of Elegant Media
*-------------------------------------------------------
* Creator: Johan Dunberg
* Component: Application, Dialog
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/
#include "EMGlobals.h"

#ifndef __EXPORT_MEDIA_DIALOG
#define __EXPORT_MEDIA_DIALOG

#include "EMListener.h"
class EMFactory;

class EMWindow;
class EMView;
class EMBrowseWindow;
class EMButton;
class EMLabel;
class EMTextControl;
class EMComboBox;
class EMListBox;
class EMMessageBox;

class EMProgressBar;

class EMListenerRepository;

const uint32 EXPORT_TYPE_COMBO_BOX_CHANGED = 8;

class ExportMediaDialog : public EMListener
{
public:
	ExportMediaDialog();
	~ExportMediaDialog();
	void DisplayDialog();
	EMComboBox* GetExportTypeComboBox();
	void HideDialog();
	void Init();
	bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage);
	void MoveDialog();
	void MoveDialog(float p_vX, float p_vY);
	void SetParent(EMWindow* p_opWindow);
	void StopExporting(bool p_vSendCommand = true);

protected:
	void AddToExportToTextControl();
	bool CheckDiskSpaceOK(const char* p_vpRoot); //Should be in a Folder/Disk management class
	int CreateDir(char* p_vpDirectory, char* p_vFilename);	//Should be in a Folder/Disk management class
	bool CreateFullPath(const char* p_opFullPath);	//Should be in a Folder/Disk management class
	void Export();
	bool FileExist(char* p_vFilename); //Should be in a Folder/Disk management class
	bool InvalidCharExist(const char* p_vpPath, const char* p_vpFileName); //Should be in a Folder/Disk management class
	bool IsDirectoryValid(string* p_opDirectory); //Should be in a Folder/Disk management class
	bool ParentFolderExist(); //Should be in a Folder/Disk management class
	void SetFileExtension(char* p_vpFileExtension);

private:
	EMButton*		m_opBrowseButton;
	EMBrowseWindow* m_opBrowseWindow;
	EMButton*		m_opCancelButton;
//	EMButton*		m_opDeleteTemplateButton;
	EMWindow*		m_opDialogWindow;
	EMButton*		m_opExportButton;
	EMLabel*		m_opExportToLabel;
	EMTextControl*	m_opExportToTextControl;
	EMComboBox*		m_opExportTypeComboBox;
	EMLabel*		m_opExportTypeLabel;
	EMLabel*		m_opFilenameLabel;
	EMTextControl*	m_opFilenameTextControl;
	EMView*			m_opMainView;
	EMMessageBox*	m_opMsgBox;
	EMWindow*		m_opParentWindow;
//	EMButton*		m_opSaveTemplateButton;
	EMButton*		m_opSettingsButton;
//	EMLabel*		m_opTemplateLabel;
//	EMListBox*		m_opTemplateListBox;
	
	string m_oDefaultExportTo;
	bool m_vExportToChanged;
	bool m_vFilenameChanged;
	string m_oDefaultFilename;
	string m_oFileExtension;
	bool m_vShiftPressed;
	bool m_vSelectedFromMenu;
	bool m_vExporting;
	bool m_vRenderAudio;
	bool m_vRenderVideo;

	int m_vExportTypeOld;
	string m_oExportToOld;
	string m_oFilenameOld;

	EMProgressBar* m_opProgressBar;
};

#endif