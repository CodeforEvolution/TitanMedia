#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#ifndef __EM_WIN_DX_AUDIO_OUTPUT_OUTPUT_PIN
#define __EM_WIN_DX_AUDIO_OUTPUT_OUTPUT_PIN

#include "EMWinDXAudioOutputFilter.h"
#include "EMThreadListener.h"

class EMSemaphore;
class EMThread;

#include <list>
#include <streams.h>

class EMMediaDataBuffer;
class EMMediaTimer;
class EMMediaUtility;
class EMMediaFormat;

class EMWinDXAudioOutputOutputPin : public CBaseOutputPin, public EMThreadListener
{
public:
	static EMWinDXAudioOutputOutputPin* Create(EMWinDXAudioOutputFilter* p_opParentFilter);
	virtual ~EMWinDXAudioOutputOutputPin();
	virtual HRESULT GetMediaType(int p_vPosition, CMediaType *pMediaType);
	virtual HRESULT CompleteConnect(IPin *pReceivePin);
	virtual HRESULT BreakConnect(void);
	virtual HRESULT Active(void);
	virtual HRESULT DecideBufferSize(IMemAllocator *pAlloc, ALLOCATOR_PROPERTIES *ppropInputRequest);
	STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void **ppv);
	HRESULT CheckMediaType(const CMediaType *pMediaType);

	void StartProcessing();
	bool StopProcessing();
	bool ProcessBufferE(EMMediaDataBuffer* p_opBuffer);
	void SetName(const char* p_vpName);
	void ThreadRun(EMThread* p_opThread);
	void Flush();
	bool Shutdown();

protected:
	EMWinDXAudioOutputOutputPin(HRESULT *phr, EMWinDXAudioOutputFilter* p_opParentFilter, LPCWSTR pName);
	CMediaType* m_opConnectedMediaType;
	CCritSec m_cSharedState;
	EMWinDXAudioOutputFilter* m_opParentFilter;
	string m_oFilterName;
	EMThread* m_opThread;
	EMSemaphore* m_opCountingSemaphore;
	EMSemaphore* m_opCacheProtectionLock;
	list<IMediaSample*>* m_opCache;
	CRefTime m_vSongTime;
	bool m_fFirstSampleDelivered;
	int64 m_vBuffersAhead;
	bool m_vShouldFlush;
	bool m_vShouldShutdown;
	int64 m_vReferenceTimeValue;

	HANDLE m_uCountingSemaphore;
	HANDLE m_uShutdownSemaphore;

	bool m_vOutputGotInitBuffer;
	EMMediaTimer* m_opTimer;
	EMMediaUtility* m_opUtility;
	EMMediaFormat* m_opSystemAudioFormat;
};

#endif

#endif