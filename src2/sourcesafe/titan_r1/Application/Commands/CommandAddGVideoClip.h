/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __COMMAND_ADD_G_VIDEO_CLIP
#define __COMMAND_ADD_G_VIDEO_CLIP

#include "EMCommand.h"

class EMGClip;
class TrackInfo;
class TrackView;

class CommandAddGVideoClip : public EMCommand
{
public:
	CommandAddGVideoClip(TrackView* p_opTrackView, TrackInfo* p_opTrackInfo, EMGClip* p_opClip);
	~CommandAddGVideoClip();
	EMCommand* CloneAndClear();
	void ExecuteE() {};
	void* ExecuteE(void* p_upMediaClipID, void* p_upTrackIndex, void* p_upTime);
	const char* GetShortDescription();
	bool IsUndoable();
	bool RequiresParameters();
	void UndoE();

private:
	EMGClip* m_opClip;
	TrackInfo* m_opTrackInfo;
	TrackView* m_opTrackView;
};

#endif

