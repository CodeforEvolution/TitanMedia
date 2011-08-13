/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __COMMAND_TOGGLE_VIEW_CLIP_CONTENT
#define __COMMAND_TOGGLE_VIEW_CLIP_CONTENT

#include "EMCommand.h"

class EMMenuBar;

class CommandToggleViewClipContent : public EMCommand
{
public:
	CommandToggleViewClipContent(EMMenuBar* p_opMenuBar);
	void ExecuteE();
	bool IsUndoable();
	bool RequiresParameters();

private:
	EMMenuBar* m_opMenuBar;
};

#endif
