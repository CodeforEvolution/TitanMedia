/*******************************************************
* Creator: Richard Kronfält
* Portability: Non-Native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_ID_MANAGER
#define __EM_ID_MANAGER

class __declspec(dllexport) EMMediaIDManager
{

public:
	~EMMediaIDManager();
	static int32 MakeID();
	static const char* MakeUniqueString();

private:
	EMMediaIDManager();
	static int32 m_vID;
	static string m_oDummyString;
};

#endif