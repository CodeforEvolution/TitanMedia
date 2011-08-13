/*******************************************************
* Creator: Martin Rudefelt
* Portability: native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifdef PLATFORM_BEOS

#ifndef __EM_BE_TEXT_VIEW_CONTAINER
#define __EM_BE_TEXT_VIEW_CONTAINER

#include "EMBeTextView.h"
#include "EMTextView.h"

class EMBeTextViewContainer : public EMTextView
{
public:
	EMBeTextViewContainer(EMRect p_oFrame, const char* p_vpName, uint32 p_vResizingMode);
	~EMBeTextViewContainer();
	EMRect Frame() const;
	void* GetNativeView() const;
	void Hide();
	void InitComponent();
	void Insert(const char* p_vpText);
	void Invalidate();
	void SelectAll();
	void SetFocus();
	void SetFrame(EMRect p_oFrame);
	void SetText(const char* p_vpText);
	void Show();
	void SetMaxBytes(int32 p_vMaxBytes);
	void SetWordWrap(bool p_vWrap);
	const char* Text();
	
private:
	EMBeTextView* m_opTextView;
};

#endif

#endif
