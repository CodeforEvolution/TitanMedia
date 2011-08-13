/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_PROJECT_COMMAND_FACTORY
#define __EM_PROJECT_COMMAND_FACTORY

class EMCommand;

class __declspec(dllexport) EMProjectCommandFactory
{
public:
	~EMProjectCommandFactory();
	static EMProjectCommandFactory* Instance();
	EMCommand* CreateCommand(uint32 p_vType);

protected:
	EMProjectCommandFactory();

private:
	static EMProjectCommandFactory* m_opInstance;
};

#endif


