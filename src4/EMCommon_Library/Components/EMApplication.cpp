#include "EMApplication.h"

#ifdef PLATFORM_WINDOWS

#include "EMMessages.h"
#include "EMWinMessages.h"

const char* EM_APPLICATION_VERSION_NUMBER = "1.0.1";

HINSTANCE EMApplication::m_opApplicationInstance = NULL;
#endif

EMApplication* EMApplication::m_opInstance = NULL;

#ifdef PLATFORM_BEOS
#include <Application.h>
#endif

EMApplication::EMApplication()
{
}

EMApplication::~EMApplication()
{
}

const char* EMApplication::GetApplicationVersionNumber()
{
	return EM_APPLICATION_VERSION_NUMBER;
}

EMApplication* EMApplication::Instance()
{
	if(m_opInstance == NULL)
		m_opInstance = EM_new EMApplication();

	return m_opInstance;
}

void EMApplication::InitiateQuitApplication()
{
#ifdef PLATFORM_WINDOWS
	PostMessage(GetActiveWindow(), NATIVE_MESSAGE_QUIT_APPLICATION, 0, 0);
#endif
}

#ifdef PLATFORM_WINDOWS

HINSTANCE EMApplication::GetApplicationInstance()
{
	return m_opApplicationInstance;
}

// The thread entering this method must not have locked any semaphores (this reduces the risk of deadlock)
void EMApplication::QuitApplication()
{
	// Phase one: Just a check to see if it is possible at all to quit the application.
	// If any listener returns true, the quit process is aborted
	if(Notify(EM_QUIT_PHASE_ONE))
	{
		Notify(EM_QUIT_ABORTED);
		return;
	}

	// Phase two: Playback should be stopped
	Notify(EM_QUIT_PHASE_TWO);

	// Phase three: As many threads as possible should be suspended/killed
	Notify(EM_QUIT_PHASE_THREE);

	// Phase four: All project data and other garbage should be cleaned up
	Notify(EM_QUIT_PHASE_FOUR);

	// Phase five: All open windows should close
	Notify(EM_QUIT_PHASE_FIVE);

	// Phase six: Misc stuff, prepare for the final shutdown
	Notify(EM_QUIT_PHASE_SIX);

	// Now, the system should be as ready to shut down as it can be
#ifdef PLATFORM_WINDOWS
	PostQuitMessage(0); // The plug is pulled, Titan goes belly up
#endif
}

void EMApplication::SetApplicationInstance(HINSTANCE p_opApplicationInstance)
{
	m_opApplicationInstance = p_opApplicationInstance;
}

#endif


