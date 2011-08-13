#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#ifndef __EM_WIN_DX_PLUGIN_ENGINE_OUTPUT_PIN
#define __EM_WIN_DX_PLUGIN_ENGINE_OUTPUT_PIN

#include "EMWinDXPluginEngineFilter.h"

#include <streams.h>

class EMMediaDataBuffer;

class EMWinDXPluginEngineOutputPin : public CBaseOutputPin
{
public:
	static EMWinDXPluginEngineOutputPin* Create(EMWinDXPluginEngineFilter* p_opParentFilter);
	virtual ~EMWinDXPluginEngineOutputPin();
	virtual HRESULT GetMediaType(int p_vPosition, CMediaType *pMediaType);
	virtual HRESULT CompleteConnect(IPin *pReceivePin);
	virtual HRESULT BreakConnect(void);
	virtual HRESULT Active(void);
	virtual HRESULT DecideBufferSize(IMemAllocator *pAlloc, ALLOCATOR_PROPERTIES *ppropInputRequest);
	STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void **ppv);
	HRESULT CheckMediaType(const CMediaType *pMediaType);
	bool ProcessBuffer(EMMediaDataBuffer* p_opBuffer);
	void SetName(string p_oName);

protected:
	EMWinDXPluginEngineOutputPin(HRESULT *phr, EMWinDXPluginEngineFilter* p_opParentFilter, LPCWSTR pName);
	CMediaType* m_opConnectedMediaType;
	CCritSec m_cSharedState;

	EMWinDXPluginEngineFilter* m_opParentFilter;
	string m_oFilterName;

	uint32 m_vNumberOfOutChannels;
};

#endif

#endif