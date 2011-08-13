#include "CommandDisplaySplashWindow.h"

#include "SplashWindow.h"

CommandDisplaySplashWindow::CommandDisplaySplashWindow() :
m_vInitialized(false)
{
}

void CommandDisplaySplashWindow::ExecuteE()
{
	if(!m_vInitialized)
	{
		m_opWindow = EM_new SplashWindow;
		m_opWindow -> Init();
		m_vInitialized = true;
		m_opWindow -> CenterWindow();
		m_opWindow -> Display();
	}
	else
	{
		m_opWindow -> Hide();
		delete m_opWindow;
		m_vInitialized = false;
	}
}

bool CommandDisplaySplashWindow::IsUndoable()
{
	return false;
}

bool CommandDisplaySplashWindow::RequiresParameters()
{
	return false;
}
