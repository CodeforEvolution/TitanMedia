/*******************************************************
* Creator: Martin Johansson
* Portability: Native, very native
*-------------------------------------------------------
* 
* Martin Johansson, 01-08-01
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_NETWORK_FILE_ID
#define __EM_NETWORK_FILE_ID

class EMNetworkFileID
{
public:
	EMNetworkFileID(int64 p_vUID, int32 p_vID, bool p_vToMediaPool);
	~EMNetworkFileID();

	virtual void Clone(EMNetworkFileID* p_opId);

	uint32 GetId();
	uint64 GetUId();
	virtual const char* GetName();

	void SetName(const char* p_opName);

	bool ToMediaPool();

protected:
	char* m_vpName;

private:
	EMNetworkFileID();

	int32 m_vID;
	int64 m_vUID;

	bool m_vIsMediaPoolMaterial;
};

#endif
