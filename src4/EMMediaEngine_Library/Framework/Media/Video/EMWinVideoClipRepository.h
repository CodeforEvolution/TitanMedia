/*******************************************************
* Creator: Richard Kronfalt
* Portability: Native
*-------------------------------------------------------
* 
* This class is a repository of all the clips that exist
* within a song/project. To find out which track a clip
* belongs to one has to ask the clip, since it has a 
* pointer to the owning EMMediaTrack.
*
* This class supplies a way of accessing the data in 
* the clips, through the GetNextBuffers method.
* It returns a number of EMMediaDataBuffers (through adding
* them to a list which is sent in). The buffers returned
* correspond to the media data that should be played
* at a certain point in songtime, namely the time given
* by EMBeMediaTimer's method ThenFrame and/or ThenTime.
* See EMBeMediaTimer for information about the concepts
* of Now-time and Then-time.
* The buffers returned are read from all the clips active
* at that time.
*
* When asking GetNextBuffers for the next set of buffers
* one has to supply a filtering-flag, for deciding which
* clip-types to read buffers from, e.g. EM_TYPE_RAW_VIDEO,
* EM_TYPE_ANY, EM_TYPE_ANY_AUDIO, etc. 
*
* If there are no active clips at "this" point in songtime
* (as reported by ThenTime - see above), the method 
* just returns the sent in list unchanged, i.e., naturally,
* without adding any EMMediaDataBuffers to it.
* 
* This class also contains methods for investigating the
* "song" as a whole, e.g. HasAudio and HasVideo returns
* true if there are any audio or video clips respectively
* in the song/project. 
*
* FramesToNextClip returns the number of audio frames 
* (calculated using the global audio format) to the start
* of the next clip, from a certain point in time.
*
* TODO: Make sure that FramesToNextClip only includes
* audio clips or video clips in the search, when needed.
*
* Richard Kronfält, 00-11-28
*******************************************************/

#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#ifndef __EM_WIN_VIDEO_CLIP_REPOSITORY
#define __EM_WIN_VIDEO_CLIP_REPOSITORY

#include "EMMediaGlobals.h"
#include "EMMediaClip.h"
#include "EMMediaItemContainer.h"
#include "EMMediaDataBuffer.h"
#include "EMMediaClipRepository.h"
#include "EMVideoClip.h"

#include <list>

class EMWinVideoClipRepository : public EMMediaClipRepository
{
public:
	EMWinVideoClipRepository();
	~EMWinVideoClipRepository();
	int32 GetID() const;
	void GetNextBuffers(list<EMMediaDataBuffer*>* p_opList, EMMediaType p_eType, int64 p_vTimeNow, bool p_vSeeking = false);
	bool InitCheckE();
	void SeekAndDisplay(int64 p_vNow);

	bool GetClipsInTimeslot(list<EMMediaClip*>* p_opList, int64 p_vTime);

private:
	EMMediaClip* GetPriorityClip(int64 p_vNow, int64 p_vFramesToNextClip, list<EMMediaDataBuffer*>* p_opList, list<int32>* p_oID);
	bool IsSoloActivated();
	void SeekTo(int64 p_vNewSongFrame);
	int64 FramesToNextClip(int64 p_vFromFrame);
	int32 m_vID;
	EMMediaClip* m_opPreviousClip;

	EMMediaDataBuffer *GetDataBuffer(EMVideoClip* p_opClip, int p_vTime, bool p_vSeeking);
};

#endif

#endif
