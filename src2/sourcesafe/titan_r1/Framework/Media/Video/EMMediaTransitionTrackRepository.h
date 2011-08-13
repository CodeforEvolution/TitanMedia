/*******************************************************
* Creator: Martin Johansson
* Portability: Non-Native
*-------------------------------------------------------
* 
* This class holds the transition tracks for each project.
* There should be one transition repository per project
*
*******************************************************/

#ifndef EM_MEDIA_TRANSITION_TRACK_REPOSITORY
#define EM_MEDIA_TRANSITION_TRACK_REPOSITORY

#include "EMMediaItemContainer.h"
#include "EMMediaVideoTransitionTrack.h"

class EMMediaTransitionTrackRepository : public EMMediaItemContainer<EMMediaVideoTransitionTrack>
{
public:
	EMMediaTransitionTrackRepository();
	virtual ~EMMediaTransitionTrackRepository();

//	static EMMediaTransitionTrackRepository* Instance();
	static void Delete();
	bool SaveData(EMProjectDataSaver* p_opSaver);
	bool LoadData(EMProjectDataLoader* p_opLoader);
	void OnItemAdd(); //Inherited from EMMediaItemContainer. Added by Richard 010821
	void OnItemRemove(); //Inherited from EMMediaItemContainer. Added by Richard 010821
	bool RestoreAfterLoad();
protected:

private:
//	static EMMediaTransitionTrackRepository* m_opInstance;
};

#endif