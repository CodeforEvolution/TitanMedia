#include "EMProjectManager.h"

#include "DirectoryIDs.h" // FIXME: Application file inside framework -> bad
#include "EMProjectDataContainer.h"
#include "EMProjectDataLoader.h"
#include "EMProjectDataSaver.h"

#ifdef PLATFORM_BEOS
#include <Application.h>
#include <Directory.h>
#include <Entry.h>
#include <Path.h>
#include <Roster.h>
#endif
#ifdef PLATFORM_WINDOWS
#include <windows.h>
#undef CreateDirectory
#endif

const char* PROJECT_DATA_FILE_NAME = "project_data.tpf";
const char* TEMPLATE_DATA_FILE_EXTENSION = ".tte";

EMProjectManager::EMProjectManager()
{
#ifdef PLATFORM_BEOS
// Should it be this hard to find the application directory??
	thread_info oThreadInfo;
	get_thread_info(find_thread(NULL), &oThreadInfo);
	app_info oAppInfo;
	be_roster -> GetRunningAppInfo(oThreadInfo.team, &oAppInfo);
	BPath oPath(&(oAppInfo.ref));
	string oPathAppStr(oPath.Path());
	string::size_type vPos = oPathAppStr.rfind('/');
	string oPathStr;
	if(vPos != string::npos)
		oPathStr = oPathAppStr.substr(0, vPos);
	else
		oPathStr = oPathAppStr;
	m_oDirApplication = oPathStr;
#endif
#ifdef PLATFORM_WINDOWS
#ifdef _DEBUG
	char vString[255];
	GetCurrentDirectory(255, vString);
	m_oDirApplication = string(vString);
#endif // _DEBUG
#ifndef _DEBUG
	// Get the application directory from windows registry
	//char vString[255];
	//vString[0] = '\0';
	m_oDirApplication = "";

	HRESULT vResult;
	HKEY vRegistryKey;

	vResult = RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("software\\Elegant Media\\Titan"), 0, KEY_READ, &vRegistryKey);
	if(vResult == ERROR_SUCCESS)
	{
		char vpValueName[MAX_PATH];
		uint32 vValueNameSize = MAX_PATH;
		
		uchar vpData[MAX_PATH];
		char vpTempData[MAX_PATH];
		uint32 vDataSize = MAX_PATH;
		
		uint32  vIndex(0);
		uint32 vType(0);

		while(true)
		{
			vResult = RegEnumValue(vRegistryKey, vIndex, vpValueName, &vValueNameSize, NULL, &vType, vpData, &vDataSize);
			if(vResult == ERROR_SUCCESS)
			{
				switch(vType)
				{
				case REG_SZ:
					if(strcmpi(vpValueName, "ApplicationPath") == 0)
					{
						memcpy(vpTempData, (char*) vpData, vDataSize); // vpData is the ApplicationPath
						m_oDirApplication = vpTempData; 
					}
					break;
				default:
					break;
				}
			}
			else if(vResult == ERROR_NO_MORE_ITEMS)
				break;

			vValueNameSize = MAX_PATH;
			vDataSize = MAX_PATH;
			vIndex++;
		}
		RegCloseKey(vRegistryKey);
	}
	if(m_oDirApplication == "")
	{
		char vString[255];
		GetCurrentDirectory(255, vString);
		m_oDirApplication = vString;
	}
//	m_oDirApplication = vString;
	m_oDirHome = "C:\\"; // FIXME
#endif // _DEBUG
#endif // PLATFORM_WINDOWS
}

EMProjectManager::~EMProjectManager()
{
	if(m_oProjects.size() > 0)
		EMDebugger("Tried to delete EMProjectManager without first closing all projects");
}

void EMProjectManager::AddProjectDataContainer(EMProjectDataContainer* p_opContainer)
{
	m_oDataContainers.push_back(p_opContainer);
	m_oDataContainers.unique();
}

void EMProjectManager::CloseProject(uint32 p_vProjectID)
{
	if(m_oProjects.find(p_vProjectID) == m_oProjects.end())
	{
		EMDebugger("Tried to close project that doesn't exist");
		return;
	}
	
	list<EMProjectDataContainer*>::iterator oIterator = m_oDataContainers.begin();
	while(oIterator != m_oDataContainers.end())
	{
		(*oIterator) -> PrepareToClearData();
		oIterator++;
	}

	oIterator = m_oDataContainers.begin();
	while(oIterator != m_oDataContainers.end())
	{
		(*oIterator) -> ClearData(); // Discard the project data
		oIterator++;
	}
	
	oIterator = m_oDataContainers.begin();
	while(oIterator != m_oDataContainers.end())
	{
		(*oIterator) -> ProjectClosed();
		oIterator++;
	}

	m_oProjects.erase(p_vProjectID);
}

bool EMProjectManager::CreateDirectory(string p_oPath)
{
eo << "Creating directory: " << p_oPath.c_str() << ef;
#ifdef PLATFORM_BEOS
	create_directory(p_oPath.c_str(), 0777);
#endif
#ifdef PLATFORM_WINDOWS
#ifdef UNICODE
	::CreateDirectoryW(p_oPath.c_str(), NULL);
#else
	::CreateDirectoryA(p_oPath.c_str(), NULL);
#endif // !UNICODE
#endif
	return true;
}

int EMProjectManager::CreateProject(string p_oPath, bool p_vCreateDirectories)
{
	string oPath = TranslatePath(p_oPath);

	// Set up the directory structure
	if(p_vCreateDirectories)
		CreateDirectoryStructure(oPath);

	int vNewID = 1; // No project should have 0 as ID (makes debugging easier)
	while(m_oProjects.find(vNewID) != m_oProjects.end())
		vNewID++;

	// Currently only one concurrent project is supported
	if(vNewID != 1)
		EMDebugger("Seems that a project hasn't been closed");

	list<EMProjectDataContainer*>::iterator oIterator = m_oDataContainers.begin();
	while(oIterator != m_oDataContainers.end())
	{
		(*oIterator) -> ProjectCreated(vNewID);
		oIterator++;
	}
	
	m_oProjects[vNewID] = oPath.c_str();
	
	// The project is saved immediately after creation
	//SaveProject(vNewID, oPath.c_str()); // Don't save here. Save in the command so the paths gets right
	return vNewID;
}
/*
// Creates a new project from a template project
// p_oTemplate is the name of the template (not the whole path)
int EMProjectManager::CreateProject(string p_oPath, string p_oTemplate)
{
	string oPath = TranslatePath(p_oPath);
	// Set up the directory structure	
	CreateDirectoryStructure(oPath);
	int vNewID = 1; // No project should have 0 as ID (makes debugging easier)
	while(m_oProjects.find(vNewID) != m_oProjects.end())
		vNewID++;
	list<EMProjectDataContainer*>::iterator oIterator = m_oDataContainers.begin();
	while(oIterator != m_oDataContainers.end())
	{
		(*oIterator) -> ProjectCreated(vNewID);
		oIterator++;
	}
	
	m_oProjects[vNewID] = oPath.c_str();

	// Convert the template to a path
	string otemplatePath = GetDirectory(vNewID, DIRECTORY_ID_TEMPLATES);
	LoadProject(vNewID, otemplatePath, p_oTemplate + TEMPLATE_DATA_FILE_EXTENSION);
	
	// The project is saved immediately after creation
	//SaveProject(vNewID, oPath.c_str()); // Don't save here. Save in the command so the paths gets right
	return vNewID;
}
*/
string EMProjectManager::GetApplicationDirectory()
{
	return m_oDirApplication;
}

string EMProjectManager::GetHomeDirectory()
{
	return m_oDirHome;
}

int EMProjectManager::GetNrProjects()
{
	return m_oProjects.size();
}

const char* EMProjectManager::GetProjectDirectory(uint32 p_vProjectID)
{
	return m_oProjects[p_vProjectID].c_str();
}

// It is assumed that the parameter is either the directory path
// or the directory path + the project filename
// Do not forget to call ReadyForUse after this call
uint32 EMProjectManager::LoadProject(const char* p_vpProjectFilePath)
{
	string oPath = TranslatePath(string(p_vpProjectFilePath));

	if(oPath.find(PROJECT_DATA_FILE_NAME) != string::npos)
		oPath = oPath.substr(0, oPath.size() - (string(PROJECT_DATA_FILE_NAME).size() + 1));

	uint32 vID = CreateProject(oPath, false);

	EMProjectDataLoader oLoader;

	if(!oLoader.InitializeLoad(oPath + EM_FILE_SEPARATOR + PROJECT_DATA_FILE_NAME))
	{
		//EMDebugger("Could not initialize project load");
		return 0;
	}

	list<EMProjectDataContainer*>::iterator oIterator = m_oDataContainers.begin();
	while(oIterator != m_oDataContainers.end())
	{
		(*oIterator) -> PrepareToLoadData();
		oIterator++;
	}

	int vContainerCounter(0);
	oIterator = m_oDataContainers.begin();
	while(oIterator != m_oDataContainers.end())
	{
eo << "Now loading from container with index " << vContainerCounter << ef;
		oLoader.PrepareForNewDataContainer();

		(*oIterator) -> LoadData(&oLoader);
		oLoader.DataContainerIsFinished();
		vContainerCounter++;
		oIterator++;
	}
	
	oLoader.CloseFile();

	oIterator = m_oDataContainers.begin();
	while(oIterator != m_oDataContainers.end())
	{
		(*oIterator) -> LoadComplete();
		oIterator++;
	}

	return vID;
}

// p_oTemplateName must have no extension (it is added here instead)
uint32 EMProjectManager::LoadTemplate(const char* p_vpProjectDirectoryPath, const char* p_vpTemplateName)
{
	string oPath = TranslatePath(string(p_vpProjectDirectoryPath));
	string oTemplate;
	if(p_vpTemplateName != NULL)
	{
		oTemplate = p_vpTemplateName;
		oTemplate += TEMPLATE_DATA_FILE_EXTENSION;
	}

	if(oPath.find(PROJECT_DATA_FILE_NAME) != string::npos)
		oPath = oPath.substr(0, oPath.size() - string(PROJECT_DATA_FILE_NAME).size());

	uint32 vID = CreateProject(oPath, true);

	if(p_vpTemplateName != NULL)
	{
		EMProjectDataLoader oLoader;

		if(!oLoader.InitializeLoad(GetDirectory(vID, DIRECTORY_ID_TEMPLATES) + EM_FILE_SEPARATOR + oTemplate))
		{
			EMDebugger("Could not initialize template load");
			return 0;
		}

int vDataContainerIndex;

vDataContainerIndex = 0;
		list<EMProjectDataContainer*>::iterator oIterator = m_oDataContainers.begin();
		while(oIterator != m_oDataContainers.end())
		{
eo << "PrepareToLoadData on data container with index " << vDataContainerIndex << ef;
			(*oIterator) -> PrepareToLoadData();
vDataContainerIndex++;
			oIterator++;
		}

vDataContainerIndex = 0;
		oIterator = m_oDataContainers.begin();
		while(oIterator != m_oDataContainers.end())
		{
eo << "Preparing for new data container" << ef;
			oLoader.PrepareForNewDataContainer();
eo << "LoadData on data container with index " << vDataContainerIndex << ef;
			(*oIterator) -> LoadData(&oLoader);
			oLoader.DataContainerIsFinished();
vDataContainerIndex++;
			oIterator++;
		}
eo << "Closing project file" << ef;					
		oLoader.CloseFile();

vDataContainerIndex = 0;
		oIterator = m_oDataContainers.begin();
		while(oIterator != m_oDataContainers.end())
		{
eo << "LoadComplete on data container with index " << vDataContainerIndex << ef;
			(*oIterator) -> LoadComplete();
vDataContainerIndex++;
			oIterator++;
		}
	}
eo << "Saving template project to project file" << ef;
	// Now save the template data to the project location
	SaveProject(vID);

	return vID;
}

/*
uint32 EMProjectManager::LoadTemplate(const char* p_vpProjectDirectoryPath, const char* p_oTemplateName)
{
	string oPath = TranslatePath(string(p_vpPath));

	EMProjectDataLoader oLoader;
	
	if(strcmp(oTemplateCheck.c_str(), TEMPLATE_DATA_FILE_EXTENSION) == 0)
	{
//eo << "EMProjectManager::LoadProject: " << (oPath + EM_FILE_SEPARATOR + p_oFileName).c_str() << ef;
		if(!oLoader.InitializeLoad(oPath + EM_FILE_SEPARATOR + p_oFileName))
			EMDebugger("Could not initialize template load");
	}
	else
	{
		m_oProjects[p_vProjectID] = oPath.c_str();
eo << "EMProjectManager::LoadProject: " <<  (string(GetProjectDirectory(p_vProjectID)) + EM_FILE_SEPARATOR + p_oFileName).c_str() << ef;
		if(!oLoader.InitializeLoad(string(GetProjectDirectory(p_vProjectID)) + EM_FILE_SEPARATOR + p_oFileName))
			EMDebugger("Could not initialize project load");
	}

	list<EMProjectDataContainer*>::iterator oIterator = m_oDataContainers.begin();
	while(oIterator != m_oDataContainers.end())
	{
		(*oIterator) -> PrepareToClearData();
		oIterator++;
	}

	// Make sure that no old project data resides in memory before loading the project, if it is not loaded from a template
	if(strcmp(oTemplateCheck.c_str(), TEMPLATE_DATA_FILE_EXTENSION) != 0)
	{
		oIterator = m_oDataContainers.begin();
		while(oIterator != m_oDataContainers.end())
		{
			(*oIterator) -> ClearData(); // Discard the project data
			oIterator++;
		}
	}
	
	// The loaded data needs a new project to reside in, create a unique ID and inform all data containers
	int vNewID = 1; // No project should have 0 as ID (makes debugging easier)
	while(m_oProjects.find(vNewID) != m_oProjects.end())
		vNewID++;
	oIterator = m_oDataContainers.begin();
	while(oIterator != m_oDataContainers.end())
	{
		(*oIterator) -> ProjectCreated(vNewID);
		oIterator++;
	}

	oIterator = m_oDataContainers.begin();
	while(oIterator != m_oDataContainers.end())
	{
		(*oIterator) -> PrepareToLoadData();
		oIterator++;
	}

//	list<EMProjectDataContainer*> oDataContainers = m_oProjects[p_vProjectID];
	oIterator = m_oDataContainers.begin();
	while(oIterator != m_oDataContainers.end())
	{
		oLoader.PrepareForNewDataContainer();
		(*oIterator) -> LoadData(&oLoader);
		oLoader.DataContainerIsFinished();
		oIterator++;
	}
	
	oLoader.CloseFile();

	oIterator = m_oDataContainers.begin();
	while(oIterator != m_oDataContainers.end())
	{
		(*oIterator) -> LoadComplete();
		oIterator++;
	}

}
*/

void EMProjectManager::MoveProject(uint32 p_vProjectID, const char* p_vpNewProjectDirectoryPath)
{
	string oOldPath = TranslatePath(m_oProjects[p_vProjectID]);
#ifdef PLATFORM_WINDOWS
	MoveFile(oOldPath.c_str(), p_vpNewProjectDirectoryPath);
#endif
	m_oProjects[p_vProjectID] = p_vpNewProjectDirectoryPath;
}

void EMProjectManager::ReadyForUse()
{
	list<EMProjectDataContainer*>::iterator oIterator = m_oDataContainers.begin();
int vCounter(0);
	while(oIterator != m_oDataContainers.end())
	{
eo << "Now calling ReadyForUse() in data container with index " << vCounter << ef;
		(*oIterator) -> ReadyForUse();
		oIterator++;
vCounter++;
	}
}

bool EMProjectManager::SaveNeeded()
{
	list<EMProjectDataContainer*>::iterator oIterator = m_oDataContainers.begin();
	while(oIterator != m_oDataContainers.end())
	{
		if((*oIterator) -> IsDirty())
			return true;
		oIterator++;
	}

	return false;
}

void EMProjectManager::SaveProject(uint32 p_vProjectID)
{
	if(!SaveNeeded())
		return;
	
	string oPath = TranslatePath(m_oProjects[p_vProjectID]);

	EMProjectDataSaver oSaver;

	if(!oSaver.InitializeSave(oPath + EM_FILE_SEPARATOR + string(PROJECT_DATA_FILE_NAME)))
		EMDebugger("Could not initialize project save");

int vDataContainerIndex;

vDataContainerIndex = 0;
	list<EMProjectDataContainer*>::iterator oIterator = m_oDataContainers.begin();
	while(oIterator != m_oDataContainers.end())
	{
eo << "PrepareToSaveData on data container with index " << vDataContainerIndex << ef;
		(*oIterator) -> PrepareToSaveData();
vDataContainerIndex++;
		oIterator++;
	}

vDataContainerIndex = 0;
	oIterator = m_oDataContainers.begin();
	while(oIterator != m_oDataContainers.end())
	{
eo << "Preparing for new data container" << ef;
		oSaver.PrepareForNewDataContainer();
eo << "SaveData on data container with index " << vDataContainerIndex << ef;
		(*oIterator) -> SaveData(&oSaver);
		oSaver.DataContainerIsFinished();
vDataContainerIndex++;
		oIterator++;
	}

	oSaver.CloseFile();

vDataContainerIndex = 0;
	oIterator = m_oDataContainers.begin();
	while(oIterator != m_oDataContainers.end())
	{
eo << "SaveComplete on data container with index " << vDataContainerIndex << ef;
		(*oIterator) -> SaveComplete();
vDataContainerIndex++;
		oIterator++;
	}
}

// Translates a directory path to native format
string EMProjectManager::TranslatePath(string p_oPath)
{
#ifdef PLATFORM_WINDOWS
	while(p_oPath.find("/") != string::npos)//0xFFFFFFFF) // FIXME: Should actually be std::npos
	{
eo << "replace: " << p_oPath.find("/") << ef;
		p_oPath.replace(p_oPath.find("/"), 1, "\\");
	}
#endif
#ifdef PLATFORM_BEOS
	while(p_oPath.find("\\") != std::npos)
		p_oPath.replace(p_oPath.find("/"), 1, "/");
#endif
	return p_oPath;
}

