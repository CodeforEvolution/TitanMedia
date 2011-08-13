/*******************************************************
* Creator: Richard Kronfält
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_PROJECT_MANAGER_LIBRARY
#define __EM_PROJECT_MANAGER_LIBRARY

#ifdef EM_PROJECT_MANAGER_LIBRARY_API
#define EM_PROJECT_MANAGER_LIBRARY_API __declspec(dllexport)
#else
#define EM_PROJECT_MANAGER_LIBRARY_API __declspec(dllimport)
#endif

class EMProjectCommandFactory;

extern "C" EM_PROJECT_MANAGER_LIBRARY_API EMProjectCommandFactory* InstanceProjectCommandFactory();
extern "C" EM_PROJECT_MANAGER_LIBRARY_API void DeleteProjectCommandFactory();

#endif
