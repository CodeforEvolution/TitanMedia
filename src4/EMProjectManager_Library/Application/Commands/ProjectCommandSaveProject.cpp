#include "ProjectCommandSaveProject.h"

//#include "EMProjectDataContainer.h"
#include "ProjectManager.h"

#include <list>

ProjectCommandSaveProject::ProjectCommandSaveProject()
{
}

void* ProjectCommandSaveProject::ExecuteE(void* p_upParameterOne, void* p_upParameterTwo, void* p_upParameterThree)
{
	uint32 vID = *(static_cast<uint32*>(p_upParameterOne));
//	string* opPath = static_cast<string*>(p_upParameterTwo);
	ProjectManager::Instance() -> SaveProject(vID);//, ProjectManager::Instance() -> GetProjectDirectory(*vpID));
//	delete vpID;
//	delete opPath;
	return NULL;
}

bool ProjectCommandSaveProject::IsUndoable()
{
	return false;
}

bool ProjectCommandSaveProject::RequiresParameters()
{
	return true;
}

