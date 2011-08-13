/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* Implements the Command design pattern
* Implements the Prototype design pattern
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_COMMAND
#define __EM_COMMAND

class __declspec(dllexport) EMCommand
{
public:
	virtual ~EMCommand();
//	virtual void Cleanup();
	virtual EMCommand* CloneAndClear();
	virtual void ExecuteE();
	virtual void* ExecuteE(void* p_opParameterOne, void* p_opParameterTwo, void* p_opParameterThree);
	virtual const char* GetShortDescription();
	virtual bool IsUndoable() = 0;
	virtual void RedoE();
	virtual bool RequiresParameters() = 0;
	virtual bool TriggerOnKeyRelease();
	virtual void UndoE();

protected:
	EMCommand();
};

#endif
