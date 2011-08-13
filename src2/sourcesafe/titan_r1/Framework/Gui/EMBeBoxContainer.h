/*******************************************************
* Creator: Martin Rudefelt
* Portability: native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifdef PLATFORM_BEOS

#ifndef __EM_BE_BOX_CONTAINER
#define __EM_BE_BOX_CONTAINER

#include "EMBorder.h"

#include "EMGUIGlobals.h"
#include "EMRect.h"

#include <memory>

class EMBeBox;
class EMView;
class EMWindow;

class EMBeBoxContainer : public EMBorder
{
public:
	EMBeBoxContainer(EMRect p_oFrame, uint32 p_vResizingMode, EMBorderStyle p_vStyle, const char* p_vLabel);
	~EMBeBoxContainer();
	void AddChild(EMGUIComponent* p_opView);
	EMRect Frame() const;
	void* GetNativeView() const;
	void Hide();
	void SetColor(EMColor p_sColor);
	void Show();

private:
	auto_ptr<EMBeBox> m_opBox;
};

#endif

#endif

