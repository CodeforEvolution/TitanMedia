/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_DEFAULT_SETTINGS_CONTAINER
#define __EM_DEFAULT_SETTINGS_CONTAINER

class EMDefaultSettingsContainer
{
public:
	virtual ~EMDefaultSettingsContainer() {};
	virtual void SetDefaultSettings() = 0;
};

#endif


