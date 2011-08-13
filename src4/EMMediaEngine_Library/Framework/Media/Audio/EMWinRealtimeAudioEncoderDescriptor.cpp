#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#include "EMWinRealtimeAudioEncoderDescriptor.h"
#include "EMMediaEngine.h"
#include "EMWinMediaSingletonMuxer.h"

EMWinRealtimeAudioEncoderDescriptor::EMWinRealtimeAudioEncoderDescriptor(IBaseFilter* p_opSourceFilter, IMoniker* p_vIMoniker) 
	:	EMRealtimeEncoderDescriptor(EM_TYPE_ANY_AUDIO),
	m_opSourceOutPin(NULL),
	m_opMuxInPin(NULL)
{
	m_opIMoniker = p_vIMoniker;
	m_opEncoderFilter = p_opSourceFilter;
	m_EncoderInPin = GetPin(m_opEncoderFilter, PINDIR_INPUT);
	m_EncoderOutPin = GetPin(m_opEncoderFilter, PINDIR_OUTPUT);

	m_opIMoniker -> AddRef();
	m_opEncoderFilter -> AddRef();
}

EMWinRealtimeAudioEncoderDescriptor::~EMWinRealtimeAudioEncoderDescriptor()
{

	if(m_opIMoniker != NULL)
	{
		m_opIMoniker -> Release();
		m_opIMoniker = NULL;
	}

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

IPin* EMWinRealtimeAudioEncoderDescriptor::GetPin(IBaseFilter *pFilter, PIN_DIRECTION PinDir) 
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

bool EMWinRealtimeAudioEncoderDescriptor::PrepareToPlayE()
{
	return true;
}

bool EMWinRealtimeAudioEncoderDescriptor::StartPreviewE()
{
	return true;
}
bool EMWinRealtimeAudioEncoderDescriptor::InitCheckE()
{
	return true;
}

void EMWinRealtimeAudioEncoderDescriptor::StartE()
{
}

void EMWinRealtimeAudioEncoderDescriptor::StopE()
{
}

bool EMWinRealtimeAudioEncoderDescriptor::StopPreviewE()
{
	return true;
}

bool EMWinRealtimeAudioEncoderDescriptor::ClearData()
{
	return true;
}

IBaseFilter* EMWinRealtimeAudioEncoderDescriptor::GetEncoderFilter()
{
	return m_opEncoderFilter;
}

IPin* EMWinRealtimeAudioEncoderDescriptor::GetEncoderInPin()
{
	return m_EncoderInPin;
}

IPin* EMWinRealtimeAudioEncoderDescriptor::GetEncoderOutPin()
{
	return m_EncoderOutPin;
}

//This function connects the sourcefilter to the encoder and the encoder to the (AVI) Mux filter (which in turn
//should be connected to the filewriter filter)
HRESULT EMWinRealtimeAudioEncoderDescriptor::LinkAndCreateEncoderFilter(IPin* p_opSourcePin, IPin* p_opMuxPin, IGraphBuilder* p_opGraphBuilder)
{
	HRESULT h;

	//RECREATE THE SAME ENCODER---
	if(m_opEncoderFilter != NULL)
	{
		m_opEncoderFilter -> Release();
		m_opEncoderFilter = NULL;
	}

	h = m_opIMoniker -> BindToObject(0, 0, IID_IBaseFilter, (void**) &m_opEncoderFilter);
	if(FAILED(h))
		eo << "ERROR! Couldn't recreate Encoder-Filter!" << ef;
	m_EncoderInPin = GetPin(m_opEncoderFilter, PINDIR_INPUT);
	m_EncoderOutPin = GetPin(m_opEncoderFilter, PINDIR_OUTPUT);
	//RECREATION DONE-------------

	m_opSourceOutPin = p_opSourcePin;
	m_opMuxInPin = p_opMuxPin;

	//Add the encoder filter to the Filter Graph
	h = p_opGraphBuilder -> AddFilter(m_opEncoderFilter, L"Encoder filter");
	if(FAILED(h))
	{
		EMDebugger("ERROR! Failed to add encoder filter to filter graph!");
		return false;
	}

	//Connect the source with the encoder
	h = p_opGraphBuilder -> Connect(p_opSourcePin, m_EncoderInPin);
	if(h != S_OK)
	{
		EMDebugger("ERROR! Failed to Connect the Source Filter with the encoder filter!");
	}

	EMWinMediaSingletonMuxer::Instance() -> SetCompressionCapability();

	//Connect the source with the encoder
	h = p_opGraphBuilder -> Connect(m_EncoderOutPin, p_opMuxPin);
	if(h != S_OK)
	{
		EMDebugger("ERROR! Failed to Connect the encoder filter with the Mux Filter!");
	}
	return h;
}

//After that the encoder filter has been disconnected and removed the filter has to use LinkAndCreateEncoderFilter
//in order to make the filtergraph one with the encoder again
HRESULT EMWinRealtimeAudioEncoderDescriptor::DisLinkAndRemoveEncoderFilter(IGraphBuilder* p_opGraphBuilder)
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
