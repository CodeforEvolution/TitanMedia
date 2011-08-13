#include "EMBeMenuContainer.h"

#ifdef PLATFORM_BEOS

#include "EMBeMenuItem.h"
#include "EMMenuItem.h"

#include <Menu.h>

EMBeMenuContainer::EMBeMenuContainer(string* p_opName) :
//m_opApplicationMessageTarget(NULL),
m_opNativeMenu(new EMBeMenu(this, p_opName -> c_str()))
{
}
	
EMBeMenuContainer::~EMBeMenuContainer()
{
	list<EMMenu*>::iterator oMenuIterator;
	while(m_oMenus.size() > 0)
	{
		oMenuIterator = m_oMenus.begin();
		m_oMenus.pop_front();
		delete *oMenuIterator;
	}
	list<EMMenuItem*>::iterator oMenuItemIterator;
	while(m_oMenus.size() > 0)
	{
		oMenuItemIterator = m_oMenuItems.begin();
		m_oMenuItems.pop_front();
		delete *oMenuItemIterator;
	}
	delete m_opNativeMenu;
}
	
bool EMBeMenuContainer::AddItem(EMMenu* p_opMenu)
{
	m_oMenus.push_back(p_opMenu);
	return m_opNativeMenu -> AddItem((BMenu*) p_opMenu -> GetNativeView());
}
	
bool EMBeMenuContainer::AddItem(EMMenuItem* p_opMenuItem)
{
	m_oMenuItems.push_back(p_opMenuItem);
	bool vResult = m_opNativeMenu -> AddItem((BMenuItem*) p_opMenuItem -> GetNativeView());
//	static_cast<BMenuItem*>(p_opMenuItem -> GetNativeView()) -> SetTarget(m_opNativeMenu);
//	m_opNativeMenu -> SetTargetForItems(m_opNativeMenu);
	return vResult;
}
	
bool EMBeMenuContainer::AddItem(EMMenu* p_opMenu, int32 p_vIndex)
{
	m_oMenus.push_back(p_opMenu);
	return m_opNativeMenu -> AddItem((BMenuItem*) p_opMenu -> GetNativeView(), p_vIndex);
}
	
bool EMBeMenuContainer::AddItem(EMMenuItem* p_opMenuItem, int32 p_vIndex)
{
	m_oMenuItems.push_back(p_opMenuItem);
	return m_opNativeMenu -> AddItem((BMenuItem*) p_opMenuItem -> GetNativeView(), p_vIndex);
}
	
bool EMBeMenuContainer::AddSeparatorItem()
{
	return m_opNativeMenu -> AddSeparatorItem();
}
	
EMRect EMBeMenuContainer::Frame() const
{
	BRect oRect = m_opNativeMenu -> Frame();
	return EMRect(oRect.left, oRect.top, oRect.right, oRect.bottom);
}

EMMenu* EMBeMenuContainer::GetMenu(string* p_opName)
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
	
EMMenuItem* EMBeMenuContainer::GetMenuItem(string* p_opName)
{
	list<EMMenuItem*>::iterator oIterator = m_oMenuItems.begin();
	while(oIterator != m_oMenuItems.end())
	{
		if(*p_opName == (*oIterator) -> GetName())
			return *oIterator;
		oIterator ++;
	}
	return NULL;
}
	
const char* EMBeMenuContainer::GetName()
{
	return m_opNativeMenu -> Name();
}

void* EMBeMenuContainer::GetNativeView() const
{
	return m_opNativeMenu;
}

void EMBeMenuContainer::Hide()
{
	EMDebugger("Menus cannot be hidden by using Hide() in this implementation");
//	m_opNativeMenu -> Hide();
}
/*
void EMBeMenuContainer::InitComponent()
{
//;//cout_commented_out_4_release << "EMBeMenuContainer::InitComponent()" << endl;
//;//cout_commented_out_4_release << static_cast<long unsigned int>(m_opNativeMenu -> SetTargetForItems(m_opNativeMenu)) << ", " << B_OK << endl;
//if(m_opNativeMenu -> IsAttached())
//	;//cout_commented_out_4_release << "Is attached" << endl;

	list<EMMenuItem*>::iterator oIterator = m_oMenuItems.begin();
	while(oIterator != m_oMenuItems.end())
	{
		(*oIterator) -> Init();
		oIterator++;
	}

}
*/
/*
bool EMBeMenuContainer::MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage)
{
;//cout_commented_out_4_release << "EMBeMenuContainer::MessageReceived" << endl;
	if(m_opApplicationMessageTarget != NULL)
		return m_opApplicationMessageTarget -> MessageReceived(p_opSender, p_vMessage);
	return false;
}
*/
bool EMBeMenuContainer::RemoveItem(EMMenu* p_opMenu)
{
	m_oMenus.remove(p_opMenu);
	return m_opNativeMenu -> RemoveItem(static_cast<EMBeMenu*>(p_opMenu -> GetNativeView()));
}
	
bool EMBeMenuContainer::RemoveItem(EMMenuItem* p_opMenuItem)
{
	m_oMenuItems.remove(p_opMenuItem);
	return m_opNativeMenu -> RemoveItem(static_cast<EMBeMenuItem*>(p_opMenuItem -> GetNativeView()));
/*
	list<EMMenu*>::iterator oIterator = m_oMenus.begin();
	while(oIterator != m_oMenus.end())
	{
		(*oIterator) -> RemoveItem(p_opMenuItem);
		oIterator++;
	}
	m_oMenuItems.remove(p_opMenu);
*/
}
/*
void EMBeMenuBarContainer::SetApplicationMessageTarget(EMListener* p_opMessageTarget)
{
	m_opApplicationMessageTarget = p_opMessageTarget;
}
*/
void EMBeMenuContainer::SetMessageTarget(void* p_opMessageTarget)
{
	list<EMMenu*>::iterator oIterator = m_oMenus.begin();
	while(oIterator != m_oMenus.end())
	{
		(*oIterator) -> SetMessageTarget(p_opMessageTarget);
		oIterator++;
	}
	m_opNativeMenu -> SetTargetForItems(static_cast<BHandler*>(p_opMessageTarget));
}

void EMBeMenuContainer::Show()
{
	EMDebugger("Menus cannot be shown by using Show() in this implementation");
//	m_opNativeMenu -> Show();
}

#endif