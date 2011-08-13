#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS
#include "EMMediaEngine.h"
DEFINE_GUID(CLSID_VideoVirtualRenderer, 0xf0e70ea5, 0x51e6, 0x11d5, 0x82, 0x59, 0x0, 0x2, 0xb3, 0x18, 0x0, 0x46);

DEFINE_GUID(CLSID_EMWinMediaVideoOutputFilter, 0xbedfce61, 0x535d, 0x11d5, 0x82, 0x59, 0x0, 0x2, 0xb3, 0x18, 0x0, 0x46);

DEFINE_GUID(IID_VideoVirtualRenderer, 0xc22ad161, 0x5523, 0x11d5, 0x82, 0x59, 0x0, 0x2, 0xb3, 0x18, 0x0, 0x46);

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
#include "EMTestVideoOut.h"
#include <math.h>


EMTestVideoOut::EMTestVideoOut() 
{
//	m_oConnection.m_opTargetFilter = p_opRenderFilter;
}

EMTestVideoOut::~EMTestVideoOut()
{
}

bool EMTestVideoOut::InitCheckE(EMVideoPreview* m_opPreviewWindow)
{
	HRESULT h = S_OK;
	IGraphBuilder* opGraphBuilder = NULL;

/*	if(m_oConnection.m_opTargetFilter == NULL)
	{
		//EMDebugger("ERROR! Cannot initialize output desc when the node is NULL!");
		return false;
	}
*/
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

	h = CoCreateInstance(CLSID_VideoVirtualRenderer, NULL, CLSCTX_INPROC, IID_IBaseFilter, (void**) &(m_oConnection.m_opTargetFilter));
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
	HRESULT hr = m_oConnection.m_opGraphBuilder ->Connect(m_oConnection.m_opOutputPin, pInPin);
	if(hr==S_OK)
		MessageBox(NULL, "Connected the god damn input pin with the god damn output pin", "Connection Message", MB_OK);
	else
		MessageBox(NULL, "Failed Connecting the god damn pins!!!", "Connection Message", MB_OK);

/*	h = m_oConnection.m_opGraphBuilder -> Render(m_oConnection.m_opOutputPin);
	if(FAILED(h))
	{
		eo << "ERROR! Failed to render graph!" << ef;
		return false;
	}
*/	
	h = m_oConnection.m_opGraphBuilder -> QueryInterface(IID_IMediaControl, (void**) &(m_oConnection.m_opControl));
	if(FAILED(h))
	{
		eo << "ERROR! Failed to retrieve interface of own output filter!" << ef;
		eo << "ERROR! Could not get control interface for graph!" << ef;
		return false;
	}

//	m_vIsInitialized = true;
	return true;//m_vIsInitialized;
}

bool EMTestVideoOut::PrepareToPlayE()
{
	return true;
}

void EMTestVideoOut::ProcessBufferLikeHell(EMMediaDataBuffer* p_opDataBuffer)
{
	try
	{
		m_oConnection.m_opSourceFilterInterface -> AddBufferToCache(p_opDataBuffer);
	}
	catch(...)
	{
		eo << "ERROR! RealtimeAudioOutputDesc caught an excepion from inside DirectX-filter!" << ef;
		p_opDataBuffer -> Recycle();
		throw;
	}

}

//Here, the EMMediaDataBuffer is converted into a BBuffer, as the final stage of the processing-chain.
//The EMMediaDataBuffers sent in are of course first mixed, and the result is then copied into the
//memory area of a BBuffer.
EMMediaDataBuffer* EMTestVideoOut::ProcessBufferE(list<EMMediaDataBuffer*>* p_opBufferList)
{


/*	
	if(p_opBufferList -> size() <= 0)
	{
		eo << "WARNING! ProcessBufferE received empty media data buffer list!" << ef;
		return NULL;
	}

	//Perform mixing
	EMMediaDataBuffer* opDataBuffer = EMAudioMixFunctions::AudioMixPostFader(p_opBufferList);

	if(opDataBuffer == NULL)
		eo << "ERROR! Result of mix operation is NULL!" << ef;

	try
	{
		m_oConnection.m_opSourceFilterInterface -> AddBufferToCache(opDataBuffer);
	}
	catch(...)
	{
		eo << "ERROR! RealtimeAudioOutputDesc caught an excepion from inside DirectX-filter!" << ef;
		opDataBuffer -> Recycle();
		throw;
	}
*/	
//	opDataBuffer -> Recycle();

	//There, now copy the mixed result into a BBuffer's data area	
//	if(m_opBufferGroup != NULL)
//	{
//		BBuffer* opBeBuffer = NULL;
//		int64 vStart = system_time();
//		while(true)
//		{
//			if((opBeBuffer = m_opBufferGroup -> RequestBuffer(opDataBuffer -> m_vSizeUsed, -1)) != NULL)
//				break;
//			if(system_time() - vStart > 500000)
//				break;
//		}
	
/*		opBeBuffer = m_opBufferGroup -> RequestBuffer(opDataBuffer -> m_vSizeUsed);
		if(opBeBuffer != NULL)
		{
			memcpy(opBeBuffer -> Data(), opDataBuffer -> Data(), opDataBuffer -> m_vSizeUsed);
			opBeBuffer -> SetSizeUsed(opDataBuffer -> m_vSizeUsed);

			if(m_opNode -> IsRunning() && opDataBuffer -> GetFrame() == EMMediaTimer::Instance() -> AudioThenFrame())
			{
				BBuffer** ptr = &opBeBuffer;
				((int64*) opBeBuffer -> Header() -> user_data)[0] = EMMediaTimer::Instance() -> AudioThenFrame();
				status_t err = write_port(m_opNode -> ControlPort(), EM_PORT_MESSAGE_INCOMING_BUFFER, ptr, sizeof(BBuffer)); //, B_TIMEOUT, B_INFINITE_TIMEOUT);
				if(err != B_OK)	
				{
					;//emout_commented_out_4_release << "ERROR! Buffer addition to node timed out!" << ef;
					opBeBuffer -> Recycle();
				}
			}
			else
			{
				;//emout_commented_out_4_release << "ERROR! Audio node not running, or wrong buffer time-stamp! Recycling buffer!" << ef;
				opBeBuffer -> Recycle();
			}
		}
		else
			;//emout_commented_out_4_release << "ERROR! Buffer request from buffer group timed out!" << ef;
		
		*/
	//	if(opDataBuffer -> m_vShouldBeDeleted)
	//		delete opDataBuffer;
	//	else 	
	//		opDataBuffer -> Recycle();
	//}
	//else
	//	EMDebugger("ERROR! EMWinOutputDescriptor doesn't have a BBufferGroup!");
	return NULL;
}

void EMTestVideoOut::StartE()
{
	HRESULT h = m_oConnection.m_opControl -> Run();

	//	status_t err = EMBeMediaUtility::GetRosterE() -> StartNode(m_opNode -> Node(), 0);
//	if(err)
//		EMDebugger("ERROR! Could not start audio node!");
}

void EMTestVideoOut::StopE()
{
//	HRESULT h = m_oConnection.m_opControl -> Pause();
//	if(FAILED(h))
//		eo << "ERROR! Failed to start DirectX filter(s)" << ef;

//	status_t err = EMBeMediaUtility::GetRosterE() -> StopNode(m_opNode -> Node(), 0, true); 
//	if(err)
//		emerr << "ERROR! Could not stop node!" << ef;
}

bool EMTestVideoOut::ClearData()
{
//	write_port_etc(m_opNode -> ControlPort(), EM_PORT_MESSAGE_RESET, NULL, 0, B_TIMEOUT, 25000);
	return true;
}

bool EMTestVideoOut::SaveData(EMProjectDataSaver* p_opSaver)
{
	return true;
}

bool EMTestVideoOut::LoadData(EMProjectDataLoader* p_opLoader)
{
	return true;
}

IPin* EMTestVideoOut::GetPin(IBaseFilter *pFilter, PIN_DIRECTION PinDir)
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


#endif
