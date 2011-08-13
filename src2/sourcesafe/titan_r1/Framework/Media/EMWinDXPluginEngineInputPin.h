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
class EMWinDXPluginEngineFilter;

class __declspec(dllexport) EMWinDXPluginEngineInputPin : public CRenderedInputPin
{
public:
	static EMWinDXPluginEngineInputPin* Create(EMWinDXPluginEngineFilter* p_opParentFilter);
	virtual ~EMWinDXPluginEngineInputPin();
	
	virtual HRESULT CheckMediaType(const CMediaType *pMediaType);
	HRESULT GetMediaType(int p_vPosition, CMediaType *p_opMediaTypeObject);
	STDMETHODIMP Receive(IMediaSample *pSample);
	virtual HRESULT Run(REFERENCE_TIME tStart);
	HRESULT Active(void);
	HRESULT Inactive(void);

	bool SetTargetBuffer(EMMediaDataBuffer* p_opBuffer);
	bool AssertBufferProcessed();

	void SetName(string p_oName);

//	virtual bool ReportFormat(CMediaType* p_opFormat) = 0;
//	virtual bool OnRun(uint64 p_vStartingSongTime) = 0;
//	virtual bool OnEndOfStream() = 0;
//	virtual bool BufferReceived(IMediaSample* p_opMediaSample) = 0;

protected:
	EMWinDXPluginEngineInputPin(HRESULT *phr, EMWinDXPluginEngineFilter* p_opParentFilter, LPCWSTR pName);

	EMWinDXPluginEngineFilter* m_opFilter;
	CMediaType* m_opConnectedMediaType;
	HACMSTREAM m_hPCMToMSADPCMConversionStream;
	CCritSec m_oLock;
	CRefTime m_vSongTime;
	bool m_fFirstSampleDelivered;
	uint64 m_vMediaTimeStart;

	EMMediaDataBuffer* m_opStoredBuffer;
	bool m_vBufferWaiting;
	string m_oFilterName;
	//bool m_fFirstSampleDelivered;

	uint32 m_vNumberOfInChannels;
};

#endif

#endif