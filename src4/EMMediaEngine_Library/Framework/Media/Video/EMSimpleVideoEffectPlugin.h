/*******************************************************
* Creator: Martin Johansson
* Portability: Non-Native
*-------------------------------------------------------
*
* This class is a wrapper for some simple video effects
*
*******************************************************/


#include "EMGlobals.h"

#ifndef __EM_SIMLPE_VIDEO_EFFECT_PLUGIN
#define __EM_SIMLPE_VIDEO_EFFECT_PLUGIN

#include "EMNativePlugin.h"
#include "EMMediaDataBuffer.h"
#include "EMVideoEffectFilter.h"

class EMSimpleVideoEffectPlugin : public EMNativePlugin
{
public:
	EMSimpleVideoEffectPlugin(EMVideoEffectFilter *p_poEffect);
	~EMSimpleVideoEffectPlugin();

	virtual bool PrepareToPlayE();
	virtual EMMediaDataBuffer* ProcessBufferE(list<EMMediaDataBuffer*>* p_opBuffers);

	virtual list<EMPluginPreset*>* GetPresets();
	virtual bool ActivatePreset(int32 p_vID);

	virtual bool LoadSetting(EMProjectDataLoader* p_opLoader);
	virtual bool SaveSetting(EMProjectDataSaver* p_opSaver);

	virtual void* GetNativeEffect() const;

	virtual EMPlugin* Clone();

	bool ShowDialog();

protected:
	EMSimpleVideoEffectPlugin();
private:
	EMVideoEffectFilter *m_opEffect;
};

#endif


