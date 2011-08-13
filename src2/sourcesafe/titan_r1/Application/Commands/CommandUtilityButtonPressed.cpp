#include "CommandUtilityButtonPressed.h"

#include "EMSlimButton.h"
#include "UtilityView.h"

CommandUtilityButtonPressed::CommandUtilityButtonPressed(UtilityView* p_opUtilityView)
{
	m_opUtilityView = p_opUtilityView;
}

void* CommandUtilityButtonPressed::ExecuteE(void* p_upParameterOne, void* p_upParameterTwo, void* p_upParameterThree)
{
;//cout_commented_out_4_release << "CommandUtilityButtonPressed::ExecuteE" << endl;
	m_opUtilityView -> ButtonPressed(static_cast<EMSlimButton*>(p_upParameterTwo));
	return NULL;
}

bool CommandUtilityButtonPressed::IsUndoable()
{
	return false;
}

bool CommandUtilityButtonPressed::RequiresParameters()
{
	return true;
}


