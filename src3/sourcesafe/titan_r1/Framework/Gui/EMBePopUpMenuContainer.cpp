#include "EMBePopUpMenuContainer.h"

#ifdef PLATFORM_BEOS

#include "EMMenu.h"
#include "EMMenuItem.h"

EMBePopUpMenuContainer::EMBePopUpMenuContainer() :
m_opNativeMenu(new EMBePopUpMenu(this))
{
}

EMBePopUpMenuContainer::~EMBePopUpMenuContainer()
{
;//cout_commented_out_4_release << "EMBePopUpMenuContainer::~EMBePopUpMenuContainer 1" << endl;
	list<EMMenu*>::iterator oMenuIterator;
	while(m_oMenus.size() > 0)
	{
		oMenuIterator = m_oMenus.begin();
		m_oMenus.pop_front();
		delete *oMenuIterator;
	}
;//cout_commented_out_4_release << "EMBePopUpMenuContainer::~EMBePopUpMenuContainer 2" << endl;
	list<EMMenuItem*>::iterator oMenuItemIterator;
	while(m_oMenus.size() > 0)
	{
		oMenuItemIterator = m_oMenuItems.begin();
		m_oMenuItems.pop_front();
		delete *oMenuItemIterator;
	}
;//cout_commented_out_4_release << "EMBePopUpMenuContainer::~EMBePopUpMenuContainer 3" << endl;
//	delete m_opNativeMenu;
;//cout_commented_out_4_release << "EMBePopUpMenuContainer::~EMBePopUpMenuContainer 4" << endl;
}

bool EMBePopUpMenuContainer::AddItem(EMMenu* p_opMenu)
{
	m_oMenus.push_back(p_opMenu);
	return m_opNativeMenu -> AddItem((BMenu*) p_opMenu -> GetNativeView());
}

bool EMBePopUpMenuContainer::AddItem(EMMenuItem* p_opMenuItem)
{
	m_oMenuItems.push_back(p_opMenuItem);
	bool vResult = m_opNativeMenu -> AddItem((BMenuItem*) p_opMenuItem -> GetNativeView());
	return vResult;
}

bool EMBePopUpMenuContainer::AddSeparatorItem()
{
	return m_opNativeMenu -> AddSeparatorItem();
}

void* EMBePopUpMenuContainer::GetNativeView() const
{
	return m_opNativeMenu;
}

void EMBePopUpMenuContainer::PopUp(float p_vX, float p_vY)
{
	m_opNativeMenu -> Go(BPoint(p_vX, p_vY), true, false, true);
//;//cout_commented_out_4_release << "EMBePopUpMenuContainer::PopUp: " << m_opNativeMenu -> SetTargetForItems(m_opNativeMenu) << endl;
}

#endif