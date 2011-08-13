#include "EMGlobals.h"

#ifdef PLATFORM_BEOS

#ifndef __EM_BE_TEXT_CONTROL
#define __EM_BE_TEXT_CONTROL

#include <Rect.h>
#include <TextControl.h>

class EMBeTextControlContainer;
class EMListener;

class EMBeTextControl : public BTextControl
{
public:
	EMBeTextControl(EMBeTextControlContainer* p_opContainer, BRect p_oFrame, const char* p_vpText, uint32 p_vResizingMode);
//	bool CanEndLine(int32 p_vOffset);
	void DetachedFromWindow();
	void Draw(BRect p_oRect);
//	void MakeFocus(bool p_vFocus = true);
	void MessageReceived(BMessage* p_opMessage);

private:
	EMBeTextControlContainer* m_opContainer;
	bool m_vHasBeenFocused;
};

#endif

#endif

