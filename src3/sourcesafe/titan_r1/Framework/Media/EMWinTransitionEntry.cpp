#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#include "EMWinTransitionEntry.h"
#include "EMWinSMPTEVideoTransition.h"

#include "EMWinMediaUtility.h"

#include <streams.h>
#include <qedit.h>


EMWinTransitionEntry::EMWinTransitionEntry(string p_oPluginName, IMoniker* p_opIMoniker, IDirectDraw7* p_opIDD, EMMediaType p_vType)
	:	EMPluginEntry(p_oPluginName, p_vType, EM_WINDOWS_DIRECTX_VIDEO_TRANSITION),
		m_opIMoniker(p_opIMoniker),
		m_opIDD(p_opIDD)
{
	p_opIMoniker -> AddRef();
	p_opIDD -> AddRef();

	HRESULT hr;

	IPropertyBag* opIPropBag = NULL;
	hr = p_opIMoniker -> BindToStorage(0, 0, IID_IPropertyBag, (void**) &opIPropBag);

	if(hr != S_OK)
		EMDebugger("ERROR! Could not CoCreateInstance of COM object, using the CLSID reported by the system inspector!");

	VARIANT var;
	var.vt = VT_BSTR;

	hr = opIPropBag -> Read(L"FriendlyName",&var,NULL);

	string str = static_cast<EMWinMediaUtility*>(EMMediaUtility::Instance()) -> UnicodeToString(var.bstrVal);

	MakeUnique(str);

	opIPropBag -> Release();

	m_oUniqueID = MakeUnique(str);
}

EMWinTransitionEntry::~EMWinTransitionEntry()
{
	m_opIMoniker -> Release();
	m_opIDD -> Release();
}

EMPlugin* EMWinTransitionEntry::InstantiateNativePlugin()
{
	EMPlugin* opTheResultingPlugin = NULL;	
	opTheResultingPlugin = WrapFilter(m_opIMoniker, m_opIDD); //pIFilter); //EM_new EMWinSMPTEVideoTransition(GetName(), pIFilter, m_vSMPTECode);
	IncreaseInstanceCount();					//Make sure we know how many instances there's been of this effect!
	opTheResultingPlugin -> SetEntryID(GetID()); //Also, make sure we store the ID number of this entry in the plugin so we can find the entry later on...
	opTheResultingPlugin -> PrepareToPlayE();
	return opTheResultingPlugin;
}

string EMWinTransitionEntry::GetUniqueID()
{
	return m_oUniqueID;
}

string EMWinTransitionEntry::MakeUnique(string& p_oName)
{
	return p_oName;
}

#endif