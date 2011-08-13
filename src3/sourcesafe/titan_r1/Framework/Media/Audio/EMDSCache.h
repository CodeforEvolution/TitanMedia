#ifndef __EM_DS_CACHE
#define __EM_DS_CACHE

#include "EMGlobals.h"

class EMDSCache
{
public:
	EMDSCache(int32 p_vSizeBytes);
	~EMDSCache();
	bool FlushNTS();
	bool ReadNTS(char* p_vpData, int32 p_vNumBytesToRead, int32* p_vpNumBytesActuallyRead);
	bool WriteNTS(const char* p_vpData, int32 p_vSize);
	int32 UsedNTS() const;
	int32 SizeNTS() const;

private:
	char* m_vpCache;
	int32 m_vCacheSize;
	int32 m_vUsed;
	char* m_vpWritePointer;
	char* m_vpReadPointer;
};

#endif