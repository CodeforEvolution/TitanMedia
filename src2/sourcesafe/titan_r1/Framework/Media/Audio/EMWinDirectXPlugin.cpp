#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#include "EMWinDirectXPlugin.h"
#include "EMMediaSystemInspector.h"
#include "EMPlugin.h"
#include "EMMediaDataBuffer.h"
#include "EMAudioMixFunctions.h"
#include "EMWinDXPluginEngine.h"
#include "EMWinMediaUtility.h"
#include "EMMediaUtility.h"
#include "EMProjectDataLoader.h"
#include "EMProjectDataSaver.h"
#include "EMMediaEngineUIDs.h"
#include "EMMediaEngine.h"
#include "EMSettingsRepository.h"
#include "EMSettingIDs.h"
#include "EMPluginRepository.h"
#include "EMPluginEntry.h"

#include <streams.h>
#include <sfiface.h>
#include <MAPIUTIL.H>
#include <fstream>

static const GUID IID_IStaticFilterPreset = { 0x36A22460, 0xcc9,  0x11D0, { 0x85, 0x74, 0x00,0xA0,0xC9, 0x05, 0x39, 0x12 } };

//Takes a filename, opens it, and copies it into the allocated memory of the new IStream.
//Returns an IStream* interface for reading/writing the contents of the file (p_oFileName)
IStream* OpenIStreamFromFile(string p_oFileName)
{
	ULARGE_INTEGER uDataLength;
    IStream* opIStream = NULL;
    HGLOBAL phglobal = NULL;
	
	//Open the file for input!
	ifstream* opInFileStream = new ifstream(p_oFileName.c_str());
	if(opInFileStream == NULL)
		return NULL;

	//Find the length of the file...
	opInFileStream -> seekg(0, ios::end);
	int64 vInFileSize = opInFileStream -> tellg();
    uDataLength.QuadPart = vInFileSize;
	opInFileStream -> seekg(0, ios::beg);
	
    //Create a new IStream..
    HRESULT vResult = CreateStreamOnHGlobal(NULL, true, &opIStream);     
	if(FAILED(vResult))
		return NULL;

    //Tell IStream the length of the data...
    vResult = opIStream -> SetSize(uDataLength);
	if(FAILED(vResult))
		return NULL;

    //Fetch a pointer to the RAM allocated by IStream (don't forget to unlock it again after we're done!)
    vResult = GetHGlobalFromStream(opIStream, &phglobal);
	if(FAILED(vResult))
	{
		opIStream -> Release();
		return NULL;
	}
    char* opBuffer = static_cast<char*>(GlobalLock(phglobal));

    //Actually read the file into RAM...
	opInFileStream -> read(opBuffer, uDataLength.QuadPart);
    GlobalUnlock(phglobal);

	//There... We have our IStream now!
	delete opInFileStream;
    return opIStream;
}

//Takes a pointer to a char* buffer that will be set to point at the stream's memory, 
//and the maximum size of the buffer that will be allocated.
//The IStream* returned is an IStream interface for writing/reading to/from the buffer (p_vppBufferPointer)
IStream* OpenIStreamFromRAM(char** p_vppBufferPointer, uint64 p_vDataSize)
{
	ULARGE_INTEGER uDataLength;
    IStream* opIStream = NULL;
    HGLOBAL phglobal = NULL;
	
	//Find the length of the data...
    uDataLength.QuadPart = p_vDataSize;
	
    //Create a new IStream..
    HRESULT vResult = CreateStreamOnHGlobal(NULL, true, &opIStream);     
	if(FAILED(vResult))
		return NULL;

    //Tell IStream the length of the data...
    vResult = opIStream -> SetSize(uDataLength);
	if(FAILED(vResult))
		return NULL;

    //Fetch a pointer to the RAM allocated by IStream (don't forget to unlock it again after we're done!)
    vResult = GetHGlobalFromStream(opIStream, &phglobal);
	if(FAILED(vResult))
	{
		opIStream -> Release();
		(*p_vppBufferPointer) = NULL;
		return NULL;
	}
	(*p_vppBufferPointer) = static_cast<char*>(GlobalLock(phglobal));;
    GlobalUnlock(phglobal);

	LARGE_INTEGER uInteger;
	uInteger.QuadPart = 0;
	vResult = opIStream -> Seek(uInteger, STREAM_SEEK_SET, NULL);
	if(FAILED(vResult))
	{
		opIStream -> Release();
		(*p_vppBufferPointer) = NULL;
		return NULL;
	}

	//There... We have our IStream now!
    return opIStream;
}

bool EMWinDirectXPlugin::ReadInternalSettings(char** p_vppData, uint32* p_vpLength)
{
	bool vReturn = false;

	IPersistStream* opIPersistStream = NULL;
	HRESULT vResult = m_opIBaseFilterInstance -> QueryInterface(IID_IPersistStream, (void**) &opIPersistStream);

	if(SUCCEEDED(vResult))
	{
		ULARGE_INTEGER uLarge;
		opIPersistStream -> GetSizeMax(&uLarge);
		*p_vpLength = (uint32) uLarge.QuadPart;
		*p_vppData = EM_new char[*p_vpLength];

		char* opTmp = NULL;

		IStream* opIStream = OpenIStreamFromRAM(&opTmp, *p_vpLength);
		HRESULT hr = opIPersistStream -> Save(opIStream, true);
		vReturn = SUCCEEDED(hr);

		memcpy(*p_vppData, opTmp, *p_vpLength);

		opIStream -> Release();
		opIPersistStream -> Release();	
	}

	return vReturn;
}

bool EMWinDirectXPlugin::WriteInternalSettings(char* p_vpData, uint32 p_vLength)
{
	bool vReturn = false;

	char* vpBuffer = NULL;
	IStream* opIStream = NULL;

	IPersistStream* opIPersistStream = NULL;
	HRESULT vResult = m_opIBaseFilterInstance -> QueryInterface(IID_IPersistStream, (void**) &opIPersistStream);

	if(SUCCEEDED(vResult))
	{
		uint64 vSizeMax = 0;
		ULARGE_INTEGER uLarge;

		opIPersistStream -> GetSizeMax(&uLarge);
		vSizeMax = (uint64) uLarge.QuadPart;

		if(vSizeMax == p_vLength)
		{
			opIStream = OpenIStreamFromRAM(&vpBuffer, vSizeMax);

			memcpy(vpBuffer, p_vpData, vSizeMax);
			HRESULT hr = opIPersistStream -> Load(opIStream);
			vReturn = SUCCEEDED(hr);

			opIStream -> Release();
		}
		
		opIPersistStream -> Release();
	}
	return vReturn;
}


EMWinDirectXPlugin::EMWinDirectXPlugin(string p_oPluginName, IBaseFilter* p_opIBaseFilterInstance) 
	:	EMPlugin(p_oPluginName, EM_TYPE_RAW_AUDIO, EM_WINDOWS_DIRECTX_AUDIO),
		m_oName(p_oPluginName),
		m_opIBaseFilterInstance(p_opIBaseFilterInstance),
		m_opPluginEngine(NULL)
{
	m_opDryBuffer = EM_new EMMediaDataBuffer(EM_TYPE_RAW_AUDIO, static_cast<int64>(*static_cast<int32*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_AUDIO_BUFFER_SIZE))));
	GetPresets();
}

EMWinDirectXPlugin::~EMWinDirectXPlugin()
{
	delete m_opDryBuffer;
//	m_opIBaseFilterInstance -> Release();
	if(m_opPluginEngine != NULL)
		delete m_opPluginEngine;
	m_opIBaseFilterInstance = NULL;
}

bool EMWinDirectXPlugin::ActivatePreset(int32 p_vID)
{
	HRESULT vResult = E_FAIL;

	list<EMPluginPreset*>::const_iterator opIter;
	for(opIter = m_opPresets -> begin(); opIter != m_opPresets -> end(); opIter++)
	{
		if((*opIter) -> m_vID == p_vID)
		{
			EMPluginPreset* opPreset = (*opIter);

			if(opPreset -> m_vType == 0)
			{	//Factory preset
				IStaticFilterPreset* opIStaticFilterPreset = NULL;
				vResult = m_opIBaseFilterInstance -> QueryInterface(IID_IStaticFilterPreset, (void**) &opIStaticFilterPreset);
				if(SUCCEEDED(vResult))
				{
					vResult = opIStaticFilterPreset -> UsePreset(*reinterpret_cast<uint32*>(opPreset -> m_vpData));
					if(FAILED(vResult))
					{
						opIStaticFilterPreset -> Release();
						EMDebugger("ERROR! Failed to set preset!");
						return false;
					}
					opIStaticFilterPreset -> Release();
					return true;
				}
				EMDebugger("ERROR! Failed to get IStaticFilterPreset interface for plugin!");
				return false;
			}
			else 
			{
				IStream* opIStream = NULL;
				if(opPreset -> m_vType == 1)
				{	
					//Binary string
					char* vpStreamPointer = NULL;
					opIStream = OpenIStreamFromRAM(&vpStreamPointer, opPreset -> m_vDataSize);
					memcpy(vpStreamPointer, opPreset -> m_vpData, opPreset -> m_vDataSize);
				}
				else if(opPreset -> m_vType == 2)
				{	
					//File
					string oFileName = string(opPreset -> m_vpData);
					opIStream = OpenIStreamFromFile(oFileName);
				}

				if(opIStream == NULL)
				{
					EMDebugger("ERROR! Failed to get IStream for preset data!");
					return false;
				}

				IPersistStream* opIPersistStream = NULL;
				vResult = m_opIBaseFilterInstance -> QueryInterface(IID_IPersistStream, (void**) &opIPersistStream);
				if(SUCCEEDED(vResult))
				{
					vResult = opIPersistStream -> Load(opIStream);
					if(FAILED(vResult))
					{
						opIStream -> Release();
						opIPersistStream -> Release();
						EMDebugger("ERROR! Failed to set preset!");
						return false;
					}
					opIStream -> Release();
					opIPersistStream -> Release();
					return true;
				}
				opIStream -> Release();
				EMDebugger("ERROR! Failed to get interface to PersistStream from plugin!");
				return false;
			}
			return true;
		}
	}

	EMDebugger("ERROR! Could not find that preset in memory!");
	return false;
}

list<EMPluginPreset*>* EMWinDirectXPlugin::GetPresets()
{
	m_opPresets -> clear();

	IStaticFilterPreset* opIStaticFilterPreset = NULL;
	HRESULT vResult = m_opIBaseFilterInstance -> QueryInterface(IID_IStaticFilterPreset, (void**) &opIStaticFilterPreset);
	if(SUCCEEDED(vResult))
	{
		int32 vPresetCount = 0;
		opIStaticFilterPreset -> GetPresetCount(&vPresetCount);

		for(uint32 vPresetNumber = 0; vPresetNumber < vPresetCount; vPresetNumber++)
		{
			WCHAR uUnicodeName[512];
			opIStaticFilterPreset -> GetPresetName(vPresetNumber, uUnicodeName, 512);
			string oPresetName = ((EMWinMediaUtility*) EMMediaUtility::Instance()) -> UnicodeToString(uUnicodeName);
				
			EMPluginPreset* opPreset = EM_new EMPluginPreset();

			opPreset -> m_oName = oPresetName;
			opPreset -> m_vpData = EM_new char[sizeof(uint32)];
			(*reinterpret_cast<uint32*>(opPreset -> m_vpData)) = vPresetNumber;
			opPreset -> m_vType = 0;
			opPreset -> m_vDataSize = 0;

			m_opPresets -> push_back(opPreset);
			opPreset = NULL;
		}

		int32 v = opIStaticFilterPreset -> Release();
		opIStaticFilterPreset = NULL;
	}

	IPersistStream* opIPersistStream = NULL;
	vResult = m_opIBaseFilterInstance -> QueryInterface(IID_IPersistStream, (void**) &opIPersistStream);
	if(SUCCEEDED(vResult))
	{
		HKEY uRegistryKey;
		CLSID vFilterCLSID;
		opIPersistStream -> GetClassID(&vFilterCLSID);

		vResult = RegOpenKeyEx(HKEY_CURRENT_USER, TEXT("Software\\DirectShow\\Presets"), 0, KEY_READ | KEY_ENUMERATE_SUB_KEYS, &uRegistryKey);
		if(vResult == ERROR_SUCCESS)
		{
			WCHAR* vFilterCLSIDString = NULL;
			StringFromCLSID(vFilterCLSID, &vFilterCLSIDString);
			
			HKEY uFilterKey;
			string oCLSIDString = ((EMWinMediaUtility*) EMMediaUtility::Instance()) -> UnicodeToString(vFilterCLSIDString);
			vResult = RegOpenKeyEx(uRegistryKey, oCLSIDString.c_str(), 0, KEY_READ | KEY_ENUMERATE_SUB_KEYS, &uFilterKey);
			if(vResult == ERROR_SUCCESS)
			{
				//uFilterKey this is what we should enumerate to the the keys and their values...
				char vpValueName[MAX_PATH];
				uint32 vValueNameSize = MAX_PATH;

				uchar vpValue[MAX_PATH];
				uint32 vValueSize = MAX_PATH;

				uint32 vIndex = 0;
				while(true)
				{
					uint32 vType = 0;
					vResult = RegEnumValue(uFilterKey, (vIndex++), vpValueName, &vValueNameSize , NULL, &vType, vpValue, &vValueSize);
					if(vResult == ERROR_SUCCESS)
					{
						string oValueName = "";
						EMPluginPreset* opPreset = EM_new EMPluginPreset();

						oValueName = string(vpValueName);

						opPreset -> m_oName = oValueName;
						opPreset -> m_vDataSize = vValueSize;
						opPreset -> m_vpData = EM_new char[vValueSize];
						memcpy(opPreset -> m_vpData, (char*) vpValue, vValueSize);

						if(vType == REG_BINARY)
							opPreset -> m_vType = 1;
						else if(vType == REG_SZ)
							opPreset -> m_vType = 2;
						else
						{
							delete opPreset;
							opPreset = NULL;
						}

						if(opPreset != NULL)
						{
							m_opPresets -> push_back(opPreset);
							opPreset = NULL;
						}
					}
					else
						break;
				}
				RegCloseKey(uFilterKey); // Closing the regestry key handel only if we got one.
			}
			RegCloseKey(uRegistryKey); // Closing the registry key handel only if we got one.
		}
		int32 v = opIPersistStream -> Release();
		opIPersistStream = NULL;
	}

	int32 vNumPresets = m_opPresets -> size();

	return m_opPresets;
}

bool EMWinDirectXPlugin::PrepareToPlayE()
{
	if(m_vIsInitialized)
	{
		EMDebugger("ERROR! Plugin already initialized!");
		return true;
	}
	m_opPluginEngine = EM_new EMWinDXPluginEngine(m_oName.c_str());
	bool vResult = m_opPluginEngine -> InitCheckE(m_opIBaseFilterInstance);
	if(! vResult)
		return false;
	m_vIsInitialized = true;
	return true;
}

EMMediaDataBuffer* EMWinDirectXPlugin::ProcessBufferE(list<EMMediaDataBuffer*>* p_opBufferList)
{
	if(p_opBufferList -> size() != 1)
	{
		while(p_opBufferList -> size() > 0)
		{
			p_opBufferList -> front() -> Recycle();
			p_opBufferList -> pop_front();
		}
		return NULL;
	}

	EMMediaDataBuffer* opBuffer = p_opBufferList -> front();

	signed short* opWetData = static_cast<signed short*>(opBuffer -> Data());
	signed short* opDryData = static_cast<signed short*>(m_opDryBuffer -> Data());
	m_opDryBuffer -> m_vSizeUsed = opBuffer -> m_vSizeUsed;

	//TODO 1: Don't process if the plugin's DryWetMix is 0.0...
	//TODO 2: Don't copy to dry-buffer if the plugin's DryWetMix is 1.0
	for(register uint16 vPosition = 0; vPosition < opBuffer -> m_vSizeUsed / sizeof(signed short); vPosition++)
	{
		opDryData[vPosition] = (signed short) (((float) opWetData[vPosition]) * (1.0 - m_vDryWetMix));
		opWetData[vPosition] = (signed short) (((float) opWetData[vPosition]) * m_vDryWetMix);
	}
	m_opPluginEngine -> ProcessBuffer(opBuffer);

	p_opBufferList -> push_back(m_opDryBuffer);
	opBuffer = EMAudioMixFunctions::AudioMixPreFader(p_opBufferList);
	p_opBufferList -> clear();

	if(GetDestination() == NULL)
		EMDebugger("ERROR! Destination is NULL in DirectX plugin!");
	opBuffer -> m_opDestination = this -> GetDestination();
	return opBuffer;
}

string EMWinDirectXPlugin::GetName() const
{
	return m_oName;
}

IBaseFilter* EMWinDirectXPlugin::GetIBaseFilter() const
{
	return m_opIBaseFilterInstance;
}

bool EMWinDirectXPlugin::ShowDialog()
{
	return m_opPluginEngine -> ShowDialog();
}

bool EMWinDirectXPlugin::LoadSetting(EMProjectDataLoader* p_opLoader)
{
	bool vReturn = false;

	if(string(p_opLoader -> LoadString()) != string("EMT"))
	{
		EMDebugger("ERROR! Error in project file!");
		return false;
	}

	if(p_opLoader -> LoadBool()) //If there's some info for us in the project file...
	{
		uint32 vSize = p_opLoader -> LoadUInt32();
		char* vpBuffer = EM_new char[vSize];

		p_opLoader -> LoadBytes(vpBuffer, vSize);

		vReturn = WriteInternalSettings(vpBuffer, vSize);

		delete vpBuffer;
		
/*		uint32 vSize = p_opLoader -> LoadUInt32();
		char* vpBuffer = NULL; //Will point into the stream's data...
		IPersistStream* opIPersistStream = NULL;
		HRESULT vResult = m_opIBaseFilterInstance -> QueryInterface(IID_IPersistStream, (void**) &opIPersistStream);
		if(SUCCEEDED(vResult))
		{
			uint64 vSizeMax = 0;
			ULARGE_INTEGER uLarge;
			opIPersistStream -> GetSizeMax(&uLarge);
			vSizeMax = (uint64) uLarge.QuadPart;
			if(vSizeMax != vSize)
				EMDebugger("ERROR! SizeMax for settings in plugin doesn't match what's stored in the project!");

			IStream* opIStream = OpenIStreamFromRAM(&vpBuffer, vSize);
			p_opLoader -> LoadBytes(vpBuffer, vSize);
			vResult = opIPersistStream -> Load(opIStream);
			if(FAILED(vResult))
			{
			}
			opIStream -> Release();
			opIPersistStream -> Release();
		}
		return true;*/
	}
	return vReturn;
}

bool EMWinDirectXPlugin::SaveSetting(EMProjectDataSaver* p_opSaver)
{
	p_opSaver -> SaveString("EMT");
	
	uint32 vSize = 0;
	char* vpBuffer = NULL;

	if(ReadInternalSettings(&vpBuffer, &vSize))
	{
		p_opSaver -> SaveBool(true);
		p_opSaver -> SaveUInt32(vSize);
		p_opSaver -> SaveBytes(vpBuffer, vSize);
		delete vpBuffer;
	}
	else
		p_opSaver -> SaveBool(false);

	return true;

/*	p_opSaver -> SaveString("EMT");
	IPersistStream* opIPersistStream = NULL;
	HRESULT vResult = m_opIBaseFilterInstance -> QueryInterface(IID_IPersistStream, (void**) &opIPersistStream);
	if(SUCCEEDED(vResult))
	{
		uint32 vSizeMax = 0;
		ULARGE_INTEGER uLarge;
		opIPersistStream -> GetSizeMax(&uLarge);
		vSizeMax = (uint32) uLarge.QuadPart;
		char* vpBuffer = NULL;
		IStream* opIStream = OpenIStreamFromRAM(&vpBuffer, vSizeMax);
		vResult = opIPersistStream -> Save(opIStream, true);
		if(SUCCEEDED(vResult))
		{
			p_opSaver -> SaveBool(true); //Success!
			p_opSaver -> SaveUInt32(vSizeMax);
			p_opSaver -> SaveBytes(vpBuffer, vSizeMax);
		}
		else
			p_opSaver -> SaveBool(false); //Failure! Project file won't contain any information about the plugin!
		opIStream -> Release();
		opIPersistStream -> Release();
	}
	else
		p_opSaver -> SaveBool(false); //Failure! Project file won't contain any information about the plugin!
	return true;*/
}

void* EMWinDirectXPlugin::GetNativeEffect() const
{
	return static_cast<void*>(m_opIBaseFilterInstance);
}

EMPlugin* EMWinDirectXPlugin::Clone()
{
	char* vpBuffer = NULL;
	uint32 vSize = -1;

	EMPluginEntry* opEntry = EMPluginRepository::Instance() -> Find(GetEntryID());
	if(opEntry == NULL)
	{
		MessageBox(NULL, "Couldn't find the plugin in the plugin repository!", "Error!", MB_OK | MB_ICONERROR);
		return NULL;
	}

	EMWinDirectXPlugin* opCopy = static_cast<EMWinDirectXPlugin*>(opEntry -> InstantiateNativePlugin());
	if(opCopy == NULL)
	{
		MessageBox(NULL, "Couldn't use the plugin!", "Error!", MB_OK | MB_ICONERROR);
		return NULL;
	}

	if(ReadInternalSettings(&vpBuffer, &vSize))
	{
		opCopy -> WriteInternalSettings(vpBuffer, vSize);
		delete vpBuffer;
	}

	opCopy -> SetDryWetMix(GetDryWetMix());
	return opCopy;
}


#endif