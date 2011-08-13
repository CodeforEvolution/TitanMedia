#include "EMWinDXAudioOutputFilter.h"

#include "EMWinDXAudioOutputExport.h"
#include "EMWinDXAudioOutputOutputPin.h"
#include "EMMediaIDManager.h"
#include "EMMediaTimer.h"
#include "EMGlobals.h"

static const GUID CLSID_EMWinDXAudioOutputFilter = { 0x53f8f7a1, 0x6b3d, 0x11d5, { 0xbc, 0x75, 0x0, 0x2, 0xb3, 0x17, 0xf3, 0xf2 } };
static const GUID IID_IEMWinDXAudioOutputInterface = { 0x53f8f7a2, 0x6b3d, 0x11d5, { 0xbc, 0x75, 0x0, 0x2, 0xb3, 0x17, 0xf3, 0xf2 } };

CUnknown* WINAPI EMWinDXAudioOutputFilter::CreateInstance(LPUNKNOWN lpunk, HRESULT *phr)
{
	CUnknown *punk = EM_new EMWinDXAudioOutputFilter(NAME(const_cast<char*>(EMMediaIDManager::MakeUniqueString())), lpunk, CLSID_EMWinDXAudioOutputFilter, NULL);
	if(punk == NULL) 
	{
		*phr = E_OUTOFMEMORY;
	} 
    return punk;
}

EMWinDXAudioOutputFilter::EMWinDXAudioOutputFilter(TCHAR *pName, LPUNKNOWN lpunk, CLSID clsid, HRESULT *phr) 
	:	CBaseFilter(pName, lpunk, &m_cStateLock, clsid),
		m_opOutgoingAudioStream(NULL),
		m_vStartingTime(0),
		m_evFilterStoppingEvent(TRUE),
		m_oName("Untitled"),
		m_opRefClock(NULL),
		m_vIsClockMaster(false),
		m_vClockMasterID(-1)
{
	m_opOutgoingAudioStream = EMWinDXAudioOutputOutputPin::Create(this);

	if(NULL == (HANDLE) m_evFilterStoppingEvent)
        *phr = E_FAIL;

}

EMWinDXAudioOutputFilter::~EMWinDXAudioOutputFilter()
{
	m_opOutgoingAudioStream -> StopProcessing();
	delete m_opOutgoingAudioStream;
}

STDMETHODIMP EMWinDXAudioOutputFilter::SetName(const char* p_vpName)
{
	m_oName = string(p_vpName);
	m_opOutgoingAudioStream -> SetName(m_oName.c_str());
	return S_OK;
}

string EMWinDXAudioOutputFilter::GetName() const
{
	return m_oName;
}

int EMWinDXAudioOutputFilter::GetPinCount()
{
	return 1;
}

CBasePin* EMWinDXAudioOutputFilter::GetPin(int n)
{
	if(n == 0)
		return m_opOutgoingAudioStream;
	else
		return NULL;
}

STDMETHODIMP EMWinDXAudioOutputFilter::JoinFilterGraph(IFilterGraph* pGraph, LPCWSTR pName)
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
			EMDebugger("ERROR! DXAudioOutput failed (1) to join filter graph!");
			return hr;
		}
		hr = CBaseFilter::JoinFilterGraph( pGraph, pName );
		if( FAILED( hr ) )
		{
		    pGraphConfig->Release();
			EMDebugger("ERROR! DXAudioOutput failed (2) to join filter graph!");
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
			EMDebugger("ERROR! DXAudioOutput failed (3) to join filter graph!");
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

STDMETHODIMP EMWinDXAudioOutputFilter::Stop()
{
	CAutoLock lock(GetStateLock());
	return CBaseFilter::Stop();
}

STDMETHODIMP EMWinDXAudioOutputFilter::Pause()
{
	CAutoLock lock(GetStateLock());
	m_opOutgoingAudioStream -> StartProcessing();
	return CBaseFilter::Pause();
}

STDMETHODIMP EMWinDXAudioOutputFilter::Run(REFERENCE_TIME tStart)
{
	CAutoLock lock(GetStateLock());
	m_opOutgoingAudioStream -> StartProcessing();
	return CBaseFilter::Run(tStart);
}

HRESULT EMWinDXAudioOutputFilter::NonDelegatingQueryInterface(REFIID riid, void **ppv)
{
	if(riid == IID_IEMWinDXAudioOutputInterface)
	{
		*ppv = (EMWinDXAudioOutputInterface*) this;
		AddRef();
		return S_OK;
	}
	else 
		return CBaseFilter::NonDelegatingQueryInterface(riid, ppv);
}

STDMETHODIMP EMWinDXAudioOutputFilter::ProcessBuffer(EMMediaDataBuffer* p_opBuffer)
{
	m_opOutgoingAudioStream -> ProcessBufferE(p_opBuffer);
	return S_OK;
}

STDMETHODIMP EMWinDXAudioOutputFilter::ProjectStarted(int64 p_vSystemTimeWhenStarted)
{
	m_vStartingTime = (REFERENCE_TIME) p_vSystemTimeWhenStarted;
	return S_OK;
}

STDMETHODIMP EMWinDXAudioOutputFilter::ProjectStopped()
{
	m_vStartingTime = 0;
	return S_OK;
}

STDMETHODIMP EMWinDXAudioOutputFilter::Flush()
{
//	eo << "EMWinDXAudioOutputFilter::Flush()" << ef;
	m_vStartingTime = EMMediaTimer::Instance() -> RealTime() * 10.0;
	m_opOutgoingAudioStream -> Flush();
//	CAutoLock lock(&GetStateLock());
//	m_opOutgoingAudioStream -> EndOfStream();
//	m_opOutgoingAudioStream -> EndFlush();
	return S_OK;
}

STDMETHODIMP EMWinDXAudioOutputFilter::Shutdown()
{
	m_opOutgoingAudioStream -> Shutdown();
	return S_OK;
}

STDMETHODIMP EMWinDXAudioOutputFilter::SetRefClock(IReferenceClock* p_opClock)
{
	m_opRefClock = p_opClock;
	return S_OK;
}

IReferenceClock* EMWinDXAudioOutputFilter::GetRefClock() const
{
	return m_opRefClock;
}

STDMETHODIMP EMWinDXAudioOutputFilter::RegisterAsClockMaster(int32 p_vRealtimeOutputDescriptorID)
{
	m_vIsClockMaster = true;
	m_vClockMasterID = p_vRealtimeOutputDescriptorID;
	return S_OK;
}

STDMETHODIMP EMWinDXAudioOutputFilter::UnregisterAsClockMaster()
{
	m_vIsClockMaster = false;
	m_vClockMasterID = -1;
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
	{L"", &CLSID_EMWinDXAudioOutputFilter, EMWinDXAudioOutputFilter::CreateInstance, NULL, &sFilterInformation}
};
int g_cTemplates = 1;

