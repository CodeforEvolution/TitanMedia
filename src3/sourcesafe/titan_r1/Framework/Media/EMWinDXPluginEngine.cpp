 #include "EMWinDXPluginEngine.h"
#include "EMWinDXPluginEngineFilter.h"
#include "EMWinDXPluginEngineInterface.h"
#include "EMMediaEngine.h"
#include "EMCommandRepository.h"
#include "CommandIDs.h"

EMWinDXPluginEngine::EMWinDXPluginEngine(const char* p_vpName)
	:	m_opSourceFilterInterface(NULL),
		m_opTargetFilterInterface(NULL),
		m_opFilterGraph(NULL),
		m_opSourceFilter(NULL),
		m_opDirectXPlugin(NULL),
		m_opTargetFilter(NULL),
		m_opSourceFilterOutputPin(NULL),
		m_opDirectXFilterInputPin(NULL),
		m_opDirectXFilterOutputPin(NULL),
		m_opTargetFilterInputPin(NULL),
		m_opGraphControl(NULL),
		m_vIsHookedUp(false),
		m_oName(p_vpName)
{
}

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
		int32 v = uPinInfo.pFilter -> Release();
		if(uPinInfo.dir == p_upDirection)
		{
			(*p_oppPinResult) = opSomePin;
			break;
		}
		opSomePin -> Release();
	}
	int32 v = opEnumPins -> Release();
	if((*p_oppPinResult) != NULL)
		return true;
	return false;
}

EMWinDXPluginEngine::~EMWinDXPluginEngine()
{
	if(m_vIsHookedUp)
	{
		//Make sure we stop the graph before we do anything!
		if(m_opGraphControl != NULL)
		{
			OAFilterState uState;
			if(m_opGraphControl -> GetState(0, &uState) != E_FAIL)
			{
				if(uState == State_Running || uState == State_Paused)
				{
					if(FAILED(m_opGraphControl -> Stop()))
					{
						MessageBox(NULL, "WARNING! Failed to stop plugin process! May cause deadlocks!", "Warning", MB_OK);
					}

					while(m_opGraphControl -> GetState(0, &uState) == VFW_S_STATE_INTERMEDIATE && uState != State_Stopped)
						::Sleep(10);
				}
			}
		}
		
		if(m_opGraphControl != NULL)																	//1
			int32 v = m_opGraphControl-> Release();

		if(m_opFilterGraph != NULL)																		//0
			int32 v = m_opFilterGraph -> Release();  //Hang here sometimes, when opening new plugin

		if(m_opTargetFilterInputPin != NULL)															//1
			int32 v = m_opTargetFilterInputPin -> Release();

		if(m_opDirectXFilterOutputPin != NULL)															//2
			int32 v = m_opDirectXFilterOutputPin -> Release();

		if(m_opDirectXFilterInputPin != NULL)															//1
			int32 v = m_opDirectXFilterInputPin -> Release();
		
		if(m_opSourceFilterOutputPin != NULL)															//1
			int32 v = m_opSourceFilterOutputPin -> Release();

		if(m_opSourceFilterInterface != NULL)
			int32 v = m_opSourceFilterInterface -> Release();
	
		if(m_opTargetFilterInterface != NULL)
			int32 v = m_opTargetFilterInterface -> Release();

		if(m_opTargetFilter != NULL)																	//0
			int32 v = m_opTargetFilter -> Release();
		
		if(m_opSourceFilter != NULL)																	//0
			int32 v = m_opSourceFilter -> Release();

		if(m_opDirectXPlugin != NULL)																	//0
			int32 v = m_opDirectXPlugin -> Release();
	}
}

bool EMWinDXPluginEngine::InitCheckE(IBaseFilter* p_opFilter)
{
	HRESULT vResult = S_OK;
	if(m_vIsHookedUp)
		return false; //Can't hook up when already hooked up!

	m_opDirectXPlugin = p_opFilter;
	//BEGIN SET UP INSTANCES
	vResult = CoCreateInstance(CLSID_EMWinDXPluginEngineFilter, NULL, CLSCTX_INPROC, IID_IBaseFilter, (void**) &m_opSourceFilter);
	if(FAILED(vResult))
	{
		MessageBox(NULL, "Failed to activate DirectX audio plugin!\nMake sure you have installed and registered\n all of Titan's components!", "Warning", MB_OK);
		int32 v = m_opDirectXPlugin -> Release();
		m_opDirectXPlugin = NULL;
		return false;
	}

	vResult = CoCreateInstance(CLSID_EMWinDXPluginEngineFilter, NULL, CLSCTX_INPROC, IID_IBaseFilter, (void**) &m_opTargetFilter);
	if(FAILED(vResult)) 
	{
		MessageBox(NULL, "Failed to activate DirectX audio plugin!\nMake sure you have installed and registered\n all of Titan's components!", "Warning", MB_OK);
		int32 v = m_opSourceFilter -> Release();
		m_opSourceFilter = NULL;
		v = m_opDirectXPlugin -> Release();
		m_opDirectXPlugin = NULL;
		return false;
	}

	vResult = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC, IID_IFilterGraph2, (void**) &m_opFilterGraph);
	if(FAILED(vResult))
	{
		MessageBox(NULL, "Failed to activate DirectX audio plugin!\nMake sure you have installed and registered\n all of Titan's components!", "Warning", MB_OK);
		int32 v = m_opTargetFilter -> Release();
		m_opTargetFilter = NULL;
		v = m_opSourceFilter -> Release();
		m_opSourceFilter = NULL;
		v = m_opDirectXPlugin -> Release();
		m_opDirectXPlugin = NULL;
		return false;
	}
	
	//END SET UP INSTANCES

	//BEGIN FETCH INTERFACES

	vResult = m_opSourceFilter -> QueryInterface(IID_IEMWinDXPluginEngineInterface, (void**) &m_opSourceFilterInterface);
	if(FAILED(vResult)) EMDebugger("ERROR! Failed to get first WinDXPluginEngine interface!");

	vResult = m_opTargetFilter -> QueryInterface(IID_IEMWinDXPluginEngineInterface, (void**) &m_opTargetFilterInterface);
	if(FAILED(vResult)) EMDebugger("ERROR! Failed to get second WinDXPluginEngine interface!");

	vResult = m_opFilterGraph -> QueryInterface(IID_IMediaControl, (void**) &m_opGraphControl);
	if(FAILED(vResult)) EMDebugger("ERROR! Failed to get control object for filter graph!");

	m_opSourceFilterInterface -> SetName(m_oName);
	m_opTargetFilterInterface -> SetName(m_oName);

	//END FETCH INTERFACES

	//BEGIN ADD FILTERS

	vResult = m_opFilterGraph -> AddFilter(m_opSourceFilter, L"Start");
	if(FAILED(vResult)) EMDebugger("ERROR! Failed to add source filter to graph!");

	vResult = m_opFilterGraph -> AddFilter(m_opTargetFilter, L"End");
	if(FAILED(vResult)) EMDebugger("ERROR! Failed to add DirectX plugin filter to graph!");

	vResult = m_opFilterGraph -> AddFilter(m_opDirectXPlugin, L"Plugin");
	if(FAILED(vResult)) EMDebugger("ERROR! Failed to add target filter to graph!");

	//END ADD FILTERS

	//BEGIN FIND PINS

	bool vBoolResult = FindPin(m_opSourceFilter, PINDIR_OUTPUT, &m_opSourceFilterOutputPin);
	if(! vBoolResult) EMDebugger("ERROR! Failed to find an output pin on the source filter!");

	vBoolResult = FindPin(m_opDirectXPlugin, PINDIR_INPUT, &m_opDirectXFilterInputPin);
	if(! vBoolResult) EMDebugger("ERROR! Failed to find an input pin in to the DirectX plugin filter!");
	
	vBoolResult = FindPin(m_opDirectXPlugin, PINDIR_OUTPUT, &m_opDirectXFilterOutputPin);
	if(! vBoolResult) EMDebugger("ERROR! Failed to find an output pin from the DirectX plugin filter!");
	
	vBoolResult = FindPin(m_opTargetFilter, PINDIR_INPUT, &m_opTargetFilterInputPin);
	if(! vBoolResult) EMDebugger("ERROR! Failed to find an input pin in to the target filter!");

	//END FIND PINS

	//BEGIN CONNECT

	IEnumMediaTypes* opEnumTypes = NULL;
	vResult = m_opSourceFilterOutputPin -> EnumMediaTypes(&opEnumTypes);

	uint32 vFetched = 0;
	AM_MEDIA_TYPE* uMediaType = NULL;
	while(opEnumTypes -> Next(1, &uMediaType, &vFetched) == S_OK)
	{
		GUID type = uMediaType -> formattype;
		if(type == FORMAT_WaveFormatEx)
		{
			WAVEFORMATEX* uWaveFmt = (WAVEFORMATEX*) (uMediaType -> pbFormat);
			int32 vChans = uWaveFmt -> nChannels;
			int32 vFRate = uWaveFmt -> nSamplesPerSec;
			int32 vBPSec = uWaveFmt -> nAvgBytesPerSec;
		}
	}
	int32 v = opEnumTypes -> Release();

	vResult = m_opFilterGraph -> ConnectDirect(m_opSourceFilterOutputPin, m_opDirectXFilterInputPin, NULL);
	if(FAILED(vResult)) 
	{
		MessageBox(NULL, string(string("Failed to activate DirectX audio plugin!\n") + m_oName + string(" will be disabled.")).c_str(), "Error", MB_OK);
		return false;
	}

	vResult = m_opFilterGraph -> ConnectDirect(m_opDirectXFilterOutputPin, m_opTargetFilterInputPin, NULL);
	if(FAILED(vResult)) 
	{
		MessageBox(NULL, string(string("Failed to activate DirectX audio plugin!\n") + m_oName + string(" will be disabled.")).c_str(), "Error", MB_OK);
		return false;
	}

	//END CONNECT

	vResult = m_opGraphControl -> Run();
	if(FAILED(vResult)) EMDebugger("ERROR! Failed to put the filter graph in Pause mode!");

	m_vIsHookedUp = true;
	return true;
}

bool EMWinDXPluginEngine::ShowDialog()
{
	if(! m_vIsHookedUp)
		return false;

	try
	{
		ISpecifyPropertyPages *pSpecify;
		HRESULT hr = m_opDirectXPlugin -> QueryInterface(IID_ISpecifyPropertyPages, (void **)&pSpecify);
		if(SUCCEEDED(hr)) 
		{
			FILTER_INFO FilterInfo;
			hr = m_opDirectXPlugin -> QueryFilterInfo(&FilterInfo);
			if(SUCCEEDED(hr))
			{

				CAUUID caGUID;
				hr = pSpecify -> GetPages(&caGUID);

				if(SUCCEEDED(hr))
				{
					int vInt = 1;
					HWND uWindowHandle = static_cast<HWND>(EMMediaEngine::Instance() -> GetCommandRepository() -> ExecuteCommand(COMMAND_GET_APPLICATION_DATA, &vInt, NULL, NULL));
					hr = OleCreatePropertyFrame(uWindowHandle, 0, 0, NULL, 1, (IUnknown **)&m_opDirectXPlugin, caGUID.cElems, caGUID.pElems, 0/*GetSystemDefaultLCID()*/, 0, NULL);
					if(FAILED(hr))
						MessageBox(NULL, "Failed to show plugin dialog!", "Error", MB_OK);
					
					CoTaskMemFree(caGUID.pElems);
				}
				int32 v = FilterInfo.pGraph -> Release();
			}
			int32 v = pSpecify -> Release();
		} 
	}
	catch(...)
	{
		return false;
	}
	return true;
}

bool EMWinDXPluginEngine::ProcessBuffer(EMMediaDataBuffer* p_opBuffer)
{
	if(! m_vIsHookedUp)
	{
		eo << "ERROR! EMWinDXPluginEngine can't process buffer if the filter graph isn't set up!" << ef;
		return false;
	}
	if(FAILED(m_opTargetFilterInterface -> SetTargetBuffer(p_opBuffer)))
	{
		eo << "WARNING! EMWinDXPluginEngine failed to store the buffer pointer!" << ef;
		return false;
	}
	if(FAILED(m_opSourceFilterInterface -> ProcessBuffer(p_opBuffer)))
	{
		eo << "WARNING! EMWinDXPluginEngine failed to process buffer in source filter!" << ef;
		return false;
	}
	if(FAILED(m_opTargetFilterInterface -> AssertBufferProcessed()))
	{
		eo << "WARNING! EMWinDXPluginEngine processing failed!" << ef;
//		return false;
	}
	return true;
}
