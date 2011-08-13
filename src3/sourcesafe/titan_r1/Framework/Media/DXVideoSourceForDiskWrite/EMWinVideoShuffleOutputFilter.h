#ifndef __EM_WIN_VIDEO_SHUFFLE_OUTPUT_FILTER
#define __EM_WIN_VIDEO_SHUFFLE_OUTPUT_FILTER

#include "EMGlobals.h"

#include "EMWinVideoShuffleOutputInterface.h"
#include "EMMediaFormat.h"

#include <streams.h>

class EMWinVideoShuffleOutputPin;
class EMMediaDataBuffer;

DEFINE_GUID(CLSID_EMWinVideoShuffleOutputFilter, 0x3fc76daf, 0x80e8, 0x11d5, 0x82, 0x59, 0x0, 0x2, 0xb3, 0x18, 0x0, 0x46);
DEFINE_GUID(IID_IEMWinVideoShuffleOutputInterface, 0x3fc76dae, 0x80e8, 0x11d5, 0x82, 0x59, 0x0, 0x2, 0xb3, 0x18, 0x0, 0x46);

class EMWinVideoShuffleOutputFilter : public CBaseFilter, public EMWinVideoShuffleOutputInterface
{
public:
	static CUnknown* WINAPI CreateInstance(LPUNKNOWN lpunk, HRESULT *phr);
	virtual ~EMWinVideoShuffleOutputFilter();

	DECLARE_IUNKNOWN

    int GetPinCount();
    CBasePin *GetPin(int n);
    STDMETHODIMP JoinFilterGraph(IFilterGraph* pGraph, LPCWSTR pName);
    STDMETHODIMP Stop();
    STDMETHODIMP Pause();

	STDMETHODIMP ProcessBuffer(EMMediaDataBuffer* p_opBuffer);
	STDMETHODIMP Flush();

	CCritSec* GetStateLock() { return &m_cStateLock; };

	STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void **ppv);
	STDMETHODIMP SetName(string p_oName);
	string GetName() const;

//	STDMETHODIMP Shutdown();
	STDMETHODIMP SetTimeStamp(REFERENCE_TIME p_vRefTime);
	STDMETHODIMP SetShuffleFormat(EMMediaFormat* p_opMediaFormat);

	STDMETHODIMP StartThread();
	STDMETHODIMP EndThread();

	STDMETHODIMP RegisterAsClockMaster(int32 p_vRealtimeOutputDescriptorID);
	STDMETHODIMP UnregisterAsClockMaster();

private:
	EMWinVideoShuffleOutputFilter(TCHAR *pName, LPUNKNOWN lpunk, CLSID clsid, HRESULT *phr);
	CCritSec m_cStateLock;	
	EMWinVideoShuffleOutputPin* m_opOutgoingVideoStream;

	CAMEvent m_evFilterStoppingEvent;
	string m_oName;
};

#endif