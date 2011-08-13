#include "EMWinDXAudioOutput.h"
#include "EMWinDXAudioOutputFilter.h"
#include "EMWinDXAudioOutputInterface.h"

#include <objbase.h>

//#include "EMGlobals.h"
//#include <objbase.h>
//#include <Windows.h>

EMWinDXAudioOutput::EMWinDXAudioOutput(const char* p_vpName)
	:	m_opSourceFilterInterface(NULL),
		m_opFilterGraph(NULL),
		m_opSourceFilter(NULL),
		m_opPhysicalOutputFilter(NULL),
		m_opSourceFilterOutputPin(NULL),
		m_opPhysicalOutputInputPin(NULL),
		m_opGraphControl(NULL),
		m_vIsHookedUp(false),
		m_oName(p_vpName),
		m_opRefClock(NULL)
{
}

EMWinDXAudioOutput::~EMWinDXAudioOutput() 
{
}

bool EMWinDXAudioOutput::InitCheckE(IBaseFilter* p_opFilter)
{
	if(m_vIsHookedUp)
		return false; //Can't hook up when already hooked up!
	m_opPhysicalOutputFilter = p_opFilter;
	return Setup();
}

bool EMWinDXAudioOutput::ProcessBuffer(EMMediaDataBuffer* p_opBuffer)
{
	try
	{
		if(! m_vIsHookedUp)
		{
			EMDebugger("ERROR! EMWinDXAudioOutput can't process buffer if the filter graph isn't set up!");
			return false;
		}

/*		OAFilterState uState;
		HRESULT hr = m_opGraphControl -> GetState(1, &uState);
		if(uState == State_Stopped)
			eo << "PUT INFORMATION (code=" << (int) hr << "): Physical out \"" << m_oName.c_str() << "\" is stopped!" << ef;
		else if(uState == State_Paused)
			eo << "PUT INFORMATION (code=" << (int) hr << "): Physical out \"" << m_oName.c_str() << "\" is paused!" << ef;
		else if(uState == State_Running)
			eo << "PUT INFORMATION (code=" << (int) hr << "): Physical out \"" << m_oName.c_str() << "\" is running!" << ef;
		else
			eo << "PUT ERROR       (code=" << (int) hr << "): Physical out \"" << m_oName.c_str() << "\" is in an unknown state!" << ef;
*/
		if(FAILED(m_opSourceFilterInterface -> ProcessBuffer(p_opBuffer)))
		{
			EMDebugger("ERROR! EMWinDXAudioOutput failed to process buffer in source filter!");
			return false;
		}
	}
	catch(...)
	{
		EMDebugger("ERROR! Exception while processing buffer in DirectX audio output filter!");
	}
	return true;
}

//There is one exact copy if this function in EMWinDXPluginEngine
bool FindPin(IBaseFilter* p_opFilter, PIN_DIRECTION p_upDirection, IPin** p_oppPinResult)
{
	(*p_oppPinResult) = NULL;
	IEnumPins* opEnumPins = NULL;
	HRESULT vResult = p_opFilter -> EnumPins(&opEnumPins);
	if(FAILED(vResult) || opEnumPins == NULL)
		EMDebugger("ERROR! Failed to ask filter for pins");
	IPin* opSomePin = NULL;
	uint32 vFetched = 0;
	while(opEnumPins -> Next(1, &opSomePin, &vFetched) == S_OK)
	{
		PIN_INFO uPinInfo;
		opSomePin -> QueryPinInfo(&uPinInfo);
		uPinInfo.pFilter -> Release();
		if(uPinInfo.dir == p_upDirection)
		{
			(*p_oppPinResult) = opSomePin;
			break;
		}
	}
	opEnumPins -> Release();
	if((*p_oppPinResult) != NULL)
		return true;
	return false;
}

bool EMWinDXAudioOutput::Setup()
{
	if(m_opPhysicalOutputFilter == NULL)
		return false;

	//BEGIN SET UP INSTANCES

	HRESULT vResult = CoCreateInstance(CLSID_EMWinDXAudioOutputFilter, NULL, CLSCTX_INPROC, IID_IBaseFilter, (void**) &m_opSourceFilter);
	if(FAILED(vResult)) EMDebugger("ERROR! Failed to create first instance of EMWinDXAudioOutput filter!");

	vResult = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC, IID_IFilterGraph2, (void**) &m_opFilterGraph);
	if(FAILED(vResult)) EMDebugger("ERROR! Failed to create instance of filter graph!");
	
	//END SET UP INSTANCES

	//BEGIN FETCH INTERFACES

	vResult = m_opSourceFilter -> QueryInterface(IID_IEMWinDXAudioOutputInterface, (void**) &m_opSourceFilterInterface);
	if(FAILED(vResult)) EMDebugger("ERROR! Failed to get first WinDXAudioOutput interface!");

	vResult = m_opFilterGraph -> QueryInterface(IID_IMediaControl, (void**) &m_opGraphControl);
	if(FAILED(vResult)) EMDebugger("ERROR! Failed to get control object for filter graph!");

	vResult = m_opPhysicalOutputFilter -> QueryInterface(IID_IReferenceClock, (void**) &m_opRefClock);
	if(FAILED(vResult)) 
	{
		return false;
	}

	m_opSourceFilterInterface -> SetName(m_oName.c_str());
	m_opSourceFilterInterface -> SetRefClock(m_opRefClock);
	vResult = m_opSourceFilter -> SetSyncSource(m_opRefClock);
	if(FAILED(vResult)) 
	{
		MessageBox(NULL, "Failed to set the ref clock for source filter!", "Warning", MB_OK);
		return false;
	}
	
	//END FETCH INTERFACES

	//BEGIN ADD FILTERS

	vResult = m_opFilterGraph -> AddFilter(m_opSourceFilter, L"Start");
	if(FAILED(vResult)) EMDebugger("ERROR! Failed to add source filter to graph!");

	vResult = m_opFilterGraph -> AddFilter(m_opPhysicalOutputFilter, L"Plugin");
	if(FAILED(vResult)) EMDebugger("ERROR! Failed to add physical output to graph!");

	//END ADD FILTERS

	//BEGIN FIND PINS
	
/*	IEnumPins* opEnumPins = NULL;
	m_opSourceFilter -> QueryInterface(IID_IEnumPins, (void**) &opEnumPins);
	IPin* opSomePin = NULL;
	int32 vFetched = 0;
	while(opEnumPins -> Next(1, &opSomePin, &vFetched) == S_OK)
	{
		opSomePin -> Release();
		PIN_INFO uPinInfo;
		opSomePin -> QueryPinInfo(&uPinInfo);
		if(uPinInfo.dir = PINDIR_OUTPUT)
		{
			m_opSourceFilterOutputPin = opSomePin;
			break;
		}
	}
	opEnumPins -> Release();
*/
	bool vBoolResult = FindPin(m_opSourceFilter, PINDIR_OUTPUT, &m_opSourceFilterOutputPin);
	if(! vBoolResult) EMDebugger("ERROR! Failed to find an output pin on the source filter!");

	vBoolResult = FindPin(m_opPhysicalOutputFilter, PINDIR_INPUT, &m_opPhysicalOutputInputPin);
	if(! vBoolResult) EMDebugger("ERROR! Failed to find an input pin in to the physical output!");
		
	//END FIND PINS

	//BEGIN CONNECT

	vResult = m_opFilterGraph -> ConnectDirect(m_opSourceFilterOutputPin, m_opPhysicalOutputInputPin, NULL);
	if(FAILED(vResult)) 
	{
//		MessageBox(NULL, "Failed to connect the source filter with the physical output!\nThis output will not be used!", "Warning", MB_OK);
//		eo << "Failed to connect the source filter with the physical output! This output will not be used!" << ef;
//		eo << "     Error code: " << (int) vResult << ef;
		return false;
	}

	//END CONNECT

	//This LINE FAILS on Jespers computer !?!?!?!?!?!?!
	::Sleep(5);
	vResult = m_opGraphControl -> Pause();
	::Sleep(5);
	vResult = m_opGraphControl -> Run();
	::Sleep(5);

	OAFilterState uState;
	
	while(m_opGraphControl -> GetState(1, &uState) == VFW_S_STATE_INTERMEDIATE && uState == State_Stopped) // && uState != State_Running)
		::Sleep(25);
	m_vIsHookedUp = true;
	return true;
}

bool EMWinDXAudioOutput::Shutdown()
{
	uint32 vRefCount = 0;
	HRESULT vResult = S_OK;

	if(m_vIsHookedUp)
	{
		if(m_opGraphControl != NULL)
		{
			vResult = m_opGraphControl -> Stop();
			if(FAILED(vResult)) EMDebugger("ERROR! Failed to put the filter graph in Stop mode!");
		}
	}

	if(m_opSourceFilterInterface != NULL)
		m_opSourceFilterInterface -> Shutdown();
	
	if(m_opGraphControl != NULL)
		vRefCount = m_opGraphControl -> Release();
	
	if(m_opFilterGraph != NULL)
		vRefCount = m_opFilterGraph -> Release();
	
	if(m_opSourceFilterOutputPin != NULL)
		vRefCount = m_opSourceFilterOutputPin -> Release();
	
	if(m_opPhysicalOutputInputPin != NULL)
		vRefCount = m_opPhysicalOutputInputPin -> Release();

	if(m_opSourceFilter != NULL)
		vRefCount = m_opSourceFilter -> Release();

	if(m_opSourceFilterInterface != NULL)
		vRefCount = m_opSourceFilterInterface -> Release();
	
	if(m_opPhysicalOutputFilter != NULL)
		vRefCount = m_opPhysicalOutputFilter -> Release();
	
	if(m_opRefClock != NULL)
	{
		vRefCount = m_opRefClock -> Release();
	}
	
	m_opPhysicalOutputFilter = NULL;
	m_vIsHookedUp = false;
	return true;	
}

void EMWinDXAudioOutput::ProjectStarted(int64 p_vSystemTimeWhenStarted)
{
	if(m_vIsHookedUp)
	{
		m_opSourceFilterInterface -> ProjectStarted(p_vSystemTimeWhenStarted);
	}
}

void EMWinDXAudioOutput::ProjectStopped()
{
	if(m_vIsHookedUp)
	{
		m_opSourceFilterInterface -> ProjectStopped();
	}
}

void EMWinDXAudioOutput::Flush()
{
	if(! m_vIsHookedUp)
		return; //EMDebugger("ERROR! EMWinDXAudioOutput can't flush if the filter graph isn't set up!");
	try
	{
//		m_opGraphControl -> Stop();
		m_opSourceFilterInterface -> Flush();
//		m_opGraphControl -> Run();
	}
	catch(...)
	{
		EMDebugger("ERROR! Exception while flushing DirectX audio output filter!");
	}
}

bool EMWinDXAudioOutput::RegisterAsClockMaster(int32 p_vWithRealtimeOutputID)
{
	if(SUCCEEDED(m_opSourceFilterInterface -> RegisterAsClockMaster(p_vWithRealtimeOutputID)))
		return true;
	return false;
}

bool EMWinDXAudioOutput::UnregisterAsClockMaster()
{
	if(SUCCEEDED(m_opSourceFilterInterface -> UnregisterAsClockMaster()))
		return true;
	return false;
}
