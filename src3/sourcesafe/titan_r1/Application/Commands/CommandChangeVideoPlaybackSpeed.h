/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __COMMAND_CHANGE_VIDEO_PLAYBACK_SPEED
#define __COMMAND_CHANGE_VIDEO_PLAYBACK_SPEED

#include "EMCommand.h"

#include <list>

class VideoGClip;
class TrackView;

class CommandChangeVideoPlaybackSpeed : public EMCommand
{
public:
	CommandChangeVideoPlaybackSpeed(TrackView* p_opTrackView);
	CommandChangeVideoPlaybackSpeed(TrackView* p_opTrackView, list<VideoGClip*> p_oAffectedClips, list<EMCommand*> p_oUndoCommands);
	EMCommand* CloneAndClear();
	void ExecuteE() {};
	void* ExecuteE(void* p_upParameterOne, void* p_upParameterTwo, void* p_upParameterThree);
	const char* GetShortDescription();
	bool IsUndoable();
	bool RequiresParameters();
	void UndoE();

private:
	list<VideoGClip*> m_oAffectedClips;
	list<EMCommand*> m_oUndoCommands;
	TrackView* m_opTrackView;
};

#endif
