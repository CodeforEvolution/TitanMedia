#include "EMBeTextView.h"

#ifdef PLATFORM_BEOS

#include "EMBeTextViewContainer.h"
#include "EMBeMessageTranslator.h"
#include "EMGUIComponent.h"
#include "EMListener.h"
#include "EMMessages.h"

#include <Message.h>

EMBeTextView::EMBeTextView(EMBeTextViewContainer* p_opContainer, BRect p_oFrame, BRect p_oTextFrame, const char* p_vpName, uint32 p_vResizingMode) :
BTextView(p_oFrame, p_vpName, p_oTextFrame, p_vResizingMode, B_FRAME_EVENTS | B_PULSE_NEEDED | B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE),
m_opContainer(p_opContainer)
{
	DisallowChar(B_ENTER);
	DisallowChar(B_ESCAPE);
}

void EMBeTextView::InsertText(const char* p_vpText, int32 p_vLength, int32 p_vOffset, const text_run_array* p_opRuns)
{
	;//cout_commented_out_4_release << "EMBeTextView::InsertText" << endl;
	/*
	if(p_vLength == 1 && p_vpText[0] == B_ENTER)
	{
		m_opContainer -> Notify(EM_TEXT_CHANGED);
		RemoveSelf();
	}
	else
	*/
		BTextView::InsertText(p_vpText, p_vLength, p_vOffset, p_opRuns);
}

void EMBeTextView::KeyDown(const char* p_vpBytes, int32 p_vLength)
{
	;//cout_commented_out_4_release << "EMBeTextView::KeyDown" << endl;
	if(p_vLength == 1 && p_vpBytes[0] == B_ENTER)
	{
		m_opContainer -> Notify(EM_TEXT_CHANGED);
		RemoveSelf();
	}
	if(p_vLength == 1 && p_vpBytes[0] == B_ESCAPE)
	{
		m_opContainer -> Notify(EM_TEXT_CHANGE_UNDOED);
		RemoveSelf();
	}
	BTextView::KeyDown(p_vpBytes, p_vLength);
}

void EMBeTextView::MakeFocus(bool p_vFocus)
{
	;//cout_commented_out_4_release << "EMBeTextView::HaveFocus" << endl;
	BTextView::MakeFocus(p_vFocus);
	if(!p_vFocus)
	{
		m_opContainer -> Notify(EM_TEXT_CHANGED);
		RemoveSelf();
	}
}

bool EMBeTextView::CanEndLine(int32 p_vOffset)
{
	return false;
}

/*
void EMBeTextView::MessageReceived(BMessage* p_opMessage)
{
	;//cout_commented_out_4_release << "EMBeTextView::MessageReceived" << endl;
	uint32 vMessage = EMBeMessageTranslator::TranslateMessage(p_opMessage -> what);
	if(!(m_opContainer -> Notify(vMessage)))
		BTextView::MessageReceived(p_opMessage);
}
*/
#endif