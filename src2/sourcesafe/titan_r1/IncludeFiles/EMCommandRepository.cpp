#include "EMCommand.h"
#include "EMCommandRepository.h"

EMCommandRepository* EMCommandRepository::m_opInstance = NULL; // Static variable instantiation

EMCommandRepository::EMCommandRepository() :
m_opRedoCommand(NULL),
m_vKeyReleased(false)
{
}

EMCommandRepository::~EMCommandRepository()
{
}

bool EMCommandRepository::AddCommand(EMCommand* p_opCommand, uint32 p_vID)
{
	if(m_oCommands.find(p_vID) != m_oCommands.end())
		return false; // The ID already exists
		
	m_oCommands[p_vID] = p_opCommand;
	return true;
}

bool EMCommandRepository::CommandExists(uint32 p_vCommandID)
{
	if(m_oCommands.find(p_vCommandID) == m_oCommands.end())
		return false;
	return true;
}
/*
bool EMCommandRepository::ExecuteCommand(uint32 p_vID)
{
	return ExecuteCommand(p_vID, false);
}
*/
bool EMCommandRepository::ExecuteCommand(uint32 p_vID, bool p_vTriggeredByBinding)
{
	m_vTriggeredByBinding = p_vTriggeredByBinding;
	m_vKeyReleased = false;

	if(m_oCommands.find(p_vID) == m_oCommands.end())
	{
//		cout << "Failed to execute command with ID: " << p_vID << endl;
		Msg()->out("Failed to execute command with ID: %d", p_vID);
		EMDebugger("The command does not exist");
		return false; // The ID does not exist
	}

	if(m_oCommands[p_vID] -> RequiresParameters())
		EMDebugger("The command requested requires parameters");
		
//	if(p_vKeyReleased && !(m_oCommands[p_vID] -> TriggerOnKeyRelease()))
//		return false; // The command should not be executed on key up events
	
	try
	{
		m_oCommands[p_vID] -> ExecuteE();
	}
	catch(...)
	{
		// Send exception to EMExceptionHandler
	}
	if(m_oCommands[p_vID] -> TriggerOnKeyRelease())// && m_oReleaseKeyCommands.find(m_oCommands[p_vID]) == m_oReleaseKeyCommands.end())
	{
		m_oReleaseKeyCommands.push_back(m_oCommands[p_vID]);
		m_oReleaseKeyCommands.unique();
	}
	if(m_oCommands[p_vID] -> IsUndoable())
	{
		m_oUndoStack.push_back((m_oCommands[p_vID] -> CloneAndClear()));
	}
	return true;
}

void* EMCommandRepository::ExecuteCommand(uint32 p_vID, void* p_opParameterOne, void* p_opParameterTwo, void* p_opParameterThree)
{
//cout << "EMCommandRepository::ExecuteCommand: Trying to execute command with ID: " << (p_vID - MEDIA_COMMAND_BASE_OFFSET) << ", " << COMMAND_BASE_OFFSET << ", " << MEDIA_COMMAND_BASE_OFFSET << ", " << endl;
	if(m_oCommands.find(p_vID) == m_oCommands.end())
	{
		EMDebugger("The command does not exist");
		return NULL; // The ID does not exist
	}
//if(dynamic_cast<MediaInfoCommandGetOutputs*>(m_oCommands[p_vID]))
//cout << "This is a MediaInfoCommandGetOutputs object" << endl;
	if(!(m_oCommands[p_vID] -> RequiresParameters()))
	{
		EMDebugger("The command requested does not take any parameters");
		return NULL; // The ID does not exist
	}
		
	m_vTriggeredByBinding = false;
	m_vKeyReleased = false;
	
	void* opReturnValue=NULL;
	try
	{
		opReturnValue = m_oCommands[p_vID] -> ExecuteE(p_opParameterOne, p_opParameterTwo, p_opParameterThree);
	}
	catch(...)
	{
		// Send exception to EMExceptionHandler
	}

	if(m_oCommands[p_vID] -> IsUndoable())
		m_oUndoStack.push_back((m_oCommands[p_vID] -> CloneAndClear()));

	return opReturnValue;
}

list<EMCommand*>* EMCommandRepository::GetUndoStack()
{
	return &m_oUndoStack;
}

EMCommandRepository* EMCommandRepository::Instance()
{
	if(m_opInstance != NULL)
		return m_opInstance;

	m_opInstance = new EMCommandRepository();
	return m_opInstance;
}

bool EMCommandRepository::KeyReleased()
{
	return m_vKeyReleased;
}

void EMCommandRepository::Redo()
{
	if(m_opRedoCommand != NULL)
	{
		try
		{
			m_opRedoCommand -> RedoE();
		}
		catch(...)
		{
			// Send exception to EMExceptionHandler
		}
		delete m_opRedoCommand;
		m_opRedoCommand = NULL;
	}
}

bool EMCommandRepository::RequiresParameters(uint32 p_vID)
{
	return m_oCommands[p_vID] -> RequiresParameters();
}

void EMCommandRepository::SetKeyReleased()
{
	m_vKeyReleased = true;
	m_vTriggeredByBinding = true;

	list<EMCommand*>::iterator oIterator = m_oReleaseKeyCommands.begin();
	while(oIterator != m_oReleaseKeyCommands.end())
	{
		try
		{
			(*oIterator) -> ExecuteE();
		}
		catch(...)
		{
			// Send exception to EMExceptionHandler
		}
		oIterator++;
	}
	m_oReleaseKeyCommands.clear();
}

// Returns true if the last executed command (or currently executing
// command was/is triggered by a key or a MIDI message
bool EMCommandRepository::TriggeredByBinding()
{
	return m_vTriggeredByBinding;
}

void EMCommandRepository::Undo()
{
	EMCommand* opCommand = m_oUndoStack.back();
	m_oUndoStack.pop_back();
	try
	{
		opCommand -> UndoE();
	}
	catch(...)
	{
		// Send exception to EMExceptionHandler
	}
	if(m_opRedoCommand != NULL)
		delete m_opRedoCommand; // The redo stack only contains one item
	m_opRedoCommand = opCommand;
//	delete opCommand;
}



