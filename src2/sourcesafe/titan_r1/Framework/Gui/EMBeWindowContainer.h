/*******************************************************
* Creator: Martin Rudefelt
* Portability: native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifdef PLATFORM_BEOS

#ifndef __EM_BE_WINDOW_CONTAINER
#define __EM_BE_WINDOW_CONTAINER

#include "EMGUIGlobals.h"
#include "EMRect.h"
#include "EMWindow.h"

class EMBeWindow;
class EMListener;
class EMView;

class EMBeWindowContainer : public EMWindow
{
public:
	EMBeWindowContainer(const EMRect p_oRect, const char* p_vpName, const EMWindowType p_oType);
	virtual ~EMBeWindowContainer();
	void AddChild(EMGUIComponent* p_opView);
	EMRect Bounds() const;
	EMRect Frame() const;
	void* GetNativeView() const;
	void Hide();
	bool IsHidden();
	bool Lock();
	void MoveTo(float p_vX, float p_vY);
	void SetBounds(EMRect p_oBounds);
	void SetFrame(EMRect p_oFrame);
	void SetSizeLimits(float p_vMinimumWidth, float p_vMaximumWidth, float p_vMinimumHeight, float p_vMaximumHeght);
	void SetTitle(const char* p_vpTitle);
	void Show();
	void Unlock();

private:
	EMBeWindow* m_opWindow; // This class retains retains ownership of its native window
};

#endif

#endif
