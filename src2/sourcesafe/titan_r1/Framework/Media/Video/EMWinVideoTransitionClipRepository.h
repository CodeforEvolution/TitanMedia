/*******************************************************
* Creator: Martin Johansson
* Portability: Native
*-------------------------------------------------------
*
* Martin Johansson, 00-11-28
*******************************************************/


#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#ifndef __EM_WIN_VIDEO_TRANSITION_CLIP_REPOSITORY
#define __EM_WIN_VIDEO_TRANSITION_CLIP_REPOSITORY

#include "EMMediaGlobals.h"
#include "EMMediaClip.h"
#include "EMMediaItemContainer.h"
#include "EMMediaDataBuffer.h"
#include "EMMediaClipRepository.h"
#include "EMMediaVideoTransitionClip.h"


class EMWinVideoTransitionClipRepository : public EMMediaClipRepository
{
public:
	EMWinVideoTransitionClipRepository();
	~EMWinVideoTransitionClipRepository();

	virtual void GetNextBuffers(list<EMMediaDataBuffer*>* p_opList, EMMediaType p_eType, int64 p_vTimeNow, bool p_vSeeking = false);
	virtual bool InitCheckE();

	virtual EMMediaClip* GetTransitionsInTimeslot(int64 p_vTime, EMMediaTrack *p_opTrack);
	virtual bool GetClipsInTimeslot(list<EMMediaClip*>* p_opList, int64 p_vTime);
};

#endif
#endif