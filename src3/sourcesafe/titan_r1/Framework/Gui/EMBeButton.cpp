#include "EMBeButton.h"

#ifdef PLATFORM_BEOS

#include "EMBeButtonContainer.h"
#include "EMBeMessageTranslator.h"
#include "EMFactory.h"
#include "EMGUIComponent.h"
#include "EMListener.h"

#include <Message.h>

EMBeButton::EMBeButton(EMBeButtonContainer* p_opContainer, const BRect p_oFrame, const char* p_vpLabel, BMessage* p_opMessage, uint32 p_vResizingMode) :
BButton(p_oFrame, NULL, p_vpLabel, p_opMessage, p_vResizingMode),
m_opContainer(p_opContainer)
{
}

void EMBeButton::MessageReceived(BMessage* p_opMessage)
{
	uint32 vMessage = EMBeMessageTranslator::TranslateMessage(p_opMessage -> what);
	if(!(m_opContainer -> Notify(vMessage)))
		BButton::MessageReceived(p_opMessage);
	m_opContainer -> ExecuteCommand();
}

#endif

