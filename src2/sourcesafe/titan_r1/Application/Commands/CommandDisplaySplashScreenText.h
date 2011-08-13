/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __COMMAND_DISPLAY_SPLASH_SCREEN_TEXT
#define __COMMAND_DISPLAY_SPLASH_SCREEN_TEXT

#include "EMCommand.h"

class SplashWindow;

class CommandDisplaySplashScreenText : public EMCommand
{
public:
	CommandDisplaySplashScreenText(SplashWindow* p_opSplashWindow);
	void ExecuteE() {};
	void* ExecuteE(void* p_upParameterOne, void* p_upParameterTwo, void* p_upParameterThree);
	bool IsUndoable();
	bool RequiresParameters();

private:
	SplashWindow* m_opSplashWindow;
};

#endif
