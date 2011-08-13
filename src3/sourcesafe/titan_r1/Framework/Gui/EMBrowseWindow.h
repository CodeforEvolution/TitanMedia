/*******************************************************
* This code is the copyrighted property of Elegant Media
*-------------------------------------------------------
* Creator: Johan Dunberg
* Component: Framework
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/
#include "EMGlobals.h"

#ifndef __EM_BROWSE_WINDOW
#define __EM_BROWSE_WINDOW

#include "EMGUIComponent.h"

class EMBrowseWindow : public EMGUIComponent
{
public:
	virtual bool DisplayDialog() = 0;
	virtual string* GetPath() = 0;
	virtual bool IsHidden() = 0;

protected:
	EMBrowseWindow();
};

#endif
