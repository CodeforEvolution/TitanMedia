#include "ProjectCommandSaveSystemSettings.h"

#include "DirectoryIDs.h"
#include "EMProjectDataContainer.h"
#include "EMProjectDataSaver.h"
#include "ProjectManager.h"

#include <string>

const char* SYSTEM_SAVE_SETTINGS_FILE_NAME = "system_settings.dat";

ProjectCommandSaveSystemSettings::ProjectCommandSaveSystemSettings()
{
}

void* ProjectCommandSaveSystemSettings::ExecuteE(void* p_upParameterOne, void* p_upParameterTwo, void* p_upParameterThree)
{
	EMProjectDataContainer* opDataContainer = static_cast<EMProjectDataContainer*>(p_upParameterOne);

	string oPath = ProjectManager::Instance() -> GetDirectory(0, DIRECTORY_ID_SYSTEM);

	EMProjectDataSaver oSaver;

	if(!oSaver.InitializeSave(oPath + EM_FILE_SEPARATOR + SYSTEM_SAVE_SETTINGS_FILE_NAME, false))
		return reinterpret_cast<void*>(false);

	oSaver.PrepareForNewDataContainer();
	opDataContainer -> SaveData(&oSaver);
	oSaver.DataContainerIsFinished();
	oSaver.CloseFile();

	return reinterpret_cast<void*>(true);
}

bool ProjectCommandSaveSystemSettings::IsUndoable()
{
	return false;
}

bool ProjectCommandSaveSystemSettings::RequiresParameters()
{
	return true;
}

