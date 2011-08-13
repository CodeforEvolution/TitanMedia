#include "EMBeMenuItemContainer.h"

#ifdef PLATFORM_BEOS

#include "EMCommand.h"
#include "EMCommandRepository.h"

#include <Message.h>

EMBeMenuItemContainer::EMBeMenuItemContainer(string* p_opName, uint32 p_vCommand)// :
//m_opNativeItem(new BMenuItem(p_opName -> c_str(), new BMessage(p_vCommand)))
{
	BMessage* opMessage = new BMessage(p_vCommand);
	opMessage -> AddBool("is menu command", true);
	m_opNativeItem = new BMenuItem(p_opName -> c_str(), opMessage);

}

EMBeMenuItemContainer::~EMBeMenuItemContainer()
{
	delete m_opNativeItem;
}
/*
void EMBeMenuItemContainer::ExecuteCommand()
{
	EMCommandRepository::Instance() -> ExecuteCommand(m_vCommand);
}
*/
/*
EMRect EMBeMenuItemContainer::Bounds() const
{
	BRect oRect = m_opNativeItem -> Bounds();
	return EMRect(oRect.left, oRect.top, oRect.right, oRect.bottom);
}
*/
bool EMBeMenuItemContainer::GetEnabled()
{
	return m_opNativeItem -> IsEnabled();
}

bool EMBeMenuItemContainer::GetMarked()
{
	return m_opNativeItem -> IsMarked();
}

const char* EMBeMenuItemContainer::GetName()
{
	return m_opNativeItem -> Label();
}

void* EMBeMenuItemContainer::GetNativeView() const
{
	return m_opNativeItem;
}
/*
void EMBeMenuItemContainer::Hide()
{
	EMDebugger("Menus cannot be hidden by using Hide() in this implementation");
}
*/	
/*
void EMBeMenuItemContainer::Init()
{
	m_opNativeItem -> SetTarget(m_opNativeItem);
}
*/
void EMBeMenuItemContainer::SetEnabled(bool p_vIsEnabled)
{
	m_opNativeItem -> SetEnabled(p_vIsEnabled);
}

void EMBeMenuItemContainer::SetMarked(bool p_vIsMarked)
{
	m_opNativeItem -> SetMarked(p_vIsMarked);
}
/*
void EMBeMenuItemContainer::Show()
{
	EMDebugger("Menus cannot be shown by using Show() in this implementation");
}
*/	

void EMBeMenuItemContainer::SetName(char* p_vpName)
{
	m_opNativeItem -> SetLabel(p_vpName);
}

#endif

