/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __COMMAND_UTILITY_BUTTON_PRESSED
#define __COMMAND_UTILITY_BUTTON_PRESSED

#include "EMCommand.h"

class UtilityView;

class CommandUtilityButtonPressed : public EMCommand
{
public:
	CommandUtilityButtonPressed(UtilityView* p_opUtilityView);
	void ExecuteE() {};
	void* ExecuteE(void* p_upParameterOne, void* p_upParameterTwo, void* p_upParameterThree);
	bool IsUndoable();
	bool RequiresParameters();

private:
	UtilityView* m_opUtilityView;
};

#endif
