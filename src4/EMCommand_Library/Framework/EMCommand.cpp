#include "EMCommand.h"

EMCommand::EMCommand()
{
}

EMCommand::~EMCommand()
{
}

// Commands that has undo data stored in their objects should
// delete it when this method is called. This is to avoid the
// undo data to be stored both in the undo stack and in the
// list of commands in the command repository
/*
void EMCommand::Cleanup()
{
}
*/

// This method does not have to be implemented if the command
// cannot be undoed
EMCommand* EMCommand::CloneAndClear()
{
	if(IsUndoable())
		EMDebugger("The UndoE() method must be implemented in this command since it is undoable");
	return NULL;
}

void EMCommand::ExecuteE()
{
	if(!RequiresParameters())
		EMDebugger("ExecuteE() must be implemented in this command since it requires no parameters");
}

// If the command does not require any parameters, this method is used
// (and does nothing)
void* EMCommand::ExecuteE(void* p_opParameterOne, void* p_opParameterTwo, void* p_opParameterThree)
{
	if(RequiresParameters())
		EMDebugger("ExecuteE(void*, void*, void*) must be implemented in this command since it requires parameters");
	return NULL;
}

const char* EMCommand::GetShortDescription()
{
	return NULL;
}

/*
// Not undoable by default
bool EMCommand::IsUndoable()
{
	return false;
}
*/
void EMCommand::RedoE()
{
	if(IsUndoable())
		EMDebugger("The RedoE() method must be implemented in this command since it is undoable");
}
/*
bool EMCommand::RequiresParameters()
{
	return false;
}
*/

// If this command should be triggered on both key up and key down events
bool EMCommand::TriggerOnKeyRelease()
{
	return false;
}

void EMCommand::UndoE()
{
	if(IsUndoable())
		EMDebugger("The UndoE() method must be implemented in this command since it is undoable");
}
