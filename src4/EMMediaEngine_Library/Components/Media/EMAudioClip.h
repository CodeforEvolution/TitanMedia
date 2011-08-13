#ifndef __EM_MEDIA_AUDIO_CLIP
#define __EM_MEDIA_AUDIO_CLIP

#include "EMGlobals.h"
#include "EMMediaClip.h" 

class EMMediaEffectTrack;
class EMMediaTrack;

class EMAudioClip : public EMMediaClip
{
public:
	EMAudioClip(EMMediaType p_eType, const char* p_vpFileName, int64 p_vStartFrame, EMMediaTrack* p_opBelongsToTrack);
	EMAudioClip(EMMediaTrack* p_opOwner);
	~EMAudioClip();

	virtual EMMediaClip* CloneClip(int64 p_vStartFrame, EMMediaTrack* p_opBelongsToTrack);

	bool InitCheckE();
	int64 GetActiveLength(); //Forced to ovverride due to C++ rules. Will become fukked up (weird access violations) otherwise!

	void SetDeleted(bool p_vFlag); //Forced to ovverride due to C++ rules. Will become fukked up (weird access violations) otherwise!

	void SetTrack(EMMediaTrack* p_opBelongsToTrack);

	void CloseFile();
	void OpenFile();

protected:
	EMAudioClip();
};

#endif