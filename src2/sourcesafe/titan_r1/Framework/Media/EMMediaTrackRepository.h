/*******************************************************
* Creator: Richard Kronfalt
* Portability: Non-Native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_MEDIA_TRACK_REPOSITORY
#define __EM_MEDIA_TRACK_REPOSITORY

#include "EMMediaGlobals.h"
#include "EMMediaItemContainer.h"
#include "EMMediaTrack.h"
#include "EMListenerRepository.h"

class EMProjectDataLoader;
class EMProjectDataSaver;

class EMMediaTrackRepository : public EMMediaItemContainer<EMMediaTrack>, public EMListenerRepository
{
public:

	static list<EMMediaTrack*>* m_opTracks;

	EMMediaTrackRepository();
	~EMMediaTrackRepository();
	void AddTrack(EMMediaTrack* p_opTrack);
	void RemoveTrack(EMMediaTrack* p_opTrack);

	void OnItemAdd(); //Inherited from EMMediaItemContainer. Added by Richard 010821
	void OnItemRemove(); //Inherited from EMMediaItemContainer. Added by Richard 010821
	bool LockContainer();
	void UnlockContainer();

	void SetAsActiveTrack(EMMediaTrack* p_opTrack);

	bool ClearData();
	bool LoadData(EMProjectDataLoader* p_opLoader);
	bool SaveData(EMProjectDataSaver* p_opSaver);

private:
};

#endif
