/*******************************************************
* Creator: Martin Rudefelt
* Portability: native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifdef PLATFORM_BEOS

#ifndef __EM_BE_TEXT_VIEW
#define __EM_BE_TEXT_VIEW

#include <Rect.h>
#include <TextView.h>

class EMBeTextViewContainer;
class EMListener;

class EMBeTextView : public BTextView
{
public:
	EMBeTextView(EMBeTextViewContainer* p_opContainer, BRect p_oFrame, BRect p_oTextFrame, const char* p_vpName, uint32 p_vResizingMode);
	void InsertText(const char* p_vpText, int32 p_vLength, int32 p_vOffset, const text_run_array* p_opRuns);
	void KeyDown(const char* p_vpBytes, int32 p_vLength);
	void MakeFocus(bool p_vFocus = true);
	bool CanEndLine(int32 p_vOffset);
//	void MessageReceived(BMessage* p_opMessage);

private:
	EMBeTextViewContainer* m_opContainer;
};

#endif

#endif

