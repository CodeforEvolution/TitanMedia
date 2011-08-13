#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS
#include "EMWinMediaSystemInspector.h"

#include "EMRealtimeInputDescriptor.h"
#include "EMRealtimeEncoderDescriptor.h"
#include "EMRealtimeOutputDescriptor.h" 
#include "EMWinRealtimeAudioOutputDescriptor.h"
#include "EMWinRealtimeVideoOutputDescriptor.h"
#include "EMWinRealtimeMIDIOutputDescriptor.h"
#include "EMWinRealtimeMIDIInputDescriptor.h"
#include "EMWinRealtimeVideoEncoderDescriptor.h"
#include "EMWinRealtimeAudioEncoderDescriptor.h"
#include "EMWinRealtimeRenderToDiskOutputDescriptor.h"
#include "EMWinRealtimeAudioInputDescriptor.h"
#include "EMMediaIDManager.h" 
#include "EMDescriptor.h"
#include "EMNativePluginEntry.h"
#include "EMSimpleVideoEffectPlugin.h"
#include "EMBrightnessFilter.h"
#include "EMContrastFilter.h"
#include "EMNegativeFilter.h"
#include "EMWipeTransition.h"
#include "EMBarnTransition.h"
#include "EMNoTransition.h"
#include "EMRGBFilter.h"
#include "EMHSVFilter.h"
#include "EMEqualizerPlugin.h"
#include "EMWinPluginEntry.h"	
#include "EMMediaUtility.h"
#include "EMWinMediaUtility.h" 
#include "EMPluginRepository.h"
#include "EMWinDXPluginEngine.h" 
#include "EMWinMIDISystem.h"  
#include "EMPlugin.h"
#include "EMWinTransitionEntry.h"
#include "EMMediaDataBuffer.h"
#include "EMWinSMPTEVideoTransitionNames.h" 
#include "EMWinChromaTransitionEntry.h"
#include "EMWinDXTransitionEntry.h" 
#include "EMWinSMPTETransitionEntry.h" 
#include "EMMediaEngineUIDs.h"
#include "EMMediaEngineBuild.h" 
#include <streams.h>
#include <Sfiface.h>
#include "EMCommandLineIDs.h"
#include "EMSettingsRepository.h"
#include "EMSettingIDs.h"
#include "EMMediaEngine.h"
#include "EMCommandRepository.h"
#include "CommandIDs.h"
#include "EMMediaDebugLog.h"

#include "EMDestructiveSilence.h" 
#include "EMDestructiveReverse.h"  
#include "EMDestructiveCrop.h" 
#include "EMDestructiveFadeIn.h"
#include "EMDestructiveFadeOut.h"
#include "EMDestructiveNormalize.h"
#include "EMDestructiveDXWrapper.h"

static const GUID CLSID_EMReadFrame = {0x36a5f770, 0xfe4c, 0x11ce, {0xa8, 0xed, 0x00, 0xaa, 0x00, 0x2f, 0xea, 0xb5}};
static const GUID IID_EMReadFrameInterface = {0xfe75b1e1, 0x3821, 0x11d5, {0x82, 0x00, 0x2, 0xb3, 0x18, 0x0, 0x46}};
static const GUID CLSID_EMWinMediaInputFilter = {0x952c35ab, 0x4dcb, 0x11d5, {0xbc, 0x74, 0x0, 0x2, 0xb3, 0x17, 0xf3, 0xf2}};
static const GUID IID_IEMWinMediaInputFilter =  {0x952c35ac, 0x4dcb, 0x11d5, {0xbc, 0x74, 0x0, 0x2, 0xb3, 0x17, 0xf3, 0xf2}};
static const GUID CLSID_EMWinMediaOutputFilter = {0x952c35a5, 0x4dcb, 0x11d5, {0xbc, 0x74, 0x0, 0x2, 0xb3, 0x17, 0xf3, 0xf2}};
static const GUID CLSID_EMWinMediaVideoOutputFilter = {0xbedfce61, 0x535d, 0x11d5, {0x82, 0x59, 0x0, 0x2, 0xb3, 0x18, 0x0, 0x46}};
static const GUID IID_IEMWinMediaOutputFilter =  {0x952c35a8, 0x4dcb, 0x11d5, {0xbc, 0x74, 0x0, 0x2, 0xb3, 0x17, 0xf3, 0xf2}};
static const GUID IID_IEMWinMediaVideoOutputFilter = { 0xbedfce63, 0x535d, 0x11d5, { 0x82, 0x59, 0x0, 0x2, 0xb3, 0x18, 0x0, 0x46 } };
static const GUID CLSID_EMWinDXPluginEngineFilter = { 0x2c56a781, 0x5c3b, 0x11d5, { 0xbc, 0x75, 0x0, 0x2, 0xb3, 0x17, 0xf3, 0xf2 } };
static const GUID IID_IEMWinDXPluginEngineInterface = { 0x1e90d8e1, 0x5c3c, 0x11d5, { 0xbc, 0x75, 0x0, 0x2, 0xb3, 0x17, 0xf3, 0xf2 } };
static const GUID CLSID_EMWinVideoShuffleOutputFilter = { 0x3fc76daf, 0x80e8, 0x11d5, { 0x82, 0x59, 0x0, 0x2, 0xb3, 0x18, 0x0, 0x46 } };
static const GUID IID_IEMWinVideoShuffleOutputInterface = { 0x3fc76dae, 0x80e8, 0x11d5, { 0x82, 0x59, 0x0, 0x2, 0xb3, 0x18, 0x0, 0x46 } };
static const GUID CLSID_EMWinAudioConverterFilter = { 0x5c347fc8, 0x79a6, 0x47c5, { 0xa1, 0xb, 0x15, 0x8b, 0x9c, 0x4d, 0x87, 0xd8 }};
static const GUID IID_IEMWinAudioConverterInterface = { 0x7e3ac55a, 0x8aaa, 0x428d, { 0xa4, 0x10, 0x74, 0x33, 0x2c, 0x94, 0xef, 0xfd } };
static const GUID CLSID_EMWaveWriter = { 0xf1569a6, 0x289b, 0x43cf, { 0xac, 0x33, 0x3f, 0x8a, 0xd7, 0x2d, 0xb, 0xd2 } };
static const GUID IID_IEMWinDXCallbackInterface = { 0x1ea34e21, 0xaa60, 0x4615, { 0xb1, 0xa2, 0x62, 0x8c, 0xd9, 0x7b, 0x6a, 0x33 } };

//things Microsoft should'v had in their libs..
static const GUID CLSID_DXTransformFactory = {0xD1FE6762, 0xFC48, 0x11D0, {0x88, 0x3A, 0x3C, 0x8B, 0x00, 0xC1, 0x00, 0x00} };
static const GUID IID_IDXTransformFactory = {0x6A950B2B, 0xA971, 0x11d1, {0x81, 0xC8, 0x00, 0x00, 0xF8, 0x75, 0x57, 0xDB} };
static const GUID IID_IDXSurface = {0xB39FD73F, 0xE139, 0x11d1, {0x90, 0x65, 0x00, 0xC0, 0x4F, 0xD9, 0x18, 0x9D} };
static const GUID IID_IDXSurfaceFactory = {0x144946F5, 0xC4D4, 0x11d1, {0x81, 0xD1, 0x00, 0x00, 0xF8, 0x75, 0x57, 0xDB} };
static const GUID IID_IDXTransform = {0x30A5FB78, 0xE11F, 0x11d1, {0x90, 0x64, 0x00, 0xC0, 0x4F, 0xD9, 0x18, 0x9D} };
static const GUID IID_IDXEffect = {0xE31FB81B, 0x1335, 0x11d1, {0x81, 0x89, 0x00, 0x00, 0xF8, 0x75, 0x57, 0xDB} };
//Defines have been moved to Framework/Globals/EMMediaEngineBuild.h

class EMGUIDNameMap
{
public:
	EMGUIDNameMap() { m_uGUID = GUID_NULL; m_oName = ""; };
	~EMGUIDNameMap() { m_uGUID = GUID_NULL; m_oName = ""; };
	GUID m_uGUID;
	string m_oName;
};

uint32 EMWinMediaSystemInspector::m_vDisableFeaturesFlags = 0;

EMWinMediaSystemInspector::EMWinMediaSystemInspector()
	:	EMMediaSystemInspector()
{
	m_vID = EMMediaIDManager::MakeID();
	m_vDisableFeaturesFlags = *(static_cast<uint32*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_DISABLE_FEATURES_FLAGS)));
	InspectCodecs();
}

EMWinMediaSystemInspector::~EMWinMediaSystemInspector()
{
}

list<EMRealtimeInputDescriptor*>* EMWinMediaSystemInspector::GetRealtimeAudioInputs()
{
	list<EMRealtimeInputDescriptor*>* opList1;

	if(IsFlagEnabled(EM_CL_SEARCH_FOR_AUDIO_INPUTS))
	{
		 opList1 = GetInputs(EM_TYPE_RAW_AUDIO);
	}
	else
	{
		opList1 = EM_new list<EMRealtimeInputDescriptor*>();
	}
	return opList1;
}

list<EMRealtimeInputDescriptor*>* EMWinMediaSystemInspector::GetRealtimeVideoInputs()
{
	list<EMRealtimeInputDescriptor*>* opList2;

	if(IsFlagEnabled(EM_CL_SEARCH_FOR_VIDEO_INPUTS))
	{
		opList2 = GetInputs(EM_TYPE_ANY_VIDEO);
	}
	else
	{
		opList2 = EM_new list<EMRealtimeInputDescriptor*>();
	}

	return opList2;
}

list<EMRealtimeOutputDescriptor*>* EMWinMediaSystemInspector::GetRealtimeAudioOutputs()
{
	list<EMRealtimeOutputDescriptor*>* opList1;

	if(IsFlagEnabled(EM_CL_SEARCH_FOR_AUDIO_OUTPUTS))
	{
		opList1 = GetOutputs(EM_TYPE_RAW_AUDIO);
	}
	else
	{
		opList1 = EM_new list<EMRealtimeOutputDescriptor*>();
	}

	return opList1;
}

list<EMRealtimeOutputDescriptor*>* EMWinMediaSystemInspector::GetRealtimeVideoOutputs() 
{
	list<EMRealtimeOutputDescriptor*>* opList1;

	if(IsFlagEnabled(EM_CL_SEARCH_FOR_VIDEO_OUTPUTS))
	{
		opList1 = GetOutputs(EM_TYPE_RAW_VIDEO);
	}
	else
	{
		opList1 = EM_new list<EMRealtimeOutputDescriptor*>();
	}

	if(opList1 == NULL)
	{
		return NULL;
	}
	return opList1;
}

list<EMRealtimeOutputDescriptor*>* EMWinMediaSystemInspector::GetRealtimeMIDIOutputs()
{
	list<EMRealtimeOutputDescriptor*>* opList1;

	if(IsFlagEnabled(EM_CL_SEARCH_FOR_MIDI_OUTPUTS))
	{
		opList1 = EMWinMIDISystem::Instance() -> GetMIDIOutputs();
	}
	else
	{
		opList1 = new list<EMRealtimeOutputDescriptor*>();
	}

	return opList1;
}

list<EMRealtimeInputDescriptor*>* EMWinMediaSystemInspector::GetRealtimeMIDIInputs()
{
	list<EMRealtimeInputDescriptor*>* opList1;

	if(IsFlagEnabled(EM_CL_SEARCH_FOR_MIDI_INPUTS))
	{
		opList1 = EMWinMIDISystem::Instance() -> GetMIDIInputs();
	}
	else
	{
		opList1 = EM_new list<EMRealtimeInputDescriptor*>();
	}

	return opList1;
}

list<EMRealtimeEncoderDescriptor*>* EMWinMediaSystemInspector::GetRealtimeVideoEncoders() 
{
	list<EMRealtimeEncoderDescriptor*>* opList1;

	if(IsFlagEnabled(EM_CL_SEARCH_FOR_VIDEO_ENCODERS))
	{
		opList1 = GetEncoders(EM_TYPE_ANY_VIDEO);
	}
	else
	{
		opList1 = EM_new list<EMRealtimeEncoderDescriptor*>();
	}
 
	if(opList1 == NULL)
	{
		return NULL;
	}
	return opList1;
}

list<EMRealtimeEncoderDescriptor*>* EMWinMediaSystemInspector::GetRealtimeAudioEncoders() 
{
	list<EMRealtimeEncoderDescriptor*>* opList1;

	if(IsFlagEnabled(EM_CL_SEARCH_FOR_AUDIO_ENCODERS))
	{
		opList1 = GetEncoders(EM_TYPE_ANY_AUDIO);
	}
	else
	{
		opList1 = EM_new list<EMRealtimeEncoderDescriptor*>();
	}

	if(opList1 == NULL)
	{
		return NULL;
	}
	return opList1;
}

list<EMRealtimeEncoderDescriptor*>* EMWinMediaSystemInspector::GetEncoders(EMMediaType p_eType)
{
	list<EMRealtimeEncoderDescriptor*>* opList = NULL;
	opList = EM_new list<EMRealtimeEncoderDescriptor*>();
	
	HRESULT h = S_OK;
	ICreateDevEnum* opSystemDevicecEnumerator = NULL;
	IEnumMoniker* opCategoryEnumerator = NULL;
	IMoniker* opMoniker;
	IPropertyBag* opProperties;
	IBaseFilter* opABaseFilterInterface = NULL;
	uint32 vNumFetched;
	VARIANT uVariant;
	TCHAR vpFilterName[256];
	string oFilterName = "";

	HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC, IID_ICreateDevEnum, (void **)&opSystemDevicecEnumerator);

	if((p_eType & EM_TYPE_ANY_AUDIO) > 0)
		opSystemDevicecEnumerator -> CreateClassEnumerator(CLSID_AudioCompressorCategory, &opCategoryEnumerator, 0);
	else if((p_eType & EM_TYPE_ANY_VIDEO) > 0)
		opSystemDevicecEnumerator -> CreateClassEnumerator(CLSID_VideoCompressorCategory, &opCategoryEnumerator, 0);

	while(opCategoryEnumerator -> Next(1, &opMoniker, &vNumFetched) == S_OK)
	{
		h = opMoniker -> BindToStorage(0, 0, IID_IPropertyBag, (void **) &opProperties);
		h = opMoniker -> BindToObject(0, 0, IID_IBaseFilter, (void**) &opABaseFilterInterface);
		uVariant.vt = VT_BSTR;

		opProperties -> Read(L"FriendlyName", &uVariant, 0);
		WideCharToMultiByte(CP_ACP, 0, uVariant.bstrVal, -1, vpFilterName, 256, 0, 0);

		SysFreeString(uVariant.bstrVal);

		oFilterName = vpFilterName;

		EMRealtimeEncoderDescriptor* opDescriptor = NULL;

		if((p_eType & EM_TYPE_ANY_AUDIO) > 0)
			opDescriptor = EM_new EMWinRealtimeAudioEncoderDescriptor(opABaseFilterInterface, opMoniker);
		else if((p_eType & EM_TYPE_ANY_VIDEO) > 0)
			opDescriptor = EM_new EMWinRealtimeVideoEncoderDescriptor(opABaseFilterInterface);

		opDescriptor -> SetName(oFilterName);
		opList -> push_back(opDescriptor);
		opProperties -> Release();
		opABaseFilterInterface -> Release(); 
		opMoniker -> Release(); 
	}
	opCategoryEnumerator -> Release();
	opSystemDevicecEnumerator -> Release();

	return opList;

}


list<EMRealtimeInputDescriptor*>* EMWinMediaSystemInspector::GetInputs(EMMediaType p_eType)
{
	list<EMRealtimeInputDescriptor*>* opList = NULL;
	opList = EM_new list<EMRealtimeInputDescriptor*>();
	
	if((p_eType & EM_TYPE_RAW_AUDIO) > 0)
	{
		HRESULT h = S_OK;
		ICreateDevEnum* opSystemDevicecEnumerator = NULL;
		IEnumMoniker* opCategoryEnumerator = NULL;
		IMoniker* opMoniker;
		IPropertyBag* opProperties;
		IBaseFilter* opABaseFilterInterface = NULL;
		uint32 vNumFetched;
		VARIANT uVariant;
		TCHAR vpFilterName[256];
		string oFilterName = "";

		HRESULT vResult = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC, IID_ICreateDevEnum, (void **)&opSystemDevicecEnumerator);
		vResult = opSystemDevicecEnumerator -> CreateClassEnumerator(CLSID_CWaveinClassManager, &opCategoryEnumerator, 0);
		if(vResult != S_OK)
		{
			EMMediaDebugLog::Instance() -> Log("WARNING! Failed to create class enumerator for \"CLSID_AudioInputDeviceCategory\". There is no such category.");
			return opList;
		}

		while(opCategoryEnumerator -> Next(1, &opMoniker, &vNumFetched) == S_OK)
		{
			h = opMoniker -> BindToStorage(0, 0, IID_IPropertyBag, (void **) &opProperties);
			if(FAILED(h))
			{
				;//eo << "ERROR! Could not get interface to IFilterBase!" << ef;
				opMoniker -> Release();
				opMoniker = NULL;
				opProperties = NULL;
			}

			h = BindMoniker(opMoniker, 0, IID_IBaseFilter, (void**) &opABaseFilterInterface);
			if(FAILED(h))
			{
				;//eo << "ERROR! Could not get interface to IFilterBase!" << ef;
				opABaseFilterInterface = NULL;
				opMoniker ->Release();
				opMoniker = NULL;
				opProperties ->Release();
				opProperties = NULL;
			}

			uVariant.vt = VT_BSTR;
			opProperties -> Read(L"FriendlyName", &uVariant, 0);
			WideCharToMultiByte(CP_ACP, 0, uVariant.bstrVal, -1, vpFilterName, 256, 0, 0);
			SysFreeString(uVariant.bstrVal);
			oFilterName = vpFilterName;

//if(oFilterName == string("Layla24 1/2 Analog Record"))
//{
			EMRealtimeInputDescriptor* opDescriptor = NULL;
			opDescriptor = EM_new EMWinRealtimeAudioInputDescriptor(opABaseFilterInterface);

			if(! opDescriptor -> InitCheckE())
			{
				;//eo << "ERROR! Failed to initialize input\"" << oFilterName.c_str() << "\"." << ef;
				delete opDescriptor;
			}
			else
			{
				EMMediaEngine::Instance() -> GetCommandRepository() -> ExecuteCommand(COMMAND_DISPLAY_SPLASH_SCREEN_TEXT, (void*) string(string("Initializing I/O: Incorporating \"") + oFilterName + string("\"")).c_str(), NULL, NULL);
				opDescriptor -> SetName(oFilterName);
				opList -> push_back(opDescriptor);
			} 
//}
//			if(opMoniker != NULL)
//				int32 v2 = opMoniker -> Release(); 
			if(opProperties != NULL)
				int32 v1 = opProperties -> Release();
		}
		int32 v3 = opCategoryEnumerator -> Release();
		int32 v4 = opSystemDevicecEnumerator -> Release();
	}

	EMMediaEngine::Instance() -> GetCommandRepository() -> ExecuteCommand(COMMAND_DISPLAY_SPLASH_SCREEN_TEXT, (void*) string("Initializing I/O...").c_str(), NULL, NULL);
	return opList;
}

list<EMRealtimeOutputDescriptor*>* EMWinMediaSystemInspector::GetOutputs(EMMediaType p_eType)
{

	list<EMRealtimeOutputDescriptor*>* opList = NULL;
	opList = EM_new list<EMRealtimeOutputDescriptor*>();

	list<string> oInitializedOutputs;
	list<string> oFailedOutputs;
	EMRealtimeOutputDescriptor* opDescriptor = NULL;

	if((p_eType & EM_TYPE_RAW_VIDEO) > 0)
	{

/*		IBaseFilter* opVBaseFilterInterface = NULL;

		HRESULT h = CoCreateInstance(CLSID_VideoVirtualRenderer, NULL, CLSCTX_INPROC, IID_IBaseFilter, (void**) &opVBaseFilterInterface);
		if(FAILED(h))
		{
			eo << "ERROR! Failed to retrieve interface of own output filter!" << ef;
			return false;
		}
*/	
		opDescriptor = EM_new EMWinRealtimeVideoOutputDescriptor(NULL); //opVBaseFilterInterface);
		if(! opDescriptor -> InitCheckE())
		{
			;//eo << "ERROR! Failed to initialize VideoOutputDescriptor" << ef;
			delete opDescriptor;
		}
		else
		{
			string oFilterName = "VideoVirtualRenderer";
			opDescriptor -> SetName(oFilterName);
			opList -> push_back(opDescriptor);
		} 
		
		//Disk Renderer Destination
		opDescriptor = EM_new EMWinRealtimeRenderToDiskOutputDescriptor(NULL, EM_TYPE_RAW_VIDEO);

		if(! opDescriptor -> InitCheckE())
		{
			;//eo << "ERROR! Failed to initialize EMWinRealtimeRenderToDiskOutputDescriptor" << ef;
			delete opDescriptor;
		}
		else
		{
			string oFilterName = "WriteVideoToDisk";
			opDescriptor -> SetName(oFilterName);
			opDescriptor -> SetRenderOutput(TRUE);
			opList -> push_back(opDescriptor);
		}

	}
	else if((p_eType & EM_TYPE_RAW_AUDIO) > 0)
	{
		list<EMGUIDNameMap*> oMap;
		HRESULT vResult = DirectSoundEnumerate(DirectSoundAudioDeviceEnumeration, static_cast<void*>(&oMap));

		if(SUCCEEDED(vResult))
		{
			while(oMap.size() > 0)
			{ 
				GUID uGUID = GUID_NULL;
				memcpy(&uGUID, &(oMap.front() -> m_uGUID), sizeof(GUID));
				string oName = oMap.front() -> m_oName;
				EMGUIDNameMap* o = oMap.front();
				oMap.pop_front();
			
//if(oName == string("Layla24 1/2 Analog"))  
//{      
				EMWinRealtimeAudioOutputDescriptor* opDescriptor = EM_new EMWinRealtimeAudioOutputDescriptor(&uGUID);
				
				//Important to set the name before the InitCheckE or the EMWinDirectSoundOutput will be started without a name
				opDescriptor -> SetName(oName); 
				if(! opDescriptor -> InitCheckE())
					delete opDescriptor;
				else
				{
					EMMediaEngine::Instance() -> GetCommandRepository() -> ExecuteCommand(COMMAND_DISPLAY_SPLASH_SCREEN_TEXT, (void*) string(string("Initializing I/O: Incorporating \"") + oName + string("\"")).c_str(), NULL, NULL);
					opList -> push_back(opDescriptor);
				}
//}

				delete o;
			} 
		}

		EMMediaEngine::Instance() -> GetCommandRepository() -> ExecuteCommand(COMMAND_DISPLAY_SPLASH_SCREEN_TEXT, (void*) string("Initializing I/O: Incorporating Rendering Engine").c_str(), NULL, NULL);

		//Begin jeppes render to disk mojs for audio!
		opDescriptor = EM_new EMWinRealtimeRenderToDiskOutputDescriptor(NULL, EM_TYPE_RAW_AUDIO);
		if(! opDescriptor -> InitCheckE())
		{
			;//eo << "ERROR! Failed to initialize EMWinRealtimeRenderToDiskOutputDescriptor" << ef;
			delete opDescriptor;
		}
		else
		{
			string oFilterName = "WriteAudioToDisk";
			opDescriptor -> SetName(oFilterName);
			opDescriptor -> SetRenderOutput(TRUE);
			opList -> push_back(opDescriptor);
		}
	}

	EMMediaEngine::Instance() -> GetCommandRepository() -> ExecuteCommand(COMMAND_DISPLAY_SPLASH_SCREEN_TEXT, (void*) string("Initializing I/O...").c_str(), NULL, NULL);
	
	return opList;
}

list<EMCodecFormat*>* EMWinMediaSystemInspector::GetCodecs()
{
	return &m_oFormats;   
}

void EMWinMediaSystemInspector::InspectCodecs()
{
}

bool EMWinMediaSystemInspector::InitializePluginSystem()
{
	bool vDirectXResult;
	bool vSMPTEResult;
	bool vStaticResult;

	try
	{
		if(IsFlagEnabled(EM_CL_SEARCH_FOR_DIRECTX_PLUGINS))
		{
			vDirectXResult = InitializeDirectXPlugins();
		}
		else
		{
			vDirectXResult = true;
		}

		if(IsFlagEnabled(EM_CL_SEARCH_FOR_SMPTE_TRANSITIONS))
		{
			vSMPTEResult = InitializeSMPTETransitions();
			vSMPTEResult |= InitializeChromaTransitions();
		}
		else
		{
			vSMPTEResult = true;
		}
		
		bool vVSTResult = true;//InitializeVSTPlugins();

		if(IsFlagEnabled(EM_CL_SEARCH_FOR_NATIVE_VIDEO_EFFECTS))
		{
			vStaticResult = InitializeStaticPlugins();
		}
		else
		{
			vStaticResult = true;
		}

		if(!vDirectXResult)
		{
			EMDebugger("ERROR! Failed to initialize DirectX plugins!");
			return false;
		}

		if(! vSMPTEResult)
		{
			EMDebugger("ERROR! Failed to initialize SMPTE Transitions!");
			return false;
		}

		if(! vVSTResult)
		{
			EMDebugger("ERROR! Failed to initialize VST plugins!");
			return false;
		}

		if(! vStaticResult)
		{
			EMDebugger("ERROR! Failed to initialize own static plugins!");
			return false;
		}
		EMPluginRepository::Instance() -> Rewind();
		while(EMPluginRepository::Instance() -> Current() != NULL)
		{
			EMPluginEntry* opEntry = EMPluginRepository::Instance() -> Current();
			EMPluginRepository::Instance() -> Next();
		}
	} catch(...)
	{
		MessageBox(NULL,"InitializePluginSystem() Failed", "FAILED", NULL);
	}
	return true;
}

//static const GUID IID_IStaticFilterPreset = { 0x36A22460, 0xcc9,  0x11D0, { 0x85, 0x74, 0x00,0xA0,0xC9, 0x05, 0x39, 0x12 } };
bool EMWinMediaSystemInspector::InitializeDirectXPlugins()
{
#ifdef __EM_MARTIN_VERSION
//	return true; 
#endif

	try
	{
		ICreateDevEnum* opSystemDeviceEnumerator = NULL;
		CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC, IID_ICreateDevEnum, (void**) &opSystemDeviceEnumerator);

		AM_MEDIA_TYPE uMediaType1;
		WAVEFORMATEX uWaveFormatEx1;
		uWaveFormatEx1.wFormatTag = WAVE_FORMAT_PCM;
		uWaveFormatEx1.nChannels = 1;
		uWaveFormatEx1.nSamplesPerSec = 44100; //(uint32) *(static_cast<int32*>(m_opFilter -> GetSettingsRepository() -> GetSetting(SETTING_AUDIO_FRAMERATE)));
		uWaveFormatEx1.wBitsPerSample = 16; //EM_AUDIO_SAMPLESIZE * 8;        
		uWaveFormatEx1.nBlockAlign = static_cast<WORD>((uWaveFormatEx1.wBitsPerSample * uWaveFormatEx1.nChannels) / 8);
		uWaveFormatEx1.nAvgBytesPerSec = uWaveFormatEx1.nBlockAlign * uWaveFormatEx1.nSamplesPerSec;
		uWaveFormatEx1.cbSize = -1;
		CreateAudioMediaType(&uWaveFormatEx1, &uMediaType1, true);
		CMediaType* opCMediaType1 = EM_new CMediaType(uMediaType1);

		AM_MEDIA_TYPE uMediaType2;
		WAVEFORMATEX uWaveFormatEx2;
		uWaveFormatEx2.wFormatTag = WAVE_FORMAT_PCM;
		uWaveFormatEx2.nChannels = 2;
		uWaveFormatEx2.nSamplesPerSec = 44100; //(uint32) *(static_cast<int32*>(m_opFilter -> GetSettingsRepository() -> GetSetting(SETTING_AUDIO_FRAMERATE)));
		uWaveFormatEx2.wBitsPerSample = 16; //EM_AUDIO_SAMPLESIZE * 8;        
		uWaveFormatEx2.nBlockAlign = static_cast<WORD>((uWaveFormatEx2.wBitsPerSample * uWaveFormatEx2.nChannels) / 8);
		uWaveFormatEx2.nAvgBytesPerSec = uWaveFormatEx2.nBlockAlign * uWaveFormatEx2.nSamplesPerSec;
		uWaveFormatEx2.cbSize = -1;
		CreateAudioMediaType(&uWaveFormatEx2, &uMediaType2, true);
		CMediaType* opCMediaType2 = EM_new CMediaType(uMediaType2);

		IEnumMoniker* opEnumCategory = NULL;

		opSystemDeviceEnumerator -> CreateClassEnumerator(CLSID_LegacyAmFilterCategory, &opEnumCategory, 0);

		IMoniker* opMoniker;
		uint32 vNumFetched;
		IBaseFilter* opFilter = NULL;
		bool vHasInput = false;
		bool vHasOutput = false;
		bool vInputWillAcceptAudio = false;
		bool vOutputWillAcceptAudio = false;
		bool vHasProperties = false;
		bool vIsPersist = false;
		bool vIsTwoChannelCompatible = false;
		bool vCanStorePresets = false;
		uint32 vPinCount = 0;

		IEnumPins* opIEnumPins = NULL;
		IPropertyBag* opPropertyBag;
		HRESULT vResult = S_OK;
		IPin* opIPin = NULL;
		PIN_INFO uPinInfo;
		VARIANT uVariant;
		WCHAR* uvCategoryName;
		VARIANT uVariantCLSID;
		CLSID vFilterCLSID;
		
		int32 vCount = 0;

		WAVEFORMATEX sWaveFormatStructure;

		//Then fill it out with the PCM format information
		sWaveFormatStructure.wFormatTag = WAVE_FORMAT_PCM;
		sWaveFormatStructure.nChannels = EM_AUDIO_NUM_CHANNELS;
		sWaveFormatStructure.nSamplesPerSec = (uint32) 44100; //*(static_cast<int32*>(m_opFilter -> GetSettingsRepository() -> GetSetting(SETTING_AUDIO_FRAMERATE)));
		sWaveFormatStructure.wBitsPerSample = EM_AUDIO_SAMPLESIZE * 8;        
		sWaveFormatStructure.nBlockAlign = static_cast<WORD>((sWaveFormatStructure.wBitsPerSample * sWaveFormatStructure.nChannels) / 8);
		sWaveFormatStructure.nAvgBytesPerSec = sWaveFormatStructure.nBlockAlign * sWaveFormatStructure.nSamplesPerSec;
		sWaveFormatStructure.cbSize = 0;

		AM_MEDIA_TYPE uAMMediaType;
		//And then fill out the CMediaType object with information based on that WAVEFORMATEX structure
		if(FAILED(CreateAudioMediaType(&sWaveFormatStructure, &uAMMediaType, true)))
			EMDebugger("ERROR! Failed to create test format!");

		opEnumCategory -> Reset();
		while(opEnumCategory -> Next(1, &opMoniker, &vNumFetched) == S_OK)
		{
			vHasInput = false;
			vHasOutput = false;
			vInputWillAcceptAudio = false;
			vOutputWillAcceptAudio = false;
			vPinCount = 0;
			vHasProperties = false;
			vIsPersist = false;
			vIsTwoChannelCompatible = false;
	//		vCanStorePresets = true;
			
			opPropertyBag = NULL;
			vResult = opMoniker -> BindToStorage(0, 0, IID_IPropertyBag, (void**) &opPropertyBag);
			if(FAILED(vResult))
			{
				EMDebugger("ERROR!");
			}

			uVariant.vt = VT_BSTR;
			opPropertyBag -> Read(L"FriendlyName", &uVariant, 0);
			opPropertyBag -> Release();
			uvCategoryName = uVariant.bstrVal;
			string oFilterName = ((EMWinMediaUtility*) EMMediaUtility::Instance()) -> UnicodeToString(uvCategoryName);

			if(oFilterName != "Overlay Mixer" && oFilterName != "Overlay Mixer2" && oFilterName != "VBI Surface Allocator" && oFilterName != "Full Screen Renderer" && oFilterName != "File stream renderer" && oFilterName != "EMWinVideoShuffleOutputFilter" && oFilterName != "Internal LMRT Renderer")
			{
				vResult = BindMoniker(opMoniker, 0, IID_IBaseFilter, (void**) &opFilter);
			}
			else
				vResult = E_FAIL;

			if(SUCCEEDED(vResult))
			{
				vResult = opFilter -> EnumPins(&opIEnumPins);
				if(SUCCEEDED(vResult))
				{
 					while(opIEnumPins -> Next(1, &opIPin, &vNumFetched) == S_OK)
					{
						vPinCount++;
						vResult = opIPin -> QueryPinInfo(&uPinInfo);
						if(SUCCEEDED(vResult))
						{
							ISpecifyPropertyPages* opISpecifyPropertyPages = NULL;
							vResult = opFilter -> QueryInterface(IID_ISpecifyPropertyPages, (void**) &opISpecifyPropertyPages);
							if(SUCCEEDED(vResult))
							{
								vHasProperties = true;
								int32 v = opISpecifyPropertyPages -> Release();
								opISpecifyPropertyPages = NULL;
							}
							
							if(! vIsPersist)
							{
								IPersistStream* opIPersistStream = NULL;
								vResult = opFilter -> QueryInterface(IID_IPersistStream, (void**) &opIPersistStream);
								if(SUCCEEDED(vResult))
								{
									vIsPersist = true;
									int32 v = opIPersistStream -> Release();
									opIPersistStream = NULL;
								}
							}
							
							if(! vCanStorePresets)
							{
								IStaticFilterPreset* opIStaticFilterPreset = NULL;
								vResult = opFilter -> QueryInterface(IID_IStaticFilterPreset, (void**) &opIStaticFilterPreset);
								if(SUCCEEDED(vResult))
								{
									vCanStorePresets = true;
									int32 v = opIStaticFilterPreset -> Release();
									opIStaticFilterPreset = NULL;

								}
							} 

							if(uPinInfo.dir == PINDIR_INPUT && ! vHasInput)
							{
								LPWSTR vInName = uPinInfo.achName;
								string oInputName = ((EMWinMediaUtility*) EMMediaUtility::Instance()) -> UnicodeToString(vInName);
								vHasInput = (oInputName == "Input");
							}
							else if(uPinInfo.dir == PINDIR_INPUT && vHasInput)
								vHasInput = false;
							if(uPinInfo.dir == PINDIR_OUTPUT && ! vHasOutput)
							{
								LPWSTR vOutName = uPinInfo.achName;
								string oOutputName = ((EMWinMediaUtility*) EMMediaUtility::Instance()) -> UnicodeToString(vOutName);
								vHasOutput = (oOutputName == "Output");
							}
							else if(uPinInfo.dir == PINDIR_OUTPUT && vHasOutput)
								vHasOutput = false;

							//Check for the number of channels!
							if(uPinInfo.dir == PINDIR_OUTPUT)
							{
								try
								{
									int32 v = ((WAVEFORMATEX*) uAMMediaType.pbFormat) -> nChannels;
									vResult = opIPin -> QueryAccept(const_cast<const AM_MEDIA_TYPE*>(&uAMMediaType));
									if(vResult == S_OK)
										vIsTwoChannelCompatible = true;
									else
										vIsTwoChannelCompatible = false;
								}
								catch(...)
								{
									vIsTwoChannelCompatible = false;
								}
							}

							try
							{
								if((! vInputWillAcceptAudio) || (! vOutputWillAcceptAudio))
								{
									if(SUCCEEDED(opIPin -> QueryAccept(opCMediaType1)))
									{
										if(uPinInfo.dir == PINDIR_INPUT)
											vInputWillAcceptAudio = true;
										else
											vOutputWillAcceptAudio = true;
									}
									else if(SUCCEEDED(opIPin -> QueryAccept(opCMediaType2)))
									{
										if(uPinInfo.dir == PINDIR_INPUT)
											vInputWillAcceptAudio = true;
										else
											vOutputWillAcceptAudio = true;
									}
								}
							}
							catch(...)
							{
							} 

							int32 v = uPinInfo.pFilter -> Release();
						}
						else
							;//MessageBox(NULL, "ERROR! Failed to get PIN_INFO!", "", MB_OK);
						
						int32 v = opIPin -> Release();
						opIPin = NULL;
					}
					int32 v = opIEnumPins -> Release();
					opIEnumPins = NULL;
				} 
				int32 vRefcount = opFilter -> Release();
				opFilter = NULL; 
			}

			if(vHasInput && vHasOutput && vInputWillAcceptAudio && vOutputWillAcceptAudio && vPinCount == 2 && vHasProperties && vIsPersist)
			{
				if(vIsTwoChannelCompatible)
				{
					uVariantCLSID.vt = VT_BSTR;
					HRESULT hr = opPropertyBag -> Read(L"CLSID", &uVariantCLSID, 0);
					if(SUCCEEDED(hr))
					{
						if(CLSIDFromString(uVariantCLSID.bstrVal, &vFilterCLSID) == S_OK)
						{

							EMMediaEngine::Instance() -> GetCommandRepository() -> ExecuteCommand(COMMAND_DISPLAY_SPLASH_SCREEN_TEXT, (void*) string(string("Initializing Plugin Engine: Incorporating \"") + oFilterName + string("\"")).c_str(), NULL, NULL);

							EMWinPluginEntry* opPluginInformation = EM_new EMWinPluginEntry(oFilterName, vFilterCLSID, EM_TYPE_RAW_AUDIO, EM_WINDOWS_DIRECTX_AUDIO);
							EMPluginRepository::Instance() -> Add(opPluginInformation);

	//						EMPluginEntry* opDestruction = EM_new EMNativePluginEntry("Destructive " + oFilterName, EM_new EMDestructiveDXWrapper(opPluginInformation));
	//						EMPluginRepository::Instance() -> Add(opDestruction);

							vCount++;
						}
					}
					SysFreeString(uVariant.bstrVal);
				}
	//			else
	//				MessageBox(NULL, "Error! Plugin must support 2-channel audio!", "", MB_OK);

			}

			int32 vRefCount = opPropertyBag -> Release();
			opMoniker = NULL;
			opPropertyBag = NULL;
		}

		opEnumCategory -> Reset();
		opEnumCategory -> Release();
		opEnumCategory = NULL;
		int32 vRefCount = opSystemDeviceEnumerator -> Release();
		opSystemDeviceEnumerator = NULL;

		delete opCMediaType1;
		delete opCMediaType2;

		EMMediaEngine::Instance() -> GetCommandRepository() -> ExecuteCommand(COMMAND_DISPLAY_SPLASH_SCREEN_TEXT, (void*) string("Initializing Plugin Engine...").c_str(), NULL, NULL);
	} catch(...)
	{
		MessageBox(NULL,"InitializeDirectXPlugins() Failed", "FAILED", NULL);
	}

	return true;

}

//Sets up our own (Elegant Media's) plugins, such as video contrast, video brightness, etc.
bool EMWinMediaSystemInspector::InitializeStaticPlugins()
{
	try
	{
		EMNativePluginEntry* opPluginInformation;
		opPluginInformation = EM_new EMNativePluginEntry("EM Brightness", EM_new EMSimpleVideoEffectPlugin(EM_new EMBrightnessFilter));
		EMPluginRepository::Instance() -> Add(opPluginInformation);
		
		opPluginInformation = EM_new EMNativePluginEntry("EM Contrast", EM_new EMSimpleVideoEffectPlugin(EM_new EMContrastFilter));
		EMPluginRepository::Instance() -> Add(opPluginInformation);

		opPluginInformation = EM_new EMNativePluginEntry("EM Negative", EM_new EMSimpleVideoEffectPlugin(EM_new EMNegativeFilter));
		EMPluginRepository::Instance() -> Add(opPluginInformation);

		opPluginInformation = EM_new EMNativePluginEntry("EM RGB", EM_new EMSimpleVideoEffectPlugin(EM_new EMRGBFilter));
		EMPluginRepository::Instance() -> Add(opPluginInformation);

		opPluginInformation = EM_new EMNativePluginEntry("EM HSV", EM_new EMSimpleVideoEffectPlugin(EM_new EMHSVFilter));
		EMPluginRepository::Instance() -> Add(opPluginInformation);

		opPluginInformation = EM_new EMNativePluginEntry("EM Fader", EM_new EMVideoClipMixer());
		EMPluginRepository::Instance() -> Add(opPluginInformation);

		opPluginInformation = EM_new EMNativePluginEntry("EM Empty", EM_new EMNoTransition());
		EMPluginRepository::Instance() -> Add(opPluginInformation);

		opPluginInformation = EM_new EMNativePluginEntry("EM Barn", EM_new EMBarnTransition());
		EMPluginRepository::Instance() -> Add(opPluginInformation);

		opPluginInformation = EM_new EMNativePluginEntry("EM Wipe", EM_new EMWipeTransition());
		EMPluginRepository::Instance() -> Add(opPluginInformation);

		opPluginInformation = EM_new EMNativePluginEntry("EM Equalizer", EM_new EMEqualizerPlugin());
		EMPluginRepository::Instance() -> Add(opPluginInformation);

	/*	opPluginInformation = EM_new EMNativePluginEntry("EM Reverse", EM_new EMDestructiveReverse);
		EMPluginRepository::Instance() -> Add(opPluginInformation);

		opPluginInformation = EM_new EMNativePluginEntry("EM Crop", EM_new EMDestructiveCrop);
		EMPluginRepository::Instance() -> Add(opPluginInformation);

		opPluginInformation = EM_new EMNativePluginEntry("EM Silence", EM_new EMDestructiveSilence);
		EMPluginRepository::Instance() -> Add(opPluginInformation);

		opPluginInformation = EM_new EMNativePluginEntry("EM Normalize", EM_new EMDestructiveNormalize);
		EMPluginRepository::Instance() -> Add(opPluginInformation);

		opPluginInformation = EM_new EMNativePluginEntry("EM Reverse", EM_new EMDestructiveFadeIn);
		EMPluginRepository::Instance() -> Add(opPluginInformation);

		opPluginInformation = EM_new EMNativePluginEntry("EM Reverse", EM_new EMDestructiveFadeOut);
		EMPluginRepository::Instance() -> Add(opPluginInformation);*/
	} catch(...)
	{
		MessageBox(NULL,"InitializeStaticPlugins() Failed", "FAILED", NULL);
	}

	return true;
}

//Sets up the native Steinberg VST2 plugins...
bool EMWinMediaSystemInspector::InitializeVSTPlugins()
{
	return true;
}

bool EMWinMediaSystemInspector::InitializeChromaTransitions()
{
/*	HRESULT hr;

	unsigned long vJunk;
	bool vFound;

	ICreateDevEnum*  opISysDevEnum = NULL;
	IBaseFilter*     opIFilter    = NULL;
	IEnumMoniker*    opIEnumCat   = NULL;
	IMoniker*        opIMoniker   = NULL;
	IDxtKey*		 opIDxtK	  = NULL;

//	string oFilterName = "SMPTE Wipe"; //((EMWinMediaUtility*) EMMediaUtility::Instance()) -> UnicodeToString(uvCategoryName);

	hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC, IID_ICreateDevEnum, (void **)&opISysDevEnum);
	opISysDevEnum->CreateClassEnumerator(CLSID_VideoEffects2Category, &opIEnumCat, 0);

	vFound = false;

	while(opIEnumCat->Next(1, &opIMoniker, &vJunk) == S_OK && !vFound)
	{
		IBindCtx* opIBcK = NULL;

		string str;
		{//DEBUG - thingy
			IPropertyBag* opIPropBag = NULL;
			hr = opIMoniker -> BindToStorage(0, 0, IID_IPropertyBag, (void**) &opIPropBag);

			if(hr != S_OK)
				EMDebugger("ERROR! Could not CoCreateInstance of COM object, using the CLSID reported by the system inspector!");

			VARIANT var;
			var.vt = VT_BSTR;

			hr = opIPropBag -> Read(L"FriendlyName",&var,NULL);

			str = static_cast<EMWinMediaUtility*>(EMMediaUtility::Instance()) -> UnicodeToString(var.bstrVal);

			opIPropBag -> Release();

			eo << "EMWinMediaSystemInspector::InitializeChromaTransitions - " << str.c_str() << ef;
		}

		CreateBindCtx(0, &opIBcK);
		hr = opIMoniker -> BindToObject(opIBcK,NULL, IID_IDxtKey, (void **)&opIDxtK);
		opIBcK -> Release();
		opIBcK = NULL;

		if (hr == S_OK)
		{
			opIDxtK -> Release();
			opIDxtK = NULL;

			vFound = true;
			
			EMWinTransitionEntry* opPluginInformation = EM_new EMWinChromaTransitionEntry("EM Color-key", opIMoniker, EM_TYPE_RAW_VIDEO);
			EMPluginRepository::Instance() -> Add(opPluginInformation);
		}

		opIMoniker -> Release();
		opIMoniker = NULL;
	}

	opIEnumCat->Release();
	opISysDevEnum->Release();

	return vFound;*/
	return true;
}


bool EMWinMediaSystemInspector::InitializeSMPTETransitions()
{
	HRESULT hr;

	int i;
	unsigned long vJunk;
	bool vFound;

	ICreateDevEnum* opISysDevEnum = NULL;
	IBaseFilter*    opIFilter   = NULL;
	IEnumMoniker*   opIEnumCat  = NULL;
	IMoniker*       opIMoniker  = NULL;
	IDxtJpeg*		opIDxtJ		= NULL;
	IDxtKey*		opIDxtK		= NULL;
	IDirectDraw7*	opIDD		= NULL;

	hr = CoCreateInstance(CLSID_DirectDraw, NULL, CLSCTX_INPROC, IID_IDirectDraw7, (void**)&opIDD);

//	hr = DirectDrawCreateEx(NULL, (void**)&opIDD, IID_IDirectDraw7, NULL);
   
	if(hr != S_OK)
		EMDebugger("Error: Failed to create DirectDraw object");

//	GUID vDummy = IID_IDirectDraw7;
	hr = opIDD -> Initialize(NULL);

	hr = opIDD -> SetCooperativeLevel(NULL, DDSCL_NORMAL | DDSCL_MULTITHREADED );


	hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC, IID_ICreateDevEnum, (void **)&opISysDevEnum);
	opISysDevEnum->CreateClassEnumerator(CLSID_VideoEffects2Category, &opIEnumCat, 0);

	vFound = false;

	while(opIEnumCat -> Next(1, &opIMoniker, &vJunk) == S_OK && !vFound)
	{
		IBindCtx* opIBcJ = NULL;

		CreateBindCtx(0, &opIBcJ);
		hr = opIMoniker -> BindToObject(opIBcJ,NULL,IID_IBaseFilter, (void **)&opIFilter);
		opIBcJ -> Release();
		opIBcJ = NULL;

		if (hr == S_OK)
		{	
			if(opIFilter -> QueryInterface(IID_IDxtJpeg, (void**)&opIDxtJ) == S_OK)
			{
				opIDxtJ -> Release();
				opIDxtJ = NULL;
			
				for (i = 0; g_SMPTEName[i].m_vNumber != -1; ++i)
				{		
					EMWinTransitionEntry* opPluginInformation = EM_new EMWinSMPTETransitionEntry(g_SMPTEName[i].m_vName, opIMoniker, opIDD, EM_TYPE_RAW_VIDEO, g_SMPTEName[i].m_vNumber);
					EMPluginRepository::Instance() -> Add(opPluginInformation);
				}
			} 
			else if(opIFilter ->QueryInterface(IID_IDxtKey, (void**)&opIDxtK) == S_OK)
			{
				opIDxtK -> Release();
				opIDxtK = NULL;
			
				EMWinTransitionEntry* opPluginInformation = EM_new EMWinChromaTransitionEntry("EM Color-key", opIMoniker, opIDD, EM_TYPE_RAW_VIDEO);
				EMPluginRepository::Instance() -> Add(opPluginInformation);
			}
			else
			{
				IBindCtx* opIBc2 = NULL;

				string str;
				IPropertyBag* opIPropBag2 = NULL;
				hr = opIMoniker -> BindToStorage(0, 0, IID_IPropertyBag, (void**) &opIPropBag2);

				if(hr != S_OK)
					EMDebugger("ERROR! Could not CoCreateInstance of COM object, using the CLSID reported by the system inspector!");

				VARIANT var;
				var.vt = VT_BSTR;

				hr = opIPropBag2 -> Read(L"FriendlyName",&var,NULL);

				str = static_cast<EMWinMediaUtility*>(EMMediaUtility::Instance()) -> UnicodeToString(var.bstrVal);

				SysFreeString(var.bstrVal);
   
				opIPropBag2 -> Release();

				EMWinTransitionEntry* opPluginInformation = EM_new EMWinDXTransitionEntry(str, opIMoniker, opIDD, EM_TYPE_RAW_VIDEO);
				EMPluginRepository::Instance() -> Add(opPluginInformation);

			}

			opIFilter -> Release();
			opIFilter = NULL;
		}

		opIMoniker -> Release();
		opIMoniker = NULL;
	}

	opIEnumCat->Release();
	opISysDevEnum->Release();

	opIDD -> Release();

	return vFound;
}

string EMWinMediaSystemInspector::GetType(const AM_MEDIA_TYPE* p_vpType)
{
	GUID vMajor = p_vpType -> majortype;
	GUID vMinor = p_vpType -> subtype;

	string oMajor = "";
	string oMinor = "";

	
	if(vMajor == MEDIATYPE_Video){
		oMajor = "MEDIATYPE_Video";
	}
	if(vMajor == MEDIATYPE_Audio){
		oMajor = "MEDIATYPE_Audio";
	}
	if(vMajor == MEDIATYPE_Text){
		oMajor = "MEDIATYPE_Text";
	}
	if(vMajor == MEDIATYPE_Midi){
		oMajor = "MEDIATYPE_Midi";
	}
	if(vMajor == MEDIATYPE_Stream){
		oMajor = "MEDIATYPE_Stream";
	}
	if(vMajor == MEDIATYPE_Interleaved){
		oMajor = "MEDIATYPE_Interleaved";
	}
	if(vMajor == MEDIATYPE_File){
		oMajor = "MEDIATYPE_File";
	}
	if(vMajor == MEDIATYPE_ScriptCommand){
		oMajor = "MEDIATYPE_ScriptCommand";
	}
	if(vMajor == MEDIATYPE_AUXLine21Data){
		oMajor = "MEDIATYPE_AUXLine21Data";
	}
	if(vMajor == MEDIATYPE_Timecode){
		oMajor = "MEDIATYPE_Timecode";
	}
	if(vMajor == MEDIATYPE_LMRT){
		oMajor = "MEDIATYPE_LMRT";
	}
	if(vMajor == MEDIATYPE_URL_STREAM){
		oMajor = "MEDIATYPE_URL_STREAM";
	}
	if(vMajor == MEDIATYPE_AnalogVideo){
		oMajor = "MEDIATYPE_AnalogVideo";
	}
	if(vMajor == MEDIATYPE_AnalogAudio){
		oMajor = "MEDIATYPE_AnalogAudio";
	}
	if(oMajor == "")
	{
		oMajor = "Unknown major type";
	}


	if(vMinor == MEDIASUBTYPE_CLPL){
		oMinor = "MEDIASUBTYPE_CLPL";
		}
	if(vMinor == MEDIASUBTYPE_YUYV){
		oMinor = "MEDIASUBTYPE_YUYV";
		}
	if(vMinor == MEDIASUBTYPE_IYUV){
		oMinor = "MEDIASUBTYPE_IYUV";
		}
	if(vMinor == MEDIASUBTYPE_YVU9){
		oMinor = "MEDIASUBTYPE_YVU9";
		}
	if(vMinor == MEDIASUBTYPE_Y411){
		oMinor = "MEDIASUBTYPE_Y411";
		}
	if(vMinor == MEDIASUBTYPE_Y41P){
		oMinor = "MEDIASUBTYPE_Y41P";
		}
	if(vMinor == MEDIASUBTYPE_YUY2){
		oMinor = "MEDIASUBTYPE_YUY2";
		}
	if(vMinor == MEDIASUBTYPE_YVYU){
		oMinor = "MEDIASUBTYPE_YVYU";
		}
	if(vMinor == MEDIASUBTYPE_UYVY){
		oMinor = "MEDIASUBTYPE_UYVY";
		}
	if(vMinor == MEDIASUBTYPE_Y211){
		oMinor = "MEDIASUBTYPE_Y211";
		}
	if(vMinor == MEDIASUBTYPE_YV12){
		oMinor = "MEDIASUBTYPE_YV12";
		}
	if(vMinor == MEDIASUBTYPE_CLJR){
		oMinor = "MEDIASUBTYPE_CLJR";
		}
	if(vMinor == MEDIASUBTYPE_IF09){
		oMinor = "MEDIASUBTYPE_IF09";
		}
	if(vMinor == MEDIASUBTYPE_CPLA){
		oMinor = "MEDIASUBTYPE_CPLA";
		}
	if(vMinor == MEDIASUBTYPE_MJPG){
		oMinor = "MEDIASUBTYPE_MJPG";
		}
	if(vMinor == MEDIASUBTYPE_TVMJ){
		oMinor = "MEDIASUBTYPE_TVMJ";
		}
	if(vMinor == MEDIASUBTYPE_WAKE){
		oMinor = "MEDIASUBTYPE_WAKE";
		}
	if(vMinor == MEDIASUBTYPE_CFCC){
		oMinor = "MEDIASUBTYPE_CFCC";
		}
	if(vMinor == MEDIASUBTYPE_IJPG){
		oMinor = "MEDIASUBTYPE_IJPG";
		}
	if(vMinor == MEDIASUBTYPE_Plum){
		oMinor = "MEDIASUBTYPE_Plum";
		}
	if(vMinor == MEDIASUBTYPE_DVCS){
		oMinor = "MEDIASUBTYPE_DVCS";
		}
	if(vMinor == MEDIASUBTYPE_DVSD){
		oMinor = "MEDIASUBTYPE_DVSD";
		}
	if(vMinor == MEDIASUBTYPE_MDVF){
		oMinor = "MEDIASUBTYPE_MDVF";
		}
	if(vMinor == MEDIASUBTYPE_RGB1){
		oMinor = "MEDIASUBTYPE_RGB1";
		}
	if(vMinor == MEDIASUBTYPE_RGB4){
		oMinor = "MEDIASUBTYPE_RGB4";
		}
	if(vMinor == MEDIASUBTYPE_RGB8){
		oMinor = "MEDIASUBTYPE_RGB8";
		}
	if(vMinor == MEDIASUBTYPE_RGB565){
		oMinor = "MEDIASUBTYPE_RGB565";
		}
	if(vMinor == MEDIASUBTYPE_RGB24){
		oMinor = "MEDIASUBTYPE_RGB24";
		}
	if(vMinor == MEDIASUBTYPE_RGB32){
		oMinor = "MEDIASUBTYPE_RGB32";
		}
	if(vMinor == MEDIASUBTYPE_ARGB32){
		oMinor = "MEDIASUBTYPE_ARGB32";
		}
	if(vMinor == MEDIASUBTYPE_Overlay){
		oMinor = "MEDIASUBTYPE_Overlay";
		}
	if(vMinor == MEDIASUBTYPE_MPEG1Packet){
		oMinor = "MEDIASUBTYPE_MPEG1Packet";
		}
	if(vMinor == MEDIASUBTYPE_MPEG1Payload){
		oMinor = "MEDIASUBTYPE_MPEG1Payload";
		}
	if(vMinor == MEDIASUBTYPE_MPEG1AudioPayload){
		oMinor = "MEDIASUBTYPE_MPEG1AudioPayload";
		}
	if(vMinor == MEDIATYPE_MPEG1SystemStream){
		oMinor = "MEDIATYPE_MPEG1SystemStream";
		}
	if(vMinor == MEDIASUBTYPE_MPEG1System){
		oMinor = "MEDIASUBTYPE_MPEG1System";
		}
	if(vMinor == MEDIASUBTYPE_MPEG1VideoCD){
		oMinor = "MEDIASUBTYPE_MPEG1VideoCD";
		}
	if(vMinor == MEDIASUBTYPE_MPEG1Video){
		oMinor = "MEDIASUBTYPE_MPEG1Video";
		}
	if(vMinor == MEDIASUBTYPE_MPEG1Audio){
		oMinor = "MEDIASUBTYPE_MPEG1Audio";
		}
	if(vMinor == MEDIASUBTYPE_Avi){
		oMinor = "MEDIASUBTYPE_Avi";
		}
	if(vMinor == MEDIASUBTYPE_Asf){
		oMinor = "MEDIASUBTYPE_Asf";
		}
	if(vMinor == MEDIASUBTYPE_QTMovie){
		oMinor = "MEDIASUBTYPE_QTMovie";
		}
	if(vMinor == MEDIASUBTYPE_QTRpza){
		oMinor = "MEDIASUBTYPE_QTRpza";
		}
	if(vMinor == MEDIASUBTYPE_QTSmc){
		oMinor = "MEDIASUBTYPE_QTSmc";
		}
	if(vMinor == MEDIASUBTYPE_QTRle){
		oMinor = "MEDIASUBTYPE_QTRle";
		}
	if(vMinor == MEDIASUBTYPE_QTJpeg){
		oMinor = "MEDIASUBTYPE_QTJpeg";
		}
	if(vMinor == MEDIASUBTYPE_PCMAudio_Obsolete){
		oMinor = "MEDIASUBTYPE_PCMAudio_Obsolete";
		}
	if(vMinor == MEDIASUBTYPE_PCM){
		oMinor = "MEDIASUBTYPE_PCM";
		}
	if(vMinor == MEDIASUBTYPE_WAVE){
		oMinor = "MEDIASUBTYPE_WAVE";
		}
	if(vMinor == MEDIASUBTYPE_AU){
		oMinor = "MEDIASUBTYPE_AU";
		}
	if(vMinor == MEDIASUBTYPE_AIFF){
		oMinor = "MEDIASUBTYPE_AIFF";
		}
	if(vMinor == MEDIASUBTYPE_dvsd){
		oMinor = "MEDIASUBTYPE_dvsd";
		}
	if(vMinor == MEDIASUBTYPE_dvhd){
		oMinor = "MEDIASUBTYPE_dvhd";
		}
	if(vMinor == MEDIASUBTYPE_dvsl){
		oMinor = "MEDIASUBTYPE_dvsl";
		}
	if(vMinor == MEDIASUBTYPE_Line21_BytePair){
		oMinor = "MEDIASUBTYPE_Line21_BytePair";
		}
	if(vMinor == MEDIASUBTYPE_Line21_GOPPacket){
		oMinor = "MEDIASUBTYPE_Line21_GOPPacket";
		}
	if(vMinor == MEDIASUBTYPE_Line21_VBIRawData){
		oMinor = "MEDIASUBTYPE_Line21_VBIRawData";
		}
	if(vMinor == MEDIASUBTYPE_DRM_Audio){
		oMinor = "MEDIASUBTYPE_DRM_Audio";
		}
	if(vMinor == MEDIASUBTYPE_IEEE_FLOAT){
		oMinor = "MEDIASUBTYPE_IEEE_FLOAT";
		}
	if(vMinor == MEDIASUBTYPE_DOLBY_AC3_SPDIF){
		oMinor = "MEDIASUBTYPE_DOLBY_AC3_SPDIF";
		}
	if(vMinor == MEDIASUBTYPE_RAW_SPORT){
		oMinor = "MEDIASUBTYPE_RAW_SPORT";
		}
	if(vMinor == MEDIASUBTYPE_SPDIF_TAG_241h){
		oMinor = "MEDIASUBTYPE_SPDIF_TAG_241h";
		}
	if(vMinor == MEDIASUBTYPE_DssVideo){
		oMinor = "MEDIASUBTYPE_DssVideo";
		}
	if(vMinor == MEDIASUBTYPE_DssAudio){
		oMinor = "MEDIASUBTYPE_DssAudio";
		}
	if(vMinor == MEDIASUBTYPE_VPVideo){
		oMinor = "MEDIASUBTYPE_VPVideo";
		}
	if(vMinor == MEDIASUBTYPE_VPVBI){
		oMinor = "MEDIASUBTYPE_VPVBI";
		}
	if(vMinor == MEDIASUBTYPE_AnalogVideo_NTSC_M){
		oMinor = "MEDIASUBTYPE_AnalogVideo_NTSC_M";
		}
	if(vMinor == MEDIASUBTYPE_AnalogVideo_PAL_B){
		oMinor = "MEDIASUBTYPE_AnalogVideo_PAL_B";
		}
	if(vMinor == MEDIASUBTYPE_AnalogVideo_PAL_D){
		oMinor = "MEDIASUBTYPE_AnalogVideo_PAL_D";
		}
	if(vMinor == MEDIASUBTYPE_AnalogVideo_PAL_G){
		oMinor = "MEDIASUBTYPE_AnalogVideo_PAL_G";
		}
	if(vMinor == MEDIASUBTYPE_AnalogVideo_PAL_H){
		oMinor = "MEDIASUBTYPE_AnalogVideo_PAL_H";
		}
	if(vMinor == MEDIASUBTYPE_AnalogVideo_PAL_I){
		oMinor = "MEDIASUBTYPE_AnalogVideo_PAL_I";
		}
	if(vMinor == MEDIASUBTYPE_AnalogVideo_PAL_M){
		oMinor = "MEDIASUBTYPE_AnalogVideo_PAL_M";
		}
	if(vMinor == MEDIASUBTYPE_AnalogVideo_PAL_N){
		oMinor = "MEDIASUBTYPE_AnalogVideo_PAL_N";
		}
	if(vMinor == MEDIASUBTYPE_AnalogVideo_PAL_N_COMBO){
		oMinor = "MEDIASUBTYPE_AnalogVideo_PAL_N_COMBO";
		}
	if(vMinor == MEDIASUBTYPE_AnalogVideo_SECAM_B){
		oMinor = "MEDIASUBTYPE_AnalogVideo_SECAM_B";
		}
	if(vMinor == MEDIASUBTYPE_AnalogVideo_SECAM_D){
		oMinor = "MEDIASUBTYPE_AnalogVideo_SECAM_D";
		}
	if(vMinor == MEDIASUBTYPE_AnalogVideo_SECAM_G){
		oMinor = "MEDIASUBTYPE_AnalogVideo_SECAM_G";
		}
	if(vMinor == MEDIASUBTYPE_AnalogVideo_SECAM_H){
		oMinor = "MEDIASUBTYPE_AnalogVideo_SECAM_H";
		}
	if(vMinor == MEDIASUBTYPE_AnalogVideo_SECAM_K){
		oMinor = "MEDIASUBTYPE_AnalogVideo_SECAM_K";
		}
	if(vMinor == MEDIASUBTYPE_AnalogVideo_SECAM_K1){
		oMinor = "MEDIASUBTYPE_AnalogVideo_SECAM_K1";
		}
	if(vMinor == MEDIASUBTYPE_AnalogVideo_SECAM_L){
		oMinor = "MEDIASUBTYPE_AnalogVideo_SECAM_L";
		}
	if(oMinor == "")
	{
		oMinor = "Unknown minor type";
	}

	return oMajor + string(" / ") + oMinor;
}


bool EMWinMediaSystemInspector::IsFlagEnabled(EMCommandLineIDs p_vID)
{
	return (m_vDisableFeaturesFlags & p_vID) == 0;
}

BOOL CALLBACK EMWinMediaSystemInspector::DirectSoundAudioDeviceEnumeration(LPGUID lpGUID, LPCTSTR lpszDesc, LPCTSTR lpszDrvName, LPVOID lpContext)
{
	list<EMGUIDNameMap*>* opMapList = (list<EMGUIDNameMap*>*) lpContext;
	if(lpGUID != NULL && *lpGUID != GUID_NULL)
	{
		EMGUIDNameMap* opMap = EM_new EMGUIDNameMap();
		opMap -> m_oName = string(lpszDesc);
		memcpy(&(opMap -> m_uGUID), lpGUID, sizeof(GUID));
		opMapList -> push_back(opMap);
	}
	return TRUE; 
}

#endif

