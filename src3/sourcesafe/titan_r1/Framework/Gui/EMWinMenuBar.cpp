#include "EMWinMenuBar.h"

#ifdef PLATFORM_WINDOWS

#include "EMWinMenu.h"
#include "EMWinMenuItem.h"
#include "EMCommand.h"
#include "EMCommandRepository.h"
#include "EMListener.h"
#include "EMMenu.h"
#include "EMMenuItem.h"
#include "EMRect.h"
#include "EMResourceRepository.h"
#include "EMWinWindow.h"
#include "ResourceIDs.h" // FIXME: Application file in the framework = bad

EMWinMenuBar::EMWinMenuBar() :
m_opMenu(NULL),
m_opWindow(NULL)
{
eo << "Creating menu bar" << ef;
}

EMWinMenuBar::~EMWinMenuBar()
{
eo << "Starting to delete menu bar" << ef;
	EMMenu* opMenu;
	while(m_oMenus.size() > 0)
	{
/*
		opMenu = dynamic_cast<EMWinMenu*>(m_oMenus.front());
eo << "1 About to delete menu item: " << opMenu -> GetName() << ef;
		m_oMenus.pop_front();
eo << "2 About to delete menu item: " << opMenu -> GetName() << ef;
*/
		opMenu = m_oMenus.front();
		m_oMenus.pop_front();
		delete opMenu;
	}
eo << "Menu bar deleted" << ef;
}

bool EMWinMenuBar::AddItem(EMMenu* p_opMenu)
{
	if(m_opWindow == NULL)
	{
		EMDebugger("The menu bar must be added to a window first");
		return false;
	}
	m_oMenus.push_back(p_opMenu);
	bool vResult = p_opMenu -> AddToComponent(this);//m_opWindow -> AddItem((BMenu*) p_opMenu -> GetNativeView());
	(dynamic_cast<EMWinMenu*>(p_opMenu)) -> SetWindowHandle(m_opWindow -> GetWindowHandle());
	DrawMenuBar(reinterpret_cast<HWND>(m_opWindow -> GetNativeView()));
	return vResult;
}

bool EMWinMenuBar::AddItem(EMMenu* p_opMenu, int32 p_vIndex)
{
	EMDebugger("Not supported yet");
	return false;
//	m_oMenus.push_back(p_opMenu);
//	return m_opNativeMenuBar -> AddItem((BMenuItem*) p_opMenu -> GetNativeView(), p_vIndex);
}

bool EMWinMenuBar::AddToComponent(EMGUIComponent* p_opComponent)
{
	HWND opWindowHandle;

	if(dynamic_cast<EMWinWindow*>(p_opComponent))
	{
		m_opWindow = dynamic_cast<EMWinWindow*>(p_opComponent);
		opWindowHandle = m_opWindow -> GetWindowHandle();
		m_opMenu = CreateMenu();
		SetMenu(opWindowHandle, m_opMenu);
//		DrawMenuBar(opWindowHandle);
//		UpdateWindow(opWindowHandle);
		return true;
	}

	EMDebugger("The parent for an EMMenuBar must be a window");
	return false;
}

EMRect EMWinMenuBar::Frame() const
{
//	BRect oRect = m_opNativeMenuBar -> Frame();
//	return EMRect(oRect.left, oRect.top, oRect.right, oRect.bottom);
//	EMDebugger("Not supported yet");
	return EMRect();
}

EMMenuItem* EMWinMenuBar::GetLastTriggeredMenuItem()
{
	EMMenuItem* opMenuItem;
	list<EMMenu*>::iterator oMenuIterator = m_oMenus.begin();
	while(oMenuIterator != m_oMenus.end())
	{
		opMenuItem = (*oMenuIterator) -> GetMenuItem(m_vLastTriggeredMenuItemID);
		if(opMenuItem != NULL)
			return opMenuItem;

		oMenuIterator++;
	}
	
	return NULL;
}

EMMenu* EMWinMenuBar::GetMenu(string* p_opName)
{
	list<EMMenu*>::iterator oIterator = m_oMenus.begin();
	while(oIterator != m_oMenus.end())
	{
		if(*p_opName == (*oIterator) -> GetName())
			return *oIterator;
		oIterator ++;
	}
	return NULL;
}

void* EMWinMenuBar::GetNativeView() const
{
	return m_opMenu;
}

void EMWinMenuBar::Hide()
{
	EMDebugger("Not supported yet");
//	m_opNativeMenuBar -> Hide();
}

//void EMWinMenuBar::InitComponent()
//{
//	list<EMMenu*>::iterator oIterator = m_oMenus.begin();
//	while(oIterator != m_oMenus.end())
//	{
//		(*oIterator) -> SetMessageTarget(m_opNativeMenuBar);//InitComponent();
//		oIterator++;
//	}
//}

/*
// This is a special case where the message is in fact a command id connected
// to a triggered menu item
bool EMWinMenuBar::Notify(uint32 p_vMessage)
{
	if(p_vMessage != 0)
		EMCommandRepository::Instance() -> ExecuteCommand(p_vMessage, true);
	return EMListenerRepository::Notify(p_vMessage);
}

bool EMWinMenuBar::Notify(list<EMListener*>* p_oListeners, uint32 p_vMessage)
{
	return EMListenerRepository::Notify(p_oListeners, p_vMessage);
}
*/

// Does not delete the menu, that's up to the caller
bool EMWinMenuBar::RemoveItem(EMMenu* p_opMenu)
{
//	m_oMenus.remove(p_opMenu);
//	return m_opNativeMenuBar -> RemoveItem(static_cast<EMBeMenu*>(p_opMenu -> GetNativeView()));
	EMDebugger("Not supported yet");
	return false;
}

void EMWinMenuBar::SetLastTriggeredMenuItemID(uint32 p_vLastTriggeredMenuItemID)
{
	m_vLastTriggeredMenuItemID = p_vLastTriggeredMenuItemID;
}

bool EMWinMenuBar::SetMarked(uint32 p_vMenuNameID, uint32 p_vMenuItemNameID, bool p_vMarked)
{
	EMMenu* opMenu = GetMenu(EMResourceRepository::GetString(p_vMenuNameID));
	if(opMenu == NULL)
		return false;
	EMMenuItem* opMenuItem = opMenu -> GetMenuItem(EMResourceRepository::GetString(p_vMenuItemNameID));
	if(opMenuItem == NULL)
		return false;
	opMenuItem -> SetMarked(p_vMarked);
	return true;
}

bool EMWinMenuBar::SetEnabled(uint32 p_vMenuNameID, bool p_vEnabled)
{
	EMMenu* opMenu = GetMenu(EMResourceRepository::GetString(p_vMenuNameID));
	if(opMenu == NULL)
		return false;
	opMenu -> SetEnabled(p_vEnabled);
	return true;
}

bool EMWinMenuBar::SetEnabled(uint32 p_vMenuNameID, uint32 p_vMenuItemNameID, bool p_vEnabled)
{
	EMMenu* opMenu = GetMenu(EMResourceRepository::GetString(p_vMenuNameID));
	if(opMenu == NULL)
		return false;
	EMMenuItem* opMenuItem = opMenu -> GetMenuItem(EMResourceRepository::GetString(p_vMenuItemNameID));
	if(opMenuItem == NULL)
	{
		EMMenu* opSubMenu = opMenu -> GetMenu(EMResourceRepository::GetString(p_vMenuItemNameID));
		if(opSubMenu == NULL)
			return false;
		opSubMenu -> SetEnabled(p_vEnabled);
		return true;
	}
	opMenuItem -> SetEnabled(p_vEnabled);
	return true;
}

void EMWinMenuBar::SetFrame(EMRect p_oRect)
{
}

void EMWinMenuBar::Show()
{
//	m_opNativeMenuBar -> Show();
	EMDebugger("Not supported yet");
}

#endif

