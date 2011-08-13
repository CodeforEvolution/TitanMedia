#include "ProjectCommandMoveProject.h"

#include "EMProjectDataContainer.h"
#include "ProjectManager.h"

#include <list>

ProjectCommandMoveProject::ProjectCommandMoveProject()
{
}

// Note: This command also deletes the project manager if there are no more projects
void* ProjectCommandMoveProject::ExecuteE(void* p_upParameterOne, void* p_upParameterTwo, void* p_upParameterThree)
{
	uint32 vID = *(static_cast<uint32*>(p_upParameterOne));
	ProjectManager::Instance() -> MoveProject(vID, const_cast<const char*>(p_upParameterTwo));

	return NULL;
}

bool ProjectCommandMoveProject::IsUndoable()
{
	return false;
}

bool ProjectCommandMoveProject::RequiresParameters()
{
	return true;
}

