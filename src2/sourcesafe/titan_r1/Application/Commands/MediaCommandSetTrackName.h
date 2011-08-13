/*******************************************************
* Creator: Richard Kronfält
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_COMMAND_SET_TRACK_NAME
#define __MEDIA_COMMAND_SET_TRACK_NAME

#include "EMCommand.h"

class EMMediaTrack;

class __declspec(dllexport) MediaCommandSetTrackName : public EMCommand
{
public:
	MediaCommandSetTrackName(EMMediaTrack* p_opRecentTrack = NULL, string p_oRecentName = "");
	void ExecuteE() {};

	//Parameter 1 = track ID, parameter 2 = new track name
	void* ExecuteE(void* p_opParameterOne, void* p_opParameterTwo, void* p_opParameterThree);
	bool RequiresParameters();
	void UndoE();
	bool IsUndoable();
	EMCommand* CloneAndClear();

private:
	EMMediaTrack* m_opRecentTrack;	
	string m_oRecentName;
};

#endif
