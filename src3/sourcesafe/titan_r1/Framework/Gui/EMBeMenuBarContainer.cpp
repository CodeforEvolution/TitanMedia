#include "EMBeMenuBarContainer.h"

#ifdef PLATFORM_BEOS

#include "EMBeMenu.h"
#include "EMBeMenuBar.h"
#include "EMBeMenuItem.h"
#include "EMCommand.h"
#include "EMCommandRepository.h"
#include "EMListener.h"
#include "EMMenu.h"
#include "EMRect.h"

#include <Menu.h>
#include <MenuBar.h>
#include <Rect.h>

EMBeMenuBarContainer::EMBeMenuBarContainer() :
//m_opApplicationMessageTarget(NULL),
m_opNativeMenuBar(new EMBeMenuBar(this, NULL))
{
}

EMBeMenuBarContainer::~EMBeMenuBarContainer()
{
	list<EMMenu*>::iterator oIterator;
	while(m_oMenus.size() > 0)
	{
		oIterator = m_oMenus.begin();
		m_oMenus.pop_front();
		delete *oIterator;
	}
//		delete *(m_oMenuBars.begin());
/*
	list<EMMenuBar*>::iterator oIterator = m_oMenuBars.begin();
	while(oIterator != m_oMenuBars.end())
	{
		delete *oIterator;
		oIterator = m_oMenuBars.pop_back();
	}
*/
}

bool EMBeMenuBarContainer::AddItem(EMMenu* p_opMenu)
{
	m_oMenus.push_back(p_opMenu);
	return m_opNativeMenuBar -> AddItem((BMenu*) p_opMenu -> GetNativeView());
}

bool EMBeMenuBarContainer::AddItem(EMMenu* p_opMenu, int32 p_vIndex)
{
	m_oMenus.push_back(p_opMenu);
	return m_opNativeMenuBar -> AddItem((BMenuItem*) p_opMenu -> GetNativeView(), p_vIndex);
}

EMRect EMBeMenuBarContainer::Frame() const
{
	BRect oRect = m_opNativeMenuBar -> Frame();
	return EMRect(oRect.left, oRect.top, oRect.right, oRect.bottom);
}

EMMenu* EMBeMenuBarContainer::GetMenu(string* p_opName)
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
/*
EMMenuItem* EMBeMenuBarContainer::GetMenuItem(string* p_opName)
{
	list<EMMenuItem*>::iterator oIterator = m_oMenuItems.begin();
	while(oIterator != m_oMenuItems.end())
	{
		if(*p_opName == (*oIterator) -> GetName())
			return *oIterator;
		oIterator ++;
	}
	return NULL;
*/
/*
	list<EMMenu*>::iterator oIterator = m_oMenus.begin();
	list<EMMenuItem*>::iterator oItemIterator;
	while(oIterator != m_oMenus.end())
	{
		oItemIterator = (*oIterator) -> GetMenuItems() -> begin();
		while(oItemIterator != (*oIterator) -> GetMenuItems() -> end())
		{
			if(*p_opName == (*oItemIterator) -> Name())
				return *oIterator;
			oItemIterator++;
		}
		oIterator ++;
	}
	return NULL;
*/
//}

void* EMBeMenuBarContainer::GetNativeView() const
{
	return m_opNativeMenuBar;
}

void EMBeMenuBarContainer::Hide()
{
	m_opNativeMenuBar -> Hide();
}

void EMBeMenuBarContainer::InitComponent()
{
;//cout_commented_out_4_release << "EMBeMenuBarContainer::InitComponent" << endl;
	list<EMMenu*>::iterator oIterator = m_oMenus.begin();
	while(oIterator != m_oMenus.end())
	{
		(*oIterator) -> SetMessageTarget(m_opNativeMenuBar);//InitComponent();
		oIterator++;
	}
}
/*
bool EMBeMenuBarContainer::MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage)
{
;//cout_commented_out_4_release << "EMBeMenuBarContainer::MessageReceived" << endl;
	if(m_opApplicationMessageTarget != NULL)
		return m_opApplicationMessageTarget -> MessageReceived(p_opSender, p_vMessage);
	return false;
}
*/

// This is a special case where the message is in fact a command id connected
// to a triggered menu item
bool EMBeMenuBarContainer::Notify(uint32 p_vMessage)
{
;//cout_commented_out_4_release << "EMBeMenuBarContainer::Notify" << endl;
	// First, check to see if it really is a command ID
//	if(!(p_vMessage & COMMAND_BASE_OFFSET) && !(p_vMessage & MEDIA_COMMAND_BASE_OFFSET))
//		return EMListenerRepository::Notify(p_vMessage);
		
//;//cout_commented_out_4_release << "EMBeMenuBarContainer::Notify: " << p_vMessage << endl;
	if(p_vMessage != 0)
		EMCommandRepository::Instance() -> ExecuteCommand(p_vMessage, true);
	return EMListenerRepository::Notify(p_vMessage);
}

bool EMBeMenuBarContainer::Notify(list<EMListener*>* p_oListeners, uint32 p_vMessage)
{
	return EMListenerRepository::Notify(p_oListeners, p_vMessage);
}

// Does not delete the menu, that's up to the caller
bool EMBeMenuBarContainer::RemoveItem(EMMenu* p_opMenu)
{
	m_oMenus.remove(p_opMenu);
	return m_opNativeMenuBar -> RemoveItem(static_cast<EMBeMenu*>(p_opMenu -> GetNativeView()));
}
/*
bool EMBeMenuBarContainer::RemoveItem(EMMenuItem* p_opMenuItem)
{
	list<EMMenu*>::iterator oIterator = m_oMenus.begin();
	while(oIterator != m_oMenus.end())
	{
		(*oIterator) -> RemoveItem(p_opMenuItem);
		oIterator++;
	}
}
*/
/*
void EMBeMenuBarContainer::SetApplicationMessageTarget(EMListener* p_opMessageTarget)
{
	m_opApplicationMessageTarget = p_opMessageTarget;
}
*/
void EMBeMenuBarContainer::Show()
{
	m_opNativeMenuBar -> Show();
}

#endif


/*
void EMBeMenuBarContainer::AddMenu(string p_oParent, string p_oLabel)
{
	BMenu* vpTargetedMenu = NULL;
	if(p_oParent == "")
		vpTargetedMenu = m_opNativeMenuBar;
	else
	{
		list<BMenu*>::iterator oIterator = m_opMenuRepository -> begin();
		while(oIterator != m_opMenuRepository -> end() && vpTargetedMenu == NULL)
		{
			if((*oIterator) -> FindView(p_oParent.c_str()))
				vpTargetedMenu = dynamic_cast<BMenu*>((*oIterator) -> FindView(p_oParent.c_str()));
			oIterator++;
		}
		if(vpTargetedMenu == NULL)
		{
			EMDebug("AddMenu: Cannot find parent menu");
			return;
		}
	}
	BMenu* vpNewMenu = new BMenu(p_oLabel.c_str());
	m_opMenuRepository -> push_back(vpNewMenu);
	vpTargetedMenu -> AddItem(vpNewMenu);
//	vpTargetedMenu -> SetTargetForItems(m_opNativeMenuBar);
}

void EMBeMenuBarContainer::AddMenuItem(string p_oParent, string p_oLabel, uint32 p_vCommand)
{
	BMenu* vpTargetedMenu = NULL;
	if(p_oParent == "")
		vpTargetedMenu = m_opNativeMenuBar;
	else
	{
		list<BMenu*>::iterator oIterator = m_opMenuRepository -> begin();
		while(oIterator != m_opMenuRepository -> end() && vpTargetedMenu == NULL)
		{
			if((*oIterator) -> FindView(p_oParent.c_str()))
				vpTargetedMenu = dynamic_cast<BMenu*>((*oIterator) -> FindView(p_oParent.c_str()));
			oIterator++;
		}
		if(vpTargetedMenu == NULL)
		{
			EMDebug("AddMenuItem: Cannot find parent menu");
			return;
		}
	}
	EMBeMenuItem* vpNewMenuItem = new EMBeMenuItem(p_oLabel.c_str(), p_vCommand);
	vpTargetedMenu -> AddItem(vpNewMenuItem);
}

EMRect EMBeMenuBarContainer::Bounds() const
{
	BRect oRect = m_opNativeMenuBar -> Bounds();
	return EMRect(oRect.left, oRect.top, oRect.right, oRect.bottom);
}

void* EMBeMenuBarContainer::GetNativeView() const
{
	return (void*) m_opNativeMenuBar;
}

void EMBeMenuBarContainer::Hide()
{
	m_opNativeMenuBar -> Hide();
}

void EMBeMenuBarContainer::InitComponent()
{
	list<BMenu*>::iterator oIterator = m_opMenuRepository -> begin();
	while(oIterator != m_opMenuRepository -> end())
	{
		(*oIterator) -> SetTargetForItems(m_opNativeMenuBar);
		oIterator++;
	}
}

void EMBeMenuBarContainer::Show()
{
	m_opNativeMenuBar -> Show();
}

#endif

#endif
*/
