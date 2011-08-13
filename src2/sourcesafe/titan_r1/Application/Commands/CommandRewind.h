/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __COMMAND_REWIND
#define __COMMAND_REWIND

#include "EMCommand.h"
#include "EMThreadListener.h"

class TrackWindowToolBar;

class CommandRewind : public EMCommand, public EMThreadListener
{
public:
	CommandRewind(TrackWindowToolBar* p_opToolBar);
	void ExecuteE();
	void* ExecuteE(void*, void*, void* ) { return NULL; };
	bool IsUndoable();
	bool RequiresParameters();
	void ThreadRun(EMThread* p_opThread);
	bool TriggerOnKeyRelease();

private:
	TrackWindowToolBar* m_opToolBar;
	bool m_vKeyPressed;
};

#endif
