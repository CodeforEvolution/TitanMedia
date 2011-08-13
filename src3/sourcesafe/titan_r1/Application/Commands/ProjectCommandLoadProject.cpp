#include "ProjectCommandLoadProject.h"

//#include "EMProjectDataContainer.h"
#include "ProjectManager.h"

//#include <list>
//#include <string>

ProjectCommandLoadProject::ProjectCommandLoadProject()
{
}

void* ProjectCommandLoadProject::ExecuteE(void* p_upParameterOne, void* p_upParameterTwo, void* p_upParameterThree)
{
	//string* opPath = static_cast<string*>(p_upParameterTwo);
	uint32 vID = ProjectManager::Instance() -> LoadProject(const_cast<const char*>(p_upParameterOne));
	return reinterpret_cast<void*>(vID);
}

bool ProjectCommandLoadProject::IsUndoable()
{
	return false;
}

bool ProjectCommandLoadProject::RequiresParameters()
{
	return true;
}

