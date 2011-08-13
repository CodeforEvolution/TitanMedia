#ifndef __EM_MEDIA_IMAGE_CLIP
#define __EM_MEDIA_IMAGE_CLIP

#include "EMGlobals.h"
#include "EMMediaClip.h"

class EMImageClip : public EMMediaClip
{
public:
	EMImageClip(EMMediaType p_eType, const char* p_vpFileName, int64 p_vStartFrame, EMMediaTrack* p_opBelongsToTrack);
	EMImageClip(EMMediaTrack* p_opOwner);
	~EMImageClip();

	virtual EMMediaClip* CloneClip(int64 p_vStartFrame, EMMediaTrack* p_opBelongsToTrack);
	
	bool InitCheckE();
	int64 GetActiveLength(); //Forced to ovveride due to C++ rules (?). Fukked up otherwise!
	void SetDeleted(bool p_vFlag); //Forced to ovveride due to C++ rules (?). Fukked up otherwise!

	EMMediaDataBuffer* GetBuffer() const;
	EMMediaDataBuffer* GetBufferAt(int64 p_vTime, bool p_vSeeking);

	bool IsActiveNow(int64 p_vNow);

	virtual int64 GetStart(); // const;
	virtual void SetStart(int64 p_vTime);
	virtual int64 GetMediaStart() const;

	virtual int64 GetStop() const;
	virtual void SetStop(int64 p_vTime);
	virtual int64 GetMediaStop() const;
	virtual int64 GetMediaLength() const;

	int64 GetMarkInLength() const;
	void SetMarkInLength(int64 p_vLength);
	
	int64 GetMarkOutLength() const;
	void SetMarkOutLength(int64 p_vLength);

	bool SaveData(EMProjectDataSaver* p_opSaver);
	bool LoadData(EMProjectDataLoader* p_opLoader);

protected:
	EMImageClip();

	uint64 m_vStart;
	uint64 m_vStop;
};

#endif