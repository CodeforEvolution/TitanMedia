/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_COMMAND_REPOSITORY
#define __EM_COMMAND_REPOSITORY

#include <list>
#include <map>

//#include "MediaInfoCommandGetOutputs.h" // Debug

class EMCommand;

// Is the command equivalent of NULL
const uint32 COMMAND_ID_NULL = 0;
// All commands have an offset (since they sometimes are sent as messages)
#define COMMAND_BASE_OFFSET 0x40000000
#define MEDIA_COMMAND_BASE_OFFSET 0x80000000

class EMCommandRepository
{
public:
	virtual ~EMCommandRepository();
	bool AddCommand(EMCommand* p_opCommand, uint32 p_vID);
	bool CommandExists(uint32 p_vCommandID);
//	bool ExecuteCommand(uint32 p_vID);
	bool ExecuteCommand(uint32 p_vID, bool p_vTriggeredByBinding = false);
	void* ExecuteCommand(uint32 p_vID, void* p_opParameterOne, void* p_opParameterTwo=NULL, void* p_opParameterThree=NULL);
	list<EMCommand*>* GetUndoStack();
	static EMCommandRepository* Instance();
	bool KeyReleased();
	void Redo();
	bool RequiresParameters(uint32 p_vID);
	void SetKeyReleased();
	bool TriggeredByBinding();
	void Undo();
	
protected:
	EMCommandRepository();

private:
	map<uint32, EMCommand*> m_oCommands;
	EMCommand* m_opRedoCommand;
	list<EMCommand*> m_oReleaseKeyCommands;
	list<EMCommand*> m_oUndoStack;
	static EMCommandRepository* m_opInstance;
	bool m_vKeyReleased;
	bool m_vTriggeredByBinding;
};

#endif

