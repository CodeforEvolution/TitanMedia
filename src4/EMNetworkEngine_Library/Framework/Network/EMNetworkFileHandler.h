	/*******************************************************
* Creator: Martin Johansson
* Portability: Native, very native
*-------------------------------------------------------
* 
* Martin Johansson, 01-08-01
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_NETWORK_FILE_HANDLER
#define __EM_NETWORK_FILE_HANDLER

#include "EMItemContainer.h"
#include "EMNetworkFileInfo.h"
#include "EMNetworkRecvFileInfo.h"

struct TSonorkMsg;

class EMNetworkFileHandler : public EMItemContainer<EMNetworkRecvFileInfo>
{
public:
	EMNetworkFileHandler();
	~EMNetworkFileHandler();

	uint64 AddFile(TSonorkMsg* p_opMsg);

	EMNetworkRecvFileInfo* GetNextFile(EMFilePhase p_vStatus);
	void TimeSlot();

protected:
private:
};

#endif
