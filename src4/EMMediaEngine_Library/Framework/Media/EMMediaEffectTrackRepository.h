/*******************************************************
* Creator: Richard Kronfalt
* Portability: Non-Native
*-------------------------------------------------------
* 
* This class holds the effect tracks for each project.
* There should be one effect repository per project,
* containing the effect tracks and their respective 
* effect chains for that project.
*
*******************************************************/

#ifndef EM_MEDIA_EFFECT_TRACK_REPOSITORY
#define EM_MEDIA_EFFECT_TRACK_REPOSITORY

#include "EMMediaItemContainer.h"
#include "EMMediaEffectTrack.h"

class EMMediaEffectTrackRepository : public EMMediaItemContainer<EMMediaEffectTrack>
{
public:
	static EMMediaEffectTrackRepository* Instance();
	static void Delete();
	bool SaveData(EMProjectDataSaver* p_opSaver);
	bool LoadData(EMProjectDataLoader* p_opLoader);
	void OnItemAdd(); //Inherited from EMMediaItemContainer. Added by Richard 010821
	void OnItemRemove(); //Inherited from EMMediaItemContainer. Added by Richard 010821
	bool RestoreAfterLoad();

protected:
	EMMediaEffectTrackRepository();
	virtual ~EMMediaEffectTrackRepository();

private:
	static EMMediaEffectTrackRepository* m_opInstance;
};

#endif