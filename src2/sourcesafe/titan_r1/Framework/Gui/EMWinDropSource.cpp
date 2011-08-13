#include "EMWinDropSource.h"

#ifdef PLATFORM_WINDOWS

EMWinDropSource::EMWinDropSource() :
m_vRefs(1)
{
	m_vOleInitializeResult = OleInitialize(NULL);
	eo << "Drop Source OleInitialize Result:" << ef;
	switch(m_vOleInitializeResult)
	{
	case S_OK:
		eo << "    S_OK" << ef;
		break;
	case S_FALSE:
		eo << "    S_FALSE" << ef;
		break;
	case OLE_E_WRONGCOMPOBJ:
		eo << "    OLE_E_WRONGCOMPOBJ" << ef;
		break;
	case RPC_E_CHANGED_MODE:
		eo << "    RPC_E_CHANGED_MODE" << ef;
		break;
	case E_INVALIDARG:
		eo << "    E_INVALIDARG" << ef;
		break;
	case E_OUTOFMEMORY:
		eo << "    E_OUTOFMEMORY" << ef;
		break;
	case E_UNEXPECTED:
		eo << "    E_UNEXPECTED" << ef;
		break;
	default:
		eo << "    UnKnown result" << ef;
	}
}

EMWinDropSource::~EMWinDropSource()
{
	if(m_vOleInitializeResult == S_OK ||
	   m_vOleInitializeResult == S_FALSE ||
	   (m_vOleInitializeResult != OLE_E_WRONGCOMPOBJ &&
	   m_vOleInitializeResult != E_INVALIDARG &&
	   m_vOleInitializeResult != E_OUTOFMEMORY &&
	   m_vOleInitializeResult != E_UNEXPECTED))
	{
		eo << "OleUninitialize will be called" << ef;
		OleUninitialize();
	}
}

STDMETHODIMP_(ULONG) EMWinDropSource::AddRef()
{
	return ++m_vRefs;
}

STDMETHODIMP EMWinDropSource::GiveFeedback(DWORD p_vEffect)
{
	return DRAGDROP_S_USEDEFAULTCURSORS;
}

STDMETHODIMP EMWinDropSource::QueryContinueDrag(BOOL p_vEscapePressed, DWORD p_vKeyState)
{
	if(p_vEscapePressed)
		return DRAGDROP_S_CANCEL;
	else if(!(p_vKeyState & MK_LBUTTON) && !(p_vKeyState & MK_RBUTTON))
		return DRAGDROP_S_DROP;
	else
		return NOERROR;
}

STDMETHODIMP EMWinDropSource::QueryInterface(REFIID p_vReferensInterfaceID, void FAR* FAR* p_oppObject)
{
	if(p_vReferensInterfaceID == IID_IUnknown || p_vReferensInterfaceID == IID_IDropSource)
	{
		*p_oppObject = this;
		++m_vRefs;
		return NOERROR;
	}
	*p_oppObject = NULL;

	return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) EMWinDropSource::Release()
{
	if(--m_vRefs == 0)
	{
		delete this;
		return 0;
	}
	return m_vRefs;

}

#endif