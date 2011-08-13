/*******************************************************
* This code is the copyrighted property of Elegant Media
*-------------------------------------------------------
* Creator: Johan Dunberg
* Component: Framework
* Portability: native
*-------------------------------------------------------
* 
*******************************************************/
#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#ifndef __EM_WIN_BROWSE_WINDOW
#define __EM_WIN_BROWSE_WINDOW

#include "EMBrowseWindow.h"
#include <shlobj.h>

class EMWinBrowseWindow : public EMBrowseWindow
{
public:
	EMWinBrowseWindow(void* p_vpNativeView, char* p_vName);	
	bool DisplayDialog();
	EMRect Frame() const;
	void* GetNativeView() const;
	string* GetPath();
	bool IsHidden();
	void Hide();
	void SetFrame(EMRect p_oFrame);
	void Show();

private:
	string m_oPath;
	BROWSEINFO m_sBrowseInfo;
	bool m_vHidden;
	char m_vFolderName[MAX_PATH];
};

#endif

#endif