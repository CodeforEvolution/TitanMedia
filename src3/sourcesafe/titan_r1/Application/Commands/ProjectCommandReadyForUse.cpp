#include "ProjectCommandReadyForUse.h"

#include "ProjectManager.h"
ProjectCommandReadyForUse::ProjectCommandReadyForUse()
{
}

void ProjectCommandReadyForUse::ExecuteE()
{
	ProjectManager::Instance() -> ReadyForUse();
}

bool ProjectCommandReadyForUse::IsUndoable()
{
	return false;
}

bool ProjectCommandReadyForUse::RequiresParameters()
{
	return false;
}

