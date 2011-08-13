/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* Contains some native code
*******************************************************/

#include "EMGlobals.h"

#ifndef __PROJECT_MANAGER
#define __PROJECT_MANAGER

#include "EMProjectManager.h"

#include <map>
#include <string>

class ProjectManager : public EMProjectManager
{
public:
	static void DeleteInstance();
	string GetDirectory(uint32 p_vProjectID, uint32 p_vDirectoryID);
	static ProjectManager* Instance();
//	void LoadProject(int p_vProjectID, string p_oPath);
//	void SaveProject(int p_vProjectID, string p_oPath);

protected:
	bool CreateDirectoryStructure(string p_oPath);

private:
	static ProjectManager* m_opInstance;
//	map<int, string> m_oProjectDirectories;
};

#endif
