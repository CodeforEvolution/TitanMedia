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

#ifndef __NEW_PROJECT_DIALOG
#define __NEW_PROJECT_DIALOG

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
class EMFileWindow;

class EMListenerRepository;

#include <list>

class NewProjectDialog : public EMListener
{
public:
	NewProjectDialog();
	~NewProjectDialog();
	void DisplayDialog();
	void HideDialog();
	void Init();
	bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage);
	void MoveDialog();
	void MoveDialog(float p_vX, float p_vY);
	void SetParent(EMWindow* p_opWindow);

protected:
	void AddToCreateInProjectTextControl();
	bool CheckDiskSpaceOK(const char* p_vpRoot); //Should be in a Folder/Disk management class
	int CreateDir(const char* p_vpDirectory);	//Should be in a Folder/Disk management class
	bool CreateFullPath(const char* p_opFullPath);	//Should be in a Folder/Disk management class
	bool GetTemplates();
	bool InvalidCharExist(); //Should be in a Folder/Disk management class
	bool IsDirectoryValid(string* p_opDirectory); //Should be in a Folder/Disk management class
	void LoadTemplate();
	bool OnlyRoot();
	bool ParentFolderExist(); //Should be in a Folder/Disk management class
	bool SearchProjectFile();

private:
	string m_oValidDirectory;
	string m_oOldCreateInTextControlString;
	EMButton* m_opBrowseButton;
	EMBrowseWindow* m_opBrowseWindow;
	EMButton* m_opCancelButton;
	EMLabel* m_opCreateInLabel;
	EMTextControl* m_opCreateInTextControl;
	EMWindow* m_opDialogWindow;
	EMFileWindow* m_opFileWindow;
	EMButton* m_opLoadProjectButton;
	EMButton* m_opOK_Button;
	EMLabel* m_opProjectNameLabel;
	EMTextControl* m_opProjectNameTextControl;
	EMView* m_opMainView;
	EMMessageBox* m_opMsgBox;
	EMWindow* m_opParentWindow;
	EMLabel* m_opTemplateLabel;
	EMListBox* m_opTemplateListBox;
	list<string> m_oTemplates;
	bool m_vCreateInChanged;
	bool m_vProjectNameChanged;
	bool m_vNoProjectLoaded;

};

#endif