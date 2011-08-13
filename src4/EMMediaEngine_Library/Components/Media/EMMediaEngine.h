/*******************************************************
* Creator: Richard Kronfalt
* Portability: Non-Native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_MEDIA_ENGINE
#define __EM_MEDIA_ENGINE

#include "EMProjectDataContainer.h"

#include <string>

class EMMediaCommandFactory; 
class EMMediaTimer;
class EMSettingsRepository;
class EMVideoPreview;
class EMCommandRepository;
class EMMediaProject;
//class EMMediaPool;
class EMInputRepository;
class EMOutputRepository;
class EMEncoderRepository;
class EMSemaphore;

class __declspec(dllexport) EMMediaEngine : public EMProjectDataContainer
{
public:
	~EMMediaEngine();
	int32 GetID() const;
	static EMMediaEngine* Instance();
	static void Delete();
	static bool m_vIsDecomposed;
	bool MediaSystemCheck(char* p_vpErrorMessage);
	EMMediaProject* GetMediaProject() const;
	EMMediaTimer* GetMediaTimer() const;
	EMMediaCommandFactory* GetCommandFactory() const;
	void SetVideoPreviewWindow(EMVideoPreview* p_opVideoPreviewWindow);
	EMVideoPreview* GetVideoPreviewWindow() const;
	void SetSettingsRepository(EMSettingsRepository* p_opSettingsRepository);
	EMSettingsRepository* GetSettingsRepository() const;
	void SetCommandRepository(EMCommandRepository* p_opCommandRepository);
	EMCommandRepository* GetCommandRepository() const;
	void InstanceOther();
	
	void GetVersion(int32* p_vpMajor, int32* p_vpMinor);

	bool IsDirty();

	void SaveComplete();
	bool SaveData(void* p_upSaver);
	bool LoadData(void* p_upLoader);
	void ProjectClosed();
	void ProjectCreated(int p_vProjectID);
	void PrepareToLoadData();
	bool ClearData();
	void ReadyForUse();
	
	void LockProject();
	void UnlockProject();

	void LockVideo();
	void UnlockVideo();
	void LockAudio();
	void UnlockAudio();
	
	
protected:
	EMMediaEngine();
	
private:
	static EMMediaEngine* m_opInstance;

	EMMediaProject* m_opProject;
	EMSettingsRepository* m_opSettingsRepository;
	EMVideoPreview* m_opVideoPreviewWindow;
	EMCommandRepository* m_opCommandRepository;

	EMSemaphore* m_opProjectProtectionSemaphore;


};

#endif
