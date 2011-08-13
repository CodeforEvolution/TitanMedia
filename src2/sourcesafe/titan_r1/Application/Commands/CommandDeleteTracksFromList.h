/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __COMMAND_DELETE_TRACKS_FROM_LIST
#define __COMMAND_DELETE_TRACKS_FROM_LIST

#include "EMCommand.h"
#include "EMGTrack.h"

#include <vector>

class TrackView;

class CommandDeleteTracksFromList : public EMCommand
{
public:
	CommandDeleteTracksFromList(TrackView* p_opTrackView);
	CommandDeleteTracksFromList(TrackView* p_opTrackView, EMCommand* p_opUndoedClipDeletion, vector<EMGTrack*> p_oTracks, list<EMCommand*> p_oUndoedMediaTrackDeletions);
	EMCommand* CloneAndClear();
	void ExecuteE() {};
	void* ExecuteE(void* p_vParameterOne, void* p_vParameterTwo, void* p_vParameterThree);
	const char* GetShortDescription();
	bool IsUndoable();
	bool RequiresParameters();
	void UndoE();

private:
	TrackView* m_opTrackView;
	vector<EMGTrack*> m_oTracks;
	EMCommand* m_opUndoedClipDeletion;
	list<EMCommand*> m_oUndoedMediaTrackDeletions;
};

#endif
