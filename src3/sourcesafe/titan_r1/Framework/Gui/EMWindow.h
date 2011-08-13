/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_WINDOW
#define __EM_WINDOW

#include "EMGUIComponent.h"
#include "EMRect.h"
#include <vector>

class EMWindow : public EMGUIComponent
{
public:
	virtual ~EMWindow();
	virtual void AddChild(EMGUIComponent* p_opView) = 0;
	void AddTabComponent(EMGUIComponent* p_opComponent);
	virtual EMRect Bounds() const = 0;
	virtual void ConvertFromWorkAreaToWindow(EMRect &p_oRect) = 0;
	virtual bool IsHidden() = 0;
	virtual bool IsMaximized() = 0;
	virtual bool Lock() = 0;
	virtual void MoveTo(int32 p_vX, int32 p_vY) = 0;
	void NextTab();
	void PreviousTab();
	virtual void SetBounds(EMRect p_oBounds) = 0;
	virtual void SetCurrentFocus() = 0;
	virtual void SetFrame(EMRect p_oBounds) = 0;
	virtual void SetFrame(EMRect p_oBounds, bool p_vAdjustRect) = 0;
	virtual void SetMaximized(bool p_vMaximized) = 0;
	virtual void SetSizeLimits(int32 p_vMinimumWidth, int32 p_vMaximumWidth, int32 p_vMinimumHeight, int32 p_vMaximumHeght) = 0;
	virtual void SetTitle(const char* p_vpTitle) = 0;
	virtual void Unlock() = 0;
	
protected:
	EMWindow();

protected:
	vector<EMGUIComponent*> __m_oTabOrder;
	int32 __m_vCurrentComponent;
	bool __m_vNextWasCalled;
	bool __m_vPreviosWasCalled;
};

#endif



