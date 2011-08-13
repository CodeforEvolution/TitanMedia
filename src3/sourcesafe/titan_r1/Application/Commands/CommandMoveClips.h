/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __COMMAND_MOVE_CLIPS
#define __COMMAND_MOVE_CLIPS

#include "EMCommand.h"

#include <list>

class EMGClip;
class TrackView;

class CommandMoveClips : public EMCommand
{
public:
	CommandMoveClips(TrackView* p_opTrackView);
	CommandMoveClips(TrackView* p_opTrackView, list<EMGClip*> p_oMovedClips, int32 p_vTrackOffset, int64 p_vPositionOffset);
	~CommandMoveClips();
	EMCommand* CloneAndClear();
	void ExecuteE() {};
	void* ExecuteE(void* p_opParameterOne, void* p_opParameterTwo, void* p_opParameterThree);
	const char* GetShortDescription();
	bool IsUndoable();
	void RedoE();
	bool RequiresParameters();
	void UndoE();

private:
	void ExecuteBody(list<EMGClip*> p_oMovedClips, int32 p_vTrackOffset, int64 p_vPositionOffset);

private:
	list<EMGClip*> m_oMovedClips;
	TrackView* m_opTrackView;
	int64 m_vPositionOffset;
	int32 m_vTrackOffset;
};

#endif

