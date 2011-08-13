/*******************************************************
* Creator: Richard Kronfält
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_INFO_COMMAND_GET_TRACK_INFO
#define __MEDIA_INFO_COMMAND_GET_TRACK_INFO

#include "EMCommand.h"

class MediaInfoCommandGetTrackInfo : public EMCommand
{
public:
	MediaInfoCommandGetTrackInfo();
	void ExecuteE() {};
	void* ExecuteE(void* p_upMediaTrackID, void*, void*);
	bool RequiresParameters();
	bool IsUndoable();

private:
	EMMediaTrackInfo m_oReturnValue;

};

#endif
