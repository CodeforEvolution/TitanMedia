#include "EMDSCache.h"

EMDSCache::EMDSCache(int32 p_vSizeBytes)
	:	m_vpCache(NULL),
		m_vCacheSize(p_vSizeBytes),
		m_vUsed(0),
		m_vpWritePointer(NULL),
		m_vpReadPointer(NULL)
{
	m_vpCache = EM_new char[p_vSizeBytes];
	memset(m_vpCache, 0, p_vSizeBytes);
	m_vpWritePointer = m_vpCache;
	m_vpReadPointer = NULL;
}

EMDSCache::~EMDSCache() 
{
	if(m_vpCache != NULL)
	{
		delete m_vpCache;
		m_vpCache = NULL;
	}
}

bool EMDSCache::FlushNTS()
{
	//eo << "Now flushing cache!" << ef;
	memset(m_vpCache, 0, m_vCacheSize);
	m_vpReadPointer = m_vpWritePointer = m_vpCache;
	m_vUsed = 0;
	return true;
}

bool EMDSCache::ReadNTS(char* p_vpData, int32 p_vNumBytesToRead, int32* p_vpNumBytesActuallyRead)
{
	int32 vSilence = 0;

	//If the cache level is above 5%, and we're told to read more than 90% of the total number of bytes,
	//then we should only read 90% (so we leave some data in the cache for the next read attempt).
//	if(p_vNumBytesToRead > (m_vUsed * 0.9) && m_vUsed > (0.05 * m_vCacheSize))
//	{
//		eo << "Adjusted read amount down, to 90% of the used data in the cache, ie from " << p_vNumBytesToRead << " to " << (m_vUsed * 0.9) << " byte(s))" << ef;
//		p_vNumBytesToRead = (m_vUsed * 0.9);
//	}

	int32 vLeftToRead = p_vNumBytesToRead;

	(*p_vpNumBytesActuallyRead) = 0;

	int64 vUsedBefore = m_vUsed;

	if(m_vUsed < p_vNumBytesToRead)
		vSilence = p_vNumBytesToRead - m_vUsed;

	vLeftToRead -= vSilence;
	while(vLeftToRead > 0 && m_vUsed > 0)
	{
		int32 vToRead = 0;
		if(m_vpReadPointer >= m_vpWritePointer)
			vToRead = m_vCacheSize - static_cast<int32>(m_vpReadPointer - m_vpCache);
		else
			vToRead = static_cast<int32>(m_vpWritePointer - m_vpReadPointer);
		
		if(vToRead > vLeftToRead)
			vToRead = vLeftToRead;

		memcpy(p_vpData + ((p_vNumBytesToRead - vSilence) - vLeftToRead), m_vpReadPointer, vToRead);

		(*p_vpNumBytesActuallyRead) += vToRead;

		memset(m_vpReadPointer, 0, vToRead);
		int64 vOld = (m_vpReadPointer - m_vpCache);
		m_vpReadPointer = m_vpCache + ((static_cast<int32>(m_vpReadPointer - m_vpCache) + vToRead) % m_vCacheSize);
		m_vUsed -= vToRead;
		vLeftToRead -= vToRead;
	}
	
	if((vSilence + vLeftToRead) > 0)
		memset(p_vpData + (p_vNumBytesToRead - vSilence), 0, vSilence);

	//eo << "Cache delivered " << (int) (*p_vpNumBytesActuallyRead) << " byte(s) (Used before was " << (int) vUsedBefore << " and now is " << (int) m_vUsed << " byte(s))" << ef;

	return true;
}

bool EMDSCache::WriteNTS(const char* p_vpData, int32 p_vSize)
{
	int32 vLeftToWrite = p_vSize;
	int32 vFreeSpace = 0;

	while(vLeftToWrite > 0)
	{
		int32 vToWrite = 0;
		if(m_vpWritePointer > m_vpReadPointer)
			vToWrite = m_vCacheSize - static_cast<int32>(m_vpWritePointer - m_vpCache);
		else
			vToWrite = (static_cast<int32>(m_vpReadPointer - m_vpWritePointer) == 0 ? m_vCacheSize - m_vUsed : static_cast<int32>(m_vpReadPointer - m_vpWritePointer));

		if(vToWrite <= 0)
		{
			//eo << "ERROR! vToWrite == 0 in EMDSCache::WriteNTS(...)" << ef;
			return false;
		}
	
		if(vToWrite > vLeftToWrite)
			vToWrite = vLeftToWrite;

		memcpy(m_vpWritePointer, p_vpData + (p_vSize - vLeftToWrite), vToWrite);
		m_vpWritePointer = m_vpCache + ((static_cast<int32>(m_vpWritePointer - m_vpCache) + vToWrite) % m_vCacheSize);

		m_vUsed += vToWrite;
		vLeftToWrite -= vToWrite;
		if(m_vpReadPointer == NULL)
			m_vpReadPointer = m_vpCache;
	} 
	return true;
}

int32 EMDSCache::UsedNTS() const
{
	return m_vUsed;
}

int32 EMDSCache::SizeNTS() const
{
	return m_vCacheSize;
}
