#include "EMWinBrowseWindow.h"

EMWinBrowseWindow::EMWinBrowseWindow(void* p_vpNativeView, char* p_vName)
{
	m_sBrowseInfo.hwndOwner = static_cast<HWND>(p_vpNativeView);
	m_sBrowseInfo.pidlRoot = NULL;
	m_sBrowseInfo.pszDisplayName = m_vFolderName;
	m_sBrowseInfo.lpszTitle = p_vName;
	m_sBrowseInfo.ulFlags = BIF_RETURNFSANCESTORS | BIF_RETURNONLYFSDIRS;
	m_sBrowseInfo.lpfn = NULL;
	m_sBrowseInfo.lParam = 0;
	//iImage 
}

bool EMWinBrowseWindow::DisplayDialog()
{
	char vpBuffer[2048];
	LPITEMIDLIST spItemIdList;

	spItemIdList = SHBrowseForFolder(&m_sBrowseInfo);
	if(spItemIdList == NULL)
		return false;
	SHGetPathFromIDList(spItemIdList, vpBuffer);
	m_oPath = vpBuffer;
//	m_oPath.append("\\");
	return true;
}

EMRect EMWinBrowseWindow::Frame() const
{
	return EMRect();
}

void* EMWinBrowseWindow::GetNativeView() const
{
	return NULL;
}

string* EMWinBrowseWindow::GetPath()
{
	return &m_oPath;
}

void EMWinBrowseWindow::Hide()
{
}

bool EMWinBrowseWindow::IsHidden()
{
	return m_vHidden;
}

void EMWinBrowseWindow::SetFrame(EMRect p_oFrame)
{
}

void EMWinBrowseWindow::Show()
{
}