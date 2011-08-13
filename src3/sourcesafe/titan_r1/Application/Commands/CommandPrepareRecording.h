/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __COMMAND_PREPARE_RECORDING
#define __COMMAND_PREPARE_RECORDING

#include "EMCommand.h"

class TrackWindowToolBar;

class CommandPrepareRecording : public EMCommand
{
public:
	CommandPrepareRecording(TrackWindowToolBar* p_opToolBar);
	void ExecuteE();
	void* ExecuteE(void*, void*, void* ) { return NULL; };
	bool IsUndoable();
	bool RequiresParameters();

private:
	TrackWindowToolBar* m_opToolBar;
};

#endif
