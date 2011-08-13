#ifndef __EM_WIN_AUDIO_CONVERTER_FILTER
#define __EM_WIN_AUDIO_CONVERTER_FILTER

#include "EMGlobals.h"

#include "EMWinAudioConverterInterface.h"
#include "EMWinDXCallbackInterface.h"

#include <streams.h>

DEFINE_GUID(CLSID_EMWinAudioConverterFilter, 0x5c347fc8, 0x79a6, 0x47c5, 0xa1, 0xb, 0x15, 0x8b, 0x9c, 0x4d, 0x87, 0xd8 );
DEFINE_GUID(IID_IEMWinAudioConverterInterface, 0x7e3ac55a, 0x8aaa, 0x428d, 0xa4, 0x10, 0x74, 0x33, 0x2c, 0x94, 0xef, 0xfd);
DEFINE_GUID(CLSID_DUMMY, 0x00000001, 0x0000, 0x0010, 0x80, 0x00,0x00, 0xAA, 0x00, 0x38, 0x9B ,0x71);

const int32 EM_UPSAMPLE   = 1;
const int32 EM_DOWNSAMPLE = 2;

typedef struct _TSample
{
	uint16 vLeft;
	uint16 vRight;
	uint16 vSrcCnt;
} TSample;


class EMWinAudioConverterFilter : public CTransformFilter, public EMWinAudioConverterInterface, public EMWinDXCallbackInterface
{
public:
	static CUnknown* WINAPI CreateInstance(LPUNKNOWN lpunk, HRESULT *phr);
	virtual ~EMWinAudioConverterFilter();

	DECLARE_IUNKNOWN;

	STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void **ppv);

	virtual HRESULT CheckInputType(const CMediaType* p_opMedia);
	virtual HRESULT CheckTransform(const CMediaType* p_opMediaIn, const CMediaType* p_opMediaOut);
	virtual HRESULT DecideBufferSize(IMemAllocator* p_opIAlloc, ALLOCATOR_PROPERTIES *p_opPropInputRequest); 
	virtual HRESULT GetMediaType(int iPosition, CMediaType *pMediaType);
	virtual HRESULT Transform(IMediaSample* p_opIn, IMediaSample* p_opOut);

	virtual STDMETHODIMP SetSampleParams(int p_vSampleRate, int p_vBitDepth, bool p_vIsMono);

	virtual STDMETHODIMP SetCallback(EMCallback* p_opCallback);

	int *CreateAddBuffer(int p_vSrc, int p_vDst);

protected:
	uint32 m_vTargetSampleRate;
	unsigned char m_vTargetBitDepth;
	bool m_vTargetIsMono;

	int m_vBytesPerSamples;

	int* m_opSteps;

	EMCallback* m_opCallback;

	int64 m_vTime;

protected:
	TSample GetNextSample(unsigned char** p_opSrc, bool p_vIs8Bits, bool p_vIsMono);
	int PutNextSample(unsigned char** p_opDst, TSample &p_oSample, int p_vCounter);


private:
	EMWinAudioConverterFilter(TCHAR *pName, LPUNKNOWN lpunk, CLSID clsid);
};

#endif