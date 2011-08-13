#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#include "EMPlugin.h"
#include "EMVideoTransition.h"

EMVideoTransition::EMVideoTransition(string p_oPluginName, EMPluginType p_ePluginType)
	:	EMNativePlugin(p_oPluginName, EM_TYPE_RAW_VIDEO, p_ePluginType, EM_TRANSITION),
		m_oName(p_oPluginName),
		m_vSwap(false),
		m_vStart(0),
		m_vStop(5000000)
{
}

EMVideoTransition::~EMVideoTransition()
{
}

string EMVideoTransition::GetName() const
{
	return m_oName;
}

int64 EMVideoTransition::GetStartTime()
{
	return m_vStart;
}

int64 EMVideoTransition::GetStopTime()
{
	return m_vStop;
}

void EMVideoTransition::SwapInputs(bool p_vSwap)
{
	m_vSwap = p_vSwap;
}

	
void EMVideoTransition::SetStartTime(int64 p_vTime)
{
	m_vStart = p_vTime;
}

void EMVideoTransition::SetStopTime(int64 p_vTime)
{
	m_vStop = p_vTime;
}

float EMVideoTransition::CalcTime(EMMediaDataBuffer* p_opBuffer)
{
	REFERENCE_TIME vCurrentTime = p_opBuffer -> GetRelativeTimeToStart();

	if(vCurrentTime <= m_vStart)
	return 0;

	if(vCurrentTime >= m_vStop)		
		return 1.0f;

	return static_cast<float>((vCurrentTime - m_vStart)) / static_cast<float>(m_vStop - m_vStart);
}


bool EMVideoTransition::SaveData(EMProjectDataSaver* p_opSaver)
{
	p_opSaver -> SaveString("EMVideoTransition - begin");
	
	EMBufferDestination::SaveData(p_opSaver);

	p_opSaver -> SaveString(m_oName.c_str());

	p_opSaver -> SaveInt64(m_vStart);
	p_opSaver -> SaveInt64(m_vStop);
	p_opSaver -> SaveBool(m_vSwap);

	p_opSaver -> SaveString("EMVideoTransition - end");

	return true;
}

bool EMVideoTransition::LoadData(EMProjectDataLoader* p_opLoader)
{
	string oChk;

	oChk = p_opLoader -> LoadString();

	if(oChk != "EMVideoTransition - begin")
		EMDebugger("EMVideoTransition::LoadData - Corrupt file!?");

	EMBufferDestination::LoadData(p_opLoader);

	m_oName = p_opLoader -> LoadString();

	m_vStart = p_opLoader ->LoadInt64();
	m_vStop = p_opLoader ->LoadInt64();
	m_vSwap = p_opLoader ->LoadBool();

	oChk = p_opLoader -> LoadString();

	if(oChk != "EMVideoTransition - end")
		EMDebugger("EMVideoTransition::LoadData - Corrupt file!?");

	return true;
}
