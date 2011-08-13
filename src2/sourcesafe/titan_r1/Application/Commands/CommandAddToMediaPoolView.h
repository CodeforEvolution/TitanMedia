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

#ifndef __COMMAND_ADD_TO_MEDIA_POOL_VIEW
#define __COMMAND_ADD_TO_MEDIA_POOL_VIEW

#include "EMCommand.h"
//#include "TransitionDialog.h"

class EMView;

class CommandAddToMediaPoolView : public EMCommand
{
public:
	CommandAddToMediaPoolView();
	void ExecuteE(){};
	void* ExecuteE(void* p_opParameterOne, void* p_opParameterTwo, void* p_opParameterThree);
	bool IsUndoable();
	bool RequiresParameters();

private:
	EMView* m_opMediaPoolView;
};

#endif
