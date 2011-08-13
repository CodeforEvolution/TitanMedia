/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __COMMAND_CUT_CLIPS
#define __COMMAND_CUT_CLIPS

#include "EMCommand.h"
#include "EMGClip.h"

#include <list>

class EMGClip;
class TrackView;

class CommandCutClips : public EMCommand
{
public:
	CommandCutClips(TrackView* p_opTrackView, list<EMGClip*> m_oCutClips, list<EMGClip*> m_oNewClips);
	CommandCutClips(TrackView* p_opTrackView);
	EMCommand* CloneAndClear();
	void ExecuteE() {};
	void* ExecuteE(void* , void* , void* );
	const char* GetShortDescription();
	bool IsUndoable();
	bool RequiresParameters();
	void UndoE();
	
private:
	list<EMGClip*> m_oCutClips;
	list<EMGClip*> m_oNewClips;
	TrackView* m_opTrackView;
};

#endif
