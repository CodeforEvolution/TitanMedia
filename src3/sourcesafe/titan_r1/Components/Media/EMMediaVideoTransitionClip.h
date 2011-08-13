#ifndef __EM_VIDEO_TRANSITION_CLIP
#define __EM_VIDEO_TRANSITION_CLIP

#include "EMGlobals.h"
#include "EMMediaClip.h"

#include "EMPlugin.h"
#include "EMPluginEntry.h"
#include "EMMediaDataBuffer.h"
#include "EMBufferDestination.h"
#include "EMWinDirectXVideoTransition.h"

class EMVideoTransitionClip : public EMMediaClip, public EMBufferDestination
{
public:
	EMVideoTransitionClip(EMMediaTrack* p_opOwner, EMPluginEntry* p_opTransEntry);
	EMVideoTransitionClip(EMMediaTrack* p_opOwner);
	~EMVideoTransitionClip();

	virtual EMMediaClip* CloneClip(int64 p_vStartFrame, EMMediaTrack* p_opBelongsToTrack);
	
	bool InitCheckE();
	virtual int64 GetActiveLength(); //Forced to ovveride due to C++ rules (?). Fukked up otherwise!
	void SetDeleted(bool p_vFlag); //Forced to ovveride due to C++ rules (?). Fukked up otherwise!

	EMMediaDataBuffer* GetBuffer() const;

	bool IsActiveNow(int64 p_vNow);

	virtual bool PrepareToPlayE();
	virtual EMMediaDataBuffer* ProcessBufferE(list<EMMediaDataBuffer*>* p_opBufferList);

	void SetTransition(EMPluginEntry* p_opTransEntry);
	EMPlugin* GetTransition();

	virtual int64 GetStart(); // const;
	virtual void SetStart(int64 p_vTime);
	virtual int64 GetMediaStart() const;

	virtual void MoveClip(int64 p_vNewTime);

	virtual int64 GetStop() const;
	virtual void SetStop(int64 p_vTime);
	virtual int64 GetMediaStop() const;
	virtual int64 GetMediaLength() const;

	virtual void SetDestination(EMBufferDestination* p_opBuffer);
	virtual EMBufferDestination* GetDestination();

	int64 GetMarkInLength() const;
	void SetMarkInLength(int64 p_vLength);
	
	int64 GetMarkOutLength() const;
	void SetMarkOutLength(int64 p_vLength);

	virtual void SwapInputs(bool p_vSwap);

	bool SaveData(EMProjectDataSaver* p_opSaver);
	bool LoadData(EMProjectDataLoader* p_opLoader);

protected:
	bool m_vSwap;
	int64 m_vTransStart;
	int64 m_vTransStop;

	EMVideoTransitionClip();

	EMVideoTransition* m_opTrans;
	EMPluginEntry* m_opTransEntry;
};

#endif