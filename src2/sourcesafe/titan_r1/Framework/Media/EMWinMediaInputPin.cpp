#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#include "EMWinMediaInputPin.h"
#include "EMMediaDataBuffer.h"
//#include "EMWinMediaOutputFilterInterface.h"
#include "EMWinMediaInputFilter.h"
#include "EMMediaTimer.h"
#include "EMMediaUtility.h"
#include "EMSettingsRepository.h"
#include "EMSettingIDs.h"
#include "EMMediaFormat.h"
#include "EMWinAudioInputPin.h"

//#include <uuids.h>
#include <math.h>

int x = 0;

EMWinMediaInputPin* EMWinMediaInputPin::Create(EMMediaType p_eType, EMDirectXFilter* p_opParentFilter, string p_oName)
{
	HRESULT vResult = S_OK;
	if((p_eType & EM_TYPE_ANY_AUDIO) > 0)
		return EM_new EMWinAudioInputPin(&vResult, p_opParentFilter, p_oName);
	else if((p_eType & EM_TYPE_ANY_VIDEO) > 0)
		return NULL;
	else
		return NULL;
}

EMWinMediaInputPin::EMWinMediaInputPin(HRESULT *phr, EMDirectXFilter* p_opParentFilter, LPCWSTR pName, CCritSec* pLock)
	:	CRenderedInputPin(NAME("Debug name"), p_opParentFilter, pLock, phr, pName),
		m_opFilter(p_opParentFilter),
		m_opConnectedMediaType(NULL),
		m_vMediaTimeStart(0)
{
}

EMWinMediaInputPin::~EMWinMediaInputPin()
{
}

HRESULT EMWinMediaInputPin::CheckMediaType(const CMediaType* p_opMediaTypeObject)
{
	return S_OK;
}

STDMETHODIMP EMWinMediaInputPin::Receive(IMediaSample* pSample)
{
	BufferReceived(pSample);
	//pSample -> Release();
	return S_OK;
}

HRESULT EMWinMediaInputPin::Run(REFERENCE_TIME tStart)
{
	HRESULT hr = CRenderedInputPin::Run(tStart);
	if(hr != S_OK)
		return hr;

	if(! OnRun(tStart))
		return E_FAIL;

	return NOERROR;
}

HRESULT EMWinMediaInputPin::Active()
{
	return CRenderedInputPin::Active();
}

HRESULT EMWinMediaInputPin::Inactive()
{
	HRESULT hr = CRenderedInputPin::Inactive();
	if(hr != S_OK)
		return hr;

//	if(! OnEndOfStream())
//		return E_FAIL;

	return NOERROR;
}

#endif