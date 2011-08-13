#include "EMWinFileWindow.h"

#ifdef PLATFORM_WINDOWS

#include "EMMessages.h"
#include "EMWinWindow.h"
#include <shlobj.h>

bool EMWinFileWindow::m_vFirstTime = true;

EMWinFileWindow::EMWinFileWindow(EMFilePanelMode p_eFilePanelMode, const char* p_vpWindowTitle, void* p_opParentNativeView, 
				bool p_vMultipleSelection, const char* p_vpSaveDefaultText, vector<string>* p_vpVisibleExtensions, 
				const char* p_vpStartDirectory, uint32 p_vMessage) :
m_eFilePanelMode(p_eFilePanelMode),
m_vMessage(p_vMessage)
{
	m_vFilterBuffer[0] = '\0';
	char* vpFilterBuffer = m_vFilterBuffer;
	m_vCustomFilterBuffer[0] = '\0';
	m_vFileNameBuffer[0] = '\0';

	if(p_vpVisibleExtensions != NULL)
	{
		for(int vCounter = 0; vCounter < p_vpVisibleExtensions -> size(); vCounter++)
		{
			strcpy(vpFilterBuffer, (p_vpVisibleExtensions -> at(vCounter).c_str()));
			vpFilterBuffer += (p_vpVisibleExtensions -> at(vCounter).length());
		//	*vpFilterBuffer = '\0';
			if(*(vpFilterBuffer - 1) != ';')
				vpFilterBuffer++;
		}
		*vpFilterBuffer = '\0';
	}

	if(p_opParentNativeView)
		m_sFileDialogStruct.hwndOwner = static_cast<HWND>(p_opParentNativeView);
	else
		m_sFileDialogStruct.hwndOwner = EMWinWindow::GetMainWindowHandle();
	m_sFileDialogStruct.hInstance = NULL;
	m_sFileDialogStruct.lpstrFilter = m_vFilterBuffer;//"All Files\0*.*\0Media Files\0*.wav;*.avi;*.mpg;*.mov\0Audio File\0*.wav\0Video Files\0*.avi;*.mpg;*.mov\0";
	m_sFileDialogStruct.lpstrCustomFilter = NULL;
	m_sFileDialogStruct.nMaxCustFilter = 0;
	m_sFileDialogStruct.nFilterIndex = 2;
	m_sFileDialogStruct.lpstrFile = m_vFileNameBuffer;
	m_sFileDialogStruct.nMaxFile = FILE_WINDOW_BUFFER_SIZE;//0;
	m_sFileDialogStruct.lpstrFileTitle = const_cast<char*>(p_vpSaveDefaultText);
	if(p_vpSaveDefaultText != NULL)
		m_sFileDialogStruct.nMaxFileTitle = strlen(p_vpSaveDefaultText) + 1;
	else
		m_sFileDialogStruct.nMaxFileTitle = FILE_WINDOW_BUFFER_SIZE;
	m_sFileDialogStruct.lpstrInitialDir = NULL;//p_vpStartDirectory;
	m_sFileDialogStruct.lpstrTitle = p_vpWindowTitle;
	m_sFileDialogStruct.Flags = OFN_OVERWRITEPROMPT | OFN_EXPLORER;
	if(p_vMultipleSelection)
		m_sFileDialogStruct.Flags |= OFN_ALLOWMULTISELECT;
	m_sFileDialogStruct.nFileOffset = 0;
	m_sFileDialogStruct.nFileExtension = 0;
	m_sFileDialogStruct.lpstrDefExt = "txt"; // Set this to filename extension
	m_sFileDialogStruct.lCustData = 0L;
	m_sFileDialogStruct.lpfnHook = NULL;
	m_sFileDialogStruct.lpTemplateName = NULL;
	m_sFileDialogStruct.lStructSize = sizeof(m_sFileDialogStruct);
}

void EMWinFileWindow::ExtractPaths()
{
	m_oFileNames.clear();
	// FIXME: Does not handle multiple files
	m_oFileNames.push_back(string(m_vFileNameBuffer));
}

EMRect EMWinFileWindow::Frame() const
{
	return EMRect();
}

void* EMWinFileWindow::GetNativeView() const
{
	return NULL;
}

vector<string>* EMWinFileWindow::GetPaths()
{
	return &m_oFileNames;
}

bool EMWinFileWindow::IsHidden()
{
	return true;
}

void EMWinFileWindow::Hide()
{
}

void EMWinFileWindow::SetFrame(EMRect p_oFrame)
{
}

void EMWinFileWindow::Show()
{
	char vPath[MAX_PATH];
	if(m_vFirstTime && m_sFileDialogStruct.lpstrInitialDir == NULL)
	{
		m_vFirstTime = false;
		SHGetSpecialFolderPath(NULL, vPath, CSIDL_PERSONAL, false);
		m_sFileDialogStruct.lpstrInitialDir = vPath;
	}

	if(m_eFilePanelMode == FILE_PANEL_LOAD_FILE)
	{
eo << "Will try to open \"Open Project\" dialog" << ef;
		if(!GetOpenFileName(&m_sFileDialogStruct))
		{
eo << "File open failed, or was canceled" << ef;
		}
		else
		{
			ExtractPaths();
			Notify(m_vMessage);
		}
	}
	else if(m_eFilePanelMode == FILE_PANEL_SAVE_FILE)
	{
eo << "Will try to open \"Save Project\" dialog" << ef;
		if(!GetSaveFileName(&m_sFileDialogStruct))
eo << "File save failed" << ef;
		else
		{
			ExtractPaths();
			Notify(m_vMessage);
		}
	}
	// FIXME: FILE_PANEL_LOAD_FOLDER not supported yet
}

#endif