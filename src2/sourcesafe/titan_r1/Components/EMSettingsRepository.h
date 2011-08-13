/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
*
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_SETTINGS_REPOSITORY
#define __EM_SETTINGS_REPOSITORY

#include "EMListenerRepository.h"
#include "EMProjectDataContainer.h"

#include <map>

class __declspec(dllexport) EMDefaultSettingsContainer;

enum EMSettingType
{
	EM_SETTING_INT,
	EM_SETTING_INT16,
	EM_SETTING_UINT16,
	EM_SETTING_INT32,
	EM_SETTING_UINT32,
	EM_SETTING_INT64,
	EM_SETTING_UINT64,
	EM_SETTING_FLOAT,
	EM_SETTING_STRING,
	EM_SETTING_BOOL,
	EM_SETTING_VOIDP
};

class __declspec(dllexport) EMSetting
{
public:
	EMSettingType m_eType;
	void* m_upSetting;
	bool m_vIsSystemSetting;
};

class __declspec(dllexport) EMSettingsRepository : public EMListenerRepository, public EMProjectDataContainer
{
public:
	virtual ~EMSettingsRepository();
	bool ClearData();
	void* GetSetting(int32 p_vSettingID);
	static EMSettingsRepository* Instance();
	bool IsDirty();
	void LoadComplete();
	bool LoadData(void* p_upDataLoader);
	bool LoadSystemSettings();
	void ProjectClosed();
	void ProjectCreated(int p_vProjectID);
	void ReadyForUse();
	void SaveComplete();
	bool SaveData(void* p_upDataSaver);
	bool SaveSystemSettings();
	void SetDefaultSettingsContainer(EMDefaultSettingsContainer* p_opContainer);
	void SetSetting(int32 p_vSettingID, EMSettingType p_eType, void* p_upSetting, bool p_vNotify = true, bool p_vIsSystemSetting = false);
	bool SettingExists(int32 p_vSettingID);
	
protected:
	EMSettingsRepository();

private:
	void DeleteSetting(int32 p_vSettingID);

private:
	map<int32, EMSetting> m_oSettings;
	EMDefaultSettingsContainer* m_opDefaultSettingsContainer;
	static EMSettingsRepository* m_opInstance;
	bool m_vIsDirty;
	bool m_vSavingSystemSettings;
};

#endif