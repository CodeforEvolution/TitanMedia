#ifndef __EM_WIN_DX_AUDIO_OUTPUT_FILTER
#define __EM_WIN_DX_AUDIO_OUTPUT_FILTER

#include "EMGlobals.h"

#include "EMWinDXAudioOutputInterface.h"
//#include "EMMediaEngineUIDs.h"

#include <streams.h>

class EMWinDXAudioOutputOutputPin;
class EMMediaDataBuffer;

DEFINE_GUID(CLSID_EMWinDXAudioOutputFilter, 0x53f8f7a1, 0x6b3d, 0x11d5, 0xbc, 0x75, 0x0, 0x2, 0xb3, 0x17, 0xf3, 0xf2);
DEFINE_GUID(IID_IEMWinDXAudioOutputInterface,  0x53f8f7a2, 0x6b3d, 0x11d5, 0xbc, 0x75, 0x0, 0x2, 0xb3, 0x17, 0xf3, 0xf2);

class EMWinDXAudioOutputFilter : public CBaseFilter, public CCritSec, public EMWinDXAudioOutputInterface
{
public:
	static CUnknown* WINAPI CreateInstance(LPUNKNOWN lpunk, HRESULT *phr);
	virtual ~EMWinDXAudioOutputFilter();

	DECLARE_IUNKNOWN

    int GetPinCount();
    CBasePin *GetPin(int n);
    STDMETHODIMP JoinFilterGraph(IFilterGraph* pGraph, LPCWSTR pName);
    STDMETHODIMP Stop();
    STDMETHODIMP Pause();
	STDMETHODIMP Run(REFERENCE_TIME tStart);

	STDMETHODIMP ProcessBuffer(EMMediaDataBuffer* p_opBuffer);
	STDMETHODIMP ProjectStarted(int64 p_vSystemTimeWhenStarted);
	STDMETHODIMP ProjectStopped();
	STDMETHODIMP Flush();

	CCritSec* GetStateLock() { return &m_cStateLock; };

	STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void **ppv);
	STDMETHODIMP SetName(const char* p_vpName);
	string GetName() const;

	STDMETHODIMP Shutdown();
	STDMETHODIMP SetRefClock(IReferenceClock* p_opClock);
	IReferenceClock* GetRefClock() const;

	STDMETHODIMP RegisterAsClockMaster(int32 p_vRealtimeOutputDescriptorID);
	STDMETHODIMP UnregisterAsClockMaster();

	bool IsClockMaster() const { return m_vIsClockMaster; };
	int32 GetClockMasterID() const { return m_vClockMasterID; };
	REFERENCE_TIME m_vStartingTime;

private:
	EMWinDXAudioOutputFilter(TCHAR *pName, LPUNKNOWN lpunk, CLSID clsid, HRESULT *phr);
	CCritSec m_cStateLock;	
	EMWinDXAudioOutputOutputPin* m_opOutgoingAudioStream;
	CAMEvent m_evFilterStoppingEvent;
	string m_oName;
	IReferenceClock* m_opRefClock;
	bool m_vIsClockMaster;
	int32 m_vClockMasterID;
};

#endif