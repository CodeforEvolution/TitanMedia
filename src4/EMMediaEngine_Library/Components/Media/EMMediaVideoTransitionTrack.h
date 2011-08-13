/*******************************************************
* Creator: Martin Johansson
* Portability: Native
*-------------------------------------------------------
* 
* Martin Johansson, 01-08-01
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_MEDIA_VIDEO_TRANSITIONS_TRACK
#define __EM_MEDIA_VIDEO_TRANSITIONS_TRACK

#include "EMGlobals.h"
#include "EMMediaTrack.h"
#include "EMBufferDestination.h"

class EMMediaVideoTransitionTrack : public EMMediaTrack, public EMBufferDestination
{
public:
	EMMediaVideoTransitionTrack(string p_oName);
	~EMMediaVideoTransitionTrack();

	virtual bool PrepareToPlayE();
	virtual EMMediaDataBuffer* ProcessBufferE(list<EMMediaDataBuffer*>* p_opBufferList);
	virtual int32 CreateClip(int64 p_vAtFrame, string p_oName, EMFileInputDescriptor* p_opSource);
	virtual void SetDestination(EMBufferDestination* p_opDestination);
	EMBufferDestination* GetDestination() const;

	virtual void SetTrackDestination(EMBufferDestination* p_opTrackDataReceiver);

	virtual uint32 GetPriority() const;
	virtual void SetPriority(uint32 p_vPrio);

	void Connect();
	void Disconnect();

	EMMediaType GetType() const;
	int32 GetID() const;

	void SetParentID(uint32 p_vParentID);
	uint32 GetParentID();

	virtual EMMediaBufferSource* GetSilenceGenerator() const;

	virtual bool LoadData(EMProjectDataLoader* p_opLoader);
	virtual bool SaveData(EMProjectDataSaver* p_opSaver);
	virtual bool RestoreAfterLoad();
protected:
	bool LoadClips(EMProjectDataLoader* p_opLoader);
	bool SaveClips(EMProjectDataSaver* p_opSaver);

	uint32 m_vParentID;
	EMMediaTrack* m_opParentTrack;
};

#endif