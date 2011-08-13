#include "ProjectCommandCloseProject.h"

#include "EMProjectDataContainer.h"
#include "ProjectManager.h"

#include <list>

ProjectCommandCloseProject::ProjectCommandCloseProject()
{
}

// Note: This command also deletes the project manager if there are no more projects
void* ProjectCommandCloseProject::ExecuteE(void* p_upParameterOne, void* p_upParameterTwo, void* p_upParameterThree)
{
	uint32 vID = *(static_cast<uint32*>(p_upParameterOne));
	ProjectManager::Instance() -> CloseProject(vID);
// FIXME: it crashes if ProjectCommandCreateProject is triggered after ProjectManager has been deleted
//	if(ProjectManager::Instance() -> GetNrProjects() == 0)
//		delete ProjectManager::Instance();
	return NULL;
}

bool ProjectCommandCloseProject::IsUndoable()
{
	return false;
}

bool ProjectCommandCloseProject::RequiresParameters()
{
	return true;
}

