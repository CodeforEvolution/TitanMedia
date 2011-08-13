#include "EMProjectCommandFactory.h"

#include "CommandIDs.h"
#include "ProjectCommandAddProjectDataContainer.h"
#include "ProjectCommandCloseProject.h"
#include "ProjectCommandCreateProject.h"
#include "ProjectCommandGetDirectory.h"
#include "ProjectCommandLoadProject.h"
#include "ProjectCommandLoadSystemSettings.h"
#include "ProjectCommandLoadTemplate.h"
#include "ProjectCommandReadyForUse.h"
#include "ProjectCommandSaveNeeded.h"
#include "ProjectCommandSaveProject.h"
#include "ProjectCommandSaveSystemSettings.h"
#include "EMProjectManager_LibraryVersion.h" 

extern "C" __declspec(dllexport) EMProjectCommandFactory* InstanceProjectCommandFactory() { return EMProjectCommandFactory::Instance(); };
extern "C" __declspec(dllexport) void DeleteProjectCommandFactory() { delete EMProjectCommandFactory::Instance(); };

EMProjectCommandFactory* EMProjectCommandFactory::m_opInstance = NULL;

EMProjectCommandFactory::EMProjectCommandFactory()
{
}

EMProjectCommandFactory::~EMProjectCommandFactory()
{
} 

EMProjectCommandFactory* EMProjectCommandFactory::Instance()
{
	if(m_opInstance == NULL)
		m_opInstance = EM_new EMProjectCommandFactory();

	eo << "ProjectManager_Library  v0.1Win, Build " << __EMProjectManager_LibraryVersion << " by " << __EMProjectManager_LibraryVersion_Owner << " (C) Elegant Media AB 2000-2001" << ef;

	return m_opInstance;
}

EMCommand* EMProjectCommandFactory::CreateCommand(uint32 p_vType)
{
	switch(p_vType)
	{
	case PROJECT_COMMAND_CREATE_PROJECT:
		return EM_new ProjectCommandCreateProject();
	case PROJECT_COMMAND_LOAD_TEMPLATE:
		return EM_new ProjectCommandLoadTemplate();
	case PROJECT_COMMAND_LOAD_PROJECT:
		return EM_new ProjectCommandLoadProject();
	case PROJECT_COMMAND_SAVE_PROJECT:
		return EM_new ProjectCommandSaveProject();
	case PROJECT_COMMAND_CLOSE_PROJECT:
		return EM_new ProjectCommandCloseProject();
	case PROJECT_COMMAND_GET_DIRECTORY:
		return EM_new ProjectCommandGetDirectory();
	case PROJECT_COMMAND_ADD_PROJECT_DATA_CONTAINER:
		return EM_new ProjectCommandAddProjectDataContainer();
	case PROJECT_COMMAND_READY_FOR_USE:
		return EM_new ProjectCommandReadyForUse();
	case PROJECT_COMMAND_SAVE_NEEDED:
		return EM_new ProjectCommandSaveNeeded();
	case PROJECT_COMMAND_LOAD_SYSTEM_SETTINGS:
		return EM_new ProjectCommandLoadSystemSettings();
	case PROJECT_COMMAND_SAVE_SYSTEM_SETTINGS:
		return EM_new ProjectCommandSaveSystemSettings();
	}
;//cout_commented_out_4_release << "p_vType: " << p_vType << endl;
	EMDebugger("Project command not found");
	return NULL;
}


