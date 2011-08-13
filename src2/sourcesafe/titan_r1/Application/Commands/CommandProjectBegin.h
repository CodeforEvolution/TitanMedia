/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __COMMAND_PROJECT_BEGIN
#define __COMMAND_PROJECT_BEGIN

#include "EMCommand.h"

class TrackWindowToolBar;

class CommandProjectBegin : public EMCommand
{
public:
	CommandProjectBegin(TrackWindowToolBar* p_opToolBar);
	void ExecuteE();
	void* ExecuteE(void*, void*, void* ) { return NULL; };
	bool IsUndoable();
	bool RequiresParameters();

private:
	TrackWindowToolBar* m_opToolBar;
};

#endif
