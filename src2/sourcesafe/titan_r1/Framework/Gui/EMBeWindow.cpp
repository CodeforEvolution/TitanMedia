#include "EMBeWindow.h"

#ifdef PLATFORM_BEOS

#include "EMBeMessageTranslator.h"
#include "EMBeWindowContainer.h"
#include "EMKeyboard.h"
#include "EMListener.h"
#include "EMMessages.h"

#include <Message.h>

EMBeWindow::EMBeWindow(EMBeWindowContainer* p_opContainer, const BRect p_oRect, const char* p_vpName, const window_type p_vType, uint32 p_vFlags) :
BWindow(p_oRect, p_vpName, p_vType, p_vFlags),
m_opContainer(p_opContainer)
{
}

EMBeWindow::EMBeWindow(EMBeWindowContainer* p_opContainer, const BRect p_oRect, const char* p_vpName, const window_look p_vLook, const window_feel p_vFeel, uint32 p_vFlags) :
BWindow(p_oRect, p_vpName, p_vLook, p_vFeel, p_vFlags),
m_opContainer(p_opContainer)
{
}

EMBeWindow::~EMBeWindow()
{
//	;//cout_commented_out_4_release << "EMBeWindow::~EMBeWindow()" << endl;
}

// Forwards native messages
void EMBeWindow::MessageReceived(BMessage* p_opMessage)
{
//;//cout_commented_out_4_release << "EMBeWindow::MessageReceived: " << p_opMessage -> what << endl;
	if(p_opMessage -> what == B_KEY_DOWN || p_opMessage -> what == B_KEY_UP)
	{
		int32 vKeyCode;
		p_opMessage -> FindInt32("raw_char", &vKeyCode);
		int32 vModifiers;
		p_opMessage -> FindInt32("modifiers", &vModifiers);
		if(p_opMessage -> what == B_KEY_DOWN)
			EMKeyboard::Instance() -> NotifyKey(m_opContainer,  vKeyCode, true, vModifiers);
		else
			EMKeyboard::Instance() -> NotifyKey(m_opContainer,  vKeyCode, false, vModifiers);
		return;
	}
	uint32 vMessage = EMBeMessageTranslator::TranslateMessage(p_opMessage -> what);
	if(!(m_opContainer -> Notify(vMessage)))
		BWindow::MessageReceived(p_opMessage);
}

bool EMBeWindow::QuitRequested()
{
	if(m_opContainer -> GetListenerAmount() > 0)
		return m_opContainer -> Notify(EM_QUIT_REQUESTED);
	return BWindow::QuitRequested();
}

#endif




















