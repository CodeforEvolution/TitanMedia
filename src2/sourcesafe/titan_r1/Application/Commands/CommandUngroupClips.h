/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __COMMAND_UNGROUP_CLIPS
#define __COMMAND_UNGROUP_CLIPS

#include "EMCommand.h"
#include "EMGClip.h"

#include <list>
#include <utility>

class TrackView;

class CommandUngroupClips : public EMCommand
{
public:
	CommandUngroupClips(TrackView* p_opTrackView, list<EMGClip*> p_oGroupedClips, list<pair<EMGClip*, EMGClip*> > p_oGroupedClipsProperties);
	CommandUngroupClips(TrackView* p_opTrackView);
	EMCommand* CloneAndClear();
	void ExecuteE();
	void* ExecuteE(void* p_upParameterOne, void* p_upParameterTwo, void* p_upParameterThree) { return NULL; };
	const char* GetShortDescription();
	bool IsUndoable();
	bool RequiresParameters();
	void UndoE();

private:
	list<EMGClip*> m_oUngroupedClips;
	list<pair<EMGClip*, EMGClip*> > m_oUngroupedClipsProperties;
	TrackView* m_opTrackView;
};

#endif
