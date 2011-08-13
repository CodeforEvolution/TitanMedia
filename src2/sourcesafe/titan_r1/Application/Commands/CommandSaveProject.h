/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __COMMAND_SAVE_PROJECT
#define __COMMAND_SAVE_PROJECT

#include "EMCommand.h"

class FileDialogTarget;
class TrackWindow;

class CommandSaveProject : public EMCommand
{
public:
	CommandSaveProject(FileDialogTarget* p_opFileDialogTarget, TrackWindow* p_opWindow);
	void ExecuteE();
	void* ExecuteE(void*, void*, void* ) { return NULL; };
	bool IsUndoable();
	bool RequiresParameters();

private:
	FileDialogTarget* m_opFileDialogTarget;	
	TrackWindow* m_opWindow;
};

#endif
