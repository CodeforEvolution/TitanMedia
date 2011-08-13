/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __COMMAND_GROUP_CLIPS_FROM_LIST
#define __COMMAND_GROUP_CLIPS_FROM_LIST

#include "EMCommand.h"
#include "EMGClip.h"

#include <list>
#include <utility>

class TrackView;

class CommandGroupClipsFromList : public EMCommand
{
public:
	CommandGroupClipsFromList(TrackView* p_opTrackView, list<EMGClip*> p_oGroupedClips, list<pair<EMGClip*, EMGClip*> > p_oGroupedClipsProperties);
	CommandGroupClipsFromList(TrackView* p_opTrackView);
	EMCommand* CloneAndClear();
	void ExecuteE(){};
	void* ExecuteE(void* p_upParameterOne, void* p_upParameterTwo, void* p_upParameterThree);
	const char* GetShortDescription();
	bool IsUndoable();
	bool RequiresParameters();
	void UndoE();

private:
	list<EMGClip*> m_oGroupedClips;
	list<pair<EMGClip*, EMGClip*> > m_oGroupedClipsProperties;
	TrackView* m_opTrackView;
};

#endif
