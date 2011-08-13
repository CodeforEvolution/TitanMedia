#include "EMWinSMPTETransitionEntry.h"
#include "EMWinSMPTEVideoTransition.h"

#include <dshow.h>


EMWinSMPTETransitionEntry::EMWinSMPTETransitionEntry(string p_oPluginName, IMoniker* p_opIMoniker, IDirectDraw7* p_opIDD, EMMediaType p_vType, int p_vSMPTECode)
	: EMWinTransitionEntry(p_oPluginName, p_opIMoniker, p_opIDD, p_vType),
	m_vSMPTECode(p_vSMPTECode)
{
	memset(&m_oDefaultParams, 0, sizeof(m_oDefaultParams));

	m_oDefaultParams.m_vID = EM_TRANSITION_WIPE;
	m_oDefaultParams.m_vParams.m_vWipe.m_vReplicateX = 1;
	m_oDefaultParams.m_vParams.m_vWipe.m_vReplicateY = 1;
	m_oDefaultParams.m_vSwap = false;

	m_oUniqueID = m_oUniqueID + " " + p_oPluginName;
}

EMWinSMPTETransitionEntry::~EMWinSMPTETransitionEntry()
{
}

void* EMWinSMPTETransitionEntry::GetDefaultProperties()
{
	return &m_oDefaultParams;
}

EMPlugin* EMWinSMPTETransitionEntry::WrapFilter(IMoniker* p_opIMoniker, IDirectDraw7* p_opIDD)
{
	EMPlugin* opTheResultingPlugin = NULL;	
	opTheResultingPlugin = EM_new EMWinSMPTEVideoTransition(GetName(), p_opIDD, p_opIMoniker, m_vSMPTECode);
	return opTheResultingPlugin;
}

string EMWinSMPTETransitionEntry::MakeUnique(string& m_oName)
{
	m_oName += m_vSMPTECode;

	return m_oName;
}