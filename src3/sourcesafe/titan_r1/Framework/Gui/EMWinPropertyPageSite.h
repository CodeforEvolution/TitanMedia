/*******************************************************
* This code is the copyrighted property of Elegant Media
*-------------------------------------------------------
* Creator: Johan Dunberg
* Component: Framework, GUI
* Portability: native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#ifndef __EM_WIN_PROPERTY_PAGE_SITE
#define __EM_WIN_PROPERTY_PAGE_SITE

#include <ocidl.h>

class FAR EMWinPropertyPageSite : public IPropertyPageSite
{
public:
	EMWinPropertyPageSite();
	~EMWinPropertyPageSite();

	STDMETHOD(OnStatusChange)(DWORD p_vFlags);
	STDMETHOD(GetLocaleID)(LCID* p_vpLocaleID);
	STDMETHOD(GetPageContainer)(IUnknown** p_oppUnk);
	STDMETHOD(TranslateAccelerator)(LPMSG p_vpMsg);

	STDMETHOD(QueryInterface)(REFIID p_vReferensInterfaceID, void FAR* FAR* p_oppObject);
	STDMETHOD_(ULONG, AddRef)(void);
    STDMETHOD_(ULONG, Release)(void);

private:
	ULONG m_vRefs;
};

#endif

#endif