#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#include "EMAudioMixFunctions.h"
#include "EMException.h"
#include "EMMediaEngine.h"
#include "EMMediaTimer.h"
#include "EMProjectDataLoader.h"
#include "EMProjectDataSaver.h"
#include "EMSettingsRepository.h"
#include "EMSettingIDs.h"
#include "EMWinMediaSystemInspector.h"
#include "EMWinMediaUtility.h"
#include "EMWinRealtimeAudioOutputDescriptor.h"
#include "EMMediaProject.h"
#include "EMWinAudioDiskWriter.h"
#include "EMWinDirectSoundPlayback.h"
//#include "EMMediaDebugLog.h"

//#include <math.h> 
//#include <windows.h> 
 

EMWinRealtimeAudioOutputDescriptor::EMWinRealtimeAudioOutputDescriptor(const GUID* p_upGUID) 
	:	EMRealtimeOutputDescriptor(EM_TYPE_ANY_AUDIO), 
		m_opAudioOutput(NULL)
{
	m_opAudioOutput = EM_new EMWinDirectSoundPlayback(p_upGUID, GetID());
	memcpy(&m_uDeviceGUID, p_upGUID, sizeof(GUID));
//	EMMediaTimer::Instance() -> AddListener(this); //This is where we get the original calls to flush the node from!
} 

EMWinRealtimeAudioOutputDescriptor::~EMWinRealtimeAudioOutputDescriptor()  
{
	if(m_opAudioOutput != NULL)
	{
		m_opAudioOutput -> Stop(); 
		m_opAudioOutput-> CleanUpE();
	}

	delete m_opAudioOutput;  
	m_opAudioOutput = NULL; 
//	EMMediaTimer::Instance() -> RemoveListener(this);
}
 
bool EMWinRealtimeAudioOutputDescriptor::InitCheckE()
{
	m_opAudioOutput -> SetName(GetName().c_str());
	m_vIsInitialized = m_opAudioOutput -> InitCheckE(); //m_opDummyWindowHandle);
 	if(m_vIsInitialized)
		m_vIsInitialized = m_opAudioOutput -> Start();
	return m_vIsInitialized;
}

#ifdef _DEBUG
EMWinDirectSoundPlayback* EMWinRealtimeAudioOutputDescriptor::GetDSPlayback()
{
	return m_opAudioOutput;
}
#endif

bool EMWinRealtimeAudioOutputDescriptor::PrepareToPlayE()
{
	return true;
}

// ################### HACK ##########################
//To make rendering work and showing it in the preview window
//TODO: Maybe fix this later on!?
EMRealtimeOutputDescriptor* EMWinRealtimeAudioOutputDescriptor::GetAudioRenderToDiskDestination()
{
	EMOutputRepository* opOutputs = EMOutputRepository::Instance();
	opOutputs -> LockContainer();
	try
	{
		opOutputs -> Rewind();
		while(opOutputs -> Current() != NULL)
		{
			if(opOutputs -> Current() -> IsRenderOutput() && (opOutputs -> Current() -> GetType() & EM_TYPE_ANY_AUDIO) > 0)
			{
				opOutputs -> UnlockContainer();
				return opOutputs	-> Current();	
			}
			opOutputs -> Next();
		}
	}
	catch(...)
	{
		EMDebugger("ERROR! Exception in EMWinRealtimeVideoOutputDescriptor::ProcessBufferE(..) (Couldn't find A Video Renderer)");
	}

	opOutputs -> UnlockContainer();
	return NULL;
}
//################# END OF HACK ######################


//Here, the EMMediaDataBuffer is converted into a BBuffer, as the final stage of the processing-chain.
//The EMMediaDataBuffers sent in are of course first mixed, and the result is then copied into the
//memory area of a BBuffer.
EMMediaDataBuffer* EMWinRealtimeAudioOutputDescriptor::ProcessBufferE(list<EMMediaDataBuffer*>* p_opBufferList)
{
	EMMediaDataBuffer* opDataBuffer = NULL;
	// ################### HACK ##########################
	//To make rendering work and showing it in the preview window
	//TODO: Maybe fix this later on!?
	if(EMMediaEngine::Instance() -> GetMediaProject() -> IsRenderingAudio() && EMMediaEngine::Instance() -> GetMediaProject() -> IsRenderingVideo())
	{
		opDataBuffer = EMWinMediaSingletonMuxer::Instance() -> GetAudioRealTimeRenderToDiskObject() -> ProcessBufferE(p_opBufferList);
		opDataBuffer -> Recycle();
		return NULL;
	}
	else if(EMMediaEngine::Instance() -> GetMediaProject() -> IsRenderingAudio())
	{
		if(!(EMWinAudioDiskWriter::Instance() -> IsRunning()))
			EMWinAudioDiskWriter::Instance() -> Start();

		opDataBuffer = EMWinAudioDiskWriter::Instance() -> ProcessBufferE(p_opBufferList);
		m_opAudioOutput -> ProcessBufferE(opDataBuffer);
		EMMediaTimer::Instance() -> IncreaseNowFrame(EMMediaUtility::Instance() -> BytesToFrames(opDataBuffer -> m_vSizeUsed, EMMediaUtility::Instance() -> GetSystemAudioFormat()), GetID());
		opDataBuffer -> Recycle();
		return NULL;
	}
	//################# END OF HACK ######################

	if(p_opBufferList -> size() <= 0)
	{
		eo << "WARNING! ProcessBufferE received empty media data buffer list!" << ef;
		return NULL;
	}

	if(opDataBuffer == NULL)
		opDataBuffer = EMAudioMixFunctions::AudioMixPostFader(p_opBufferList);

	m_opAudioOutput -> ProcessBufferE(opDataBuffer);
//	EMMediaTimer::Instance() -> IncreaseNowFrame(EMMediaUtility::Instance() -> BytesToFrames(opDataBuffer -> m_vSizeUsed, EMMediaUtility::Instance() -> GetSystemAudioFormat()), GetID());
	opDataBuffer -> Recycle();

	return NULL; 
}

void EMWinRealtimeAudioOutputDescriptor::StartE()
{
	if(m_vIsInitialized)
	{
	}
	else
		EMDebugger("ERROR! Can't start output if it's not initialized!"); 
}

void EMWinRealtimeAudioOutputDescriptor::StopE()
{

	if(m_vIsInitialized)
	{
	}
}

bool EMWinRealtimeAudioOutputDescriptor::MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage)
{
	try
	{
		if(p_vMessage == EM_MESSAGE_BEGIN_PLAYORRECORD)
		{
			return true;
		}
		else if(p_vMessage == EM_MESSAGE_STOP_PLAYORRECORD)
		{
			return true;
		}
		else if(p_vMessage == EM_TIMED_EVENT_FLUSH_CASH)
		{
		}
		else if(p_vMessage == EM_MESSAGE_TIME_WAS_SEEKED)
		{
			return true;
		}
	}
	catch(...)
	{
		EMDebugger("ERROR! Exception in RealtimeAudioOutput::MessageReceived!");
	}
	return false;
}

bool EMWinRealtimeAudioOutputDescriptor::ClearData()
{
	return true;
}

bool EMWinRealtimeAudioOutputDescriptor::SaveData(EMProjectDataSaver* p_opSaver)
{
	return true;
}

bool EMWinRealtimeAudioOutputDescriptor::LoadData(EMProjectDataLoader* p_opLoader)
{
	return true;
}

bool EMWinRealtimeAudioOutputDescriptor::RegisterAsClockMaster()
{
	return true; //m_opAudioOutput -> RegisterAsClockMaster(GetID());
}

bool EMWinRealtimeAudioOutputDescriptor::UnregisterAsClockMaster()
{
	return true; //m_opAudioOutput -> UnregisterAsClockMaster();
}

#endif
