/*******************************************************
* Creator: Martin Johansson
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_INFO_COMMAND_GET_INPUT_SIGNAL_METERS
#define __MEDIA_INFO_COMMAND_GET_INPUT_SIGNAL_METERS

#include "EMCommand.h"

class MediaInfoCommandGetInputSignalMeters : public EMCommand
{
public:
	MediaInfoCommandGetInputSignalMeters();
	void ExecuteE() {};
	void* ExecuteE(void*, void*, void*);
	bool RequiresParameters();
	bool IsUndoable();

private:
	float m_vReturn[50*2];
};

#endif
