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

#ifndef __EM_MEDIA_CLIP_REPOSITORY
#define __EM_MEDIA_CLIP_REPOSITORY

#include "EMMediaGlobals.h"
#include "EMMediaClip.h"
#include "EMMediaItemContainer.h"
#include "EMMediaDataBuffer.h"

#include <list>

class EMMediaTrack;
class EMProjectDataLoader;
class EMProjectDataSaver;

class EMMediaClipRepository : public EMMediaItemContainer<EMMediaClip>
{
public:
	virtual ~EMMediaClipRepository();
	int64 CountClipsForTrack(const EMMediaTrack* p_opTrack, EMMediaType p_eSpecType = EM_TYPE_ANY);
	int64 CountActiveClips();
	int32 GetID() const;
	static EMMediaClipRepository* New(EMMediaType p_eType);
	virtual void GetNextBuffers(list<EMMediaDataBuffer*>* p_opList, EMMediaType p_eType, int64 p_vTimeNow, bool p_vSeeking = false) = 0;
	virtual bool InitCheckE() = 0;
	
	void OnItemAdd(); //Inherited from EMMediaItemContainer. Added by Richard 010821
	void OnItemRemove(); //Inherited from EMMediaItemContainer. Added by Richard 010821
	bool ClearData();
	bool SaveData(EMProjectDataSaver* p_opSaver);
	bool LoadData(EMProjectDataLoader* p_opLoader);

	virtual bool GetClipsInTimeslot(list<EMMediaClip*>* p_opList, int64 p_vTime) {return false;};

protected:
	EMMediaClipRepository(EMMediaType p_eType);
	
private:
	int32 m_vID;
};

#endif
