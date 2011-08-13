#include "EMGlobals.h"
#include "EMBrightnessFilter.h"
#include "EMContrastFilter.h"
#include "EMVideoEffectFilter.h"
#include "EMSimpleVideoEffectDestination.h"

#include "EMVideoPreview.h"
#include "EMBitmap.h"
#include "EMWinBitmap.h"

#include "EMBitmapBuffer.h"


#ifdef PLATFORM_WINDOWS
#include "EMMediaEngine.h"

//DEFINE_GUID(CLSID_EMWinMediaVideoOutputFilter, 0xbedfce61, 0x535d, 0x11d5, 0x82, 0x59, 0x0, 0x2, 0xb3, 0x18, 0x0, 0x46);

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
#include "EMWinRealtimeVideoOutputDescriptor.h"
#include "EMMediaProject.h"
#include "EMVideoSynchronizer.h"
#include <math.h>
class EMWinMediaSingletonMuxer;

EMWinRealtimeVideoOutputDescriptor::EMWinRealtimeVideoOutputDescriptor(IBaseFilter* p_opRenderFilter) 
	:	EMRealtimeOutputDescriptor(EM_TYPE_ANY_VIDEO),
		m_vVideoRenderToDisk(NULL),
		m_opOldEMBeBitmapContainer(NULL),
		m_vIsClockMaster(true),
		m_opNewEMBeBitmapContainer(NULL),
		m_vOldWidth(0),
		m_vOldHeight(0)

//		m_opDirectXRenderFilter(p_opRenderFilter),
//		m_opDirectXProducerFilter(NULL)
{
//	m_oConnection.m_opTargetFilter = p_opRenderFilter;
	EMMediaTimer::Instance() -> AddListener(this); //This is where we get the original calls to flush the node from!
//	m_oConnection.m_opControl = NULL;
}


EMWinRealtimeVideoOutputDescriptor::~EMWinRealtimeVideoOutputDescriptor()
{
//	m_opNode -> RemoveListener(this);
//	m_opNode -> Shutdown();
	//EMBeMediaUtility::GetRosterE() -> ReleaseNode(m_opNode -> Node());
	EMMediaTimer::Instance() -> RemoveListener(this);

	if(m_opOldEMBeBitmapContainer != NULL)
		delete m_opOldEMBeBitmapContainer;
/*	if(m_oConnection.m_opControl != NULL)
	{
//		m_oConnection.m_opSourceFilterInterface -> Shutdown();

		HRESULT vResult = m_oConnection.m_opControl -> Stop();  //Test Kaneda
		if(FAILED(vResult))
			EMDebugger("ERROR! Failed to stop graph!");
		uint32 vRefCount2 = m_oConnection.m_opControl -> Release();

		IEnumFilters* opFilterEnumerator = NULL;
		
		if(m_oConnection.m_opGraphBuilder != NULL)
		{
			m_oConnection.m_opGraphBuilder -> EnumFilters(&opFilterEnumerator);
			
			IBaseFilter* opFilter = NULL;
			uint32 vFetched = 0;

			while(opFilterEnumerator -> Next(1, &opFilter, &vFetched) == S_OK)
			{
				IEnumPins* opPinEnumerator = NULL;
				opFilter -> EnumPins(&opPinEnumerator);

				IPin* opPin = NULL;
				vFetched = 0;

				while(opPinEnumerator -> Next(1, &opPin, &vFetched) == S_OK)
				{
					m_oConnection.m_opGraphBuilder -> Disconnect(opPin);
					uint32 vRefCount = opPin -> Release();
				}

				uint32 vRefCount = opPinEnumerator -> Release();
				vRefCount = opFilter -> Release();
			}
			uint32 vRefCount = opFilterEnumerator -> Release();
		}
	}

//	if(m_oConnection.m_opOutputPin != NULL)
//		m_oConnection.m_opOutputPin -> Release();
//	if(m_oConnection.m_opInputPin != NULL)
//		m_oConnection.m_opInputPin -> Release();
	if(m_oConnection.m_opSourceFilterInterface != NULL)
		m_oConnection.m_opSourceFilterInterface -> Release();
	if(m_oConnection.m_opTargetVideoFilterInterface != NULL)
		m_oConnection.m_opTargetVideoFilterInterface -> Release();

	if(m_oConnection.m_opSourceFilter != NULL)
	{
		m_oConnection.m_opGraphBuilder -> RemoveFilter(m_oConnection.m_opSourceFilter);
		uint32 vRefCount12 = m_oConnection.m_opSourceFilter -> Release();
	}
	if(m_oConnection.m_opTargetFilter != NULL)
	{
		m_oConnection.m_opGraphBuilder -> RemoveFilter(m_oConnection.m_opTargetFilter);
		uint32 vRefCount = m_oConnection.m_opTargetFilter -> Release();
	}
	if(m_oConnection.m_opGraphBuilder != NULL)
		uint32 vRefCount = m_oConnection.m_opGraphBuilder -> Release();
*/
}



bool EMWinRealtimeVideoOutputDescriptor::InitCheckE()
{
//	m_opPreviewWindow = static_cast<EMVideoPreview*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_VIDEO_PREVIEW));

	/*
	HRESULT h = S_OK;
	IGraphBuilder* opGraphBuilder = NULL;
	if(m_oConnection.m_opTargetFilter == NULL)
	{
		//EMDebugger("ERROR! Cannot initialize output desc when the node is NULL!");
		return false;
	}

	h = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC, IID_IFilterGraph, (void**) &(m_oConnection.m_opGraphBuilder));
	if(FAILED(h))
	{
		eo << "ERROR! Failed to create graph!" << ef;
		return false;
	}

	h = CoCreateInstance(CLSID_EMWinMediaVideoOutputFilter, NULL, CLSCTX_INPROC, IID_IBaseFilter, (void**) &(m_oConnection.m_opSourceFilter));
	if(FAILED(h))
	{
		eo << "ERROR! Failed to retrieve interface of own output filter!" << ef;
		return false;
	}

	h = m_oConnection.m_opTargetFilter -> QueryInterface(IID_VideoVirtualRenderer, (void**) &(m_oConnection.m_opTargetVideoFilterInterface));
	if(FAILED(h))
	{
		eo << "ERROR! Failed to retrieve interface of own renderer filter!" << ef;
		return false;
	} else
	{
		m_opPreviewWindow = static_cast<EMVideoPreview*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_VIDEO_PREVIEW));
	}

	h = m_oConnection.m_opSourceFilter -> QueryInterface(IID_IEMWinMediaOutputFilter, (void**) &(m_oConnection.m_opSourceFilterInterface));
	if(FAILED(h))
	{
		eo << "ERROR! Failed to retrieve interface of own output filter!" << ef;
		return false;
	}

	h = m_oConnection.m_opGraphBuilder -> AddFilter(m_oConnection.m_opTargetFilter, L"Video renderer");
	if(FAILED(h))
	{
		eo << "ERROR! Failed to add renderer filter to graph!" << ef;
		return false;
	}

	h = m_oConnection.m_opGraphBuilder -> AddFilter(m_oConnection.m_opSourceFilter, L"Elegant Media Producer Filter");
	if(FAILED(h))
	{
		eo << "ERROR! Failed to add own producer filter to filter graph!" << ef;
		return false;
	}


	m_oConnection.m_opTargetVideoFilterInterface -> InitializeRenderer(m_opPreviewWindow);

	m_oConnection.m_opSourceFilterInterface -> InitializeFilter(EMMediaTimer::Instance(), EMMediaEngine::Instance() -> GetSettingsRepository());
	
	m_oConnection.m_opOutputPin = ((EMWinMediaUtility*) EMMediaUtility::Instance()) -> GetFirstPin(m_oConnection.m_opSourceFilter);

	//Get the input pin of our renderer
	PIN_DIRECTION pPinDirection = PINDIR_INPUT;
	IPin* pInPin = GetPin(m_oConnection.m_opTargetFilter, pPinDirection);

	//Connect: Decoder_OUTPIN -> DumpRenderer_INPIN
	HRESULT hr = m_oConnection.m_opGraphBuilder -> Connect(m_oConnection.m_opOutputPin, pInPin);
	if(FAILED(hr))
		EMDebugger("ERROR! Failed connecting the video output pins!");
	pInPin -> Release();
	
	h = m_oConnection.m_opGraphBuilder -> QueryInterface(IID_IMediaControl, (void**) &(m_oConnection.m_opControl));
	if(FAILED(h))
	{
		eo << "ERROR! Failed to retrieve interface of own output filter!" << ef;
		eo << "ERROR! Could not get control interface for graph!" << ef;
		return false;
	}
	*/

	m_vIsInitialized = true;
	return m_vIsInitialized;
}

bool EMWinRealtimeVideoOutputDescriptor::PrepareToPlayE()
{
	return true;
}


// ################### HACK ##########################
//To make rendering work and showing it in the preview window
//TODO: Maybe fix this later on!?
EMRealtimeOutputDescriptor* EMWinRealtimeVideoOutputDescriptor::GetVideoRenderToDiskDestination()
{
	EMOutputRepository* opOutputs = EMOutputRepository::Instance();
	opOutputs -> LockContainer();
	try
	{
		opOutputs -> Rewind();
		while(opOutputs -> Current() != NULL)
		{
			if(opOutputs -> Current() -> IsRenderOutput() && (opOutputs -> Current() -> GetType() & EM_TYPE_ANY_VIDEO) > 0)
			{
				opOutputs -> UnlockContainer();
				return opOutputs -> Current();	
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
EMMediaDataBuffer* EMWinRealtimeVideoOutputDescriptor::ProcessBufferE(list<EMMediaDataBuffer*>* p_opBufferList)
{
	EMMediaDataBuffer* opDataBuffer = NULL;

	try
	{
		if(EMMediaEngine::Instance() -> GetMediaProject() -> IsRenderingVideo())
		{
			//if(m_vVideoRenderToDisk != NULL)
			//	m_vVideoRenderToDisk = dynamic_cast<EMWinRealtimeRenderToDiskOutputDescriptor*>(GetVideoRenderToDiskDestination());
			opDataBuffer = EMWinMediaSingletonMuxer::Instance() -> GetVideoRealTimeRenderToDiskObject() -> ProcessBufferE(p_opBufferList);
		}
		if(opDataBuffer == NULL)
			opDataBuffer = EMVideoMixFunctions::VideoPrioritizer(p_opBufferList);

		if(opDataBuffer != NULL)
		{
			if(! EMMediaEngine::Instance() -> GetMediaProject() -> IsRenderingVideo() || opDataBuffer -> m_vDiscon || opDataBuffer -> m_vIsSilence)
			{
				EMVideoSynchronizer::Instance() -> WaitForBufferStart(opDataBuffer -> GetRelativeTimeToStart());//GetRelativeTimeToStartForSynchClock());
				opDataBuffer -> m_vDiscon = false;
			}	
		
			EMBitmapBuffer::Instance() -> PutMediaBuffer(opDataBuffer);
			EMBitmapBuffer::Instance() -> Swap();

			if(m_vIsClockMaster)
			{
				int64 vDeltaTime = 1000000.0f / static_cast<float>(opDataBuffer -> m_oFormat.m_vFrameRate);
				EMMediaTimer::Instance() -> IncreaseNowFrame(EMMediaUtility::Instance() -> TimeToFrames(vDeltaTime, EMMediaUtility::Instance() -> GetSystemAudioFormat()), EMOutputDescriptor::GetID());
			}
		}

	}
	catch(...)
	{
		eo << "ERROR! RealtimeAudioOutputDesc caught an excepion from inside DirectX-filter!" << ef;
		if(opDataBuffer != NULL)
			opDataBuffer -> Recycle();
		//throw;
	}
	if(opDataBuffer != NULL)
		opDataBuffer -> Recycle();

	return NULL;
}

void EMWinRealtimeVideoOutputDescriptor::StartE()
{
//	HRESULT h = m_oConnection.m_opControl -> Run();

	//	status_t err = EMBeMediaUtility::GetRosterE() -> StartNode(m_opNode -> Node(), 0);
//	if(err)
//		EMDebugger("ERROR! Could not start audio node!");
}

void EMWinRealtimeVideoOutputDescriptor::StopE()
{

//	HRESULT h = m_oConnection.m_opControl -> Pause();
//	if(FAILED(h))
//		eo << "ERROR! Failed to start DirectX filter(s)" << ef;

//	status_t err = EMBeMediaUtility::GetRosterE() -> StopNode(m_opNode -> Node(), 0, true); 
//	if(err)
//		emerr << "ERROR! Could not stop node!" << ef;
}

bool EMWinRealtimeVideoOutputDescriptor::MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage)
{
	//eo << "MessageReceived!" << ef;
	if(p_vMessage == EM_MESSAGE_BEGIN_PLAYORRECORD)
	{
		return true;
	}
	else if(p_vMessage == EM_MESSAGE_STOP_PLAYORRECORD)
	{
		//m_oConnection.m_opSourceFilterInterface -> FlushCache();
		return true;
	}
	else if(p_vMessage == EM_TIMED_EVENT_FLUSH_CASH)
	{
	}
/*	else if(p_vMessage == EM_MESSAGE_PAUSE)
	{
		m_oConnection.m_opSourceFilterInterface -> FlushCache();
		return true;
	} */
/*	else if(p_vMessage == EM_MESSAGE_RESUME)
	{
		return true;
	} */
	else if(p_vMessage == EM_MESSAGE_TIME_WAS_SEEKED)
	{
		return true;
	}
	return false;
}

bool EMWinRealtimeVideoOutputDescriptor::ClearData()
{
//	write_port_etc(m_opNode -> ControlPort(), EM_PORT_MESSAGE_RESET, NULL, 0, B_TIMEOUT, 25000);
	return true;
}

bool EMWinRealtimeVideoOutputDescriptor::SaveData(EMProjectDataSaver* p_opSaver)
{
	return true;
}

bool EMWinRealtimeVideoOutputDescriptor::LoadData(EMProjectDataLoader* p_opLoader)
{
	return true;
}

IPin* EMWinRealtimeVideoOutputDescriptor::GetPin(IBaseFilter *pFilter, PIN_DIRECTION PinDir) 
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

bool EMWinRealtimeVideoOutputDescriptor::RegisterAsClockMaster()
{
//	m_oConnection.m_opSourceFilterInterface -> RegisterAsClockMaster(EMOutputDescriptor::GetID());
	m_vIsClockMaster = true;
	return true;
}

bool EMWinRealtimeVideoOutputDescriptor::UnregisterAsClockMaster()
{
//	m_oConnection.m_opSourceFilterInterface -> UnregisterAsClockMaster();
	m_vIsClockMaster = false;
	return true;
}

#endif
