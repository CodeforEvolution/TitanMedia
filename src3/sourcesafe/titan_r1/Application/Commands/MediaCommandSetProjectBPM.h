/*******************************************************
* Creator: Richard Kronfält
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_COMMAND_SET_PROJECT_BPM
#define __MEDIA_COMMAND_SET_PROJECT_BPM

#include "EMCommand.h"

class MediaCommandSetProjectBPM : public EMCommand
{
public:
	MediaCommandSetProjectBPM();
	void ExecuteE() {};
	void* ExecuteE(void* p_upBPM, void*, void*);
	bool RequiresParameters();
	bool IsUndoable();
};

#endif
