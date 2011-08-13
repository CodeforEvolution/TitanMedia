/*******************************************************
* Creator: Martin Rudefelt
* Portability: native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifdef PLATFORM_BEOS

#ifndef __EM_BE_TEXT_CONTROL_CONTAINER
#define __EM_BE_TEXT_CONTROL_CONTAINER

#include "EMBeTextControl.h"
#include "EMTextControl.h"

class EMBeTextControlContainer : public EMTextControl
{
public:
	EMBeTextControlContainer(EMRect p_oFrame, const char* p_vpText, uint32 p_vResizingMode);
	~EMBeTextControlContainer();
	EMRect Frame() const;
	void* GetNativeView() const;
	void Hide();
	void InitComponent();
	void Invalidate();
	void SelectAll();
	void SetFocus();
	void SetFrame(EMRect p_oFrame);
	void SetText(const char* p_vpText);
	void Show();
	void SetMaxBytes(int32 p_vMaxBytes);
//	void SetWordWrap(bool p_vWrap);
	const char* Text();
	
private:
	EMBeTextControl* m_opTextControl;
};

#endif

#endif
