#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#ifndef __EM_WIN_MEDIA_INPUT_FILTER
#define __EM_WIN_MEDIA_INPUT_FILTER

#include <list>
#include <streams.h>
#include <mmreg.h>
#include <msacm.h>
#include <dynsrc.h>

#include "EMDirectXFilter.h"
#include "EMWinMediaInputFilterInterface.h"

class EMWinMediaInputPin;
class EMWaveFileWriter;
class EMMediaSignalMeter;
class EMMediaDataBuffer;

DEFINE_GUID(CLSID_EMWinMediaInputFilter, 0x952c35ab, 0x4dcb, 0x11d5, 0xbc, 0x74, 0x0, 0x2, 0xb3, 0x17, 0xf3, 0xf2);
DEFINE_GUID(IID_IEMWinMediaInputFilter, 0x952c35ac, 0x4dcb, 0x11d5, 0xbc, 0x74, 0x0, 0x2, 0xb3, 0x17, 0xf3, 0xf2);

class EMWinMediaInputFilter : public EMWinMediaInputFilterInterface, public EMDirectXFilter
{
public:
	static CUnknown * WINAPI CreateInstance(LPUNKNOWN pUnk, HRESULT *phr);
	~EMWinMediaInputFilter();
	STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void **ppv);
    CBasePin * GetPin(int n);
    int GetPinCount();
    STDMETHODIMP Run(REFERENCE_TIME tStart);
    STDMETHODIMP Pause();
    STDMETHODIMP Stop();
	
	STDMETHODIMP SetTargetFile(string p_oFileName);
	STDMETHODIMP CloseTargetFile();


	STDMETHODIMP StartOnTime(int64 p_vStartTime);
	STDMETHODIMP Suspend(bool p_vSuspended);
	STDMETHODIMP SetSignalMeter(EMMediaSignalMeter* p_opMeter);

	bool BufferReceived(IMediaSample* p_opMediaSample);

	DECLARE_IUNKNOWN

protected:
    EMWinMediaInputFilter(LPUNKNOWN pUnk, HRESULT *phr);
private:
	CCritSec m_oLock;
	EMWinMediaInputPin* m_opPin;
	EMWaveFileWriter* m_opWaveFile;
	EMMediaSignalMeter* m_opMeter;
	bool m_vIsSuspended;
	EMMediaDataBuffer* m_opBuffer;
	int64 m_vStartTime;
};

#endif

#endif