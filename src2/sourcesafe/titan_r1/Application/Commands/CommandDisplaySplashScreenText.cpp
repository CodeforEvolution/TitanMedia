#include "CommandDisplaySplashScreenText.h"

#include "SplashWindow.h"

CommandDisplaySplashScreenText::CommandDisplaySplashScreenText(SplashWindow* p_opSplashWindow) :
m_opSplashWindow(p_opSplashWindow)
{
}

void* CommandDisplaySplashScreenText::ExecuteE(void* p_upParameterOne, void* p_upParameterTwo, void* p_upParameterThree)
{
	m_opSplashWindow -> DisplayText(const_cast<const char*>(p_upParameterOne));

	return NULL;
}

bool CommandDisplaySplashScreenText::IsUndoable()
{
	return false;
}

bool CommandDisplaySplashScreenText::RequiresParameters()
{
	return true;
}

