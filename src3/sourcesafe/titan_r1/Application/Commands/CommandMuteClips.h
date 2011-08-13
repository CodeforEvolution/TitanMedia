/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __COMMAND_MUTE_CLIPS
#define __COMMAND_MUTE_CLIPS

#include "EMCommand.h"
#include "EMGClip.h"

#include <list>

class TrackView;

class CommandMuteClips : public EMCommand
{
public:
	CommandMuteClips(TrackView* p_opTrackView, list<EMGClip*> p_oMutedClips);
	CommandMuteClips(TrackView* p_opTrackView);
	EMCommand* CloneAndClear();
	void ExecuteE() {};
	void* ExecuteE(void* p_upParameterOne, void* p_upParameterTwo, void* p_upParameterThree);
	const char* GetShortDescription();
	bool IsUndoable();
	bool RequiresParameters();
	void UndoE();

private:
	list<EMGClip*> m_oMutedClips;
	TrackView* m_opTrackView;
};

#endif
