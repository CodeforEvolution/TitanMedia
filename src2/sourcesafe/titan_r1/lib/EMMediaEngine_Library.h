/*******************************************************
* Creator: Richard Kronfält
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_MEDIA_ENGINE_LIBRARY
#define __EM_MEDIA_ENGINE_LIBRARY

/*
class EMMediaCommandFactory;
class EMMediaTimer;
class EMSettingsRepository;
class EMCommandRepository;

*/

#ifdef __EM_MEDIA_ENGINE_LIBRARY_EXPORT
#define EM_MEDIA_ENGINE_LIBRARY_API __declspec(dllexport)
#else
#define EM_MEDIA_ENGINE_LIBRARY_API __declspec(dllimport)
#endif

class EMSettingsRepository;
class EMCommandRepository;
class EMMediaCommandFactory;
class EMMediaTimer;

extern "C" EM_MEDIA_ENGINE_LIBRARY_API void InstanceMediaEngine();
extern "C" EM_MEDIA_ENGINE_LIBRARY_API bool MediaSystemCheck(char* p_vpErrorMessage);
extern "C" EM_MEDIA_ENGINE_LIBRARY_API void InitializeMediaEngine(/*EMSettingsRepository*/void*, EMCommandRepository*);
extern "C" EM_MEDIA_ENGINE_LIBRARY_API void DeleteMediaEngine();
extern "C" EM_MEDIA_ENGINE_LIBRARY_API EMMediaCommandFactory* InstanceMediaCommandFactory();
extern "C" EM_MEDIA_ENGINE_LIBRARY_API void DeleteMediaCommandFactory();
extern "C" EM_MEDIA_ENGINE_LIBRARY_API EMMediaTimer* InstanceMediaTimer();
extern "C" EM_MEDIA_ENGINE_LIBRARY_API void DeleteMediaTimer();

#endif




