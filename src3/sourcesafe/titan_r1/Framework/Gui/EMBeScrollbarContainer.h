/*******************************************************
* Creator: Martin Rudefelt
* Portability: native
*-------------------------------------------------------
* The BScrollbar seems to draw outside its bounds and
* this class corrects that error
*******************************************************/

#include "EMGlobals.h"

#ifdef PLATFORM_BEOS

#ifndef __EM_BE_SCROLLBAR_CONTAINER
#define __EM_BE_SCROLLBAR_CONTAINER

#include "EMBeScrollbar.h"
#include "EMGUIGlobals.h"
#include "EMRect.h"
#include "EMScrollbar.h"

class EMBeScrollbarContainer : public EMScrollbar
{
public:
	EMBeScrollbarContainer(EMRect p_oFrame, float p_vMinValue, float p_vMaxValue, EMOrientation p_vOrientation);
	~EMBeScrollbarContainer();
	virtual EMRect Frame() const;
	virtual void* GetNativeView() const;
	void GetRange(float& p_vMin, float& p_vMax);
	void GetSteps(float& p_vSmallStep, float& p_vBigStep);
	float GetValue();
	void Hide();
	EMOrientation Orientation();
	void SetFrame(EMRect p_oFrame);
	void SetProportion(float p_vProportion);
	void SetRange(float p_vMin, float p_vMax);
	void SetSteps(float p_vSmallStep, float p_vBigStep);
	void SetValue(float p_vValue);
	void Show();

private:
	EMBeScrollbar* m_opScrollbar;
};

#endif

#endif
