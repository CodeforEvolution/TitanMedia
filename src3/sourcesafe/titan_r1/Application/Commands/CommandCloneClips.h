/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __COMMAND_CLONE_CLIPS
#define __COMMAND_CLONE_CLIPS

#include "EMCommand.h"
#include "EMGClip.h"

#include <list>

class EMGClip;
class TrackView;

class CommandCloneClips : public EMCommand
{
public:
	CommandCloneClips(TrackView* p_opTrackView, list<EMGClip*> p_oClonedClips);
	CommandCloneClips(TrackView* p_opTrackView);
	EMCommand* CloneAndClear();
	void ExecuteE() {};
	void* ExecuteE(void* p_opParameterOne, void* p_opParameterTwo, void* p_opParameterThree);
	const char* GetShortDescription();
	bool IsUndoable();
	bool RequiresParameters();
	void UndoE();

private:
	list<EMGClip*> m_oClonedClips;
	TrackView* m_opTrackView;
	int64 m_vPositionOffset;
	int32 m_vTrackOffset;
};

#endif
