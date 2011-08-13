/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_G_EFFECTS_REPOSITORY
#define __EM_G_EFFECTS_REPOSITORY

#include "EMListenerRepository.h"
#include "EMMediaGlobals.h"

//#include <list>
#include <map>
#include <string>
#include <vector>

const int MAX_EFFECT_SLOTS = 20;

class EMGEffectsRepository : public EMListenerRepository
{
public:
	~EMGEffectsRepository();
	static EMGEffectsRepository* Instance();
	string* GetEffectName(EMMediaType p_eMediaType, int p_vindex, int32* p_vpEffectEntryID);
	map<int32, string*>* GetEffectNames(EMMediaType p_eMediaType);

protected:
	EMGEffectsRepository();

private:
	void Init();

private:
	static EMGEffectsRepository* m_opInstance;
	map<int32, string*> m_oAudioEffects;
	map<int32, string*> m_oVideoEffects;
	bool m_vInitialized;
};

#endif