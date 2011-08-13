#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#ifndef __EM_WIN_MEDIA_OUTPUT_PIN
#define __EM_WIN_MEDIA_OUTPUT_PIN

#include <streams.h>
#include <windows.h>
#include <process.h>
#include <mmreg.h>
#include <msacm.h>
#include <dynsrc.h>
#include "EMMediaGlobals.h"
#include "EMDirectXFilter.h"

class EMMediaDataBuffer;
class EMWinMediaInputFilter;
struct IMediaSample;

class __declspec(dllexport) EMWinMediaInputPin : public CRenderedInputPin
{
public:
	static EMWinMediaInputPin* Create(EMMediaType p_eType, EMDirectXFilter* p_opParentFilter, string p_oName = "Untitled pin");
	virtual ~EMWinMediaInputPin();
	
	virtual HRESULT CheckMediaType(const CMediaType *pMediaType);

	STDMETHODIMP Receive(IMediaSample *pSample);
	virtual HRESULT Run(REFERENCE_TIME tStart);
	HRESULT Active(void);
	HRESULT Inactive(void);

	//virtual HRESULT CompleteConnect(IPin *pReceivePin);
	//virtual HRESULT BreakConnect(void);
	//STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void **ppv);

	virtual bool ReportFormat(CMediaType* p_opFormat) = 0;
//	virtual uint32 ReportNumBuffers() = 0;
//	virtual uint32 ReportBufferSize() = 0;
	virtual bool OnRun(uint64 p_vStartingSongTime) = 0;
	virtual bool OnEndOfStream() = 0;
	virtual bool BufferReceived(IMediaSample* p_opMediaSample) = 0;

protected:
	EMWinMediaInputPin(HRESULT *phr, EMDirectXFilter* p_opParentFilter, LPCWSTR pName, CCritSec* pLock);

	EMDirectXFilter* m_opFilter;
	CMediaType* m_opConnectedMediaType;
	HACMSTREAM m_hPCMToMSADPCMConversionStream;
	CCritSec    m_cSharedState;
	CRefTime m_vSongTime;
	bool m_fFirstSampleDelivered;
	uint64 m_vMediaTimeStart;
	//bool m_fFirstSampleDelivered;
};

#endif

#endif