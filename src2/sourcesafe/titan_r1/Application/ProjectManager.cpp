#include "ProjectManager.h"

#include "DirectoryIDs.h"

#ifndef PLATFORM_WINDOWS
const char* DIRECTORY_AUDIO_DATA = "/audio_data";
const char* DIRECTORY_AUDIO_DATA_UNDO = "/audio_data/undo_stack";
const char* DIRECTORY_AUDIO_DATA_USED = "/audio_data/used";
const char* DIRECTORY_AUDIO_DATA_UNUSED = "/audio_data/unused";
const char* DIRECTORY_AUDIO_DATA_PEAKS = "/audio_data/peaks";
const char* DIRECTORY_PROJECT_SETTINGS = "/settings";
const char* DIRECTORY_PROJECT_DATA = "";
const char* DIRECTORY_SYSTEM = "/system";
const char* DIRECTORY_RENDER_TARGET = "/render";
const char* DIRECTORY_VIDEO_DATA = "/video_data";
const char* DIRECTORY_TEMPLATES = "/templates";
const char* DIRECTORY_DOWNLOAD = "/download";
#endif
#ifdef PLATFORM_WINDOWS
const char* DIRECTORY_AUDIO_DATA = "\\audio_data";
const char* DIRECTORY_AUDIO_DATA_UNDO = "\\audio_data\\undo_stack";
const char* DIRECTORY_AUDIO_DATA_USED = "\\audio_data\\used";
const char* DIRECTORY_AUDIO_DATA_UNUSED = "\\audio_data\\unused";
const char* DIRECTORY_AUDIO_DATA_PEAKS = "\\audio_data\\peaks";
const char* DIRECTORY_PROJECT_SETTINGS = "\\settings";
const char* DIRECTORY_PROJECT_DATA = "";
const char* DIRECTORY_SYSTEM = "\\system";
const char* DIRECTORY_RENDER_TARGET = "\\render";
const char* DIRECTORY_VIDEO_DATA = "\\video_data";
const char* DIRECTORY_TEMPLATES = "\\templates";
const char* DIRECTORY_DOWNLOAD = "\\download";
#endif

ProjectManager* ProjectManager::m_opInstance = NULL;

extern "C" __declspec(dllexport) void DeleteProjectManager() { delete ProjectManager::Instance(); };

void ProjectManager::DeleteInstance()
{
	delete m_opInstance;
	m_opInstance = NULL;
}

bool ProjectManager::CreateDirectoryStructure(string p_oPath)
{
	CreateDirectory(p_oPath);
	CreateDirectory(p_oPath + DIRECTORY_AUDIO_DATA);
	CreateDirectory(p_oPath + DIRECTORY_AUDIO_DATA_USED);
	CreateDirectory(p_oPath + DIRECTORY_AUDIO_DATA_PEAKS);
	CreateDirectory(p_oPath + DIRECTORY_RENDER_TARGET);
	CreateDirectory(p_oPath + DIRECTORY_VIDEO_DATA);
	CreateDirectory(p_oPath + DIRECTORY_DOWNLOAD);
	return true;
}

string ProjectManager::GetDirectory(uint32 p_vProjectID, uint32 p_vDirectoryID)
{
	switch(p_vDirectoryID)
	{
	case DIRECTORY_ID_APPLICATION:
		return GetApplicationDirectory();
	case DIRECTORY_ID_AUDIO_DATA_UNDO:
		return m_oProjects[p_vProjectID] + DIRECTORY_AUDIO_DATA_UNDO;
	case DIRECTORY_ID_AUDIO_DATA_USED:
		return m_oProjects[p_vProjectID] + DIRECTORY_AUDIO_DATA_USED;
	case DIRECTORY_ID_AUDIO_DATA_UNUSED:
		return m_oProjects[p_vProjectID] + DIRECTORY_AUDIO_DATA_UNUSED;
	case DIRECTORY_ID_PROJECT_DATA:
		return m_oProjects[p_vProjectID] + DIRECTORY_PROJECT_DATA;
	case DIRECTORY_ID_SYSTEM:
		return GetApplicationDirectory() + DIRECTORY_SYSTEM;
	case DIRECTORY_ID_PROJECT_SETTINGS:
		return m_oProjects[p_vProjectID] + DIRECTORY_PROJECT_SETTINGS;
	case DIRECTORY_ID_AUDIO_DATA_PEAKS:
		return m_oProjects[p_vProjectID] + DIRECTORY_AUDIO_DATA_PEAKS;
	case DIRECTORY_ID_RENDER_TARGET:
		return m_oProjects[p_vProjectID] + DIRECTORY_RENDER_TARGET;
	case DIRECTORY_ID_VIDEO_DATA:
		return m_oProjects[p_vProjectID] + DIRECTORY_VIDEO_DATA;
	case DIRECTORY_ID_HOME:
		return GetHomeDirectory();
	case DIRECTORY_ID_TEMPLATES:
		return GetApplicationDirectory() + DIRECTORY_TEMPLATES;
	case DIRECTORY_ID_DOWNLOAD:
		return m_oProjects[p_vProjectID] + DIRECTORY_DOWNLOAD;
	case DIRECTORY_ID_PROJECT:
		return m_oProjects[p_vProjectID];
	}
	
	EMDebugger("Unknown directory ID");
	return NULL;
}

ProjectManager* ProjectManager::Instance()
{
	if(m_opInstance == NULL)
		m_opInstance = new ProjectManager();
	return m_opInstance;
}
/*
void ProjectManager::LoadProject(int p_vProjectID, string p_oPath)
{
	m_oProjectDirectories[p_vProjectID] = string(p_oPath.c_str());
	EMProjectManager::LoadProject(p_vProjectID, p_oPath);
}

void ProjectManager::SaveProject(int p_vProjectID, string p_oPath)
{
	m_oProjectDirectories[p_vProjectID] = string(p_oPath.c_str());
	EMProjectManager::SaveProject(p_vProjectID, p_oPath.c_str());
}
*/

