#include "ProjectCommandSaveNeeded.h"

#include "EMProjectDataContainer.h"
#include "ProjectManager.h"

#include <list>

ProjectCommandSaveNeeded::ProjectCommandSaveNeeded()
{
}

void* ProjectCommandSaveNeeded::ExecuteE(void* p_upParameterOne, void* p_upParameterTwo, void* p_upParameterThree)
{
	bool vSaveNeeded = ProjectManager::Instance() -> SaveNeeded();
	return reinterpret_cast<void*>(vSaveNeeded);
}

bool ProjectCommandSaveNeeded::IsUndoable()
{
	return false;
}

bool ProjectCommandSaveNeeded::RequiresParameters()
{
	return true;
}

