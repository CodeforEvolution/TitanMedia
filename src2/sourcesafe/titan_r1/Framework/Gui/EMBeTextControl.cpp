#include "EMBeTextView.h"

#ifdef PLATFORM_BEOS

#include "EMBeTextControlContainer.h"
#include "EMBeMessageTranslator.h"
#include "EMGUIComponent.h"
#include "EMListener.h"
#include "EMMessages.h"

#include <Message.h>
#include <TextView.h>

//const char* MESSAGE_DONE = "Control Done";

EMBeTextControl::EMBeTextControl(EMBeTextControlContainer* p_opContainer, BRect p_oFrame, const char* p_vpText, uint32 p_vResizingMode) :
BTextControl(p_oFrame, NULL, NULL, p_vpText, new BMessage(EM_TEXT_CHANGED), p_vResizingMode),
m_opContainer(p_opContainer),
m_vHasBeenFocused(false)
{
//	TextView() -> Insert("x");
//	SetTarget(this);
}
/*
bool EMBeTextControl::CanEndLine(int32 p_vOffset)
{
	return false;
}
*/

void EMBeTextControl::DetachedFromWindow()
{
	;//cout_commented_out_4_release << "Detached!!" << endl;
}

void EMBeTextControl::Draw(BRect p_oRect)
{
/*
	// Hackorama 2000 - Very ugly hack indeed!
	if(TextView() -> IsFocus())
		m_vHasBeenFocused = true;
	else if(m_vHasBeenFocused)
	{
		RemoveSelf();
		delete this;
//		m_opContainer -> Notify(EM_FOCUS_CHANGED);
//		m_opContainer -> DeleteTextControl();
	}
	else
*/
		BTextControl::Draw(p_oRect);
}
/*
void EMBeTextControl::MakeFocus(bool p_vFocus)
{
	;//cout_commented_out_4_release << "EMBeTextControl::MakeFocus" << endl;
	BTextControl::MakeFocus(p_vFocus);
}
*/
void EMBeTextControl::MessageReceived(BMessage* p_opMessage)
{
	;//cout_commented_out_4_release << "EMBeTextView::MessageReceived" << endl;
	uint32 vMessage = EMBeMessageTranslator::TranslateMessage(p_opMessage -> what);
	if(!(m_opContainer -> Notify(vMessage)))
		BTextControl::MessageReceived(p_opMessage);
}

#endif