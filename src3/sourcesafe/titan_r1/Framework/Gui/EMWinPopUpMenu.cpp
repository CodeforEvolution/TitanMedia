#include "EMWinPopUpMenu.h"

#include "EMMessages.h"
#include "EMView.h"
#include "EMWinMenu.h"
#include "EMWinMenuItem.h"
#include "EMWinView.h"
#include "EMWinWindow.h"

EMWinPopUpMenu::EMWinPopUpMenu() : 
m_opWindowHandle(NULL)
{
	m_opPopUpMenu = CreatePopupMenu();
}

EMWinPopUpMenu::~EMWinPopUpMenu()
{
eo << "Deleting PopUpMenu: " << ef;
	EMMenu* opMenu;
	while(m_oMenus.size() > 0)
	{
		opMenu = m_oMenus.front();
		m_oMenus.pop_front();
		delete opMenu;
	}
	EMMenuItem* opMenuItem;
	while(m_oMenus.size() > 0)
	{
		opMenuItem = m_oMenuItems.front();
		m_oMenuItems.pop_front();
		delete opMenuItem;
	}

	DestroyMenu(m_opPopUpMenu);
}

bool EMWinPopUpMenu::AddItem(EMMenu* p_opMenu)
{
	m_oMenus.push_back(p_opMenu);
	bool vResult = p_opMenu -> AddToComponent(this);
//	(dynamic_cast<EMWinMenu*>(p_opMenu)) -> SetWindowHandle(m_opWindowHandle);
//	AppendMenu(m_opPopUpMenu, MF_STRING, GetMenuItemCount(m_opPopUpMenu), p_opMenu -> GetName());
	return vResult;
}
	
bool EMWinPopUpMenu::AddItem(EMMenuItem* p_opMenuItem)
{
//	MENUITEMINFO oMenuItemInfo;

//	GetMenuItemInfo(p_opMenuItem ->

	m_oMenuItems.push_back(p_opMenuItem);
	bool vResult = p_opMenuItem -> AddToComponent(this);
//	InsertMenuItem(m_opPopUpMenu, 0, FALSE, &oMenuItemInfo);
	return vResult;
}

bool EMWinPopUpMenu::AddSeparatorItem()
{
	AppendMenu(m_opPopUpMenu, MF_SEPARATOR, 0, NULL);
	return true;
}

void* EMWinPopUpMenu::GetNativeView() const
{
	return m_opPopUpMenu;
}
/*
uint32 EMWinPopUpMenu::GetResult()
{
	return m_vResult;
}

void EMWinPopUpMenu::SetResult(uint32 p_vResult)
{
	m_vResult = p_vResult;
}
*/
void EMWinPopUpMenu::PopUp(int32 p_vX, int32 p_vY, EMView* p_opView)
{
	int vResult = TrackPopupMenuEx(m_opPopUpMenu, TPM_NONOTIFY | TPM_RETURNCMD | TPM_LEFTALIGN | TPM_RIGHTBUTTON, p_vX, p_vY, GetForegroundWindow(), NULL);
	if(vResult != 0)
		Notify(vResult);


	// FIXME: This is a hack to compensate for lost mouse up messages
	if(p_opView != NULL)
		if(p_opView -> LockWindow())
		{
			p_opView -> Notify(EM_MOUSE_UP);
			p_opView -> UnlockWindow();
		}

/*
//	eo << "p_vX = " << p_vX << " p_vY = " << p_vY << ef;
	int vResult;
	if((vResult = TrackPopupMenuEx(m_opPopUpMenu, TPM_NONOTIFY | TPM_RETURNCMD | TPM_LEFTALIGN | TPM_RIGHTBUTTON, p_vX, p_vY, GetForegroundWindow(), NULL)) == 0)
		EMWinWindow::SetPopUpMenu(NULL);
//		eo << "TrackPopupMenuEx, faild with error code:" << GetLastError() << ef;
//eo << "Popup returned " << vResult << ef;
//	eo << "Exit PopUp" << ef;
	if(vResult != 0)
		Notify(vResult);


	// FIXME: This is a hack to compensate for lost mouse up messages
	EMWinView::ResetMouse();
	if(p_opView != NULL)
		if(p_opView -> LockWindow())
		{
			p_opView -> Notify(EM_MOUSE_UP);
			p_opView -> UnlockWindow();
		}
*/
}
