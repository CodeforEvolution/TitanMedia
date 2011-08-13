#include "ProjectCommandLoadTemplate.h"

//#include "EMProjectDataContainer.h"
#include "ProjectManager.h"

//#include <list>

ProjectCommandLoadTemplate::ProjectCommandLoadTemplate()
{
}

void* ProjectCommandLoadTemplate::ExecuteE(void* p_upParameterOne, void* p_upParameterTwo, void* p_upParameterThree)
{
	uint32 vID = ProjectManager::Instance() -> LoadTemplate(const_cast<const char*>(p_upParameterOne), const_cast<const char*>(p_upParameterTwo));
	return reinterpret_cast<void*>(vID);
/*
	string* opPath = static_cast<string*>(p_upParameterOne);
	string* opTemplate = static_cast<string*>(p_upParameterTwo);
	int vProjectID = ProjectManager::Instance() -> CreateProject(*opPath, *opTemplate);
	ProjectManager::Instance() -> SaveProject(vProjectID, ProjectManager::Instance() -> GetProjectDirectory(vProjectID));
	//const char* test = ProjectManager::Instance() -> GetProjectDirectory(vProjectID)
	return const_cast<char*>(ProjectManager::Instance() -> GetProjectDirectory(vProjectID));
*/
}

bool ProjectCommandLoadTemplate::IsUndoable()
{
	return false;
}

bool ProjectCommandLoadTemplate::RequiresParameters()
{
	return true;
}

