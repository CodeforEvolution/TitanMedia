/*******************************************************
* Creator: Richard Kronfält
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_COMMAND_SEEK
#define __MEDIA_COMMAND_SEEK

#include "EMCommand.h"

class MediaCommandSeek : public EMCommand
{
public:
	MediaCommandSeek();
	void ExecuteE() {};
	void* ExecuteE(void* p_opParameterOne, void* p_opParameterTwo, void* p_opParameterThree);
	bool RequiresParameters();
	bool IsUndoable();

private:

};

#endif
