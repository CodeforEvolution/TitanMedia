#include "EMWinVideoShuffleOutputFilter.h"

#include "EMWinVideoShuffleOutputExport.h"
#include "EMWinVideoShuffleOutputPin.h"

#include "EMGlobals.h"

static const GUID CLSID_EMWinVideoShuffleOutputFilter = { 0x3fc76daf, 0x80e8, 0x11d5, { 0x82, 0x59, 0x0, 0x2, 0xb3, 0x18, 0x0, 0x46 } };
static const GUID IID_IEMWinVideoShuffleOutputInterface = { 0x3fc76dae, 0x80e8, 0x11d5, { 0x82, 0x59, 0x0, 0x2, 0xb3, 0x18, 0x0, 0x46 } };

CUnknown* WINAPI EMWinVideoShuffleOutputFilter::CreateInstance(LPUNKNOWN lpunk, HRESULT *phr)
{
	CUnknown *punk = EM_new EMWinVideoShuffleOutputFilter(NAME(""), lpunk, CLSID_EMWinVideoShuffleOutputFilter, NULL);
	if(punk == NULL) 
	{
		*phr = E_OUTOFMEMORY;
	}
    return punk;
}
EMWinVideoShuffleOutputFilter::EMWinVideoShuffleOutputFilter(TCHAR *pName, LPUNKNOWN lpunk, CLSID clsid, HRESULT *phr) 
	:	CBaseFilter(pName, lpunk, &m_cStateLock, clsid),
		m_opOutgoingVideoStream(NULL),
		m_evFilterStoppingEvent(TRUE),
		m_oName("Untitled")
{
	m_opOutgoingVideoStream = EMWinVideoShuffleOutputPin::Create(this);

	if(NULL == (HANDLE) m_evFilterStoppingEvent)
        *phr = E_FAIL;
 
//	m_opOutgoingVideoStream -> StartProcessing();
}

EMWinVideoShuffleOutputFilter::~EMWinVideoShuffleOutputFilter()
{
//	m_opOutgoingVideoStream -> StopProcessing();
	delete m_opOutgoingVideoStream;
}

STDMETHODIMP EMWinVideoShuffleOutputFilter::SetTimeStamp(REFERENCE_TIME p_vRefTime)
{
	m_opOutgoingVideoStream -> SetTimeStamp(p_vRefTime);
	return S_OK;
}

STDMETHODIMP EMWinVideoShuffleOutputFilter::SetName(string p_oName)
{
	m_oName = p_oName;
	m_opOutgoingVideoStream -> SetName(m_oName);
	return S_OK;
}

string EMWinVideoShuffleOutputFilter::GetName() const
{
	return m_oName;
}

int EMWinVideoShuffleOutputFilter::GetPinCount()
{
	return 1;
}

CBasePin* EMWinVideoShuffleOutputFilter::GetPin(int n)
{
	if(n == 0)
		return m_opOutgoingVideoStream;
	else
		return NULL;
}


STDMETHODIMP EMWinVideoShuffleOutputFilter::RegisterAsClockMaster(int32 p_vRealtimeOutputDescriptorID)
{
	m_opOutgoingVideoStream -> RegisterAsClockMaster(p_vRealtimeOutputDescriptorID);
	return S_OK;
}

STDMETHODIMP EMWinVideoShuffleOutputFilter::UnregisterAsClockMaster()
{
	m_opOutgoingVideoStream -> UnregisterAsClockMaster();
	return S_OK;
}

STDMETHODIMP EMWinVideoShuffleOutputFilter::JoinFilterGraph(IFilterGraph* pGraph, LPCWSTR pName)
{
    CAutoLock lock(GetStateLock());

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
	hr = pGraph -> ConnectDirect(m_opOutgoingVideoStream, m_opIncomingAudioStream, NULL);
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

STDMETHODIMP EMWinVideoShuffleOutputFilter::Stop()
{
	CAutoLock lock(GetStateLock());
	return CBaseFilter::Stop();
}

STDMETHODIMP EMWinVideoShuffleOutputFilter::Pause()
{
	CAutoLock lock(GetStateLock());
	return CBaseFilter::Pause();
}

HRESULT EMWinVideoShuffleOutputFilter::NonDelegatingQueryInterface(REFIID riid, void **ppv)
{
	if(riid == IID_IEMWinVideoShuffleOutputInterface)
	{
		this -> AddRef();
		*ppv = (EMWinVideoShuffleOutputInterface*) this;
		return S_OK;
	}
	else 
		return CBaseFilter::NonDelegatingQueryInterface(riid, ppv);
}

STDMETHODIMP EMWinVideoShuffleOutputFilter::ProcessBuffer(EMMediaDataBuffer* p_opBuffer)
{
	try
	{
		bool vResult = m_opOutgoingVideoStream -> ProcessBufferE(p_opBuffer);
		if(! vResult)
		{
			//eo << "WARNING! Couldn't process buffer!" << ef;
		}
	}
	catch(...)
	{
		//Some serious error occurred... Probably!
		return E_FAIL;
	}
	return S_OK;
}

STDMETHODIMP EMWinVideoShuffleOutputFilter::SetShuffleFormat(EMMediaFormat* p_opMediaFormat)
{
	m_opOutgoingVideoStream->SetShuffleFormat(p_opMediaFormat);
	return S_OK;
}

STDMETHODIMP EMWinVideoShuffleOutputFilter::StartThread()
{
	return m_opOutgoingVideoStream->StartThread();
}

STDMETHODIMP EMWinVideoShuffleOutputFilter::EndThread()
{
	return m_opOutgoingVideoStream->EndThread();
}


STDMETHODIMP EMWinVideoShuffleOutputFilter::Flush()
{
//	eo << "EMWinVideoShuffleOutputFilter::Flush()" << ef;
	m_opOutgoingVideoStream -> Flush();
//	CAutoLock lock(&GetStateLock());
//	m_opOutgoingVideoStream -> EndOfStream();
//	m_opOutgoingVideoStream -> EndFlush();
	return S_OK;
}

/*STDMETHODIMP EMWinVideoShuffleOutputFilter::Shutdown()
{
//	m_opOutgoingVideoStream -> Shutdown();
	return S_OK;
}
*/

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
	{L"", &CLSID_EMWinVideoShuffleOutputFilter, EMWinVideoShuffleOutputFilter::CreateInstance, NULL, &sFilterInformation}
};
int g_cTemplates = 1;

