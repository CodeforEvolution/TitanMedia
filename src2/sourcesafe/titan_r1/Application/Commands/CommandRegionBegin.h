/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __COMMAND_REGION_BEGIN
#define __COMMAND_REGION_BEGIN

#include "EMCommand.h"

class TrackWindowToolBar;

class CommandRegionBegin : public EMCommand
{
public:
	CommandRegionBegin(TrackWindowToolBar* p_opToolBar);
	void ExecuteE();
	void* ExecuteE(void*, void*, void* ) { return NULL; };
	bool IsUndoable();
	bool RequiresParameters();

private:
	TrackWindowToolBar* m_opToolBar;
};

#endif
