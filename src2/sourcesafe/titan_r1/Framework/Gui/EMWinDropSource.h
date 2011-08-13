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

#ifndef __EM_WIN_DROP_SOURCE
#define __EM_WIN_DROP_SOURCE

#include <shlobj.h>

class FAR EMWinDropSource : public IDropSource
{
public:
	EMWinDropSource();
	~EMWinDropSource();

	STDMETHOD_(ULONG, AddRef)(void);
    STDMETHOD(GiveFeedback)(DWORD p_vEffect);
	STDMETHOD(QueryContinueDrag)(BOOL p_vEscapePressed, DWORD p_vKeyState);
	STDMETHOD(QueryInterface)(REFIID p_vReferensInterfaceID, void FAR* FAR* p_oppObject);
    STDMETHOD_(ULONG, Release)(void);

private:
	ULONG m_vRefs;
	long m_vOleInitializeResult;
};

#endif

#endif