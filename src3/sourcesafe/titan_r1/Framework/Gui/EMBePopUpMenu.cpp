#include "EMBePopUpMenu.h"

#ifdef PLATFORM_BEOS

#include "EMBePopUpMenuContainer.h"

#include <Message.h>

EMBePopUpMenu::EMBePopUpMenu(EMBePopUpMenuContainer* p_opContainer) :
BPopUpMenu("PopUp Menu"),
m_opContainer(p_opContainer)
{
}
/*
EMBePopUpMenu::~EMBePopUpMenu()
{
	Hide();
	RemoveSelf();
}
*/
void EMBePopUpMenu::AttachedToWindow()
{
;//cout_commented_out_4_release << "EMBePopUpMenu::AttachedToWindow" << endl;
	SetTargetForItems(this);
	BPopUpMenu::AttachedToWindow();
}

void EMBePopUpMenu::MessageReceived(BMessage* p_opMessage)
{
//;//cout_commented_out_4_release << "EMBePopUpMenu::MessageReceived: " << p_opMessage -> what << endl;
	if(!(m_opContainer -> Notify(p_opMessage -> what)))
		BMenu::MessageReceived(p_opMessage);
}


#endif