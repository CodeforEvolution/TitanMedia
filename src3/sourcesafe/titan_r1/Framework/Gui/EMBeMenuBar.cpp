#include "EMBeMenuBar.h"

#ifdef PLATFORM_BEOS

#include "EMBeMessageTranslator.h"
#include "EMBeMenuBarContainer.h"
#include "EMCommandRepository.h"
#include "EMListener.h"

#include <Message.h>
#include <Rect.h>

EMBeMenuBar::EMBeMenuBar(EMBeMenuBarContainer* p_opContainer, char* p_vpLabel) :
BMenuBar(BRect(), p_vpLabel),
m_opContainer(p_opContainer)
{
}

EMBeMenuBar::~EMBeMenuBar()
{
}

void EMBeMenuBar::MessageReceived(BMessage* p_opMessage)
{
	;//cout_commented_out_4_release << "EMBeMenuBar::MessageReceived" << endl;
//	uint32 vMessage = EMBeMessageTranslator::TranslateMessage(p_opMessage -> what);
	// First, check to see if it really is a valid message from a menu item
	bool vIsCommand;
	p_opMessage -> FindBool("is menu command", 0, &vIsCommand);
	if(!vIsCommand)
		return BMenuBar::MessageReceived(p_opMessage);
	if(!(m_opContainer -> Notify(p_opMessage -> what)))
		BMenuBar::MessageReceived(p_opMessage);
}

#endif
