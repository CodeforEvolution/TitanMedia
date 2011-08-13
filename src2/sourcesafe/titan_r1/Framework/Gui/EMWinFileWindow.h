/*******************************************************
* Creator: Martin Rudefelt
* Portability: native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#ifndef __EM_WIN_FILE_WINDOW
#define __EM_WIN_FILE_WINDOW

#include "EMFileWindow.h"

#include <string>
#include <vector>
#include <windows.h>

const int16 FILE_WINDOW_BUFFER_SIZE = 2048;

class EMWinFileWindow : public EMFileWindow
{
public:
	EMWinFileWindow(EMFilePanelMode p_eFilePanelMode, const char* p_vpWindowTitle, void* p_opParentNativeView,  
					bool p_vMultipleSelection, const char* p_vpSaveDefaultText, vector<string>* p_vpVisibleExtensions = NULL, 
					const char* p_vpStartDirectory = NULL, uint32 p_vMessage = 0);
	EMRect Frame() const;
	void* GetNativeView() const;
	vector<string>* GetPaths();
	bool IsHidden();
	void Hide();
	void SetFrame(EMRect p_oFrame);
	void Show();

private:
	void ExtractPaths();

private:
	EMFilePanelMode m_eFilePanelMode;
	char m_vCustomFilterBuffer[FILE_WINDOW_BUFFER_SIZE];
	char m_vFileNameBuffer[FILE_WINDOW_BUFFER_SIZE];
	char m_vFilterBuffer[FILE_WINDOW_BUFFER_SIZE];
	static bool m_vFirstTime;
	vector<string> m_oFileNames;
	OPENFILENAME m_sFileDialogStruct;
	uint32 m_vMessage;
};

#endif

#endif

