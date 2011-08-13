/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_TEXT_CONTROL
#define __EM_TEXT_CONTROL

#include "EMGUIComponent.h"
#include "EMRect.h"

class EMTextControl : public EMGUIComponent
{
public:
	virtual ~EMTextControl();
	virtual void Enable(bool p_vEnable) = 0;
	void GetAutoClose();
	virtual void GetSelection(int32 &p_vStart, int32 &p_vEnd) = 0 ;
	virtual bool IsEnabled() = 0;
	virtual void SelectAll() = 0;
	virtual void SetFocus() = 0;
	virtual void SetFrame(EMRect p_oRect) = 0;
	virtual void SetMaxBytes(int32 p_vMaxBytes) = 0;
	virtual void SetSelection(int32 p_vStart, int32 p_vEnd) = 0; // If p_vStart == 0 and p_vEnd == -1 all text will be selected
	virtual void SetText(const char* p_vpText) = 0;
	virtual const char* Text() = 0;

protected:
	EMTextControl(bool p_vAutoClose);

private:
	bool m_vAutoClose;
};

#endif

