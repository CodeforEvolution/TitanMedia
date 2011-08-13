#include "ProjectCommandLoadSystemSettings.h"

#include "DirectoryIDs.h"
#include "EMProjectDataContainer.h"
#include "EMProjectDataLoader.h"
#include "ProjectManager.h"

#include <string>

const char* SYSTEM_LOAD_SETTINGS_FILE_NAME = "system_settings.dat";

ProjectCommandLoadSystemSettings::ProjectCommandLoadSystemSettings()
{
}

void* ProjectCommandLoadSystemSettings::ExecuteE(void* p_upParameterOne, void* p_upParameterTwo, void* p_upParameterThree)
{
	EMProjectDataContainer* opDataContainer = static_cast<EMProjectDataContainer*>(p_upParameterOne);

	string oPath = ProjectManager::Instance() -> GetDirectory(0, DIRECTORY_ID_SYSTEM);

	EMProjectDataLoader oLoader;

	if(!oLoader.InitializeLoad(oPath + EM_FILE_SEPARATOR + SYSTEM_LOAD_SETTINGS_FILE_NAME, false))
	{
		//EMDebugger("Could not initialize load");
		// If the system settings cannot be loaded, return false so that default system settings can be set and hopefully saved
		return reinterpret_cast<void*>(false);
	}

	oLoader.PrepareForNewDataContainer();
	opDataContainer -> LoadData(&oLoader);
	oLoader.DataContainerIsFinished();
	oLoader.CloseFile();

	return reinterpret_cast<void*>(true);
}

bool ProjectCommandLoadSystemSettings::IsUndoable()
{
	return false;
}

bool ProjectCommandLoadSystemSettings::RequiresParameters()
{
	return true;
}

