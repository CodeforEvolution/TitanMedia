/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __COMMAND_ADD_G_AUDIO_TRACK
#define __COMMAND_ADD_G_AUDIO_TRACK

#include "EMCommand.h"

class EMGTrack;
class TrackInfo;
class TrackView;

class CommandAddGAudioTrack : public EMCommand
{
public:
	CommandAddGAudioTrack(TrackView* p_opTrackView, TrackInfo* p_opTrackInfo);
	CommandAddGAudioTrack(TrackView* p_opTrackView, TrackInfo* p_opTrackInfo, EMGTrack* p_opTrack);
	~CommandAddGAudioTrack();
	EMCommand* CloneAndClear();
	void ExecuteE() {};
	void* ExecuteE(void* p_upParameterOne, void* p_upParameterTwo, void* p_upParameterThree);
	const char* GetShortDescription();
	bool IsUndoable();
	bool RequiresParameters();
	void UndoE();

private:
	EMGTrack* m_opTrack;
	TrackInfo* m_opTrackInfo;
	TrackView* m_opTrackView;
};

#endif

