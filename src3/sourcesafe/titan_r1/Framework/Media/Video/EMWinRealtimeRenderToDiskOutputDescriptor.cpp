#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#include "EMMediaEngine.h"
#include "EMAudioMixFunctions.h"
#include "EMVideoMixFunctions.h"
#include "EMException.h"
#include "EMMediaEngine.h"
#include "EMMediaTimer.h"
#include "EMProjectDataLoader.h"
#include "EMProjectDataSaver.h"
#include "EMSettingsRepository.h"
#include "EMSettingIDs.h"
#include "EMWinMediaSystemInspector.h"
#include "EMWinMediaUtility.h"
#include "EMWinRealtimeRenderToDiskOutputDescriptor.h"
#include "EMMediaFormat.h"
#include <math.h>
#include "EMWinRealtimeVideoEncoderDescriptor.h"
#include "EMWinRealtimeAudioEncoderDescriptor.h"
#include "EMEncoderRepository.h"
#include "EMMediaProject.h"
#include "EMMediaEngineUIDs.h"


EMWinRealtimeRenderToDiskOutputDescriptor::EMWinRealtimeRenderToDiskOutputDescriptor(IBaseFilter* p_opRenderFilter, EMMediaType p_eEMMediaType) 
	:	EMRealtimeOutputDescriptor(p_eEMMediaType),//EM_TYPE_RAW_AUDIO_VIDEO),
		m_opMuxInPin(NULL),
		m_opSourceFilterInterface(NULL),
		m_vShuffleFormatSet(false),
		m_vFileRendererCreated(false),
		m_vAllConnectionsMade(false),
		m_vIsRunning(false),
		m_opMediaFormat(NULL),
		m_opRealtimeEncoderDescriptor(NULL),
		m_opSingletonMuxer(NULL),
		m_eEMMediaType(p_eEMMediaType)
{
	EMMediaTimer::Instance() -> AddListener(this); //This is where we get the original calls to flush the node from!
	m_oConnection.m_opControl = NULL;
	m_oConnection.m_opOutputPin = NULL;
	m_oConnection.m_opTargetFilter = p_opRenderFilter;
}


EMWinRealtimeRenderToDiskOutputDescriptor::~EMWinRealtimeRenderToDiskOutputDescriptor()
{
	//Destructor stuff added by Richard, 010820
	EMMediaTimer::Instance() ->RemoveListener(this); //This is where we get the original calls to flush the node from!

	if(m_vIsInitialized) 
	{
		m_oConnection.m_opControl -> Stop();
	}

	//TODO: Fix bugs in the singletonmuxer class!! (in the destructor! Make sure we can handle that the member variables are unset/NULL!!)
//	if(m_opSingletonMuxer != NULL)
//		delete m_opSingletonMuxer;

	if(m_oConnection.m_opControl != NULL)
	{
		int32 vRefCount = m_oConnection.m_opControl -> Release();
		m_oConnection.m_opControl = NULL;
	}

	if(m_oConnection.m_opGraphBuilder != NULL)
	{
		int32 vRefCount = m_oConnection.m_opGraphBuilder -> Release();
		m_oConnection.m_opGraphBuilder = NULL;
	}

	if(m_oConnection.m_opTargetFilter != NULL)
	{
		int32 vRefCount = m_oConnection.m_opTargetFilter -> Release();
		m_oConnection.m_opTargetFilter = NULL;
	}

	if(m_oConnection.m_opSourceFilter != NULL)
	{
		int32 vRefCount = m_oConnection.m_opSourceFilter -> Release();
		m_oConnection.m_opSourceFilter = NULL;
	}

//	if(m_opSourceFilterInterface != NULL)
//		int32 vRefCount = m_opSourceFilterInterface -> Release();

	if(m_oConnection.m_opOutputPin != NULL)
	{
		int32 vRefCount = m_oConnection.m_opOutputPin -> Release();
		m_oConnection.m_opOutputPin = NULL;
	}

	if(m_opSourceFilterInterface != NULL)
	{
		int32 vRefCount = m_opSourceFilterInterface -> Release();
		m_opSourceFilterInterface = NULL;
	}

///	CoUninitialize();

}


bool EMWinRealtimeRenderToDiskOutputDescriptor::InitCheckE()
{
	m_opQuicktimeWriter = EMWinQuickTimeWriterSuper::Instance();
	//If not instansiated already get the Singleton Mux Filter and the Filtergraph
	//And a new input pin to it
	m_opSingletonMuxer = EMWinMediaSingletonMuxer::Instance();
	m_oConnection.m_opGraphBuilder = m_opSingletonMuxer -> GetFilterGraph();

	//Prepare for AVI
	if((m_eEMMediaType & EM_TYPE_ANY_VIDEO) > 0)
	{
		m_opSingletonMuxer -> StoreVideoRealTimeRenderToDiskObject(this);
	}
	else if((m_eEMMediaType & EM_TYPE_ANY_AUDIO) > 0)
	{
		m_opSingletonMuxer -> StoreAudioRealTimeRenderToDiskObject(this);
	}

	HRESULT h = S_OK;

	//Get the Interface to the Filter Graphs Controls
	h = m_oConnection.m_opGraphBuilder -> QueryInterface(IID_IMediaControl, (void**) &(m_oConnection.m_opControl));
	if(FAILED(h))
	{
		EMDebugger("ERROR! Could not get control interface for graph!");
		return false;
	}

	//CoCreate Elegant Medias Producer Filter For Write to Disk (AVI functionality)
	h = CoCreateInstance(CLSID_EMWinVideoShuffleOutputFilter, NULL, CLSCTX_INPROC, IID_IBaseFilter, (void**) &(m_oConnection.m_opSourceFilter));
	if(FAILED(h))
	{
		EMDebugger("ERROR! Failed to retrieve interface of own output filter!");
		return false;
	}

	//Get the Interface of Elegant Medias Producer Filter For Write
	h = m_oConnection.m_opSourceFilter -> QueryInterface(IID_IEMWinVideoShuffleOutputInterface, (void**) &(m_opSourceFilterInterface));
	if(FAILED(h))
	{
		EMDebugger("ERROR! Failed to retrieve interface of own output filter!");
		return false;
	}

	//Add this Filter to the Filter Graph
	h = m_oConnection.m_opGraphBuilder -> AddFilter(m_oConnection.m_opSourceFilter, L"Elegant Media Producer Filter For Write to Disk");
	if(FAILED(h))
	{
		EMDebugger("ERROR! Failed to add own producer filter to filter graph!");
		return false;
	}	
	
	m_oConnection.m_opOutputPin = GetPin(m_oConnection.m_opSourceFilter, PINDIR_OUTPUT);


	m_vIsInitialized = true;
	return true;
}


//Sets the specified format to use in the Filter Graph
void EMWinRealtimeRenderToDiskOutputDescriptor::SetShuffleFormat(EMMediaFormat* p_opMediaFormat)
{
	if(m_vIsInitialized)
	{
		if(m_opSingletonMuxer->m_vCurrentlyUsedFamily == EM_FAMILY_QUICKTIME && ((p_opMediaFormat->m_eType & EM_TYPE_ANY_VIDEO) > 0))
		{
			m_opQuicktimeWriter -> SetShuffleFormatForVideo(p_opMediaFormat);
		}
		else if(m_opSingletonMuxer->m_vCurrentlyUsedFamily == EM_FAMILY_QUICKTIME && ((p_opMediaFormat->m_eType & EM_TYPE_ANY_AUDIO) > 0))
		{
			m_opQuicktimeWriter -> SetShuffleFormatForAudio(p_opMediaFormat);
		}
		else
		{
			m_opSourceFilterInterface -> SetName("Elegant Media Producer Filter For Write to Disk Interface");
			m_opSourceFilterInterface -> SetShuffleFormat(p_opMediaFormat);
		}
		m_opMediaFormat = p_opMediaFormat;
		m_vShuffleFormatSet = true;
	}
	else
		MessageBox(NULL, "You can't shuffle the format if the EMWinRealtimeRenderToDiskOutputDescriptor has not initialized", "MESSAGE", MB_OK);

}

//Connects the SourceFilter with the Encoder and the Encoder with the Write Filter
bool EMWinRealtimeRenderToDiskOutputDescriptor::ConnectWithEncoder(int32 p_vEncoderID)
{
	HRESULT hr;
	if(m_vShuffleFormatSet)
	{

		if(m_opSingletonMuxer->m_vCurrentlyUsedFamily == EM_FAMILY_QUICKTIME)
		{
			//m_opQuicktimeWriter -> SetDecoderToUse(p_vEncoderID);
		}
		else
		{
			//This gets a new In Pin to the Connecting Mux Filter
			//TODO: WHEN DESTROYING THIS FILTER IT SHOULD BE DISCONNECTED
			m_opMuxInPin = m_opSingletonMuxer -> GetAMuxInPin();

			m_opRealtimeEncoderDescriptor = m_opSingletonMuxer -> GetEncoderDescriptor(p_vEncoderID);

			if(m_opRealtimeEncoderDescriptor == NULL)
			{
				m_opRealtimeEncoderDescriptor = m_opSingletonMuxer -> GetEncoderDescriptor(-1);
			}

			if((m_opMediaFormat -> m_eType & EM_TYPE_ANY_VIDEO) > 0)
			{
				EMWinRealtimeVideoEncoderDescriptor* opWinRealtimeVideoEncoderDescriptor = dynamic_cast<EMWinRealtimeVideoEncoderDescriptor*>(m_opRealtimeEncoderDescriptor);
				hr = opWinRealtimeVideoEncoderDescriptor -> LinkAndCreateEncoderFilter(m_oConnection.m_opOutputPin, m_opMuxInPin, m_oConnection.m_opGraphBuilder);
				if(hr != S_OK)
				{
					MessageBox(NULL,"The video encoder could not be used", "Titan - Error Message", NULL);
					return false;
				}
			}
			else if((m_opMediaFormat->m_eType & EM_TYPE_ANY_AUDIO) > 0)
			{
				EMWinRealtimeAudioEncoderDescriptor* opWinRealtimeAudioEncoderDescriptor = dynamic_cast<EMWinRealtimeAudioEncoderDescriptor*>(m_opRealtimeEncoderDescriptor);
				hr = opWinRealtimeAudioEncoderDescriptor -> LinkAndCreateEncoderFilter(m_oConnection.m_opOutputPin, m_opMuxInPin, m_oConnection.m_opGraphBuilder);
				if(hr != S_OK)
				{
					MessageBox(NULL,"The audio encoder could not be used", "Titan - Error Message", NULL);
					return false;
				}

			}
		}
		m_vAllConnectionsMade = true;
	}
	else
		MessageBox(NULL, "You can't connect to any encoder before you have shuffled the format with SetShuffleFormat(EMMediaFormat* xx)", "MESSAGE", MB_OK);

	return true;
}

//Disconnects the Encoder with the Source Filter and the FileWriter Filter
bool EMWinRealtimeRenderToDiskOutputDescriptor::DisConnectWithEncoder()
{
	if(m_vAllConnectionsMade)
	{
		if(m_opSingletonMuxer->m_vCurrentlyUsedFamily == EM_FAMILY_QUICKTIME)
		{
				m_opQuicktimeWriter -> DisConnectWithEncoders();
		}
		else if((m_opMediaFormat->m_eType & EM_TYPE_ANY_VIDEO) > 0)
		{
			EMWinRealtimeVideoEncoderDescriptor* opWinRealtimeVideoEncoderDescriptor = dynamic_cast<EMWinRealtimeVideoEncoderDescriptor*>(m_opRealtimeEncoderDescriptor);
			opWinRealtimeVideoEncoderDescriptor -> DisLinkAndRemoveEncoderFilter(m_oConnection.m_opGraphBuilder);
			m_vAllConnectionsMade = false;

		}
		else if((m_opMediaFormat->m_eType & EM_TYPE_ANY_AUDIO) > 0)
		{
			EMWinRealtimeAudioEncoderDescriptor* opWinRealtimeAudioEncoderDescriptor = dynamic_cast<EMWinRealtimeAudioEncoderDescriptor*>(m_opRealtimeEncoderDescriptor);
			opWinRealtimeAudioEncoderDescriptor -> DisLinkAndRemoveEncoderFilter(m_oConnection.m_opGraphBuilder);
			m_vAllConnectionsMade = false;
		}

	}
	else
	{
		//MessageBox(NULL, "You can't disconnect if you haven't connected the encoder ;)", "MESSAGE", MB_OK);
	}
	return true;
}

//Get the (this) source filters output pin
IPin* EMWinRealtimeRenderToDiskOutputDescriptor::GetSourceOutPin()
{
 return m_oConnection.m_opOutputPin;
}

bool EMWinRealtimeRenderToDiskOutputDescriptor::PrepareToPlayE()
{
	return true;
}

//Here, the EMMediaDataBuffer is converted into a BBuffer, as the final stage of the processing-chain.
//The EMMediaDataBuffers sent in are of course first mixed, and the result is then copied into the
//memory area of a BBuffer.
EMMediaDataBuffer* EMWinRealtimeRenderToDiskOutputDescriptor::ProcessBufferE(list<EMMediaDataBuffer*>* p_opBufferList)
{

	EMMediaDataBuffer* opDataBuffer;

	//If this is the first time that a buffer arrives (in video case).
	//The Audio format is set through message receive before the first buffer arrives!
	if(m_opMediaFormat == NULL)
	{
		EMMediaDataBuffer* opTrashBuffer = p_opBufferList -> front();
		m_opMediaFormat = &(opTrashBuffer -> m_oFormat);
	}

	//Get the / mixed / a buffer from the received databuffer list
	if((m_opMediaFormat->m_eType & EM_TYPE_ANY_VIDEO) > 0)
	{
		opDataBuffer = EMVideoMixFunctions::VideoPrioritizer(p_opBufferList);
	}
	else if((m_opMediaFormat->m_eType & EM_TYPE_ANY_AUDIO) > 0)
	{
		if(p_opBufferList -> size() <= 0)
			return NULL;
		opDataBuffer = EMAudioMixFunctions::AudioMixPostFader(p_opBufferList);
	}
	
	//Has the filter been started yet?
	//If not start it, if it already is strated Process it.
	if(m_vIsRunning)
	{
		//If it is QuickTime and video buffers are to be processed
		if(((m_opMediaFormat -> m_eType & EM_TYPE_ANY_VIDEO) > 0) && m_opSingletonMuxer -> m_vCurrentlyUsedFamily == EM_FAMILY_QUICKTIME)
		{
			try
			{
				m_opQuicktimeWriter -> ProcessVideoBuffer(opDataBuffer);
			}
			catch(...)
			{
				throw;
			}
			return opDataBuffer;
		}//If it is QuickTime and audio buffers are to be processed
		else if(((m_opMediaFormat -> m_eType & EM_TYPE_ANY_AUDIO) > 0) && m_opSingletonMuxer -> m_vCurrentlyUsedFamily == EM_FAMILY_QUICKTIME)
		{
			try
			{
				m_opQuicktimeWriter -> ProcessAudioBuffer(opDataBuffer);
			}
			catch(...)
			{
				throw;
			}
			return opDataBuffer;
		}

		//It was not QuickTime, probably AVI
		try
		{
			m_opSourceFilterInterface -> ProcessBuffer(opDataBuffer);
		}
		catch(...)
		{
			throw;
		}
		return opDataBuffer;
	}
	else
	{
		//If it is QuickTime and video buffers are to be prcessed
		if(((m_opMediaFormat -> m_eType & EM_TYPE_ANY_VIDEO) > 0) && m_opSingletonMuxer -> m_vCurrentlyUsedFamily == EM_FAMILY_QUICKTIME)
		{
			SetShuffleFormat(&opDataBuffer -> m_oFormat);
			m_opQuicktimeWriter -> InitCheckE(EM_TYPE_ANY_VIDEO);
			try
			{
				m_opQuicktimeWriter -> ProcessVideoBuffer(opDataBuffer);
			}
			catch(...)
			{
				throw;
			}
			m_vIsRunning = true;
			return opDataBuffer;
		}//If it is QuickTime and audio buffers are to be processed
		else if(((m_opMediaFormat -> m_eType & EM_TYPE_ANY_AUDIO) > 0) && m_opSingletonMuxer -> m_vCurrentlyUsedFamily == EM_FAMILY_QUICKTIME)
		{
			SetShuffleFormat(&opDataBuffer -> m_oFormat );
			m_opQuicktimeWriter -> InitCheckE(EM_TYPE_ANY_AUDIO);
			try
			{
				m_opQuicktimeWriter -> ProcessAudioBuffer(opDataBuffer);
			}
			catch(...)
			{
				throw;
			}
			m_vIsRunning = true;
			return opDataBuffer;
		}

		//It was not QuickTime, probably AVI
		counter = 0;
		//Set the video format in our video source filter (same filter as audio)
		if(((m_opMediaFormat -> m_eType & EM_TYPE_ANY_VIDEO) > 0) && (! m_opSingletonMuxer -> VideoIsReady()))
		{
			if(m_vAllConnectionsMade)
				DisConnectWithEncoder();
			SetShuffleFormat(&opDataBuffer -> m_oFormat );
			if(! ConnectWithEncoder(m_opSingletonMuxer -> m_vVideoEncoderID))
			{
				//Error has occured .. stop everything
				m_opSingletonMuxer -> SetErrorHasOccured(true);
				m_vIsRunning = false;
				return opDataBuffer;
			}
			else
			{
				m_opSingletonMuxer -> SetVideoIsReady(true);
			}
		}
		else if((m_opMediaFormat->m_eType & EM_TYPE_ANY_AUDIO) > 0)
		{
			//MOVED tO MESSAGE RECEIVED
			//The format for Audio is set in Message Received
		}

		//Start the filter
		StartE();

		try
		{
			//AVI
			m_opSourceFilterInterface -> ProcessBuffer(opDataBuffer);
		}
		catch(...)
		{
			throw;
		}
		return opDataBuffer;
	}

	return NULL;
}

//Start running the filter graph
void EMWinRealtimeRenderToDiskOutputDescriptor::StartE()
{
	if(m_vAllConnectionsMade && !m_vIsRunning)
	{

		//If audio has not yet been initialized (connected to the graph)
		//Wait until it has been so.
		while(! m_opSingletonMuxer -> AllIsReady() && m_opSingletonMuxer -> ErrorHasOccured() == false)
			Sleep(50);
		if(m_opSingletonMuxer -> ErrorHasOccured() == true)
		{
			DisConnectWithEncoder();
			m_vIsRunning = false;
			
		}
		else
		{
			//Set compression quality
			if((m_opMediaFormat -> m_eType & EM_TYPE_ANY_VIDEO) > 0)
			{
				int vQualityInt = *(static_cast<int*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_VIDEO_RENDER_QUALITY)));
				float vQualityFloat = static_cast<float>(vQualityInt/100.0);
				m_opSingletonMuxer -> SetCompressionQuality(vQualityFloat);
			}
			HRESULT h = m_oConnection.m_opControl -> Run();
			if(FAILED(h))
			{
				if(h == -2147024864)
					MessageBox(NULL, "The file is already in use. Try closing the application using the file.", "Message", MB_OK | MB_ICONSTOP);
				DisConnectWithEncoder();
				m_vIsRunning = false;
				m_opSingletonMuxer -> SetVideoIsReady(false);
				m_opSingletonMuxer -> SetAudioIsReady(false);
				m_opSingletonMuxer -> SetErrorHasOccured(true);
			}
			else
				m_vIsRunning = true;
		}
	}
	else
		EMDebugger("You are not alowed to start if not all connections are made");
}

//Stop the Filter Graph
void EMWinRealtimeRenderToDiskOutputDescriptor::StopE()
{
	if(m_vIsRunning || m_opSingletonMuxer -> ErrorHasOccured() == true)
	{
		HRESULT h = m_oConnection.m_opControl -> Stop();
		DisConnectWithEncoder();
		if(FAILED(h))
		{
			EMDebugger("ERROR! Failed to Stop the Elegant Media Write To Disk Filter");
		}
		if((m_opMediaFormat->m_eType & EM_TYPE_ANY_VIDEO) > 0)
		{
			EMMediaEngine::Instance() -> GetMediaProject() -> SetRenderingVideo(false);
			m_opSingletonMuxer -> SetVideoIsReady(false);
			//EMMediaEngine::Instance() -> GetMediaProject() -> ShouldProduceVideo(false, true);
		}
		else if((m_opMediaFormat->m_eType & EM_TYPE_ANY_AUDIO) > 0)
		{
			EMMediaEngine::Instance() -> GetMediaProject() -> SetRenderingAudio(false);
			m_opSingletonMuxer -> SetAudioIsReady(false);
			//EMMediaEngine::Instance() -> GetMediaProject() -> ShouldProduceAudio(false, true);
		}
		//if(m_vAllConnectionsMade
		EMMediaEngine::Instance() -> GetMediaProject() -> StopPlaybackE();
		m_vIsRunning = false;

	}
}

bool EMWinRealtimeRenderToDiskOutputDescriptor::MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage)
{
	if(p_vMessage == EM_MESSAGE_BEGIN_PLAYORRECORD)
	{

		if((m_eEMMediaType == EM_TYPE_RAW_AUDIO) && ((EMMediaEngine::Instance() -> GetMediaProject() -> IsRenderingAudio() && EMMediaEngine::Instance() -> GetMediaProject() -> IsRenderingVideo()) || EMMediaEngine::Instance() -> GetMediaProject() -> IsRenderingVideo()))
		{
			//If it is QuickTime do nothing
			if(m_opSingletonMuxer -> m_vCurrentlyUsedFamily == EM_FAMILY_QUICKTIME)
			{
				return true;
			}

			if(m_vAllConnectionsMade)
				DisConnectWithEncoder();

			m_opMediaFormat = EM_new EMMediaFormat(EM_TYPE_RAW_AUDIO);
			SetShuffleFormat(m_opMediaFormat);

			if(! ConnectWithEncoder(m_opSingletonMuxer -> m_vAudioEncoderID))
			{
				//Error has occured .. stop everything
				m_opSingletonMuxer -> SetErrorHasOccured(true);
				m_vIsRunning = false;
			}
			else
			{
				m_opSingletonMuxer -> SetAudioIsReady(true);
			}

		} 
		else if((m_eEMMediaType == EM_TYPE_RAW_AUDIO) && ! EMMediaEngine::Instance() -> GetMediaProject() -> IsRenderingAudio())
		{
			m_opSingletonMuxer -> SetAudioIsReady(true);
		}
		return true;
	}
	else if(p_vMessage == EM_MESSAGE_STOP_PLAYORRECORD)
	{
		if(m_vIsRunning)
		{
			//If it is QuickTime do nothing
			if(m_opSingletonMuxer -> m_vCurrentlyUsedFamily == EM_FAMILY_QUICKTIME)
			{
				m_opQuicktimeWriter -> StopWritingToDisk();
				EMMediaEngine::Instance() -> GetMediaProject() -> StopPlaybackE();
				m_vIsRunning = false;
				return true;
			}

			if((m_eEMMediaType == EM_TYPE_RAW_AUDIO) && EMMediaEngine::Instance() -> GetMediaProject() -> IsRenderingAudio())
			{
				StopE();
			}
			else if((m_eEMMediaType == EM_TYPE_RAW_AUDIO) && ! EMMediaEngine::Instance() -> GetMediaProject() -> IsRenderingAudio())
			{
				if(m_opSingletonMuxer -> AudioIsReady())
				{
					StopE();
				}
			}

			if((m_eEMMediaType == EM_TYPE_RAW_VIDEO) && EMMediaEngine::Instance() -> GetMediaProject() -> IsRenderingVideo())
			{
				StopE();
			}
			else if((m_eEMMediaType == EM_TYPE_RAW_VIDEO) && ! EMMediaEngine::Instance() -> GetMediaProject() -> IsRenderingVideo())
			{
				if(m_opSingletonMuxer -> VideoIsReady())
				{
					StopE();
				}
			}
		}
		return true;
	}
	else if(p_vMessage == EM_TIMED_EVENT_FLUSH_CASH)
	{
		return true;
	}
	else if(p_vMessage == EM_MESSAGE_TIME_WAS_SEEKED)
	{
		return true;
	}
	return false;
}

bool EMWinRealtimeRenderToDiskOutputDescriptor::ClearData()
{
	return true;
}

bool EMWinRealtimeRenderToDiskOutputDescriptor::SaveData(EMProjectDataSaver* p_opSaver)
{
	return true;
}

bool EMWinRealtimeRenderToDiskOutputDescriptor::LoadData(EMProjectDataLoader* p_opLoader)
{
	return true;
}

IPin* EMWinRealtimeRenderToDiskOutputDescriptor::GetPin(IBaseFilter *pFilter, PIN_DIRECTION PinDir) 
{
    BOOL       bFound = FALSE;
    IEnumPins  *pEnum;
    IPin       *pPin;
	bool		FoundIt = false;
    pFilter->EnumPins(&pEnum);
    while(pEnum->Next(1, &pPin, 0) == S_OK)
    {
        PIN_DIRECTION PinDirThis;
        pPin->QueryDirection(&PinDirThis);
        if (bFound = (PinDir == PinDirThis))
		{
//			MessageBox(NULL, "Found Matching PIN_DIRECTION", "Looking for a PIN", MB_OK);
			FoundIt = true;
            break;
		}
        pPin->Release();
//			MessageBox(NULL, "Found A Non Matching PIN_DIRECTION", "Looking for a PIN", MB_OK);
    }
//	if(!FoundIt)
//			MessageBox(NULL, "DIDN'T FIND A MATCHING PIN", "WARNING WARNING!!!", MB_OK);
    pEnum->Release();
    return (bFound ? pPin : 0);  
}


bool EMWinRealtimeRenderToDiskOutputDescriptor::RegisterAsClockMaster()
{
	m_opSourceFilterInterface -> RegisterAsClockMaster(EMOutputDescriptor::GetID());
	return true;
}

bool EMWinRealtimeRenderToDiskOutputDescriptor::UnregisterAsClockMaster()
{
	m_opSourceFilterInterface -> UnregisterAsClockMaster();
	return true;
}


#endif
