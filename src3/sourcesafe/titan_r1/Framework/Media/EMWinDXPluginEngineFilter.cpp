#include "EMWinDXPluginEngineFilter.h"

#include "EMWinDXPluginEngineExport.h"
#include "EMWinDXPluginEngineOutputPin.h"
#include "EMWinDXPluginEngineInputPin.h"

#include "EMGlobals.h"

static const GUID CLSID_EMWinDXPluginEngineFilter = { 0x2c56a781, 0x5c3b, 0x11d5, { 0xbc, 0x75, 0x0, 0x2, 0xb3, 0x17, 0xf3, 0xf2 } };
static const GUID IID_IEMWinDXPluginEngineInterface = { 0x1e90d8e1, 0x5c3c, 0x11d5, { 0xbc, 0x75, 0x0, 0x2, 0xb3, 0x17, 0xf3, 0xf2 } };

CUnknown* WINAPI EMWinDXPluginEngineFilter::CreateInstance(LPUNKNOWN lpunk, HRESULT *phr)
{
	CUnknown *punk = EM_new EMWinDXPluginEngineFilter(NAME(""), lpunk, CLSID_EMWinDXPluginEngineFilter, NULL);
	if(punk == NULL) 
	{
		*phr = E_OUTOFMEMORY;
	}
    return punk;
}

EMWinDXPluginEngineFilter::EMWinDXPluginEngineFilter(TCHAR *pName, LPUNKNOWN lpunk, CLSID clsid, HRESULT *phr) 
	:	CBaseFilter(pName, lpunk, &m_cStateLock, clsid),
		m_opOutgoingAudioStream(NULL),
		m_opIncomingAudioStream(NULL),
		m_evFilterStoppingEvent(TRUE),
		m_oName("Untitled")
{
	m_opOutgoingAudioStream = EMWinDXPluginEngineOutputPin::Create(this);
	m_opIncomingAudioStream = EMWinDXPluginEngineInputPin::Create(this);

	if(NULL == (HANDLE) m_evFilterStoppingEvent)
	{ 
        *phr = E_FAIL;
    }
}

STDMETHODIMP EMWinDXPluginEngineFilter::SetName(string p_oName)
{
	m_oName = p_oName;
	m_opOutgoingAudioStream -> SetName(m_oName);
	m_opIncomingAudioStream -> SetName(m_oName);
	return S_OK;
}

EMWinDXPluginEngineFilter::~EMWinDXPluginEngineFilter()
{
	delete m_opOutgoingAudioStream;
	delete m_opIncomingAudioStream;
}

int EMWinDXPluginEngineFilter::GetPinCount()
{
	return 2;
}

CBasePin* EMWinDXPluginEngineFilter::GetPin(int n)
{
	if(n == 0)
		return m_opIncomingAudioStream;
	else
		return m_opOutgoingAudioStream;
}

STDMETHODIMP EMWinDXPluginEngineFilter::JoinFilterGraph(IFilterGraph* pGraph, LPCWSTR pName)
{
    CAutoLock lock(&m_cStateLock);

    HRESULT hr;
//    int nCurrentPin;
//    CDynamicSourceStream* pOutputPin;

	if( NULL != pGraph )
	{
		IGraphConfig* pGraphConfig = NULL;

		hr = pGraph->QueryInterface( IID_IGraphConfig, (void**)&pGraphConfig );
		if( FAILED( hr ) )
		{
			return hr;
		}
		hr = CBaseFilter::JoinFilterGraph( pGraph, pName );
		if( FAILED( hr ) )
		{
	//            pGraphConfig->Release();
			return hr;
		}

		//for(nCurrentPin = 0; nCurrentPin < GetPinCount(); nCurrentPin++) 
		//{
			//pOutputPin = (CDynamicSourceStream*)GetPin(nCurrentPin);
			//pOutputPin->SetConfigInfo( pGraphConfig, m_evFilterStoppingEvent );
		//}

		 pGraphConfig -> Release();
	}
	else
	{
		hr = CBaseFilter::JoinFilterGraph( pGraph, pName );
		if( FAILED( hr ) )
		{
			return hr;
		}
		//for(nCurrentPin = 0; nCurrentPin < GetPinCount(); nCurrentPin++) 
		//{
		//	pOutputPin = (CDynamicSourceStream*)GetPin(nCurrentPin);
		//	pOutputPin->SetConfigInfo( NULL, NULL );
		//}
	}

/*
	eo << (char*) "Now connecting output pin with input pin! (Strange connection!)" << ef;
	hr = pGraph -> ConnectDirect(m_opOutgoingAudioStream, m_opIncomingAudioStream, NULL);
	if(FAILED(hr))
	{
		eo << (char*) "Sorry! It didn't work!" << ef;
		return S_OK;
	}
	else
		eo << (char*) "Yeah! It worked!" << ef;

*/

	return S_OK;
}

STDMETHODIMP EMWinDXPluginEngineFilter::Stop()
{
	return CBaseFilter::Stop();
}

STDMETHODIMP EMWinDXPluginEngineFilter::Pause()
{
	return CBaseFilter::Pause();
}

HRESULT EMWinDXPluginEngineFilter::NonDelegatingQueryInterface(REFIID riid, void **ppv)
{
	if(riid == IID_IEMWinDXPluginEngineInterface)
	{
		*ppv = (EMWinDXPluginEngineInterface*) this;
		AddRef();
		return S_OK;
	}
	else 
		return CBaseFilter::NonDelegatingQueryInterface(riid, ppv);
}

STDMETHODIMP EMWinDXPluginEngineFilter::ProcessBuffer(EMMediaDataBuffer* p_opBuffer)
{
	if(! m_opOutgoingAudioStream -> ProcessBuffer(p_opBuffer))
		return E_FAIL;
	return S_OK;
}

STDMETHODIMP EMWinDXPluginEngineFilter::AssertBufferProcessed()
{
	if(! m_opIncomingAudioStream -> AssertBufferProcessed())
		return E_FAIL;
	return S_OK;
}

STDMETHODIMP EMWinDXPluginEngineFilter::SetTargetBuffer(EMMediaDataBuffer* p_opBuffer)
{
	if(! m_opIncomingAudioStream -> SetTargetBuffer(p_opBuffer))
		return E_FAIL;
	return S_OK;
}

STDAPI DllRegisterServer()
{
	return AMovieDllRegisterServer2(TRUE);
}

STDAPI DllUnregisterServer()
{
	return AMovieDllRegisterServer2(FALSE);
}

CFactoryTemplate g_Templates[] = 
{
	{L"", &CLSID_EMWinDXPluginEngineFilter, EMWinDXPluginEngineFilter::CreateInstance, NULL, &sFilterInformation}
};
int g_cTemplates = 1;

