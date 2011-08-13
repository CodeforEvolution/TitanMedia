#include "EMWinMenuItem.h"

#ifdef PLATFORM_WINDOWS

#include "EMCommand.h"
#include "EMCommandRepository.h"
#include "EMGUIComponent.h"
#include "EMKeyboard.h"
#include "EMWinMenu.h"
#include "EMWinPopUpMenu.h"

EMWinMenuItem::EMWinMenuItem(string* p_opName, uint32 p_vCommand) :
m_oName(*p_opName),
//m_upData(NULL),
m_vCommand(p_vCommand),
//m_vID(p_vCommand),
m_vIsEnabled(true),
m_vIsMarked(false)
{
	if(m_vCommand == 0)
		EMDebugger("A menu item cannot have a command = 0");
//eo << "Creating menu item: " << m_oName << ef;
}

EMWinMenuItem::~EMWinMenuItem()
{
//eo << "Deleting menu item: " << m_oName << ef;
}

bool EMWinMenuItem::AddToComponent(EMGUIComponent* p_opComponent)
{
	if(dynamic_cast<EMWinMenu*>(p_opComponent))
		m_opParentMenu = reinterpret_cast<HMENU>((dynamic_cast<EMWinMenu*>(p_opComponent)) -> GetNativeView());
	else if(dynamic_cast<EMWinPopUpMenu*>(p_opComponent))
		m_opParentMenu = reinterpret_cast<HMENU>((dynamic_cast<EMWinPopUpMenu*>(p_opComponent)) -> GetNativeView());
	else
	{
		EMDebugger("The parent must be a menu");
		return false;
	}

//	// 1 is added to the command to make sure it can't be 0 (it is removed in EMWinWindow later on)
	AppendMenu(m_opParentMenu, MF_STRING, m_vCommand/* + 1*/, m_oName.c_str());
	return true;
}

uint32 EMWinMenuItem::GetCommand()
{
	return m_vCommand;
}

/*
void* EMWinMenuItem::GetData()
{
	return m_upData;
}
*/
bool EMWinMenuItem::GetEnabled()
{
	MENUITEMINFO vMenuItemInfo;
	vMenuItemInfo.fMask = MIIM_STATE;
	vMenuItemInfo.cbSize = sizeof(vMenuItemInfo);
	GetMenuItemInfo(m_opParentMenu, m_vCommand, FALSE, &vMenuItemInfo);

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

bool EMWinMenuItem::GetMarked()
{
	return m_vIsMarked;
}

const char* EMWinMenuItem::GetName()
{
//eo << "Menu name: " << m_oName << ef;
	return m_oName.c_str();
}

void* EMWinMenuItem::GetNativeView() const
{
	EMDebugger("A menu item in MS Windows has no native view");
	return NULL;
}
/*
void EMWinMenuItem::SetData(void p_upData)
{
	m_upData = p_upData;
}
*/
void EMWinMenuItem::SetEnabled(bool p_vIsEnabled)
{
	if(p_vIsEnabled)
	{
		if(EnableMenuItem(m_opParentMenu, m_vCommand, MF_ENABLED) == 0xFFFFFFFF)
			EMDebugger("SetEnabled, the menu item does not exist!");
//		eo << "Enable MenuItem: " << m_oName.c_str() << ef;
	}
	else
	{
		if(EnableMenuItem(m_opParentMenu, m_vCommand, MF_GRAYED) == 0xFFFFFFFF)
			EMDebugger("SetEnabled, the menu item does not exist!");
//		eo << "Disable MenuItem: " << m_oName << ef;
	}
//	eo << "ParentMenu: " << (uint32)m_opParentMenu << ef;
	m_vIsEnabled = p_vIsEnabled;
	// the command triggered by the menu should not be triggered by keyboard bindings either
	EMKeyboard::Instance() -> EnableKeyBinding(m_vCommand, p_vIsEnabled);
}

void EMWinMenuItem::SetMarked(bool p_vIsMarked)
{
	if(p_vIsMarked)
	{
		if(CheckMenuItem(m_opParentMenu, m_vCommand, MF_CHECKED) == 0xFFFFFFFF)
			EMDebugger("SetMarked, the menu item does not exist!");
	}
	else
	{
		if(CheckMenuItem(m_opParentMenu, m_vCommand, MF_UNCHECKED) == 0xFFFFFFFF)
			EMDebugger("SetMarked, the menu item does not exist!");
	}
	m_vIsMarked = p_vIsMarked;
}

void EMWinMenuItem::SetName(const char* p_vpName)
{
	m_oName = string(p_vpName);

	MENUITEMINFO sInfo;
	sInfo.cbSize = sizeof(MENUITEMINFO);
	sInfo.fMask = MIIM_TYPE;
	sInfo.fType = MFT_STRING;
	sInfo.dwTypeData = const_cast<char*>(m_oName.c_str());
	sInfo.cch = m_oName.size();

	SetMenuItemInfo(m_opParentMenu, m_vCommand, false, &sInfo);
}

#endif

