/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* This class is a base class for all destructive audio
* editing commands.
*******************************************************/

#include "EMGlobals.h"

#ifndef __DESTRUCTIVE_EDIT_BASE
#define __DESTRUCTIVE_EDIT_BASE

#include <list>

class AudioGClip;
class EMCommand;
class TrackView;

class DestructiveEditBase
{
public:
	DestructiveEditBase(TrackView* p_opTrackView);
	DestructiveEditBase(TrackView* p_opTrackView, list<AudioGClip*> p_oAffectedClips, list<AudioGClip*> p_oNewClips, list<EMCommand*> p_oUndoCommands);
	void DestructiveEdit(uint32 p_vDestructiveEditMediaCommandID, int32 p_vEffectEntryID = -1, bool vFillSpaceWithClonedClips = true, int64 p_vDialogParameter = 0);

protected:
	list<AudioGClip*> m_oAffectedClips;
	list<AudioGClip*> m_oNewClips;
	TrackView* m_opTrackView;
	list<EMCommand*> m_oUndoCommands;
};

#endif
