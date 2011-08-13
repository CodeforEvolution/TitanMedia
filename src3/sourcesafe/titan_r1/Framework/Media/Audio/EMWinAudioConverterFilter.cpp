#include "EMWinAudioConverterFilter.h"

#include "EMWinAudioConverterExport.h"

#include "EMGlobals.h"

// {5C347FC8-79A6-47c5-A10B-158B9C4D87D8}
static const GUID CLSID_EMWinAudioConverterFilter = { 0x5c347fc8, 0x79a6, 0x47c5, { 0xa1, 0xb, 0x15, 0x8b, 0x9c, 0x4d, 0x87, 0xd8 }};
// {7E3AC55A-8AAA-428d-A410-74332C94EFFD}
static const GUID IID_IEMWinAudioConverterInterface = { 0x7e3ac55a, 0x8aaa, 0x428d, { 0xa4, 0x10, 0x74, 0x33, 0x2c, 0x94, 0xef, 0xfd } };
// {1EA34E21-AA60-4615-B1A2-628CD97B6A33}
static const GUID IID_IEMWinDXCallbackInterface = { 0x1ea34e21, 0xaa60, 0x4615, { 0xb1, 0xa2, 0x62, 0x8c, 0xd9, 0x7b, 0x6a, 0x33 } };

static const GUID CLSID_DUMMY = {0x00000001, 0x0000, 0x0010, {0x80, 0x00,0x00, 0xAA, 0x00, 0x38, 0x9B ,0x71}};

CUnknown* WINAPI EMWinAudioConverterFilter::CreateInstance(LPUNKNOWN lpunk, HRESULT *phr)
{
	CUnknown *punk = EM_new EMWinAudioConverterFilter(NAME(""), lpunk, CLSID_EMWinAudioConverterFilter);
	if(punk == NULL) 
	{
		*phr = E_OUTOFMEMORY;
	}
    return punk;
}

EMWinAudioConverterFilter::EMWinAudioConverterFilter(TCHAR *pName, LPUNKNOWN lpunk, CLSID clsid) 
	:	CTransformFilter(pName, lpunk, clsid),
	m_vTargetSampleRate(44100),
	m_vTargetBitDepth(16),
	m_vTargetIsMono(false),
	m_opCallback(NULL),
	m_vTime(0),
	m_vBytesPerSamples(4)
{
}

EMWinAudioConverterFilter::~EMWinAudioConverterFilter()
{
}

HRESULT EMWinAudioConverterFilter::NonDelegatingQueryInterface(REFIID riid, void **ppv)
{
	if(riid == IID_IEMWinAudioConverterInterface)
	{
		*ppv = (EMWinAudioConverterInterface*) this;
		AddRef();
		return S_OK;
	}
	else if(riid == IID_IEMWinDXCallbackInterface)
	{
		*ppv = (EMWinDXCallbackInterface*) this;
		AddRef();
		return S_OK;
	}
	else 
		return CBaseFilter::NonDelegatingQueryInterface(riid, ppv);
}

HRESULT EMWinAudioConverterFilter::CheckInputType(const CMediaType* p_opMedia)
{
	

	AM_MEDIA_TYPE* opMedia = (AM_MEDIA_TYPE*)(p_opMedia);

	if(opMedia -> majortype != MEDIATYPE_Audio)
		return VFW_E_TYPE_NOT_ACCEPTED;

	if(opMedia -> subtype != CLSID_DUMMY)
		return VFW_E_TYPE_NOT_ACCEPTED;

	if(opMedia -> formattype != FORMAT_WaveFormatEx)
		return VFW_E_TYPE_NOT_ACCEPTED;

	WAVEFORMATEX* opWave = reinterpret_cast<WAVEFORMATEX*>(opMedia -> pbFormat);

	if(opWave -> wFormatTag != WAVE_FORMAT_PCM)
		return VFW_E_TYPE_NOT_ACCEPTED;

	return S_OK;
}


HRESULT EMWinAudioConverterFilter::CheckTransform(const CMediaType* p_opMediaIn, const CMediaType* p_opMediaOut)
{
	AM_MEDIA_TYPE* opMediaIn = (AM_MEDIA_TYPE*)(p_opMediaIn);
	AM_MEDIA_TYPE* opMediaOut = (AM_MEDIA_TYPE*)(p_opMediaOut);


	//MessageBox(NULL, "EMWinAudioConverterFilter::CheckTransform", "", MB_OK);
	if(CheckInputType(p_opMediaIn) != S_OK)
		return VFW_E_TYPE_NOT_ACCEPTED;

	if(CheckInputType(p_opMediaOut) != S_OK)
		return VFW_E_TYPE_NOT_ACCEPTED;


	WAVEFORMATEX* opWave = reinterpret_cast<WAVEFORMATEX*>(opMediaOut -> pbFormat);

	if(opWave != NULL)
	{
		if(opWave -> wBitsPerSample != m_vTargetBitDepth)
			return VFW_E_TYPE_NOT_ACCEPTED;

		if(opWave -> nSamplesPerSec != m_vTargetSampleRate)
			return VFW_E_TYPE_NOT_ACCEPTED;

		if((opWave -> nChannels != 2 && !m_vTargetIsMono) ||
			(opWave -> nChannels != 1 && m_vTargetIsMono))
			return VFW_E_TYPE_NOT_ACCEPTED;
	}
	else
		return VFW_E_TYPE_NOT_ACCEPTED;

	//MessageBox(NULL, "EMWinAudioConverterFilter::CheckTransform - end", "", MB_OK);
	
	return S_OK;
}


HRESULT EMWinAudioConverterFilter::DecideBufferSize(IMemAllocator* p_opIAlloc, ALLOCATOR_PROPERTIES *p_opPropInputRequest)
{
	//MessageBox(NULL, "EMWinAudioConverterFilter::DecideBufferSize - ", "", MB_OK);

	p_opPropInputRequest -> cBuffers = 2;
	p_opPropInputRequest -> cbBuffer = m_vTargetSampleRate*m_vTargetBitDepth / 8;

	if(!m_vTargetIsMono)
		p_opPropInputRequest -> cbBuffer *= 2;

    ALLOCATOR_PROPERTIES Actual;
    HRESULT hr = p_opIAlloc -> SetProperties(p_opPropInputRequest, &Actual);
    if(FAILED(hr)) 
	{
		//MessageBox(NULL, "EMWinAudioConverterFilter::DecideBufferSize - Error", "", MB_OK);
        return hr;
	}

	if(Actual.cbBuffer < p_opPropInputRequest -> cbBuffer) 
	{
		//MessageBox(NULL, "EMWinAudioConverterFilter::DecideBufferSize - Fail", "", MB_OK);
        return E_FAIL;
	}

	//MessageBox(NULL, "EMWinAudioConverterFilter::DecideBufferSize - Done", "", MB_OK);

	return NOERROR;
}


HRESULT EMWinAudioConverterFilter::GetMediaType(int p_vPosition, CMediaType *p_opMediaTypeObject)
{
	if(p_vPosition == 0)
	{
		//First, allocate a new WAVEFORMATEX structure and get a pointer to it...
		WAVEFORMATEX* sWaveFormatStructure = reinterpret_cast<WAVEFORMATEX*>(p_opMediaTypeObject -> AllocFormatBuffer(sizeof(WAVEFORMATEX)));
		//Then fill it out with the PCM format information
		sWaveFormatStructure -> wFormatTag = WAVE_FORMAT_PCM;
		sWaveFormatStructure -> nChannels = m_vTargetIsMono ? 1 : 2;
		sWaveFormatStructure -> nSamplesPerSec = (uint32) m_vTargetSampleRate;
		sWaveFormatStructure -> wBitsPerSample = m_vTargetBitDepth;        
		sWaveFormatStructure -> nBlockAlign = static_cast<WORD>((sWaveFormatStructure -> wBitsPerSample * sWaveFormatStructure -> nChannels) / 8);
		sWaveFormatStructure -> nAvgBytesPerSec = sWaveFormatStructure -> nBlockAlign * sWaveFormatStructure -> nSamplesPerSec;
		sWaveFormatStructure -> cbSize = 0;

		//And then fill out the CMediaType object with information based on that WAVEFORMATEX structure
		if(FAILED(CreateAudioMediaType(sWaveFormatStructure, p_opMediaTypeObject, FALSE)))
		{
			return E_UNEXPECTED;
		}
	}
 	else
	{
		return VFW_S_NO_MORE_ITEMS;
	}

	return S_OK;
}

STDMETHODIMP EMWinAudioConverterFilter::SetSampleParams(int p_vSampleRate, int p_vBitDepth, bool p_vIsMono)
{
	
	CBasePin* opInput = GetPin(1);

	if(opInput -> IsConnected())
	{
		return VFW_E_ALREADY_CONNECTED;
	}

	m_vTargetSampleRate = p_vSampleRate;
	m_vTargetBitDepth = p_vBitDepth;
	m_vTargetIsMono = p_vIsMono;

	m_vBytesPerSamples = p_vBitDepth * (p_vIsMono ? 1 : 2) / 8;

	return S_OK;
}

STDMETHODIMP EMWinAudioConverterFilter::SetCallback(EMCallback* p_opCallback)
{
	if(m_State == State_Stopped )
	{
		m_opCallback = p_opCallback;
		return S_OK;
	}

	return VFW_E_NOT_STOPPED;
}



int *EMWinAudioConverterFilter::CreateAddBuffer(int p_vSrc, int p_vDst) 
{
	int tmp = 0;

	int *pBuff = new int[p_vDst];
	int *pPtr = pBuff;

	float k = (float)p_vSrc / (float)p_vDst;

	for (int i = 0; i < p_vDst; ++i, ++pPtr) 
	{
		*pPtr = (int)(k*(i+1) - (int)(k*i));
		tmp += *pPtr;
	}

	return pBuff;
}


TSample EMWinAudioConverterFilter::GetNextSample(unsigned char** p_opSrc, bool p_vIs8Bits, bool p_vIsMono)
{
	TSample oSample;
	unsigned char* opSrc = *p_opSrc;
	
	if(p_vIsMono && p_vIs8Bits)
	{
		oSample.vLeft = *opSrc - 128;
		
		oSample.vLeft *= 256;

		oSample.vRight = oSample.vLeft;
		opSrc ++;
		oSample.vSrcCnt = 1;
	}
	else if(p_vIsMono && !p_vIs8Bits)
	{
		oSample.vLeft = *reinterpret_cast<int16*>(opSrc);
		oSample.vRight = oSample.vLeft;
		opSrc += 2;
		oSample.vSrcCnt = 2;
	}
	else if(!p_vIsMono && p_vIs8Bits)
	{
		oSample.vLeft = *opSrc - 128;
		oSample.vLeft *= 256;

		opSrc ++;
		oSample.vRight = *opSrc - 128;
		oSample.vRight *= 256;

		opSrc ++;
		oSample.vSrcCnt = 2;
	}
	else
	{
		oSample.vLeft = *reinterpret_cast<int16*>(opSrc);
		opSrc += 2;
		oSample.vRight = *reinterpret_cast<int16*>(opSrc);
		opSrc += 2;
		oSample.vSrcCnt = 4;
	}

	*p_opSrc = opSrc;

	return oSample;
}


int EMWinAudioConverterFilter::PutNextSample(unsigned char** p_opDst, TSample &p_oSample, int p_vCounter)
{
	int vCnt = 0;
	int vAddPntr;

	unsigned char* opRef = *p_opDst;
	uint16* opDst = (reinterpret_cast<uint16*>(opRef));
	
	if(m_vTargetIsMono && (m_vTargetBitDepth == 8))
	{
		p_oSample.vLeft /= 2;
//		p_oSample.vRight /= 2;
		
//		p_oSample.vLeft += p_oSample.vRight;
		p_oSample.vLeft /= 128;
		p_oSample.vLeft += 128;

		vAddPntr = 1;
	}
	else if(m_vTargetIsMono && (m_vTargetBitDepth == 16))
	{
//		p_oSample.vLeft /= 2;
//		p_oSample.vRight /= 2;
		
//		p_oSample.vLeft += p_oSample.vRight;

		vAddPntr = 2;
	}
	else if(!m_vTargetIsMono && (m_vTargetBitDepth == 8))
	{
		p_oSample.vLeft /= 256;
		p_oSample.vLeft += 128;

		p_oSample.vRight /= 256;
		p_oSample.vRight += 128;

		vAddPntr = 1;
	}
	else
	{
		vAddPntr = 2;
	}

	for(int i = 0; i < m_opSteps[p_vCounter]; ++i)
	{
		opDst = (reinterpret_cast<uint16*>(opRef));
		*opDst = p_oSample.vLeft;

		opRef += vAddPntr;
		vCnt += vAddPntr;

		if(!m_vTargetIsMono)
		{
			opDst = (reinterpret_cast<uint16*>(opRef));
			*opDst = p_oSample.vRight;

			opRef += vAddPntr;
			vCnt += vAddPntr;		
		}
	}

	*p_opDst = opRef;

	return vCnt;
}




HRESULT EMWinAudioConverterFilter::Transform(IMediaSample* p_opIn, IMediaSample* p_opOut)
{
	AM_MEDIA_TYPE oMedia;
	WAVEFORMATEX* opWave;

	bool vIsMono;
	bool vIs8bits;

	uint64 vSourceSize;
	uint64 vDestinationSize;
	uint64 srcCnt = 0;
	uint64 dstCnt = 0;
	uint64 vCounter;
	int64 vStartTime = m_vTime;

	unsigned char* pSrc;
	unsigned char* pDst;


	CBasePin* opInput = GetPin(0);

	opInput -> ConnectionMediaType(&oMedia);

	opWave = reinterpret_cast<WAVEFORMATEX*>(oMedia.pbFormat);

	vIsMono = (opWave -> nChannels == 1);
	vIs8bits = (opWave -> wBitsPerSample == 8);


	vSourceSize = p_opIn -> GetActualDataLength();
	vDestinationSize = p_opOut -> GetSize();

	p_opIn -> GetPointer((unsigned char**)&pSrc);
	p_opOut -> GetPointer((unsigned char**)&pDst);

	float vDivisability = static_cast<float>(m_vTargetSampleRate) / static_cast<float>(opWave ->nSamplesPerSec);

	int64 vNumSourceFrames = (8 * opWave -> nChannels * vSourceSize) / opWave -> wBitsPerSample;
	int64 vNumTargetFrames = static_cast<int64>(static_cast<float>(vNumSourceFrames * vDivisability));

	m_opSteps = CreateAddBuffer(vNumTargetFrames, vNumSourceFrames);
	
	vCounter = 0;
	while(dstCnt < vDestinationSize && srcCnt < vSourceSize)
	{
		TSample oSample = GetNextSample(&pSrc, vIs8bits, vIsMono);
//		pSrc += oSample.vSrcCnt;
		srcCnt += oSample.vSrcCnt;

		int vInc = PutNextSample(&pDst, oSample, vCounter); 
		dstCnt += vInc;
//		pDst += vInc;
		
		++ vCounter;
	}

	delete [] m_opSteps;

	p_opOut -> SetActualDataLength(dstCnt);

	m_vTime += dstCnt / m_vBytesPerSamples;

	p_opOut -> SetMediaTime(&vStartTime, &m_vTime);

	if(m_opCallback != NULL)
	{
		m_opCallback -> Callback(vStartTime, m_vTime);
	}
	
	return S_OK;
}



STDAPI DllRegisterServer()
{
	return AMovieDllRegisterServer2(TRUE);
}

STDAPI DllUnregisterServer()
{
	return AMovieDllRegisterServer2(FALSE);
}

CFactoryTemplate g_Templates[] = 
{
	{L"", &CLSID_EMWinAudioConverterFilter, EMWinAudioConverterFilter::CreateInstance, NULL, &sFilterInformation}
};

int g_cTemplates = 1;

