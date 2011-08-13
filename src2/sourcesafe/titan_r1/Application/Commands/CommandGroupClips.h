/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __COMMAND_GROUP_CLIPS
#define __COMMAND_GROUP_CLIPS

#include "EMCommand.h"
#include "EMGClip.h"

#include <list>
#include <utility>

class TrackView;

class CommandGroupClips : public EMCommand
{
public:
	CommandGroupClips(TrackView* p_opTrackView, list<EMGClip*> p_oGroupedClips, list<pair<EMGClip*, EMGClip*> > p_oGroupedClipsProperties);
	CommandGroupClips(TrackView* p_opTrackView);
	EMCommand* CloneAndClear();
	void ExecuteE();
	void* ExecuteE(void* p_upParameterOne, void* p_upParameterTwo, void* p_upParameterThree) { return NULL; };
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
