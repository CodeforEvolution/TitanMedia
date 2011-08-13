/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* Contains some native code
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_PROJECT_MANAGER
#define __EM_PROJECT_MANAGER

#include <list>
#include <map>
#include <string>

class EMCommandRepository;
class EMProjectDataContainer;

class EMProjectManager
{
public:
	virtual ~EMProjectManager();
	void AddProjectDataContainer(EMProjectDataContainer* p_opContainer);
	void CloseProject(uint32 p_vProjectID);
	//int CreateProject(const char* p_oPath, const char* p_oTemplate);
	int GetNrProjects();
	virtual string GetDirectory(uint32 p_vProjectID, uint32 p_vDirectoryID) = 0;
	const char* GetProjectDirectory(uint32 p_vProjectID);
	virtual uint32 LoadProject(const char* p_vpProjectFilePath);
	virtual uint32 LoadTemplate(const char* p_vpProjectDirectoryPath, const char* p_vpTemplateName);
	void ReadyForUse();
	bool SaveNeeded();
	virtual void SaveProject(uint32 p_vProjectID);
	
protected:
	EMProjectManager();
	bool CreateDirectory(string p_oPath);
	virtual bool CreateDirectoryStructure(string p_oPath) = 0;
	string GetApplicationDirectory();
	string GetHomeDirectory();

protected:
	map<uint32, string> m_oProjects;

private:
	int CreateProject(string p_oPath, bool p_vCreateDirectories);
	string TranslatePath(string p_oPath);

private:
	list<EMProjectDataContainer*> m_oDataContainers;
	string m_oDirApplication;
	string m_oDirApplicationSettings;
	string m_oDirApplicationSystem;
	string m_oDirHome;
//	map<int, list<EMProjectDataContainer*> > m_oProjects;
};

#endif