#ifndef __EM_MEDIA_VIDEO_CLIP
#define __EM_MEDIA_VIDEO_CLIP

#include "EMGlobals.h"
#include "EMMediaClip.h"

class EMVideoClip : public EMMediaClip
{
public:
	EMVideoClip(EMMediaType p_eType, const char* p_vpFileName, int64 p_vStartFrame, EMMediaTrack* p_opBelongsToTrack);
	EMVideoClip(EMMediaTrack* p_opOwner);
	~EMVideoClip();

	virtual EMMediaClip* CloneClip(int64 p_vStartFrame, EMMediaTrack* p_opBelongsToTrack);
	
	bool InitCheckE();
	int64 GetActiveLength(); //Forced to ovveride due to C++ rules (?). Fukked up otherwise!
	void SetDeleted(bool p_vFlag); //Forced to ovveride due to C++ rules (?). Fukked up otherwise!

	EMMediaDataBuffer* GetBuffer() const;
	EMMediaDataBuffer* GetBufferAt(int64 p_vTime, bool p_vSeeking);

	bool IsActiveNow(int64 p_vNow);

protected:
	EMVideoClip();

	EMMediaDataBuffer* m_opBackBuffer;
};

#endif