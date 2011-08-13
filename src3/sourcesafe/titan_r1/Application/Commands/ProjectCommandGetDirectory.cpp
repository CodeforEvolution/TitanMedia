#include "ProjectCommandGetDirectory.h"

#include "ProjectManager.h"

#include <string>

ProjectCommandGetDirectory::ProjectCommandGetDirectory()
{
}

// Ownership of parameters is transferred to this object
// Ownership of the return value is transferred to the caller
void* ProjectCommandGetDirectory::ExecuteE(void* p_upParameterOne, void* p_upParameterTwo, void* p_upParameterThree)
{
	int* vpProjectID = static_cast<int*>(p_upParameterOne);
	uint32* vpDirectoryID = static_cast<uint32*>(p_upParameterTwo);
	string oPathString(ProjectManager::Instance() -> GetDirectory((*vpProjectID), (*vpDirectoryID)));
	m_oReturnValue = oPathString;
	return &m_oReturnValue;
//	string* opDirectory = new string(oPathString);
//	return opDirectory;
}

bool ProjectCommandGetDirectory::IsUndoable()
{
	return false;
}

bool ProjectCommandGetDirectory::RequiresParameters()
{
	return true;
}

