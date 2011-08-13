/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* Manages and owns application resources
* Contains some native code
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_RESOURCE_REPOSITORY
#define __EM_RESOURCE_REPOSITORY

#include "EMResource.h"

#include <list>
#include <map>
#include <string>

class EMBitmap;

class EMResourceRepository
{
public:
	~EMResourceRepository();
	static string* GetString(uint32 p_vID);
	static list<string*>* GetStringList(uint32 p_vID);
	static EMBitmap* GetBitmap(uint32 p_vID);
	static bool LoadResources(string p_oPath);

private:
	static string FixString(string p_oString);

private:
	static map<uint32, EMResource*> m_oResources;
};

#endif