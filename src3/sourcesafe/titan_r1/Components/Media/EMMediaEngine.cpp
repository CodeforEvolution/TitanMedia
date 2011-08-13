#include "EMMediaEngine.h"

#include "CommandIDs.h"
#include "EMAudioExtractor.h"
#include "EMBeMediaRenderingNode.h" 
#include "EMBeVideoConsumerNode.h"
#include "EMBitmapBuffer.h"
#include "EMCommandRepository.h"  
#include "EMEncoderRepository.h"
#include "EMMediaClipRepository.h"
#include "EMMediaCommandFactory.h" 
#include "EMMediaDebugLog.h"
#include "EMMediaEffectTrack.h"
#include "EMMediaEffectTrackRepository.h"
#include "EMMediaEngine_LibraryVersion.h"
#include "EMMediaMetronome.h"
#include "EMMediaPool.h"
#include "EMMediaProject.h"
#include "EMMediaSystemInspector.h"
#include "EMMediaSystemInspector.h"
#include "EMMediaTimer.h"
#include "EMMediaTrack.h"
#include "EMMediaTrackRepository.h"  
#include "EMMediaUtility.h"
#include "EMOutputRepository.h"
#include "EMPlugin.h"
#include "EMPluginRepository.h"
#include "EMProjectDataContainer.h"
#include "EMProjectDataLoader.h"
#include "EMProjectDataSaver.h" 
#include "EMSemaphore.h" 
#include "EMSettingIDs.h"
#include "EMSettingsRepository.h"
#include "EMVideoPreview.h"
#include "EMVideoSynchronizer.h"

#ifdef PLATFORM_WINDOWS
#include "EMWinAudioDiskWriter.h"
#include "EMWinDXMIDIConsumer.h"
#include "EMWinDXMIDIProducer.h"
#include "EMWinDXPluginEngineFilter.h"
#include "EMWinMediaInputFilter.h"
#include "EMWinMediaSingletonMuxer.h"
#include "EMWinMIDISystem.h"
#include "EMWinMIDISystem.h"
#include "EMWinVideoShuffleOutputFilter.h"

#include <dplay8.h>
#include <objbase.h>
#endif

extern "C" __declspec(dllexport) void InstanceMediaEngine()
{
	EMMediaDebugLog::Instance();
	EMMediaEngine::Instance();
};

extern "C" __declspec(dllexport) bool MediaSystemCheck(char* p_vpErrorMessage)
{
	return EMMediaEngine::Instance() -> MediaSystemCheck(p_vpErrorMessage);
};

extern "C" __declspec(dllexport) void InitializeMediaEngine(EMSettingsRepository* p_opSettingsRepository, EMCommandRepository* p_opCommandRepository)
{
	eo << "InitializeMediaEngine called by " << (uint32) GetCurrentThreadId() << ef;

	EMMediaEngine::Instance() -> SetSettingsRepository(p_opSettingsRepository); 

	eo << "Setting address is " << (int) p_opSettingsRepository ->GetSetting(SETTING_AUDIO_FRAMERATE) << ef;
//SetVideoPreviewWindow(EMVideoPreview* p_opVideoPreviewWindow)	

	EMMediaEngine::Instance() -> SetCommandRepository(p_opCommandRepository);

	if(p_opSettingsRepository == NULL)
		MessageBox(NULL, "ERROR! SettingsRep is NULL!", "", MB_OK);

	EMMediaEngine::Instance() -> InstanceOther();
};

extern "C" __declspec(dllexport) void DeleteMediaEngine() 
{ 
	EMMediaEngine::Delete(); 
	EMMediaDebugLog::Delete();
};

EMMediaEngine* EMMediaEngine::m_opInstance = NULL;
bool EMMediaEngine::m_vIsDecomposed = false;

EMMediaEngine::EMMediaEngine()
	:	m_opProject(NULL),
		m_opSettingsRepository(NULL),
		m_opVideoPreviewWindow(NULL),
		m_opCommandRepository(NULL),
		m_opProjectProtectionSemaphore(NULL)
{
	EMMediaDebugLog::Instance() -> Log("Entering EMMediaEngine constructor");
	eo << "EMMediaEngine::EMMediaEngine() called by " << (uint32) GetCurrentThreadId() << ef;
	eo << "MediaEngine_Library v0.1Win, Build " << __EMMediaEngine_LibraryVersion << " by " << __EMMediaEngine_LibraryVersion_Owner << " (C) Elegant Media AB 2000-2001" << ef;
	EMMediaDebugLog::Instance() -> Log("Leaving EMMediaEngine constructor");
}

bool CheckComponent(GUID p_uID)
{
	EMMediaDebugLog::Instance() -> Log("Entering CheckComponent");
	IUnknown* opDummyObject = NULL;
	HRESULT vResult = CoCreateInstance(p_uID, NULL, CLSCTX_INPROC, IID_IUnknown, (void**) &opDummyObject);
	if(FAILED(vResult))
	{
		EMMediaDebugLog::Instance() -> Log("CheckComponent failed!");
		return false;
	}
	opDummyObject -> Release();
	EMMediaDebugLog::Instance() -> Log("Leaving CheckComponent");
	return true;
}

//TODO: Fix so we throw an exception if something fails here.... 
EMMediaEngine* EMMediaEngine::Instance()
{
	if(m_opInstance != NULL)
		return m_opInstance;

	if(m_vIsDecomposed)
		EMDebugger("ERROR! EMMediaEngine already dead!");
	if(m_opInstance == NULL)
		m_opInstance = EM_new EMMediaEngine();

	EMMediaEngine* opInstance = m_opInstance;
	return opInstance;
}

bool EMMediaEngine::MediaSystemCheck(char* p_vpErrorMessage)
{
	return true;
	EMMediaDebugLog::Instance() -> Log("Entering EMMediaEngine::MediaSystemCheck");
	//We can only run on Windows 98 Second or Millenium Edition and on Windows 2000. So we need to check that..
	//TODO: Implement OS version check!

	//Media engine needs at least DirectX 8.0
	//However, there is no direct way of fetching the DX version, so try to instantiate a DirectX COM object 
	//that was introduced in DirectX 8.
#ifdef PLATFORM_WINDOWS
	if(! CheckComponent(CLSID_DirectPlay8Client))
	{
		strcpy(p_vpErrorMessage, "DirectX version 8 or higher is not present on your system. Please install at least DirectX 8.0");
		EMMediaDebugLog::Instance() -> Log("Failed: DirectX version 8 or higher is not present on your system. Please install at least DirectX 8.0");
		return false;
	}
	if(! CheckComponent(CLSID_FilterGraph))
	{
		strcpy(p_vpErrorMessage, "DirectShow (version 8 or higher) is not present on your system. Please install DirectShow from the DirectX 8.0 (or later) package");
		EMMediaDebugLog::Instance() -> Log("Failed: DirectShow (version 8 or higher) is not present on your system. Please install DirectShow from the DirectX 8.0 (or later) package");
		return false;
	}
#endif

	//Titan comes with alot of different components (COM objects). So, check their existance in the system, 
	//and also check their version numbers (latter not impl. yet)
	//TODO: Implement component check!
#ifdef PLATFORM_WINDOWS
	if(! CheckComponent(CLSID_EMWinDXPluginEngineFilter))
	{
		strcpy(p_vpErrorMessage, "Could not locate one of the media components for Titan (the objects that come with EMWinDXPluginEngine.dll)");
		EMMediaDebugLog::Instance() -> Log("Failed: Could not locate one of the media components for Titan (the objects that come with EMWinDXPluginEngine.dll)");
		return false;
	}
	if(! CheckComponent(CLSID_EMWinMediaInputFilter))
	{
		strcpy(p_vpErrorMessage, "Could not locate one of the media components for Titan (the objects that come with EMWinMediaInputFilter.dll)");
		EMMediaDebugLog::Instance() -> Log("Failed: Could not locate one of the media components for Titan (the objects that come with EMWinMediaInputFilter.dll)");
		return false;
	}

///*if(! CheckComponent(CLSID_EMWinMediaVideoOutputFilter))
//	{
//		strcpy(p_vpErrorMessage, "Could not locate one of the media components for Titan (the objects that come with EMWinMediaOutputFilter.dll)");
//		return false;
//	}
//*/
///*	if(! CheckComponent(CLSID_EMWinVideoShuffleOutputFilter))
//	{
//		strcpy(p_vpErrorMessage, "Could not locate one of the media components for Titan (the objects that come with EMWinVideoShuffleOutputFilter.dll)");
//		return false;
//	} */


	if(! CheckComponent(CLSID_EMWinAudioConverterFilter))
	{
		strcpy(p_vpErrorMessage, "Could not locate one of the media components for Titan (the objects that come with EMWinAudioConverterFilter.dll)");
		EMMediaDebugLog::Instance() -> Log("Failed: Could not locate one of the media components for Titan (the objects that come with EMWinAudioConverterFilter.dll)");
		return false;
	} 

	if(! CheckComponent(CLSID_EMReadFrame))
	{
		strcpy(p_vpErrorMessage, "Could not locate one of the media components for Titan (the objects that come with EMReadFrame.dll)");
		EMMediaDebugLog::Instance() -> Log("Failed: Could not locate one of the media components for Titan (the objects that come with EMReadFrame.dll)");
		return false;
	}
	if(! CheckComponent(CLSID_EMWaveWriter))
	{
		strcpy(p_vpErrorMessage, "Could not locate one of the media components for Titan (the objects that come with EMWaveWriter.dll)");
		EMMediaDebugLog::Instance() -> Log("Failed: Could not locate one of the media components for Titan (the objects that come with EMWaveWriter.dll)");
		return false;
	}

#endif

	EMMediaDebugLog::Instance() -> Log("Leaving EMMediaEngine::MediaSystemCheck");
	return true;
}

void EMMediaEngine::InstanceOther()
{
	EMMediaDebugLog::Instance() -> Log("Entering EMMediaEngine::InstanceOther");
	if(m_opCommandRepository != NULL)
	{
		EMMediaDebugLog::Instance() -> Log("EMMediaEngine::InstanceOther() ");
		m_opCommandRepository -> ExecuteCommand(PROJECT_COMMAND_ADD_PROJECT_DATA_CONTAINER, dynamic_cast<EMProjectDataContainer*>(this));
	}
	else
		EMMediaDebugLog::Instance() -> Log("EMMediaEngine::InstanceOther() No command repository so can't become ProjectDataContainer!");
	
	EMMediaDebugLog::Instance() -> Log("EMMediaEngine::InstanceOther() Initializing System Inspector...");
	m_opCommandRepository -> ExecuteCommand(COMMAND_DISPLAY_SPLASH_SCREEN_TEXT, (void*) "Initializing System Inspector...", NULL, NULL);
	EMMediaSystemInspector::Create();
	EMMediaDebugLog::Instance() -> Log("EMMediaEngine::InstanceOther() Initializing Media Timer...");
	m_opCommandRepository -> ExecuteCommand(COMMAND_DISPLAY_SPLASH_SCREEN_TEXT, (void*) "Initializing Media Timer...", NULL, NULL);
	EMMediaTimer::Instance(); //2 threads
	EMMediaDebugLog::Instance() -> Log("EMMediaEngine::InstanceOther() Initializing Plugin Engine...");
	m_opCommandRepository -> ExecuteCommand(COMMAND_DISPLAY_SPLASH_SCREEN_TEXT, (void*) "Initializing Plugin Engine...", NULL, NULL);
	EMMediaSystemInspector::GetInstance() -> InitializePluginSystem(); //One thread created, and then killed

	m_opCommandRepository -> ExecuteCommand(COMMAND_DISPLAY_SPLASH_SCREEN_TEXT, (void*) "Initializing Audio-extractor...", NULL, NULL);
	EMAudioExtractor::Instance();

	EMMediaDebugLog::Instance() -> Log("EMMediaEngine::InstanceOther() Initializing Muxer...");
	m_opCommandRepository -> ExecuteCommand(COMMAND_DISPLAY_SPLASH_SCREEN_TEXT, (void*) "Initializing Muxer...", NULL, NULL);
	EMWinMediaSingletonMuxer::Instance(); //1 thread

	EMMediaDebugLog::Instance() -> Log("EMMediaEngine::InstanceOther() Initializing I/O...");
	m_opCommandRepository -> ExecuteCommand(COMMAND_DISPLAY_SPLASH_SCREEN_TEXT, (void*) "Initializing I/O...", NULL, NULL);
	EMOutputRepository::Instance() -> InitCheckE();
	EMMediaDebugLog::Instance() -> Log("EMMediaEngine::InstanceOther() Now Adding lots of User Value...");
	m_opCommandRepository -> ExecuteCommand(COMMAND_DISPLAY_SPLASH_SCREEN_TEXT, (void*) "Now Adding lots of User Value...", NULL, NULL);
	EMInputRepository::Instance() -> InitCheckE();
	EMMediaDebugLog::Instance() -> Log("EMMediaEngine::InstanceOther() Initializing Encoder Repository...");
	m_opCommandRepository -> ExecuteCommand(COMMAND_DISPLAY_SPLASH_SCREEN_TEXT, (void*) "Initializing Encoder Repository...", NULL, NULL);
	EMEncoderRepository::Instance() -> InitCheckE();
	EMMediaDebugLog::Instance() -> Log("EMMediaEngine::InstanceOther() Initializing Destination Repository...");
	m_opCommandRepository -> ExecuteCommand(COMMAND_DISPLAY_SPLASH_SCREEN_TEXT, (void*) "Initializing Destination Repository...", NULL, NULL);
	EMMediaDebugLog::Instance() -> Log("EMMediaEngine::InstanceOther() Initializing Video Syncronizer...");
	m_opCommandRepository -> ExecuteCommand(COMMAND_DISPLAY_SPLASH_SCREEN_TEXT, (void*) "Initializing Video Syncronizer...", NULL, NULL);
	EMVideoSynchronizer::Instance(); //1 thread
	EMMediaDebugLog::Instance() -> Log("EMMediaEngine::InstanceOther() Initializing MIDI Consumer...");
	m_opCommandRepository -> ExecuteCommand(COMMAND_DISPLAY_SPLASH_SCREEN_TEXT, (void*) "Initializing MIDI Consumer...", NULL, NULL);
	EMWinDXMIDIConsumer::Instance();
	EMMediaDebugLog::Instance() -> Log("EMMediaEngine::InstanceOther() Initializing MIDI Producer...");
	m_opCommandRepository -> ExecuteCommand(COMMAND_DISPLAY_SPLASH_SCREEN_TEXT, (void*) "Initializing MIDI Producer...", NULL, NULL);
	EMWinDXMIDIProducer::Instance();
	EMMediaDebugLog::Instance() -> Log("EMMediaEngine::InstanceOther() Starting MIDI Engine...");
	m_opCommandRepository -> ExecuteCommand(COMMAND_DISPLAY_SPLASH_SCREEN_TEXT, (void*) "Starting MIDI Engine...", NULL, NULL);
	EMWinMIDISystem::Instance() -> StartMIDISystemE(); //TODO: Make non-native!
	EMMediaDebugLog::Instance() -> Log("EMMediaEngine::InstanceOther() Initializing Media Utility Objects...");
	m_opCommandRepository -> ExecuteCommand(COMMAND_DISPLAY_SPLASH_SCREEN_TEXT, (void*) "Initializing Media Utility Objects...", NULL, NULL);
	EMMediaUtility::Instance();
	EMMediaDebugLog::Instance() -> Log("EMMediaEngine::InstanceOther() Initializing Bitmap Buffers...");
	m_opCommandRepository -> ExecuteCommand(COMMAND_DISPLAY_SPLASH_SCREEN_TEXT, (void*) "Initializing Bitmap Buffers...", NULL, NULL);
	EMBitmapBuffer::Instance();
	EMMediaDebugLog::Instance() -> Log("EMMediaEngine::InstanceOther() Initializing MIDI Metronome...");
	m_opCommandRepository -> ExecuteCommand(COMMAND_DISPLAY_SPLASH_SCREEN_TEXT, (void*) "Initializing MIDI Metronome...", NULL, NULL);
	EMMediaMetronome::Instance(); //1 thread
	EMMediaDebugLog::Instance() -> Log("EMMediaEngine::InstanceOther() Initializing Audio Disk Writer...");
	m_opCommandRepository -> ExecuteCommand(COMMAND_DISPLAY_SPLASH_SCREEN_TEXT, (void*) "Initializing Audio Disk Writer...", NULL, NULL);
	EMWinAudioDiskWriter::Instance();

//	SetVideoPreviewWindow(static_cast<EMVideoPreview*>(GetSettingsRepository() -> GetSetting(SETTING_VIDEO_PREVIEW)));

/*	//BEGIN TEMP (Also two lines above that is commented out temporarily)

	IDirectSound8* opDS8 = NULL; //EMWinMIDISystem::Instance() ->GetDirectMusicObject();
	HRESULT vResult = CoCreateInstance(CLSID_DirectSound8, NULL, CLSCTX_INPROC, IID_IDirectSound8, (void**) &opDS8);
	if(FAILED(vResult))
		eo << "??" << ef;

	vResult = opDS8 -> Initialize(NULL);
	if(FAILED(vResult))
		eo << "??" << ef;

//	AM_MEDIA_TYPE uSystemAudioFormat;
	WAVEFORMATEX* upWaveFormatEx = EM_new WAVEFORMATEX;
	upWaveFormatEx -> wFormatTag = WAVE_FORMAT_PCM;
	upWaveFormatEx -> nChannels = 2;
	upWaveFormatEx -> nSamplesPerSec = 44100; //(uint32) *(static_cast<int32*>(m_opFilter -> GetSettingsRepository() -> GetSetting(SETTING_AUDIO_FRAMERATE)));
	upWaveFormatEx -> wBitsPerSample = 16; //EM_AUDIO_SAMPLESIZE * 8;        
	upWaveFormatEx -> nBlockAlign = static_cast<WORD>((upWaveFormatEx -> wBitsPerSample * upWaveFormatEx -> nChannels) / 8);
	upWaveFormatEx -> nAvgBytesPerSec = upWaveFormatEx -> nBlockAlign * upWaveFormatEx -> nSamplesPerSec;
	upWaveFormatEx -> cbSize = 0;
//	CreateAudioMediaType(upWaveFormatEx, &uSystemAudioFormat, true);
//	CMediaType* opCMediaType2 = EM_new CMediaType(uSystemAudioFormat);

	DSBUFFERDESC uBuffDesc;
	uBuffDesc.dwSize = sizeof(uBuffDesc);
	uBuffDesc.dwFlags = DSBCAPS_CTRLFX;
	uBuffDesc.dwBufferBytes = 32768; // (EM_AUDIO_BUFFER_SIZE < DSBSIZE_FX_MIN ? DSBSIZE_FX_MIN : (EM_AUDIO_BUFFER_SIZE > DSBSIZE_MAX ? DSBSIZE_MAX : EM_AUDIO_BUFFER_SIZE));
	uBuffDesc.lpwfxFormat = upWaveFormatEx;
	uBuffDesc.dwReserved = 0;

	IDirectSoundBuffer* upBuffer = NULL;
	vResult = opDS8 -> CreateSoundBuffer(&uBuffDesc, &upBuffer, NULL);
	if(vResult != DS_OK)
	{
		eo << "ERROR!" << ef;
	}

	IDirectSoundBuffer8* upBuffer8 = NULL;
	vResult = upBuffer -> QueryInterface(IID_IDirectSoundBuffer8, (void**) &upBuffer8);
	if(vResult != DS_OK)
	{
		eo << "ERROR!" << ef;
	}

	DSEFFECTDESC uFXDesc;
	uFXDesc.dwSize = sizeof(DSEFFECTDESC);
	uFXDesc.dwFlags = 0;
	uFXDesc.guidDSFXClass = GUID_DSFX_STANDARD_DISTORTION;
	uFXDesc.dwReserved1 = 0;
	uFXDesc.dwReserved2 = 0;
	vResult = upBuffer8 -> SetFX(1, &uFXDesc, NULL);
	if(vResult != DS_OK)
	{
		eo << "ERROR!" << ef;
	}
	exit(0); 
	//END TEMP */
	m_opCommandRepository -> ExecuteCommand(COMMAND_DISPLAY_SPLASH_SCREEN_TEXT, (void*) "Media Engine Initialized OK", NULL, NULL);
	::Sleep(250);
	EMMediaDebugLog::Instance() -> Log("Leaving EMMediaEngine::InstanceOther");
}

void EMMediaEngine::Delete()
{
	delete m_opInstance;
	m_opInstance = NULL;
	m_vIsDecomposed = true;
}

EMMediaEngine::~EMMediaEngine()
{
	delete m_opProjectProtectionSemaphore;  
	delete m_opProject;
	m_opProject = NULL;
	EMMediaMetronome::Delete();
	//EMBitmapBuffer::Delete();
	EMOutputRepository::Delete();
	EMInputRepository::Delete();
	EMMediaTimer::Delete();
	EMEncoderRepository::Delete();
	EMPluginRepository::Delete();
	EMMediaSystemInspector::DeleteAndResetInstance();
	EMWinDXMIDIConsumer::Delete();
	EMWinDXMIDIProducer::Delete();
	EMWinMIDISystem::Delete();
	delete EMWinMediaSingletonMuxer::Instance();   //<----------- OBS!!! ÄNDRA!!!
	EMAudioExtractor::Delete();
	EMVideoSynchronizer::Delete();
	EMMediaUtility::Delete();
	EMWinAudioDiskWriter::Delete();
}

EMMediaProject* EMMediaEngine::GetMediaProject() const
{
	return m_opProject;
}

EMMediaTimer* EMMediaEngine::GetMediaTimer() const 
{
	return EMMediaTimer::Instance();
}

EMMediaCommandFactory* EMMediaEngine::GetCommandFactory() const
{
	return EMMediaCommandFactory::Instance();
}

EMVideoPreview* EMMediaEngine::GetVideoPreviewWindow() const
{
	return m_opVideoPreviewWindow;
}

void EMMediaEngine::SetVideoPreviewWindow(EMVideoPreview* p_opVideoPreviewWindow)
{
	m_opVideoPreviewWindow = p_opVideoPreviewWindow;
}

void EMMediaEngine::SetSettingsRepository(EMSettingsRepository* p_opSettingsRepository)
{
	m_opSettingsRepository = p_opSettingsRepository;
}

EMSettingsRepository* EMMediaEngine::GetSettingsRepository() const
{
	return m_opSettingsRepository;
}

void EMMediaEngine::SetCommandRepository(EMCommandRepository* p_opCommandRepository)
{
	m_opCommandRepository = p_opCommandRepository;
}

EMCommandRepository* EMMediaEngine::GetCommandRepository() const
{
	return m_opCommandRepository;
}

bool EMMediaEngine::SaveData(void* p_upSaver)
{
	EMProjectDataSaver* opSaver = static_cast<EMProjectDataSaver*>(p_upSaver);

	opSaver -> SaveString("EMMediaEngine - Begin");
	if(! m_opProject -> SaveData(opSaver))
		return false;

	opSaver -> SaveString("EMMediaEngine - End");
	
	return true;
}

bool EMMediaEngine::LoadData(void* p_upLoader)												// Step 3 during project loading
{
	EMProjectDataLoader* opLoader = static_cast<EMProjectDataLoader*>(p_upLoader);
	if(opLoader ->LoadString() != string("EMMediaEngine - Begin"))
	{
		EMDebugger("ERROR! Unknown project file format! Media engine can't do anything with it.");
		return false;
	}
	
	if(m_opProject == NULL)
	{
		m_opProject = EM_new EMMediaProject();
		m_opProject -> InitCheckE();
	}

	if(! m_opProject -> LoadData(opLoader))
		return false;

	if(opLoader ->LoadString() != string("EMMediaEngine - End"))
	{
		EMDebugger("ERROR(2)! Unknown project file format! Media engine can't do anything with it.");
		return false;
	}
	
	
	return true;
}

bool EMMediaEngine::ClearData()																// Step 1 during project loading
{
	EMMediaDebugLog::Instance() -> Log("Entering EMMediaEngine::ClearData()");
	delete m_opProject;
	m_opProject = NULL;
	EMPluginRepository::Instance() -> ResetEntries();
	EMMediaSystemInspector::DeleteAndResetInstance();

	//This call (with all three parameters set to NULL) will reset the command's internal member-attributes
	//so we can handle the NewProject/CloseProject situation
	m_opCommandRepository -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_VIDEO_CLIP_CONTENT, NULL, NULL, NULL);
	EMMediaDebugLog::Instance() -> Log("Leaving EMMediaEngine::ClearData()");
	return true;
}

void EMMediaEngine::ProjectClosed()
{
}

void EMMediaEngine::PrepareToLoadData()														// Step 2 during project loading
{
//	ProjectCreated(0); //TEMP
}

void EMMediaEngine::ProjectCreated(int p_vProjectID)
{
	EMMediaDebugLog::Instance() -> Log("Entering EMMediaEngine::ProjectCreated()");
	LockProject();
	try
	{
		EMMediaTimer::Instance();

		UnlockProject();
		if(m_opProject != NULL)
			delete m_opProject;
		m_opProject = EM_new EMMediaProject();
		if(! m_opProject -> InitCheckE())
			EMDebugger("ERROR! EMMediaEngine could not initialize the new EMMediaProject!");
		LockProject();
	}
	catch(...)
	{
		EMMediaDebugLog::Instance() -> Log("Failed: Caught an exception in EMMediaEngine::ProjectCreated()");
		EMDebugger("ERROR! MediaEngine caught an exception during project post creation");
	}
	UnlockProject();
	EMMediaDebugLog::Instance() -> Log("Leaving EMMediaEngine::ProjectCreated()");
}

bool EMMediaEngine::IsDirty()
{
	if(m_opProject == NULL)
		return false;
	return m_opProject -> IsDirty();
}

void EMMediaEngine::ReadyForUse()
{
	EMMediaDebugLog::Instance() -> Log("Entering EMMediaEngine::ReadyForUse()");
	if(m_opProject != NULL)
		m_opProject -> SetDirty(false);
	EMMediaDebugLog::Instance() -> Log("Leaving EMMediaEngine::ReadyForUse()");
}

void EMMediaEngine::LockProject()
{
	if(m_opProject != NULL)
	{
		LockAudio();
		LockVideo();
	}
}

void EMMediaEngine::UnlockProject()
{
	if(m_opProject != NULL)
	{
		UnlockAudio();
		UnlockVideo();
	}
}

void EMMediaEngine::LockVideo()
{
	if(m_opProject != NULL)
	{
		m_opProject -> LockVideo();
	}
}

void EMMediaEngine::UnlockVideo()
{
	if(m_opProject != NULL)
	{
		m_opProject -> UnlockVideo();
	}
}

void EMMediaEngine::LockAudio()
{
	if(m_opProject != NULL)
	{
		m_opProject -> LockAudio();
	}
}

void EMMediaEngine::UnlockAudio()
{
	if(m_opProject != NULL)
	{
		m_opProject -> UnlockAudio();
	}
}

void EMMediaEngine::SaveComplete()
{
#ifdef __EM_MARTIN_VERSION
	return;
#endif
}

void EMMediaEngine::GetVersion(int32* p_vpMajor, int32* p_vpMinor)
{
	ASSERT(p_vpMajor != NULL && p_vpMinor != NULL);
	*p_vpMajor = 1; //__EMMediaEngine_LibraryVersion_MajorVersion;
	*p_vpMinor = __EMMediaEngine_LibraryVersion;  
}
