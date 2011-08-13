#include "EMGEffectsRepository.h"

#include "CommandIDs.h" // FIXME: Application file included in framework = bad
#include "EMCommandRepository.h"
#include "EMMediaGlobals.h"

EMGEffectsRepository* EMGEffectsRepository::m_opInstance = NULL;

EMGEffectsRepository::EMGEffectsRepository() :
m_vInitialized(false)
{
}

EMGEffectsRepository::~EMGEffectsRepository()
{
	string* opString;
	map<int32, string*>::iterator oIterator = m_oAudioEffects.begin();
	while(oIterator != m_oAudioEffects.end())
	{
		opString = oIterator -> second;
		oIterator++;
		delete opString;
	}

	oIterator = m_oVideoEffects.begin();
	while(oIterator != m_oVideoEffects.end())
	{
		opString = oIterator -> second;
		oIterator++;
		delete opString;
	}
}

EMGEffectsRepository* EMGEffectsRepository::Instance()
{
	if(m_opInstance == NULL)
		m_opInstance = EM_new EMGEffectsRepository();
	return m_opInstance;
}

string* EMGEffectsRepository::GetEffectName(EMMediaType p_eMediaType, int p_vIndex, int32* p_vpEffectEntryID)
{
	if(!m_vInitialized)
		Init(); // Lazy init

	map<int32, string*>::iterator oIterator;
	int vCounter(0);

	if(p_eMediaType == EM_TYPE_ANY_AUDIO)
	{
		oIterator = m_oAudioEffects.begin();
		while(oIterator != m_oAudioEffects.end())
		{
			if(vCounter == p_vIndex)
			{
				*p_vpEffectEntryID = oIterator -> first;
				return oIterator -> second;
			}
			oIterator++;
			vCounter++;
		}
		return NULL;
	}

	if(p_eMediaType == EM_TYPE_ANY_VIDEO)
	{
		oIterator = m_oVideoEffects.begin();
		while(oIterator != m_oVideoEffects.end())
		{
			if(vCounter == p_vIndex)
			{
				*p_vpEffectEntryID = oIterator -> first;
				return oIterator -> second;
			}
			oIterator++;
			vCounter++;
		}
		return NULL;
	}

	return NULL;
}

map<int32, string*>* EMGEffectsRepository::GetEffectNames(EMMediaType p_eMediaType)
{
	if(!m_vInitialized)
		Init();

	if(p_eMediaType == EM_TYPE_ANY_AUDIO)
		return &m_oAudioEffects;
	else if(p_eMediaType == EM_TYPE_ANY_VIDEO)
		return &m_oVideoEffects;

	return NULL;
}

void EMGEffectsRepository::Init()
{
	list<int32>* opFXEntries = static_cast<list<int32>* >(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_EFFECT_ENTRIES, NULL, NULL, NULL));

	EMMediaEffectEntryInfo* opEntryInfo = NULL;
	list<int32>::const_iterator oIterator = opFXEntries -> begin();
	int32 vEntry;
	while(oIterator != opFXEntries -> end())
	{
		vEntry = *oIterator;
		opEntryInfo = static_cast<EMMediaEffectEntryInfo*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_EFFECT_ENTRY_INFO, &vEntry, NULL, NULL));
		if(opEntryInfo -> IsAudio())
			m_oAudioEffects[vEntry] = EM_new string((opEntryInfo -> m_oPluginName).c_str());
		else if(opEntryInfo -> IsVideo())
			m_oVideoEffects[vEntry] = EM_new string((opEntryInfo -> m_oPluginName).c_str());
		oIterator++;
	}

	m_vInitialized = true;
}


