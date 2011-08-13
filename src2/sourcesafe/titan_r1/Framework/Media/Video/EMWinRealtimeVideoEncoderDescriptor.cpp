#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#include "EMWinRealtimeVideoEncoderDescriptor.h"
#include "EMMediaEngine.h"
#include "EMWinMediaSingletonMuxer.h"

EMWinRealtimeVideoEncoderDescriptor::EMWinRealtimeVideoEncoderDescriptor(IBaseFilter* p_opSourceFilter) 
	:	EMRealtimeEncoderDescriptor(EM_TYPE_ANY_VIDEO),
	m_opSourceOutPin(NULL),
	m_opMuxInPin(NULL)
{
	m_opEncoderFilter = p_opSourceFilter;
	m_EncoderInPin = GetPin(m_opEncoderFilter, PINDIR_INPUT);
	m_EncoderOutPin = GetPin(m_opEncoderFilter, PINDIR_OUTPUT);

	m_opEncoderFilter -> AddRef();
}

EMWinRealtimeVideoEncoderDescriptor::~EMWinRealtimeVideoEncoderDescriptor()
{
	if(m_opEncoderFilter != NULL)
	{
		m_opEncoderFilter -> Release();
		m_opEncoderFilter = NULL;
	}

	if(m_EncoderInPin != NULL)
	{
		m_EncoderInPin -> Release();
		m_EncoderInPin = NULL;
	}

	if(m_EncoderOutPin != NULL)
	{
		m_EncoderOutPin -> Release();
		m_EncoderOutPin = NULL;
	}

	if(m_opSourceOutPin != NULL)
	{
		m_opSourceOutPin -> Release();
		m_opSourceOutPin = NULL;
	}
	if(m_opMuxInPin != NULL)
	{
		m_opMuxInPin -> Release();
		m_opMuxInPin = NULL;
	}
}

IPin* EMWinRealtimeVideoEncoderDescriptor::GetPin(IBaseFilter *pFilter, PIN_DIRECTION PinDir) 
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
			FoundIt = true;
            break;
		}
        pPin->Release();
    }
    pEnum->Release();
    return (bFound ? pPin : 0);  
}

bool EMWinRealtimeVideoEncoderDescriptor::PrepareToPlayE()
{
	return true;
}

bool EMWinRealtimeVideoEncoderDescriptor::StartPreviewE()
{
	return true;
}
bool EMWinRealtimeVideoEncoderDescriptor::InitCheckE()
{
	return true;
}

void EMWinRealtimeVideoEncoderDescriptor::StartE()
{
}

void EMWinRealtimeVideoEncoderDescriptor::StopE()
{
}

bool EMWinRealtimeVideoEncoderDescriptor::StopPreviewE()
{
	return true;
}

bool EMWinRealtimeVideoEncoderDescriptor::ClearData()
{
	return true;
}

IBaseFilter* EMWinRealtimeVideoEncoderDescriptor::GetEncoderFilter()
{
	return m_opEncoderFilter;
}

IPin* EMWinRealtimeVideoEncoderDescriptor::GetEncoderInPin()
{
	return m_EncoderInPin;
}

IPin* EMWinRealtimeVideoEncoderDescriptor::GetEncoderOutPin()
{
	return m_EncoderOutPin;
}

//This function connects the sourcefilter to the encoder and the encoder to the (AVI) Mux filter (which in turn
//should be connected to the filewriter filter)
HRESULT EMWinRealtimeVideoEncoderDescriptor::LinkAndCreateEncoderFilter(IPin* p_opSourcePin, IPin* p_opMuxPin, IGraphBuilder* p_opGraphBuilder)
{
	HRESULT h;
	m_opSourceOutPin = p_opSourcePin;
	m_opMuxInPin = p_opMuxPin;

	//Add the encoder filter to the Filter Graph
	h = p_opGraphBuilder -> AddFilter(m_opEncoderFilter, L"Encoder filter");
	if(FAILED(h))
	{
		return h;
	}

	//Connect the source with the encoder
	h = p_opGraphBuilder -> Connect(p_opSourcePin, m_EncoderInPin);
	if(h != S_OK)
	{
		p_opGraphBuilder->RemoveFilter(m_opEncoderFilter);
		return h;
	}

	//Set the Audio Compression Capabilities
	//EMWinMediaSingletonMuxer::Instance() -> SetCompressionCapability();

	//Connect the encoder with the Mux Filter
	h = p_opGraphBuilder -> Connect(m_EncoderOutPin, p_opMuxPin);
	if(h != S_OK)
	{
		DisLinkAndRemoveEncoderFilter(p_opGraphBuilder);
		return h;
	}
	return h;
}

//After that the encoder filter has been disconnected and removed the filter has to use LinkAndCreateEncoderFilter
//in order to make the filtergraph one with the encoder again
HRESULT EMWinRealtimeVideoEncoderDescriptor::DisLinkAndRemoveEncoderFilter(IGraphBuilder* p_opGraphBuilder)
{

	//Disconnect the Mux Filter In Pin
	if(m_opMuxInPin != NULL)
		p_opGraphBuilder->Disconnect(m_opMuxInPin);

	//Disconnect the Encoder Out Pin
	p_opGraphBuilder->Disconnect(m_EncoderOutPin);
	
	//Disconnect the Encoder In Pin
	p_opGraphBuilder->Disconnect(m_EncoderInPin);

	//Disconnect the Source Filter Out Pin
	if(m_opSourceOutPin != NULL)
		p_opGraphBuilder->Disconnect(m_opSourceOutPin);

	//If the filter is not present the Filter Graph will just ignore the request
	p_opGraphBuilder->RemoveFilter(m_opEncoderFilter);
	
	return S_OK;
}


#endif
