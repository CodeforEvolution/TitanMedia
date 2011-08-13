#include "EMBeMenu.h"

#ifdef PLATFORM_BEOS

#include "EMBeMessageTranslator.h"
#include "EMBeMenuContainer.h"
#include "EMCommandRepository.h"
#include "EMListener.h"

#include <Message.h>
#include <Rect.h>

EMBeMenu::EMBeMenu(EMBeMenuContainer* p_opContainer, const char* p_vpLabel) :
BMenu(p_vpLabel),
m_opContainer(p_opContainer)
{
}

EMBeMenu::~EMBeMenu()
{
}

void EMBeMenu::MessageReceived(BMessage* p_opMessage)
{
;//cout_commented_out_4_release << "***** EMBeMenu::MessageReceived" << endl;
	uint32 vMessage = EMBeMessageTranslator::TranslateMessage(p_opMessage -> what);
	if(!(m_opContainer -> Notify(vMessage)))
		BMenu::MessageReceived(p_opMessage);
}

#endif

