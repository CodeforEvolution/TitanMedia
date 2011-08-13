#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#ifndef __EM_WIN_VIDEO_SHUFFLE_OUTPUT_PIN
#define __EM_WIN_VIDEO_SHUFFLE_OUTPUT_PIN

#include "EMWinVideoShuffleOutputFilter.h"

#include <list>
#include <streams.h>

DEFINE_GUID(CLSID_DUMMY, 0x00000001, 0x0000, 0x0010, 0x80, 0x00,0x00, 0xAA, 0x00, 0x38, 0x9B ,0x71);

class EMMediaDataBuffer;

class EMWinVideoShuffleOutputPin : public CBaseOutputPin
{
public:
	static EMWinVideoShuffleOutputPin* Create(EMWinVideoShuffleOutputFilter* p_opParentFilter);
	virtual ~EMWinVideoShuffleOutputPin();
	virtual HRESULT GetMediaType(int p_vPosition, CMediaType *pMediaType);
	virtual HRESULT CompleteConnect(IPin *pReceivePin);
	virtual HRESULT BreakConnect(void);
	virtual HRESULT Active(void);
	virtual HRESULT DecideBufferSize(IMemAllocator *pAlloc, ALLOCATOR_PROPERTIES *ppropInputRequest);
	virtual STDMETHODIMP Notify(IBaseFilter * pSender, Quality q) {return E_NOTIMPL;}

	STDMETHODIMP StartThread();
	STDMETHODIMP EndThread();

	STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void **ppv);
	HRESULT CheckMediaType(const CMediaType *pMediaType);

//	void StartProcessing();
//	bool StopProcessing();
	bool ProcessBufferE(EMMediaDataBuffer* p_opBuffer);
	void SetName(string p_oName);
//	void ThreadRun(EMThread* p_opThread);
	void Flush();
//	bool Shutdown();

//	bool Lock(uint32 p_vTimeoutMilliSeconds = 0);
//	void Unlock();
	void SetShuffleFormat(EMMediaFormat* p_opMediaFormat);
	void SetTimeStamp(REFERENCE_TIME p_vRefTime);
	HRESULT TimeStampAndDeliverBuffer(IMediaSample* p_opIMediaSampleOut);

	static unsigned __stdcall ThreadProc(void *p_vpThis);

	virtual void RegisterAsClockMaster(int32 p_vRealtimeOutputDescriptorID);
	virtual void UnregisterAsClockMaster();
protected:
	EMWinVideoShuffleOutputPin(HRESULT *phr, EMWinVideoShuffleOutputFilter* p_opParentFilter, LPCWSTR pName);
	CMediaType oMediaType;
	CMediaType* m_opConnectedMediaType;
	CCritSec m_cSharedState;
	EMWinVideoShuffleOutputFilter* m_opParentFilter;
	string m_oFilterName;
	//EMThread* m_opThread;
	//EMSemaphore* m_opCountingSemaphore;
	//EMSemaphore* m_opCacheProtectionLock;
	//list<IMediaSample*>* m_opCache;
	CRefTime m_vSongTime;
	bool m_fFirstSampleDelivered;
	int64 m_vBuffersAhead;
	bool m_vShouldFlush;
	EMMediaFormat m_opCopiedMediaFormat;
	EMMediaFormat* m_opMediaFormat;
	//EMSemaphore* m_opThreadLifetimeSemaphore;
	EMMediaDataBuffer* m_opBuffer;
	bool m_vIsRunning;
	IMediaSample *m_opIMediaSampleOut;
	
	void DeliverCached(IMediaSample *p_opIMediaSampleOut);

	HANDLE m_hFullCache;
	HANDLE m_hEmptyCache;
	HANDLE m_hThreadQuit;

	HANDLE m_hThread;
	unsigned int  m_vThreadID;
	unsigned int ShuffleData();

	bool m_vIsClockMaster;
	int32 m_vMasterID;
};

#endif

#endif