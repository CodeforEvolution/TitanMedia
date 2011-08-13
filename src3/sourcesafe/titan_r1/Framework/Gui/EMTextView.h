/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_TEXT_VIEW
#define __EM_TEXT_VIEW

#include "EMGUIComponent.h"
#include "EMRect.h"

class EMTextView : public EMGUIComponent
{
public:
	virtual ~EMTextView();
	virtual void Insert(const char* p_vpText) = 0;
	virtual void Invalidate() = 0;
	virtual void SelectAll() = 0;
	virtual void SetFocus() = 0;
	virtual void SetFrame(EMRect p_oRect) = 0;
	virtual void SetMaxBytes(int32 p_vMaxBytes) = 0;
	virtual void SetText(const char* p_vpText) = 0;
	virtual void SetWordWrap(bool p_vWrap) = 0;
	virtual const char* Text() = 0;

protected:
	EMTextView();
};

#endif

