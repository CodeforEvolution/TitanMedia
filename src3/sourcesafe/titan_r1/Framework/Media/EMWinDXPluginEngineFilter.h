#ifndef __EM_WIN_DX_PLUGIN_ENGINE_FILTER
#define __EM_WIN_DX_PLUGIN_ENGINE_FILTER

#include "EMGlobals.h"

#include "EMWinDXPluginEngineInterface.h"

#include <streams.h>

class EMWinDXPluginEngineOutputPin;
class EMWinDXPluginEngineInputPin;
class EMMediaDataBuffer;

DEFINE_GUID(CLSID_EMWinDXPluginEngineFilter, 0x2c56a781, 0x5c3b, 0x11d5, 0xbc, 0x75, 0x0, 0x2, 0xb3, 0x17, 0xf3, 0xf2);
DEFINE_GUID(IID_IEMWinDXPluginEngineInterface,  0x1e90d8e1, 0x5c3c, 0x11d5, 0xbc, 0x75, 0x0, 0x2, 0xb3, 0x17, 0xf3, 0xf2);

class EMWinDXPluginEngineFilter : public CBaseFilter, public EMWinDXPluginEngineInterface
{
public:
	static CUnknown* WINAPI CreateInstance(LPUNKNOWN lpunk, HRESULT *phr);
	virtual ~EMWinDXPluginEngineFilter();

	DECLARE_IUNKNOWN

    int GetPinCount();
    CBasePin *GetPin(int n);
    STDMETHODIMP JoinFilterGraph(IFilterGraph* pGraph, LPCWSTR pName);
    STDMETHODIMP Stop();
    STDMETHODIMP Pause();

	STDMETHODIMP ProcessBuffer(EMMediaDataBuffer* p_opBuffer);
	STDMETHODIMP AssertBufferProcessed();
	STDMETHODIMP SetTargetBuffer(EMMediaDataBuffer* p_opBuffer);

	CCritSec* GetLock() { return &m_cStateLock; };

	STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void **ppv);
	STDMETHODIMP SetName(string p_oName);

private:
	EMWinDXPluginEngineFilter(TCHAR *pName, LPUNKNOWN lpunk, CLSID clsid, HRESULT *phr);
	CCritSec m_cStateLock;	
	EMWinDXPluginEngineOutputPin* m_opOutgoingAudioStream;
	EMWinDXPluginEngineInputPin* m_opIncomingAudioStream;

	CAMEvent m_evFilterStoppingEvent;
	string m_oName;
};

#endif