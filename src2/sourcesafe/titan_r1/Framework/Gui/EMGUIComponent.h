/*******************************************************
* Creator: Martin Rudefelt
* Component: none
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_GUI_COMPONENT
#define __EM_GUI_COMPONENT

//#include "EMGUIGlobals.h"
#include "EMListenerRepository.h"
#include "EMRect.h"

class EMListener;

class EMGUIComponent : public EMListenerRepository
{
public:
	virtual ~EMGUIComponent();
	virtual bool AddToComponent(EMGUIComponent* p_opComponent);
	virtual bool CanGetTab();
	virtual void* GetNativeView() const = 0;
	virtual uint32 GetResizingMode();
//	virtual EMRect Bounds() const = 0;
	virtual EMRect Frame() const = 0;
	virtual void InitComponent();
	virtual bool IsEnabled();
	virtual bool IsVisible();
	virtual void SetFocus();
	virtual void SetFrame(EMRect p_oFrame) = 0;
	virtual void Show() = 0;
	virtual void Hide() = 0;

protected:
	EMGUIComponent();
};

#endif
