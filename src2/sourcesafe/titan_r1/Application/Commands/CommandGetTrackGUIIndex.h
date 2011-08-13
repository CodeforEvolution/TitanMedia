/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __COMMAND_GET_TRACK_GUI_INDEX
#define __COMMAND_GET_TRACK_GUI_INDEX

#include "EMCommand.h"

class EMGTrack;
class TrackInfo;
class TrackView;

class CommandGetTrackGUIIndex : public EMCommand
{
public:
	CommandGetTrackGUIIndex();
	~CommandGetTrackGUIIndex();
	void ExecuteE() {};
	void* ExecuteE(void* p_upParameterOne, void* p_upParameterTwo, void* p_upParameterThree);
	bool IsUndoable();
	bool RequiresParameters();
	void SetTrack(EMGTrack* p_opTrack);

private:
	int32 m_vReturnValue;
};

#endif

