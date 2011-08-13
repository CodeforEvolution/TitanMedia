/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* Sets up the application environment
* Owns all global repositories
*******************************************************/

#include "EMGlobals.h"

#ifndef __APPLICATION_INIT
#define __APPLICATION_INIT

//#include "FileDialogTarget.h"
#include "TrackWindow.h"
#include "SplashWindow.h"

//#include <memory>

class EMExceptionHandler;
class EMResourceRepository;
class FileDialogTarget;
class SplashWindow;

class ApplicationInit
{
public:
	ApplicationInit();
	virtual ~ApplicationInit();
	void SetUpSystem(const char* p_vpCommandLine);

private:
	void AddCommands();
	void AddListeners();
	void InitObjects();
	void InstantiateSingletons();
	bool LoadResources();
	uint32 ProcessCommandLine(const char* p_vpCommandLine, string* p_opProjectFile);
	void RegisterMediaObjects();
	void RegisterProjectDataContainers();
	void SetKeyBindings();
	bool SystemCheck(char* p_vpErrorMessage);
	bool ValidateRegistration(bool* p_vIsCrippled);
	
private:
//	auto_ptr<TrackWindow> m_opTrackWindow;
	SplashWindow* m_opSplashWindow;
	TrackWindow* m_opTrackWindow;
	EMResourceRepository* m_opResourceRepository;
	FileDialogTarget* m_opFileDialogTarget;
	char m_vDirApplication[255];
};

#endif