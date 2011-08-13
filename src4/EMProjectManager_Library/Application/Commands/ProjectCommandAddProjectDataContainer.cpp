#include "ProjectCommandAddProjectDataContainer.h"

#include "EMProjectDataContainer.h"
#include "ProjectManager.h"

ProjectCommandAddProjectDataContainer::ProjectCommandAddProjectDataContainer()
{
}

void* ProjectCommandAddProjectDataContainer::ExecuteE(void* p_upParameterOne, void* p_upParameterTwo, void* p_upParameterThree)
{
	EMProjectDataContainer* opContainer = static_cast<EMProjectDataContainer*>(p_upParameterOne);
	ProjectManager::Instance() -> AddProjectDataContainer(opContainer);
	return NULL;
}

bool ProjectCommandAddProjectDataContainer::IsUndoable()
{
	return false;
}

bool ProjectCommandAddProjectDataContainer::RequiresParameters()
{
	return true;
}

