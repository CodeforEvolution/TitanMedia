/*******************************************************
* Creator: Richard Kronfält
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_INFO_COMMAND_GET_ALL_TRACK_IDS
#define __MEDIA_INFO_COMMAND_GET_ALL_TRACK_IDS

#include "EMCommand.h"

class MediaInfoCommandGetAllTrackIDs : public EMCommand
{
public:
	MediaInfoCommandGetAllTrackIDs();
	void ExecuteE() {};
	void* ExecuteE(void*, void*, void*);
	bool RequiresParameters();
	bool IsUndoable();
	
private:
	list<int32> m_oReturnValue;

};

#endif
