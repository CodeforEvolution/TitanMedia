#include "ProjectCommandCreateProject.h"

#include "EMProjectDataContainer.h"
#include "ProjectManager.h"

#include <list>

ProjectCommandCreateProject::ProjectCommandCreateProject()
{
}

// Ownership of the list transferred to this object
void* ProjectCommandCreateProject::ExecuteE(void* p_upParameterOne, void* p_upParameterTwo, void* p_upParameterThree)
{
	uint32 vID = *(static_cast<uint32*>(p_upParameterOne));
	ProjectManager::Instance() -> SaveProject(vID);
//	string* opPath = static_cast<string*>(p_upParameterOne);
//	int vProjectID = ProjectManager::Instance() -> CreateProject(/**opDataContainers, */*opPath);
//	ProjectManager::Instance() -> SaveProject(vProjectID, ProjectManager::Instance() -> GetProjectDirectory(vProjectID));
	return NULL;
}

bool ProjectCommandCreateProject::IsUndoable()
{
	return false;
}

bool ProjectCommandCreateProject::RequiresParameters()
{
	return true;
}

