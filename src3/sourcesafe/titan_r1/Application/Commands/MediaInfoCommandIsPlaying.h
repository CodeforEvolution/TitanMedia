/*******************************************************
* Creator: Richard Kronfält
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_INFO_COMMAND_IS_PLAYING
#define __MEDIA_INFO_COMMAND_IS_PLAYING

#include "EMCommand.h"

class MediaInfoCommandIsPlaying : public EMCommand
{
public:
	MediaInfoCommandIsPlaying();
	void ExecuteE() {};
	void* ExecuteE(void*, void*, void*);
	bool RequiresParameters();
	bool IsUndoable();

private:
	bool m_vReturnValue;

};

#endif
