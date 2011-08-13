#include "EMBeMenuItem.h"

#ifdef PLATFORM_BEOS

#include "EMBeMenuBarContainer.h"
#include "EMBeMessageTranslator.h"

#include <Message.h>

EMBeMenuItem::EMBeMenuItem(const char* p_vpLabel) :
BMenuItem(p_vpLabel, new BMessage(45))//NULL)//, new BMessage(p_vCommand))
{
//	m_vCommand = p_vCommand;
}

EMBeMenuItem::~EMBeMenuItem()
{
}
/*
status_t EMBeMenuItem::Invoke(BMessage* p_opMessage)
{
;//cout_commented_out_4_release << "Invoke!" << endl;
	BMenuItem::Invoke(p_opMessage);
}
*/
void EMBeMenuItem::SetLabel(char const* p_vpLabel)
{
	;//cout_commented_out_4_release << "Gronk" << endl;
}

/*
void EMBeMenuItem::ExecuteCommand()
{
	EMCommandRepository::Instance() -> ExecuteCommand(m_vCommand);
}
*/
/*
void EMBeMenuItem::MessageReceived(BMessage* p_opMessage)
{
//	uint32 vMessage = EMBeMessageTranslator::TranslateMessage(p_opMessage -> what);
//	if(!(m_opContainer -> Notify(vMessage)))
	BMenuItem::MessageReceived(p_opMessage);
	EMCommandRepository::Instance() -> ExecuteCommand(m_vCommand);
}
*/
#endif

