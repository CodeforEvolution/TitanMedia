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
//#include <map>

class EMCommand;

// Is the command equivalent of NULL
const uint32 COMMAND_ID_NULL = 0;
const uint32 MAX_NR_COMMANDS = 3000;

class __declspec(dllexport) EMCommandRepository
{
public:
	virtual ~EMCommandRepository();
	bool AddCommand(EMCommand* p_opCommand, uint32 p_vID);
	void ClearUndoStack();
	bool CommandExists(uint32 p_vCommandID);
	bool ExecuteCommand(uint32 p_vID);
	void* ExecuteCommand(uint32 p_vID, void* p_opParameterOne, void* p_opParameterTwo = NULL, void* p_opParameterThree = NULL);
	void* ExecuteCommandNoUndo(uint32 p_vID, void* p_opParameterOne, void* p_opParameterTwo = NULL, void* p_opParameterThree = NULL);
	bool ExecuteTriggeredCommand(uint32 p_vID);
	EMCommand* GetUndoClone(uint32 p_vID);
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
	void* ExecuteCommandBody(uint32 p_vID, void* p_opParameterOne, void* p_opParameterTwo, void* p_opParameterThree, bool p_vPreventUndo, bool p_vTriggeredByBinding);

private:
	EMCommand* m_opCommands[MAX_NR_COMMANDS];
	//map<uint32, EMCommand*>* m_opCommands;
	//EMCommand* m_opRedoCommand;
	list<EMCommand*> m_oReleaseKeyCommands;
	list<EMCommand*> m_oUndoStack;
	static EMCommandRepository* m_opInstance;
	bool m_vKeyReleased;
	bool m_vTriggeredByBinding;
};

#endif

