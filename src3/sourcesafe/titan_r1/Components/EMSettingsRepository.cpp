#include "EMSettingsRepository.h"

#include "CommandIDs.h" // FIXME: An application class inside the framework
#include "EMCommandRepository.h"
#include "EMDefaultSettingsContainer.h"
#include "EMMessages.h"
#include "EMProjectDataLoader.h"
#include "EMProjectDataSaver.h"
#include "EMSettingIDs.h"

#include <string>

#include <crtdbg.h> // Debug stuff

EMSettingsRepository* EMSettingsRepository::m_opInstance = NULL;

EMSettingsRepository::EMSettingsRepository() :
m_opDefaultSettingsContainer(NULL),
m_vIsDirty(false),
m_vSavingSystemSettings(false)
{
}

EMSettingsRepository::~EMSettingsRepository()
{
	map<int32, EMSetting> oDeletions(m_oSettings);
	map<int32, EMSetting>::iterator oIterator = oDeletions.begin();
	while(oIterator != oDeletions.end())
	{
		DeleteSetting(oIterator -> first);
		oIterator++;
	}
}

bool EMSettingsRepository::ClearData()
{
	map<int32, EMSetting> oClearList = m_oSettings;
	map<int32, EMSetting>::iterator oIterator = oClearList.begin();
	while(oIterator != oClearList.end())
	{
		// Only project settings are cleared, the others will live through all projects
		if(!((oIterator -> second).m_vIsSystemSetting))
			DeleteSetting(oIterator -> first);
		oIterator++;
	}
	// Set default settings
	EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_SET_DEFAULT_SETTINGS);
	// If available, add additional default settings
	if(m_opDefaultSettingsContainer != NULL)
		m_opDefaultSettingsContainer -> SetDefaultSettings();

	m_vIsDirty = true;

	return true;
}

// Make sure that the setting exists before calling this method
void EMSettingsRepository::DeleteSetting(int32 p_vSettingID)
{
if(p_vSettingID == 2012)
eo << "gronk";
	EMSetting oSetting = m_oSettings[p_vSettingID];

	if(oSetting.m_upSetting == NULL)
		EMDebugger("Strange, the setting exists, but the data pointer is NULL");
	
	m_oSettings.erase(p_vSettingID);

	switch(oSetting.m_eType)
	{
	case EM_SETTING_INT:
		delete static_cast<int*>(oSetting.m_upSetting);
		break;
	case EM_SETTING_INT16:
		delete static_cast<int16*>(oSetting.m_upSetting);
		break;
	case EM_SETTING_UINT16:
		delete static_cast<uint16*>(oSetting.m_upSetting);
		break;
	case EM_SETTING_INT32:
		delete static_cast<int32*>(oSetting.m_upSetting);
		break;
	case EM_SETTING_UINT32:
		delete static_cast<uint32*>(oSetting.m_upSetting);
		break;
	case EM_SETTING_INT64:
		delete static_cast<int64*>(oSetting.m_upSetting);
		break;
	case EM_SETTING_UINT64:
		delete static_cast<uint64*>(oSetting.m_upSetting);
		break;
	case EM_SETTING_FLOAT:
		delete static_cast<float*>(oSetting.m_upSetting);
		break;
	case EM_SETTING_STRING:
		delete static_cast<string*>(oSetting.m_upSetting);
		break;
	case EM_SETTING_BOOL:
		delete static_cast<bool*>(oSetting.m_upSetting);
		break;
	// Someone else is responsible for deleting void* settings
	case EM_SETTING_VOIDP: 
		break;
	}
//	oSetting.m_upSetting = NULL;
	m_oSettings.erase(p_vSettingID);

	m_vIsDirty = true;
}

void* EMSettingsRepository::GetSetting(int32 p_vSettingID)
{
	if(!SettingExists(p_vSettingID))
	{
		EMDebugger("This setting does not exist");
		return NULL;
	}
	return m_oSettings[p_vSettingID].m_upSetting;
}

EMSettingsRepository* EMSettingsRepository::Instance()
{
	if(m_opInstance == NULL)
		m_opInstance = EM_new EMSettingsRepository();
	return m_opInstance;
}

bool EMSettingsRepository::IsDirty()
{
	return m_vIsDirty;
}

void EMSettingsRepository::LoadComplete()
{
//;//cout_commented_out_4_release << "EMSettingsRepository::LoadComplete" << endl;
}

bool EMSettingsRepository::LoadData(void* p_upDataLoader)
{
//;//cout_commented_out_4_release << "EMSettingsRepository::LoadData" << endl;
	EMSettingType eType;
	uint32 vSettingID;
	EMProjectDataLoader* opLoader = static_cast<EMProjectDataLoader*>(p_upDataLoader);
	// Load the number of saved settings
	uint32 vNrSettings = opLoader -> LoadUInt32();
//;//cout_commented_out_4_release << "Will load " << vNrSettings << " settings" << endl;

	int vInt;
	int16 vInt16;
	uint16 vUInt16;
	int32 vInt32;
	uint32 vUInt32;
	int64 vInt64;
	uint64 vUInt64;
	float vFloat;
	string oString;
	bool vBool;
	bool vIsSystemSetting;

	for(uint32 vIndex = 0; vIndex < vNrSettings; vIndex++)
	{
		// First load the type of setting (EMSettingType)
		eType = static_cast<EMSettingType>(opLoader -> LoadInt16());
		// Now load the ID of the setting
		vSettingID = opLoader -> LoadUInt32();
		switch(eType)
		{
		case EM_SETTING_INT:
			if(vSettingID >= FRAMEWORK_SYSTEM_SETTING_ID_OFFSET)
				vIsSystemSetting = true;
			else
				vIsSystemSetting = false;
			vInt = opLoader -> LoadInt();
			SetSetting(vSettingID, eType, &vInt, false, vIsSystemSetting);
			break;
		case EM_SETTING_INT16:
			if(vSettingID >= FRAMEWORK_SYSTEM_SETTING_ID_OFFSET)
				vIsSystemSetting = true;
			else
				vIsSystemSetting = false;
			vInt16 = opLoader -> LoadInt16();
			SetSetting(vSettingID, eType, &vInt16, false, vIsSystemSetting);
			break;
		case EM_SETTING_UINT16:
			if(vSettingID >= FRAMEWORK_SYSTEM_SETTING_ID_OFFSET)
				vIsSystemSetting = true;
			else
				vIsSystemSetting = false;
			vUInt16 = opLoader -> LoadUInt16();
			SetSetting(vSettingID, eType, &vUInt16, false, vIsSystemSetting);
			break;
		case EM_SETTING_INT32:
			if(vSettingID >= FRAMEWORK_SYSTEM_SETTING_ID_OFFSET)
				vIsSystemSetting = true;
			else
				vIsSystemSetting = false;
			vInt32 = opLoader -> LoadInt32();
			SetSetting(vSettingID, eType, &vInt32, false, vIsSystemSetting);
			break;
		case EM_SETTING_UINT32:
			if(vSettingID >= FRAMEWORK_SYSTEM_SETTING_ID_OFFSET)
				vIsSystemSetting = true;
			else
				vIsSystemSetting = false;
			vUInt32 = opLoader -> LoadUInt32();
			SetSetting(vSettingID, eType, &vUInt32, false, vIsSystemSetting);
			break;
		case EM_SETTING_INT64:
			if(vSettingID >= FRAMEWORK_SYSTEM_SETTING_ID_OFFSET)
				vIsSystemSetting = true;
			else
				vIsSystemSetting = false;
			vInt64 = opLoader -> LoadInt64();
			SetSetting(vSettingID, eType, &vInt64, false, vIsSystemSetting);
			break;
		case EM_SETTING_UINT64:
			if(vSettingID >= FRAMEWORK_SYSTEM_SETTING_ID_OFFSET)
				vIsSystemSetting = true;
			else
				vIsSystemSetting = false;
			vUInt64 = opLoader -> LoadUInt64();
			SetSetting(vSettingID, eType, &vUInt64, false, vIsSystemSetting);
			break;
		case EM_SETTING_FLOAT:
			if(vSettingID >= FRAMEWORK_SYSTEM_SETTING_ID_OFFSET)
				vIsSystemSetting = true;
			else
				vIsSystemSetting = false;
			vFloat = opLoader -> LoadFloat();
			SetSetting(vSettingID, eType, &vFloat, false, vIsSystemSetting);
			break;
		case EM_SETTING_STRING:
			if(vSettingID >= FRAMEWORK_SYSTEM_SETTING_ID_OFFSET)
				vIsSystemSetting = true;
			else
				vIsSystemSetting = false;
			oString = opLoader -> LoadString();
			SetSetting(vSettingID, eType, &oString, false, vIsSystemSetting);
			break;
		case EM_SETTING_BOOL:
			if(vSettingID >= FRAMEWORK_SYSTEM_SETTING_ID_OFFSET)
				vIsSystemSetting = true;
			else
				vIsSystemSetting = false;
			vBool = opLoader -> LoadBool();
			SetSetting(vSettingID, eType, &vBool, false, vIsSystemSetting);
			break;
		}
	}
	return true;
}

bool EMSettingsRepository::LoadSystemSettings()
{
	bool vResult = reinterpret_cast<bool>(EMCommandRepository::Instance() -> ExecuteCommand(PROJECT_COMMAND_LOAD_SYSTEM_SETTINGS, dynamic_cast<EMProjectDataContainer*>(this)));
	if(!vResult)
	{
		EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_SET_DEFAULT_SYSTEM_SETTINGS);
		SaveSystemSettings();
	}
	return vResult;
}

void EMSettingsRepository::ProjectClosed()
{
//	DeleteSetting(SETTING_PROJECT_ID);
	if(SettingExists(SETTING_PROJECT_ID))
		DeleteSetting(SETTING_PROJECT_ID);
//	int vProjectID(0);
//	SetSetting(SETTING_PROJECT_ID, EM_SETTING_INT, &vProjectID, false);
}

void  EMSettingsRepository::ProjectCreated(int p_vProjectID)
{
//	int* vpProjectID = EM_new int(p_vProjectID);
	SetSetting(SETTING_PROJECT_ID, EM_SETTING_INT, &p_vProjectID/*vpProjectID*/, false);
;//cout_commented_out_4_release << "EMSettingsRepository::ProjectCreated: " << p_vProjectID << endl;
}

void EMSettingsRepository::ReadyForUse()
{
	Notify(EM_SETTING_UPDATED);
	m_vIsDirty = false;
}

void EMSettingsRepository::SaveComplete()
{
//;//cout_commented_out_4_release << "EMSettingsRepository::SaveComplete" << endl;
}

bool EMSettingsRepository::SaveData(void* p_upDataSaver)
{
	EMProjectDataSaver* opSaver = static_cast<EMProjectDataSaver*>(p_upDataSaver);
	// Calculate and save the number of settings
	map<int32, EMSetting>::iterator oIterator = m_oSettings.begin();
	uint32 vNrSettings(0);
	while(oIterator != m_oSettings.end())
	{
		if((m_vSavingSystemSettings && (*oIterator).second.m_vIsSystemSetting) || (!m_vSavingSystemSettings && !((*oIterator).second.m_vIsSystemSetting)))
			vNrSettings++;

		oIterator++;
	}
	opSaver -> SaveUInt32(vNrSettings);

	oIterator = m_oSettings.begin();
	while(oIterator != m_oSettings.end())
	{
		if((m_vSavingSystemSettings && (*oIterator).second.m_vIsSystemSetting) || (!m_vSavingSystemSettings && !((*oIterator).second.m_vIsSystemSetting)))
		{
			// First save the type of setting (EMSettingType)
			opSaver -> SaveInt16(static_cast<int16>((*oIterator).second.m_eType));
			// Now save the ID of the setting
			opSaver -> SaveUInt32(static_cast<uint32>((*oIterator).first));
			switch((*oIterator).second.m_eType)
			{
			case EM_SETTING_INT:
				opSaver -> SaveInt(*(static_cast<int*>((*oIterator).second.m_upSetting)));
				break;
			case EM_SETTING_INT16:
				opSaver -> SaveInt16(*(static_cast<int16*>((*oIterator).second.m_upSetting)));
				break;
			case EM_SETTING_UINT16:
				opSaver -> SaveUInt16(*(static_cast<uint16*>((*oIterator).second.m_upSetting)));
				break;
			case EM_SETTING_INT32:
				opSaver -> SaveInt32(*(static_cast<int32*>((*oIterator).second.m_upSetting)));
				break;
			case EM_SETTING_UINT32:
				opSaver -> SaveUInt32(*(static_cast<uint32*>((*oIterator).second.m_upSetting)));
				break;
			case EM_SETTING_INT64:
				opSaver -> SaveInt64(*(static_cast<int64*>((*oIterator).second.m_upSetting)));
				break;
			case EM_SETTING_UINT64:
				opSaver -> SaveUInt64(*(static_cast<uint64*>((*oIterator).second.m_upSetting)));
				break;
			case EM_SETTING_FLOAT:
				opSaver -> SaveFloat(*(static_cast<float*>((*oIterator).second.m_upSetting)));
				break;
			case EM_SETTING_STRING:
				opSaver -> SaveString(static_cast<string*>((*oIterator).second.m_upSetting) -> c_str());
				break;
			case EM_SETTING_BOOL:
				opSaver -> SaveBool(*(static_cast<bool*>((*oIterator).second.m_upSetting)));
				break;
			default:
				break; // type VOIDP not saved for obvious reasons
			}
		}

		oIterator++;
	}

	m_vSavingSystemSettings = false;

	return true;
}

bool EMSettingsRepository::SaveSystemSettings()
{
	m_vSavingSystemSettings = true;
	return reinterpret_cast<bool>(EMCommandRepository::Instance() -> ExecuteCommand(PROJECT_COMMAND_SAVE_SYSTEM_SETTINGS, dynamic_cast<EMProjectDataContainer*>(this)));
}

void EMSettingsRepository::SetDefaultSettingsContainer(EMDefaultSettingsContainer* p_opContainer)
{
	m_opDefaultSettingsContainer = p_opContainer;
}

// p_vNotify should be false if many settings are updated at once (except for the last of these settings)
void EMSettingsRepository::SetSetting(int32 p_vSettingID, EMSettingType p_eType, void* p_upSetting, bool p_vNotify, bool p_vIsSystemSetting)
{
if(p_vSettingID == SETTING_SOFTWARE_IS_CRIPPLED)
eo << "gronk";
/*
if(p_vNotify)
;//cout_commented_out_4_release << "EMSettingsRepository::SetSetting (will notify): " << p_vSettingID << ", nr listeners: " << GetListenerAmount() << endl;
else
;//cout_commented_out_4_release << "EMSettingsRepository::SetSetting (will not notify): " << p_vSettingID << ", nr listeners: " << GetListenerAmount() << endl;
*/
	//Since OS:es like Win2000 cannot allocate memory in one heap and delete it in another, we must make a new allocation here
	void* upSettingClone;
	switch(p_eType)
	{
	case EM_SETTING_INT:
		upSettingClone = static_cast<void*>(EM_new int(*(static_cast<int*>(p_upSetting))));
		break;
	case EM_SETTING_INT16:
		upSettingClone = static_cast<void*>(EM_new int16(*(static_cast<int16*>(p_upSetting))));
		break;
	case EM_SETTING_UINT16:
		upSettingClone = static_cast<void*>(EM_new uint16(*(static_cast<uint16*>(p_upSetting))));
		break;
	case EM_SETTING_INT32:
		upSettingClone = static_cast<void*>(EM_new int32(*(static_cast<int32*>(p_upSetting))));
		break;
	case EM_SETTING_UINT32:
		upSettingClone = static_cast<void*>(EM_new uint32(*(static_cast<uint32*>(p_upSetting))));
		break;
	case EM_SETTING_INT64:
		upSettingClone = static_cast<void*>(EM_new int64(*(static_cast<int64*>(p_upSetting))));
		break;
	case EM_SETTING_UINT64:
		upSettingClone = static_cast<void*>(EM_new uint64(*(static_cast<uint64*>(p_upSetting))));
		break;
	case EM_SETTING_FLOAT:
		upSettingClone = static_cast<void*>(EM_new float(*(static_cast<float*>(p_upSetting))));
		break;
	//	// For some reason, a string object doesn't like to be deleted in this class, so char* is used internally instead
	case EM_SETTING_STRING:
		{
//eo << "banan" << ef;
//eo << "Setting setting: " << (char*) static_cast<string*>(p_upSetting) -> c_str() << ef;
			string oTemporaryString(*(static_cast<string*>(p_upSetting)));
			upSettingClone = static_cast<void*>(EM_new string(oTemporaryString.c_str()));//*(static_cast<string*>(p_upSetting))));
//delete static_cast<string*>(upSettingClone);
/*
EMSetting oSettingX;
oSettingX.m_eType = EM_SETTING_STRING;
void* zzz = static_cast<void*>(new string(*(static_cast<string*>(p_upSetting))));
oSettingX.m_upSetting = zzz;
m_oSettings[666] = oSettingX;
DeleteSetting(666);
*/
//upSettingClone = static_cast<void*>(EM_new string(oTemporaryString));//*(static_cast<string*>(p_upSetting))));
//			upSettingClone = static_cast<void*>(EM_new char[static_cast<string*>(p_upSetting) -> size() + 1]);
//			strcpy(static_cast<char*>(upSettingClone), static_cast<string*>(p_upSetting) -> c_str());
//eo << "New string setting: " << static_cast<char*>(upSettingClone) << ef;
/*
if(!_CrtIsValidHeapPointer(upSettingClone))
eo << "Not valid heap pointer" << ef;
void* yyy = new string(*(static_cast<string*>(p_upSetting)));
delete static_cast<string*>(yyy);
void* zzz = new string(*(static_cast<string*>(p_upSetting)));
EMSetting oSettingX;
oSettingX.m_eType = EM_SETTING_STRING;
oSettingX.m_upSetting = zzz;
m_oSettings[666] = oSettingX;
EMSetting oSettingY = m_oSettings[666];
delete static_cast<string*>(oSettingY.m_upSetting);
//char* xxx = p_upSetting -> c_str();
*/
			break;
		}
	case EM_SETTING_BOOL:
		upSettingClone = static_cast<void*>(EM_new bool(*(static_cast<bool*>(p_upSetting))));
		break;
	case EM_SETTING_VOIDP:
		upSettingClone = p_upSetting;
		break;
	}

	if(SettingExists(p_vSettingID))
		DeleteSetting(p_vSettingID);
	EMSetting oSetting;
	oSetting.m_eType = p_eType;
	oSetting.m_upSetting = upSettingClone;
	oSetting.m_vIsSystemSetting = p_vIsSystemSetting;
	m_oSettings[p_vSettingID] = oSetting;
//if(p_vSettingID == SETTING_SIGNATURE)
//DeleteSetting(SETTING_SIGNATURE);
	if(p_vNotify)
		Notify(EM_SETTING_UPDATED);

	m_vIsDirty = true;
}

// If there is any doubt that a setting does not exist, call this method
// before retreiving the setting
bool EMSettingsRepository::SettingExists(int32 p_vSettingID)
{
	if(m_oSettings.find(p_vSettingID) == m_oSettings.end())
		return false;
	return true;
}

