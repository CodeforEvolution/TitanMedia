/*******************************************************
* This code is the copyrighted property of Elegant Media
*-------------------------------------------------------
* Creator: Johan Dunberg
* Component: Command
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/
#include "EMGlobals.h"

#ifndef __COMMAND_DISPLAY_SPLASH_WINDOW
#define __COMMAND_DISPLAY_SPLASH_WINDOW

#include "EMCommand.h"

class SplashWindow;

class CommandDisplaySplashWindow : public EMCommand
{
public:
	CommandDisplaySplashWindow();
	void ExecuteE();
	void* ExecuteE(void*, void*, void* ){return NULL;};
	bool IsUndoable();
	bool RequiresParameters();

private:
	SplashWindow* m_opWindow;
	bool m_vInitialized;
};

#endif