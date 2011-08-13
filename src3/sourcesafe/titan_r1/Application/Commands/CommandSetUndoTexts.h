/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __COMMAND_SET_UNDO_TEXTS
#define __COMMAND_SET_UNDO_TEXTS

#include "EMCommand.h"

class EMMenuBar;
class EMMenuItem;

class CommandSetUndoTexts : public EMCommand
{
public:
	CommandSetUndoTexts(EMMenuBar* p_opMenuBar);
	void ExecuteE() {};
	void* ExecuteE(void* p_upParameterOne, void* p_upParameterTwo, void* p_upParameterThree);
	bool IsUndoable();
	bool RequiresParameters();

private:
	EMMenuBar* m_opMenuBar;
	EMMenuItem* m_opMenuItem;
};

#endif

