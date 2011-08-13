/*******************************************************
* Creator: Martin Johansson
* Portability: Non-Native
*-------------------------------------------------------
*
* This class is a wrapper for some simple video effects
*
*******************************************************/


#include "EMGlobals.h"

#ifndef __EM_SIMLPE_VIDEO_EFFECT_DESTINATION
#define __EM_SIMLPE_VIDEO_EFFECT_DESTINATION

#include "EMBufferDestination.h"
#include "EMMediaDataBuffer.h"
#include "EMVideoEffectFilter.h"

class EMSimpleVideoEffectDestination : public EMBufferDestination
{
public:
	EMSimpleVideoEffectDestination(EMVideoEffectFilter *p_poEffect);
	~EMSimpleVideoEffectDestination();

	bool PrepareToPlayE();
	EMMediaDataBuffer* ProcessBufferE(list<EMMediaDataBuffer*>* p_opBufferList);
protected:
	EMSimpleVideoEffectDestination();
private:
	EMVideoEffectFilter *m_opEffect;
};

#endif


