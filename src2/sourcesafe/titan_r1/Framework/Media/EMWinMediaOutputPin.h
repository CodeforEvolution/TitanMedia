#include "EMGlobals.h"
#include "EMMediaGlobals.h"
#ifdef PLATFORM_WINDOWS

#ifndef __EM_WIN_MEDIA_OUTPUT_PIN
#define __EM_WIN_MEDIA_OUTPUT_PIN

#include <streams.h>
#include <windows.h>
#include <process.h>
#include <mmreg.h>
#include <msacm.h>
#include <dynsrc.h>

class EMMediaDataBuffer;
class EMWinMediaOutputFilter;
struct IMediaSample;

class EMWinMediaOutputPin : public CDynamicSourceStream
{
public:
	static EMWinMediaOutputPin* Create(EMMediaType p_eType, CDynamicSource* p_opParentFilter);
	virtual ~EMWinMediaOutputPin();
	
	virtual HRESULT GetMediaType(CMediaType *pMediaType);
	virtual HRESULT CompleteConnect(IPin *pReceivePin);
	virtual HRESULT BreakConnect(void);
	virtual HRESULT Active(void);
	virtual HRESULT Inactive(void);
	virtual HRESULT DecideBufferSize(IMemAllocator *pAlloc, ALLOCATOR_PROPERTIES *ppropInputRequest);
	STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void **ppv);

	virtual HRESULT FillBuffer(IMediaSample** pSamp);

	IMediaSample* ConvertFromBufferToSample(EMMediaDataBuffer* p_opFromEMMediaDataBuffer);
	IMediaSample* RequestBuffer();
	virtual EMMediaType ReportFormatType() = 0;
	virtual bool ReportFormat(CMediaType* p_opFormat) = 0;
	virtual uint32 ReportNumBuffers() = 0;
	virtual uint32 ReportBufferSize() = 0;
	virtual bool OnActive() = 0;
	virtual bool OnDisconnect() = 0;

	
	virtual void RegisterAsClockMaster(int32 p_vRealtimeOutputDescriptorID);
	virtual void UnregisterAsClockMaster();

protected:
	HANDLE	m_oTimerEvent;
	IReferenceClock *m_opClock;

	REFERENCE_TIME m_vStartTime;
	REFERENCE_TIME m_vSampleTime;
	REFERENCE_TIME m_vSongOffset;
	EMWinMediaOutputPin(HRESULT *phr, CDynamicSource* p_opParentFilter, LPCWSTR pName);

	EMWinMediaOutputFilter* m_opFilter;
	CMediaType* m_opConnectedMediaType;
	HACMSTREAM m_hPCMToMSADPCMConversionStream;
	CCritSec    m_cSharedState;
	CRefTime m_vSongTime;
	bool m_fFirstSampleDelivered;
	uint64 m_vMediaTimeStart;
	//bool m_fFirstSampleDelivered;
	EMMediaType* m_eType;
	bool m_vTimeToShutdown;
	bool m_vDiscont;

	bool m_vIsClockMaster;
	int32 m_vMasterID;
};

#endif

#endif