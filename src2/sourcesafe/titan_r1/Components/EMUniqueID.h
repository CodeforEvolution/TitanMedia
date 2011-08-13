/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_UINIQUE_ID
#define __EM_UINIQUE_ID

class EMUniqueID
{
public:
	static uint32 GetUniqueID();
	static const char* GetUniqueName();

private:
	static uint32 m_vUniqueID;
	static char m_vUniqueName[40];
};

#endif