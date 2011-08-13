/*******************************************************
* Creator: Martin Johansson
* Portability: Non-Native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_NETWORK_COMMAND_FACTORY
#define __EM_NETWORK_COMMAND_FACTORY

#include "EMCommand.h"

#ifdef __EM_NETWORK_ENGINE_LIBRARY_IS_BUILDING
#define EM_NETWORK_COMMAND_FACTORY_DECLSPECIFIER __declspec(dllexport)
#else
#define EM_NETWORK_COMMAND_FACTORY_DECLSPECIFIER __declspec(dllimport)
#endif

class EMNetworkCommandFactory;

extern "C" __declspec(dllexport) EMNetworkCommandFactory* InstanceNetworkCommandFactory();
extern "C" __declspec(dllexport) void DeleteNetworkCommandFactory();

class EM_NETWORK_COMMAND_FACTORY_DECLSPECIFIER EMNetworkCommandFactory
{
public:
	static EMNetworkCommandFactory* Instance();
	static void Delete();
	~EMNetworkCommandFactory();
	EMCommand* CreateNetworkCommand(uint32 p_vType);

private:
	EMNetworkCommandFactory();
	static EMNetworkCommandFactory* m_opInstance;

};

#endif