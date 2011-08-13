#include "EMWinChromaTransitionEntry.h"
#include "EMWinChromaVideoTransition.h"

#include <dshow.h>


EMWinChromaTransitionEntry::EMWinChromaTransitionEntry(string p_oPluginName, IMoniker* p_opIMoniker, IDirectDraw7* p_opIDD, EMMediaType p_vType)
	: EMWinTransitionEntry(p_oPluginName, p_opIMoniker, p_opIDD, p_vType)
{
	memset(&m_oDefaultParams, 0, sizeof(m_oDefaultParams));

	m_oDefaultParams.m_vID = EM_TRANSITION_KEY;
	m_oDefaultParams.m_vParams.m_vKey.m_vColorID = EM_KEY_BLUE;
	m_oDefaultParams.m_vParams.m_vKey.m_vTolerance = 100;
	m_oDefaultParams.m_vSwap = false;
}

EMWinChromaTransitionEntry::~EMWinChromaTransitionEntry()
{
}

void* EMWinChromaTransitionEntry::GetDefaultProperties()
{
	return &m_oDefaultParams;
}

/*
EMPlugin* EMWinChromaTransitionEntry::WrapFilter(IBaseFilter* p_opIFilter)
{
	return EM_new EMWinChromaVideoTransition(GetName(), p_opIFilter);
}
*/

EMPlugin* EMWinChromaTransitionEntry::WrapFilter(IMoniker* p_opIMoniker, IDirectDraw7* p_opIDD)
{
	EMPlugin* opTheResultingPlugin = NULL;	

	opTheResultingPlugin = EM_new EMWinChromaVideoTransition(GetName(), p_opIDD, p_opIMoniker);

	return opTheResultingPlugin;
}

string EMWinChromaTransitionEntry::MakeUnique(string& m_oName)
{
	return m_oName;
}