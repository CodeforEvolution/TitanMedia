/*******************************************************
* This code is the copyrighted property of Elegant Media
*-------------------------------------------------------
* Creator: Johan Dunberg
* Component: Command
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/
#include "EMGlobals.h"

#ifndef __COMMAND_CHANGE_EXPLORER_LIST_VIEW_CONTROL_STYLE
#define __COMMAND_CHANGE_EXPLORER_LIST_VIEW_CONTROL_STYLE

#include "EMCommand.h"
#include "EMListViewControl.h"

class CommandChangeExplorerListViewControlStyle : public EMCommand
{
public:
	CommandChangeExplorerListViewControlStyle(EMListViewControl* p_opExplorerListViewControl, EMListViewStyle p_eListViewStyle);
	void ExecuteE();
	void* ExecuteE(void*, void*, void* ){return NULL;};
	bool IsUndoable();
	bool RequiresParameters();

private:
	EMListViewControl* m_opExplorerListViewControl;
	EMListViewStyle m_eListViewStyle;

};

#endif
