#include "EMWinMenu.h"

#ifdef PLATFORM_WINDOWS

#include "EMWinMenuBar.h"
#include "EMMenuItem.h"
#include "EMWinMenuItem.h"
#include "EMWinPopUpMenu.h"

EMWinMenu::EMWinMenu(string* p_opName) :
m_oName(*p_opName),
m_opMenu(NULL),
m_opParentMenu(NULL),
m_opWindowHandle(NULL)
{
//eo << "Creating menu: " << m_oName << ef;
}
	
EMWinMenu::~EMWinMenu()
{
eo << "Deleting menu: " << m_oName.c_str() << ef;
	EMMenu* opMenu;
	while(m_oMenus.size() > 0)
	{
		opMenu = m_oMenus.front();
DestroyMenu(dynamic_cast<EMWinMenu*>(opMenu) -> m_opMenu); // I should not have to do this
		m_oMenus.pop_front();
		delete opMenu;
	}
	EMMenuItem* opMenuItem;
	while(m_oMenuItems.size() > 0)
	{
		opMenuItem = m_oMenuItems.front();
		m_oMenuItems.pop_front();
		delete opMenuItem;
	}
}

bool EMWinMenu::AddItem(EMMenu* p_opMenu)
{
	if(m_opParentMenu == NULL)
	{
		EMDebugger("The menu must be added to a menu bar or a menu first");
		return false;
	}
	m_oMenus.push_back(p_opMenu);
	bool vResult = p_opMenu -> AddToComponent(this);//m_opWindow -> AddItem((BMenu*) p_opMenu -> GetNativeView());
	(dynamic_cast<EMWinMenu*>(p_opMenu)) -> SetWindowHandle(m_opWindowHandle);
	DrawMenuBar(m_opWindowHandle);
	return vResult;
}
	
bool EMWinMenu::AddItem(EMMenuItem* p_opMenuItem)
{
	if(m_opParentMenu == NULL)
	{
		EMDebugger("The menu must be added to a menu bar or a menu first");
		return false;
	}
	m_oMenuItems.push_back(p_opMenuItem);
//	bool vResult = m_opNativeMenu -> AddItem((BMenuItem*) p_opMenuItem -> GetNativeView());
	bool vResult = p_opMenuItem -> AddToComponent(this);
	DrawMenuBar(m_opWindowHandle);
	return vResult;
}
	
bool EMWinMenu::AddItem(EMMenu* p_opMenu, int32 p_vIndex)
{
//	m_oMenus.push_back(p_opMenu);
//	return m_opNativeMenu -> AddItem((BMenuItem*) p_opMenu -> GetNativeView(), p_vIndex);
	EMDebugger("Not supported yet");
	return false;
}
	
bool EMWinMenu::AddItem(EMMenuItem* p_opMenuItem, int32 p_vIndex)
{
//	m_oMenuItems.push_back(p_opMenuItem);
//	return m_opNativeMenu -> AddItem((BMenuItem*) p_opMenuItem -> GetNativeView(), p_vIndex);
	EMDebugger("Not supported yet");
	return false;
}
	
bool EMWinMenu::AddSeparatorItem()
{
//	return m_opNativeMenu -> AddSeparatorItem();
	if(m_opParentMenu == NULL)
	{
		EMDebugger("The menu must be added to a menu bar or a menu first");
		return false;
	}
	AppendMenu(m_opMenu, MF_SEPARATOR, 0, NULL);
	return true;
}
	
bool EMWinMenu::AddToComponent(EMGUIComponent* p_opComponent)
{
	if(dynamic_cast<EMWinMenuBar*>(p_opComponent))
		m_opParentMenu = reinterpret_cast<HMENU>((dynamic_cast<EMWinMenuBar*>(p_opComponent)) -> GetNativeView());
	else if(dynamic_cast<EMWinMenu*>(p_opComponent))
		m_opParentMenu = reinterpret_cast<HMENU>((dynamic_cast<EMWinMenu*>(p_opComponent)) -> GetNativeView());
	else if(dynamic_cast<EMWinPopUpMenu*>(p_opComponent))
		m_opParentMenu = reinterpret_cast<HMENU>((dynamic_cast<EMWinPopUpMenu*>(p_opComponent)) -> GetNativeView());
	else
	{
		EMDebugger("The parent is neither a menu bar nor a menu");
		return false;
	}

	m_opMenu = CreateMenu();
	AppendMenu(m_opParentMenu, MF_POPUP, reinterpret_cast<uint32>(m_opMenu), m_oName.c_str());
	return true;
}

EMRect EMWinMenu::Frame() const
{
//	BRect oRect = m_opNativeMenu -> Frame();
//	return EMRect(oRect.left, oRect.top, oRect.right, oRect.bottom);
//	EMDebugger("Not supported yet");
	return EMRect();
}

bool EMWinMenu::GetEnabled()
{
	MENUITEMINFO vMenuItemInfo;
	vMenuItemInfo.fMask = MIIM_STATE;
	vMenuItemInfo.cbSize = sizeof(vMenuItemInfo);
	GetMenuItemInfo(m_opParentMenu, reinterpret_cast<uint32>(m_opMenu), FALSE, &vMenuItemInfo);

	if(vMenuItemInfo.fState == MFS_ENABLED || vMenuItemInfo.fState == (MFS_ENABLED | MFS_CHECKED))
		return true;
		//eo << "MenuItem Enabled" << ef;
	if(vMenuItemInfo.fState == MF_GRAYED || vMenuItemInfo.fState == (MF_GRAYED | MFS_CHECKED))
		return false;
		//eo << "MenuItem Disabled" << ef;
	// If reaching this point menu item has unexpected settings
	EMDebugger("Menu item has unexpected settings!");
	return false;
}

EMMenu* EMWinMenu::GetMenu(string* p_opName)
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
	
EMMenuItem* EMWinMenu::GetMenuItem(string* p_opName)
{
	list<EMMenuItem*>::iterator oMenuItemIterator = m_oMenuItems.begin();
	while(oMenuItemIterator != m_oMenuItems.end())
	{
		if(*p_opName == (*oMenuItemIterator) -> GetName())
			return *oMenuItemIterator;
		oMenuItemIterator++;
	}

	EMMenuItem* opMenuItem;
	list<EMMenu*>::iterator oMenuIterator = m_oMenus.begin();
	while(oMenuIterator != m_oMenus.end())
	{
		opMenuItem = (*oMenuIterator) -> GetMenuItem(p_opName);
		if(opMenuItem != NULL)
			return opMenuItem;

		oMenuIterator++;
	}
	
	return NULL;
}
	
EMMenuItem* EMWinMenu::GetMenuItem(uint32 p_vCommand)
{
	list<EMMenuItem*>::iterator oIterator = m_oMenuItems.begin();
	while(oIterator != m_oMenuItems.end())
	{
		if(p_vCommand == (*oIterator) -> GetCommand())
			return *oIterator;
		oIterator++;
	}

	EMMenuItem* opMenuItem;
	list<EMMenu*>::iterator oMenuIterator = m_oMenus.begin();
	while(oMenuIterator != m_oMenus.end())
	{
		opMenuItem = (*oMenuIterator) -> GetMenuItem(p_vCommand);
		if(opMenuItem != NULL)
			return opMenuItem;

		oMenuIterator++;
	}
	
	return NULL;
}

const char* EMWinMenu::GetName()
{
	return m_oName.c_str();//m_opNativeMenu -> Name();
}

void* EMWinMenu::GetNativeView() const
{
	return m_opMenu;
}

void EMWinMenu::Hide()
{
	EMDebugger("Menus cannot be hidden by using Hide() in this implementation");
//	m_opNativeMenu -> Hide();
}

bool EMWinMenu::RemoveItem(EMMenu* p_opMenu)
{
//	m_oMenus.remove(p_opMenu);
//	return m_opNativeMenu -> RemoveItem(static_cast<EMBeMenu*>(p_opMenu -> GetNativeView()));
	EMDebugger("Not supported yet");
	return false;
}
	
bool EMWinMenu::RemoveItem(EMMenuItem* p_opMenuItem)
{
//	m_oMenuItems.remove(p_opMenuItem);
//	return m_opNativeMenu -> RemoveItem(static_cast<EMBeMenuItem*>(p_opMenuItem -> GetNativeView()));
	EMDebugger("Not supported yet");
	return false;
}

void EMWinMenu::SetEnabled(bool p_vEnabled)
{
	if(p_vEnabled)
	{
		if(EnableMenuItem(m_opParentMenu, reinterpret_cast<uint32>(m_opMenu), MF_ENABLED) == 0xFFFFFFFF)
			EMDebugger("SetEnabled, the menudoes not exist!");
	}
	else
	{
		if(EnableMenuItem(m_opParentMenu, reinterpret_cast<uint32>(m_opMenu), MF_GRAYED) == 0xFFFFFFFF)
			EMDebugger("SetEnabled, the menudoes not exist!");
	}
	DrawMenuBar(m_opWindowHandle);
}

void EMWinMenu::SetFrame(EMRect p_oFrame)
{
}

void EMWinMenu::SetMessageTarget(void* p_upTarget)
{
	// This method does nothing in MS Windows
}

void EMWinMenu::Show()
{
	EMDebugger("Menus cannot be shown by using Show() in this implementation");
//	m_opNativeMenu -> Show();
}

void EMWinMenu::SetWindowHandle(HWND p_opWindow)
{
	m_opWindowHandle = p_opWindow;
}

#endif