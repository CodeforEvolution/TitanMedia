/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_EXCEPTION
#define __EM_EXCEPTION

#include <string>
enum EMExceptionType
{
//	EM_BAD_ALLOC,
	EM_DIALOG_ALWAYS,
	EM_DIALOG_SELECTABLE,
	EM_DIALOG_NEVER
};

class __declspec(dllexport) EMException
{
public:
	EMException(EMExceptionType p_eType, const string* p_opMessage);
	string* GetMessage();
	EMExceptionType GetType();

private:
	string m_oMessage;
	EMExceptionType m_eType;
};

#endif	
	