#ifndef __EM_DIRECTX_FILTER
#define __EM_DIRECTX_FILTER

#include <streams.h>

struct IMediaSample;

class EMDirectXFilter : public CBaseFilter
{
public:
	virtual ~EMDirectXFilter() {};
	virtual bool BufferReceived(IMediaSample* p_opMediaSample) = 0;
protected:
	CCritSec m_oLock;
	EMDirectXFilter(const TCHAR* p_vpName, LPUNKNOWN p_upUnknown, REFCLSID p_vCLSID) : CBaseFilter(p_vpName, p_upUnknown, &m_oLock, p_vCLSID) {};
private:
};

#endif