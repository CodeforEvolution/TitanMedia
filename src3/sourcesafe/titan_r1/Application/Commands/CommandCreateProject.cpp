#include "CommandCreateProject.h"

#include "EMFactory.h"
#include "EMFileWindow.h"
#include "EMListener.h"
#include "EMMediaGlobals.h"
#include "EMMessages.h"
#include "FileDialogTarget.h"
#include "TrackWindow.h"

#include <string>

#define FILE_WINDOW_TITLE "Create New Project"

CommandCreateProject::CommandCreateProject(FileDialogTarget* p_opFileDialogTarget) :
m_opFileDialogTarget(p_opFileDialogTarget)
{
}

void* CommandCreateProject::ExecuteE(void* p_upParameterOne, void* p_upParameterTwo, void* p_upParameterThree)
{
	EMListener* opListener = NULL;
	if(p_upParameterOne != NULL)
		opListener = static_cast<EMListener*>(p_upParameterOne);
	char* vpStartDirectory = "/boot/home/";
	EMFileWindow* m_opFileWindow = EMFactory::Instance() -> CreateFileWindow(FILE_PANEL_SAVE_FILE, FILE_WINDOW_TITLE, NULL, false, NULL, NULL, vpStartDirectory, EM_CREATE_PROJECT_REQUESTED);
	m_opFileWindow -> AddListener(m_opFileDialogTarget);
	m_opFileWindow -> AddListener(opListener);
	return NULL;
}

bool CommandCreateProject::IsUndoable()
{
	return false;
}

bool CommandCreateProject::RequiresParameters()
{
	return true;
}
