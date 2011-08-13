#include "EMWinDXTransitionEntry.h"
#include "EMWinDXVideoTransition.h"

#include <dshow.h>


EMWinDXTransitionEntry::EMWinDXTransitionEntry(string p_oPluginName, IMoniker* p_opIMoniker, IDirectDraw7* p_opIDD, EMMediaType p_vType)
	: EMWinTransitionEntry(p_oPluginName, p_opIMoniker, p_opIDD, p_vType)
{
	memset(&m_oDefaultParams, 0, sizeof(m_oDefaultParams));
	
	m_oDefaultParams.m_vID = EM_TRANSITION_DX;
	m_oDefaultParams.m_vSwap = false;
}

EMWinDXTransitionEntry::~EMWinDXTransitionEntry()
{
}

void* EMWinDXTransitionEntry::GetDefaultProperties()
{
	return &m_oDefaultParams;
}

EMPlugin* EMWinDXTransitionEntry::WrapFilter(IMoniker* p_opIMoniker, IDirectDraw7* p_opIDD)
{
	EMPlugin* opTheResultingPlugin = NULL;	
	opTheResultingPlugin = EM_new EMWinDXVideoTransition(GetName(), p_opIDD, p_opIMoniker);
	return opTheResultingPlugin;
}

string EMWinDXTransitionEntry::MakeUnique(string& m_oName)
{
	return m_oName;
}