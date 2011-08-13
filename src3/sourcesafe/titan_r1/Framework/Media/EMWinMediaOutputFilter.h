#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#ifndef __EM_WIN_MEDIA_OUTPUT_FILTER
#define __EM_WIN_MEDIA_OUTPUT_FILTER

#include "EMWinMediaOutputFilterInterface.h"
#include "EMDirectXFilter.h"
#include "EMMediaGlobals.h"
#include "EMMediaFormat.h"

#include <list>
#include <streams.h>
#include <mmreg.h>
#include <msacm.h>
#include <dynsrc.h>

class EMMediaDataBuffer;
class CDynamicOutputPin;
class EMSemaphore;
class EMMediaTimer;
class EMSettingsRepository;

DEFINE_GUID(CLSID_EMWinMediaOutputFilter, 0x952c35a5, 0x4dcb, 0x11d5, 0xbc, 0x74, 0x0, 0x2, 0xb3, 0x17, 0xf3, 0xf2);
DEFINE_GUID(CLSID_EMWinMediaVideoOutputFilter, 0xbedfce61, 0x535d, 0x11d5, 0x82, 0x59, 0x0, 0x2, 0xb3, 0x18, 0x0, 0x46);
DEFINE_GUID(IID_IEMWinMediaOutputFilter, 0x952c35a8, 0x4dcb, 0x11d5, 0xbc, 0x74, 0x0, 0x2, 0xb3, 0x17, 0xf3, 0xf2);
DEFINE_GUID(IID_IEMWinMediaVideoOutputFilter, 0xbedfce63, 0x535d, 0x11d5, 0x82, 0x59, 0x0, 0x2, 0xb3, 0x18, 0x0, 0x46);

class EMWinMediaOutputFilter : public EMWinMediaOutputFilterInterface, public CDynamicSource
{
public:
	static CUnknown* WINAPI CreateInstance(LPUNKNOWN lpunk, HRESULT *phr);
	virtual ~EMWinMediaOutputFilter();

//	LPAMOVIESETUP_FILTER GetSetupData();
//	int GetPinCount(void);
//	CBasePin* GetPin(int);
//	STDMETHODIMP JoinFilterGraph(IFilterGraph *pGraph, LPCWSTR pName);
	STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void **ppv);

	STDMETHODIMP Stop(void);
//  STDMETHODIMP Pause(void);

	STDMETHODIMP AddBufferToCache(EMMediaDataBuffer* p_opBuffer);
	STDMETHODIMP GetBufferFromCache(IMediaSample** p_opIMediaSample);
	virtual AM_MEDIA_TYPE *GetMediaFormatInternal(CMediaType *p_opFormat);
	STDMETHODIMP InitializeFilter(EMMediaTimer* p_opTimer, EMSettingsRepository* p_opSettingsRepository);
	STDMETHODIMP FlushCache();
	STDMETHODIMP Shutdown();

	STDMETHODIMP RegisterAsClockMaster(int32 p_vRealtimeOutputDescriptorID);
	STDMETHODIMP UnregisterAsClockMaster();

	EMMediaTimer* GetTimer();
	EMSettingsRepository* GetSettingsRepository();
	DECLARE_IUNKNOWN;

	IReferenceClock *GetClock();
	IMediaEvent *GetEventSource();
	int GetNumBuffers();

//	CCritSec* GetLock() { return &m_cStateLock; };
//	virtual AM_MEDIA_TYPE *GetMediaFormatInternal(CMediaType *p_opFormat);
	EMMediaFormat* m_opFormat;	
protected:

private:
	EMWinMediaOutputFilter(LPUNKNOWN lpunk, HRESULT *phr);
//	CCritSec m_cStateLock;

//	CDynamicOutputPin* m_opOutputPin;
//	CAMEvent m_evFilterStoppingEvent;
	bool FormatHasChanged(EMMediaFormat *p_opFormat);
	void SaveFormat(EMMediaFormat *p_opFormat);

	list<IMediaSample*> m_oCache;
	EMSemaphore* m_opSemaphoreOne;
	EMSemaphore* m_opSemaphoreTwo;
	EMSemaphore* m_opProtection;
	EMMediaTimer* m_opTimer;
	EMSettingsRepository* m_opSettingsRepository;
	bool m_vRecentlyFlushed;
	EMMediaType m_eMediaType;
	bool m_vTimeToShutDown;
	bool m_vIsRunning;
	bool m_vIsSilenceBuffer;
	VIDEOINFOHEADER* m_sVideoFormatStructure;
	int32 m_vBelongedToUniqueID;
	//HANDLE m_vCacheCounter;
};

#endif

#endif