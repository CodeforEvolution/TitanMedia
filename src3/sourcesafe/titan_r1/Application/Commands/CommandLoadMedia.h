/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __COMMAND_LOAD_MEDIA
#define __COMMAND_LOAD_MEDIA

#include "EMCommand.h"

class EMFileWindow;
class FileDialogTarget;

class CommandLoadMedia : public EMCommand
{
public:
	CommandLoadMedia(FileDialogTarget* p_opFileDialogTarget);
	void ExecuteE();
	void* ExecuteE(void*, void*, void* ) { return NULL; };
	bool IsUndoable();
	bool RequiresParameters();

private:
	FileDialogTarget* m_opFileDialogTarget;
	EMFileWindow* m_opFileWindow;	
};

#endif
