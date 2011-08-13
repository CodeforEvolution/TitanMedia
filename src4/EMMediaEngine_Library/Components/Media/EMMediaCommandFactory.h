/*******************************************************
* Creator: Richard Kronfält
* Portability: Non-Native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_MEDIA_COMMAND_FACTORY
#define __EM_MEDIA_COMMAND_FACTORY

#include "EMCommand.h"

#ifdef __EM_MEDIA_ENGINE_LIBRARY_IS_BUILDING
#define EM_MEDIA_COMMAND_FACTORY_DECLSPECIFIER __declspec(dllexport)
#else
#define EM_MEDIA_COMMAND_FACTORY_DECLSPECIFIER __declspec(dllimport)
#endif

class EM_MEDIA_COMMAND_FACTORY_DECLSPECIFIER EMMediaCommandFactory
{
public:
	static EMMediaCommandFactory* Instance();
	static void Delete();
	~EMMediaCommandFactory();
	EMCommand* CreateMediaCommand(uint32 p_vType);

private:
	EMMediaCommandFactory();
	static EMMediaCommandFactory* m_opInstance;

};

#endif