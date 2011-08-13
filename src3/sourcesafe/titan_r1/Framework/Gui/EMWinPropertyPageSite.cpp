#include "EMWinPropertyPageSite.h"

#ifdef PLATFORM_WINDOWS

EMWinPropertyPageSite::EMWinPropertyPageSite()
{
}

EMWinPropertyPageSite::~EMWinPropertyPageSite()
{
}

STDMETHODIMP EMWinPropertyPageSite::OnStatusChange(DWORD p_vFlags)
{
	switch(p_vFlags)
	{
	case PROPPAGESTATUS_DIRTY:  // Enable ApplyButton
		eo << "PROPPAGESTATUS_DIRTY" << ef;
		break;
	case PROPPAGESTATUS_VALIDATE: // Updata changes
		eo << "PROPPAGESTATUS_VALIDATE" << ef;
		break;
	}
	return S_OK;
}

STDMETHODIMP EMWinPropertyPageSite::GetLocaleID(LCID* p_vpLocaleID)
{
	if(p_vpLocaleID == NULL)
		return E_POINTER;
	return S_OK;
}

STDMETHODIMP EMWinPropertyPageSite::GetPageContainer(IUnknown** p_oppUnk)
{
	return E_NOTIMPL;
}

STDMETHODIMP EMWinPropertyPageSite::TranslateAccelerator(LPMSG p_vpMsg)
{
	return S_FALSE;
}

STDMETHODIMP EMWinPropertyPageSite::QueryInterface(REFIID p_vReferensInterfaceID, void FAR* FAR* p_oppObject)
{
	if(p_vReferensInterfaceID == IID_IUnknown || p_vReferensInterfaceID == IID_IPropertyPageSite)
	{
		*p_oppObject = this;
		++m_vRefs;
		return NOERROR;
	}
	*p_oppObject = NULL;

	return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) EMWinPropertyPageSite::AddRef()
{
	return ++m_vRefs;
}
STDMETHODIMP_(ULONG) EMWinPropertyPageSite::Release()
{
	if(--m_vRefs == 0)
	{
		delete this;
		return 0;
	}
	return m_vRefs;
}

#endif