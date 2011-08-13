/*******************************************************
* Creator: Martin Johansson
* Portability: Non-Native
*-------------------------------------------------------
*
* This class contains the destructive plugin 
*
*******************************************************/

#ifndef __EM_DESTRUCTIVE_DX_WRAPPER
#define __EM_DESTRUCTIVE_DX_WRAPPER

#include "EMGlobals.h"
#include "EMMediaGlobals.h"
#include "EMDestructivePlugin.h"

#include <list>


class EMDestructiveDXWrapper : public EMDestructivePlugin
{
public:
	EMDestructiveDXWrapper(EMPluginEntry* p_opDXPluginEntry);
	~EMDestructiveDXWrapper();

//	const char* Process(const char* p_opFile, int64 p_vStart, int64 p_vStop);

	EMPlugin* Clone();
	void* GetNativeEffect() const;

	int NeedExtraParams();
protected:
	bool DoPlugin(char* p_opDataSource, char* p_opDataDest, int64 p_vLen, int64* p_vpParams, int64 p_vStart, int64 p_vStop);
	
private:
	EMDestructiveDXWrapper();

private:
	EMPluginEntry* m_opDXEntry;

	static char* m_opTemp;
	static uint32 m_vLength;
};

#endif