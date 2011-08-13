/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __COMMAND_NEW_PROJECT
#define __COMMAND_NEW_PROJECT

#include "EMCommand.h"

class CommandNewProject : public EMCommand
{
public:
	CommandNewProject();
	void ExecuteE(){};
	void* ExecuteE(void*, void*, void* );
	bool IsUndoable();
	bool RequiresParameters();

private:
	bool m_vContinue;
};

#endif
